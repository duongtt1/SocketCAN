#include "CacheThread.h"

// Constructor definition
CacheThread::CacheThread() : runThread(false) {}

// Destructor definition
CacheThread::~CacheThread() {
    stop();
}

// Start the thread
void CacheThread::start() {
    if (!runThread.load()) {
        runThread.store(true);
        workerThread = std::thread(&CacheThread::threadFunction, this);
    }
}

// Stop the thread
void CacheThread::stop() {
    if (runThread.load()) {
        runThread.store(false);
        if (workerThread.joinable()) {
            workerThread.join();
        }
    }
}

// Thread function
void CacheThread::threadFunction() {
    can_frame frame;
    while (runThread.load() || !receivedFrameCache->isEmpty()) {
        if (receivedFrameCache->popFrame(frame)) {
            auto canMsgRx = CanDB::getInstance().getRxCanMessageByID(frame.can_id);
            if (canMsgRx != nullptr) {
                canMsgRx->unpack(frame.data);
            }
            else
            {
                // Do nothing
            }

            displayCanFrame(frame);
        } else {
            std::this_thread::yield();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

// Set cache memory
bool CacheThread::setCacheMemory(std::shared_ptr<ReceivedFrameCache> cache) {
    receivedFrameCache = cache;
    return true;
}
