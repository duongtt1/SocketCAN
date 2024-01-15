// CanSocket.cpp
#include "CanSocket.h"
#include <iostream>

CanSocket CanSocket::getInstance(const char *interfaceName)
{
    CanSocket instance(interfaceName);
    return instance;
}

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
}

CanSocket::~CanSocket()
{
    close(socketDescriptor);
}

void CanSocket::sendCanFrame(const CanFrame &canFrame)
{
    if (write(socketDescriptor, &canFrame.getFrame(), sizeof(can_frame)) != sizeof(can_frame))
    {
        perror("Write");
        throw std::runtime_error("Failed to write to the CAN socket");
    }
}

CanFrame CanSocket::receiveCanFrame()
{
    struct can_frame frame;
    ssize_t bytesRead = read(socketDescriptor, &frame, sizeof(can_frame));

    if (bytesRead == sizeof(can_frame))
    {
        return CanFrame(frame.can_id, reinterpret_cast<int8_t *>(frame.data), frame.can_dlc);
    }
    else
    {
        perror("Read");
        throw std::runtime_error("Failed to read from the CAN socket");
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