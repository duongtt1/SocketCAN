// CanSocket.cpp
#include "CanSocket.h"
#include <iostream>
#include <iomanip>

CanSocket::CanSocket(const char *interfaceName) {
    // Create CAN socket
    socketDescriptor = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (socketDescriptor < 0) {
        perror("Socket");
        throw std::runtime_error("Failed to create CAN socket");
    }

    // Set socket interface index
    std::strncpy(ifr.ifr_name, interfaceName, IFNAMSIZ);
    ioctl(socketDescriptor, SIOCGIFINDEX, &ifr);

    // Bind CAN socket
    std::memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(socketDescriptor, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) < 0) {
        perror("Bind");
        throw std::runtime_error("Failed to bind CAN socket");
    }

    // Enable CAN FD support
    int enable_canfd = 1;
    if (setsockopt(socketDescriptor, SOL_CAN_RAW, CAN_RAW_FD_FRAMES, &enable_canfd, sizeof(enable_canfd)) < 0) {
        perror("setsockopt");
        throw std::runtime_error("Failed to enable CAN FD support");
    }

    // Initialize cache and thread
    cacheMem = std::make_shared<ReceivedFrameCache>();
    tCache.setCacheMemory(cacheMem);
}

CanSocket::~CanSocket() {
    // Stop threads and close socket
    stopRecvThread();
    stopSendThread();
    close(socketDescriptor);
}

void CanSocket::setFilter(const canid_t *canIds, size_t numIds) {
    // Set CAN filter
    if (numIds == 0) {
        throw std::invalid_argument("Empty array of CAN IDs");
    }

    std::vector<struct can_filter> filter(numIds);
    for (size_t i = 0; i < numIds; ++i) {
        filter[i].can_id = canIds[i];
        filter[i].can_mask = CAN_SFF_MASK; // Use standard identifier mask
    }

    if (setsockopt(socketDescriptor, SOL_CAN_RAW, CAN_RAW_FILTER, filter.data(), sizeof(struct can_filter) * numIds) < 0) {
        perror("setsockopt");
        throw std::runtime_error("Failed to set CAN filter");
    }
}

bool CanSocket::sendCanMsgByID(const canid_t id) {
    // Send CAN message by ID
    auto dbTx = CanDB::getInstance().getTxMessages();
    auto it = dbTx.find(id);

    if (it != dbTx.end()) {
        CanFrame frame = it->second->pack();

        // Check if socketDescriptor is valid
        if (socketDescriptor < 0) {
            std::cerr << "Invalid socket descriptor" << std::endl;
            return false;
        }

        // Write to CAN socket
        if (write(socketDescriptor, &frame.getFrame(), CANFD_MTU) != CANFD_MTU) {
            perror("Write");
            std::cerr << "Error writing to CAN socket. Errno: " << errno << std::endl;
            throw std::runtime_error("Failed to write to the CAN socket");
        }

        // Update last activation time
        it->second->setLastActivated(std::chrono::steady_clock::now());
        return true; // Successfully sent
    }

    return false;
}

bool CanSocket::sendCandPeriodly() {
    // Send CAN messages periodically
    std::chrono::steady_clock::time_point now;
    auto dbTx = CanDB::getInstance().getTxMessages();

    while (runSend.load()) {
        now = std::chrono::steady_clock::now();
        for (auto it = dbTx.begin(); it != dbTx.end(); ++it) {
            auto delay = now - it->second->getLastActivated();
            auto periodTimeMsg = std::chrono::duration<uint16_t>(it->second->getPeriodTime());

            if (delay > std::chrono::microseconds(periodTimeMsg.count())) {
                sendCanMsgByID(it->first);
            } else {
                // Do nothing
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return false;
}

bool CanSocket::receiveCanMsg() {
    // Receive CAN messages
    struct can_frame receivedFrame;
    tCache.start();
    while (runRecv.load()) {
        ssize_t bytesRead = read(socketDescriptor, &receivedFrame, sizeof(can_frame));

        if (bytesRead < 0) {
            perror("Read");
            throw std::runtime_error("Failed to read from the CAN socket");
            return false;
        } else if (bytesRead == sizeof(can_frame)) {
            // Add received frame to cache
            cacheMem->addFrame(receivedFrame);
            std::memset(&receivedFrame, 0, sizeof(receivedFrame));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return false;
}

void CanSocket::startSendThread() {
    sendThread = std::thread(&CanSocket::sendCandPeriodly, this);
}

void CanSocket::startRecvThread() {
    recvThread = std::thread(&CanSocket::receiveCanMsg, this);
}

void CanSocket::stopSendThread() {
    runSend.store(false);
    if (sendThread.joinable()) {
        sendThread.join();
    }
}

void CanSocket::stopRecvThread() {
    runRecv.store(false);
    if (recvThread.joinable()) {
        recvThread.join();
    }
}
