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

enum CAN_STATUS
{
    eCS_Unknown = 0,
    eCS_Normal,
    eCS_Timeout,
    eCS_Error
};

template <typename T>
struct CAN_DATA
{
    T value;
    bool updated;
};

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

// Define a helper function to copy a property to a buffer
template <typename T>
void copyPropertyToBuffer(const T& property, uint8_t*& buffer) {
    std::memcpy(buffer, &property, sizeof(property));
    buffer += sizeof(property);
}

// Recursive function to copy all properties to the buffer
template <typename... Properties>
void copyPropertiesToBuffer(uint8_t*& buffer, const Properties&... properties) {
    (copyPropertyToBuffer(properties, buffer), ...);
}

class CanFrame {
public:
    CanFrame(canid_t canId, const uint8_t* data, uint8_t dataSize) : frame{} {
        if (dataSize > CANFD_MAX_DLEN) {
            throw std::invalid_argument("DLC exceeds maximum allowed value");
        }

        frame.can_id = canId;
        frame.can_dlc = dataSize;

        if (dataSize > 0) {
            // Copying data to __u8 array
            std::copy(data, data + dataSize, frame.data);
        }
    }

    const struct can_frame& getFrame() const {
        return frame;
    }

private:
    struct can_frame frame;
};

class ICAN_MSG 
{
protected:
    canid_t     id;
    uint16_t    periodTimeMs;
    bool        updated;
    uint16_t    timeoutMs;
    uint8_t     dlc;
    CAN_STATUS status;
public:
    virtual CanFrame pack() = 0;
    virtual void unpack(const uint8_t* frame) = 0; 
};

#endif