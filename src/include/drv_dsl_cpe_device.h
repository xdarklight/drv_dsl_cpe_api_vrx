/******************************************************************************

                              Copyright (c) 2014
                            Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef _DRV_DSL_CPE_DEVICE_H
#define _DRV_DSL_CPE_DEVICE_H

#ifdef __cplusplus
   extern "C" {
#endif

#ifndef SWIG
/** \addtogroup DRV_DSL_DEVICE
 @{ */

#if defined (INCLUDE_DSL_CPE_API_DANUBE)
   #include "drv_dsl_cpe_device_danube.h"
#elif defined (INCLUDE_DSL_CPE_API_VRX)
   #include "drv_dsl_cpe_device_vrx.h"
#else
   #error "Device is not defined!"
#endif

/** Number of retries to enter L3 state*/
#define DSL_LX_TO_L3_ATTEMPT_COUNT   3

/** L3 request timeout in ms*/
#define DSL_LX_TO_L3_TIMEOUT     2000
/** Wait time for L3 status request interval in ms */
#define DSL_L3_WAIT_INTERVAL     100
/** Offset of TxRetransmitted parameter in HDLC responce */
#define DSL_TX_RETRANSMITTED_OFFSET 18

typedef DSL_DEV_Device_t* DSL_DEV_Handle_t;

/** This structure represents Device context structure */
typedef struct DSL_DEV_Context
{
   /** Handle to the corresponding low level driver */
   DSL_DEV_Handle_t lowHandle;
#if defined(INCLUDE_DSL_CPE_API_VRX)
   /** Handle to the corresponding low level driver for the autonomous messages processing*/
   DSL_DEV_Handle_t nfc_lowHandle;
#endif /* defined(INCLUDE_DSL_CPE_API_VRX)*/
   /** Specifies a usage count of this device  */
   DSL_uint32_t nUsageCount;

   /** Link to the DSL_Context structure instance */
   DSL_Context_t *pContext;

   /** Open Context List mutex */
   DSL_DRV_Mutex_t openContextListMutex;

   /** Number of device */
   DSL_int_t nNum;

   /** Firmware loaded flag */
   DSL_boolean_t bFirmwareReady;

   /** Whether this device will be initialized at the first time */
   DSL_boolean_t bFirstPowerOn;
   /** Internal structure for each device implementation */
   DSL_DEV_Data_t data;
} DSL_DEV_Context_t;

typedef DSL_DEV_Context_t DSL_devCtx_t;

extern DSL_devCtx_t ifxDevices[DSL_DRV_MAX_ENTITIES];

DSL_DEV_Handle_t DSL_DRV_DEV_DriverHandleGet(
   DSL_int_t nMaj,
   DSL_int_t nNum
);

DSL_Error_t DSL_DRV_DEV_DriverHandleDelete(
   DSL_DEV_Handle_t handle
);

#if defined (INCLUDE_DSL_CPE_API_DANUBE)
DSL_Error_t DSL_DRV_DEV_Reboot(DSL_Context_t *pContext);
#endif

DSL_Error_t DSL_DRV_DEV_FwDownload
(
   DSL_Context_t *pContext,
   const DSL_char_t *pFw1,
   DSL_uint32_t nSize1,
   const DSL_char_t *pFw2,
   DSL_uint32_t nSize2,
   DSL_int32_t *pLoff,
   DSL_int32_t *pCurrentOff,
   DSL_boolean_t bLastChunk
);

/** This function should be called to perform low level showtime entry/exit
    handling */
DSL_Error_t DSL_DRV_DEV_MeiShowtimeSignaling
(
   DSL_Context_t *pContext,
   DSL_boolean_t bShowtimeEntry
);


DSL_boolean_t DSL_DRV_DEV_ModemIsReady(DSL_Context_t *pContext);

DSL_Error_t DSL_DRV_DEV_ModemWriteConfig(
   DSL_Context_t *pContext);

