/******************************************************************************

                              Copyright (c) 2013
                            Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef _DRV_DSL_CPE_VRX_MSG_H
#define _DRV_DSL_CPE_VRX_MSG_H

#ifdef __cplusplus
   extern "C" {
#endif

/** \addtogroup DRV_DSL_DEVICE
 @{ */

#include "drv_dsl_cpe_api.h"

#include "drv_dsl_cpe_vrx_msg_const.h"
#include "drv_dsl_cpe_vrx_msg_config_pmd.h"
#include "drv_dsl_cpe_vrx_msg_dmt_scope.h"
#include "drv_dsl_cpe_vrx_msg_ghs_status.h"
#include "drv_dsl_cpe_vrx_msg_olr.h"
#include "drv_dsl_cpe_vrx_msg_perf_pmd.h"
#include "drv_dsl_cpe_vrx_msg_tc_counters.h"

/**
   This function triggers to send a specified message which requests data from
   the local device and waits for appropriate answer.

   \param pContext Pointer to dsl cpe drive context structure, [I]
   \param nMsgID   Specifies the message ID as defined in the VRX firmware
                   message specification. It includes the message type and
                   subtype, [I]
   \param nLength  number of bytes of the message payload, [I]
   \param nData    pointer to the message payload data, [I]
   \param nLenAck  available buffer size for received ack, [I]
   \param pDataAck pointer to buffer for receiving ack message, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed or message ID of acknowledge wrong
*/
DSL_Error_t DSL_DRV_VRX_SendMessage(
   DSL_Context_t *pContext,
   const DSL_uint32_t nMsgID,
   const DSL_uint16_t nLength,
   const DSL_uint8_t *pData,
   const DSL_uint16_t nLenAck,
   DSL_uint8_t *pDataAck);

DSL_Error_t DSL_DRV_VRX_ChReadMessage(
   DSL_Context_t *pContext,
   IOCTL_MEI_message_t *pMsg);

DSL_Error_t DSL_DRV_VRX_ChRequestMessage(
   DSL_Context_t *pContext,
   IOCTL_MEI_messageSend_t *pRw);

/*
   This function sends the firmware message "CMD_ModemFSM_StateGet".

   \param pContext Pointer to dsl library context structure, [I]
   \param pAck pointer to the msg ACK, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgModemFsmStateGet(
   DSL_Context_t *pContext,
   DSL_uint8_t *pAck);

/*
   This function sends the firmware message "CMD_Clause30_StatsLineGet".

   \param pContext Pointer to dsl library context structure, [I]
   \param nChannel channel number, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgClause30StatsLineGet(
   DSL_Context_t *pContext,
   const DSL_uint8_t nChannel);

/*
   This function sends the firmware message "CMD_Clause30_StatsSysGet".

   \param pContext Pointer to dsl library context structure, [I]
   \param nChannel channel number, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgClause30StatsSysGet(
   DSL_Context_t *pContext,
   const DSL_uint8_t nChannel);

#if defined(INCLUDE_DSL_G997_PER_TONE) || defined(INCLUDE_DSL_DELT)
/*
   This function calculates the SNR per subcarrier group from the
   SNR per subcarrier values for a given direction. For each subcarrier group
   an average is built.

   \param pContext   Pointer to dsl library context structure, [I]
   \param nDirection Specifies the direction (upstream/downstream) to which
                     the function will apply. [I]
   \param pData      Returns the SNR data , [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
   - or any other defined specific error code
*/
DSL_Error_t DSL_DRV_VRX_SendMsgSnrGet(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDirection,
   DSL_G997_NSCData8_t *pData);
#endif /* defined(INCLUDE_DSL_G997_PER_TONE) || defined(INCLUDE_DSL_DELT)*/

#ifdef INCLUDE_DSL_DELT
/**
   This function requests a set of up to 60 entries of the DELT data.

   \param pContext   DSL CPE API context
   \param nStartIndex Specifies the first tone group index to retrieve. [I]
   \param nStopIndex Specifies the last tone group index to retrieve. [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgTestParamsFeRequest(
   DSL_Context_t *pContext,
   const DSL_uint16_t nStartIndex,
   const DSL_uint16_t nStopIndex);
#endif /* INCLUDE_DSL_DELT*/

