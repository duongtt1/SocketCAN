// CanSocket.h
#ifndef CAN_SOCKET_H
#define CAN_SOCKET_H

#include "Common/Common.h"
#include "CanDB.h" // Include CanDB header
#include <stdexcept>
#include <cstring>
#include <vector>
#include <sys/ioctl.h>
#include <net/if.h>
#include <thread>
#include <chrono>

class CanSocket
{
public:
    CanSocket(const char *interfaceName);
    ~CanSocket();

    void sendCanFrame(const CanFrame &canFrame);
    void receiveCanFramesPeriodically(std::chrono::milliseconds interval);
    void receiveCanFramesOnEvent(); // Implement this based on your event mechanism
    void sendCanFramesPeriodically(std::chrono::milliseconds interval);

    void setFilter(const canid_t *canIds, size_t numIds);

private:
    int socketDescriptor;
    struct sockaddr_can addr;
    struct ifreq ifr;

    std::thread receiveThread;
    std::thread sendThread;
    bool stopThreads = false;

    void receiveLoop(std::chrono::milliseconds interval);
    void sendLoop(std::chrono::milliseconds interval);
};

#endif // CAN_SOCKET_H
