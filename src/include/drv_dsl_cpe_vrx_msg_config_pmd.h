/******************************************************************************

                              Copyright (c) 2012
                            Lantiq Deutschland GmbH
                     Am Campeon 3; 85579 Neubiberg, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/


#ifndef _DRV_DSL_CPE_VRX_MSG_CONFIG_PMD_H_
#define _DRV_DSL_CPE_VRX_MSG_CONFIG_PMD_H_

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


/** @defgroup _CONFIG_PMD_
 *  @{
 */

#ifdef __cplusplus
   extern "C" {
#endif

/* ----- Message Specific Constants Definition section ----- */
#define ACK_ModemFSM_StateGet_RESET_STATE 0
#define ACK_ModemFSM_StateGet_READY_STATE 1
#define ACK_ModemFSM_StateGet_FAIL_STATE 2
#define ACK_ModemFSM_StateGet_DIAG_COMPLETE_STATE 3
#define ACK_ModemFSM_StateGet_GHS_STATE 5
#define ACK_ModemFSM_StateGet_FULLINIT_STATE 6
#define ACK_ModemFSM_StateGet_STEADY_STATE_TC_NOSYNC 7
#define ACK_ModemFSM_StateGet_STEADY_STATE_TC_SYNC 8
#define ACK_ModemFSM_StateGet_DIAGMODE_STATE 9
#define ACK_ModemFSM_StateGet_T1413_STATE 14
#define ACK_ModemFSM_StateGet_GHS_BONDING_CLR_STATE 16
#define ACK_ModemFSM_StateGet_MFD_STATE 18
#define ACK_ModemFSM_StateGet_MFD_COMPLETE_STATE 19
#define ACK_ModemFSM_StateGet_TEST_STATE 240
#define ACK_ModemFSM_StateGet_L0 0
#define ACK_ModemFSM_StateGet_L2 2
#define ACK_ModemFSM_StateGet_L3 3
#define EVT_ModemFSM_StateGet_RESET_STATE 0
#define EVT_ModemFSM_StateGet_READY_STATE 1
#define EVT_ModemFSM_StateGet_FAIL_STATE 2
#define EVT_ModemFSM_StateGet_DIAG_COMPLETE_STATE 3
#define EVT_ModemFSM_StateGet_GHS_STATE 5
#define EVT_ModemFSM_StateGet_FULLINIT_STATE 6
#define EVT_ModemFSM_StateGet_STEADY_STATE_TC_NOSYNC 7
#define EVT_ModemFSM_StateGet_STEADY_STATE_TC_SYNC 8
#define EVT_ModemFSM_StateGet_DIAGMODE_STATE 9
#define EVT_ModemFSM_StateGet_T1413_STATE 14
#define EVT_ModemFSM_StateGet_GHS_BONDING_CLR_STATE 16
#define EVT_ModemFSM_StateGet_MFD_STATE 18
#define EVT_ModemFSM_StateGet_MFD_COMPLETE_STATE 19
#define EVT_ModemReady_MRERR_OK 0x0
#define EVT_ModemReady_MRWARN_OK 0x0
#define CMD_ModemFSM_StateSet_LINKRES 0
#define CMD_ModemFSM_StateSet_LINKINI 2
#define CMD_ModemFSM_StateSet_TESTSTA 8
#define CMD_ModemFSM_StateSet_MFD 9
#define ALM_ModemFSM_FailReasonGet_S_OK 0x0
#define ALM_ModemFSM_FailReasonGet_S_CODESWAP 0x1
#define ALM_ModemFSM_FailReasonGet_S_MINRATE_DS 0x4
#define ALM_ModemFSM_FailReasonGet_S_INIT 0x5
#define ALM_ModemFSM_FailReasonGet_S_MODE 0x6
#define ALM_ModemFSM_FailReasonGet_S_TIMEOUT 0x9
#define ALM_ModemFSM_FailReasonGet_S_XDSL_MODE 0xA
#define ALM_ModemFSM_FailReasonGet_S_PORT_MODE 0xB
#define ALM_ModemFSM_FailReasonGet_S_FW_RETRY 0xE
#define ALM_ModemFSM_FailReasonGet_S_FW_HYBRID 0xF
#define ALM_ModemFSM_FailReasonGet_S_LOS 0x10
#define ALM_ModemFSM_FailReasonGet_S_LOF 0x11
#define ALM_ModemFSM_FailReasonGet_S_LPR 0x12
#define ALM_ModemFSM_FailReasonGet_S_LOM 0x13
#define ALM_ModemFSM_FailReasonGet_S_ESE 0x15
#define ALM_ModemFSM_FailReasonGet_S_OOS0 0x18
#define ALM_ModemFSM_FailReasonGet_S_LCD0 0x20
#define ALM_ModemFSM_FailReasonGet_S_NCD0 0x22
#define ALM_ModemFSM_FailReasonGet_S_CRC_DS 0x24
#define ALM_ModemFSM_FailReasonGet_E_OK 0x0
#define ALM_ModemFSM_FailReasonGet_E_CONFIG 0x1
#define ALM_ModemFSM_FailReasonGet_E_NOTFEASIBLE 0x2
#define ALM_ModemFSM_FailReasonGet_E_COMM 0x3
#define ALM_ModemFSM_FailReasonGet_E_COMMCRC 0x4
#define ALM_ModemFSM_FailReasonGet_E_NOPEER 0x5
#define ALM_ModemFSM_FailReasonGet_E_CHIPSET 0x6
#define ALM_ModemFSM_FailReasonGet_E_UNKNOWN 0x7
#define ALM_ModemFSM_FailReasonGet_E_RESERVED 0x8
#define ALM_ModemFSM_FailReasonGet_E_SHOWTIME_NE 0x10
#define ALM_ModemFSM_FailReasonGet_E_SHOWTIME_FE 0x11
#define ACK_ModemFSM_FailReasonGet_S_OK 0x0
#define ACK_ModemFSM_FailReasonGet_S_CODESWAP 0x1
#define ACK_ModemFSM_FailReasonGet_S_MINRATE_DS 0x4
#define ACK_ModemFSM_FailReasonGet_S_INIT 0x5
#define ACK_ModemFSM_FailReasonGet_S_MODE 0x6
#define ACK_ModemFSM_FailReasonGet_S_TIMEOUT 0x9
#define ACK_ModemFSM_FailReasonGet_S_XDSL_MODE 0xA
#define ACK_ModemFSM_FailReasonGet_S_PORT_MODE 0xB
#define ACK_ModemFSM_FailReasonGet_S_FW_RETRY 0xE
#define ACK_ModemFSM_FailReasonGet_S_FW_HYBRID 0xF
#define ACK_ModemFSM_FailReasonGet_S_LOS 0x10
#define ACK_ModemFSM_FailReasonGet_S_LOF 0x11
#define ACK_ModemFSM_FailReasonGet_S_LPR 0x12
#define ACK_ModemFSM_FailReasonGet_S_LOM 0x13
#define ACK_ModemFSM_FailReasonGet_S_ESE 0x15
#define ACK_ModemFSM_FailReasonGet_S_OOS0 0x18
#define ACK_ModemFSM_FailReasonGet_S_LCD0 0x20
#define ACK_ModemFSM_FailReasonGet_S_NCD0 0x22
#define ACK_ModemFSM_FailReasonGet_S_CRC_DS 0x24
#define ACK_ModemFSM_FailReasonGet_E_OK 0x0
#define ACK_ModemFSM_FailReasonGet_E_CONFIG 0x1
#define ACK_ModemFSM_FailReasonGet_E_NOTFEASIBLE 0x2
#define ACK_ModemFSM_FailReasonGet_E_COMM 0x3
#define ACK_ModemFSM_FailReasonGet_E_COMMCRC 0x4
#define ACK_ModemFSM_FailReasonGet_E_NOPEER 0x5
#define ACK_ModemFSM_FailReasonGet_E_CHIPSET 0x6
#define ACK_ModemFSM_FailReasonGet_E_UNKNOWN 0x7
#define ACK_ModemFSM_FailReasonGet_E_RESERVED 0x8
#define ACK_ModemFSM_FailReasonGet_E_SHOWTIME_NE 0x10
#define ACK_ModemFSM_FailReasonGet_E_SHOWTIME_FE 0x11
#define ACK_TxL3RequestStatusGet_L3PENDING 0
#define ACK_TxL3RequestStatusGet_L3REJECTED 1
#define ACK_TxL3RequestStatusGet_L3ACCEPTED 2
#define ACK_TxL3RequestStatusGet_L3FAIL 3
#define ACK_TxL3ReqFailReasonGet_NOT_L0 5
#define ACK_TxL3ReqFailReasonGet_TIMEOUT 9
#define ACK_FW_ImageInfoGet_STANDALONE 0x0
#define ACK_FW_ImageInfoGet_COMBINED 0x1
#define ACK_FW_ImageInfoGet_VDSL_FW 0x0
#define ACK_FW_ImageInfoGet_ADSL_FW 0x1
#define ACK_FW_ImageInfoGet_CPE_FW 0x1
#define CMD_BearerCh0_DS_Set_FV 19
#define CMD_BearerCh0_DS_Set_MAX_BER3 0
#define CMD_BearerCh0_DS_Set_MAX_BER5 1
#define CMD_BearerCh0_DS_Set_MAX_BER7 2
#define ACK_BearerCh0_DS_Set_FV 19
#define CMD_BearerCh0_US_Set_FV 19
#define CMD_BearerCh0_US_Set_MAX_BER3 0
#define CMD_BearerCh0_US_Set_MAX_BER5 1
#define CMD_BearerCh0_US_Set_MAX_BER7 2
#define ACK_BearerCh0_US_Set_FV 19
#define CMD_InitPolicySet_MAX_NETRATE 0x0
#define CMD_InitPolicySet_MAX_INP 0x1
#define CMD_ErasureControlSet_MAX_NETRATE 0x0
#define CMD_ErasureControlSet_MAX_INP 0x1
#define CMD_Misc_ConfigSet_GHSMODE 0x0
#define CMD_Misc_ConfigSet_T1413MODE 0x1
/* ----- Message Specific Constants Definition section (End) ----- */

/** Message ID for CMD_ModemFSM_StateGet */
#define CMD_MODEMFSM_STATEGET 0x0002

/**
   Requests information about the current state of the modem state-machine.The
   command can be sent in all states of the modem state machine (see Figure 2).
*/
typedef struct CMD_ModemFSM_StateGet CMD_ModemFSM_StateGet_t;

/** Message ID for ACK_ModemFSM_StateGet */
#define ACK_MODEMFSM_STATEGET 0x0002

/**
   Returns information about the current state of the modem state-machine.
*/
typedef struct ACK_ModemFSM_StateGet ACK_ModemFSM_StateGet_t;

/** Message ID for EVT_ModemFSM_StateGet */
#define EVT_MODEMFSM_STATEGET 0x0002

/**
   Returns information about the current state of the modem state-machine. This
   message is sent autonomously, if the modem enters a new state and reporting
   is enabled for that state (see CMD_ModemFSM_EventConfigure).
*/
typedef struct EVT_ModemFSM_StateGet EVT_ModemFSM_StateGet_t;

/** Message ID for EVT_ModemReady */
#define EVT_MODEMREADY 0xFF02

/**
   Autonomous message indicating that the modem online code was reached and that
   initialization was completed. The management entity may not perform any
   query-response messaging until the EVT_ModemReady message is received. Its
   generation cannot be disabled. If it is not received this indicates a
   problem, possibly with the boot sequence. If the message is received, it
   contains a result code that may also indicate that an error was detected by
   the firmware during initialization.
*/
typedef struct EVT_ModemReady EVT_ModemReady_t;

/** Message ID for CMD_ModemFSM_StateSet */
#define CMD_MODEMFSM_STATESET 0x0041

/**
   Controls state transitions of the modem state-machine.
*/
typedef struct CMD_ModemFSM_StateSet CMD_ModemFSM_StateSet_t;

/** Message ID for ACK_ModemFSM_StateSet */
#define ACK_MODEMFSM_STATESET 0x0041

/**
   Acknowledgement for message CMD_ModemFSM_StateSet
*/
typedef struct ACK_ModemFSM_StateSet ACK_ModemFSM_StateSet_t;

/** Message ID for ALM_ModemFSM_FailReasonGet */
#define ALM_MODEMFSM_FAILREASONGET 0x0502

/**
   Reports failure information after entering FAIL state. This message is sent
   autonomously without host request after entrance into FAIL was indicated by
   EVT_ModemFSM_StateGet. Both messages are generated only if enabled with
   CMD_ModemFSM_EventConfigure (common Bit E8 "FAIL").
*/
typedef struct ALM_ModemFSM_FailReasonGet ALM_ModemFSM_FailReasonGet_t;

/** Message ID for CMD_ModemFSM_FailReasonGet */
#define CMD_MODEMFSM_FAILREASONGET 0x0502

/**
   Requests failure information after entering FAIL state.
*/
typedef struct CMD_ModemFSM_FailReasonGet CMD_ModemFSM_FailReasonGet_t;

/** Message ID for ACK_ModemFSM_FailReasonGet */
#define ACK_MODEMFSM_FAILREASONGET 0x0502

/**
   Returns failure information after entering FAIL state.
*/
typedef struct ACK_ModemFSM_FailReasonGet ACK_ModemFSM_FailReasonGet_t;

/** Message ID for CMD_ModemFSM_OptionsSet */
#define CMD_MODEMFSM_OPTIONSSET 0x0449

/**
   Configuration of options for the modem state machine.
*/
typedef struct CMD_ModemFSM_OptionsSet CMD_ModemFSM_OptionsSet_t;

/** Message ID for ACK_ModemFSM_OptionsSet */
#define ACK_MODEMFSM_OPTIONSSET 0x0449

/**
   Acknowledgement to CMD_ModemFSM_OptionsSet.
*/
typedef struct ACK_ModemFSM_OptionsSet ACK_ModemFSM_OptionsSet_t;

/** Message ID for CMD_ModemFSM_Options2Set */
#define CMD_MODEMFSM_OPTIONS2SET 0x0062

/**
   Configuration of options for the modem state machine.
*/
typedef struct CMD_ModemFSM_Options2Set CMD_ModemFSM_Options2Set_t;

/** Message ID for ACK_ModemFSM_Options2Set */
#define ACK_MODEMFSM_OPTIONS2SET 0x0062

/**
   Acknowledgement to CMD_ModemFSM_Options2Set.
*/
typedef struct ACK_ModemFSM_Options2Set ACK_ModemFSM_Options2Set_t;

/** Message ID for CMD_ModemFSM_EventConfigure */
#define CMD_MODEMFSM_EVENTCONFIGURE 0x0049

/**
   Sets options for the modem state-machine. Enables/Disables autonomous
   messages for specific state transitions (EVT_ModemFSM_StateGet). The EVT
   message is generated after entering the particular state listed in the
   parameter description of the enable bits.
*/
typedef struct CMD_ModemFSM_EventConfigure CMD_ModemFSM_EventConfigure_t;

/** Message ID for ACK_ModemFSM_EventConfigure */
#define ACK_MODEMFSM_EVENTCONFIGURE 0x0049

/**
   Acknowledgement for CMD_ModemFSM_EventConfigure. (Enabling/Disabling EVENT
   messages (EVTs) for specific state transitions.)
*/
typedef struct ACK_ModemFSM_EventConfigure ACK_ModemFSM_EventConfigure_t;

/** Message ID for CMD_FailuresNE_AlarmConfigure */
#define CMD_FAILURESNE_ALARMCONFIGURE 0x0149

/**
   Enables/Disables the generation of ALARM messages (ALM) for specific near-end
   line failures. If the corresponding Enable bit for a line failure is set,
   then the modem firmware will send an autonomous message ALM_LineFailureNE_Get
   if this failure occurs.
*/
typedef struct CMD_FailuresNE_AlarmConfigure CMD_FailuresNE_AlarmConfigure_t;

/** Message ID for ACK_FailuresNE_AlarmConfigure */
#define ACK_FAILURESNE_ALARMCONFIGURE 0x0149

/**
   Acknowledgement to CMD_FailuresNE_AlarmConfigure.
*/
typedef struct ACK_FailuresNE_AlarmConfigure ACK_FailuresNE_AlarmConfigure_t;

/** Message ID for CMD_FailuresFE_AlarmConfigure */
#define CMD_FAILURESFE_ALARMCONFIGURE 0x0249

/**
   Enables/Disables the generation of ALARM messages (ALM) for specific far-end
   line failures. If the corresponding Enable bit for a line failure is set,
   then the modem firmware will send an autonomous message ALM_LineFailureFE_Get
   if this failure occurs.
*/
typedef struct CMD_FailuresFE_AlarmConfigure CMD_FailuresFE_AlarmConfigure_t;

/** Message ID for ACK_FailuresFE_AlarmConfigure */
#define ACK_FAILURESFE_ALARMCONFIGURE 0x0249

/**
   Acknowledgement to CMD_FailuresFE_AlarmConfigure.
*/
typedef struct ACK_FailuresFE_AlarmConfigure ACK_FailuresFE_AlarmConfigure_t;

/** Message ID for CMD_ReInitNE_Configure */
#define CMD_REINITNE_CONFIGURE 0x0549

/**
   Configures re-initialization triggers for near-end failure conditions in
   modem state STEADY STATE transmission (see Figure 1). This command is
   accepted in RESET state only.
*/
typedef struct CMD_ReInitNE_Configure CMD_ReInitNE_Configure_t;

/** Message ID for ACK_ReInitNE_Configure */
#define ACK_REINITNE_CONFIGURE 0x0549

/**
   Acknowledgment to CMD_ReInitNE_Configure (Configuration of re-initialization
   triggers for near-end failures).
*/
typedef struct ACK_ReInitNE_Configure ACK_ReInitNE_Configure_t;

/** Message ID for CMD_L3ShutdownRequest */
#define CMD_L3SHUTDOWNREQUEST 0x0341

/**
   Triggers the sending of an L3 orderly shutdown request to the remote side.If
   applied at the CPE and accepted by the CO, the following shall happen: The
   CPE-Host forces L3 entry with transition to RESET state by applying
   CMD_ModemFSM_StateGet.
*/
typedef struct CMD_L3ShutdownRequest CMD_L3ShutdownRequest_t;

/** Message ID for ACK_L3ShutdownRequest */
#define ACK_L3SHUTDOWNREQUEST 0x0341

/**
   Acknowledgement for CMD_L3ShutdownRequest.
*/
typedef struct ACK_L3ShutdownRequest ACK_L3ShutdownRequest_t;

/** Message ID for CMD_RxL3RequestStatusGet */
#define CMD_RXL3REQUESTSTATUSGET 0x0402

/**
   Requests information about L3 orderly shutdown requests initiated from the
   remote side.Applying CMD_RxL3RequestStatusGet allows the host to determine
   whether a near-end LOS or LOM failure is due to an L3 shutdown or an
   unexpected line failure.
*/
typedef struct CMD_RxL3RequestStatusGet CMD_RxL3RequestStatusGet_t;

/** Message ID for ACK_RxL3RequestStatusGet */
#define ACK_RXL3REQUESTSTATUSGET 0x0402

/**
   Acknowledgement for CMD_RxL3RequestStatusGet.As long as the modem is in L0
   (Showtime), an ATU-R will automatically respond to any L3 request from the
   ATU-C by sending an accept message.  If the ATU-R is in L2 mode when it
   receives the request, it will send a reject message. Upon receiving an
   acceptance, the remote side should enter L3 state and shut off its
   transmitter.
*/
typedef struct ACK_RxL3RequestStatusGet ACK_RxL3RequestStatusGet_t;

/** Message ID for CMD_TxL3RequestStatusGet */
#define CMD_TXL3REQUESTSTATUSGET 0x1402

/**
   Requests the status of a near-end initiated L3 shutdown request. If the
   remote side accepted the request, in ADSL the host will force L3 entry using
   CMD_ModemFSM_StateSet.
*/
typedef struct CMD_TxL3RequestStatusGet CMD_TxL3RequestStatusGet_t;

/** Message ID for ACK_TxL3RequestStatusGet */
#define ACK_TXL3REQUESTSTATUSGET 0x1402

/**
   Acknowledgement for CMD_TxL3RequestStatusGet.
*/
typedef struct ACK_TxL3RequestStatusGet ACK_TxL3RequestStatusGet_t;

/** Message ID for CMD_TxL3ReqFailReasonGet */
#define CMD_TXL3REQFAILREASONGET 0x1502

/**
   Requests the fail reason of a failed near-end initiated L3 request. (see also
   ACK_TxL3RequestStatusGet)
*/
typedef struct CMD_TxL3ReqFailReasonGet CMD_TxL3ReqFailReasonGet_t;

/** Message ID for ACK_TxL3ReqFailReasonGet */
#define ACK_TXL3REQFAILREASONGET 0x1502

/**
   Reports the fail reason of a near-end initiated L3 request, as response to
   CMD_TxL3ReqFailReasonGet.
*/
typedef struct ACK_TxL3ReqFailReasonGet ACK_TxL3ReqFailReasonGet_t;

/** Message ID for CMD_FW_ImageInfoGet */
#define CMD_FW_IMAGEINFOGET 0xD103

/**
   Requests information about the loaded FW image. This info is used e.g. for
   swapping between ADSL and VDSL.
*/
typedef struct CMD_FW_ImageInfoGet CMD_FW_ImageInfoGet_t;

/** Message ID for ACK_FW_ImageInfoGet */
#define ACK_FW_IMAGEINFOGET 0xD103

/**
   Provides the information about the FW image requested by ACK_FW_ImageInfoGet
*/
typedef struct ACK_FW_ImageInfoGet ACK_FW_ImageInfoGet_t;

/** Message ID for CMD_XTSE_Configure */
#define CMD_XTSE_CONFIGURE 0x0045

/**
   Configuration of the VTU Transmission System Enabling (XTSE).Configures the
   transmission system coding types to be supported on the line (VDSL flavour
   and annex support).(References: Section 7.3.1.1.1 of G.997.1 and G.994.1 Amd4
   [10] Tables "Standard information field Ð SPar(1) coding")
*/
typedef struct CMD_XTSE_Configure CMD_XTSE_Configure_t;

/** Message ID for ACK_XTSE_Configure */
#define ACK_XTSE_CONFIGURE 0x0045

/**
   Acknowledgement for message CMD_XTSE_Configure.
*/
typedef struct ACK_XTSE_Configure ACK_XTSE_Configure_t;

/** Message ID for CMD_BandControl_US_Set */
#define CMD_BANDCONTROL_US_SET 0x2548

/**
   Controls the upstream band usage.The message can only be used for DMTscope
   tests without handshake and training phases.
*/
typedef struct CMD_BandControl_US_Set CMD_BandControl_US_Set_t;

/** Message ID for ACK_BandControl_US_Set */
#define ACK_BANDCONTROL_US_SET 0x2548

/**
   Acknowledgement for message CMD_BandControl_US_Set.
*/
typedef struct ACK_BandControl_US_Set ACK_BandControl_US_Set_t;

/** Message ID for CMD_PSD_Set */
#define CMD_PSD_SET 0x2348

/**
   Configuration of PSD and Power parameters. Only used for DMTscope tests
   without handshake and training phases.
*/
typedef struct CMD_PSD_Set CMD_PSD_Set_t;

/** Message ID for ACK_PSD_Set */
#define ACK_PSD_SET 0x2348

/**
   Acknowledgement for the message CMD_PSD_Set.
*/
typedef struct ACK_PSD_Set ACK_PSD_Set_t;

/** Message ID for CMD_PSD_BreakpointsTxUS_Set */
#define CMD_PSD_BREAKPOINTSTXUS_SET 0x2848

/**
   Specifies the maximum upstream transmit PSD by means of breakpoints.To be
   used only for DMTscope tests without handshake and training phases.
*/
typedef struct CMD_PSD_BreakpointsTxUS_Set CMD_PSD_BreakpointsTxUS_Set_t;

/** Message ID for ACK_PSD_BreakpointsTxUS_Set */
#define ACK_PSD_BREAKPOINTSTXUS_SET 0x2848

/**
   Acknowledgement for the message CMD_PSD_BreakpointsTxUS_Set.
*/
typedef struct ACK_PSD_BreakpointsTxUS_Set ACK_PSD_BreakpointsTxUS_Set_t;

/** Message ID for CMD_PSD_OptionsSet */
#define CMD_PSD_OPTIONSSET 0x0262

/**
   Configuration of PSD related options.
*/
typedef struct CMD_PSD_OptionsSet CMD_PSD_OptionsSet_t;

/** Message ID for ACK_PSD_OptionsSet */
#define ACK_PSD_OPTIONSSET 0x0262

/**
   Acknowledgement to CMD_PSD_OptionsSet.
*/
typedef struct ACK_PSD_OptionsSet ACK_PSD_OptionsSet_t;

/** Message ID for CMD_UPBO_KL0Get */
#define CMD_UPBO_KL0GET 0xD603

/**
   Requests the electrical loop length estimate kl0.
*/
typedef struct CMD_UPBO_KL0Get CMD_UPBO_KL0Get_t;

/** Message ID for ACK_UPBO_KL0Get */
#define ACK_UPBO_KL0GET 0xD603

/**
   Delivers the data requested by CMD_UPBO_KL0Get.(Section 7.5.1.23 of G.997.1
   [11])
*/
typedef struct ACK_UPBO_KL0Get ACK_UPBO_KL0Get_t;

/** Message ID for CMD_NoiseMarginDeltaSet */
#define CMD_NOISEMARGINDELTASET 0x1C45

/**
   Configuration of a target noise margin delta, which is added to the target
   noise margin value configured at the CO (and in case of the CPE received from
   there). The resulting value is then taken as target noise margin, e.g. for
   bit loading.
*/
typedef struct CMD_NoiseMarginDeltaSet CMD_NoiseMarginDeltaSet_t;

/** Message ID for ACK_NoiseMarginDeltaSet */
#define ACK_NOISEMARGINDELTASET 0x1C45

/**
   Acknowledgement for the message CMD_NoiseMarginDeltaSet.
*/
typedef struct ACK_NoiseMarginDeltaSet ACK_NoiseMarginDeltaSet_t;

/** Message ID for CMD_BearerCh0_DS_Set */
#define CMD_BEARERCH0_DS_SET 0x0048

/**
   Sets parameters for downstream bearer channel 0 (Chapters 7.3.2.1-5 of
   G.997.1 [11]). Bearer channel configuration at the CPE is optional and just
   used to further limit the parameters usually configured at the CO.The mode,
   ATM or PTM, is selected with the configuration controls "ATMControl" and
   "PTMControl" in Parameter 2. Exactly one of them must be enabled, not both!
*/
typedef struct CMD_BearerCh0_DS_Set CMD_BearerCh0_DS_Set_t;

/** Message ID for ACK_BearerCh0_DS_Set */
#define ACK_BEARERCH0_DS_SET 0x0048

/**
   Acknowledgement for CMD_BearerCh0_DS_Set. (Configuration of bearer channel
   0).
*/
typedef struct ACK_BearerCh0_DS_Set ACK_BearerCh0_DS_Set_t;

/** Message ID for CMD_BearerCh0_US_Set */
#define CMD_BEARERCH0_US_SET 0x0248

/**
   Sets parameters for upstream bearer channel 0 (Chapters 7.3.2.1-5 of G.997.1
   [11]). Bearer channel configuration at the CPE is optional and just used to
   further limit the parameters usually configured at the CO.The mode, ATM or
   PTM, is selected with the configuration controls "ATMControl" and
   "PTMControl" in Parameter 2. Exactly one of them must be enabled, not both!
*/
typedef struct CMD_BearerCh0_US_Set CMD_BearerCh0_US_Set_t;

/** Message ID for ACK_BearerCh0_US_Set */
#define ACK_BEARERCH0_US_SET 0x0248

/**
   Acknowledgement for CMD_BearerCh0_US_Set. (Configuration of bearer channel
   0).
*/
typedef struct ACK_BearerCh0_US_Set ACK_BearerCh0_US_Set_t;

/** Message ID for CMD_InitPolicySet */
#define CMD_INITPOLICYSET 0x1A45

/**
   The message selects the channel initialization policy (CIPOLICY) to be
   applied for the tranceiver configuration. (Section 7.3.2.10 of G.997.1)In
   ADSL mode, the message is applicable only for ADSL2/2+ (Section 7.10.3 of
   G.992.3 Amd 3). The standard defines the parameter for the CO only. Here, the
   message is applied at the ADSL-CPE, then it overrides the policy given by the
   CO!
*/
typedef struct CMD_InitPolicySet CMD_InitPolicySet_t;

/** Message ID for ACK_InitPolicySet */
#define ACK_INITPOLICYSET 0x1A45

/**
   Acknowledgment for message CMD_InitPolicySet.
*/
typedef struct ACK_InitPolicySet ACK_InitPolicySet_t;

/** Message ID for CMD_ErasureControlSet */
#define CMD_ERASURECONTROLSET 0x0162

/**
   Configuration of options for the modem state machine.
*/
typedef struct CMD_ErasureControlSet CMD_ErasureControlSet_t;

/** Message ID for ACK_ErasureControlSet */
#define ACK_ERASURECONTROLSET 0x0162

/**
   Acknowledgement to CMD_ModemFSM_Options2Set.
*/
typedef struct ACK_ErasureControlSet ACK_ErasureControlSet_t;

/** Message ID for CMD_AlgorithmControlSet */
#define CMD_ALGORITHMCONTROLSET 0x0245

/**
   Controls certain funtions of the chip set.Attention: 1. The default values
   may be different for VDSL and ADSL2. This message uses a negative logic! Thus
   all "Reserved" bit parameters of this message shall be set to "1" unless
   explicitely otherwise noted.
*/
typedef struct CMD_AlgorithmControlSet CMD_AlgorithmControlSet_t;

/** Message ID for ACK_AlgorithmControlSet */
#define ACK_ALGORITHMCONTROLSET 0x0245

/**
   Acknowledgement for message CMD_AlgorithmControlSet.
*/
typedef struct ACK_AlgorithmControlSet ACK_AlgorithmControlSet_t;

/** Message ID for CMD_Misc_ConfigSet */
#define CMD_MISC_CONFIGSET 0x3A48

/**
   Performs some miscellaneous chip set configurations.
*/
typedef struct CMD_Misc_ConfigSet CMD_Misc_ConfigSet_t;

/** Message ID for ACK_Misc_ConfigSet */
#define ACK_MISC_CONFIGSET 0x3A48

/**
   This is the acknowledgement for ACK_Misc_ConfigSet.
*/
typedef struct ACK_Misc_ConfigSet ACK_Misc_ConfigSet_t;

/** Message ID for CMD_OperationOptionsSet */
#define CMD_OPERATIONOPTIONSSET 0x0D62

/**
   Performs some miscellaneous chip set configurations.
*/
typedef struct CMD_OperationOptionsSet CMD_OperationOptionsSet_t;

/** Message ID for ACK_OperationOptionsSet */
#define ACK_OPERATIONOPTIONSSET 0x0D62

/**
   This is the acknowledgement for CMD_OperationOptionsSet.
*/
typedef struct ACK_OperationOptionsSet ACK_OperationOptionsSet_t;

/** Message ID for CMD_InteropConfigSet */
#define CMD_INTEROPCONFIGSET 0x6743

/**
   The message configures interoperability settings for ADSL.
*/
typedef struct CMD_InteropConfigSet CMD_InteropConfigSet_t;

/** Message ID for ACK_InteropConfigSet */
#define ACK_INTEROPCONFIGSET 0x6743

/**
   This is the acknowledgement for CMD_InteropConfigSet.
*/
typedef struct ACK_InteropConfigSet ACK_InteropConfigSet_t;

/** Message ID for CMD_InteropConfigGet */
#define CMD_INTEROPCONFIGGET 0x6703

/**
   The message requests the interoperability settings.
*/
typedef struct CMD_InteropConfigGet CMD_InteropConfigGet_t;

/** Message ID for ACK_InteropConfigGet */
#define ACK_INTEROPCONFIGGET 0x6703

/**
   This is the acknowledgement for CMD_InteropConfigGet.
*/
typedef struct ACK_InteropConfigGet ACK_InteropConfigGet_t;

/** Message ID for CMD_ClockSet */
#define CMD_CLOCKSET 0x0F62

/**
   Sets the PPE clock.
*/
typedef struct CMD_ClockSet CMD_ClockSet_t;

/** Message ID for ACK_ClockSet */
#define ACK_CLOCKSET 0x0F62

/**
   This is the acknowledgement for CMD_ClockSet.
*/
typedef struct ACK_ClockSet ACK_ClockSet_t;

/**
   Requests information about the current state of the modem state-machine.The
   command can be sent in all states of the modem state machine (see Figure 2).
*/
struct CMD_ModemFSM_StateGet
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
   Returns information about the current state of the modem state-machine.
*/
struct ACK_ModemFSM_StateGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Modem Status */
   DSL_uint16_t ModemState;
   /** Reserved */
   DSL_uint16_t Res0 : 14;
   /** Line Power Management State */
   DSL_uint16_t LxState : 2;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Modem Status */
   DSL_uint16_t ModemState;
   /** Line Power Management State */
   DSL_uint16_t LxState : 2;
   /** Reserved */
   DSL_uint16_t Res0 : 14;
#endif
} __PACKED__ ;


