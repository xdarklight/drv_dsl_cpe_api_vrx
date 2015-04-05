/******************************************************************************

                              Copyright (c) 2012
                            Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/


#ifndef _DRV_DSL_CPE_VRX_MSG_OLR_H_
#define _DRV_DSL_CPE_VRX_MSG_OLR_H_

/** \file

*/

#ifndef __PACKED__
   #if defined (__GNUC__) || defined (__GNUG__)
      /* GNU C or C++ compiler */
      #define __PACKED__ __attribute__ ((packed))
   #else
      /* Byte alignment adjustment */
      #pragma pack(1)
      #define __PACKED__      /* nothing */
   #endif
   #define __PACKED_DEFINED__ 1
#endif


/** @defgroup _OLR_
 *  @{
 */

#ifdef __cplusplus
   extern "C" {
#endif

/* ----- Message Specific Constants Definition section ----- */
#define CMD_TestParamsFE_Request_TRIGGER 0x1
#define CMD_TestParamsFE_Request_ABORT 0x0
#define ACK_TestParamsFE_Poll_ONGOING 0x1
#define CMD_ClearEOC_TxTrigger_IDLE 0
#define CMD_ClearEOC_TxTrigger_START 1
#define ACK_ClearEOCStatusGet_IDLE 0
#define ACK_ClearEOCStatusGet_TXPROG 1
#define ACK_ClearEOCStatusGet_Reserved 2
#define ACK_ClearEOCStatusGet_TXERR 3
#define ACK_ClearEOCStatusGet_RXPROG 1
#define ACK_ClearEOCStatusGet_RXDONE 2
#define ACK_ClearEOCStatusGet_RXERR 3
#define EVT_ClearEOCStatusGet_IDLE 0
#define EVT_ClearEOCStatusGet_TXPROG 1
#define EVT_ClearEOCStatusGet_TXDONE 2
#define EVT_ClearEOCStatusGet_TXERR 3
#define EVT_ClearEOCStatusGet_RXPROG 1
#define EVT_ClearEOCStatusGet_RXDONE 2
#define EVT_ClearEOCStatusGet_RXERR 3
#define CMD_ClearEOCStatusSet_IDLE 0
#define ACK_RA_ModeDS_Get_MANUAL 1
#define ACK_RA_ModeDS_Get_AT_INIT 2
#define ACK_RA_ModeDS_Get_DYNAMIC 3
#define ACK_RA_ModeDS_Get_SOS 4
#define ACK_RA_ModeUS_Get_MANUAL 1
#define ACK_RA_ModeUS_Get_AT_INIT 2
#define ACK_RA_ModeUS_Get_DYNAMIC 3
#define ACK_RA_ModeUS_Get_SOS 4
/* ----- Message Specific Constants Definition section (End) ----- */

/** Message ID for CMD_BAT_TableEntriesGet */
#define CMD_BAT_TABLEENTRIESGET 0x1703

/**
   Requests information about the bit-allocation per subcarrier in VDSL mode.
   The values for all subcarriers of both directions can be retrieved. (For ADSL
   use CMD_ADSL_BAT_DS_Get and CMD_ADSL_BAT_US_Get).
*/
typedef struct CMD_BAT_TableEntriesGet CMD_BAT_TableEntriesGet_t;

/** Message ID for ACK_BAT_TableEntriesGet */
#define ACK_BAT_TABLEENTRIESGET 0x1703

/**
   Returns information about the bit-allocation per tone for the chosen range of
   subcarriers.(Section 7.5.1.29.1-2 of G997.1)
*/
typedef struct ACK_BAT_TableEntriesGet ACK_BAT_TableEntriesGet_t;

/** Message ID for CMD_GainTableEntriesGet */
#define CMD_GAINTABLEENTRIESGET 0x1903

/**
   Requests information about the Gains per subcarrier. The values for all
   subcarriers of both directions can be retrieved.(For ADSL use
   CMD_ADSL_GainTableDS_Get and CMD_ADSL_GainTableUS_Get).
*/
typedef struct CMD_GainTableEntriesGet CMD_GainTableEntriesGet_t;

/** Message ID for ACK_GainTableEntriesGet */
#define ACK_GAINTABLEENTRIESGET 0x1903

/**
   Returns information about the Gain-per-tone for the chosen range of
   subcarriers.(Section 7.5.1.29.3-4 of G997.1)
*/
typedef struct ACK_GainTableEntriesGet ACK_GainTableEntriesGet_t;

/** Message ID for CMD_SNR_NE_TableEntriesGet */
#define CMD_SNR_NE_TABLEENTRIESGET 0x0B03

/**
   The message requests information about the SNR per subcarrier available at
   the near-end side , which means for downstream direction at the CPE. It is
   the hosts responsibility to select the tone indices accordingly.
*/
typedef struct CMD_SNR_NE_TableEntriesGet CMD_SNR_NE_TableEntriesGet_t;

/** Message ID for ACK_SNR_NE_TableEntriesGet */
#define ACK_SNR_NE_TABLEENTRIESGET 0x0B03

/**
   Returns information about the SNR per subcarrier available at the near-end
   side, which means for downstream direction at the CPE.
*/
typedef struct ACK_SNR_NE_TableEntriesGet ACK_SNR_NE_TableEntriesGet_t;

/** Message ID for CMD_ADSL_BAT_DS_Get */
#define CMD_ADSL_BAT_DS_GET 0x070E

/**
   Requests information about the downstream bit-allocation per subcarrier in
   ADSL mode.
*/
typedef struct CMD_ADSL_BAT_DS_Get CMD_ADSL_BAT_DS_Get_t;

/** Message ID for ACK_ADSL_BAT_DS_Get */
#define ACK_ADSL_BAT_DS_GET 0x070E

/**
   Returns information about the downstream bit-allocation per tone for the
   chosen range of subcarriers.(Section 7.5.1.29.1 of G997.1)
*/
typedef struct ACK_ADSL_BAT_DS_Get ACK_ADSL_BAT_DS_Get_t;

/** Message ID for CMD_ADSL_BAT_US_Get */
#define CMD_ADSL_BAT_US_GET 0x060E

/**
   Requests information about the upstream bit-allocation per subcarrier in ADSL
   mode.
*/
typedef struct CMD_ADSL_BAT_US_Get CMD_ADSL_BAT_US_Get_t;

/** Message ID for ACK_ADSL_BAT_US_Get */
#define ACK_ADSL_BAT_US_GET 0x060E

/**
   Returns information about the upstream bit-allocation per tone for the chosen
   range of subcarriers.(Section 7.5.1.29.2 of G997.1)
*/
typedef struct ACK_ADSL_BAT_US_Get ACK_ADSL_BAT_US_Get_t;

/** Message ID for CMD_ADSL_GainTableDS_Get */
#define CMD_ADSL_GAINTABLEDS_GET 0x090E

/**
   Requests information about the Gains per subcarrier for the downstream
   direction.
*/
typedef struct CMD_ADSL_GainTableDS_Get CMD_ADSL_GainTableDS_Get_t;

/** Message ID for ACK_ADSL_GainTableDS_Get */
#define ACK_ADSL_GAINTABLEDS_GET 0x090E

/**
   Returns information about the Gain-per-tone for the chosen range of
   subcarriers for the downstream direction.(Section 7.5.1.29.3 of G997.1) as
   requested by CMD_ADSL_GainTableDS_Get.
*/
typedef struct ACK_ADSL_GainTableDS_Get ACK_ADSL_GainTableDS_Get_t;

/** Message ID for CMD_ADSL_GainTableUS_Get */
#define CMD_ADSL_GAINTABLEUS_GET 0x080E

/**
   Requests information about the Gains per subcarrier for the upstream
   direction.
*/
typedef struct CMD_ADSL_GainTableUS_Get CMD_ADSL_GainTableUS_Get_t;

/** Message ID for ACK_ADSL_GainTableUS_Get */
#define ACK_ADSL_GAINTABLEUS_GET 0x080E

/**
   Returns information about the Gain-per-tone for the chosen range of
   subcarriers for the upstream direction.(Section 7.5.1.29.4 of G997.1)
*/
typedef struct ACK_ADSL_GainTableUS_Get ACK_ADSL_GainTableUS_Get_t;

/** Message ID for CMD_ADSL_ExMarginReductionGet */
#define CMD_ADSL_EXMARGINREDUCTIONGET 0x2F03

/**
   Requests information about the Excess Margin Reduction needed for the fine
   gain calculation at the ATU_R.(See also ACK_ADSL_GainTableDS_Get).
*/
typedef struct CMD_ADSL_ExMarginReductionGet CMD_ADSL_ExMarginReductionGet_t;

/** Message ID for ACK_ADSL_ExMarginReductionGet */
#define ACK_ADSL_EXMARGINREDUCTIONGET 0x2F03

/**
   Returns information about the Excess Margin Reduction for the downstream
   direction.
*/
typedef struct ACK_ADSL_ExMarginReductionGet ACK_ADSL_ExMarginReductionGet_t;

/** Message ID for CMD_HlogDS_Get */
#define CMD_HLOGDS_GET 0x4A03

/**
   Requests information about the downstream HLOG information per subcarrier
   group (Section 7.5.1.26.6 of G997.1).During STEADY_STATE, the command can be
   used in VDSL to request near-end data only (CPE). For the far-end HLOG to be
   provided via the EOC channel CMD_TestParamsFE_Request must be used. In ADSL,
   the command can be applied for near-end as well as far-end parameters.During
   loop diagnostic mode, the command shall be used to request both near-end and
   far-end data.
*/
typedef struct CMD_HlogDS_Get CMD_HlogDS_Get_t;

/** Message ID for ACK_HlogDS_Get */
#define ACK_HLOGDS_GET 0x4A03

/**
   Returns information about the downstream HLOG per subcarrier group for the
   chosen range. (Section 7.5.1.26.6 of G997.1)
*/
typedef struct ACK_HlogDS_Get ACK_HlogDS_Get_t;

/** Message ID for CMD_HlogUS_Get */
#define CMD_HLOGUS_GET 0x4B03

/**
   Requests information about the upstream HLOG information per subcarrier group
   (Section 7.5.1.26.11 of G997.1).In VDSL the command cannot be used in
   STEADY_STATE. Instead, CMD_TestParamsFE_Request must be applied.
*/
typedef struct CMD_HlogUS_Get CMD_HlogUS_Get_t;

/** Message ID for ACK_HlogUS_Get */
#define ACK_HLOGUS_GET 0x4B03

/**
   Returns information about the upstream HLOG per subcarrier group for the
   chosen range. (Section 7.5.1.26.11 of G997.1)
*/
typedef struct ACK_HlogUS_Get ACK_HlogUS_Get_t;

/** Message ID for CMD_HlinDS_Get */
#define CMD_HLINDS_GET 0x4803

/**
   Requests information about the downstream HLIN information per subcarrier
   group. (Section 7.5.1.26.3 of G997.1).The HLIN data are available during loop
   diagnostic mode only.
*/
typedef struct CMD_HlinDS_Get CMD_HlinDS_Get_t;

/** Message ID for ACK_HlinDS_Get */
#define ACK_HLINDS_GET 0x4803

/**
   Returns information about the downstream HLIN per subcarrier group for the
   chosen range. (Section 7.5.1.26.3 of G997.1)
*/
typedef struct ACK_HlinDS_Get ACK_HlinDS_Get_t;

/** Message ID for CMD_HlinUS_Get */
#define CMD_HLINUS_GET 0x4903

/**
   Requests information about the upstream HLIN information per subcarrier
   group. (Section 7.5.1.26.9 of G997.1).The HLIN data are available in loop
   diagnostic mode only.
*/
typedef struct CMD_HlinUS_Get CMD_HlinUS_Get_t;

/** Message ID for ACK_HlinUS_Get */
#define ACK_HLINUS_GET 0x4903

/**
   Returns information about the upstream HLIN per subcarrier group for the
   chosen range. (Section 7.5.1.26.9 of G997.1)
*/
typedef struct ACK_HlinUS_Get ACK_HlinUS_Get_t;

/** Message ID for CMD_QLN_DS_Get */
#define CMD_QLN_DS_GET 0x4C03

/**
   Requests information about the downstream QLN information (QLNpsds) per
   subcarrier group (Section 7.5.1.27.3 of G997.1).During STEADY_STATE, the
   command can be used in VDSL to request near-end data only (CPE).
*/
typedef struct CMD_QLN_DS_Get CMD_QLN_DS_Get_t;

/** Message ID for ACK_QLN_DS_Get */
#define ACK_QLN_DS_GET 0x4C03

/**
   Returns information about the QLN per subcarrier group for the chosen range.
   (Section 7.5.1.27.3 of G997.1)
*/
typedef struct ACK_QLN_DS_Get ACK_QLN_DS_Get_t;

/** Message ID for CMD_QLN_US_Get */
#define CMD_QLN_US_GET 0x4D03

/**
   Requests information about the upstream QLN (QLNpsus) per subcarrier group.
   (Section 7.5.1.27.6 of G997.1).IIn VDSL the command cannot be used in
   STEADY_STATE. Instead, CMD_TestParamsFE_Request must be applied.
*/
typedef struct CMD_QLN_US_Get CMD_QLN_US_Get_t;

/** Message ID for ACK_QLN_US_Get */
#define ACK_QLN_US_GET 0x4D03

/**
   Returns information about the QLN per subcarrier group for the chosen range.
   (Section 7.5.1.27.6 of G997.1)
*/
typedef struct ACK_QLN_US_Get ACK_QLN_US_Get_t;

/** Message ID for CMD_SNR_DS_Get */
#define CMD_SNR_DS_GET 0x5503

/**
   Requests information about the downstream SNR per subcarrier group (Section
   7.5.1.28.3 of G997.1).
*/
typedef struct CMD_SNR_DS_Get CMD_SNR_DS_Get_t;

/** Message ID for ACK_SNR_DS_Get */
#define ACK_SNR_DS_GET 0x5503

/**
   Returns information about the SNR per subcarrier group for the chosen range.
   (Section 7.5.1.28.3 of G997.1)
*/
typedef struct ACK_SNR_DS_Get ACK_SNR_DS_Get_t;

/** Message ID for CMD_SNR_US_Get */
#define CMD_SNR_US_GET 0x4E03

/**
   Requests information about the upstream SNR per subcarrier group (Section
   7.5.1.28.6 of G997.1).During Showtime the message is used to retrieve the
   SNRpsus in ADSL only. In VDSL CMD_TestParamsFE_Request is to be applied
   instead. During loop diagnostic mode the message is used for both ADSL and
   VDSL.
*/
typedef struct CMD_SNR_US_Get CMD_SNR_US_Get_t;

/** Message ID for ACK_SNR_US_Get */
#define ACK_SNR_US_GET 0x4E03

/**
   Returns information about the upstream SNR per subcarrier group for the
   chosen range. (Section 7.5.1.28.6 of G997.1)
*/
typedef struct ACK_SNR_US_Get ACK_SNR_US_Get_t;

/** Message ID for CMD_TestParamsAuxDS_Get */
#define CMD_TESTPARAMSAUXDS_GET 0x4F03

/**
   Requests test parameter related information for the downstream direction: The
   HLIN scaling factor (HLINSCds), the subcarrier group size "G" and the
   measurement times for HLOGpsds, QLNpsds, SNRpsds.(Sections 7.5.1.26.1/2/4/5,
   7.5.1.27.1/2 and 7.5.1.28.1/2 of G997.1)
*/
typedef struct CMD_TestParamsAuxDS_Get CMD_TestParamsAuxDS_Get_t;

/** Message ID for ACK_TestParamsAuxDS_Get */
#define ACK_TESTPARAMSAUXDS_GET 0x4F03

/**
   Provides the test-parameter related information as requested by
   CMD_TestParamsAuxDS_Get: The HLIN scaling factor (HLINSCds), the subcarrier
   group size "G" and the measurement times for HLOGpsds, QLNpsds,
   SNRpsds.(Sections 7.5.1.26.1/2/4/5, 7.5.1.27.1/2 and 7.5.1.28.1/2 of G997.1)
*/
typedef struct ACK_TestParamsAuxDS_Get ACK_TestParamsAuxDS_Get_t;

/** Message ID for CMD_TestParamsAuxUS_Get */
#define CMD_TESTPARAMSAUXUS_GET 0x5003

/**
   Requests test parameter related information for the upstream direction: The
   HLIN scaling factor (HLINSCus), the subcarrier group size "G" and the
   measurement times for HLOGpsus, QLNpsus, SNRpsus.(Sections
   7.5.1.26.7/8/10/11, 7.5.1.27.4/5 and 7.5.1.28.4/5 of G997.1)The test
   parameters can be requested during loop diagnostic mode (VDSL, ADSL) and in
   ADSL also during STEADY_STATE.In VDSL during STEADY_STATE, the command can be
   used only to retrieve the "group size" parameters. To get the measurement
   time parameters (via the EOC channel) the message CMD_TestParamsFE_Request
   must be applied instead.
*/
typedef struct CMD_TestParamsAuxUS_Get CMD_TestParamsAuxUS_Get_t;

/** Message ID for ACK_TestParamsAuxUS_Get */
#define ACK_TESTPARAMSAUXUS_GET 0x5003

/**
   Provides the test-parameter related information as requested by
   CMD_TestParamsAuxUS_Get: The HLIN scaling factor (HLINSCus), the subcarrier
   group size "G" and the measurement times for HLOGpsus, QLNpsus, SNRpsus.
*/
typedef struct ACK_TestParamsAuxUS_Get ACK_TestParamsAuxUS_Get_t;

/** Message ID for CMD_TestParamsFE_Request */
#define CMD_TESTPARAMSFE_REQUEST 0x0849

/**
   This message is used to trigger retrieval of the far-end PMD Test Parameters
   Hlog, SNR and QLN via the EOC channel during Showtime. If StartIndex or
   EndIndex do not contain values in a valid range then no autonomous message
   will be sent by the firmware. Once the retrieval process is started, it can
   also be aborted with this message via the control parameter. The status of
   the parameter retrieval process can be polled via the CMD_TestParamsFE_Poll
   message. Once the parameters are retrieved, they are provided with the
   EVT_PMD_TestParamsGet message.
*/
typedef struct CMD_TestParamsFE_Request CMD_TestParamsFE_Request_t;

/** Message ID for ACK_TestParamsFE_Request */
#define ACK_TESTPARAMSFE_REQUEST 0x0849

/**
   This is the acknowledgement for CMD_TestParamsFE_Request.
*/
typedef struct ACK_TestParamsFE_Request ACK_TestParamsFE_Request_t;

/** Message ID for CMD_TestParamsFE_Poll */
#define CMD_TESTPARAMSFE_POLL 0x0809

/**
   This message polls the status of the far end parameter retrieval process.
*/
typedef struct CMD_TestParamsFE_Poll CMD_TestParamsFE_Poll_t;

/** Message ID for ACK_TestParamsFE_Poll */
#define ACK_TESTPARAMSFE_POLL 0x0809

/**
   This is the acknowledgement for CMD_TestParamsFE_Poll.
*/
typedef struct ACK_TestParamsFE_Poll ACK_TestParamsFE_Poll_t;

/** Message ID for EVT_PMD_TestParamsGet */
#define EVT_PMD_TESTPARAMSGET 0x5803

/**
   This event message provides the far-end data after the far end data retrieval
   process. This message is a result of the CMD_TestParamsFE_Request message.
*/
typedef struct EVT_PMD_TestParamsGet EVT_PMD_TestParamsGet_t;

/** Message ID for CMD_ClearEOC_Configure */
#define CMD_CLEAREOC_CONFIGURE 0x0A49

/**
   The message is used to configure the autonomous messaging related to Clear
   EOC transmission.
*/
typedef struct CMD_ClearEOC_Configure CMD_ClearEOC_Configure_t;

/** Message ID for ACK_ClearEOC_Configure */
#define ACK_CLEAREOC_CONFIGURE 0x0A49

/**
   This is the acknowledgement for CMD_ClearEOC_Configure.
*/
typedef struct ACK_ClearEOC_Configure ACK_ClearEOC_Configure_t;

/** Message ID for CMD_ClearEOC_TxTrigger */
#define CMD_CLEAREOC_TXTRIGGER 0x0949

/**
   The message is used to trigger the transmission of  Clear EOC messages that
   were placed into the Clear EOC transmit buffer before with
   CMD_ClearEOC_Write.
*/
typedef struct CMD_ClearEOC_TxTrigger CMD_ClearEOC_TxTrigger_t;

/** Message ID for ACK_ClearEOC_TxTrigger */
#define ACK_CLEAREOC_TXTRIGGER 0x0949

/**
   This is the acknowledgement for CMD_ClearEOC_TxTrigger.
*/
typedef struct ACK_ClearEOC_TxTrigger ACK_ClearEOC_TxTrigger_t;

/** Message ID for CMD_ClearEOC_Write */
#define CMD_CLEAREOC_WRITE 0x5143

/**
   This message is used to write data to the ClearEOC write buffer of type
   VRX_ClearEOC_t. When the buffer is filled, the transmission is started
   applying CMD_ClearEOC_TxTrigger. If the message to transmit is longer than
   the mailbox size, a sequence of writes to the ClearEOC buffer has to be done
   before the transmission is started with CMD_ClearEOC_TxTrigger. When
   autonomous Tx status messaging is activated via CMD_ClearEOC_Configure, then
   the finished transmission is indicated by EVT_ClearEOCStatusGet.
*/
typedef struct CMD_ClearEOC_Write CMD_ClearEOC_Write_t;

/** Message ID for ACK_ClearEOC_Write */
#define ACK_CLEAREOC_WRITE 0x5143

/**
   This message is the acknowledgement for CMD_ClearEOC_Write.
*/
typedef struct ACK_ClearEOC_Write ACK_ClearEOC_Write_t;

/** Message ID for CMD_ClearEOC_Read */
#define CMD_CLEAREOC_READ 0x5203

/**
   This message is used to read data from the ClearEOC buffer of type
   VRX_ClearEOC_t. The length of the actual Clear EOC message can be found in
   the buffer. Please refer to VRX_ClearEOC_t. The availability of data can
   either be checked via CMD_ClearEOCStatusGet in polling mode or it can be
   reported by an autonomous EVT_ClearEOCStatusGet message when data is received
   (to be enabled using CMD_ClearEOC_Configure).
*/
typedef struct CMD_ClearEOC_Read CMD_ClearEOC_Read_t;

/** Message ID for ACK_ClearEOC_Read */
#define ACK_CLEAREOC_READ 0x5203

/**
   This message is the acknowledgement to CMD_ClearEOC_Read.
*/
typedef struct ACK_ClearEOC_Read ACK_ClearEOC_Read_t;

/** Message ID for EVT_ClearEOC_Read */
#define EVT_CLEAREOC_READ 0x5203

/**
   This message is an autonomous message that is generated when ClearEOC data
   was received and autonomous Clear EOC data messaging has been activated via
   CMD_ClearEOC_Configure. If the ClearEOC data does not fit in one message,
   then a sequence of messages is generated. The ClearEOC buffer is of type
   VRX_ClearEOC_t.
*/
typedef struct EVT_ClearEOC_Read EVT_ClearEOC_Read_t;

/** Message ID for CMD_ClearEOCStatusGet */
#define CMD_CLEAREOCSTATUSGET 0x0B09

/**
   This message is used to retrieve the status of the clear eoc data
   transmission.
*/
typedef struct CMD_ClearEOCStatusGet CMD_ClearEOCStatusGet_t;

/** Message ID for ACK_ClearEOCStatusGet */
#define ACK_CLEAREOCSTATUSGET 0x0B09

/**
   This is the acknowledgement for CMD_ClearEOCStatusGet.
*/
typedef struct ACK_ClearEOCStatusGet ACK_ClearEOCStatusGet_t;

/** Message ID for EVT_ClearEOCStatusGet */
#define EVT_CLEAREOCSTATUSGET 0x0B09

/**
   This autonomous message reports the Clear EOC status. It is sent only if the
   "Autonomous Status Message Control" was enabled for Tx and/or Rx direction
   with CMD_ClearEOC_Configure. If Tx direction is enabled, the message is
   generated when a Tx transmission is finished or failed. If Rx direction is
   enabled, the message is generated when the Rx status transitions from "Idle"
   to "Data Available" for retrieval by the host.
*/
typedef struct EVT_ClearEOCStatusGet EVT_ClearEOCStatusGet_t;

/** Message ID for CMD_ClearEOCStatusSet */
#define CMD_CLEAREOCSTATUSSET 0x0B49

/**
   The message is used to reset the transmit or receive status of the clear eoc
   data transmission to IDLE (for defined states see also
   CMD_ClearEOCStatusGet). See the description on the Clear EOC handling on Page
   491 for when it has to be applied. Transmit and receive status are
   distinguished by the Index parameter.
*/
typedef struct CMD_ClearEOCStatusSet CMD_ClearEOCStatusSet_t;

/** Message ID for ACK_ClearEOCStatusSet */
#define ACK_CLEAREOCSTATUSSET 0x0B49

/**
   This is the acknowledgement for CMD_ClearEOCStatusSet.
*/
typedef struct ACK_ClearEOCStatusSet ACK_ClearEOCStatusSet_t;

/** Message ID for CMD_OH_OptionsSet */
#define CMD_OH_OPTIONSSET 0x1945

/**
   Configuration of options for the overhead handling.
*/
typedef struct CMD_OH_OptionsSet CMD_OH_OptionsSet_t;

/** Message ID for ACK_OH_OptionsSet */
#define ACK_OH_OPTIONSSET 0x1945

/**
   Acknowledgement for CMD_OH_OptionsSet.
*/
typedef struct ACK_OH_OptionsSet ACK_OH_OptionsSet_t;

/** Message ID for CMD_OH_StatsGet */
#define CMD_OH_STATSGET 0x0F03

/**
   Requests OH polling statistic information.
*/
typedef struct CMD_OH_StatsGet CMD_OH_StatsGet_t;

/** Message ID for ACK_OH_StatsGet */
#define ACK_OH_STATSGET 0x0F03

/**
   Reports the OH polling statistics as requested by CMD_OH_StatsGet.
*/
typedef struct ACK_OH_StatsGet ACK_OH_StatsGet_t;

/** Message ID for CMD_OLR_Control */
#define CMD_OLR_CONTROL 0x0F45

/**
   Enables/Disables support for OLR events (Bitswaps, SRA, SOS, ROC).An OLR
   transition is always initiated by the receiving PMD, so the CPE requests the
   OLR event for downstream direction.
*/
typedef struct CMD_OLR_Control CMD_OLR_Control_t;

/** Message ID for ACK_OLR_Control */
#define ACK_OLR_CONTROL 0x0F45

/**
   Acknowledgement for CMD_OLR_Control.
*/
typedef struct ACK_OLR_Control ACK_OLR_Control_t;

/** Message ID for CMD_OLR_US_StatsGet */
#define CMD_OLR_US_STATSGET 0x5F03

/**
   Requests the OLR status information on bit swaps, DRR, SRA and SOS events for
   the upstream direction.
*/
typedef struct CMD_OLR_US_StatsGet CMD_OLR_US_StatsGet_t;

/** Message ID for ACK_OLR_US_StatsGet */
#define ACK_OLR_US_STATSGET 0x5F03

/**
   Reports the OLR status information for the upstream direction as requested by
   CMD_OLR_US_StatsGet.
*/
typedef struct ACK_OLR_US_StatsGet ACK_OLR_US_StatsGet_t;

/** Message ID for CMD_OLR_DS_StatsGet */
#define CMD_OLR_DS_STATSGET 0x6003

/**
   Requests the OLR status information on bit swaps, DRR, SRA and SOS events for
   the downstream direction.
*/
typedef struct CMD_OLR_DS_StatsGet CMD_OLR_DS_StatsGet_t;

/** Message ID for ACK_OLR_DS_StatsGet */
#define ACK_OLR_DS_STATSGET 0x6003

/**
   Reports the OLR status information for the downstream direction as requested
   by CMD_OLR_DS_StatsGet.
*/
typedef struct ACK_OLR_DS_StatsGet ACK_OLR_DS_StatsGet_t;

/** Message ID for CMD_RA_ModeDS_Get */
#define CMD_RA_MODEDS_GET 0xD903

/**
   Requests the actual active downstream RA mode (ACT-RA-MODEds, Section
   7.5.1.33.1 of G.997.1).
*/
typedef struct CMD_RA_ModeDS_Get CMD_RA_ModeDS_Get_t;

/** Message ID for ACK_RA_ModeDS_Get */
#define ACK_RA_MODEDS_GET 0xD903

/**
   Reports the actual active downstream RA mode (ACT-RA-MODEds) as requested by
   CMD_RA_ModeDS_Get.
*/
typedef struct ACK_RA_ModeDS_Get ACK_RA_ModeDS_Get_t;

/** Message ID for CMD_RA_ModeUS_Get */
#define CMD_RA_MODEUS_GET 0xDB03

/**
   Requests the actual active upstream RA mode (ACT-RA-MODEus, Section
   7.5.1.33.2 of G.997.1).
*/
typedef struct CMD_RA_ModeUS_Get CMD_RA_ModeUS_Get_t;

/** Message ID for ACK_RA_ModeUS_Get */
#define ACK_RA_MODEUS_GET 0xDB03

/**
   Reports the actual active upstream RA mode (ACT-RA-MODEus) as requested by
   CMD_RA_ModeUS_Get.
*/
typedef struct ACK_RA_ModeUS_Get ACK_RA_ModeUS_Get_t;

/** Message ID for EVT_OLR_US_EventGet */
#define EVT_OLR_US_EVENTGET 0x0307

/**
   Autonomous message indicating a successful upstream OLR event (SRA or SOS).
*/
typedef struct EVT_OLR_US_EventGet EVT_OLR_US_EventGet_t;

/** Message ID for EVT_OLR_DS_EventGet */
#define EVT_OLR_DS_EVENTGET 0x0407

/**
   Autonomous message indicating a successful downstream OLR event (SRA or SOS).
*/
typedef struct EVT_OLR_DS_EventGet EVT_OLR_DS_EventGet_t;

/** Message ID for CMD_OLR_US_EventConfigure */
#define CMD_OLR_US_EVENTCONFIGURE 0x0F49

/**
   Enables/Disables the generation of EVENT messages (EVT) for specific upstream
   OLR events. If the corresponding Enable bit for an OLR event is set, then the
   modem firmware will send an autonomous message EVT_OLR_US_EventGet if the OLR
   event happened in the last 1-second interval.
*/
typedef struct CMD_OLR_US_EventConfigure CMD_OLR_US_EventConfigure_t;

/** Message ID for ACK_OLR_US_EventConfigure */
#define ACK_OLR_US_EVENTCONFIGURE 0x0F49

/**
   Acknoledgement for CMD_OLR_US_EventConfigure.
*/
typedef struct ACK_OLR_US_EventConfigure ACK_OLR_US_EventConfigure_t;

/** Message ID for CMD_OLR_DS_EventConfigure */
#define CMD_OLR_DS_EVENTCONFIGURE 0x1049

/**
   Enables/Disables the generation of EVENT messages (EVT) for specific
   downstream OLR events. If the corresponding Enable bit for an OLR event is
   set, then the modem firmware will send an autonomous message
   EVT_OLR_DS_EventGet if the OLR event happened in the last 1-second interval.
*/
typedef struct CMD_OLR_DS_EventConfigure CMD_OLR_DS_EventConfigure_t;

/** Message ID for ACK_OLR_DS_EventConfigure */
#define ACK_OLR_DS_EVENTCONFIGURE 0x1049

/**
   Acknoledgement for CMD_OLR_DS_EventConfigure.
*/
typedef struct ACK_OLR_DS_EventConfigure ACK_OLR_DS_EventConfigure_t;

/** Message ID for CMD_RTX_DS_Configure */
#define CMD_RTX_DS_CONFIGURE 0x5048

/**
   Configures a link for retransmission of downstream data. For using the RTX
   function, this message has to be sent.
*/
typedef struct CMD_RTX_DS_Configure CMD_RTX_DS_Configure_t;

/** Message ID for ACK_RTX_DS_Configure */
#define ACK_RTX_DS_CONFIGURE 0x5048

/**
   Acknowledgement for CMD_RTX_DS_Configure.
*/
typedef struct ACK_RTX_DS_Configure ACK_RTX_DS_Configure_t;

/** Message ID for CMD_BearerChsDS_RTX_Get */
#define CMD_BEARERCHSDS_RTX_GET 0x0206

/**
   Requests RTX specific status information for the downstream bearer channels
   if G.INP retransmission is used.
*/
typedef struct CMD_BearerChsDS_RTX_Get CMD_BearerChsDS_RTX_Get_t;

/** Message ID for ACK_BearerChsDS_RTX_Get */
#define ACK_BEARERCHSDS_RTX_GET 0x0206

/**
   Delivers status information for the downstream bearer channels when G.INP
   retransmission is used.
*/
typedef struct ACK_BearerChsDS_RTX_Get ACK_BearerChsDS_RTX_Get_t;

/** Message ID for CMD_RTX_PMwoThreshDS_Get */
#define CMD_RTX_PMWOTHRESHDS_GET 0x2B0A

/**
   Requests counters for RTX performance monitoring which don’t include TR1
   threshold supervision.
*/
typedef struct CMD_RTX_PMwoThreshDS_Get CMD_RTX_PMwoThreshDS_Get_t;

/** Message ID for ACK_RTX_PMwoThreshDS_Get */
#define ACK_RTX_PMWOTHRESHDS_GET 0x2B0A

/**
   Delivers retransmission counters for RTX performance monitoring which don’t
   include TR1 threshold supervision.
*/
typedef struct ACK_RTX_PMwoThreshDS_Get ACK_RTX_PMwoThreshDS_Get_t;

/** Message ID for CMD_RTX_DS_StatsGet */
#define CMD_RTX_DS_STATSGET 0x2C0A

/**
   Requests retransmission test and debug wrap-around counters.
*/
typedef struct CMD_RTX_DS_StatsGet CMD_RTX_DS_StatsGet_t;

/** Message ID for ACK_RTX_DS_StatsGet */
#define ACK_RTX_DS_STATSGET 0x2C0A

/**
   Delivers retransmission test and debug wrap-around counters.
*/
typedef struct ACK_RTX_DS_StatsGet ACK_RTX_DS_StatsGet_t;

/** Message ID for CMD_RTX_StatusGet */
#define CMD_RTX_STATUSGET 0xE503

/**
   Requests the actually used G.INP retransmission status.
*/
typedef struct CMD_RTX_StatusGet CMD_RTX_StatusGet_t;

/** Message ID for ACK_RTX_StatusGet */
#define ACK_RTX_STATUSGET 0xE503

/**
   Provides the actually used G.INP retransmission status.
*/
typedef struct ACK_RTX_StatusGet ACK_RTX_StatusGet_t;

/**
   Requests information about the bit-allocation per subcarrier in VDSL mode.
   The values for all subcarriers of both directions can be retrieved. (For ADSL
   use CMD_ADSL_BAT_DS_Get and CMD_ADSL_BAT_US_Get).
*/
struct CMD_BAT_TableEntriesGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the bit-allocation per tone for the chosen range of
   subcarriers.(Section 7.5.1.29.1-2 of G997.1)
*/
struct ACK_BAT_TableEntriesGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Bit Allocation */
   VRX_BAT_TableEntry_t BAT[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Bit Allocation */
   VRX_BAT_TableEntry_t BAT[128];
#endif
} __PACKED__ ;


/**
   Requests information about the Gains per subcarrier. The values for all
   subcarriers of both directions can be retrieved.(For ADSL use
   CMD_ADSL_GainTableDS_Get and CMD_ADSL_GainTableUS_Get).
*/
struct CMD_GainTableEntriesGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the Gain-per-tone for the chosen range of
   subcarriers.(Section 7.5.1.29.3-4 of G997.1)
*/
struct ACK_GainTableEntriesGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Gains */
   DSL_uint16_t Gains[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Gains */
   DSL_uint16_t Gains[128];
#endif
} __PACKED__ ;


/**
   The message requests information about the SNR per subcarrier available at
   the near-end side , which means for downstream direction at the CPE. It is
   the hosts responsibility to select the tone indices accordingly.
*/
struct CMD_SNR_NE_TableEntriesGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the SNR per subcarrier available at the near-end
   side, which means for downstream direction at the CPE.
*/
struct ACK_SNR_NE_TableEntriesGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** SNR */
   DSL_uint16_t SNRps[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** SNR */
   DSL_uint16_t SNRps[128];
#endif
} __PACKED__ ;


/**
   Requests information about the downstream bit-allocation per subcarrier in
   ADSL mode.
*/
struct CMD_ADSL_BAT_DS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the downstream bit-allocation per tone for the
   chosen range of subcarriers.(Section 7.5.1.29.1 of G997.1)
*/
struct ACK_ADSL_BAT_DS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Bit Allocation */
   VRX_BAT_TableEntry_t BAT[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Bit Allocation */
   VRX_BAT_TableEntry_t BAT[128];
#endif
} __PACKED__ ;


/**
   Requests information about the upstream bit-allocation per subcarrier in ADSL
   mode.
*/
struct CMD_ADSL_BAT_US_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the upstream bit-allocation per tone for the chosen
   range of subcarriers.(Section 7.5.1.29.2 of G997.1)
*/
struct ACK_ADSL_BAT_US_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Bit Allocation */
   VRX_BAT_TableEntry_t BAT[32];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Bit Allocation */
   VRX_BAT_TableEntry_t BAT[32];
#endif
} __PACKED__ ;


