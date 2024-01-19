#include "CacheRecv.h"

void displayCanFrame(const can_frame& frame) {
    // Print interface name (e.g., vcan0)
    std::cout << "vcan0 ";

    // Print CAN ID
    std::cout << std::hex << std::setw(3) << std::setfill('0') << frame.can_id << " ";

    std::cout << "[" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(frame.can_dlc) << "] ";

    // Print data bytes
    for (int i = 0; i < frame.can_dlc; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(frame.data[i]) << " ";
    }

    // Reset to decimal for the next output
    std::cout << std::dec << std::endl;
}

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
    while (runThread.load() || !cacheRecv->empty()) {
        if (cacheRecv->popFrame(frame)) {
            std::cout << "Queue not empty \n";
            std::cout << "frame.can_id: " << frame.can_id << "\n";

            if (CanDB::getInstance().getCanMsgRx(frame.can_id) != nullptr)
            {
                std::cout << "unpack \n";
                CanDB::getInstance().getCanMsgRx(frame.can_id)->unpack(frame.data);
            }
            else
            {
                std::cout << "NULL \n";
            }

            displayCanFrame(frame);
        } else {
            // std::cout << "Queue empty \n";
            std::this_thread::yield();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}


bool CacheThread::setCacheMemory(std::shared_ptr<CacheRecv> cache)
{
    this->cacheRecv = cache;
    return true;
}