#ifdef INCLUDE_DSL_DELT
/*
   This function reads the H linear parameters per subcarrier group
   measured during diagnostic state or training.

   \param pContext   Pointer to dsl library context structure, [I]
   \param nDirection Specifies the direction (upstream/downstream) to which
                     the function will apply. [I]
   \param pData      Returns the requested HLIN data, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
   - or any other defined specific error code
*/
DSL_Error_t DSL_DRV_VRX_SendMsgHlinGet(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDirection,
   DSL_G997_NSCComplexData_t *pData);
#endif /* INCLUDE_DSL_DELT*/

#ifdef INCLUDE_DSL_DELT
/*
   This function reads the H logarithmic parameters per subcarrier group
   measured during diagnostic state or training.

   \param pContext   Pointer to dsl library context structure, [I]
   \param nDirection Specifies the direction (upstream/downstream) to which
                     the function will apply. [I]
   \param pData      Returns the requested HLOG data, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
   - or any other defined specific error code
*/
DSL_Error_t DSL_DRV_VRX_SendMsgHlogGet(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDirection,
   DSL_G997_NSCData16_t *pData);
#endif /* INCLUDE_DSL_DELT*/

#ifdef INCLUDE_DSL_DELT
/*
   This function reads the Quality of line parameters per subcarrier group
   measured during diagnostic state or training.

   \param pContext   Pointer to dsl library context structure, [I]
   \param nDirection Specifies the direction (upstream/downstream) to which
                     the function will apply. [I]
   \param pData      Returns the QLN data, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
   - or any other defined specific error code
*/
DSL_Error_t DSL_DRV_VRX_SendMsgQlnGet(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDirection,
   DSL_G997_NSCData8_t * pData);
#endif /* INCLUDE_DSL_DELT*/

#ifdef INCLUDE_DSL_G997_PER_TONE
/**
   Retrieve the current gain allocation table
   by evaluation of the bit allocation table.
   This function causes relevant message traffic.

   \param pContext - Pointer to DSL library context structure, [I]
   \param nDirection Specifies the direction (upstream/downstream) to which
                     the function will apply. [I]
   \param pData      if this pointer is not NULL, it returns the current
                     gain allocation table, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_GainAllocationTableGet(
   DSL_IN DSL_Context_t *pContext,
   const DSL_AccessDir_t nDirection,
   DSL_IN_OUT DSL_G997_NSCData16_t *pData);
#endif /* INCLUDE_DSL_G997_PER_TONE*/

#ifdef INCLUDE_DSL_G997_PER_TONE
/**
   Retrieve the current SNR allocation table
   by evaluation of the bit allocation table.
   This function causes relevant message traffic.

   \param pContext - Pointer to DSL library context structure, [I]
   \param nDirection Specifies the direction (upstream/downstream) to which
                     the function will apply. [I]
   \param pData      if this pointer is not NULL, it returns the current
                     SNR allocation table, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SnrAllocationTableNeGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_OUT DSL_G997_NSCData8_t *pData);
#endif /* INCLUDE_DSL_G997_PER_TONE*/

#if defined(INCLUDE_DSL_G997_PER_TONE) || defined(INCLUDE_DSL_CPE_MISC_LINE_STATUS)
/**
   Retrieve the current bit allocation table and/or
   the number of bits per symbol
   by evaluation of the bit allocation table.
   This function causes relevant message traffic.

   \param pContext - Pointer to DSL library context structure, [I]
   \param nDirection Specifies the direction (upstream/downstream) to which
                     the function will apply. [I]
   \param pData      if this pointer is not NULL, it returns the current
                     bit allocation table, [O]
   \param pBitsPerSymbol
                     if this pointer is not NULL, it returns the current
                     bits per symbol, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_BitAllocationTableGet(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDirection,
   DSL_G997_NSCData8_t *pData,
   DSL_uint32_t *pBitsPerSymbol);
#endif /* defined(INCLUDE_DSL_G997_PER_TONE) || defined(INCLUDE_DSL_CPE_MISC_LINE_STATUS)*/

/*
   This function sends the firmware message "CMD_NoiseMarginDeltaSet".

   \param pContext Pointer to dsl library context structure, [I]
   \param nNoiseMarginDelta Noise Margin Delta, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgNoiseMarginDeltaSet(
   DSL_Context_t *pContext,
   DSL_int16_t nNoiseMarginDelta);

#if defined(INCLUDE_DSL_PM) && defined(INCLUDE_DSL_CPE_PM_CONFIG)
/*
   This function sends the firmware message "CMD_OH_OptionsSet".

   \param pContext Pointer to dsl library context structure, [I]
   \param nEocPollPeriod TBD, [I]
   \param bEocPoll1 TBD, [I]
   \param bEocPoll2 TBD, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgOhOptionsSet(
   DSL_Context_t *pContext,
   DSL_uint32_t nEocPollPeriod,
   DSL_boolean_t bEocPoll1,
   DSL_boolean_t bEocPoll2);
#endif /* defined(INCLUDE_DSL_PM) && defined(INCLUDE_DSL_CPE_PM_CONFIG)*/

