#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <cstring>
#include <stdexcept>
#include <memory>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <vector>

enum eCAN_STATUS
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
    eCAN_STATUS status;
};

#include <cstdint>
#include <stdexcept>
#include <algorithm>

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



#endif