/**
   Requests information about the Gains per subcarrier for the downstream
   direction.
*/
struct CMD_ADSL_GainTableDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the Gain-per-tone for the chosen range of
   subcarriers for the downstream direction.(Section 7.5.1.29.3 of G997.1) as
   requested by CMD_ADSL_GainTableDS_Get.
*/
struct ACK_ADSL_GainTableDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Gains */
   DSL_uint16_t Gains[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Gains */
   DSL_uint16_t Gains[128];
#endif
} __PACKED__ ;


/**
   Requests information about the Gains per subcarrier for the upstream
   direction.
*/
struct CMD_ADSL_GainTableUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the Gain-per-tone for the chosen range of
   subcarriers for the upstream direction.(Section 7.5.1.29.4 of G997.1)
*/
struct ACK_ADSL_GainTableUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Gains */
   DSL_uint16_t Gains[64];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Gains */
   DSL_uint16_t Gains[64];
#endif
} __PACKED__ ;


/**
   Requests information about the Excess Margin Reduction needed for the fine
   gain calculation at the ATU_R.(See also ACK_ADSL_GainTableDS_Get).
*/
struct CMD_ADSL_ExMarginReductionGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the Excess Margin Reduction for the downstream
   direction.
*/
struct ACK_ADSL_ExMarginReductionGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Excess Margin Reduction  */
   DSL_uint16_t eSnrmReduction;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Excess Margin Reduction  */
   DSL_uint16_t eSnrmReduction;
