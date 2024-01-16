// CanSocket.h
#ifndef CAN_SOCKET_H
#define CAN_SOCKET_H

#include "Common/Common.h"
#include <stdexcept>
#include <cstring>
#include <vector>
#include <sys/ioctl.h>
#include <net/if.h>

class CanSocket
{
public:
    ~CanSocket();

    void sendCanFrame(const CanFrame &canFrame);

    CanFrame receiveCanFrame();

    void setFilter(const canid_t *canIds, size_t numIds);
    
    CanSocket(const char *interfaceName);

private:
    int socketDescriptor;
    struct sockaddr_can addr;
    struct ifreq ifr;
};

#endif // CAN_SOCKET_H