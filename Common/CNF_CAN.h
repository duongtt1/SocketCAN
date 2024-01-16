/* Auto-generated file - DO NOT EDIT */
#ifndef CNF_CLASS_CAN_H
#define CNF_CLASS_CAN_H

#include <stdint.h>
#include "Common.h"
#include <type_traits>
#include <iostream>

#define DATC11_ID 0x68
#define DATC13_ID 0x67
#define DATC12_ID 0x66
#define DATC14_ID 0x64

uint32_t u32CAN_ID_Receive_Filter[] = {
    DATC11_ID,
    DATC13_ID,
    DATC12_ID,
    DATC14_ID,
};

template <typename T>
T extractSignalValue(const uint8_t* frame, uint8_t startBit, uint8_t endBit) {
    // Ensure valid start and end bit positions
    if (startBit < 0 || endBit >= 64 || startBit > endBit) {
        std::cerr << "Invalid start or end bit positions." << std::endl;
        return static_cast<T>(0);  // Return 0 as an error value
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

    std::cout << "extractedValue: " << static_cast<uint32_t>(extractedValue) << "\n";
    return extractedValue;
}



class DATC11
{
private:
        CAN_DATA<uint8_t> CF_Datc_Type;
        CAN_DATA<uint8_t> CF_Datc_VerMaj;
        CAN_DATA<uint8_t> CF_Datc_VerMin;
        CAN_DATA<uint8_t> CR_Datc_OutTempC;
        CAN_DATA<uint8_t> CR_Datc_OutTempF;
        CAN_DATA<uint8_t> CF_Datc_IncarTemp;
public:
    void Unpack_DATC11(const uint8_t* frame) {
            CF_Datc_Type.value = static_cast<decltype(CF_Datc_Type.value)>(extractSignalValue<decltype(CF_Datc_Type.value)>(frame, (uint8_t)0, (uint8_t)7));
            CF_Datc_VerMaj.value = static_cast<decltype(CF_Datc_VerMaj.value)>(extractSignalValue<decltype(CF_Datc_VerMaj.value)>(frame, (uint8_t)8, (uint8_t)15));
            CF_Datc_VerMin.value = static_cast<decltype(CF_Datc_VerMin.value)>(extractSignalValue<decltype(CF_Datc_VerMin.value)>(frame, (uint8_t)16, (uint8_t)23));
            CR_Datc_OutTempC.value = static_cast<decltype(CR_Datc_OutTempC.value)>(extractSignalValue<decltype(CR_Datc_OutTempC.value)>(frame, (uint8_t)24, (uint8_t)31));
            CR_Datc_OutTempF.value = static_cast<decltype(CR_Datc_OutTempF.value)>(extractSignalValue<decltype(CR_Datc_OutTempF.value)>(frame, (uint8_t)32, (uint8_t)39));
            CF_Datc_IncarTemp.value = static_cast<decltype(CF_Datc_IncarTemp.value)>(extractSignalValue<decltype(CF_Datc_IncarTemp.value)>(frame, (uint8_t)40, (uint8_t)47));
    }
};

class DATC13
{
private:
        CAN_DATA<uint8_t> CF_Datc_TempDispUnit;
        CAN_DATA<uint8_t> CF_Datc_ModDisp;
        CAN_DATA<uint8_t> CF_Datc_IonClean;
        CAN_DATA<uint8_t> CF_Datc_ChgReqDisp;
        CAN_DATA<uint8_t> CF_Datc_IntakeDisp;
        CAN_DATA<uint8_t> CF_Datc_AutoDisp;
        CAN_DATA<uint8_t> CF_Datc_FrDefLed;
        CAN_DATA<uint8_t> CF_Datc_AutoDefogBlink;
        CAN_DATA<uint8_t> CF_Datc_ClmScanDisp;
        CAN_DATA<uint8_t> CF_Datc_AqsDisp;
        CAN_DATA<uint8_t> CF_Datc_AcDisp;
        CAN_DATA<uint8_t> CF_Datc_OpSts;
        CAN_DATA<uint8_t> CF_Mtc_MaxAcDisp;
        CAN_DATA<uint8_t> CF_Datc_DualDisp;
        CAN_DATA<uint8_t> CF_Datc_PwrInf;
        CAN_DATA<uint8_t> CF_Datc_RearManual;
        CAN_DATA<uint8_t> CF_Datc_RearAutoDisp;
        CAN_DATA<uint8_t> CF_Datc_RearOffDisp;
        CAN_DATA<uint8_t> CF_Datc_RearClimateScnDisp;
        CAN_DATA<uint8_t> CF_Datc_RearChgReqDisp;
        CAN_DATA<uint8_t> CF_Datc_RearModDisp;
        CAN_DATA<uint8_t> CF_Datc_RearBlwDisp;
        CAN_DATA<uint8_t> CF_Datc_PSModDisp;
        CAN_DATA<uint8_t> CF_Datc_FrontBlwDisp;
public:
    void Unpack_DATC13(const uint8_t* frame) {
            CF_Datc_TempDispUnit.value = static_cast<decltype(CF_Datc_TempDispUnit.value)>(extractSignalValue<decltype(CF_Datc_TempDispUnit.value)>(frame, (uint8_t)0, (uint8_t)1));
            CF_Datc_ModDisp.value = static_cast<decltype(CF_Datc_ModDisp.value)>(extractSignalValue<decltype(CF_Datc_ModDisp.value)>(frame, (uint8_t)2, (uint8_t)5));
            CF_Datc_IonClean.value = static_cast<decltype(CF_Datc_IonClean.value)>(extractSignalValue<decltype(CF_Datc_IonClean.value)>(frame, (uint8_t)6, (uint8_t)7));
            CF_Datc_ChgReqDisp.value = static_cast<decltype(CF_Datc_ChgReqDisp.value)>(extractSignalValue<decltype(CF_Datc_ChgReqDisp.value)>(frame, (uint8_t)8, (uint8_t)9));
            CF_Datc_IntakeDisp.value = static_cast<decltype(CF_Datc_IntakeDisp.value)>(extractSignalValue<decltype(CF_Datc_IntakeDisp.value)>(frame, (uint8_t)10, (uint8_t)11));
            CF_Datc_AutoDisp.value = static_cast<decltype(CF_Datc_AutoDisp.value)>(extractSignalValue<decltype(CF_Datc_AutoDisp.value)>(frame, (uint8_t)12, (uint8_t)13));
            CF_Datc_FrDefLed.value = static_cast<decltype(CF_Datc_FrDefLed.value)>(extractSignalValue<decltype(CF_Datc_FrDefLed.value)>(frame, (uint8_t)14, (uint8_t)15));
            CF_Datc_AutoDefogBlink.value = static_cast<decltype(CF_Datc_AutoDefogBlink.value)>(extractSignalValue<decltype(CF_Datc_AutoDefogBlink.value)>(frame, (uint8_t)16, (uint8_t)17));
            CF_Datc_ClmScanDisp.value = static_cast<decltype(CF_Datc_ClmScanDisp.value)>(extractSignalValue<decltype(CF_Datc_ClmScanDisp.value)>(frame, (uint8_t)18, (uint8_t)19));
            CF_Datc_AqsDisp.value = static_cast<decltype(CF_Datc_AqsDisp.value)>(extractSignalValue<decltype(CF_Datc_AqsDisp.value)>(frame, (uint8_t)20, (uint8_t)21));
            CF_Datc_AcDisp.value = static_cast<decltype(CF_Datc_AcDisp.value)>(extractSignalValue<decltype(CF_Datc_AcDisp.value)>(frame, (uint8_t)22, (uint8_t)23));
            CF_Datc_OpSts.value = static_cast<decltype(CF_Datc_OpSts.value)>(extractSignalValue<decltype(CF_Datc_OpSts.value)>(frame, (uint8_t)25, (uint8_t)27));
            CF_Mtc_MaxAcDisp.value = static_cast<decltype(CF_Mtc_MaxAcDisp.value)>(extractSignalValue<decltype(CF_Mtc_MaxAcDisp.value)>(frame, (uint8_t)28, (uint8_t)29));
            CF_Datc_DualDisp.value = static_cast<decltype(CF_Datc_DualDisp.value)>(extractSignalValue<decltype(CF_Datc_DualDisp.value)>(frame, (uint8_t)30, (uint8_t)31));
            CF_Datc_PwrInf.value = static_cast<decltype(CF_Datc_PwrInf.value)>(extractSignalValue<decltype(CF_Datc_PwrInf.value)>(frame, (uint8_t)32, (uint8_t)35));
            CF_Datc_RearManual.value = static_cast<decltype(CF_Datc_RearManual.value)>(extractSignalValue<decltype(CF_Datc_RearManual.value)>(frame, (uint8_t)38, (uint8_t)39));
            CF_Datc_RearAutoDisp.value = static_cast<decltype(CF_Datc_RearAutoDisp.value)>(extractSignalValue<decltype(CF_Datc_RearAutoDisp.value)>(frame, (uint8_t)40, (uint8_t)41));
            CF_Datc_RearOffDisp.value = static_cast<decltype(CF_Datc_RearOffDisp.value)>(extractSignalValue<decltype(CF_Datc_RearOffDisp.value)>(frame, (uint8_t)42, (uint8_t)43));
            CF_Datc_RearClimateScnDisp.value = static_cast<decltype(CF_Datc_RearClimateScnDisp.value)>(extractSignalValue<decltype(CF_Datc_RearClimateScnDisp.value)>(frame, (uint8_t)44, (uint8_t)45));
            CF_Datc_RearChgReqDisp.value = static_cast<decltype(CF_Datc_RearChgReqDisp.value)>(extractSignalValue<decltype(CF_Datc_RearChgReqDisp.value)>(frame, (uint8_t)46, (uint8_t)47));
            CF_Datc_RearModDisp.value = static_cast<decltype(CF_Datc_RearModDisp.value)>(extractSignalValue<decltype(CF_Datc_RearModDisp.value)>(frame, (uint8_t)48, (uint8_t)51));
            CF_Datc_RearBlwDisp.value = static_cast<decltype(CF_Datc_RearBlwDisp.value)>(extractSignalValue<decltype(CF_Datc_RearBlwDisp.value)>(frame, (uint8_t)52, (uint8_t)55));
            CF_Datc_PSModDisp.value = static_cast<decltype(CF_Datc_PSModDisp.value)>(extractSignalValue<decltype(CF_Datc_PSModDisp.value)>(frame, (uint8_t)56, (uint8_t)59));
            CF_Datc_FrontBlwDisp.value = static_cast<decltype(CF_Datc_FrontBlwDisp.value)>(extractSignalValue<decltype(CF_Datc_FrontBlwDisp.value)>(frame, (uint8_t)60, (uint8_t)63));
    }
};

class DATC12
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
    void Unpack_DATC12(const uint8_t* frame) {
            CR_Datc_DrTempDispC.value = static_cast<decltype(CR_Datc_DrTempDispC.value)>(extractSignalValue<decltype(CR_Datc_DrTempDispC.value)>(frame, (uint8_t)0, (uint8_t)7));
            CR_Datc_DrTempDispF.value = static_cast<decltype(CR_Datc_DrTempDispF.value)>(extractSignalValue<decltype(CR_Datc_DrTempDispF.value)>(frame, (uint8_t)8, (uint8_t)15));
            CR_Datc_PsTempDispC.value = static_cast<decltype(CR_Datc_PsTempDispC.value)>(extractSignalValue<decltype(CR_Datc_PsTempDispC.value)>(frame, (uint8_t)16, (uint8_t)23));
            CR_Datc_PsTempDispF.value = static_cast<decltype(CR_Datc_PsTempDispF.value)>(extractSignalValue<decltype(CR_Datc_PsTempDispF.value)>(frame, (uint8_t)24, (uint8_t)31));
            CR_Datc_RearDrTempDispC.value = static_cast<decltype(CR_Datc_RearDrTempDispC.value)>(extractSignalValue<decltype(CR_Datc_RearDrTempDispC.value)>(frame, (uint8_t)40, (uint8_t)47));
            CR_Datc_RearDrTempDispF.value = static_cast<decltype(CR_Datc_RearDrTempDispF.value)>(extractSignalValue<decltype(CR_Datc_RearDrTempDispF.value)>(frame, (uint8_t)48, (uint8_t)55));
            CF_Datc_CO2_Warning.value = static_cast<decltype(CF_Datc_CO2_Warning.value)>(extractSignalValue<decltype(CF_Datc_CO2_Warning.value)>(frame, (uint8_t)56, (uint8_t)63));
    }
};

