/******************************************************************************

                              Copyright (c) 2013
                            Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef _DRV_DSL_CPE_DEVICE_VRX_H
#define _DRV_DSL_CPE_DEVICE_VRX_H

#ifdef __cplusplus
   extern "C" {
#endif

/** \addtogroup DRV_DSL_DEVICE
 @{ */

#include "drv_dsl_cpe_api.h"

#if defined(DSL_CPE_SIMULATOR_DRIVER) && defined(WIN32)
#else
   /** MEI CPE driver configurations*/
   #define MEI_SUPPORT_DEVICE_VR9   1

   /** MEI CPE driver interface header file*/
   #include "drv_mei_cpe_api_intern.h"

#ifdef INCLUDE_DSL_ATM_PTM_INTERFACE_SUPPORT
   #ifdef MEI_DRV_ATM_PTM_INTERFACE_ENABLE
      #undef MEI_DRV_ATM_PTM_INTERFACE_ENABLE
   #endif
   #define MEI_DRV_ATM_PTM_INTERFACE_ENABLE 1
   /** MEI CPE driver ATM/PTM interface header file*/
   #include "drv_mei_cpe_api_atm_ptm_intern.h"
#endif /* INCLUDE_DSL_ATM_PTM_INTERFACE_SUPPORT*/

#endif /* defined(DSL_CPE_SIMULATOR_DRIVER) && defined(WIN32)*/

/** \addtogroup DRV_DSL_DEVICE
 @{ */


/** The following defines specify firmware versions that are needed to be
    checked at runtime to get information if a dedicated feature is available
    from firmware point of view.

    \note The first digit (platform ID) of the six digit FW version number is
          not included within definition.
    \note In case of using '-1' value the according firmware version part is
          don't care.

                                            Feature set
                                            |    Major version
                                            |    |    Major version
                                            |    |    |    Release status
                                            |    |    |    |    Application Type
#define DSL_MIN_FW_VERSION_xxx             -1,   3,  -1,  -1,  -1 */
#define DSL_MIN_FW_VERSION_RETX_ADSL        5,   0, 0xF,  -1,  -1
#define DSL_MIN_FW_VERSION_RETX_VDSL        4,   4,   6,  -1,  -1
#define DSL_MIN_FW_VERSION_RETX             4,   4,   6,  -1,  -1
#define DSL_MIN_FW_VERSION_NTR              4,   5, 0xF,  -1,  -1
#define DSL_MIN_FW_VERSION_MFD              4,   2,   7,  -1,  -1
#define DSL_MIN_FW_VERSION_FGAIN            4,  -1,  -1,  -1,  -1
#define DSL_MIN_FW_VERSION_VIRT_NOISE_ADSL  5,   0, 0xE,  -1,   1
#define DSL_MIN_FW_VERSION_VIRT_NOISE_VDSL  5,   0,   5,  -1,  -1
#define DSL_MIN_FW_VERSION_SRA_VDSL         5,   0,   4,  -1,  -1
#define DSL_MIN_FW_VERSION_R6_RELEASE       6,  -1,  -1,  -1,  -1
#define DSL_MIN_FW_VERSION_TC_STAT_ADSL     4,   0,   8,  -1,  -1

/*
   Internal device dependent configuration options
*/
/* Include DELT showtime values support*/
#define INCLUDE_DSL_DELT_SHOWTIME
#define INCLUDE_DSL_INIT_DATA_PREPARE
#define INCLUDE_DSL_TIMEOUT
#define INCLUDE_DSL_LINIT_STATUS
#define INCLUDE_DSL_NFC_HANDLE

#define DSL_MAX_LINE_NUMBER 1

#if defined (DSL_VRX_DEVICE_VR9)
   #define DSL_CHIPSET_TYPE_STRING "Lantiq-VRX200"
#elif defined (DSL_VRX_DEVICE_VR10)
   #define DSL_CHIPSET_TYPE_STRING "Lantiq-VRX300"
#else
   #error Please defined a valid VRX device (VR9|VR10)!!!
#endif

/** Timeout setting for far end status in mili seconds */
#define DSL_DEV_TIMEOUT_FE_STATUS         12000

/** Defines the Minimum possible Data Rate in bit/s */
#define DSL_DEV_MIN_DATARATE              16000
/** Defines the Maximum possible Data Rate for ADSL2+ in bit/s (DS/US) */
#define DSL_DEV_MAX_DATARATE_DS_ADSL2P    32736000
#define DSL_DEV_MAX_DATARATE_US_ADSL2P    2048000
/** Defines the Maximum possible Data Rates for ADSL and ADSL2 in bit/s (DS/US) */
#define DSL_DEV_MAX_DATARATE_DS_ADSL2     16352000
#define DSL_DEV_MAX_DATARATE_US_ADSL2     2048000

