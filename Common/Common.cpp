#include "Common.h"

std::string timePointToString(const std::chrono::steady_clock::time_point& timePoint) {
    // Convert steady_clock::time_point to system_clock::time_point
    auto systemTimePoint = std::chrono::system_clock::now() +
                           std::chrono::duration_cast<std::chrono::system_clock::duration>(timePoint.time_since_epoch());

    // Convert system_clock::time_point to std::time_t
    auto timeT = std::chrono::system_clock::to_time_t(systemTimePoint);

    // Convert std::time_t to tm structure
    std::tm timeInfo = *std::localtime(&timeT);

    // Extract milliseconds from time_point
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(timePoint.time_since_epoch()) % 1000;

    // Format the string
    std::ostringstream oss;
    oss << std::put_time(&timeInfo, "%Y-%m-%d %H:%M:%S") << '.' << std::setfill('0') << std::setw(3) << ms.count();

    return oss.str();
}

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

std::chrono::steady_clock::time_point ICAN_MSG::getLastActivated() {
    return lastActivated;
}

std::chrono::steady_clock::time_point ICAN_MSG::setLastActivated(steady_clock::time_point newTime) {
    lastActivated = newTime;
    return lastActivated;
}

uint16_t ICAN_MSG::getPeriodTime() {
    return periodTimeMs;
}