/** Device dependent handling of L3 power state */
DSL_Error_t DSL_DRV_DEV_AutobootHandleL3(
   DSL_Context_t *pContext,
   DSL_boolean_t bL3Forced
);

/** Device dependent part of DSL_DRV_AutobootHandleStart() */
DSL_Error_t DSL_DRV_DEV_AutobootHandleStart(
   DSL_Context_t *pContext,
   DSL_boolean_t bLoopTest,
   DSL_boolean_t bShortInit
);

#if defined(INCLUDE_DSL_CPE_API_VRX)
/** Device dependent part of DSL_AutobootHandleFwRequest() */
DSL_Error_t DSL_DRV_DEV_AutobootHandleFwRequest
(
   DSL_Context_t *pContext
);
#endif /* defined(INCLUDE_DSL_CPE_API_VRX)*/

/** Device dependent part of DSL_AutobootHandleTraining() */
DSL_Error_t DSL_DRV_DEV_AutobootHandleTraining(
   DSL_Context_t *pContext
);

/** Device dependent part of DSL_AutobootHandleRestart() */
DSL_Error_t DSL_DRV_DEV_AutobootHandleRestart(
   DSL_Context_t *pContext
);

/** Device dependent part of DSL_AutobootHandleException() */
DSL_Error_t DSL_DRV_DEV_AutobootHandleException(
   DSL_Context_t *pContext
);

/**
   This routine is a device specific implementation of
   a device initialization

   \param   pContext       pointer to the DSL context
   \param   pInit          pointer to the DSL_Init_t structure
*/

#ifndef SWIG

DSL_Error_t DSL_DRV_DEV_DeviceInit(
   DSL_Context_t *pContext,
   DSL_Init_t *pData
);

#endif

/**
   This routine resets a DSL line

   \param pContext
      pointer to the DSL context
*/

#ifndef SWIG

DSL_Error_t DSL_DRV_DEV_LinkReset(
   DSL_Context_t *pContext
);

#endif

/**
   This routine freezes a DSL line

   \param pContext
      pointer to the DSL context

   \param pXTSE
      pointer to the last XTSE configuration status
*/

#ifndef SWIG

DSL_Error_t DSL_DRV_DEV_LinkFreeze(
   DSL_Context_t *pContext,
   DSL_uint8_t *pXTSE
);

#endif

DSL_Error_t DSL_DRV_DEV_XtseSettingsCheck(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN DSL_uint8_t *pXTSE);

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
#ifndef SWIG
DSL_Error_t DSL_DRV_DEV_LinePathCounterTotalGet(
   DSL_Context_t *pContext,
   const DSL_uint8_t nChannel,
   DSL_LinePathCounterTotalData_t *pData
);
#endif
#endif /** #if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)*/

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
#ifndef SWIG
DSL_Error_t DSL_DRV_DEV_DataPathCounterTotalGet(
   DSL_Context_t *pContext,
   const DSL_uint8_t nChannel,
   DSL_DataPathCounterTotalData_t *pData
);
#endif
#endif /** #if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)*/

#ifndef SWIG
DSL_Error_t DSL_DRV_DEV_LinkActivate(
   DSL_Context_t *pContext
);
#endif

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_LOW_LEVEL_CONFIGURATION_SET
*/
#ifndef SWIG
DSL_Error_t DSL_DRV_DEV_LowLevelConfigurationSet(
   DSL_Context_t *pContext,
   DSL_DeviceLowLevelConfig_t *pData);