/**
   This bit of the Message ID identifies 32 bit aligned TC messages */
#define VDSL2_MBX_MSG_ID_IFX_MASK 0x0010

typedef MEI_DYN_CNTRL_T DSL_DEV_Device_t;

/**
   States for the CPE Auto Mode (CAM) state machine
   to toggle between ADSL and VDSL activations */
typedef enum
{
   /** CPE Automode feature is not used */
   DSL_CAM_DISABLED = 0,
   /** CPE Automode feature initialization state */
   DSL_CAM_INIT = 1,
   /** CPE Automode feature restricted to VDSL single-port mode*/
   DSL_CAM_VDSL_SINGLE = 2,
   /** CPE Automode feature restricted to VDSL dual-port mode*/
   DSL_CAM_VDSL_DUAL = 3,
   /** CPE Automode feature restricted to ADSL single-port mode*/
   DSL_CAM_ADSL_SINGLE = 4,
   /** CPE Automode delimiter */
   DSL_CAM_LAST = DSL_CAM_ADSL_SINGLE
} DSL_DEV_CamStates_t;


/**
   Structure used to indicate the L3 request status
*/
typedef enum
{
   DSL_VRX_L3_STATUS_NA       = -1,
   DSL_VRX_L3_STATUS_PENDING  = 0,
   DSL_VRX_L3_STATUS_REJECTED = 1,
   DSL_VRX_L3_STATUS_ACCEPTED = 2,
   DSL_VRX_L3_STATUS_FAIL     = 3
} DSL_VRX_L3RequestStatus_t;

typedef enum
{
   DSL_VRX_L3_FAIL_REASON_NA      = -1,
   DSL_VRX_L3_FAIL_REASON_NOT_L0  = 0,
   DSL_VRX_L3_FAIL_REASON_TIMEOUT = 1
} DSL_VRX_L3RequestFailReason_t;

/**
   Structure used to build table which includes a list of 32bit message IDs
*/
typedef struct
{
   /** 16 Bit message ID */
   DSL_uint16_t nMsgId;
} VRX_MsgId_t;

/**
   Structure to keep the decoded Firmware Verion information
   contained in the 32bit Version number
*/
typedef struct
{
   DSL_uint8_t nPlatform;
   /**
      Feature Set Number */
   DSL_uint8_t nFeatureSet;
   /** */
   DSL_uint8_t nMajor;
   /** */
   DSL_uint8_t nMinor;
   /** Pre, Verification, Development */
   DSL_uint8_t nReleaseStatus;
   /** VDSL1/2, ADSL, ... */
   DSL_uint8_t nApplication;
} DSL_VRX_FwVersion_t;

/**
 * A structure for code<->string conversion tables.
 * used mostly during FW/HW version retrieval.
 */
typedef struct
{
   /** version code */
   DSL_uint32_t code;
   /** version string */
   DSL_char_t *string;
} VRX_CodeString_t;

/** Internal identifier for swapping a combined FW between
   ADSL and VDSL */
typedef enum
{
   FWMODE_NA = -1,
   /** VDSL part of FW */
   FWMODE_VDSL = 0,
   /** ADSL part of FW */
   FWMODE_ADSL = 1
} DSL_VRX_FwMode_t;


/**
 * VRx application version codes
 */
typedef enum
{
   DSL_APP_ADSLB_J            = 0x00,  /* ADSL Annex B/J*/
   DSL_APP_ADSLA              = 0x01,  /* ADSL Annex A */
   DSL_APP_ADSLB              = 0x02,  /* ADSL Annex B*/
   DSL_APP_RESERVED_1         = 0x03,  /* Reserved*/
   DSL_APP_RESERVED_2         = 0x04,  /* Reserved*/
   DSL_APP_VDSL_POTS          = 0x05,  /* VDSL over POTS*/
   DSL_APP_VDSL_ISDN          = 0x06,  /* VDSL over ISDN*/
   DSL_APP_VDSL_ISDN_VEC      = 0x07,  /* VDSL over ISDN incl. vectoring support */
   DSL_APP_UNKNOWN,
   DSL_APP_LAST = DSL_APP_UNKNOWN
} DSL_VRX_ApplicationCode_t;

/** VDSL2 profile 8a */
#define DSL_VRX_PROFILES_8A     0x01
/** VDSL2 profile 8b */
#define DSL_VRX_PROFILES_8B     0x02
/** VDSL2 profile 8c */
#define DSL_VRX_PROFILES_8C     0x04
/** VDSL2 profile 8d */
#define DSL_VRX_PROFILES_8D     0x08
/** VDSL2 profile 12a */
#define DSL_VRX_PROFILES_12A    0x10
/** VDSL2 profile 12b */
#define DSL_VRX_PROFILES_12B    0x20
/** VDSL2 profile 17a */
#define DSL_VRX_PROFILES_17A    0x40
/** VDSL2 profile 30a */
#define DSL_VRX_PROFILES_30A    0x80