class DATC14
{
private:
        CAN_DATA<uint8_t> CF_Datc_AqsLevelOut;
        CAN_DATA<uint8_t> CF_Datc_DiagMode;
        CAN_DATA<uint8_t> CR_Datc_SelfDiagCode;
        CAN_DATA<uint8_t> DATC_SyncDisp;
        CAN_DATA<uint8_t> DATC_OffDisp;
        CAN_DATA<uint8_t> DATC_SmartVentDisp;
        CAN_DATA<uint8_t> DATC_SmartVentOnOffStatus;
        CAN_DATA<uint8_t> DATC_AutoDefogSysOff_Disp;
        CAN_DATA<uint8_t> DATC_ADSDisp;
public:
    void Unpack_DATC14(const uint8_t* frame) {
            CF_Datc_AqsLevelOut.value = static_cast<decltype(CF_Datc_AqsLevelOut.value)>(extractSignalValue<decltype(CF_Datc_AqsLevelOut.value)>(frame, (uint8_t)0, (uint8_t)3));
            CF_Datc_DiagMode.value = static_cast<decltype(CF_Datc_DiagMode.value)>(extractSignalValue<decltype(CF_Datc_DiagMode.value)>(frame, (uint8_t)6, (uint8_t)7));
            CR_Datc_SelfDiagCode.value = static_cast<decltype(CR_Datc_SelfDiagCode.value)>(extractSignalValue<decltype(CR_Datc_SelfDiagCode.value)>(frame, (uint8_t)8, (uint8_t)15));
            DATC_SyncDisp.value = static_cast<decltype(DATC_SyncDisp.value)>(extractSignalValue<decltype(DATC_SyncDisp.value)>(frame, (uint8_t)16, (uint8_t)19));
            DATC_OffDisp.value = static_cast<decltype(DATC_OffDisp.value)>(extractSignalValue<decltype(DATC_OffDisp.value)>(frame, (uint8_t)20, (uint8_t)21));
            DATC_SmartVentDisp.value = static_cast<decltype(DATC_SmartVentDisp.value)>(extractSignalValue<decltype(DATC_SmartVentDisp.value)>(frame, (uint8_t)22, (uint8_t)23));
            DATC_SmartVentOnOffStatus.value = static_cast<decltype(DATC_SmartVentOnOffStatus.value)>(extractSignalValue<decltype(DATC_SmartVentOnOffStatus.value)>(frame, (uint8_t)24, (uint8_t)25));
            DATC_AutoDefogSysOff_Disp.value = static_cast<decltype(DATC_AutoDefogSysOff_Disp.value)>(extractSignalValue<decltype(DATC_AutoDefogSysOff_Disp.value)>(frame, (uint8_t)26, (uint8_t)27));
            DATC_ADSDisp.value = static_cast<decltype(DATC_ADSDisp.value)>(extractSignalValue<decltype(DATC_ADSDisp.value)>(frame, (uint8_t)28, (uint8_t)29));
    }
};


#endif