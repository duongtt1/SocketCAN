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