#endif
} __PACKED__ ;


/**
   Requests information about the downstream HLOG information per subcarrier
   group (Section 7.5.1.26.6 of G997.1).During STEADY_STATE, the command can be
   used in VDSL to request near-end data only (CPE). For the far-end HLOG to be
   provided via the EOC channel CMD_TestParamsFE_Request must be used. In ADSL,
   the command can be applied for near-end as well as far-end parameters.During
   loop diagnostic mode, the command shall be used to request both near-end and
   far-end data.
*/
struct CMD_HlogDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the downstream HLOG per subcarrier group for the
   chosen range. (Section 7.5.1.26.6 of G997.1)
*/
struct ACK_HlogDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** HLOG per SG: HLOGpsds */
   DSL_uint16_t HLOGpsds[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** HLOG per SG: HLOGpsds */
   DSL_uint16_t HLOGpsds[128];
#endif
} __PACKED__ ;


/**
   Requests information about the upstream HLOG information per subcarrier group
   (Section 7.5.1.26.11 of G997.1).In VDSL the command cannot be used in
   STEADY_STATE. Instead, CMD_TestParamsFE_Request must be applied.
*/
struct CMD_HlogUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the upstream HLOG per subcarrier group for the
   chosen range. (Section 7.5.1.26.11 of G997.1)