/**
   Firmware application and feature information extracted from
   the FW version. */
typedef struct
{
   /**
   Applications supported by FW  */
   DSL_VRX_ApplicationCode_t nApplicationType;
} DSL_FwSupport_t;

/**
   Structure to handle DELT Aux test parameters
*/
typedef struct
{
   /** HLIN Scaling Factor*/
   DSL_uint16_t HLINSC;
   /* HLIN Subcarrier Group Size */
   DSL_uint16_t HLING;
   /* HLOG Measurement Time */
   DSL_uint16_t HLOGMT;
   /* HLOG Subcarrier Group Size */
   DSL_uint16_t HLOGG;
   /* QLN Measurment Time */
   DSL_uint16_t QLNMT;
   /* QLN Subcarrier Group Size */
   DSL_uint16_t QLNG;
   /* SNR Measurement Time */
   DSL_uint16_t SNRMT;
   /* SNR Subcarrier Group Size */
   DSL_uint16_t SNRG;
} DSL_VRX_DeltTestParamsAux_t;

/**
   Structure that stores global variables for function calls that are waiting
   in a blocking call for direct response from the device.
*/
typedef struct
{
   /**
   Semaphore used to wait for availability of message waiting procedure. */
   DSL_DRV_Mutex_t MsgTransferMutex;
   /**
   Flag which is used to indicate that a function call waits for data */
   DSL_boolean_t bFctWaiting;
   /**
   Flag which is used to indicate that the data for a waiting function has been
   received */
   DSL_boolean_t bDataReceived;
   /**
   Specifies for which message type the function is waiting */
   DSL_uint8_t nMsgClassifier;
   /**
   Specifies for which message ID the function is waiting */
   DSL_uint16_t nMsgId;
   /**
   Memory to store message data that is requested from the waiting function */
   DSL_uint8_t nData[512];
   /**
   Size of stored message data */
   DSL_uint16_t nLen;
} DSL_FctWaiting_t;

#define DSL_CAM_MAX_FW_REINITS   10

/**
   VRX specific device data*/
typedef struct
{
   /**
   Defines device specific configuration information.*/
   DSL_DeviceConfig_t deviceCfg;
   /**
   Downloaded FW support features*/
   DSL_VRX_FwVersion_t fwFeatures;
   /**
   Multimode operation identification
   */
   DSL_DEV_CamStates_t nCamState;
   DSL_uint8_t nCamReinits;
   DSL_uint8_t nCamFwReinits;
   DSL_boolean_t bCamDualPortModeSteady;
   DSL_boolean_t bCamSinglePortModeSteady;

   DSL_MultimodeFsmConfigData_t nCamStatus;
   DSL_MultimodeFsmConfigData_t nCamConfig;
   DSL_ActivationFsmConfigData_t nActivationCfg;
   DSL_boolean_t bT1_413;
   DSL_boolean_t bRemember;

   DSL_uint8_t nSpar1;
   DSL_uint8_t nSpar2;
   DSL_uint8_t nSpar3;
   DSL_uint8_t nSpar4;
   DSL_uint8_t nSpar5;
   DSL_boolean_t bCamFwReload;
   /**
   Indicates last showtime according to the EAPS timeout*/
   DSL_VRX_FwMode_t nLastShowtime;
   /**
   Indicates current showtime according to the EAPS timeout*/
   DSL_VRX_FwMode_t nCurrShowtime;
   /**
   Indicates permanent showtime according to the EASP timeout*/
   DSL_boolean_t bPermanentShowtime;
   /**
   Structure that stores information about a waiting function call and its
   received data */
   DSL_FctWaiting_t fctWait;
#ifdef INCLUDE_DSL_SYSTEM_INTERFACE
   DSL_boolean_t bSystemIfStatusValid;
   DSL_SystemInterfaceConfigData_t sysCIFSts;
   /**
   Backup configuration value for used TC-Layer. This value will be updated on
   configuration of the DSL FW and reflects therefore the FW configuration.
   Such backup is needed because timing optimized SytemInterfaceStatus event
   handling shall update/send according event ASAP after FULL_INIT is reached.
   For example in case of *no* auto TC is configured the update/event can be
   always generated directly after FULL_INIT.
   However at this point the FW configuration is relevant for a decision because
   the user configuration (via "sics") might have changed in the meantime, which
   would be only applied to the FW after autoboot restart and within next
   configuration phase. */
   DSL_TcLayerSelection_t nTcModeFwCfg;
#endif
   /** Defines the device port mode for the firmware request. */
   DSL_PortMode_t nPortMode;
} DSL_DEV_Data_t;