/**
   Returns information about the current state of the modem state-machine. This
   message is sent autonomously, if the modem enters a new state and reporting
   is enabled for that state (see CMD_ModemFSM_EventConfigure).
*/
struct EVT_ModemFSM_StateGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Modem Status */
   DSL_uint16_t ModemState;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Modem Status */
   DSL_uint16_t ModemState;
#endif
} __PACKED__ ;


/**
   Autonomous message indicating that the modem online code was reached and that
   initialization was completed. The management entity may not perform any
   query-response messaging until the EVT_ModemReady message is received. Its
   generation cannot be disabled. If it is not received this indicates a
   problem, possibly with the boot sequence. If the message is received, it
   contains a result code that may also indicate that an error was detected by
   the firmware during initialization.
*/
struct EVT_ModemReady
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Error Code */
   DSL_uint8_t ErrorCode;
   /** Warning Code */
   DSL_uint8_t WarningCode;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Warning Code */
   DSL_uint8_t WarningCode;
   /** Error Code */
   DSL_uint8_t ErrorCode;
#endif
} __PACKED__ ;


/**
   Controls state transitions of the modem state-machine.
*/
struct CMD_ModemFSM_StateSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 12;
   /** Link Control */
   DSL_uint16_t LinkControl : 4;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Link Control */
   DSL_uint16_t LinkControl : 4;
   /** Reserved */
   DSL_uint16_t Res0 : 12;
