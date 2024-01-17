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
    CanSocket canSck("vcan0");

    return 0;
}