typedef enum
{
   /** No transmission*/
   DSL_CEOC_TX_IDLE = 0,
   /** Transmission in progress*/
   DSL_CEOC_TX_PROG = 1,
   /** Transmission error*/
   DSL_CEOC_TX_ERR  = 3
} DSL_VRX_ClearEocTxStatus_t;

typedef enum
{
   /** No transmission*/
   DSL_CEOC_RX_IDLE = 0,
   /** Reception in progress*/
   DSL_CEOC_RX_PROG = 1,
   /** Reception done. Data is available in RX buffer.*/
   DSL_CEOC_RX_DONE = 2,
   /** Reception error*/
   DSL_CEOC_RX_ERR  = 3
} DSL_VRX_ClearEocRxStatus_t;

typedef struct
{
   /** CEOC Tx Status*/
   DSL_VRX_ClearEocTxStatus_t tx;
   /** CEOC Rx Status*/
   DSL_VRX_ClearEocRxStatus_t rx;
} DSL_VRX_ClearEocStatus_t;

/**
   This structure defines a set of carrier bands
*/
typedef struct
{
   /**
   Number of used data elements */
   DSL_uint16_t nNumData;
   /**
   band definitions */
   DSL_Band_t nBand[DSL_MAX_NUMBER_OF_BANDS];
}DSL_BandList_t;

/** Value out of Q8.8 range. Means "no value". */
#define Q88_NOVALUE 0x8000

/**
   to supporte the first ADSL firmware some special treatments are necessary */
#define PRELIMINARY_ADSL_SUPPORT       1
#define ADSL_ALPHEUS_SUPPORT           0

/**
 * VRX Mailbox error codes */
#define D2H_ERROR_BKGD_TASK_WAITING    0xE1 /* 1110 0001b */
#define D2H_ERROR_GENERAL_FAILURE      0xE2 /* 1110 0010b */
#define D2H_ERROR_CMD_NOT_ALLOWED      0xE3 /* 1110 0011b */
#define D2H_ERROR_PARAMETER_WRONG      0xE4 /* 1110 0100b */
#define D2H_ERROR_RESOURCE_OCCUPIED    0xE5 /* 1110 0101b */
#define D2H_DBG_VISP_RUNNING           0xE6 /* 1110 0110b */
#define D2H_DBG_VISP_STOPPED           0xE7 /* 1110 0111b */
#define D2H_CMV_CURRENTLY_UNAVAILABLE  0x91


/**
   Calculate and return the lenght field of 16Bit Get/Set messages:
   16 bit length field = (sizeof(msg) - sizeof(index + length)) / 2
   This is valid for all messages of the PMD layer (16bit parameters)
   \note The "get" messages require the length of the ACK-msg in the length field */
#define DSL_VRX_16BIT_RD_MSG_LEN_GET(msgStruct)   ((sizeof(msgStruct) - 4) / 2)

/**
   Calculate and return the lenght field of 32Bit Get messages:
   32 bit length field = (sizeof(msg) - sizeof(index + length)) / 4
   This is valid for xTC "set" messages (32bit parameters) and "get" messages
   which contain not the channel parameter "LineNo" additionally */
#define DSL_VRX_32BIT_RD_MSG_LEN_GET(msgStruct)   ((sizeof(msgStruct) - 4) / 4)

/**
   Calculate and return the lenght field of 32Bit Channel Get messages:
   32 bit length field = (sizeof(msg) - sizeof(index + length + LineNo)) / 4
   This is valid for xTC "set" messages (32bit parameters) and "get" messages
   which contain the channel parameter "LineNo" additionally */
#define DSL_VRX_32BIT_CH_RD_MSG_LEN_GET(msgStruct) ((sizeof(msgStruct) - 8) / 4)

/**
   Calculate and return the lenght field of 32Bit Channel Set messages:
   This is the same as for the Get messages without channel */
#define DSL_VRX_32BIT_CH_WR_MSG_LEN_GET DSL_VRX_32BIT_RD_MSG_LEN_GET

/**
   Calculate and return the lenght field of 32Bit Set messages:
   This is the same as for the Get messages */
#define DSL_VRX_32BIT_WR_MSG_LEN_GET DSL_VRX_32BIT_RD_MSG_LEN_GET

/** Processes a given bit value of a firmware and maps it into the bit field
    value (and bit field mask) if the state of the bit has been changed since
    last occurence of the macro call.
    \param MSGBIT        Name of bit in "RSP_Line_Failures" message, [I]
    \param nPreBitField  Previous value of line failure bit mask from data
                         base, [I]
    \param nBitMask      Definition of line failure bit as defined in enum type
                         of DSL library, [I]
    \param nActBitField  Actual value of line failure bit mask from GMX-D
                         (mapped from MSGBIT).
                         This value will be generated by the macro, [O]
    \param nBitFieldMask Returns a bit mask that indicates which failure bits
                         has been toggled from previous to actual line failure
                         status. This value will be generated by the macro, [O]
*/
#ifdef INCLUDE_DSL_G997_ALARM
#define DSL_FAILUREBIT_SET(MSGBIT, nPreBitField, nBitMask, nActBitField, nBitFieldMask) \
   if (sAck.MSGBIT) \
   { \
      nActBitField |= nBitMask; \
   } \
   if ( (nActBitField & nBitMask) != (nPreBitField & nBitMask) ) \
   { \
      nBitFieldMask |= nBitMask; \
   }