#endif
} __PACKED__ ;


/**
   Acknowledgement for message CMD_ModemFSM_StateSet
*/
struct ACK_ModemFSM_StateSet
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
   Reports failure information after entering FAIL state. This message is sent
   autonomously without host request after entrance into FAIL was indicated by
   EVT_ModemFSM_StateGet. Both messages are generated only if enabled with
   CMD_ModemFSM_EventConfigure (common Bit E8 "FAIL").
*/
struct ALM_ModemFSM_FailReasonGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** System Sub Error Code */
   DSL_uint8_t SubErrorCode;
   /** System Error Code */
   DSL_uint8_t ErrorCode;
   /** Failure State Information */
   DSL_uint16_t FW_FailCode;
   /** Reserved */
   DSL_uint16_t Res0[8];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** System Error Code */
   DSL_uint8_t ErrorCode;
   /** System Sub Error Code */
   DSL_uint8_t SubErrorCode;
   /** Failure State Information */
   DSL_uint16_t FW_FailCode;
   /** Reserved */
   DSL_uint16_t Res0[8];
#endif
} __PACKED__ ;


/**
   Requests failure information after entering FAIL state.
*/
struct CMD_ModemFSM_FailReasonGet
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
   Returns failure information after entering FAIL state.
*/
struct ACK_ModemFSM_FailReasonGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** System Sub Error Code */
   DSL_uint8_t SubErrorCode;
   /** System Error Code */
   DSL_uint8_t ErrorCode;
   /** Failure State Information */
   DSL_uint16_t FW_FailCode;
   /** Reserved */
   DSL_uint16_t Res0[8];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** System Error Code */
   DSL_uint8_t ErrorCode;
   /** System Sub Error Code */
   DSL_uint8_t SubErrorCode;
   /** Failure State Information */
   DSL_uint16_t FW_FailCode;
   /** Reserved */
   DSL_uint16_t Res0[8];
