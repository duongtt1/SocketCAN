#include "Common/CNF_CAN.h"
#include "CanSocket.h"
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <cstddef>  // Include for std::size_t
#include <boost/type_index.hpp>
#include <bitset>

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


class DATC01_100ms
{
public:
    CAN_DATA<uint8_t> CR_Datc_DrTempDispC;
    CAN_DATA<uint8_t> CR_Datc_DrTempDispF;
    CAN_DATA<uint8_t> CR_Datc_PsTempDispC;
    CAN_DATA<uint8_t> CR_Datc_PsTempDispF;
    CAN_DATA<uint8_t> CR_Datc_RearDrTempDispC;
    CAN_DATA<uint8_t> CR_Datc_RearDrTempDispF;
    CAN_DATA<uint8_t> CF_Datc_CO2_Warning;

    canid_t id;

    CanFrame Pack_DATC01_100ms()
    {
        id = 0x290;
        // Create a buffer to hold the combined data
        uint8_t combinedData[CANFD_MAX_DLEN] = {0};  // Assuming MAX_DATA_SIZE is a static constant in CanFrame

        // Copy all properties to the combined buffer
        uint8_t* buffer = combinedData;
        copyPropertiesToBuffer(buffer, CR_Datc_DrTempDispC, CR_Datc_DrTempDispF,
                                CR_Datc_PsTempDispC, CR_Datc_PsTempDispF,
                                CR_Datc_RearDrTempDispC, CR_Datc_RearDrTempDispF,
                                CF_Datc_CO2_Warning);

        // Create a CanFrame instance with the combined data
        CanFrame combinedFrame(id, combinedData, CANFD_MAX_DLEN);

        return combinedFrame;
    }
};


class DATC11_100ms
{
public:
        CAN_DATA<uint8_t> CF_Datc_Type;
        CAN_DATA<uint8_t> CF_Datc_VerMaj;
        CAN_DATA<uint8_t> CF_Datc_VerMin;
        CAN_DATA<uint8_t> CR_Datc_OutTempC;
        CAN_DATA<uint8_t> CR_Datc_OutTempF;
        CAN_DATA<uint8_t> CF_Datc_IncarTemp;

    void Unpack_DATC11(const uint8_t* frame) {
        std::cout << "enter 1 \n"; 
            CF_Datc_Type.value = static_cast<decltype(CF_Datc_Type.value)>(extractSignalValue<decltype(CF_Datc_Type.value)>(frame, (uint8_t)0, (uint8_t)7));
            CF_Datc_VerMaj.value = static_cast<decltype(CF_Datc_VerMaj.value)>(extractSignalValue<decltype(CF_Datc_VerMaj.value)>(frame, (uint8_t)8, (uint8_t)15));
            CF_Datc_VerMin.value = static_cast<decltype(CF_Datc_VerMin.value)>(extractSignalValue<decltype(CF_Datc_VerMin.value)>(frame, (uint8_t)16, (uint8_t)23));
            CR_Datc_OutTempC.value = static_cast<decltype(CR_Datc_OutTempC.value)>(extractSignalValue<decltype(CR_Datc_OutTempC.value)>(frame, (uint8_t)24, (uint8_t)31));
            CR_Datc_OutTempF.value = static_cast<decltype(CR_Datc_OutTempF.value)>(extractSignalValue<decltype(CR_Datc_OutTempF.value)>(frame, (uint8_t)32, (uint8_t)39));
            CF_Datc_IncarTemp.value = static_cast<decltype(CF_Datc_IncarTemp.value)>(extractSignalValue<decltype(CF_Datc_IncarTemp.value)>(frame, (uint8_t)40, (uint8_t)47));
    }
};

template <typename T>
std::string toBinaryString(T value) {
    return std::bitset<sizeof(T) * 8>(value).to_string();
}


int main() {
    CanSocket csck("vcan0");
    DATC01_100ms a;
    a.CF_Datc_CO2_Warning.value = 1;
    a.CR_Datc_DrTempDispF.value = 2;
    a.CR_Datc_PsTempDispC.value = 3;
    a.CR_Datc_PsTempDispF.value = 4;
    a.CR_Datc_RearDrTempDispC.value = 5;
    a.CR_Datc_RearDrTempDispF.value = 6;
    a.CF_Datc_CO2_Warning.value = 0x1u;
    
    CanFrame b = a.Pack_DATC01_100ms();
    
    // while (1)
    // {
    //     csck.sendCanFrame(b);
    //     std::this_thread::sleep_for(std::chrono::milliseconds(500));
    // }
    const canid_t canIds[] = {0x291};
    csck.setFilter(canIds, 1);
    CanFrame datc = csck.receiveCanFrame();
    DATC11_100ms datc11;;
    datc11.Unpack_DATC11(datc.getFrame().data);

    std::cout << "CF_Datc_IncarTemp: " << toBinaryString(datc11.CF_Datc_IncarTemp.value) << "\n";
    std::cout << "CF_Datc_Type: " << toBinaryString(datc11.CF_Datc_Type.value) << "\n";
    std::cout << "CF_Datc_VerMaj: " << toBinaryString(datc11.CF_Datc_VerMaj.value) << "\n";
    std::cout << "CF_Datc_VerMin: " << toBinaryString(datc11.CF_Datc_VerMin.value) << "\n";
    std::cout << "CR_Datc_OutTempC: " << toBinaryString(datc11.CR_Datc_OutTempC.value) << "\n";
    std::cout << "CR_Datc_OutTempF: " << toBinaryString(datc11.CR_Datc_OutTempF.value) << "\n";
    
    //  cansend vcan0 291##112345678
    //  vcan0  291  [04]  12 34 56 78
    std::cout << boost::typeindex::type_id_with_cvr<decltype(datc11.CF_Datc_IncarTemp.value)>().pretty_name() << std::endl;
    return 0;
}


// enter 1 
// extractedValue: 2
// extractedValue: 3
// extractedValue: 4
// extractedValue: 5
// extractedValue: 0
// extractedValue: 0
// CF_Datc_IncarTemp: 00000000
// CF_Datc_Type: 00000010
// CF_Datc_VerMaj: 00000011
// CF_Datc_VerMin: 00000100
// CR_Datc_OutTempC: 00000101
// CR_Datc_OutTempF: 00000000
// unsigned char

// cansend vcan0 291##102030405