#else
#define DSL_FAILUREBIT_SET(MSGBIT, nBitMask, nActBitField) \
   if (sAck.MSGBIT) \
   { \
      nActBitField |= nBitMask; \
   }
#endif /* INCLUDE_DSL_G997_ALARM*/

#if defined(DSL_CPE_SIMULATOR_DRIVER) && defined(WIN32)
#else
/*
   Low-level driver  access mapping
*/

#define DSL_DRV_VRX_InternalDevOpen            MEI_InternalDevOpen
#define DSL_DRV_VRX_InternalDevClose           MEI_InternalDevClose
#define DSL_DRV_VRX_InternalInitDevice         MEI_InternalInitDevice
#define DSL_DRV_VRX_InternalDrvVersionGet      MEI_InternalDrvVersionGet
#define DSL_DRV_VRX_InternalDevReset           MEI_InternalDevReset
#define DSL_DRV_VRX_InternalRequestConfig      MEI_InternalRequestConfig
#define DSL_DRV_VRX_InternalFirmwareDownload   MEI_InternalFirmwareDownload
#define DSL_DRV_VRX_InternalFwModeCtrlSet      MEI_InternalFwModeCtrlSet
#define DSL_DRV_VRX_InternalMsgSend            MEI_InternalMsgSend
#define DSL_DRV_VRX_InternalNfcEnable          MEI_InternalNfcEnable
#define DSL_DRV_VRX_InternalNfcMsgRead         MEI_InternalNfcMsgRead
#define DSL_DRV_VRX_InternalDebugLevelSet      MEI_InternalDebugLevelSet

#endif /* defined(DSL_CPE_SIMULATOR_DRIVER) && defined(WIN32)*/

/**
   This function sends the VRX firmware message
   "CMD_LineStatusPerBandUS/DS_Get" and waits for the answer to get the per
   band values for LATN, SATN and SNR.
   This information is necessary for the G.997.1 API function
   DSL_DEV_G997_LineStatusPerBandGet.
   Instead of the direct request here the values might be retrieved
   when reaching showtime and read from the database in this function.

   \param pContext   Pointer to dsl library context structure, [I]
   \param nDirection Specifies the direction (upstream/downstream) to which
                     the function will apply. [I]
   \param pData      Returns the paramter pData->SNR, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
   - or any other defined specific error code
*/
DSL_Error_t DSL_DRV_VRX_LineStatusPerBandGet(
   DSL_Context_t *pContext,
   DSL_AccessDir_t nDirection,
   DSL_G997_LineStatusPerBandData_t *pData);

/**
   This function sends the VRX firmware message
   "CMD_PBO_AELEM_Status_Get" and waits for the answer to get the per
   band values for nKlePb.
   This information is necessary for the API function
   DSL_DEV_G997_UsPowerBackOffStatusGet.

   \param pContext   Pointer to dsl library context structure, [I]
   \param pData      Returns the paramter pData->SNR, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
   - or any other defined specific error code
*/
DSL_Error_t DSL_DRV_VRX_UsPowerBackOffStatusGet(
   DSL_Context_t *pContext,
   DSL_G997_UsPowerBackOffStatusData_t *pData);

/**
   This function sends the VRX firmware message "CMD_LineStatusUS/DS_Get"
   and waits for the answer to get the values for SNR.
   This information is necessary for the G.997.1 API function
   DSL_DEV_G997_LineStatusGet.
   Instead of the direct request here the values might be retrieved
   when reaching showtime and read from the database in this function.

   \param pContext   Pointer to dsl library context structure, [I]
   \param nDirection Specifies the direction (upstream/downstream) to which
                     the function will apply. [I]
   \param pData      Returns the paramter pData->SNR, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
   - or any other defined specific error code
*/
DSL_Error_t DSL_DRV_VRX_LineStatusGet(
   DSL_Context_t *pContext,
   DSL_AccessDir_t nDirection,
   DSL_G997_LineStatusData_t * pData);

#ifdef INCLUDE_DSL_FRAMING_PARAMETERS
DSL_Error_t DSL_DRV_VRX_FramingParametersVdsl2StatusGet(
   DSL_Context_t *pContext,
   const DSL_uint8_t nChannel,
   const DSL_AccessDir_t nDir,
   DSL_FramingParameterStatusData_t *pData);

