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
#include <atomic>
#include "Common/SafeQueue.h"
#include "CacheRecv.h"
class CanSocket
{
public:
    CanSocket(const char *interfaceName);
    ~CanSocket();
    void setFilter(const canid_t *canIds, size_t numIds);
    bool sendCanMsgByID(const canid_t id);
    bool sendCandPeriodly();
    bool receiveCanMsg();
    void startRecv();
    void stopRecv();

private:
    int socketDescriptor;
    struct sockaddr_can addr;
    struct ifreq ifr;
    std::atomic<bool> runSend;
    std::atomic<bool> runRecv;
    std::shared_ptr<CacheRecv> cacheMem;
    CacheThread tCache;
};

#endif // CAN_SOCKET_H