#endif
} __PACKED__ ;


/**
   Configuration of options for the modem state machine.
*/
struct CMD_ModemFSM_OptionsSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 7;
   /** L2 Low-Power Mode (ADSL only) */
   DSL_uint16_t E8 : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 5;
   /** Loop Diagnostic Mode Control */
   DSL_uint16_t E2 : 1;
   /** Reserved 0 */
   DSL_uint16_t E1 : 1;
   /** Automatic Re-Start Control */
   DSL_uint16_t E0 : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** L2 Low-Power Mode (ADSL only) */
   FX_bit_t E8;
   /** Reserved */
   DSL_uint16_t Res0 : 7;
   /** Automatic Re-Start Control */
   DSL_uint16_t E0 : 1;
   /** Reserved 0 */
   DSL_uint16_t E1 : 1;
   /** Loop Diagnostic Mode Control */
   DSL_uint16_t E2 : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 5;
#endif
} __PACKED__ ;


/**
   Acknowledgement to CMD_ModemFSM_OptionsSet.
*/
struct ACK_ModemFSM_OptionsSet
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
   Configuration of options for the modem state machine.
*/
struct CMD_ModemFSM_Options2Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 13;
   /** Short Init Control (ADSL only), Bit 2 */
   DSL_uint16_t shortInit : 1;
   /** US Virtual Noise Support, Bit 1 */
   DSL_uint16_t enableVN_US : 1;
   /** DS Virtual Noise Support, Bit 0 */
   DSL_uint16_t enableVN_DS : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** DS Virtual Noise Support, Bit 0 */
   DSL_uint16_t enableVN_DS : 1;
   /** US Virtual Noise Support, Bit 1 */
   DSL_uint16_t enableVN_US : 1;
   /** Short Init Control (ADSL only), Bit 2 */
   DSL_uint16_t shortInit : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 13;