*/
struct ACK_HlogUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** HLOG per SG: HLOGpsus */
   DSL_uint16_t HLOGpsus[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** HLOG per SG: HLOGpsus */
   DSL_uint16_t HLOGpsus[128];
#endif
} __PACKED__ ;


/**
   Requests information about the downstream HLIN information per subcarrier
   group. (Section 7.5.1.26.3 of G997.1).The HLIN data are available during loop
   diagnostic mode only.
*/
struct CMD_HlinDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the downstream HLIN per subcarrier group for the
   chosen range. (Section 7.5.1.26.3 of G997.1)
*/
struct ACK_HlinDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** HLIN per SG: HLINpsds */
   VRX_HLIN_t HLINpsds[64];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** HLIN per SG: HLINpsds */
   VRX_HLIN_t HLINpsds[64];
#endif
} __PACKED__ ;


/**
   Requests information about the upstream HLIN information per subcarrier
   group. (Section 7.5.1.26.9 of G997.1).The HLIN data are available in loop
   diagnostic mode only.
*/
struct CMD_HlinUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the upstream HLIN per subcarrier group for the
   chosen range. (Section 7.5.1.26.9 of G997.1)
*/
struct ACK_HlinUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** HLIN per SG: HLINpsus */
   VRX_HLIN_t HLINpsus[64];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** HLIN per SG: HLINpsus */
   VRX_HLIN_t HLINpsus[64];
