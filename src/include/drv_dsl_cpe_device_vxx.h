/******************************************************************************

                               Copyright (c) 2011
                            Lantiq Deutschland GmbH
                     Am Campeon 3; 85579 Neubiberg, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef _DRV_DSL_CPE_DEVICE_VXX_H
#define _DRV_DSL_CPE_DEVICE_VXX_H

#ifdef __cplusplus
   extern "C" {
#endif

/** \addtogroup DRV_DSL_DEVICE
 @{ */

#include "drv_dsl_cpe_api.h"

/**
   Returns the smaller of the two parameters.
   Take care to use compatible types!

   \param a first element to compare, [I]
   \param b second element to compare, [I]
*/
#define DSL_MIN(a,b) ((a)<(b)?(a):(b))

/**
   Number activation retries for the
   CPE Auto Mode (CAM) state machine. */
#define DSL_CAM_MAX_REINITS      3

/**
   States for the CPE Auto Mode (CAM) state machine
   to toggle between ADSL and VDSL activations */
typedef enum
{
   /** CPE Automode feature is not used */
   DSL_CAM_DISABLED = 0,
   /** CPE Automode feature initialization state */
   DSL_CAM_INIT = 1,
   /** CPE Automode feature restricted to VDSL */
   DSL_CAM_VDSL_FORCED = 2,
   /** CPE Automode feature multi mode */
   DSL_CAM_MULTI = 3,
   /** CPE Automode feature VDSL only mode with retries */
   DSL_CAM_VDSL_RETRY = 4,
   /** CPE Automode feature ADSL only mode with retries */
   DSL_CAM_ADSL_RETRY = 5,
   /** CPE Automode delimiter */
   DSL_CAM_LAST = DSL_CAM_ADSL_RETRY
} DSL_DEV_CamStates_t;

/**
   FW Features for use inside the DSL CPE API
*/
typedef enum
{
   /**
      FW supports VDSL2 */
   DSL_VXX_FW_VDSL2,
   /**
      FW supports ADSL Annex A */
   DSL_VXX_FW_ADSLA,
   /**
      FW supports ADSL Annex B */
   DSL_VXX_FW_ADSLB,
   /**
      FW supports ADSL Annex A or Annex B */
   DSL_VXX_FW_ADSL
} DSL_VXX_Feature_t;

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

DSL_boolean_t DSL_DRV_VXX_FwFeatureCheck(
   DSL_Context_t *pContext,
   const DSL_VXX_Feature_t nFeature);

DSL_Error_t DSL_DRV_VXX_CamFsmStateSet(
   DSL_Context_t *pContext,
   const DSL_DEV_CamStates_t nCamStateNew);

DSL_Error_t DSL_DRV_VXX_CamFsmEnable(
   DSL_Context_t *pContext,
   const DSL_boolean_t bEnable);

DSL_Error_t DSL_DRV_VXX_CamTrainingTimeoutHandle(
   DSL_Context_t *pContext);

DSL_DEV_CamStates_t DSL_DRV_VXX_CamFsmStateGet(
   DSL_Context_t *pContext);

DSL_Error_t DSL_DRV_VXX_CamFwLoadNeededCheck(
   DSL_Context_t *pContext,
   DSL_boolean_t *bDownloadRequired,
   DSL_VNX_FwMode_t *nFwMode);

DSL_Error_t DSL_DRV_VXX_CamExceptionHandle(
   DSL_Context_t *pContext);