/*
   This function sends the firmware message "CMD_HS_SelectedProfileVDSL2Get".

   \param pContext Pointer to dsl library context structure, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgSelectedProfileVdsl2Get(
   DSL_Context_t *pContext);

/*
   This function sends the firmware message "CMD_ModemFSM_FailReasonGet".

   \param pContext Pointer to dsl library context structure, [I]
   \param pAck pointer to the msg ACK, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgModemFSMFailReasonGet(
   DSL_Context_t *pContext,
   DSL_uint8_t *pAck);

/*
   This function handles the XTSE status bits

   \param pContext Pointer to dsl library context structure, [I/O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgXtseStatusGet(
   DSL_Context_t *pContext);

/*
   This function gets the FE line failures.

   \param pContext - Pointer to dsl library context structure, [I/O]
   \param pAck pointer to the msg ACK, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgLineFailureFeGet(
   DSL_Context_t *pContext,
   DSL_uint8_t *pAck);

/*
   This function gets the NE line failures.

   \param pContext - Pointer to dsl library context structure, [I/O]
   \param pAck pointer to the msg ACK, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgLineFailureNeGet(
   DSL_Context_t *pContext,
   DSL_uint8_t *pAck);

/*
   This function gets the NE/FE data path failures.

   \param pContext - Pointer to dsl library context structure, [I/O]
   \param pAck pointer to the msg ACK, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgDataPathFailuresGet(
   DSL_Context_t *pContext,
   DSL_uint8_t *pAck);

/*
   This function sends the firmware message "CMD_LineStatusPerBandXX_Get".

   \param pContext Pointer to dsl library context structure, [I]
   \param nDirection Upstream or Downstream, [I]
   \param pAck pointer to the msg ACK, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgLineStatusPerBandGet(
   DSL_Context_t *pContext,
   DSL_AccessDir_t nDirection,
   DSL_uint8_t *pAck);

/*
   This function sends the firmware message "CMD_PBO_AELEM_Status_Get".

   \param pContext Pointer to dsl library context structure, [I]
   \param pAck pointer to the msg ACK, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgUsPowerBackOffStatusGet(
   DSL_Context_t *pContext,
   DSL_uint8_t *pAck);

#ifdef INCLUDE_DSL_G997_LINE_INVENTORY
/*
   Read all the Inventory information of the Far End from the device,
   and store it in the DSL CPE context

   \param pContext   Pointer to dsl library context structure, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgInventoryFeGet(
   DSL_Context_t *pContext);
#endif /* INCLUDE_DSL_G997_LINE_INVENTORY*/

/*
   Write all the Inventory information of the Near End to the device

   \param pContext   Pointer to dsl cpe library context structure, [I]
   \param pData      Pointer to the Inventory data to set, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgInventoryNeSet(
   DSL_Context_t *pContext);

/*
   This function sends the firmware message "CMD_OLR_Control".

   \param pContext Pointer to dsl library context structure, [I]
   \param bAutoSRA_DS AutoSRA control DS, [I]
   \param bAutoSRA_US AutoSRA control US, [I]
   \param bSos_DS SOS control DS, [I]
   \param bSos_US SOS control US, [I]


   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgOlrControl(
   DSL_Context_t *pContext,
   DSL_boolean_t bAutoSRA_DS,
   DSL_boolean_t bAutoSRA_US,
   DSL_boolean_t bSos_DS,
   DSL_boolean_t bSos_US);

/*
   This function sends the firmware message "CMD_RTX_DS_Configure".

   \param pContext Pointer to dsl library context structure, [I]
   \param bReTxEnable ReTx control DS

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgRtxDsConfigure(
   DSL_Context_t *pContext,
   DSL_boolean_t bReTxEnable);

/*
   This function sends the firmware message "CMD_BandConfig_US/DS_Get"
   and returns the retrieved Data .

   \param pContext Pointer to dsl library context structure, [I]
   \param nDirection Upstream or Downstream, [I]
   \param pBandList  Retrieved Band Configuration, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgBandControlGet(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDirection,
   DSL_BandList_t *pBandList);

/**
   This function sends the firmware message "CMD_TR1ControlSet".

   \param pContext   Pointer to dsl library context structure, [I]
   \param bTR1Enable TBD, [I]
   \param bTR1EventEnable TBD, [I]
   \param nTR1Period TBD, [I]
   \param nTR1Control TBD, [I]
   \param nTR1Adjust TBD, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
   - or any other defined specific error code
*/
DSL_Error_t DSL_DRV_VRX_SendMsgTr1Control(
   DSL_Context_t *pContext,
   DSL_boolean_t bTR1Enable,
   DSL_boolean_t bTR1EventEnable,
   DSL_uint16_t nTR1Period,
   DSL_uint16_t nTR1Control,
   DSL_uint16_t nTR1Adjust);