#endif
} __PACKED__ ;


/**
   Requests information about the downstream QLN information (QLNpsds) per
   subcarrier group (Section 7.5.1.27.3 of G997.1).During STEADY_STATE, the
   command can be used in VDSL to request near-end data only (CPE).
*/
struct CMD_QLN_DS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the QLN per subcarrier group for the chosen range.
   (Section 7.5.1.27.3 of G997.1)
*/
struct ACK_QLN_DS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** QLN per SG: QLNpsds */
   VRX_QLN_NE_t QLNds[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** QLN per SG: QLNpsds */
   VRX_QLN_NE_t QLNds[128];
#endif
} __PACKED__ ;


/**
   Requests information about the upstream QLN (QLNpsus) per subcarrier group.
   (Section 7.5.1.27.6 of G997.1).IIn VDSL the command cannot be used in
   STEADY_STATE. Instead, CMD_TestParamsFE_Request must be applied.
*/
struct CMD_QLN_US_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the QLN per subcarrier group for the chosen range.
   (Section 7.5.1.27.6 of G997.1)
*/
struct ACK_QLN_US_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** QLN per SG: QLNpsus */
   VRX_QLN_NE_t QLNus[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** QLN per SG: QLNpsus */
   VRX_QLN_NE_t QLNus[128];
#endif
} __PACKED__ ;


/**
   Requests information about the downstream SNR per subcarrier group (Section
   7.5.1.28.3 of G997.1).
*/
struct CMD_SNR_DS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the SNR per subcarrier group for the chosen range.
   (Section 7.5.1.28.3 of G997.1)
*/
struct ACK_SNR_DS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** SNR per SG: SNRpsds */
   VRX_SNR_t SNRpsds[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** SNR per SG: SNRpsds */
   VRX_SNR_t SNRpsds[128];
#endif
} __PACKED__ ;


/**
   Requests information about the upstream SNR per subcarrier group (Section
   7.5.1.28.6 of G997.1).During Showtime the message is used to retrieve the
   SNRpsus in ADSL only. In VDSL CMD_TestParamsFE_Request is to be applied
   instead. During loop diagnostic mode the message is used for both ADSL and
   VDSL.
*/
struct CMD_SNR_US_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the upstream SNR per subcarrier group for the
   chosen range. (Section 7.5.1.28.6 of G997.1)
*/
struct ACK_SNR_US_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** SNR per SG: SNRpsus */
   VRX_SNR_t SNRpsus[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** SNR per SG: SNRpsus */
   VRX_SNR_t SNRpsus[128];
#endif
} __PACKED__ ;


/**
   Requests test parameter related information for the downstream direction: The
   HLIN scaling factor (HLINSCds), the subcarrier group size "G" and the
   measurement times for HLOGpsds, QLNpsds, SNRpsds.(Sections 7.5.1.26.1/2/4/5,
   7.5.1.27.1/2 and 7.5.1.28.1/2 of G997.1)
*/
struct CMD_TestParamsAuxDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Provides the test-parameter related information as requested by
   CMD_TestParamsAuxDS_Get: The HLIN scaling factor (HLINSCds), the subcarrier
   group size "G" and the measurement times for HLOGpsds, QLNpsds,
   SNRpsds.(Sections 7.5.1.26.1/2/4/5, 7.5.1.27.1/2 and 7.5.1.28.1/2 of G997.1)
*/
struct ACK_TestParamsAuxDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** HLIN Scaling Factor "HLINSCds" */
   DSL_uint16_t HLINSC;
   /** HLIN Subcarrier Group Size DS "HLINGds" */
   DSL_uint16_t HLING;
   /** HLOG Measurement Time "HLOGMTds" */
   DSL_uint16_t HLOGMT;
   /** HLOG Subcarrier Group Size DS "HLOGGds" */
   DSL_uint16_t HLOGG;
   /** QLN Measurment Time "QLNMTds" */
   DSL_uint16_t QLNMT;
   /** QLN Subcarrier Group Size DS "QLNGds" */
   DSL_uint16_t QLNG;
   /** SNR Measurement Time "SNRMTds" */
   DSL_uint16_t SNRMT;
   /** SNR Subcarrier Group Size DS "SNRGds" */
   DSL_uint16_t SNRG;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** HLIN Scaling Factor "HLINSCds" */
   DSL_uint16_t HLINSC;
   /** HLIN Subcarrier Group Size DS "HLINGds" */
   DSL_uint16_t HLING;
   /** HLOG Measurement Time "HLOGMTds" */
   DSL_uint16_t HLOGMT;
   /** HLOG Subcarrier Group Size DS "HLOGGds" */
   DSL_uint16_t HLOGG;
   /** QLN Measurment Time "QLNMTds" */
   DSL_uint16_t QLNMT;
   /** QLN Subcarrier Group Size DS "QLNGds" */
   DSL_uint16_t QLNG;
   /** SNR Measurement Time "SNRMTds" */
   DSL_uint16_t SNRMT;
   /** SNR Subcarrier Group Size DS "SNRGds" */
   DSL_uint16_t SNRG;
#endif
} __PACKED__ ;


/**
   Requests test parameter related information for the upstream direction: The
   HLIN scaling factor (HLINSCus), the subcarrier group size "G" and the
   measurement times for HLOGpsus, QLNpsus, SNRpsus.(Sections
   7.5.1.26.7/8/10/11, 7.5.1.27.4/5 and 7.5.1.28.4/5 of G997.1)The test
   parameters can be requested during loop diagnostic mode (VDSL, ADSL) and in
   ADSL also during STEADY_STATE.In VDSL during STEADY_STATE, the command can be
   used only to retrieve the "group size" parameters. To get the measurement
   time parameters (via the EOC channel) the message CMD_TestParamsFE_Request
   must be applied instead.
*/
struct CMD_TestParamsAuxUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Provides the test-parameter related information as requested by
   CMD_TestParamsAuxUS_Get: The HLIN scaling factor (HLINSCus), the subcarrier
   group size "G" and the measurement times for HLOGpsus, QLNpsus, SNRpsus.