#endif
} __PACKED__ ;


/**
   Acknowledgement to CMD_ModemFSM_Options2Set.
*/
struct ACK_ModemFSM_Options2Set
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
   Sets options for the modem state-machine. Enables/Disables autonomous
   messages for specific state transitions (EVT_ModemFSM_StateGet). The EVT
   message is generated after entering the particular state listed in the
   parameter description of the enable bits.
*/
struct CMD_ModemFSM_EventConfigure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 1;
   /** Enable Bit 13 */
   DSL_uint16_t E13 : 1;
   /** Enable Bit 13 */
   DSL_uint16_t E12 : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 1;
   /** Enable Bit 11 */
   DSL_uint16_t E11 : 1;
   /** Enable Bit 10 */
   DSL_uint16_t E10 : 1;
   /** Enable Bit 9 */
   DSL_uint16_t E9 : 1;
   /** Enable Bit 8 */
   DSL_uint16_t E8 : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 1;
   /** Enable Bit 6 */
   DSL_uint16_t E6 : 1;
   /** Enable Bit 5 */
   DSL_uint16_t E5 : 1;
   /** Enable Bit 4 */
   DSL_uint16_t E4 : 1;
   /** Enable Bit 3 */
   DSL_uint16_t E3 : 1;
   /** Enable Bit 2 */
   DSL_uint16_t E2 : 1;
   /** Reserved */
   DSL_uint16_t Res3 : 1;
   /** Enable Bit 0 */
   DSL_uint16_t E0 : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Enable Bit 0 */
   DSL_uint16_t E0 : 1;
   /** Reserved */
   DSL_uint16_t Res3 : 1;
   /** Enable Bit 2 */
   DSL_uint16_t E2 : 1;
   /** Enable Bit 3 */
   DSL_uint16_t E3 : 1;
   /** Enable Bit 4 */
   DSL_uint16_t E4 : 1;
   /** Enable Bit 5 */
   DSL_uint16_t E5 : 1;
   /** Enable Bit 6 */
   DSL_uint16_t E6 : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 1;
   /** Enable Bit 8 */
   DSL_uint16_t E8 : 1;
   /** Enable Bit 9 */
   DSL_uint16_t E9 : 1;
   /** Enable Bit 10 */
   DSL_uint16_t E10 : 1;
   /** Enable Bit 11 */
   DSL_uint16_t E11 : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 1;
   /** Enable Bit 13 */
   DSL_uint16_t E12 : 1;
   /** Enable Bit 13 */
   DSL_uint16_t E13 : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 1;
#endif
} __PACKED__ ;


/**
   Acknowledgement for CMD_ModemFSM_EventConfigure. (Enabling/Disabling EVENT
   messages (EVTs) for specific state transitions.)
*/
struct ACK_ModemFSM_EventConfigure
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
   Enables/Disables the generation of ALARM messages (ALM) for specific near-end
   line failures. If the corresponding Enable bit for a line failure is set,
   then the modem firmware will send an autonomous message ALM_LineFailureNE_Get
   if this failure occurs.
*/
struct CMD_FailuresNE_AlarmConfigure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 2;
   /** Enable Bit 13 */
   DSL_uint16_t E13 : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 9;
   /** Enable Bit3 */
   DSL_uint16_t E3 : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 1;
   /** Enable Bit1 */
   DSL_uint16_t E1 : 1;
   /** Enable Bit0 */
   DSL_uint16_t E0 : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Enable Bit0 */
   DSL_uint16_t E0 : 1;
   /** Enable Bit1 */
   DSL_uint16_t E1 : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 1;
   /** Enable Bit3 */
   DSL_uint16_t E3 : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 9;
   /** Enable Bit 13 */
   DSL_uint16_t E13 : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 2;
#endif
} __PACKED__ ;


/**
   Acknowledgement to CMD_FailuresNE_AlarmConfigure.
*/
struct ACK_FailuresNE_AlarmConfigure
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
   Enables/Disables the generation of ALARM messages (ALM) for specific far-end
   line failures. If the corresponding Enable bit for a line failure is set,
   then the modem firmware will send an autonomous message ALM_LineFailureFE_Get
   if this failure occurs.
*/
struct CMD_FailuresFE_AlarmConfigure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 12;
   /** Enable Bit3 */
   DSL_uint16_t E3 : 1;
   /** Enable Bit2 */
   DSL_uint16_t E2 : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 1;
   /** Enable Bit0 */
   DSL_uint16_t E0 : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Enable Bit0 */
   DSL_uint16_t E0 : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 1;
   /** Enable Bit2 */
   DSL_uint16_t E2 : 1;
   /** Enable Bit3 */
   DSL_uint16_t E3 : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 12;
#endif
} __PACKED__ ;


/**
   Acknowledgement to CMD_FailuresFE_AlarmConfigure.
*/
struct ACK_FailuresFE_AlarmConfigure
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
   Configures re-initialization triggers for near-end failure conditions in
   modem state STEADY STATE transmission (see Figure 1). This command is
   accepted in RESET state only.
*/
struct CMD_ReInitNE_Configure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 1;
   /** Excess DS CRCs (ADSL only) */
   DSL_uint16_t E14 : 1;
   /** ESE */
   DSL_uint16_t E13 : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 2;
   /** OOS BC0 */
   DSL_uint16_t E10 : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 1;
   /** NCD BC0 */
   DSL_uint16_t E8 : 1;
   /** Reserved */
   DSL_uint16_t Res3 : 1;
   /** LCD BC0 */
   DSL_uint16_t E6 : 1;
   /** Reserved */
   DSL_uint16_t Res4 : 2;
   /** LOM */
   DSL_uint16_t E3 : 1;
   /** Reserved */
   DSL_uint16_t Res5 : 1;
   /** LOF */
   DSL_uint16_t E1 : 1;
   /** LOS */
   DSL_uint16_t E0 : 1;
   /** Reserved */
   DSL_uint16_t Res6 : 14;
   /** ES90 */
   DSL_uint16_t F1 : 1;
   /** SES30 */
   DSL_uint16_t F0 : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** LOS */
   DSL_uint16_t E0 : 1;
   /** LOF */
   DSL_uint16_t E1 : 1;
   /** Reserved */
   DSL_uint16_t Res5 : 1;
   /** LOM */
   DSL_uint16_t E3 : 1;
   /** Reserved */
   DSL_uint16_t Res4 : 2;
   /** LCD BC0 */
   DSL_uint16_t E6 : 1;
   /** Reserved */
   DSL_uint16_t Res3 : 1;
   /** NCD BC0 */
   DSL_uint16_t E8 : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 1;
   /** OOS BC0 */
   DSL_uint16_t E10 : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 2;
   /** ESE */
   DSL_uint16_t E13 : 1;
   /** Excess DS CRCs (ADSL only) */
   DSL_uint16_t E14 : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 1;
   /** SES30 */
   DSL_uint16_t F0 : 1;
   /** ES90 */
   DSL_uint16_t F1 : 1;
   /** Reserved */
   DSL_uint16_t Res6 : 14;
#endif
} __PACKED__ ;


/**
   Acknowledgment to CMD_ReInitNE_Configure (Configuration of re-initialization
   triggers for near-end failures).
*/
struct ACK_ReInitNE_Configure
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
   Triggers the sending of an L3 orderly shutdown request to the remote side.If
   applied at the CPE and accepted by the CO, the following shall happen: The
   CPE-Host forces L3 entry with transition to RESET state by applying
   CMD_ModemFSM_StateGet.
*/
struct CMD_L3ShutdownRequest
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 15;
   /** L3 Orderly Shutdown Request */
   DSL_uint16_t L3shutdown : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** L3 Orderly Shutdown Request */
   DSL_uint16_t L3shutdown : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 15;
#endif
} __PACKED__ ;


/**
   Acknowledgement for CMD_L3ShutdownRequest.
*/
struct ACK_L3ShutdownRequest
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
   Requests information about L3 orderly shutdown requests initiated from the
   remote side.Applying CMD_RxL3RequestStatusGet allows the host to determine
   whether a near-end LOS or LOM failure is due to an L3 shutdown or an
   unexpected line failure.
*/
struct CMD_RxL3RequestStatusGet
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
   Acknowledgement for CMD_RxL3RequestStatusGet.As long as the modem is in L0
   (Showtime), an ATU-R will automatically respond to any L3 request from the
   ATU-C by sending an accept message.  If the ATU-R is in L2 mode when it
   receives the request, it will send a reject message. Upon receiving an
   acceptance, the remote side should enter L3 state and shut off its
   transmitter.
*/
struct ACK_RxL3RequestStatusGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** L3 Request Response */
   DSL_uint16_t L3ReqResponse : 1;
   /** L3 Request Received */
   DSL_uint16_t L3ReqReceived : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 14;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 14;
   /** L3 Request Received */
   DSL_uint16_t L3ReqReceived : 1;
   /** L3 Request Response */
   DSL_uint16_t L3ReqResponse : 1;
#endif
} __PACKED__ ;


/**
   Requests the status of a near-end initiated L3 shutdown request. If the
   remote side accepted the request, in ADSL the host will force L3 entry using
   CMD_ModemFSM_StateSet.
*/
struct CMD_TxL3RequestStatusGet
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
   Acknowledgement for CMD_TxL3RequestStatusGet.
*/
struct ACK_TxL3RequestStatusGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 14;
   /** Status L3 Request */
   DSL_uint16_t L3ReqStatus : 2;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Status L3 Request */
   DSL_uint16_t L3ReqStatus : 2;
   /** Reserved */
   DSL_uint16_t Res0 : 14;
