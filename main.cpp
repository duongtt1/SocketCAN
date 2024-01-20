#include "Common/CNF_CAN.h"
#include "CanSocket.h"
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <cstddef>  // Include for std::size_t
#include <boost/type_index.hpp>
#include <bitset>
#include "CanDB.h"
#include "CanSocket.h"

int main() {
    initRx();
    initTx();
    CanSocket canSck("vcan0");
    canSck.startRecvThread();
    canSck.startSendThread();
    
    while (1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    

    return 0;
}
