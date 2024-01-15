#include "Common/CNF_CAN.h"
#include "CanSocket.h"
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>

class CanApplication {
public:
    CanApplication() : canSocket("vcan0"), stopFlag(false) {}

    void runLogic() {
        while (!stopFlag.load()) {
            // Your main application logic here

            std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Adjust as needed
        }
    }

    void sendThread() {
        while (!stopFlag.load()) {
            // Your CAN message sending logic here
            // Example: canSocket.sendCanFrame(yourCanFrame);

            std::this_thread::sleep_for(std::chrono::milliseconds(500));  // Adjust as needed
        }
    }

    void receiveAndStoreThread() {
        while (!stopFlag.load()) {
            // Your CAN message receiving and storing logic here
            // Example: CanFrame receivedFrame = canSocket.receiveCanFrame();
            // Store receivedFrame in a data structure

            std::this_thread::sleep_for(std::chrono::milliseconds(10));  // Adjust as needed
        }
    }

    void startThreads() {
        stopFlag.store(false);

        logicThread = std::thread(&CanApplication::runLogic, this);
        sendThreadHandle = std::thread(&CanApplication::sendThread, this);
        receiveThreadHandle = std::thread(&CanApplication::receiveAndStoreThread, this);
    }

    void stopThreads() {
        stopFlag.store(true);

        if (logicThread.joinable())
            logicThread.join();
        if (sendThreadHandle.joinable())
            sendThreadHandle.join();
        if (receiveThreadHandle.joinable())
            receiveThreadHandle.join();
    }

private:
    CanSocket canSocket;  // Assume CanSocket is properly implemented
    std::thread logicThread;
    std::thread sendThreadHandle;
    std::thread receiveThreadHandle;
    std::atomic<bool> stopFlag;
};

int main() {
    CanApplication myApp;
    myApp.startThreads();

    // Run your main application logic in the main thread if needed

    myApp.stopThreads();
    
    return 0;
}
