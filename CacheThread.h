#ifndef CACHE_THREAD_H
#define CACHE_THREAD_H

#include <iostream>
#include <memory>
#include <atomic>
#include <thread>
#include "Common/SafeQueue.h"
#include "linux/can.h"
#include "CanDB.h"
#include "Common/Common.h"

/**
 * @file CacheThread.h
 * @brief Class for managing a thread that handles a cache of received frames.
 */

/**
 * @class CacheThread
 * @brief Provides functionality to start and stop a thread that manages a cache of received frames.
 */
class CacheThread {
private:
    std::atomic<bool> runThread; /**< Atomic flag to control the execution of the thread. */
    std::shared_ptr<ReceivedFrameCache> receivedFrameCache; /**< Shared pointer to the received frame cache. */
    std::thread workerThread; /**< Thread responsible for handling the cache. */

public:
    /**
     * @brief Constructor for the CacheThread class.
     */
    CacheThread();

    /**
     * @brief Destructor for the CacheThread class.
     */
    ~CacheThread();

    /**
     * @brief Start the cache thread.
     * 
     * Initiates the thread that manages the cache.
     */
    void start();

    /**
     * @brief Stop the cache thread.
     * 
     * Stops the execution of the cache thread.
     */
    void stop();

    /**
     * @brief Set the cache memory for the thread.
     * 
     * @param cache A shared pointer to the ReceivedFrameCache object.
     * @return True if the cache memory is set successfully, false otherwise.
     */
    bool setCacheMemory(std::shared_ptr<ReceivedFrameCache> cache);

private:
    /**
     * @brief Thread function for handling the cache.
     * 
     * This function is called when the thread is started and is responsible for managing the received frame cache.
     */
    void threadFunction();
};

#endif // CACHE_THREAD_H