*/
struct ACK_TestParamsAuxUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** HLIN Scaling Factor "HLINSCus" */
   DSL_uint16_t HLINSC;
   /** HLIN Subcarrier Group Size DS "HLINGus" */
   DSL_uint16_t HLING;
   /** HLOG Measurement Time "HLOGMTus" */
   DSL_uint16_t HLOGMT;
   /** HLOG Subcarrier Group Size DS "HLOGGus" */
   DSL_uint16_t HLOGG;
   /** QLN Measurment Time "QLNMTus" */
   DSL_uint16_t QLNMT;
   /** QLN Subcarrier Group Size DS "QLNGus" */
   DSL_uint16_t QLNG;
   /** SNR Measurement Time "SNRMTus" */
   DSL_uint16_t SNRMT;
   /** SNR Subcarrier Group Size DS "SNRGus" */
   DSL_uint16_t SNRG;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** HLIN Scaling Factor "HLINSCus" */
   DSL_uint16_t HLINSC;
   /** HLIN Subcarrier Group Size DS "HLINGus" */
   DSL_uint16_t HLING;
   /** HLOG Measurement Time "HLOGMTus" */
   DSL_uint16_t HLOGMT;
   /** HLOG Subcarrier Group Size DS "HLOGGus" */
   DSL_uint16_t HLOGG;
   /** QLN Measurment Time "QLNMTus" */
   DSL_uint16_t QLNMT;
   /** QLN Subcarrier Group Size DS "QLNGus" */
   DSL_uint16_t QLNG;
   /** SNR Measurement Time "SNRMTus" */
   DSL_uint16_t SNRMT;
   /** SNR Subcarrier Group Size DS "SNRGus" */
   DSL_uint16_t SNRG;
#endif
} __PACKED__ ;


/**
   This message is used to trigger retrieval of the far-end PMD Test Parameters
   Hlog, SNR and QLN via the EOC channel during Showtime. If StartIndex or
   EndIndex do not contain values in a valid range then no autonomous message
   will be sent by the firmware. Once the retrieval process is started, it can
   also be aborted with this message via the control parameter. The status of
   the parameter retrieval process can be polled via the CMD_TestParamsFE_Poll
   message. Once the parameters are retrieved, they are provided with the
   EVT_PMD_TestParamsGet message.
*/
struct CMD_TestParamsFE_Request
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Far-end Retrieval Control */
   DSL_uint16_t Control;
   /** Subcarrier Group Start Index */
   DSL_uint16_t StartIndex;
   /** Subcarrier Group End Index */
   DSL_uint16_t EndIndex;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Far-end Retrieval Control */
   DSL_uint16_t Control;
   /** Subcarrier Group Start Index */
   DSL_uint16_t StartIndex;
   /** Subcarrier Group End Index */
   DSL_uint16_t EndIndex;
#endif
} __PACKED__ ;


/**
   This is the acknowledgement for CMD_TestParamsFE_Request.
*/
struct ACK_TestParamsFE_Request
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This message polls the status of the far end parameter retrieval process.
*/
struct CMD_TestParamsFE_Poll
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This is the acknowledgement for CMD_TestParamsFE_Poll.
*/
struct ACK_TestParamsFE_Poll
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Far-end retrieval status */
   DSL_uint16_t Status;
   /** Reserved1 */
   DSL_uint16_t Res1;
   /** Reserved2 */
   DSL_uint16_t Res2;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Far-end retrieval status */
   DSL_uint16_t Status;
   /** Reserved1 */
   DSL_uint16_t Res1;
   /** Reserved2 */
   DSL_uint16_t Res2;
#endif
} __PACKED__ ;


/**
   This event message provides the far-end data after the far end data retrieval
   process. This message is a result of the CMD_TestParamsFE_Request message.
*/
struct EVT_PMD_TestParamsGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved1 */
   DSL_uint16_t Res1;
   /** Subcarrier Group Start Index */
   DSL_uint16_t StartIndex;
   /** Subcarrier Group End Index */
   DSL_uint16_t EndIndex;
   /** HLOG Measurement Time HLOGMT */
   DSL_uint16_t hlogTime;
   /** SNR Measurement Time SNRMT */
   DSL_uint16_t snrTime;
   /** QLN Measurment Time QLNMT */
   DSL_uint16_t qlnTime;
   /** Testparameter Result */
   VRX_TestParam_t TestPar[60];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved1 */
   DSL_uint16_t Res1;
   /** Subcarrier Group Start Index */
   DSL_uint16_t StartIndex;
   /** Subcarrier Group End Index */
   DSL_uint16_t EndIndex;
   /** HLOG Measurement Time HLOGMT */
   DSL_uint16_t hlogTime;
   /** SNR Measurement Time SNRMT */
   DSL_uint16_t snrTime;
   /** QLN Measurment Time QLNMT */
   DSL_uint16_t qlnTime;
   /** Testparameter Result */
   VRX_TestParam_t TestPar[60];
#endif
} __PACKED__ ;


/**
   The message is used to configure the autonomous messaging related to Clear
   EOC transmission.
*/
struct CMD_ClearEOC_Configure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 13;
   /** Rx Autonomous Clear EOC Data Message Control */
   DSL_uint16_t RxEVTdata : 1;
   /** Rx Autonomous Status Message Control */
   DSL_uint16_t RxEVTstatus : 1;
   /** Tx Autonomous Status Message Control */
   DSL_uint16_t TxEVTstatus : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Tx Autonomous Status Message Control */
   DSL_uint16_t TxEVTstatus : 1;
   /** Rx Autonomous Status Message Control */
   DSL_uint16_t RxEVTstatus : 1;
   /** Rx Autonomous Clear EOC Data Message Control */
   DSL_uint16_t RxEVTdata : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 13;
#endif
} __PACKED__ ;


/**
   This is the acknowledgement for CMD_ClearEOC_Configure.
*/
struct ACK_ClearEOC_Configure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   The message is used to trigger the transmission of  Clear EOC messages that
   were placed into the Clear EOC transmit buffer before with
   CMD_ClearEOC_Write.
*/
struct CMD_ClearEOC_TxTrigger
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 15;
   /** Transmit Control Trigger */
   DSL_uint16_t txTrigger : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Transmit Control Trigger */
   DSL_uint16_t txTrigger : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 15;
#endif
} __PACKED__ ;


/**
   This is the acknowledgement for CMD_ClearEOC_TxTrigger.
*/
struct ACK_ClearEOC_TxTrigger
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This message is used to write data to the ClearEOC write buffer of type
   VRX_ClearEOC_t. When the buffer is filled, the transmission is started
   applying CMD_ClearEOC_TxTrigger. If the message to transmit is longer than
   the mailbox size, a sequence of writes to the ClearEOC buffer has to be done
   before the transmission is started with CMD_ClearEOC_TxTrigger. When
   autonomous Tx status messaging is activated via CMD_ClearEOC_Configure, then
   the finished transmission is indicated by EVT_ClearEOCStatusGet.
*/
struct CMD_ClearEOC_Write
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Data */
   DSL_uint16_t Data[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Data */
   DSL_uint16_t Data[128];
#endif
} __PACKED__ ;


/**
   This message is the acknowledgement for CMD_ClearEOC_Write.
*/
struct ACK_ClearEOC_Write
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This message is used to read data from the ClearEOC buffer of type
   VRX_ClearEOC_t. The length of the actual Clear EOC message can be found in
   the buffer. Please refer to VRX_ClearEOC_t. The availability of data can
   either be checked via CMD_ClearEOCStatusGet in polling mode or it can be
   reported by an autonomous EVT_ClearEOCStatusGet message when data is received
   (to be enabled using CMD_ClearEOC_Configure).
*/
struct CMD_ClearEOC_Read
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This message is the acknowledgement to CMD_ClearEOC_Read.
*/
struct ACK_ClearEOC_Read
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Data */
   DSL_uint16_t Data[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Data */
   DSL_uint16_t Data[128];
#endif
} __PACKED__ ;


/**
   This message is an autonomous message that is generated when ClearEOC data
   was received and autonomous Clear EOC data messaging has been activated via
   CMD_ClearEOC_Configure. If the ClearEOC data does not fit in one message,
   then a sequence of messages is generated. The ClearEOC buffer is of type
   VRX_ClearEOC_t.
*/
struct EVT_ClearEOC_Read
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Data */
   DSL_uint16_t Data[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Data */
   DSL_uint16_t Data[128];
#endif
} __PACKED__ ;


/**
   This message is used to retrieve the status of the clear eoc data
   transmission.
*/
struct CMD_ClearEOCStatusGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This is the acknowledgement for CMD_ClearEOCStatusGet.
*/
struct ACK_ClearEOCStatusGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 14;
   /** Transmit Status */
   DSL_uint16_t txstat : 2;
   /** Reserved */
   DSL_uint16_t Res1 : 14;
   /** Receive Status */
   DSL_uint16_t rxstat : 2;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Transmit Status */
   DSL_uint16_t txstat : 2;
   /** Reserved */
   DSL_uint16_t Res0 : 14;
   /** Receive Status */
   DSL_uint16_t rxstat : 2;
   /** Reserved */
   DSL_uint16_t Res1 : 14;
#endif
} __PACKED__ ;


/**
   This autonomous message reports the Clear EOC status. It is sent only if the
   "Autonomous Status Message Control" was enabled for Tx and/or Rx direction
   with CMD_ClearEOC_Configure. If Tx direction is enabled, the message is
   generated when a Tx transmission is finished or failed. If Rx direction is
   enabled, the message is generated when the Rx status transitions from "Idle"
   to "Data Available" for retrieval by the host.
*/
struct EVT_ClearEOCStatusGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 14;
   /** Transmit Status */
   DSL_uint16_t txstat : 2;
   /** Reserved */
   DSL_uint16_t Res1 : 14;
   /** Receive Status */
   DSL_uint16_t rxstat : 2;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Transmit Status */
   DSL_uint16_t txstat : 2;
   /** Reserved */
   DSL_uint16_t Res0 : 14;
   /** Receive Status */
   DSL_uint16_t rxstat : 2;
   /** Reserved */
   DSL_uint16_t Res1 : 14;
