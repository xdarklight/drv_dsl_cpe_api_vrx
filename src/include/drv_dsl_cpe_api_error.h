/******************************************************************************

                              Copyright (c) 2014
                            Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef _DRV_DSL_API_ERROR_H
#define _DRV_DSL_API_ERROR_H

#ifdef __cplusplus
   extern "C" {
#endif

/** \addtogroup DRV_DSL_CPE_ERROR_CODES
 @{ */

/**
   Defines all possible error codes.
   Error codes are negative, warning codes are positive and success has the
   value 0.

   \note If there are more than one warnings during processing of one DSL CPE API
         call the warning with the lowest value will be returned
*/
typedef enum
{
   /* *********************************************************************** */
   /* *** Error Codes Start here !                                        *** */
   /* *********************************************************************** */

   /* *********************************************************************** */
   /* *** Error Codes for bonding functionality                           *** */
   /* *********************************************************************** */
   /** Command/feature can not be performed because only one line can be in
       showtime in case of disabled bonding on the CO side. This line has been
       disabled because the other line has reached a line state that is equal
       or bigger than \ref DSL_LINESTATE_FULL_INIT.
       To activate this line again please do one of the following actions
       - in case of *on-chip* bonding scenario, use command
         \ref DSL_FIO_AUTOBOOT_CONTROL_SET with nCommand equals
         \ref DSL_AUTOBOOT_CTRL_RESTART_FULL on any line (CLI: "acs [x] 6").
         This will start both lines.
       - use command \ref DSL_FIO_AUTOBOOT_CONTROL_SET with nCommand equals
         \ref DSL_AUTOBOOT_CTRL_DISABLE on the other line (that is not
         disabled, CLI: "acs [x] 4").
         Afterwards both lines are disabled and can be enabled again
         1) individually by using command \ref DSL_FIO_AUTOBOOT_CONTROL_SET with
         nCommand equals \ref DSL_AUTOBOOT_CTRL_ENABLE or
         \ref DSL_AUTOBOOT_CTRL_RESTART (CLI: "acs [x] 5" or "acs [x] 2")
         2) all togther by using command \ref DSL_FIO_AUTOBOOT_CONTROL_SET with
         nCommand equals \ref DSL_AUTOBOOT_CTRL_RESTART_FULL on any line
         (CLI: "acs [x] 6"), only in case of *on-chip* bonding.
       \note The value [x] includes the optional line/device parameter that is
             used only in case of bonding. */
   DSL_ERR_BND_REMOTE_PAF_DISABLED = -502,
   /** Command/feature is only supported if bonding functionality is enabled.
       Please use the command \ref DSL_FIO_BND_CONFIG_SET with bPafEnable
       equals DSL_TRUE to enable bonding functionality. */
   DSL_ERR_BND_ONLY_SUPPORTED_WITH_BONDING_ENABLED = -501,
   /** Command/feature can not be performed because the firmware does not
       support bonding functionality. */
   DSL_ERR_BND_NOT_SUPPORTED_BY_FIRMWARE = -500,

   /* *********************************************************************** */
   /* *** Error Codes for configuration parameter consistency check       *** */
   /* *********************************************************************** */
   /** The configuration of the TC-Layer does not fit to the bonding
       configuration. Due to the fact that PAF bonding is only supported within
       PTM/EFM TC-Layer please note that it is not allowed to select only the
       \ref DSL_TC_ATM TC-Layer in case of bonding is enabled
       (CLI: "BND_ConfigSet"/"bndcs").
       This error will occur in case of bonding is enabled and user
       configuration of \ref DSL_TC_ATM is applied or vice versa. The
       configuration is rejected, means that the original configuration will
       be kept. */
   DSL_ERR_CONFIG_BND_VS_TCLAYER = -401,
   /** parameter out of range */
   DSL_ERR_PARAM_RANGE = -400,

   /* *********************************************************************** */
   /* *** Error Codes for EOC handler                                     *** */
   /* *********************************************************************** */
   /** transmission error */
   DSL_ERR_CEOC_TX_ERR = -300,

   /* *********************************************************************** */
   /* *** Error Codes for modem handling                                  *** */
   /* *********************************************************************** */
   /** Modem is not ready */
   DSL_ERR_MODEM_NOT_READY = -201,

   /* *********************************************************************** */
   /* *** Error Codes for Autoboot handler                                *** */
   /* *********************************************************************** */
   /** Autoboot handling has been disabled.
       \note Also refer to description of \ref DSL_LINESTATE_DISABLED */
   DSL_ERR_AUTOBOOT_DISABLED = -103,
   /** Autoboot thread is not started yet */
   DSL_ERR_AUTOBOOT_NOT_STARTED = -102,
   /** Autoboot thread is busy */
   DSL_ERR_AUTOBOOT_BUSY = -101,

   /* *********************************************************************** */
   /* *** Error Codes for IOCTL handler                                   *** */
   /* *********************************************************************** */
   /** An error occurred during execution of a low level (MEI BSP) driver
       function */
   DSL_ERR_LOW_LEVEL_DRIVER_ACCESS = -32,
   /** Invalid parameter is passed */
   DSL_ERR_INVALID_PARAMETER = -31,

   /* *********************************************************************** */
   /* *** Common Error Codes                                              *** */
   /* *********************************************************************** */
   /** invalid DSL mode */
   DSL_ERR_NO_FIRMWARE_LOADED = -43,
   /** Data is currently not available.
       Update handling for the relevant interval was not completed before and
       is just in progress. Please request the data at a later point in time
       again and/or wait for the according "data available" event, for
       example \ref DSL_EVENT_S_FE_TESTPARAMS_AVAILABLE */
   DSL_ERR_DATA_UPDATE_IN_PROGRESS = -42,
   /** invalid DSL mode */
   DSL_ERR_DSLMODE = -41,
   /** The requested values are not supported in the current VDSL mode */
   DSL_ERR_NOT_SUPPORTED_IN_CURRENT_VDSL_MODE = -40,
   /** Real time trace unavailable */
   DSL_ERR_RTT_NOT_AVAILABLE = -39,
   /** Feature unavailable in case of disabled retransmission.
       This error can happen in the following cases
       - The retransmission feature is not enabled on CPE side.
         The feature can be enabled by using configuration parameter
         'bReTxEnable' within context of ioctl
         \ref DSL_FIO_LINE_FEATURE_CONFIG_SET
       - The feature is enabled on CPE side but the CO side does not support it
         or has not enabled it.
         This state can be checked by getting retransmission status value
         'bReTxEnable' within context of ioctl
         \ref DSL_FIO_LINE_FEATURE_STATUS_GET that needs to be called in
         showtime. */
   DSL_ERR_RETRANSMISSION_DISABLED = -38,
   /** CPE triggered L3 request has been rejected by the CO side,
       reason - not desired*/
   DSL_ERR_L3_REJECTED_NOT_DESIRED = -37,
   /** ioctl not supported by DSL CPE API.
       The reason might be because of current configure options. */
   DSL_ERR_IOCTL_NOT_SUPPORTED = -36,
   /** Feature or functionality is not defined by standards. */
   DSL_ERR_NOT_SUPPORTED_BY_DEFINITION = -35,
   /** DSL CPE API not initialized yet*/
   DSL_ERR_NOT_INITIALIZED = -34,
   /** The requested values are not supported in the current
       ADSL mode or Annex*/
   DSL_ERR_NOT_SUPPORTED_IN_CURRENT_ADSL_MODE_OR_ANNEX = -33,
   /** The DELT data is not available within DSL CPE API.
       Whether the diagnostic complete state was never reached (no successful
       completion of DELT measurement) or the DELT data was already deleted
       by using ioctl \ref DSL_FIO_G997_DELT_FREE_RESOURCES */
   DSL_ERR_DELT_DATA_NOT_AVAILABLE = -30,
   /** The event that should be processed are not active for the current
       instance */
   DSL_ERR_EVENTS_NOT_ACTIVE = -29,
   /** During CPE triggered L3 request an error occurred that could not be
       classified more in detail. Please check if the L3 entry is allowed on
       the CO side.*/
   DSL_ERR_L3_UNKNOWN_FAILURE = -28,
   /** CPE triggered L3 request timed out */
   DSL_ERR_L3_NOT_IN_L0 = -27,
   /** During CPE triggered L3 request the CO side has returned the error
       that the line is not in L0 state. */
   DSL_ERR_L3_TIMED_OUT = -26,
   /** CPE triggered L3 request has been rejected by the CO side. */
   DSL_ERR_L3_REJECTED = -25,
   /** failed to get low level driver handle */
   DSL_ERR_LOWLEVEL_DRIVER_HANDLE = -24,
   /** invalid direction */
   DSL_ERR_DIRECTION = -23,
   /** invalid channel number is passed */
   DSL_ERR_CHANNEL_RANGE = -22,
   /** function available only in the Showtime state */
   DSL_ERR_ONLY_AVAILABLE_IN_SHOWTIME = -21,
   /** Device has no data for application */
   DSL_ERR_DEVICE_NO_DATA = -20,
   /** Device is busy */
   DSL_ERR_DEVICE_BUSY = -19,
   /** The answer from the device does not return within the specifies timeout */
   DSL_ERR_FUNCTION_WAITING_TIMEOUT = -18,
   /** Last operation is supported if debug is enabled only error */
   DSL_ERR_ONLY_SUPPORTED_WITH_DEBUG_ENABLED = -17,
   /** Semaphore lock error */
   DSL_ERR_SEMAPHORE_GET = -16,
   /** Common error on send message and wait for answer handling */
   DSL_ERR_FUNCTION_WAITING = -15,
   /** Message exchange error */
   DSL_ERR_MSG_EXCHANGE = -14,
   /** Not implemented error */
   DSL_ERR_NOT_IMPLEMENTED = -13,
   /** Internal error */
   DSL_ERR_INTERNAL = -12,
   /** Feature or functionality not supported by device */
   DSL_ERR_NOT_SUPPORTED_BY_DEVICE = -11,
   /** Feature or functionality not supported by firmware */
   DSL_ERR_NOT_SUPPORTED_BY_FIRMWARE = -10,
   /** Feature or functionality not supported by DSL CPE API */
   DSL_ERR_NOT_SUPPORTED = -9,
   /** function returned with timeout */
   DSL_ERR_TIMEOUT = -8,
   /** invalid pointer */
   DSL_ERR_POINTER = -7,
   /** invalid memory */
   DSL_ERR_MEMORY = -6,
   /** file open failed */
   DSL_ERR_FILE_OPEN = -5,
   /** file write failed */
   DSL_ERR_FILE_WRITE = -4,
   /** file reading failed */
   DSL_ERR_FILE_READ = -3,
   /** file close failed */
   DSL_ERR_FILE_CLOSE = -2,
   /** Common error */
   DSL_ERROR = -1,

   /** Success */
   DSL_SUCCESS = 0,

   /* *********************************************************************** */
   /* *** Warning Codes Start here !                                      *** */
   /* *********************************************************************** */

   /* *********************************************************************** */
   /* *** Common Warning Codes                                            *** */
   /* *********************************************************************** */
   /** One or more parameters are truncated to min./max or next possible value */
   DSL_WRN_CONFIG_PARAM_TRUNCATED = 1,
   /** DSL CPE API already initialized*/
   DSL_WRN_ALREADY_INITIALIZED = 2,
   /** XTSE settings consist of unsupported bits. All unsupported bits removed,
      configuration applied*/
   DSL_WRN_INCONSISTENT_XTSE_CONFIGURATION = 3,
   /** One or more parameters are ignored */
   DSL_WRN_CONFIG_PARAM_IGNORED = 4,
   /** This warning is used in case of an event was lost.
   This could happen due to the following reasons
   - polling cycle within polling based event handling is to slow
   - system overload respective improper priorities within interrupt based
     event handling
   Also refer to "Event Handling" chapter within UMPR to get all the details. */
   DSL_WRN_EVENT_FIFO_OVERFLOW  = 5,
   /** The ioctl function that has been used is deprecated.
   Please do not use this function anymore. Refer to the according documentation
   (release notes and/or User's Manual Programmer's Reference [UMPR]) of
   the DSL CPE API to find the new function that has to be used. */
   DSL_WRN_DEPRECATED  = 6,
   /** This warning occurs if the firmware did not accept the last message.
      This may occur if the message is unknown or not allowed in the current
      state. */
   DSL_WRN_FIRMWARE_MSG_DENIED = 9,
   /** This warning occurs if no data available from the device. */
   DSL_WRN_DEVICE_NO_DATA = 10,
   /** The requested functionality is not supported due to build configuration.
       Please refer to the documentation for "Configure options for the DSL CPE
       API Driver" */
   DSL_WRN_NOT_SUPPORTED_DUE_TO_BUILD_CONFIG = 13,
   /** The performed API interface access is not allowed within current autoboot
       state. */
   DSL_WRN_NOT_ALLOWED_IN_CURRENT_STATE = 14,
   /** This warning occurs if there was a request of status information but not
       all returned values includes updated data.
       For example the ioctl \ref DSL_FIO_G997_LINE_STATUS_GET includes six
       parameters that are returned and three of them are requested from far end
       side via overhead channel. If this is not possible because of a not
       responding CO this warning is returned and the according value will have
       its special value.
       The higher layer application shall check all returned values according
       to its special value if this warning is returned. */
   DSL_WRN_INCOMPLETE_RETURN_VALUES = 15,
   /** Some (or all) of the requested values are not supported in the current
       ADSL mode or Annex*/
   DSL_WRN_NOT_SUPPORTED_IN_CURRENT_ADSL_MODE_OR_ANNEX = 16,
   /** Not defined ADSL MIB flags detected*/
   DSL_WRN_INCONSISTENT_ADSL_MIB_FLAGS = 17,
   /** Common warning to indicate some incompatibility in the used SW, FW or HW
       versions*/
   DSL_WRN_VERSION_INCOMPATIBLE = 18,
   /** Warning to indicate violation between Band Limits and actual borders*/
   DSL_WRN_FW_BB_STANDARD_VIOLATION = 19,
   /** Warning to indicate not recommended configuration*/
   DSL_WRN_NOT_RECOMMENDED_CONFIG = 20,

   /* *********************************************************************** */
   /* *** PM related warning Codes                                        *** */
   /* *********************************************************************** */
   /** Performance Monitor thread was not able to receive showtime related
       counter (TR-98) */
   DSL_WRN_PM_NO_SHOWTIME_DATA = 100,
   /** Requested functionality not supported in the current PM Sync mode*/
   DSL_WRN_PM_NOT_ALLOWED_IN_CURRENT_SYNC_MODE = 101,
   /** Previous External Trigger is not handled*/
   DSL_WRN_PM_PREVIOUS_EXTERNAL_TRIGGER_NOT_HANDLED = 102,

   /** PM poll cycle not updated due to the active Burnin Mode. Poll cycle
       configuration changes will be loaded automatically after disabling
       Burnin Mode*/
   DSL_WRN_PM_POLL_CYCLE_NOT_UPDATED_IN_BURNIN_MODE = 103,

   /* *********************************************************************** */
   /* *** SNMP/EOC related warning Codes                                 *** */
   /* *********************************************************************** */
   /** CEOC Rx SNMP fifo of DSL CPE API is empty or firmware does not provide
       any data with interrupt. */
   DSL_WRN_SNMP_NO_DATA = 200,
   /** Currently the only protocol that is handled by the DSL CPE API is
       SNMP (0x814C) */
   DSL_WRN_EOC_UNSUPPORTED_PROTOCOLL_ID = 201,

   /* *********************************************************************** */
   /* *** Warning Codes for configuration parameter consistency check     *** */
   /* *********************************************************************** */
   /** This warning code is not used anymore. */
   DSL_WRN_CONFIG_BND_VS_RETX = 400,
   /** The configuration of the TC-Layer does not fit to the bonding
       configuration. Due to the fact that PAF bonding is only supported
       within PTM/EFM TC-Layer please note that in case of enabled bonding
       support (CLI: "BND_ConfigSet"/"bndcs") the TC-Layer configuration (of the
       DSL Firmware) will be set to fixed PTM/EFM operation.
       This warning will occur in case of bonding is enabled and user
       configuration of \ref DSL_TC_AUTO is applied or vice versa. The
       configuration is accepted but during link configuration only PTM/EFM
       is enabled. */
   DSL_WRN_CONFIG_BND_VS_TCLAYER = 401,
   /** The configuration parameter for upstream (US) direction can be only
       enabled if according downstream (DS) value is enabled. In this case
       the configuration to enable upstream RTX was discared. Please enable
       RTX for downstream first! */
   DSL_WRN_CONFIG_RTX_US_ONLY_SUPPORTED_WITH_DS_ENABLED = 402,
   /* *********************************************************************** */
   /* *** Bonding functionality related warning codes                     *** */
   /* *********************************************************************** */
   /** The DSL PHY Firmware does not support bonding but bonding is required
       from DSL CPE API compilation and configuration point of view.
       In this case the DSL CPE API is compiled for bonding and one of the
       following cases apply
       - bonding is enabled while changing the firmware binary which does not
         support bonding (related CLI command: "alf")
       - a firmware is running that does not support bonding and a
         configuration change to enable bonding will be done (related CLI
         command: "bndcs")
       In both above cases bonding is *not* activated within firmware
       configuration handling. This means that the bonding enable configuration
       is ignored.
       In case of on-chip bonding the firmware is started in single port mode
       and only line 0 is accessible. */
   DSL_WRN_BND_NOT_SUPPORTED_BY_FIRMWARE = 500,

   /* Last warning code marker */
   DSL_WRN_LAST
} DSL_Error_t;

/** @} DRV_DSL_CPE_ERROR_CODES */

#ifdef __cplusplus
}
#endif

#endif /* _DRV_DSL_API_ERROR_H */