#endif

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_SYSTEM_INTERFACE_CONFIG_SET
*/
#ifdef INCLUDE_DSL_SYSTEM_INTERFACE
#ifndef SWIG
DSL_Error_t DSL_DRV_DEV_SystemInterfaceConfigSet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_SystemInterfaceConfig_t *pData
);
#endif
#endif /* INCLUDE_DSL_SYSTEM_INTERFACE*/

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_SYSTEM_INTERFACE_CONFIG_GET
*/
#ifdef INCLUDE_DSL_SYSTEM_INTERFACE
#ifndef SWIG
DSL_Error_t DSL_DRV_DEV_SystemInterfaceConfigGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_SystemInterfaceConfig_t *pData
);
#endif
#endif /* INCLUDE_DSL_SYSTEM_INTERFACE*/

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_SYSTEM_INTERFACE_STATUS_GET
*/
#ifdef INCLUDE_DSL_SYSTEM_INTERFACE
#ifndef SWIG
DSL_Error_t DSL_DRV_DEV_SystemInterfaceStatusGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_SystemInterfaceStatus_t *pData
);
#endif
#endif /* INCLUDE_DSL_SYSTEM_INTERFACE*/

/**
   This function requests the pilot tones used in Showtime.

   \param pContext   Pointer to dsl library context structure, [I]
   \param pData      Pointer to the Pilot Tines Status data, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
   - or any other defined specific error code
*/
#ifdef INCLUDE_PILOT_TONES_STATUS
DSL_Error_t DSL_DRV_DEV_PilotTonesStatusGet(
   DSL_Context_t *pContext,
   DSL_PilotTonesStatus_t *pData);
#endif /* #ifdef INCLUDE_PILOT_TONES_STATUS*/

DSL_Error_t DSL_DRV_DEV_LoopLengthStatusGet(
   DSL_Context_t *pContext);

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
#ifdef INCLUDE_DSL_G997_LINE_INVENTORY
DSL_Error_t DSL_DRV_DEV_AuxLineInventoryGet(
   DSL_Context_t *pContext,
   DSL_AuxLineInventory_t *pData);
#endif /* INCLUDE_DSL_G997_LINE_INVENTORY*/

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_UTOPIA_ADDRESS_CONFIG_SET
*/
#ifndef SWIG
DSL_Error_t DSL_DRV_DEV_UtopiaAddressConfigSet(
   DSL_Context_t *pContext,
   DSL_uint8_t nChannel,
   DSL_PhyAddressConfigData_t *pData);
#endif

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_UTOPIA_ADDRESS_CONFIG_GET
*/
#ifdef INCLUDE_DSL_CONFIG_GET
#ifndef SWIG
DSL_Error_t DSL_DRV_DEV_UtopiaAddressConfigGet(
   DSL_Context_t *pContext,
   DSL_uint8_t nChannel,
   DSL_PhyAddressConfigData_t *pData);
#endif
#endif /* INCLUDE_DSL_CONFIG_GET*/
#endif /* #if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)*/

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_UTOPIA_BUS_WIDTH_CONFIG_SET
*/
#ifndef SWIG
DSL_Error_t DSL_DRV_DEV_UtopiaBusWidthConfigSet(
   DSL_Context_t *pContext,
   DSL_UtopiaBusWidthConfigData_t *pData
);
#endif

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_UTOPIA_BUS_WIDTH_CONFIG_GET
*/
#ifdef INCLUDE_DSL_CONFIG_GET
#ifndef SWIG
DSL_Error_t DSL_DRV_DEV_UtopiaBusWidthConfigGet(
   DSL_Context_t *pContext,
   DSL_UtopiaBusWidthConfigData_t *pData
);
#endif
#endif /* INCLUDE_DSL_CONFIG_GET*/
#endif /* #if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)*/

/**
   This routine is a device specific implementation of
   the DSL_DRV_InitDataPrepare() function

   \param   bIsInKernel    where from the initial call is performed
   \param   pInit          pointer to the DSL_Init_t structure
*/

#ifndef SWIG
#if defined(INCLUDE_DSL_CPE_API_VRX)
DSL_Error_t DSL_DRV_DEV_InitDataPrepare(
   DSL_Context_t *pContext,
   DSL_boolean_t bIsInKernel,
   DSL_Init_t *pInit
);
#endif /* defined(INCLUDE_DSL_CPE_API_VRX)*/
#endif

