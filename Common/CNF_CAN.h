/* Auto-generated file - DO NOT EDIT */
#ifndef CNF_CAN_H
#define CNF_CAN_H

#include <stdint.h>
#include "Common.h"

#define MSG_DATC11 0x68
#define MSG_DATC13 0x67
#define MSG_DATC12 0x66
#define MSG_DATC14 0x64

uint32_t u32CAN_ID_Receive_Filter[] = {
    MSG_DATC11,
    MSG_DATC13,
    MSG_DATC12,
    MSG_DATC14,
};

struct DATC11_t
{
    CAN_DATA<uint8_t> CF_Datc_Type;
    CAN_DATA<uint8_t> CF_Datc_VerMaj;
    CAN_DATA<uint8_t> CF_Datc_VerMin;
    CAN_DATA<uint8_t> CR_Datc_OutTempC;
    CAN_DATA<uint8_t> CR_Datc_OutTempF;
    CAN_DATA<uint8_t> CF_Datc_IncarTemp;
};

struct DATC13_t
{
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
};

struct DATC12_t
{
    CAN_DATA<uint8_t> CR_Datc_DrTempDispC;
    CAN_DATA<uint8_t> CR_Datc_DrTempDispF;
    CAN_DATA<uint8_t> CR_Datc_PsTempDispC;
    CAN_DATA<uint8_t> CR_Datc_PsTempDispF;
    CAN_DATA<uint8_t> CR_Datc_RearDrTempDispC;
    CAN_DATA<uint8_t> CR_Datc_RearDrTempDispF;
    CAN_DATA<uint8_t> CF_Datc_CO2_Warning;
};

struct DATC14_t
{
    CAN_DATA<uint8_t> CF_Datc_AqsLevelOut;
    CAN_DATA<uint8_t> CF_Datc_DiagMode;
    CAN_DATA<uint8_t> CR_Datc_SelfDiagCode;
    CAN_DATA<uint8_t> DATC_SyncDisp;
    CAN_DATA<uint8_t> DATC_OffDisp;
    CAN_DATA<uint8_t> DATC_SmartVentDisp;
    CAN_DATA<uint8_t> DATC_SmartVentOnOffStatus;
    CAN_DATA<uint8_t> DATC_AutoDefogSysOff_Disp;
    CAN_DATA<uint8_t> DATC_ADSDisp;
};

#endif
