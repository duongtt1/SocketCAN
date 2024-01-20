/**
 * @file common.h
 * @brief Common header file for SocketCAN communication.
 */

#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <cstring>
#include <memory>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream>

#include "SafeQueue.h"

using namespace std::chrono;


#define MAX_SIZE_CACHE 50

/**
 * @brief Enumeration representing the status of CAN communication.
 */
enum CAN_STATUS
{
    eCS_Unknown = 0, /**< Unknown CAN status. */
    eCS_Normal,      /**< Normal CAN communication. */
    eCS_Timeout,     /**< Timeout during CAN communication. */
    eCS_Error        /**< Error in CAN communication. */
};

/**
 * @brief Structure to hold CAN data along with an update flag.
 * @tparam T Type of the CAN data value.
 */
template <typename T>
struct CAN_DATA
{
    T value;        /**< CAN data value. */
    bool updated;   /**< Flag indicating whether the data has been updated. */
};

/**
 * @brief Extracts the signal value from a CAN frame.
 * 
 * @tparam T Type of the signal value.
 * @param frame Pointer to the CAN frame data.
 * @param startBit Starting bit position of the signal in the CAN frame.
 * @param endBit Ending bit position of the signal in the CAN frame.
 * 
 * @return Extracted signal value.
 */
template <typename T>
T extractSignalValue(const uint8_t* frame, uint8_t startBit, uint8_t endBit) {
    // Ensure valid start and end bit positions
    if (startBit < 0 || endBit >= 64 || startBit > endBit) {
        std::cerr << "Invalid start or end bit positions." << std::endl;
        return 0;  // Return 0 as an error value
    }

    // Calculate the number of bits to extract
    int8_t numBits = endBit - startBit + 1;

    // Initialize the extracted value to zero
    T extractedValue = 0;

    // Extract the bits from the CAN frame data
    for (int8_t i = startBit; i <= endBit; ++i) {
        int8_t byteIndex = i / 8;
        int8_t bitIndex = i % 8;

        // Extract the bit and update the extracted value
        extractedValue |= ((frame[byteIndex] >> bitIndex) & 0x01) << (i - startBit);
    }

    return extractedValue;
}

/**
 * @brief Converts a steady_clock::time_point to a string representation.
 * 
 * @param timePoint The time point to be converted.
 * 
 * @return String representation of the time point.
 */
std::string timePointToString(const std::chrono::steady_clock::time_point& timePoint);

template <typename T>
void copyPropertyToBuffer(const T& property, uint8_t*& buffer) {
    std::memcpy(buffer, &property, sizeof(property));
    buffer += sizeof(property);
}

void displayCanFrame(const can_frame& frame);

/**
 * @brief Copies a property to a buffer.
 * 
 * @tparam T Type of the property.
 * @param property The property to be copied.
 * 
 * @param buffer Pointer to the destination buffer.
 */
template <typename... Properties>
void copyPropertiesToBuffer(uint8_t*& buffer, const Properties&... properties) {
    (copyPropertyToBuffer(properties, buffer), ...);
}

/**
 * @brief Class representing a CAN frame.
 */
class CanFrame {
public:
    /**
     * @brief Constructor to create a CanFrame object.
     * 
     * @param canId CAN identifier.
     * @param data Pointer to the CAN frame data.
     * @param dataSize Size of the CAN frame data.
     * 
     * @throw std::invalid_argument if DLC exceeds the maximum allowed value.
     */
    CanFrame(canid_t canId, const uint8_t* data, uint8_t dataSize) : frame() {
        if (dataSize > CANFD_MAX_DLEN) {
            throw std::invalid_argument("DLC exceeds maximum allowed value");
        }

        frame.can_id = canId;
        frame.len = dataSize;  // For CAN FD, use 'len' instead of 'can_dlc'

        if (dataSize > 0) {
            // Copying data to __u8 array
            std::copy(data, data + dataSize, frame.data);
        }
    }

    /**
     * @brief Gets the underlying CAN frame structure.
     * @return Reference to the canfd_frame structure.
     */
    const struct canfd_frame& getFrame() const {
        return frame;
    }

private:
    /**< Underlying CAN frame structure. */
    struct canfd_frame frame;
};

class ReceivedFrameCache {
private:
    SafeQueue<can_frame> frameCache;

public:
    ReceivedFrameCache() : frameCache(MAX_SIZE_CACHE) {}
    ~ReceivedFrameCache() {}

    bool addFrame(const can_frame frame) {
        if (frameCache.fully()) {
            return false;
        }
        frameCache.push(frame);
        return true;
    }

    bool popFrame(can_frame& frame) {
        if (frameCache.empty()) {
            return false;
        }
        frame = frameCache.pop();
        return true;
    }

    bool isEmpty() {
        return frameCache.empty();
    }
};

/**
 * @brief Interface class for CAN messages.
 */
class ICAN_MSG {
protected:
    canid_t id;                           /**< CAN identifier. */
    uint16_t periodTimeMs;                /**< Periodic time in milliseconds. */
    bool updated;                         /**< Flag indicating whether the message is updated. */
    uint16_t timeoutMs;                   /**< Timeout value in milliseconds. */
    uint8_t dlc;                          /**< Data Length Code. */
    CAN_STATUS status;                    /**< CAN message status. */
    std::chrono::steady_clock::time_point lastActivated; /**< Last activation time point. */
public:
    /**
     * @brief Packs the CAN message into a CanFrame object.
     * @return CanFrame object representing the packed CAN message.
     */
    virtual CanFrame pack() = 0;

    /**
     * @brief Unpacks the CAN message from a CAN frame.
     * @param frame Pointer to the CAN frame data.
     */
    virtual void unpack(const uint8_t* frame) = 0;

    /**
     * @brief Gets the last activation time point.
     * @return Last activation time point.
     */
    std::chrono::steady_clock::time_point getLastActivated();

    /**
     * @brief Sets the last activation time point.
     * @param newTime New time point to be set.
     */
    std::chrono::steady_clock::time_point setLastActivated(std::chrono::steady_clock::time_point newTime);

    /**
     * @brief Gets the periodic time of the CAN message.
     * @return Periodic time in milliseconds.
     */
    uint16_t getPeriodTime();
};

#endif