#endif
} __PACKED__ ;


/**
   Requests the fail reason of a failed near-end initiated L3 request. (see also
   ACK_TxL3RequestStatusGet)
*/
struct CMD_TxL3ReqFailReasonGet
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
   Reports the fail reason of a near-end initiated L3 request, as response to
   CMD_TxL3ReqFailReasonGet.
*/
struct ACK_TxL3ReqFailReasonGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 8;
   /** L3 Fail Reason */
   DSL_uint16_t L3FailReason : 4;
   /** Reserved */
   DSL_uint16_t Res1 : 4;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res1 : 4;
   /** L3 Fail Reason */
   DSL_uint16_t L3FailReason : 4;
   /** Reserved */
   DSL_uint16_t Res0 : 8;
#endif
} __PACKED__ ;


/**
   Requests information about the loaded FW image. This info is used e.g. for
   swapping between ADSL and VDSL.
*/
struct CMD_FW_ImageInfoGet
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
   Provides the information about the FW image requested by ACK_FW_ImageInfoGet
*/
struct ACK_FW_ImageInfoGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 13;
   /** Site */
   DSL_uint16_t imageType : 1;
   /** DSL mode */
   DSL_uint16_t dslMode : 1;
   /** Site */
   DSL_uint16_t location : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Site */
   DSL_uint16_t location : 1;
   /** DSL mode */
   DSL_uint16_t dslMode : 1;
   /** Site */
   DSL_uint16_t imageType : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 13;
#endif
} __PACKED__ ;


/**
   Configuration of the VTU Transmission System Enabling (XTSE).Configures the
   transmission system coding types to be supported on the line (VDSL flavour
   and annex support).(References: Section 7.3.1.1.1 of G.997.1 and G.994.1 Amd4
   [10] Tables "Standard information field Ð SPar(1) coding")
*/
struct CMD_XTSE_Configure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** ADSL Mode-Bit15: G.992.5, Annex A */
   DSL_uint16_t A15 : 1;
   /** ADSL Mode-Bit14: G.992.5, Annex B */
   DSL_uint16_t A14 : 1;
   /** ADSL Mode-Bit13: G.992.3, Annex M */
   DSL_uint16_t A13 : 1;
   /** ADSL Mode-Bit12: G.992.3, Annex L, US Mask2 */
   DSL_uint16_t A12 : 1;
   /** ADSL Mode-Bit11: G.992.3, Annex L, US Mask1 */
   DSL_uint16_t A11 : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 1;
   /** ADSL Mode-Bit9: G.992.3, Annex B */
   DSL_uint16_t A9 : 1;
   /** ADSL Mode-Bit8: G.992.3, Annex A */
   DSL_uint16_t A8 : 1;
   /** ADSL Mode-Bit7: G.992.5, Annex J */
   DSL_uint16_t A7 : 1;
   /** ADSL Mode-Bit6: G.992.5, Annex I */
   DSL_uint16_t A6 : 1;
   /** ADSL Mode-Bit5: G.992.3, Annex J */
   DSL_uint16_t A5 : 1;
   /** ADSL Mode-Bit4: G.992.3, Annex I */
   DSL_uint16_t A4 : 1;
   /** ADSL Mode-Bit3: G.992.1, Annex B */
   DSL_uint16_t A3 : 1;
   /** ADSL Mode-Bit2: G.992.1, Annex A */
   DSL_uint16_t A2 : 1;
   /** ADSL Mode-Bit1: G.992.5, Annex M */
   DSL_uint16_t A1 : 1;
   /** ADSL Mode-Bit0: T1.413 */
   DSL_uint16_t A0 : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 13;
   /** VDSL Mode-Bit2: VDSL2 */
   DSL_uint16_t V2 : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 2;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** ADSL Mode-Bit0: T1.413 */
   DSL_uint16_t A0 : 1;
   /** ADSL Mode-Bit1: G.992.5, Annex M */
   DSL_uint16_t A1 : 1;
   /** ADSL Mode-Bit2: G.992.1, Annex A */
   DSL_uint16_t A2 : 1;
   /** ADSL Mode-Bit3: G.992.1, Annex B */
   DSL_uint16_t A3 : 1;
   /** ADSL Mode-Bit4: G.992.3, Annex I */
   DSL_uint16_t A4 : 1;
   /** ADSL Mode-Bit5: G.992.3, Annex J */
   DSL_uint16_t A5 : 1;
   /** ADSL Mode-Bit6: G.992.5, Annex I */
   DSL_uint16_t A6 : 1;
   /** ADSL Mode-Bit7: G.992.5, Annex J */
   DSL_uint16_t A7 : 1;
   /** ADSL Mode-Bit8: G.992.3, Annex A */
   DSL_uint16_t A8 : 1;
   /** ADSL Mode-Bit9: G.992.3, Annex B */
   DSL_uint16_t A9 : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 1;
   /** ADSL Mode-Bit11: G.992.3, Annex L, US Mask1 */
   DSL_uint16_t A11 : 1;
   /** ADSL Mode-Bit12: G.992.3, Annex L, US Mask2 */
   DSL_uint16_t A12 : 1;
   /** ADSL Mode-Bit13: G.992.3, Annex M */
   DSL_uint16_t A13 : 1;
   /** ADSL Mode-Bit14: G.992.5, Annex B */
   DSL_uint16_t A14 : 1;
   /** ADSL Mode-Bit15: G.992.5, Annex A */
   DSL_uint16_t A15 : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 2;
   /** VDSL Mode-Bit2: VDSL2 */
   DSL_uint16_t V2 : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 13;
#endif
} __PACKED__ ;


/**
   Acknowledgement for message CMD_XTSE_Configure.
*/
struct ACK_XTSE_Configure
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
   Controls the upstream band usage.The message can only be used for DMTscope
   tests without handshake and training phases.
*/
struct CMD_BandControl_US_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint8_t Res0;
   /** Number of Upstream Bands */
   DSL_uint8_t NumBandsUS;
   /** Band Descriptor US */
   VRX_ToneIndex_t band[8];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Number of Upstream Bands */
   DSL_uint8_t NumBandsUS;
   /** Reserved */
   DSL_uint8_t Res0;
   /** Band Descriptor US */
   VRX_ToneIndex_t band[8];
#endif
} __PACKED__ ;


/**
   Acknowledgement for message CMD_BandControl_US_Set.
*/
struct ACK_BandControl_US_Set
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
   Configuration of PSD and Power parameters. Only used for DMTscope tests
   without handshake and training phases.
*/
struct CMD_PSD_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0;
   /** MAXNOMPSDus */
   DSL_uint16_t MAXNOMPSDus;
   /** Reserved */
   DSL_uint16_t Res1;
   /** MAXNOMATPus */
   DSL_uint16_t MAXNOMATPus;
   /** Reserved */
   DSL_uint16_t Res2;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0;
   /** MAXNOMPSDus */
   DSL_uint16_t MAXNOMPSDus;
   /** Reserved */
   DSL_uint16_t Res1;
   /** MAXNOMATPus */
   DSL_uint16_t MAXNOMATPus;
   /** Reserved */
   DSL_uint16_t Res2;
#endif
} __PACKED__ ;


/**
   Acknowledgement for the message CMD_PSD_Set.
*/
struct ACK_PSD_Set
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
   Specifies the maximum upstream transmit PSD by means of breakpoints.To be
   used only for DMTscope tests without handshake and training phases.
*/
struct CMD_PSD_BreakpointsTxUS_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint8_t Res0;
   /** Number of Breakpoints */
   DSL_uint8_t NumBreakPts;
   /** Breakpoint Descriptor TxPSDus */
   VRX_PSDbreak_t breakpoint[32];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Number of Breakpoints */
   DSL_uint8_t NumBreakPts;
   /** Reserved */
   DSL_uint8_t Res0;
   /** Breakpoint Descriptor TxPSDus */
   VRX_PSDbreak_t breakpoint[32];
#endif
} __PACKED__ ;


/**
   Acknowledgement for the message CMD_PSD_BreakpointsTxUS_Set.
*/
struct ACK_PSD_BreakpointsTxUS_Set
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
   Configuration of PSD related options.
*/
struct CMD_PSD_OptionsSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 15;
   /** US TSSI Control, Bit 0 */
   DSL_uint16_t useTssiUS : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** US TSSI Control, Bit 0 */
   DSL_uint16_t useTssiUS : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 15;
#endif
} __PACKED__ ;


/**
   Acknowledgement to CMD_PSD_OptionsSet.
*/
struct ACK_PSD_OptionsSet
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
   Requests the electrical loop length estimate kl0.
*/
struct CMD_UPBO_KL0Get
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
   Delivers the data requested by CMD_UPBO_KL0Get.(Section 7.5.1.23 of G.997.1
   [11])
*/
struct ACK_UPBO_KL0Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Kl0 Estimate VTU-O */
   DSL_uint16_t kl0_EstimO;
   /** Kl0 Estimate VTU-R */
   DSL_uint16_t kl0_EstimR;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Kl0 Estimate VTU-O */
   DSL_uint16_t kl0_EstimO;
   /** Kl0 Estimate VTU-R */
   DSL_uint16_t kl0_EstimR;
#endif
} __PACKED__ ;


/**
   Configuration of a target noise margin delta, which is added to the target
   noise margin value configured at the CO (and in case of the CPE received from
   there). The resulting value is then taken as target noise margin, e.g. for
   bit loading.
*/
struct CMD_NoiseMarginDeltaSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Target Noise Margin Delta DS */
   DSL_int16_t deltaTARSNRMds;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Target Noise Margin Delta DS */
   DSL_int16_t deltaTARSNRMds;
