#ifndef CACHE_RECV_H
#define CACHE_RECV_H

#include <iostream>
#include "Common/SafeQueue.h"
#include "linux/can.h"
#include "CanDB.h"
#include <atomic>
#include <thread>

#define MAX_SIZE_CACHE 50

class CacheRecv {
private:
    SafeQueue<can_frame> cache;

public:
    CacheRecv() : cache(MAX_SIZE_CACHE) {}
    ~CacheRecv() {}

    bool addFrame(const can_frame frame) {
        if (cache.fully()) {
            return false;
        }
        cache.push(frame);
        return true;
    }

    bool popFrame(can_frame& frame) {
        if (cache.empty()) {
            return false;
        }
        frame = cache.pop();
        return true;
    }

    bool empty() {
        return cache.empty();
    }
};

class CacheThread {
private:
    std::atomic<bool> runThread;
    std::shared_ptr<CacheRecv> cacheRecv;
    std::thread workerThread;

public:
    // Constructor
    CacheThread();

    // Destructor
    ~CacheThread();

    // Start the thread
    void start();

    // Stop the thread
    void stop();

    bool setCacheMemory(std::shared_ptr<CacheRecv> cache);

private:
    // Thread function
    void threadFunction();
};


#endif // CACHE_RECV_H