/**
   This function gets the Measurement time and group sizes for
   DELT related measurements.

   \param pContext   Pointer to dsl library context structure, [I]
   \param nDirection Specifies the direction (upstream/downstream) to which
                     the function will apply. [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
   - or any other defined specific error code
*/
DSL_Error_t DSL_DRV_VRX_SendMsgTestParamsAuxGet(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDirection,
   DSL_uint8_t *pAck);

/*
   This function requests the far end SPAR1 bits from the handshake.

   \param pContext  Pointer to dsl library context structure, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgSpar1Get(
   DSL_Context_t *pContext);

#ifdef INCLUDE_DSL_G997_LINE_INVENTORY
/*
   Reads the Far End xTSE capabilities from the device and stores it
   into the internal structure.

   \param pContext   Pointer to dsl library context structure, [I]
   \param pXTSE   Pointer to XTSE status data, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgStandardInfoFeVdsl2Get(
   DSL_Context_t *pContext,
   DSL_uint8_t *pXTSE);
#endif /* INCLUDE_DSL_G997_LINE_INVENTORY*/

/*
   This function writes the configuration settings for the ATU System Enabling
   from the DSL CPE library configuration memory (which have been
   programmed using the configuration  functions).

   \param pContext - Pointer to dsl library context structure, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgVdsl2ProfileControl(
   DSL_Context_t *pContext);

/*
   This function writes the configuration settings for the bearer channels from
   the DSL library configuration memory (which have been programmed using the
   configuration  functions) for the specified line to the device.
   NOTE: Dual latency is not supported yet. Thus only bearer channel 0 will be
         programmed at the moment.

   \param pContext - Pointer to DSL CPE library context structure, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgBearerChSet(
   DSL_Context_t *pContext);

/*
   This function writes the TC layer configuration settings for the bearer channels from
   the DSL library configuration memory (which have been programmed using the
   configuration  functions) for the specified line to the device.

   \param pContext - Pointer to DSL CPE library context structure, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgBearerChTcLayerSet(
   DSL_Context_t *pContext);

/*
   This function sends the firmware message "CMD_OLR_XX_StatsGet".

   \param pContext Pointer to dsl library context structure, [I]
   \param nDirection Upstream or Downstream, [I]
   \param pAck pointer to the msg ACK, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgOlrStatusGet(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDirection,
   DSL_uint8_t *pAck);

/*
   This function sends the firmware message "CMD_BearerChsXX_Get".

   \param pContext Pointer to dsl library context structure, [I]
   \param nDirection Upstream or Downstream, [I]
   \param pAck pointer to the msg ACK, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgBearerChannelStatusGet(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDirection,
   DSL_uint8_t *pAck);

/*
   This function send the firmware message "CMD_TxL3ReqFailReasonGet".

   \param pContext - Pointer to dsl cpe library context structure, [I]
   \param pFailReason - Pointer to L3 Request Fail Reason data, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgTxL3RequestFailReasonGet(
   DSL_Context_t *pContext,
   DSL_VRX_L3RequestFailReason_t *pFailReason);

/*
   This function send the firmware message "CMD_TxL3RequestStatusGet".

   \param pContext - Pointer to dsl cpe library context structure, [I]
   \param pStatus - Pointer to L3 Request Status data, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgTxL3RequestStatusGet(
   DSL_Context_t *pContext,
   DSL_VRX_L3RequestStatus_t *pStatus);

/*
   This function send the firmware message "CMD_ShutdownRequest".

   \param pContext Pointer to DSL CPE library context structure, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgShutdownRequest(
   DSL_Context_t *pContext);

/*
   This function writes the configuration settings for the ATU System Enabling
   from the DSL CPE API configuration memory (which have been programmed using
   the configuration  functions) for the specified line to the device.

   \param pContext - Pointer to dsl cpe library context structure, [I]
   \param pXTSE - Pointer to XTSE configuration data, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgXtseConfigure(
   DSL_Context_t *pContext,
   DSL_G997_XTUSystemEnablingData_t *pXTSE);

/*
   This function sends the VRX firmware message
   CMD_LineStatusUS/CMD_LineStatusDS

   \param pContext Pointer to dsl library context structure, [I]
   \param nDirection Direction to retrieve data for, US/DS, [I]
   \param pAck Pointer to the msg ACK, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgLineStatusGet(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDirection,
   DSL_uint8_t *pAck);

/*
   This function sends the VRX firmware message
   CMD_BearerChsDS_RTX_Get

   \param pContext Pointer to dsl library context structure, [I]
   \param pAck Pointer to the msg ACK, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgBearerChsDsRtxGet(
   DSL_Context_t *pContext,
   DSL_uint8_t *pAck);

/*
   This function sends the VRX firmware message
   CMD_RTX_DS_StatsGet

   \param pContext Pointer to dsl library context structure, [I]
   \param pAck Pointer to the msg ACK, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgRtxDsStatsGet(
   DSL_Context_t *pContext,
   DSL_uint8_t *pAck);

/*
   This function sends the VRX firmware message
   CMD_VDSL_RTX_StatusGet

   \param pContext Pointer to dsl library context structure, [I]
   \param pAck Pointer to the msg ACK, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgRtxDsEnableStatusGet(
   DSL_Context_t *pContext,
   DSL_uint8_t *pAck);

/*
   This function sends the VRX firmware message
   CMD_RTX_PM_DS_Get

   \param pContext Pointer to dsl library context structure, [I]
   \param pAck Pointer to the msg ACK, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgRtxPmDsGet(
   DSL_Context_t *pContext,
   DSL_uint8_t *pAck);

/*
   This function handles the VRX firmware message
   for the ADSL framing parameters

   \param pContext Pointer to dsl library context structure, [I]
   \param nDir     Direction to retrieve data for, US/DS, [I]
   \param pAck Pointer to the msg ACK, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgAdslFrameDataGet(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDir,
   DSL_uint8_t *pAck);

/*
   This function sends the firmware message "CMD_FrameDataExt2XX_Get".

   \param pContext Pointer to dsl library context structure, [I]
   \param nDirection Upstream or Downstream, [I]
   \param pAck Pointer to the msg ACK, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgFrameDataExt2Get(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDirection,
   DSL_uint8_t *pAck);

/*
   This function sends the VRX firmware message CMD_TEST_TxControlSet
   to configure some chip hardware related issues.

   \param pContext  - Pointer to DSL library context structure, [I]
   \param TxControl - Tx Control options, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgTxControlSet(
   DSL_Context_t *pContext,
   const DSL_uint16_t TxControl);

/*
   This function sends the VRX firmware message CMD_ReInitNE_Configure
   to configure some chip hardware related issues.

   \param pContext - Pointer to DSL library context structure, [I]
   \param bEnableRestart - Enables the restart triggers, [I]
   \param pReInitNeConfigure - ReInit triggers config data, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgReInitNeConfigure(
   DSL_Context_t *pContext,
   const DSL_boolean_t bEnableRestart,
   DSL_BF_RebootCriteriaConfigData_t nReInitNeConfigure);

/*
   This function send the firmware message "CMD_AlgorithmControlSet_t".

   \param pContext     Pointer to dsl cpe library context structure, [I]
   \param nInitWord    Specifies the config word for initialisation, [I]
   \param nSteadyWord  Specifies the config word for SteadyState, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgAlgorithmControl(
   DSL_Context_t *pContext,
   const DSL_uint16_t nInitWord,
   const DSL_uint16_t nSteadyWord);

/*
   This function send the firmware message "CMD_OperationOptionsSet_t".

   \param pContext     Pointer to dsl cpe library context structure, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendOperationOptionsSet(
   DSL_Context_t *pContext);

/*
   This function sends the VRX firmware message CMD_ModemFSM_OptionsSet
   to configure some chip hardware related issues.

   \param pContext - Pointer to DSL CPE library context structure, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgModemFsmOptionsSet(
   DSL_Context_t *pContext,
   const DSL_boolean_t bAutoRestart,
   const DSL_boolean_t bDiagMode);

/*
   This function sends the VRX firmware message CMD_ModemFSM_Options2Set
   to configure some chip hardware related issues.

   \param pContext - Pointer to DSL CPE library context structure, [I]

   \param bEnableVirtualNoise - Enable/Disable virtual noise flag

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgModemFsmOptions2Set(
   DSL_Context_t *pContext,
   DSL_boolean_t bEnableVirtualNoise);

/*
   This function sends the firmware message "CMD_HS_ToneGroupSet".
   It configures the hand shake tone set according to the given config.

   \param pContext Pointer to dsl cpe library context structure, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgHsToneGroupSet(
   DSL_Context_t *pContext,
   const DSL_uint8_t *pXtseAdslA,
   const DSL_uint8_t *pXtseAdslB,
   const DSL_uint8_t *pXtseVdsl2);

/*
   This function send the VRX firmware message "CMD_ModemFSM_StateSet".

   \param pContext Pointer to dsl cpe library context structure, [I]
   \param nMode    Specifies the command for the message, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgModemStateSet(
   DSL_Context_t *pContext,
   const DSL_uint16_t nMode);

#if defined(INCLUDE_DSL_CPE_API_VRX)
/*
   This function sends the VRX firmware message "CMD_ShutdownRequest".

   \param pContext Pointer to dsl cpe library context structure, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgOrderlyShutDownRequest(
   DSL_Context_t *pContext);
#endif /* INCLUDE_DSL_CPE_API_VRX */

