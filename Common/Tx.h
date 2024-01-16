#include "Common.h"

class DATC01_100ms
{
private:
    CAN_DATA<uint8_t> CR_Datc_DrTempDispC;
    CAN_DATA<uint8_t> CR_Datc_DrTempDispF;
    CAN_DATA<uint8_t> CR_Datc_PsTempDispC;
    CAN_DATA<uint8_t> CR_Datc_PsTempDispF;
    CAN_DATA<uint8_t> CR_Datc_RearDrTempDispC;
    CAN_DATA<uint8_t> CR_Datc_RearDrTempDispF;
    CAN_DATA<uint8_t> CF_Datc_CO2_Warning;
public:
    
};