#endif
} __PACKED__ ;


/**
   The message is used to reset the transmit or receive status of the clear eoc
   data transmission to IDLE (for defined states see also
   CMD_ClearEOCStatusGet). See the description on the Clear EOC handling on Page
   491 for when it has to be applied. Transmit and receive status are
   distinguished by the Index parameter.
*/
struct CMD_ClearEOCStatusSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 14;
   /** Status */
   DSL_uint16_t stat : 2;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Status */
   DSL_uint16_t stat : 2;
   /** Reserved */
   DSL_uint16_t Res0 : 14;
#endif
} __PACKED__ ;


/**
   This is the acknowledgement for CMD_ClearEOCStatusSet.
*/
struct ACK_ClearEOCStatusSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Configuration of options for the overhead handling.
*/
struct CMD_OH_OptionsSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** EOC Polling Period Scaling Factor, Bits 15 to 8 */
   DSL_uint8_t eocPollFactor;
   /** Reserved */
   DSL_uint16_t Res0 : 3;
   /** EOC Polling Control Prio 2: PMD Test Parameters, Bit 4 */
   DSL_uint16_t eocPoll2 : 1;
   /** EOC Polling Control Prio 1: Inventory, Counters, Bit 3 */
   DSL_uint16_t eocPoll1 : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 3;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res1 : 3;
   /** EOC Polling Control Prio 1: Inventory, Counters, Bit 3 */
   DSL_uint16_t eocPoll1 : 1;
   /** EOC Polling Control Prio 2: PMD Test Parameters, Bit 4 */
   DSL_uint16_t eocPoll2 : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 3;
   /** EOC Polling Period Scaling Factor, Bits 15 to 8 */
   DSL_uint8_t eocPollFactor;
#endif
} __PACKED__ ;


/**
   Acknowledgement for CMD_OH_OptionsSet.
*/
struct ACK_OH_OptionsSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Requests OH polling statistic information.
*/
struct CMD_OH_StatsGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Reports the OH polling statistics as requested by CMD_OH_StatsGet.
*/
struct ACK_OH_StatsGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** EOC CMD Tx PRIO1 Sent */
   DSL_uint16_t eocCMD1;
   /** EOC ACK Rx PRIO1 Count */
   DSL_uint16_t eocRSP1;
   /** EOC CMD Tx PRIO2 Sent */
   DSL_uint16_t eocCMD2;
   /** EOC ACK Rx PRIO2 Count */
   DSL_uint16_t eocRSP2;
   /** EOC NACK Rx PRIO1 Count  */
   DSL_uint16_t eocNACK1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** EOC CMD Tx PRIO1 Sent */
   DSL_uint16_t eocCMD1;
   /** EOC ACK Rx PRIO1 Count */
   DSL_uint16_t eocRSP1;
   /** EOC CMD Tx PRIO2 Sent */
   DSL_uint16_t eocCMD2;
   /** EOC ACK Rx PRIO2 Count */
   DSL_uint16_t eocRSP2;
   /** EOC NACK Rx PRIO1 Count  */
   DSL_uint16_t eocNACK1;
#endif
} __PACKED__ ;


/**
   Enables/Disables support for OLR events (Bitswaps, SRA, SOS, ROC).An OLR
   transition is always initiated by the receiving PMD, so the CPE requests the
   OLR event for downstream direction.
*/
struct CMD_OLR_Control
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 4;
   /** ROC DS */
   DSL_uint16_t ROC_DS : 1;
   /** ROC US */
   DSL_uint16_t ROC_US : 1;
   /** SOS DS */
   DSL_uint16_t SOS_DS : 1;
   /** SOS US */
   DSL_uint16_t SOS_US : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 2;
   /** RX Bitswap */
   DSL_uint16_t RxBitswap : 1;
   /** TX Bitswap */
   DSL_uint16_t TxBitswap : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 1;
   /** Auto-SRA DS */
   DSL_uint16_t autoSRA_DS : 1;
   /** Auto-SRA US */
   DSL_uint16_t autoSRA_US : 1;
   /** Reserved */
   DSL_uint16_t Res3 : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res3 : 1;
   /** Auto-SRA US */
   DSL_uint16_t autoSRA_US : 1;
   /** Auto-SRA DS */
   DSL_uint16_t autoSRA_DS : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 1;
   /** TX Bitswap */
   DSL_uint16_t TxBitswap : 1;
   /** RX Bitswap */
   DSL_uint16_t RxBitswap : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 2;
   /** SOS US */
   DSL_uint16_t SOS_US : 1;
   /** SOS DS */
   DSL_uint16_t SOS_DS : 1;
   /** ROC US */
   DSL_uint16_t ROC_US : 1;
   /** ROC DS */
   DSL_uint16_t ROC_DS : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 4;
#endif
} __PACKED__ ;


/**
   Acknowledgement for CMD_OLR_Control.
*/
struct ACK_OLR_Control
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Requests the OLR status information on bit swaps, DRR, SRA and SOS events for
   the upstream direction.
*/
struct CMD_OLR_US_StatsGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Reports the OLR status information for the upstream direction as requested by
   CMD_OLR_US_StatsGet.
*/
struct ACK_OLR_US_StatsGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** US Bit Swap Requests */
   DSL_uint16_t BitswapReqs;
   /** US Extended Bit Swap Requests */
   DSL_uint16_t ExtBitswapReqs;
   /** US Bit Swap UTC Responses */
   DSL_uint16_t BitswapUTCs;
   /** US "Bit Swaps Performed" Count */
   DSL_uint16_t BitswapsDone;
   /** Reserved */
   DSL_uint16_t Res0;
   /** Reserved for DRR */
   DSL_uint16_t Res1[5];
   /** US SRA Requests */
   DSL_uint16_t SRA_Reqs;
   /** Reserved */
   DSL_uint16_t Res2;
   /** US SRA UTC Responses */
   DSL_uint16_t SRA_UTCs;
   /** US "SRA Performed" Count */
   DSL_uint16_t SRAsDone;
   /** Reserved */
   DSL_uint16_t Res3;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** US Bit Swap Requests */
   DSL_uint16_t BitswapReqs;
   /** US Extended Bit Swap Requests */
   DSL_uint16_t ExtBitswapReqs;
   /** US Bit Swap UTC Responses */
   DSL_uint16_t BitswapUTCs;
   /** US "Bit Swaps Performed" Count */
   DSL_uint16_t BitswapsDone;
   /** Reserved */
   DSL_uint16_t Res0;
   /** Reserved for DRR */
   DSL_uint16_t Res1[5];
   /** US SRA Requests */
   DSL_uint16_t SRA_Reqs;
   /** Reserved */
   DSL_uint16_t Res2;
   /** US SRA UTC Responses */
   DSL_uint16_t SRA_UTCs;
   /** US "SRA Performed" Count */
   DSL_uint16_t SRAsDone;
   /** Reserved */
   DSL_uint16_t Res3;
#endif
} __PACKED__ ;


/**
   Requests the OLR status information on bit swaps, DRR, SRA and SOS events for
   the downstream direction.
*/
struct CMD_OLR_DS_StatsGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Reports the OLR status information for the downstream direction as requested
   by CMD_OLR_DS_StatsGet.
*/
struct ACK_OLR_DS_StatsGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** DS Bit Swap Requests */
   DSL_uint16_t BitswapReqs;
   /** DS Extended Bit Swap Requests */
   DSL_uint16_t ExtBitswapReqs;
   /** DS Bit Swap UTC Responses */
   DSL_uint16_t BitswapUTCs;
   /** DS "Bit Swaps Performed" Count */
   DSL_uint16_t BitswapsDone;
   /** DS Bitswap Timeouts */
   DSL_uint16_t BitswapTimeOuts;
   /** Reserved for DRR */
   DSL_uint16_t Res0[5];
   /** DS SRA Requests */
   DSL_uint16_t SRA_Reqs;
   /** Reserved */
   DSL_uint16_t Res1;
   /** DS SRA UTC Responses */
   DSL_uint16_t SRA_UTCs;
   /** DS "SRA Performed" Count */
   DSL_uint16_t SRAsDone;
   /** DS SRA Timeouts */
   DSL_uint16_t SRA_TimeOuts;
   /** DS SOS Requests */
   DSL_uint16_t SOS_Reqs;
   /** Reserved */
   DSL_uint16_t Res2;
   /** DS SOS UTC Responses */
   DSL_uint16_t SOS_UTCs;
   /** DS "SOS Performed" Count */
   DSL_uint16_t SOS_Done;
   /** DS SOS Timeouts */
   DSL_uint16_t SOS_TimeOuts;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** DS Bit Swap Requests */
   DSL_uint16_t BitswapReqs;
   /** DS Extended Bit Swap Requests */
   DSL_uint16_t ExtBitswapReqs;
   /** DS Bit Swap UTC Responses */
   DSL_uint16_t BitswapUTCs;
   /** DS "Bit Swaps Performed" Count */
   DSL_uint16_t BitswapsDone;
   /** DS Bitswap Timeouts */
   DSL_uint16_t BitswapTimeOuts;
   /** Reserved for DRR */
   DSL_uint16_t Res0[5];
   /** DS SRA Requests */
   DSL_uint16_t SRA_Reqs;
   /** Reserved */
   DSL_uint16_t Res1;
   /** DS SRA UTC Responses */
   DSL_uint16_t SRA_UTCs;
   /** DS "SRA Performed" Count */
   DSL_uint16_t SRAsDone;
   /** DS SRA Timeouts */
   DSL_uint16_t SRA_TimeOuts;
   /** DS SOS Requests */
   DSL_uint16_t SOS_Reqs;
   /** Reserved */
   DSL_uint16_t Res2;
   /** DS SOS UTC Responses */
   DSL_uint16_t SOS_UTCs;
   /** DS "SOS Performed" Count */
   DSL_uint16_t SOS_Done;
   /** DS SOS Timeouts */
   DSL_uint16_t SOS_TimeOuts;