DSL_Error_t DSL_DRV_VRX_FramingParametersAdslStatusGet(
   DSL_Context_t *pContext,
   const DSL_uint8_t nChannel,
   const DSL_AccessDir_t nDir,
   DSL_FramingParameterStatusData_t *pData);
#endif /* #ifdef INCLUDE_DSL_FRAMING_PARAMETERS*/

DSL_Error_t DSL_DRV_VRX_DataPathFailuresStatusGet(
   DSL_Context_t *pContext);

DSL_Error_t DSL_DRV_VRX_LineFailuresStatusUpdate(
   DSL_Context_t *pContext);

#ifdef INCLUDE_DSL_G997_FRAMING_PARAMETERS
DSL_Error_t DSL_DRV_VRX_G997_FramingParameterStatusAdslGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN const DSL_AccessDir_t nDirection,
   DSL_IN const DSL_uint8_t nChannel,
   DSL_OUT DSL_G997_FramingParameterStatusData_t *pData);
#endif /* INCLUDE_DSL_G997_FRAMING_PARAMETERS*/

#ifdef INCLUDE_DSL_SYSTEM_INTERFACE
DSL_Error_t DSL_DRV_VRX_SystemInterfaceStatusUpdate(
   DSL_IN DSL_Context_t *pContext);
#endif

#ifdef INCLUDE_DSL_G997_FRAMING_PARAMETERS
DSL_Error_t DSL_DRV_VRX_G997_FramingParameterStatusVdsl2Get(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN const DSL_AccessDir_t nDirection,
   DSL_IN const DSL_uint8_t nChannel,
   DSL_OUT DSL_G997_FramingParameterStatusData_t *pData);
#endif /* INCLUDE_DSL_G997_FRAMING_PARAMETERS*/

#include "drv_dsl_cpe_msg_vrx.h"

DSL_Error_t DSL_DRV_VRX_ShowtimeMeasurementUpdate(
   DSL_Context_t *pContext,
   DSL_boolean_t bForceUpdate);

DSL_Error_t DSL_DRV_VRX_ShowtimeMeasurementCompleted(
   DSL_Context_t *pContext);

/**
   Returns the smaller of the two parameters.
   Take care to use compatible types!

   \param a first element to compare, [I]
   \param b second element to compare, [I]
*/
#define DSL_MIN(a,b) ((a)<(b)?(a):(b))

/**
   Returns the biggest of the two parameters.
   Take care to use compatible types!

   \param a first element to compare, [I]
   \param b second element to compare, [I]
*/
#define DSL_MAX(a,b) ((a)>(b)?(a):(b))

/**
   Number activation retries for the
   CPE Auto Mode (CAM) state machine. */
#define DSL_CAM_MAX_REINITS      3

/**
   FW Features for use inside the DSL CPE API
*/
typedef enum
{
   /**
      FW supports VDSL2 */
   DSL_VRX_FW_VDSL2,
   /**
      FW supports ADSL Annex A */
   DSL_VRX_FW_ADSLA,
   /**
      FW supports ADSL Annex B */
   DSL_VRX_FW_ADSLB,
   /**
      FW supports ADSL Annex A or Annex B */
   DSL_VRX_FW_ADSL
} DSL_VRX_XdslMode_t;

/**
   Defines the return types of version processing functions
   \ref DSL_DEV_CheckFirmwareVersion
   \ref DSL_DEV_CheckDeviceVersion
*/
typedef enum
{
   /** Version processing is not possible due to an error */
   DSL_VERSION_ERROR = 0,
   /** Version is not compared because of different common firmware types
       Currently this indicates a mismatch of the ApplicationType (Annex) only */
   DSL_VERSION_MISMATCH = 1,
   /** The version actually used is smaller than the version to check */
   DSL_VERSION_SMALLER = 2,
   /** The version actually used is equal to the version to check */
   DSL_VERSION_EQUAL = 3,
   /** The version actually used is bigger than the version to check */
   DSL_VERSION_BIGGER = 4,
   /** Delimiter only */
   DSL_VERSION_LAST = 5
} DSL_DEV_VersionCheck_t;

DSL_int_t DSL_DRV_VRX_DslModeIndexGet(
   DSL_Context_t *pContext);

DSL_boolean_t DSL_DRV_VRX_FirmwareXdslModeCheck(
   DSL_Context_t *pContext,
   const DSL_VRX_XdslMode_t nXdslMode);

DSL_Error_t DSL_DRV_VRX_CamFsmStateSet(
   DSL_Context_t *pContext,
   const DSL_DEV_CamStates_t nCamStateNew);

DSL_Error_t DSL_DRV_VRX_CamFsmEnable(
   DSL_Context_t *pContext,
   const DSL_boolean_t bEnable);

DSL_Error_t DSL_DRV_VRX_CamTrainingTimeoutHandle(
   DSL_Context_t *pContext);