/**
   This routine is a device specific implementation of
   the DSL_DRV_InitDataFree() function

   \param   pInit          pointer to the DSL_Init_t structure
*/

#ifndef SWIG
#if defined(INCLUDE_DSL_CPE_API_VRX)
DSL_void_t DSL_DRV_DEV_InitDataFree(
   DSL_Init_t *pInit
);
#endif /* defined(INCLUDE_DSL_CPE_API_VRX)*/
#endif

/**
   Updates G994 Vendor Id with FW version values

   \param pContext      Pointer to dsl library context structure, [I]

   \return  Return whether the feature is supported by the FW or not
*/
#ifndef SWIG
DSL_Error_t DSL_DRV_DEV_G994VendorIdFirmwareUpdate(
   DSL_Context_t *pContext
);
#endif

/**
   This routine calls low level driver ioctl handler

   \param pContext
      pointer to the DSL context
*/

#ifndef SWIG
DSL_Error_t DSL_DRV_DEV_BspIoctl(
   DSL_Context_t *pContext,
   DSL_uint_t nCommand,
   DSL_uint32_t *nArg
);
#endif

/**
   Device implementation of firmware version string retreive routine

   \param pContext    Pointer to dsl library context structure, [I]
   \param pString     Pointer to FW version string, [O]

   \return  Return values are defined within the \ref DSL_Error_t definition
    - DSL_SUCCESS in case of success
    - DSL_ERROR if operation failed
    - or any other defined specific error code

   \ingroup DRV_DSL_CPE_DEVICE
*/

#ifndef SWIG
DSL_Error_t DSL_DRV_DEV_FirmwareVersionGet(
   DSL_Context_t *pContext,
   DSL_char_t    *pString);
#endif

/**
   Device implementation of chip version string retreive routine

   \param pContext    Pointer to dsl library context structure, [I]
   \param pString     Pointer to Chip HW version string, [O]

   \return  Return values are defined within the \ref DSL_Error_t definition
    - DSL_SUCCESS in case of success
    - DSL_ERROR if operation failed
    - or any other defined specific error code

   \ingroup DRV_DSL_CPE_DEVICE
*/

#ifndef SWIG
DSL_Error_t DSL_DRV_DEV_ChipHardwareVersionGet(
   DSL_Context_t *pContext,
   DSL_char_t    *pString);
#endif

/**
   Device implementation of chip type string retreive routine

   \param pContext    Pointer to dsl library context structure, [I]
   \param pString     Pointer to Chip Type string, [O]

   \return  Return values are defined within the \ref DSL_Error_t definition
    - DSL_SUCCESS in case of success
    - DSL_ERROR if operation failed
    - or any other defined specific error code

   \ingroup DRV_DSL_CPE_DEVICE
*/
#ifndef SWIG
DSL_Error_t DSL_DRV_DEV_ChipSetTypeGet(
   DSL_Context_t *pContext,
   DSL_char_t    *pString);
#endif

/**
   Device implementation of chip name string retreive routine

   \param pContext    Pointer to dsl library context structure, [I]
   \param pString     Pointer to Device Driver version string, [O]

   \return  Return values are defined within the \ref DSL_Error_t definition
    - DSL_SUCCESS in case of success
    - DSL_ERROR if operation failed
    - or any other defined specific error code

   \ingroup DRV_DSL_CPE_DEVICE
*/

#ifndef SWIG
DSL_Error_t DSL_DRV_DEV_DriverVersionGet(
   DSL_Context_t *pContext,
   DSL_char_t    *pString);
#endif

/**
   Device implementation of line state getting routine

   \param pContext      Pointer to dsl library context structure, [I]
   \param pnLineState   Reference to value of type LineStateValue_t where
                        current line state should be placed

   \return  Return values are defined within the \ref DSL_Error_t definition
    - DSL_SUCCESS in case of success
    - DSL_ERROR if operation failed
    - or any other defined specific error code

   \ingroup DRV_DSL_CPE_DEVICE
*/

