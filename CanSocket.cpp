// CanSocket.cpp
#include "CanSocket.h"
#include <iostream>
#include "CacheRecv.h"
#include <iomanip>



CanSocket::CanSocket(const char *interfaceName)
{
    socketDescriptor = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (socketDescriptor < 0)
    {
        perror("Socket");
        throw std::runtime_error("Failed to create CAN socket");
    }

    std::strncpy(ifr.ifr_name, interfaceName, IFNAMSIZ);
    ioctl(socketDescriptor, SIOCGIFINDEX, &ifr);

    std::memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(socketDescriptor, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) < 0)
    {
        perror("Bind");
        throw std::runtime_error("Failed to bind CAN socket");
    }

    int enable_canfd = 1; // Enable CAN FD support

    // Set socket options to enable CAN FD
    if (setsockopt(socketDescriptor, SOL_CAN_RAW, CAN_RAW_FD_FRAMES, &enable_canfd, sizeof(enable_canfd)) < 0) {
        perror("setsockopt");
        throw std::runtime_error("Failed to enable CAN FD support");
    }

    cacheMem = std::make_shared<CacheRecv>();
    this->tCache.setCacheMemory(cacheMem);
    
}

CanSocket::~CanSocket()
{
    close(socketDescriptor);
}

void CanSocket::setFilter(const canid_t *canIds, size_t numIds)
{
    if (numIds == 0)
    {
        throw std::invalid_argument("Empty array of CAN IDs");
    }

    std::vector<struct can_filter> filter(numIds);
    for (size_t i = 0; i < numIds; ++i)
    {
        filter[i].can_id = canIds[i];
        filter[i].can_mask = CAN_SFF_MASK; // Use standard identifier mask
    }

    if (setsockopt(socketDescriptor, SOL_CAN_RAW, CAN_RAW_FILTER, filter.data(), sizeof(struct can_filter) * numIds) < 0)
    {
        perror("setsockopt");
        throw std::runtime_error("Failed to set CAN filter");
    }
}

bool CanSocket::sendCanMsgByID(const canid_t id)
{
    auto dbRx = CanDB::getInstance().getDbTX(); // Assuming dbRx is a reference
    auto it = dbRx.find(id);
    if (it != dbRx.end()) {
        CanFrame frame = it->second->pack(); // Convert ICAN_MSG to CanFrame
        if (write(socketDescriptor, &frame.getFrame(), sizeof(can_frame)) != sizeof(can_frame))
        {
            perror("Write");
            throw std::runtime_error("Failed to write to the CAN socket");
        }
        it->second->setLastActivated(std::chrono::steady_clock::now());
        return true; // Successfully sent
    }
    return false;
}

bool CanSocket::sendCandPeriodly()
{
    std::chrono::steady_clock::time_point now;
    auto dbRx = CanDB::getInstance().getDbTX();
    while (runSend.load())
    {
        now = steady_clock::now();
        for (auto it = dbRx.begin(); it != dbRx.end(); ++it) { 
            auto delay = now - it->second->getLastActivated();
            auto periodTimeMsg = std::chrono::duration<uint16_t>(it->second->getPeriodTime());
            if ( delay > periodTimeMsg){
                sendCanMsgByID(it->first);
            }
        } 
    }
    return false;
}

bool CanSocket::receiveCanMsg()
{
    struct can_frame receivedFrame;
    tCache.start();
    while(runRecv.load())
    {
        ssize_t bytesRead = read(socketDescriptor, &receivedFrame, sizeof(can_frame));
        
        if (bytesRead < 0)
        {
            perror("Read");
            throw std::runtime_error("Failed to read from the CAN socket");
            return false;
        }
        else if (bytesRead == sizeof(can_frame))
        {
            cacheMem->addFrame(receivedFrame);
            // displayCanFrame(receivedFrame);
            memset(&receivedFrame, 0, sizeof(receivedFrame));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return false;
}

void CanSocket::startRecv(){
    runRecv.store(true);
}

void CanSocket::stopRecv(){
    runRecv.store(false);
}