// CanSocket.h
#ifndef CAN_SOCKET_H
#define CAN_SOCKET_H

#include "Common/Common.h"
#include "CanDB.h"
#include <stdexcept>
#include <cstring>
#include <vector>
#include <sys/ioctl.h>
#include <net/if.h>
#include <thread>
#include <chrono>
#include <atomic>
#include "Common/SafeQueue.h"
#include "CacheThread.h"

/**
 * @brief Class representing a CAN socket for sending and receiving CAN messages.
 */
class CanSocket {
public:
    /**
     * @brief Constructor.
     * 
     * @param interfaceName Name of the CAN interface.
     */
    CanSocket(const char *interfaceName);

    /**
     * @brief Destructor.
     */
    ~CanSocket();

    /**
     * @brief Sets a filter for receiving CAN messages.
     * 
     * @param canIds Array of CAN identifiers.
     * @param numIds Number of CAN identifiers in the array.
     */
    void setFilter(const canid_t *canIds, size_t numIds);

    /**
     * @brief Sends a CAN message by ID.
     * 
     * @param id CAN identifier.
     * @return True if the message is sent successfully, false otherwise.
     */
    bool sendCanMsgByID(const canid_t id);

    /**
     * @brief Sends CAN messages periodically based on configured periods.
     * 
     * @return False (currently not used).
     */
    bool sendCandPeriodly();

    /**
     * @brief Receives CAN messages and adds them to the cache.
     * 
     * @return False (currently not used).
     */
    bool receiveCanMsg();

    /**
     * @brief Starts the sending thread.
     */
    void startSendThread();

    /**
     * @brief Starts the receiving thread.
     */
    void startRecvThread();

    /**
     * @brief Stops the sending thread.
     */
    void stopSendThread();

    /**
     * @brief Stops the receiving thread.
     */
    void stopRecvThread();

private:
    int socketDescriptor; /**< Socket descriptor for the CAN socket. */
    struct sockaddr_can addr; /**< CAN socket address. */
    struct ifreq ifr; /**< Interface request structure. */
    std::shared_ptr<ReceivedFrameCache> cacheMem; /**< Shared pointer to the received frame cache. */
    CacheThread tCache; /**< Cache thread for processing received frames. */
    std::thread sendThread; /**< Thread for sending CAN messages periodically. */
    std::thread recvThread; /**< Thread for receiving CAN messages. */
    std::atomic<bool> runSend{true}; /**< Atomic flag for the sending thread. */
    std::atomic<bool> runRecv{true}; /**< Atomic flag for the receiving thread. */
};

#endif // CAN_SOCKET_H