#endif
} __PACKED__ ;


/**
   Acknowledgement for the message CMD_NoiseMarginDeltaSet.
*/
struct ACK_NoiseMarginDeltaSet
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
   Sets parameters for downstream bearer channel 0 (Chapters 7.3.2.1-5 of
   G.997.1 [11]). Bearer channel configuration at the CPE is optional and just
   used to further limit the parameters usually configured at the CO.The mode,
   ATM or PTM, is selected with the configuration controls "ATMControl" and
   "PTMControl" in Parameter 2. Exactly one of them must be enabled, not both!
*/
struct CMD_BearerCh0_DS_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 13;
   /** Reserved (STM) */
   DSL_uint16_t Res1 : 1;
   /** ATM Configuration Control (ADSL) */
   DSL_uint16_t ATMControl : 1;
   /** PTM Configuration Control */
   DSL_uint16_t PTMControl : 1;
   /** Minimum Data Rate BC0 PTM DS */
   DSL_uint16_t minRate0_PTMds;
   /** Maximum Data Rate BC0 PTM DS */
   DSL_uint16_t maxRate0_PTMds;
   /** Minimum Reserved Data Rate BC0 PTM DS */
   DSL_uint16_t minResRate0_PTMds;
   /** Reserved */
   DSL_uint8_t Res2;
   /** Maximum Interleaving Delay BC0 PTM DS */
   DSL_uint8_t maxDelay0_PTMds;
   /** Reserved */
   DSL_uint16_t Res3 : 6;
   /** "SHORT PACKETS" OPTION BC0 PTM DS */
   DSL_uint16_t ShortPacket_PTMds : 1;
   /** "PRE-EMPTION" OPTION BC0 PTM DS */
   DSL_uint16_t Preempt_PTMds : 1;
   /** Reserved */
   DSL_uint16_t Res4 : 6;
   /** Maximum BER BC0 PTM DS */
   DSL_uint16_t BER_PTMds : 2;
   /** Reserved */
   DSL_uint16_t Res5 : 4;
   /** Minimum INP BC0 PTM DS */
   DSL_uint16_t minINP_PTMds : 12;
   /** Minimum Data Rate BC0 ATM DS */
   DSL_uint16_t minRate0_ATMds;
   /** Maximum Data Rate BC0 ATM DS */
   DSL_uint16_t maxRate0_ATMds;
   /** Minimum Reserved Data Rate BC0 ATM DS */
   DSL_uint16_t minResRate0_ATMds;
   /** Reserved */
   DSL_uint8_t Res6;
   /** Maximum Interleaving Delay BC0 ATM DS */
   DSL_uint8_t maxDelay0_ATMds;
   /** Reserved */
   DSL_uint16_t Res7 : 14;
   /** Maximum BER BC0 ATM DS */
   DSL_uint16_t BER_ATMds : 2;
   /** Reserved */
   DSL_uint16_t Res8 : 4;
   /** Minimum INP BC0 ATM DS */
   DSL_uint16_t minINP_ATMds : 12;
   /** Reserved */
   DSL_uint16_t Res9[6];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** PTM Configuration Control */
   DSL_uint16_t PTMControl : 1;
   /** ATM Configuration Control (ADSL) */
   DSL_uint16_t ATMControl : 1;
   /** Reserved (STM) */
   DSL_uint16_t Res1 : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 13;
   /** Minimum Data Rate BC0 PTM DS */
   DSL_uint16_t minRate0_PTMds;
   /** Maximum Data Rate BC0 PTM DS */
   DSL_uint16_t maxRate0_PTMds;
   /** Minimum Reserved Data Rate BC0 PTM DS */
   DSL_uint16_t minResRate0_PTMds;
   /** Maximum Interleaving Delay BC0 PTM DS */
   DSL_uint8_t maxDelay0_PTMds;
   /** Reserved */
   DSL_uint8_t Res2;
   /** Maximum BER BC0 PTM DS */
   DSL_uint16_t BER_PTMds : 2;
   /** Reserved */
   DSL_uint16_t Res4 : 6;
   /** "PRE-EMPTION" OPTION BC0 PTM DS */
   DSL_uint16_t Preempt_PTMds : 1;
   /** "SHORT PACKETS" OPTION BC0 PTM DS */
   DSL_uint16_t ShortPacket_PTMds : 1;
   /** Reserved */
   DSL_uint16_t Res3 : 6;
   /** Minimum INP BC0 PTM DS */
   DSL_uint16_t minINP_PTMds : 12;
   /** Reserved */
   DSL_uint16_t Res5 : 4;
   /** Minimum Data Rate BC0 ATM DS */
   DSL_uint16_t minRate0_ATMds;
   /** Maximum Data Rate BC0 ATM DS */
   DSL_uint16_t maxRate0_ATMds;
   /** Minimum Reserved Data Rate BC0 ATM DS */
   DSL_uint16_t minResRate0_ATMds;
   /** Maximum Interleaving Delay BC0 ATM DS */
   DSL_uint8_t maxDelay0_ATMds;
   /** Reserved */
   DSL_uint8_t Res6;
   /** Maximum BER BC0 ATM DS */
   DSL_uint16_t BER_ATMds : 2;
   /** Reserved */
   DSL_uint16_t Res7 : 14;
   /** Minimum INP BC0 ATM DS */
   DSL_uint16_t minINP_ATMds : 12;
   /** Reserved */
   DSL_uint16_t Res8 : 4;
   /** Reserved */
   DSL_uint16_t Res9[6];
#endif
} __PACKED__ ;


/**
   Acknowledgement for CMD_BearerCh0_DS_Set. (Configuration of bearer channel
   0).
*/
struct ACK_BearerCh0_DS_Set
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
   Sets parameters for upstream bearer channel 0 (Chapters 7.3.2.1-5 of G.997.1
   [11]). Bearer channel configuration at the CPE is optional and just used to
   further limit the parameters usually configured at the CO.The mode, ATM or
   PTM, is selected with the configuration controls "ATMControl" and
   "PTMControl" in Parameter 2. Exactly one of them must be enabled, not both!
*/
struct CMD_BearerCh0_US_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 13;
   /** Reserved (STM) */
   DSL_uint16_t Res1 : 1;
   /** ATM Configuration Control (ADSL only) */
   DSL_uint16_t ATMControl : 1;
   /** PTM Configuration Control */
   DSL_uint16_t PTMControl : 1;
   /** Minimum Data Rate BC0 PTM US */
   DSL_uint16_t minRate0_PTMus;
   /** Maximum Data Rate BC0 PTM US */
   DSL_uint16_t maxRate0_PTMus;
   /** Minimum Reserved Data Rate BC0 PTM US */
   DSL_uint16_t minResRate0_PTMus;
   /** Reserved */
   DSL_uint8_t Res2;
   /** Maximum Interleaving Delay BC0 PTM US */
   DSL_uint8_t maxDelay0_PTMus;
   /** Reserved */
   DSL_uint16_t Res3 : 6;
   /** "SHORT PACKETS" OPTION BC0 PTM US */
   DSL_uint16_t ShortPacket_PTMus : 1;
   /** "PRE-EMPTION" OPTION BC0 PTM US */
   DSL_uint16_t Preempt_PTMus : 1;
   /** Reserved */
   DSL_uint16_t Res4 : 6;
   /** Maximum BER BC0 PTM US */
   DSL_uint16_t BER_PTMus : 2;
   /** Reserved */
   DSL_uint16_t Res5 : 4;
   /** Minimum INP BC0 PTM US */
   DSL_uint16_t minINP_PTMus : 12;
   /** Minimum Data Rate BC0 ATM US */
   DSL_uint16_t minRate0_ATMus;
   /** Maximum Data Rate BC0 ATM US */
   DSL_uint16_t maxRate0_ATMus;
   /** Minimum Reserved Data Rate BC0 ATM US */
   DSL_uint16_t minResRate0_ATMus;
   /** Reserved */
   DSL_uint8_t Res6;
   /** Maximum Interleaving Delay BC0 ATM US */
   DSL_uint8_t maxDelay0_ATMus;
   /** Reserved */
   DSL_uint16_t Res7 : 14;
   /** Maximum BER BC0 ATM US */
   DSL_uint16_t BER_ATMus : 2;
   /** Reserved */
   DSL_uint16_t Res8 : 4;
   /** Minimum INP BC0 ATM US */
   DSL_uint16_t minINP_ATMus : 12;
   /** Reserved */
   DSL_uint16_t Res9[6];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** PTM Configuration Control */
   DSL_uint16_t PTMControl : 1;
   /** ATM Configuration Control (ADSL only) */
   DSL_uint16_t ATMControl : 1;
   /** Reserved (STM) */
   DSL_uint16_t Res1 : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 13;
   /** Minimum Data Rate BC0 PTM US */
   DSL_uint16_t minRate0_PTMus;
   /** Maximum Data Rate BC0 PTM US */
   DSL_uint16_t maxRate0_PTMus;
   /** Minimum Reserved Data Rate BC0 PTM US */
   DSL_uint16_t minResRate0_PTMus;
   /** Maximum Interleaving Delay BC0 PTM US */
   DSL_uint8_t maxDelay0_PTMus;
   /** Reserved */
   DSL_uint8_t Res2;
   /** Maximum BER BC0 PTM US */
   DSL_uint16_t BER_PTMus : 2;
   /** Reserved */
   DSL_uint16_t Res4 : 6;
   /** "PRE-EMPTION" OPTION BC0 PTM US */
   DSL_uint16_t Preempt_PTMus : 1;
   /** "SHORT PACKETS" OPTION BC0 PTM US */
   DSL_uint16_t ShortPacket_PTMus : 1;
   /** Reserved */
   DSL_uint16_t Res3 : 6;
   /** Minimum INP BC0 PTM US */
   DSL_uint16_t minINP_PTMus : 12;
   /** Reserved */
   DSL_uint16_t Res5 : 4;
   /** Minimum Data Rate BC0 ATM US */
   DSL_uint16_t minRate0_ATMus;
   /** Maximum Data Rate BC0 ATM US */
   DSL_uint16_t maxRate0_ATMus;
   /** Minimum Reserved Data Rate BC0 ATM US */
   DSL_uint16_t minResRate0_ATMus;
   /** Maximum Interleaving Delay BC0 ATM US */
   DSL_uint8_t maxDelay0_ATMus;
   /** Reserved */
   DSL_uint8_t Res6;
   /** Maximum BER BC0 ATM US */
   DSL_uint16_t BER_ATMus : 2;
   /** Reserved */
   DSL_uint16_t Res7 : 14;
   /** Minimum INP BC0 ATM US */
   DSL_uint16_t minINP_ATMus : 12;
   /** Reserved */
   DSL_uint16_t Res8 : 4;
   /** Reserved */
   DSL_uint16_t Res9[6];
