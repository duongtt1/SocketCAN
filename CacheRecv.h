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
        cache.push(frame);
        return true;
    }

    bool popFrame(can_frame& frame) {
        frame = cache.pop();
        return true;
    }

    bool empty(){
        return cache.empty();
    }
    
};

class CacheThread {
private:
    std::atomic<bool>& runThread;
    CacheRecv& cacheRecv;
    CanDB& canDB;

public:
    CacheThread(std::atomic<bool>& run, CacheRecv& cache, CanDB& db)
        : runThread(run), cacheRecv(cache), canDB(db) {}

    void operator()() {
        can_frame frame;
        while (runThread.load() || !cacheRecv.empty()) {
            if (cacheRecv.popFrame(frame)) {
                CanDB::getInstance().getCanMsgRx(frame.can_id)->unpack(frame.data);
            }
            else {
                std::this_thread::yield();
            }
        }
    }
};