/**
   This function sends the VRX firmware undefined message
   to freeze TX link

   \param pContext - Pointer to DSL library context structure,
   \param nVal  - firmware internal value

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgLinkFreezeTX(
   DSL_Context_t *pContext,
   const DSL_uint16_t nVal);

/**
   This function sends the VRX firmware undefined message
   to freeze RX link

   \param pContext - Pointer to DSL library context structure,
   \param nVal  - firmware internal value

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgLinkFreezeRX(
   DSL_Context_t *pContext,
   const DSL_uint16_t nVal);

/**
   This function sends the VRX firmware message CMD_VersionInfoGet.

   \param pContext - Pointer to DSL library context structure, [I]
   \param pVerNum  - Pointer to the FW version number, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgFwVerInfoGet(
    DSL_Context_t *pContext,
    DSL_uint32_t *pVerNum);

/**
   This function sends the VRX firmware message CMD_VersionInfoGet.

   \param pContext - Pointer to DSL library context structure, [I]
   \param pVerNum  - Pointer to the HW version number, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgHwVerInfoGet(
   DSL_Context_t *pContext,
   DSL_uint32_t *pVerNum);

#ifdef INCLUDE_PILOT_TONES_STATUS
/**
   This function requests the pilot tones used in Showtime.

   \param pContext - Pointer to DSL library context structure, [I]
   \param pAck Pointer to the msg ACK, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgPilotTonesGet(
   DSL_Context_t *pContext,
   DSL_uint8_t *pAck);
#endif /* #ifdef INCLUDE_PILOT_TONES_STATUS*/

