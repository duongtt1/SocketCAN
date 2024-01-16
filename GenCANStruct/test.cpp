#include <iostream>
#include <stdint.h>
#include <bitset>

template <typename T>
T extractSignalValue(const uint8_t* frame, uint8_t startBit, uint8_t endBit) {
    // Ensure valid start and end bit positions
    if (startBit < 0 || endBit >= 64 || startBit > endBit) {
        std::cerr << "Invalid start or end bit positions." << std::endl;
        return 0;  // Return 0 as an error value
    }
    int8_t numBits = endBit - startBit + 1;
    T extractedValue = 0;
    for (int8_t i = startBit; i <= endBit; ++i) {
        int8_t byteIndex = i / 8;
        int8_t bitIndex = i % 8;
        extractedValue |= ((frame[byteIndex] >> bitIndex) & 0x01) << (i - startBit);
    }

    return extractedValue;
}

int main() {
    // Define a test frame (8 bytes)
    const uint8_t testFrame[] = {0b11011010, 0b10101010, 0b11110000, 0b00110011, 0b01010101, 0b11001100, 0b00111100, 0b11100011};

    // Test cases
    uint8_t result8 = extractSignalValue<uint8_t>(testFrame, 4, 7);
    uint16_t result16 = extractSignalValue<uint16_t>(testFrame, 0, 15);
    uint32_t result32 = extractSignalValue<uint32_t>(testFrame, 16, 47);

    // Print results
    std::cout << "Result (4-7 bits): " << std::bitset<4>(result8) << std::endl;
    std::cout << "Result (0-15 bits): " << std::bitset<16>(result16) << std::endl;
    std::cout << "Result (16-47 bits): " << std::bitset<32>(result32) << std::endl;

    std::cout << "Result (4-7 bits): " << static_cast<int8_t>(result8) << std::endl;
    std::cout << "Result (0-15 bits): " << result16 << std::endl;
    std::cout << "Result (16-47 bits): " << result32 << std::endl;

    return 0;
}