DSL_Error_t DSL_DRV_VXX_CamFsmUpdate(
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
DSL_Error_t DSL_DRV_VXX_BandPlanStatusGet(
   DSL_Context_t *pContext,
   DSL_BandPlanConfigData_t *pData);

DSL_Error_t DSL_DRV_VXX_CheckTruncParamRange(
   DSL_Context_t *pContext,
   DSL_int_t nMin,
   DSL_int_t nMax,
   DSL_int_t *pVal,
   DSL_char_t *pName);

#ifdef INCLUDE_DSL_DELT
DSL_Error_t DSL_DRV_VXX_DiagnosticDeltDataGet(
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
DSL_Error_t DSL_DRV_VXX_SnrPerGroupCalculate(
   DSL_Context_t *pContext,
   DSL_uint8_t nGroupSize,
   DSL_G997_NSCData8_t *pData);
#endif /* INCLUDE_DSL_DELT*/

DSL_Error_t DSL_DRV_VXX_ChannelStatusGet(
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
DSL_Error_t DSL_DRV_VXX_PowerManagementStatusSet(
   DSL_Context_t *pContext,
   const DSL_G997_PowerManagement_t nPMMode);

DSL_Error_t DSL_DRV_VXX_DataRateUpdate(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDir,
   DSL_uint32_t dataRate);

DSL_Error_t DSL_DRV_VXX_FailReasonGet(
   DSL_Context_t *pContext);

DSL_Error_t DSL_DRV_VXX_XtseConfigure(
   DSL_Context_t *pContext);

#ifdef INCLUDE_DSL_G997_LINE_INVENTORY
DSL_Error_t DSL_DRV_VXX_OnLineInventoryFe(
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
DSL_boolean_t DSL_DRV_VXX_BandPlanSupportedCheck(
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
DSL_Error_t DSL_DRV_VXX_BandPlanSupportedGet(
   DSL_Context_t *pContext,
   DSL_BandPlanSupportData_t *pData);

/** TBD
*/
DSL_Error_t DSL_DRV_VXX_FirmwareVersionCheck(
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
#define DSL_DRV_VXX_H2D_CMV_READ                0x00
#define DSL_DRV_VXX_H2D_CMV_WRITE               0x04


/* CMV Message Codes: ARC-to-ME(-to-WinHost) */
#define DSL_DRV_VXX_D2H_CMV_READ_REPLY                0x01
#define DSL_DRV_VXX_D2H_CMV_WRITE_REPLY               0x05
#define DSL_DRV_VXX_D2H_CMV_INDICATE                  0x11
#define DSL_DRV_VXX_D2H_ERROR_OPCODE_UNKNOWN          0x21
#define DSL_DRV_VXX_D2H_ERROR_CMV_UNKNOWN             0x31
#define DSL_DRV_VXX_D2H_ERROR_CMV_READ_NOT_AVAILABLE  0x41
#define DSL_DRV_VXX_D2H_ERROR_CMV_WRITE_ONLY          0x51
#define DSL_DRV_VXX_D2H_ERROR_CMV_READ_ONLY           0x61
#define DSL_DRV_VXX_D2H_ERROR_CMV_UNINITIALIZED_ENTRY 0x71
#define DSL_DRV_VXX_D2H_ERROR_ILLEGAL_PAYLOAD_SIZE    0x81
#define DSL_DRV_VXX_D2H_ERROR_MSG_TEMP_NOT_AVAIL      0x91


/* Peek/Poke Message Codes: (WinHost-to-)ME-to-ARC */
#define DSL_DRV_VXX_H2D_DEBUG_READ_DM           0x02
#define DSL_DRV_VXX_H2D_DEBUG_READ_PM           0x06
#define DSL_DRV_VXX_H2D_DEBUG_WRITE_DM          0x0A
#define DSL_DRV_VXX_H2D_DEBUG_WRITE_PM          0x0E


/* Peek/Poke Message Codes: ARC-to-ME(-to-WinHost) */
#define DSL_DRV_VXX_D2H_DEBUG_READ_DM_REPLY     0x03
#define DSL_DRV_VXX_D2H_DEBUG_READ_PM_REPLY     0x07
#define DSL_DRV_VXX_D2H_DEBUG_WRITE_DM_REPLY    0x0B
#define DSL_DRV_VXX_D2H_DEBUG_WRITE_PM_REPLY    0x0F
#define DSL_DRV_VXX_D2H_ERROR_ADDR_UNKNOWN      0x33


/* Modem Ready Message Codes: ARC-to-ME */
#define DSL_DRV_VXX_D2DCE_AUTONOMOUS_MODEM_READY         0xF1
#define DSL_DRV_VXX_RESULT_AUTO_MODEM_READY_SUCCESS      0x0
#define DSL_DRV_VXX_RESULT_AUTO_MODEM_READY_ERROR_AC_CLK 0x101

/* Device functional opcodes */
#define DSL_DRV_VXX_D2H_ERROR_BKGD_TASK_WAITING       0xE1 /* 1110 0001b */
#define DSL_DRV_VXX_D2H_ERROR_GENERAL_FAILURE         0xE2 /* 1110 0010b */
#define DSL_DRV_VXX_D2H_ERROR_CMD_NOT_ALLOWED         0xE3 /* 1110 0011b */
#define DSL_DRV_VXX_D2H_ERROR_PARAMETER_WRONG         0xE4 /* 1110 0100b */
#define DSL_DRV_VXX_D2H_ERROR_RESOURCE_OCCUPIED       0xE5 /* 1110 0101b */
#define DSL_DRV_VXX_D2H_DBG_VISP_RUNNING              0xE6 /* 1110 0110b */
#define DSL_DRV_VXX_D2H_DBG_VISP_STOPPED              0xE7 /* 1110 0111b */

/** @} DRV_DSL_DEVICE */

#ifdef __cplusplus
}
#endif

#endif