#ifndef SWIG

DSL_Error_t DSL_DRV_DEV_LineStateGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_LineStateValue_t *pnLineState
);

#endif

/** This function retrieves different information about line in showtime state
    and updates DSL context structure with actual values if needed */
DSL_Error_t DSL_DRV_DEV_ShowtimeStatusUpdate(
   DSL_Context_t *pContext,
   DSL_boolean_t bInit
);

/**
   Device implementation of DSL_DRV_FramingParameterStatusGet() routine

   \param pContext      Pointer to dsl library context structure, [I]
   \param pData         Reference to room where framing parameters should be
                        saved

   \return  Return values are defined within the \ref DSL_Error_t definition
    - DSL_SUCCESS in case of success
    - DSL_ERROR if operation failed
    - or any other defined specific error code

   \ingroup DRV_DSL_CPE_DEVICE
*/
#ifdef INCLUDE_DSL_FRAMING_PARAMETERS
DSL_Error_t DSL_DRV_DEV_FramingParameterStatusGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_FramingParameterStatus_t *pData
);
#endif /* INCLUDE_DSL_FRAMING_PARAMETERS*/

/**
   Send device low-level message*/
#ifndef SWIG
DSL_Error_t DSL_DRV_DEV_DBG_DeviceMessageSend(
   DSL_Context_t *pContext,
   DSL_DeviceMessageData_t *pMsg);
#endif


#ifndef SWIG
DSL_Error_t DSL_DRV_DEV_OnTimeoutEvent(
   DSL_Context_t *pContext,
   DSL_int_t nEventType,
   DSL_uint32_t nTimeoutID);
#endif

/**
   This enum represents the action to be performed by autoboot thread
   after showtime status is updated
*/
typedef enum DSL_DEV_RebootCheckResult
{
   DSL_DEV_REBOOT_UNHANDLED = 0,
   DSL_DEV_REBOOT_PENDING = 1,
   DSL_DEV_REBOOT_NO_ACTION = 2
} DSL_DEV_RebootCheckResult_t;


/**
   This function sets test mode settings according to the actually used
   ADSL/VDSL mode
*/
DSL_Error_t DSL_DRV_DEV_TestModeControlSet
(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_TestModeControl_t *pData
);


#ifdef INCLUDE_DSL_RESOURCE_STATISTICS
DSL_Error_t DSL_DRV_DEV_ResourceUsageStatisticsGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_OUT DSL_ResourceUsageStatisticsData_t *pData);
#endif /* INCLUDE_DSL_RESOURCE_STATISTICS*/

#ifdef INCLUDE_DSL_CPE_MISC_LINE_STATUS
DSL_Error_t DSL_DRV_DEV_BandBorderStatusGet(
   DSL_Context_t *pContext,
   DSL_BandBorderStatus_t *pData);
#endif /* INCLUDE_DSL_CPE_MISC_LINE_STATUS*/

#ifdef INCLUDE_DSL_CPE_MISC_LINE_STATUS
/*
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_MISC_LINE_STATUS_GET
*/
DSL_Error_t DSL_DRV_DEV_MiscLineStatusGet(
   DSL_Context_t *pContext,
   DSL_MiscLineStatus_t *pData);
#endif /* INCLUDE_DSL_CPE_MISC_LINE_STATUS*/

#if defined(INCLUDE_DSL_CPE_MISC_LINE_STATUS) || defined(INCLUDE_DSL_CPE_API_DANUBE)
DSL_Error_t DSL_DRV_DEV_Annex_M_J_UsBandBordersStatusGet(
   DSL_Context_t *pContext,
   DSL_Band_t *pData);
#endif /* defined(INCLUDE_DSL_CPE_MISC_LINE_STATUS) || defined(INCLUDE_DSL_CPE_API_DANUBE)*/