DSL_DEV_CamStates_t DSL_DRV_VRX_CamFsmStateGet(
   DSL_Context_t *pContext);

DSL_Error_t DSL_DRV_VRX_CamFwLoadNeededCheck(
   DSL_Context_t *pContext,
   DSL_boolean_t *bDownloadRequired,
   DSL_VRX_FwMode_t *nFwMode);

DSL_Error_t DSL_DRV_VRX_CamExceptionHandle(
   DSL_Context_t *pContext);

DSL_Error_t DSL_DRV_VRX_CamFsmUpdate(
   DSL_Context_t *pContext);

/**
   This function returns VDSL2 actual bandplan/profile information.

   \param pContext Pointer to dsl cpe library context structure, [I]
   \param pData    Returns the actual bandplan/profile information, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_BandPlanStatusGet(
   DSL_Context_t *pContext,
   DSL_BandPlanConfigData_t *pData);

DSL_Error_t DSL_DRV_VRX_CheckTruncParamRange(
   DSL_Context_t *pContext,
   DSL_int_t nMin,
   DSL_int_t nMax,
   DSL_int_t *pVal,
   DSL_char_t *pName);

#ifdef INCLUDE_DSL_DELT
DSL_Error_t DSL_DRV_VRX_DiagnosticDeltDataGet(
   DSL_Context_t *pContext);

/**
   This function calculates the SNR per subcarrier group from the
   SNR per subcarrier values for a given direction. For each subcarrier group
   an average is built.

   \param pContext   Pointer to dsl library context structure, [I]
   \param nLine      Specifies for which line the function will apply, [I]
   \param nDirection Specifies the direction (upstream/downstream) to which
                     the function will apply. [I]
   \param pData      Returns the SNR data , [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
   - or any other defined specific error code
*/
DSL_Error_t DSL_DRV_VRX_SnrPerGroupCalculate(
   DSL_Context_t *pContext,
   DSL_uint8_t nGroupSize,
   DSL_G997_NSCData8_t *pData);
#endif /* INCLUDE_DSL_DELT*/

DSL_Error_t DSL_DRV_VRX_ChannelStatusGet(
   DSL_Context_t *pContext,
   DSL_uint8_t nChannel,
   DSL_AccessDir_t nDirection,
   DSL_G997_ChannelStatusData_t *pData);

/*
   This function set the actual power management mode in the data base and also
   handles to set the previous values if necessary.
   If necessary a callback event will be processed to inform the upper layer
   software about data power management state change.

   \param pContext   Pointer to dsl library context structure, [I]
   \param nLine      Specifies on which line the message was received, [I]
   \param pData      Includes the the new power management mode, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_PowerManagementStatusSet(
   DSL_Context_t *pContext,
   const DSL_G997_PowerManagement_t nPMMode);

DSL_Error_t DSL_DRV_VRX_DataRateUpdate(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDir,
   DSL_uint32_t dataRate);

DSL_Error_t DSL_DRV_VRX_FailReasonGet(
   DSL_Context_t *pContext);

DSL_Error_t DSL_DRV_VRX_XtseConfigure(
   DSL_Context_t *pContext);

#ifdef INCLUDE_DSL_G997_LINE_INVENTORY
DSL_Error_t DSL_DRV_VRX_OnLineInventoryFe(
   DSL_Context_t *pContext);
#endif /* #ifdef INCLUDE_DSL_G997_LINE_INVENTORY*/

/*
   This function returns the support state of a bandplan/profile
   combination.

   \param pContext Pointer to dsl cpe library context structure, [I]
   \param pData    Defines the bandplan from
                   \ref DSL_BandPlanConfigData_t, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_TRUE (1) if supported
   - DSL_FALSE (0) if not supported
*/
DSL_boolean_t DSL_DRV_VRX_BandPlanSupportedCheck(
   DSL_Context_t *pContext,
   const DSL_BandPlanConfigData_t *pData);

/**
   This function returns all VDSL2 bandplan/profile combinations
   supported by the device.

   \param pContext Pointer to dsl cpe library context structure, [I]
   \param pData    Returns the support matrix, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_BandPlanSupportedGet(
   DSL_Context_t *pContext,
   DSL_BandPlanSupportData_t *pData);

/** TBD
*/
DSL_Error_t DSL_DRV_VRX_FirmwareVersionCheck(
   DSL_Context_t *pContext,
   DSL_int_t nFs,
   DSL_int_t nMajVer,
   DSL_int_t nMinVer,
   DSL_int_t nRs,
   DSL_int_t nApp,
   DSL_DEV_VersionCheck_t *pVerCheck);

/* ============================================================================
   Definition of the MESSAGE OPCODES.
   ========================================================================= */