#endif
} __PACKED__ ;


/**
   Requests the actual active downstream RA mode (ACT-RA-MODEds, Section
   7.5.1.33.1 of G.997.1).
*/
struct CMD_RA_ModeDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Reports the actual active downstream RA mode (ACT-RA-MODEds) as requested by
   CMD_RA_ModeDS_Get.
*/
struct ACK_RA_ModeDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** RA Mode DS */
   DSL_uint16_t actRA_mode;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** RA Mode DS */
   DSL_uint16_t actRA_mode;
#endif
} __PACKED__ ;


/**
   Requests the actual active upstream RA mode (ACT-RA-MODEus, Section
   7.5.1.33.2 of G.997.1).
*/
struct CMD_RA_ModeUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Reports the actual active upstream RA mode (ACT-RA-MODEus) as requested by
   CMD_RA_ModeUS_Get.
*/
struct ACK_RA_ModeUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** RA Mode US */
   DSL_uint16_t actRA_mode;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** RA Mode US */
   DSL_uint16_t actRA_mode;
#endif
} __PACKED__ ;


/**
   Autonomous message indicating a successful upstream OLR event (SRA or SOS).
*/
struct EVT_OLR_US_EventGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 14;
   /** SRA Event */
   DSL_uint16_t SRA_OK : 1;
   /** SOS Event (VDSL only) */
   DSL_uint16_t SOS_OK : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** SOS Event (VDSL only) */
   DSL_uint16_t SOS_OK : 1;
   /** SRA Event */
   DSL_uint16_t SRA_OK : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 14;
#endif
} __PACKED__ ;


/**
   Autonomous message indicating a successful downstream OLR event (SRA or SOS).
*/
struct EVT_OLR_DS_EventGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 14;
   /** SRA Event */
   DSL_uint16_t SRA_OK : 1;
   /** SOS Event (VDSL only) */
   DSL_uint16_t SOS_OK : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** SOS Event (VDSL only) */
   DSL_uint16_t SOS_OK : 1;
   /** SRA Event */
   DSL_uint16_t SRA_OK : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 14;
#endif
} __PACKED__ ;


/**
   Enables/Disables the generation of EVENT messages (EVT) for specific upstream
   OLR events. If the corresponding Enable bit for an OLR event is set, then the
   modem firmware will send an autonomous message EVT_OLR_US_EventGet if the OLR
   event happened in the last 1-second interval.
*/
struct CMD_OLR_US_EventConfigure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 14;
   /** SRA Event US, Bit 1 */
   DSL_uint16_t SRA_OK : 1;
   /** SOS Event US , Bit 0 (VDSL only) */
   DSL_uint16_t SOS_OK : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** SOS Event US , Bit 0 (VDSL only) */
   DSL_uint16_t SOS_OK : 1;
   /** SRA Event US, Bit 1 */
   DSL_uint16_t SRA_OK : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 14;
#endif
} __PACKED__ ;


/**
   Acknoledgement for CMD_OLR_US_EventConfigure.
*/
struct ACK_OLR_US_EventConfigure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Enables/Disables the generation of EVENT messages (EVT) for specific
   downstream OLR events. If the corresponding Enable bit for an OLR event is
   set, then the modem firmware will send an autonomous message
   EVT_OLR_DS_EventGet if the OLR event happened in the last 1-second interval.
*/
struct CMD_OLR_DS_EventConfigure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 14;
   /** SRA Event DS, Bit 1 */
   DSL_uint16_t SRA_OK : 1;
   /** SOS Event DS , Bit 0 (VDSL only) */
   DSL_uint16_t SOS_OK : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** SOS Event DS , Bit 0 (VDSL only) */
   DSL_uint16_t SOS_OK : 1;
   /** SRA Event DS, Bit 1 */
   DSL_uint16_t SRA_OK : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 14;
#endif
} __PACKED__ ;


/**
   Acknoledgement for CMD_OLR_DS_EventConfigure.
*/
struct ACK_OLR_DS_EventConfigure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Configures a link for retransmission of downstream data. For using the RTX
   function, this message has to be sent.
*/
struct CMD_RTX_DS_Configure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 15;
   /** Retransmission Enable */
   DSL_uint16_t RtxMode : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Retransmission Enable */
   DSL_uint16_t RtxMode : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 15;
#endif
} __PACKED__ ;


/**
   Acknowledgement for CMD_RTX_DS_Configure.
*/
struct ACK_RTX_DS_Configure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Requests RTX specific status information for the downstream bearer channels
   if G.INP retransmission is used.
*/
struct CMD_BearerChsDS_RTX_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers status information for the downstream bearer channels when G.INP
   retransmission is used.
*/
struct ACK_BearerChsDS_RTX_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Expected Throughput ETR of RTX Function, LSW */
   DSL_uint16_t ETR_LSW;
   /** Expected Throughput ETR of RTX Function, MSW */
   DSL_uint16_t ETR_MSW;
   /** Actual Delay of RTX Function */
   DSL_uint16_t ActDelay;
   /** Actual INP SHINE of RTX Function */
   DSL_uint16_t ActInpSHINE;
   /** Actual INP REIN of RTX Function */
   DSL_uint16_t ActInpREIN;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Expected Throughput ETR of RTX Function, LSW */
   DSL_uint16_t ETR_LSW;
   /** Expected Throughput ETR of RTX Function, MSW */
   DSL_uint16_t ETR_MSW;
   /** Actual Delay of RTX Function */
   DSL_uint16_t ActDelay;
   /** Actual INP SHINE of RTX Function */
   DSL_uint16_t ActInpSHINE;
   /** Actual INP REIN of RTX Function */
   DSL_uint16_t ActInpREIN;
#endif
} __PACKED__ ;


/**
   Requests counters for RTX performance monitoring which don’t include TR1
   threshold supervision.
*/
struct CMD_RTX_PMwoThreshDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers retransmission counters for RTX performance monitoring which don’t
   include TR1 threshold supervision.
*/
struct ACK_RTX_PMwoThreshDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** ErrorFreeBitsCNT, LSW, Current Period */
   DSL_uint16_t ErrorFreeBits_LSW;
   /** ErrorFreeBitsCNT, MSW, Current Period */
   DSL_uint16_t ErrorFreeBits_MSW;
   /** Reserved */
   DSL_uint16_t Res0[2];
   /** EFTR_min, LSW, Current Period */
   DSL_uint16_t EFTR_min_LSW;
   /** EFTR_min, MSW, Current Period */
   DSL_uint16_t EFTR_min_MSW;
   /** Reserved */
   DSL_uint16_t Res1[2];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** ErrorFreeBitsCNT, LSW, Current Period */
   DSL_uint16_t ErrorFreeBits_LSW;
   /** ErrorFreeBitsCNT, MSW, Current Period */
   DSL_uint16_t ErrorFreeBits_MSW;
   /** Reserved */
   DSL_uint16_t Res0[2];
   /** EFTR_min, LSW, Current Period */
   DSL_uint16_t EFTR_min_LSW;
   /** EFTR_min, MSW, Current Period */
   DSL_uint16_t EFTR_min_MSW;
   /** Reserved */
   DSL_uint16_t Res1[2];
#endif
} __PACKED__ ;


/**
   Requests retransmission test and debug wrap-around counters.
*/
struct CMD_RTX_DS_StatsGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers retransmission test and debug wrap-around counters.
*/
struct ACK_RTX_DS_StatsGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** TxDtuRetransmitted Count, LSW */
   DSL_uint16_t TxDtuRTX_LSW;
   /** TxDtuRetransmitted Count, MSW */
   DSL_uint16_t TxDtuRTX_MSW;
   /** RxDtuCorrected Count, LSW */
   DSL_uint16_t RxDtuCorr_LSW;
   /** RxDtuCorrected Count, MSW */
   DSL_uint16_t RxDtuCorr_MSW;
   /** RxDtuUncorrected Count, LSW */
   DSL_uint16_t RxDtuNoCorr_LSW;
   /** RxDtuUncorrected Count, MSW */
   DSL_uint16_t RxDtuNoCorr_MSW;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** TxDtuRetransmitted Count, LSW */
   DSL_uint16_t TxDtuRTX_LSW;
   /** TxDtuRetransmitted Count, MSW */
   DSL_uint16_t TxDtuRTX_MSW;
   /** RxDtuCorrected Count, LSW */
   DSL_uint16_t RxDtuCorr_LSW;
   /** RxDtuCorrected Count, MSW */
   DSL_uint16_t RxDtuCorr_MSW;
   /** RxDtuUncorrected Count, LSW */
   DSL_uint16_t RxDtuNoCorr_LSW;
   /** RxDtuUncorrected Count, MSW */
   DSL_uint16_t RxDtuNoCorr_MSW;
#endif
} __PACKED__ ;


/**
   Requests the actually used G.INP retransmission status.
*/
struct CMD_RTX_StatusGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Provides the actually used G.INP retransmission status.
*/
struct ACK_RTX_StatusGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Retransmission Used "RTX_USED" */
   DSL_uint16_t RtxUsed;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Retransmission Used "RTX_USED" */
   DSL_uint16_t RtxUsed;
#endif
} __PACKED__ ;




#ifdef __cplusplus
}
#endif

#ifdef __PACKED_DEFINED__
   #if !(defined (__GNUC__) || defined (__GNUG__))
      #pragma pack()
   #endif
   #undef __PACKED_DEFINED__
#endif /* __PACKED_DEFINED__ */

/** @} */

#endif /** _DRV_DSL_CPE_VRX_MSG_OLR_H_*/