#ifndef SWIG
DSL_Error_t DSL_DRV_DEV_TrainingTimeoutSet(
   DSL_IN DSL_Context_t *pContext);
#endif

/*
   This function saves retransmission related counters
   into driver context.
*/
#ifndef SWIG
DSL_Error_t DSL_DRV_DEV_ReTxCountersSave(
   DSL_Context_t *pContext);
#endif

#ifndef SWIG
DSL_Error_t DSL_DRV_DEV_RetxStatisticsGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_ReTxStatistics_t *pData);
#endif

#if defined(INCLUDE_DSL_CPE_API_DANUBE)

#if defined(INCLUDE_REAL_TIME_TRACE)
#ifndef SWIG
DSL_Error_t DSL_DRV_DEV_RTT_StatusUpdate(
   DSL_IN DSL_Context_t *pContext);
#endif
#ifndef SWIG
DSL_Error_t DSL_DRV_DEV_RTT_Init(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN DSL_RTT_InitData_t *pData);
#endif
#ifndef SWIG
DSL_Error_t DSL_DRV_DEV_RTT_ConfigSet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN DSL_RTT_ConfigData_t *pData);
#endif
#ifndef SWIG
DSL_Error_t DSL_DRV_DEV_RTT_ConfigGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_RTT_ConfigData_t *pData);
#endif
#ifndef SWIG
DSL_Error_t DSL_DRV_DEV_RTT_ControlSet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN DSL_RTT_ControlData_t *pData);
#endif
#ifndef SWIG
DSL_Error_t DSL_DRV_DEV_RTT_StatisticsGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_RTT_StatisticsData_t *pData);
#endif
#ifndef SWIG
DSL_Error_t DSL_DRV_DEV_RTT_Start(
   DSL_IN DSL_Context_t *pContext);
#endif

#endif /*#if defined(INCLUDE_REAL_TIME_TRACE)*/
#endif /*#if defined(INCLUDE_DSL_CPE_API_DANUBE)*/

#ifndef SWIG
DSL_Error_t DSL_DRV_DEV_HybridTypeGet(
   DSL_Context_t *pContext,
   DSL_HybridType_t *pHybrid);
#endif

#ifndef SWIG

DSL_Error_t DSL_DRV_DEV_ChannelsStatusUpdate(
   DSL_Context_t *pContext
);

/**
   Firmware feature check routine

   \param pContext      Pointer to dsl library context structure, [I]
   \param nFeature      Feature to check, [I]

   \return  Return whether the feature is supported by the FW or not
*/
#ifndef SWIG
#if defined(INCLUDE_DSL_CPE_API_VRX)
DSL_boolean_t DSL_DRV_DEV_FirmwareFeatureCheck(
   DSL_Context_t *pContext,
   DSL_FirmwareXdslFeature_t nFeature
);
#endif /* defined(INCLUDE_DSL_CPE_API_VRX)*/
#endif

DSL_Error_t DSL_DRV_DEV_OlrStatisticsGet(
   DSL_Context_t *pContext,
   DSL_OUT DSL_OlrStatistics_t *pData
);

#ifdef INCLUDE_DSL_FILTER_DETECTION
DSL_Error_t DSL_DRV_FilterDetectionInit(
   DSL_Context_t *pContext
);
#endif /* INCLUDE_DSL_FILTER_DETECTION */

DSL_Error_t DSL_DRV_HybridSelectionInit(
   DSL_Context_t *pContext
);

DSL_Error_t DSL_DRV_HybridContextReset(
   DSL_Context_t *pContext
);

DSL_Error_t DSL_DRV_HybridContextInit(
   DSL_Context_t *pContext
);

#endif
/** @} DRV_DSL_DEVICE */

#endif /* #ifndef SWIG*/

#ifdef __cplusplus
}
#endif

#endif /* _DRV_DSL_CPE_DEVICE_H */