#endif
} __PACKED__ ;


/**
   Acknowledgement for CMD_BearerCh0_US_Set. (Configuration of bearer channel
   0).
*/
struct ACK_BearerCh0_US_Set
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
   The message selects the channel initialization policy (CIPOLICY) to be
   applied for the tranceiver configuration. (Section 7.3.2.10 of G.997.1)In
   ADSL mode, the message is applicable only for ADSL2/2+ (Section 7.10.3 of
   G.992.3 Amd 3). The standard defines the parameter for the CO only. Here, the
   message is applied at the ADSL-CPE, then it overrides the policy given by the
   CO!
*/
struct CMD_InitPolicySet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 14;
   /** Reserved for Init Policy BC1 */
   DSL_uint16_t Res1 : 1;
   /** Init Policy BC0 */
   DSL_uint16_t policy0 : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Init Policy BC0 */
   DSL_uint16_t policy0 : 1;
   /** Reserved for Init Policy BC1 */
   DSL_uint16_t Res1 : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 14;
#endif
} __PACKED__ ;


/**
   Acknowledgment for message CMD_InitPolicySet.
*/
struct ACK_InitPolicySet
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
   Configuration of options for the modem state machine.
*/
struct CMD_ErasureControlSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 13;
   /** "Trust-Me"-Bit, Bit 2 (ADSL only) */
   DSL_uint16_t trustMe : 1;
   /** Erasure Policy, Bit 1 */
   DSL_uint16_t erasurePolicy : 1;
   /** Erasure Decoding ON/OFF, Bit 0 */
   DSL_uint16_t erasureEnable : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Erasure Decoding ON/OFF, Bit 0 */
   DSL_uint16_t erasureEnable : 1;
   /** Erasure Policy, Bit 1 */
   DSL_uint16_t erasurePolicy : 1;
   /** "Trust-Me"-Bit, Bit 2 (ADSL only) */
   DSL_uint16_t trustMe : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 13;
#endif
} __PACKED__ ;


/**
   Acknowledgement to CMD_ModemFSM_Options2Set.
*/
struct ACK_ErasureControlSet
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
   Controls certain funtions of the chip set.Attention: 1. The default values
   may be different for VDSL and ADSL2. This message uses a negative logic! Thus
   all "Reserved" bit parameters of this message shall be set to "1" unless
   explicitely otherwise noted.
*/
struct CMD_AlgorithmControlSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Steady-State Algorithm Control, Bit15 */
   DSL_uint16_t SAC15 : 1;
   /** Steady-State Algorithm Control, Bit14 */
   DSL_uint16_t SAC14 : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 3;
   /** Steady-State Algorithm Control, Bit10 */
   DSL_uint16_t SAC10 : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 1;
   /** Steady-State Algorithm Control, Bit8 */
   DSL_uint16_t SAC8 : 1;
   /** Steady-State Algorithm Control, Bit7 */
   DSL_uint16_t SAC7 : 1;
   /** Steady-State Algorithm Control, Bit6 */
   DSL_uint16_t SAC6 : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 1;
   /** Steady-State Algorithm Control, Bit4 */
   DSL_uint16_t SAC4 : 1;
   /** Reserved */
   DSL_uint16_t Res3 : 2;
   /** Steady-State Algorithm Control, Bit1 */
   DSL_uint16_t SAC1 : 1;
   /** Reserved */
   DSL_uint16_t Res4 : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res4 : 1;
   /** Steady-State Algorithm Control, Bit1 */
   DSL_uint16_t SAC1 : 1;
   /** Reserved */
   DSL_uint16_t Res3 : 2;
   /** Steady-State Algorithm Control, Bit4 */
   DSL_uint16_t SAC4 : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 1;
   /** Steady-State Algorithm Control, Bit6 */
   DSL_uint16_t SAC6 : 1;
   /** Steady-State Algorithm Control, Bit7 */
   DSL_uint16_t SAC7 : 1;
   /** Steady-State Algorithm Control, Bit8 */
   DSL_uint16_t SAC8 : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 1;
   /** Steady-State Algorithm Control, Bit10 */
   DSL_uint16_t SAC10 : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 3;
   /** Steady-State Algorithm Control, Bit14 */
   DSL_uint16_t SAC14 : 1;
   /** Steady-State Algorithm Control, Bit15 */
   DSL_uint16_t SAC15 : 1;
#endif
} __PACKED__ ;


/**
   Acknowledgement for message CMD_AlgorithmControlSet.
*/
struct ACK_AlgorithmControlSet
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
   Performs some miscellaneous chip set configurations.
*/
struct CMD_Misc_ConfigSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 12;
   /** INP Protection Improvement (VDSL only) */
   DSL_uint16_t FrameParams : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 1;
   /** Activation Start Mode (ADSL only) */
   DSL_uint16_t StartMode : 1;
   /** Non-standard GHS-ANSI Activation (ADSL only) */
   DSL_uint16_t GhsAnsiSeq : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Non-standard GHS-ANSI Activation (ADSL only) */
   DSL_uint16_t GhsAnsiSeq : 1;
   /** Activation Start Mode (ADSL only) */
   DSL_uint16_t StartMode : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 1;
   /** INP Protection Improvement (VDSL only) */
   DSL_uint16_t FrameParams : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 12;
#endif
} __PACKED__ ;


/**
   This is the acknowledgement for ACK_Misc_ConfigSet.
*/
struct ACK_Misc_ConfigSet
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
   Performs some miscellaneous chip set configurations.
*/
struct CMD_OperationOptionsSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 13;
   /** VRX_ENABLE, Fixed value */
   DSL_uint16_t Mfd : 1;
   /** Enhanced Upstream Framing (ADSL-Only) */
   DSL_uint16_t UsFramingExt : 1;
   /** NTR (currently VDSL-Only) */
   DSL_uint16_t Ntr : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** NTR (currently VDSL-Only) */
   DSL_uint16_t Ntr : 1;
   /** Enhanced Upstream Framing (ADSL-Only) */
   DSL_uint16_t UsFramingExt : 1;
   /** VRX_ENABLE, Fixed value */
   DSL_uint16_t Mfd : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 13;
#endif
} __PACKED__ ;


/**
   This is the acknowledgement for CMD_OperationOptionsSet.
*/
struct ACK_OperationOptionsSet
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
   The message configures interoperability settings for ADSL.
*/
struct CMD_InteropConfigSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 10;
   /** CRC/FEC Counter Restore On Showtime Entry, Bit 5 */
   DSL_uint16_t cntRestore : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 5;
   /** Reserved */
   DSL_uint16_t Res2 : 15;
   /** PCB Level Adjustment, Bit 0 */
   DSL_uint16_t pcbAdjust : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res1 : 5;
   /** CRC/FEC Counter Restore On Showtime Entry, Bit 5 */
   DSL_uint16_t cntRestore : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 10;
   /** PCB Level Adjustment, Bit 0 */
   DSL_uint16_t pcbAdjust : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 15;
#endif
} __PACKED__ ;


/**
   This is the acknowledgement for CMD_InteropConfigSet.
*/
struct ACK_InteropConfigSet
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
   The message requests the interoperability settings.
*/
struct CMD_InteropConfigGet
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
   This is the acknowledgement for CMD_InteropConfigGet.
*/
struct ACK_InteropConfigGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 10;
   /** CRC/FEC Counter Restore On Showtime Entry, Bit 5 */
   DSL_uint16_t cntRestore : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 5;
   /** Reserved */
   DSL_uint16_t Res2 : 15;
   /** PCB Level Adjustment, Bit 0 */
   DSL_uint16_t pcbAdjust : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res1 : 5;
   /** CRC/FEC Counter Restore On Showtime Entry, Bit 5 */
   DSL_uint16_t cntRestore : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 10;
   /** PCB Level Adjustment, Bit 0 */
   DSL_uint16_t pcbAdjust : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 15;
#endif
} __PACKED__ ;


/**
   Sets the PPE clock.
*/
struct CMD_ClockSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 11;
   /** Clock Change Trigger */
   DSL_uint16_t ppeClkSet : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 2;
   /** PPE Clock */
   DSL_uint16_t ppeClock : 2;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** PPE Clock */
   DSL_uint16_t ppeClock : 2;
   /** Reserved */
   DSL_uint16_t Res1 : 2;
   /** Clock Change Trigger */
   DSL_uint16_t ppeClkSet : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 11;
#endif
} __PACKED__ ;


/**
   This is the acknowledgement for CMD_ClockSet.
*/
struct ACK_ClockSet
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

#endif /** _DRV_DSL_CPE_VRX_MSG_CONFIG_PMD_H_*/
