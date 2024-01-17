// CanSocket.cpp
#include "CanSocket.h"
#include <iostream>

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

    // Set socket option for CAN FD
    int enable_canfd = 1;
    setsockopt(socketDescriptor, SOL_CAN_RAW, CAN_RAW_FD_FRAMES, &enable_canfd, sizeof(enable_canfd));

    if (bind(socketDescriptor, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) < 0)
    {
        perror("Bind");
        throw std::runtime_error("Failed to bind CAN socket");
    }

    // Start the receive thread with a default interval of 100ms
    receiveThread = std::thread(&CanSocket::receiveLoop, this, std::chrono::milliseconds(100));
}

CanSocket::~CanSocket()
{
    stopThreads = true;
    if (receiveThread.joinable())
        receiveThread.join();
    if (sendThread.joinable())
        sendThread.join();

    close(socketDescriptor);
}

void CanSocket::sendCanFrame(const CanFrame &canFrame)
{
    // if (write(socketDescriptor, &canFrame.getData(), canFrame.getLength()) != canFrame.getLength())
    // {
    //     perror("Write");
    //     throw std::runtime_error("Failed to write to the CAN socket");
    // }
}

void CanSocket::receiveCanFramesPeriodically(std::chrono::milliseconds interval)
{
    // Start the receive thread with the specified interval
    receiveThread = std::thread(&CanSocket::receiveLoop, this, interval);
}

void CanSocket::receiveLoop(std::chrono::milliseconds interval)
{
    while (!stopThreads)
    {
        // CanFrame receivedFrame = receiveCanFrame();

        // // Store the received frame in CanDB for other threads to read
        // CanDB::getInstance().setCanMsgRx(receivedFrame.getCanId(), std::make_shared<ICAN_MSG>(receivedFrame));

        // std::this_thread::sleep_for(interval);
    }
}

void CanSocket::sendCanFramesPeriodically(std::chrono::milliseconds interval)
{
    // Start the send thread with the specified interval
    sendThread = std::thread(&CanSocket::sendLoop, this, interval); 
}

void CanSocket::sendLoop(std::chrono::milliseconds interval)
{
    while (!stopThreads)
    {
        // Retrieve a CanFrame from CanDB for sending
        // std::shared_ptr<ICAN_MSG> msg = CanDB::getInstance().getCanMsgTx(/* specify CAN ID */);
        // if (msg)
        // {
        //     CanFrame frame = msg->pack();
        //     sendCanFrame(frame);
        // }

        std::this_thread::sleep_for(interval);
    }
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