/*
   NOTE:
      The MESSAGE OPCODE is a combination of the function fields:
      OPCODE, MSG MODE and DIRECTION
*/

/* CMV Message Codes: (WinHost-to-)ME-to-ARC */
#define DSL_DRV_VRX_H2D_CMV_READ                0x00
#define DSL_DRV_VRX_H2D_CMV_WRITE               0x04

/* CMV Message Codes: ARC-to-ME(-to-WinHost) */
#define DSL_DRV_VRX_D2H_CMV_READ_REPLY                0x01
#define DSL_DRV_VRX_D2H_CMV_WRITE_REPLY               0x05
#define DSL_DRV_VRX_D2H_CMV_INDICATE                  0x11
#define DSL_DRV_VRX_D2H_ERROR_OPCODE_UNKNOWN          0x21
#define DSL_DRV_VRX_D2H_ERROR_CMV_UNKNOWN             0x31
#define DSL_DRV_VRX_D2H_ERROR_CMV_READ_NOT_AVAILABLE  0x41
#define DSL_DRV_VRX_D2H_ERROR_CMV_WRITE_ONLY          0x51
#define DSL_DRV_VRX_D2H_ERROR_CMV_READ_ONLY           0x61
#define DSL_DRV_VRX_D2H_ERROR_CMV_UNINITIALIZED_ENTRY 0x71
#define DSL_DRV_VRX_D2H_ERROR_ILLEGAL_PAYLOAD_SIZE    0x81
#define DSL_DRV_VRX_D2H_ERROR_MSG_TEMP_NOT_AVAIL      0x91


/* Peek/Poke Message Codes: (WinHost-to-)ME-to-ARC */
#define DSL_DRV_VRX_H2D_DEBUG_READ_DM           0x02
#define DSL_DRV_VRX_H2D_DEBUG_READ_PM           0x06
#define DSL_DRV_VRX_H2D_DEBUG_WRITE_DM          0x0A
#define DSL_DRV_VRX_H2D_DEBUG_WRITE_PM          0x0E


/* Peek/Poke Message Codes: ARC-to-ME(-to-WinHost) */
#define DSL_DRV_VRX_D2H_DEBUG_READ_DM_REPLY     0x03
#define DSL_DRV_VRX_D2H_DEBUG_READ_PM_REPLY     0x07
#define DSL_DRV_VRX_D2H_DEBUG_WRITE_DM_REPLY    0x0B
#define DSL_DRV_VRX_D2H_DEBUG_WRITE_PM_REPLY    0x0F
#define DSL_DRV_VRX_D2H_ERROR_ADDR_UNKNOWN      0x33

/* Modem Ready Message Codes: ARC-to-ME */
#define DSL_DRV_VRX_D2DCE_AUTONOMOUS_MODEM_READY         0xF1
#define DSL_DRV_VRX_RESULT_AUTO_MODEM_READY_SUCCESS      0x0
#define DSL_DRV_VRX_RESULT_AUTO_MODEM_READY_ERROR_AC_CLK 0x101

/* Device functional opcodes */
#define DSL_DRV_VRX_D2H_ERROR_BKGD_TASK_WAITING       0xE1 /* 1110 0001b */
#define DSL_DRV_VRX_D2H_ERROR_GENERAL_FAILURE         0xE2 /* 1110 0010b */
#define DSL_DRV_VRX_D2H_ERROR_CMD_NOT_ALLOWED         0xE3 /* 1110 0011b */
#define DSL_DRV_VRX_D2H_ERROR_PARAMETER_WRONG         0xE4 /* 1110 0100b */
#define DSL_DRV_VRX_D2H_ERROR_RESOURCE_OCCUPIED       0xE5 /* 1110 0101b */
#define DSL_DRV_VRX_D2H_DBG_VISP_RUNNING              0xE6 /* 1110 0110b */
#define DSL_DRV_VRX_D2H_DBG_VISP_STOPPED              0xE7 /* 1110 0111b */

/* CMD_TestParamsFE_Request block size */
#define VRX_TESTPARAMS_FE_BLOCK_SIZE    60

/* Timeout for periodically update (in ms) */
#define VRX_TESTPARAMS_FE_UPDATE_PERIOD 30000

/** Validity status of data for CMD_TestParamsFE_Request */
typedef enum
{
   /** data is not initialized */
   VRX_FE_TESTPARAMS_NOT_INIT       = 0,
   /** first updating in progress (on first showtime entry) */
   VRX_FE_TESTPARAMS_FIRST_UPDATING = 1,
   /** periodically in progress (backgroung) */
   VRX_FE_TESTPARAMS_UPDATING       = 2,
   /** data is ready */
   VRX_FE_TESTPARAMS_READY          = 3,
} DSL_VRX_TestParams_Fe_Status_t;

/** @} DRV_DSL_DEVICE */

#ifdef __cplusplus
}
#endif

#endif