DSL_Error_t DSL_DRV_VRX_SendMsgHybridTypeGet(
   DSL_Context_t *pContext,
   DSL_HybridType_t *pHybridType);

DSL_Error_t DSL_DRV_VRX_SendMsgMfdResultsGet(
   DSL_Context_t *pContext,
   DSL_uint8_t *pAck);

DSL_Error_t DSL_DRV_VRX_SendMsgLoopLengthGet(
   DSL_Context_t *pContext,
   DSL_uint8_t *pAck);

DSL_Error_t DSL_DRV_VRX_SendMsgHybridInfoGet(
   DSL_Context_t *pContext,
   DSL_uint8_t *pAck);

DSL_Error_t DSL_DRV_VRX_SendMsgMiscConfigSet(
   DSL_Context_t *pContext);

DSL_Error_t DSL_DRV_VRX_SendMsgTestOptionsSet(
   DSL_Context_t *pContext);

/** @} DRV_DSL_DEVICE */

#ifdef INCLUDE_DSL_SYSTEM_INTERFACE
/*
   This function gets the TC status.

   \param pContext - Pointer to dsl library context structure, [I/O]
   \param pAck pointer to the msg ACK, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgTcStatusGet(
   DSL_Context_t *pContext,
   DSL_uint8_t *pAck);
#endif

#ifdef __cplusplus
}
#endif

#endif
