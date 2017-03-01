/******************************************************************************

                              Copyright (c) 2014
                            Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef _DRV_DSL_CPE_API_IOCTL_H
#define _DRV_DSL_CPE_API_IOCTL_H

#ifdef __cplusplus
   extern "C" {
#endif

#include "drv_dsl_cpe_api.h"
#include "drv_dsl_cpe_api_pm.h"
#include "drv_dsl_cpe_api_bnd.h"

#ifdef INCLUDE_DSL_ADSL_MIB
   /**
      Specifies to include ADSL Line Extension MIB (RFC3440).
      \ note The Line Extension MIB will be automatically included if ADSL MIB
             feature is included (no possibility to include RFC2662 only at the
             moment) */
   #define INCLUDE_ADSL_MIB_RFC3440

   #ifndef DSL_DOC_GENERATION_EXCLUDE_ADSL_MIB
      #include "drv_dsl_cpe_api_adslmib.h"
      #include "drv_dsl_cpe_api_adslmib_ioctl.h"
   #endif /* DSL_DOC_GENERATION_EXCLUDE_ADSL_MIB */
#endif

/** \file drv_dsl_cpe_api_ioctl.h
   Definition of all ioctls to be used by upper layer software.

   This file includes all available ioctls (except ADSL MIB part which is
   included within file drv_dsl_cpe_api_adsl_mib_ioctl.h) that should be used
   by an DSL Management application for configuration and control of the
   DSL CPE API.

   \remarks
   Definitions of ioctls is done by using the following standard macros
   - _IOC_DIR
   - _IOC_TYPE,
   - _IOC_NR
   - _IOC_SIZE.
   If _IOC_SIZE returns a value >0 this is the size that can be used
   for copying the data between user and kernel space.
   If  _IOC_SIZE return 0 a special handling is necessary because the
   data size may exceed the max. size of 256 byte.
*/

#ifndef DSL_CPE_API_USE_KERNEL_INTERFACE

/* ************************************************************************** */
/* *** ioctl structures                                                   *** */
/* ************************************************************************** */

/** encapsulate all ioctl command arguments */
typedef union
{
   /* special member */
   DSL_AccessCtl_t                   accessCtl;

   DSL_Init_t                        init;
   DSL_AutobootLoadFirmware_t        autobootLoadFirmware;
   DSL_AutobootControl_t             autobootControl;
   DSL_AutobootConfig_t              autobootConfig;
   DSL_VersionInformation_t          versionInformation;
   DSL_LineState_t                   lineState;
   DSL_LineFeature_t                 lineFeature;
   DSL_FramingParameterStatus_t      framingParameterStatus;
   DSL_SystemInterfaceConfig_t       systemInterfaceConfig;
   DSL_SystemInterfaceStatus_t       systemInterfaceStatus;

   DSL_TestModeControl_t             testModeControl;
   DSL_TestModeStatus_t              testModeStatus;

   DSL_DBG_ModuleLevel_t             dbg_moduleLevel;
   DSL_ShowtimeLogging_t             showtimeLogging;

#ifdef INCLUDE_DSL_CPE_MISC_LINE_STATUS
   DSL_BandBorderStatus_t            bandBorderStatus;
   DSL_MiscLineStatus_t              miscLineStatus;
#endif /* INCLUDE_DSL_CPE_MISC_LINE_STATUS*/
#if defined(INCLUDE_DSL_CPE_API_VRX)
   DSL_LineOptionsConfig_t           lineOptionsConfig;
#endif /*  defined(INCLUDE_DSL_CPE_API_VRX)*/

#ifdef INCLUDE_DEVICE_EXCEPTION_CODES
   DSL_LastExceptionCodes_t          lastExceptionCodes;
#endif /* INCLUDE_DEVICE_EXCEPTION_CODES*/
#ifdef INCLUDE_DSL_CPE_API_DANUBE
#ifndef DSL_DEBUG_DISABLE
   DSL_DBG_DebugFeatureConfig_t     debugFeatureConfig;
#endif /* DSL_DEBUG_DISABLE*/
   DSL_LoopLengthStatus_t           loopLenghtStatus;
#endif /* INCLUDE_DSL_CPE_API_DANUBE*/

   DSL_EventStatus_t                 event;
   DSL_EventStatusMask_t             eventStatusMask;
   DSL_InstanceControl_t             instanceControl;
   DSL_InstanceStatus_t              instanceStatus;

   DSL_DeviceMessage_t               dbgMsg;
   DSL_DeviceMessageModify_t         dbgMsgMod;

   DSL_InteropFeatureConfig_t        interopFeatureConfig;

   DSL_ResourceUsageStatistics_t     resourceUsageStatistics;
   DSL_LowLevelConfiguration_t       lowLevelConfig;
#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
   DSL_AuxLineInventory_t            auxLineInventory;
   DSL_BandPlanSupport_t             bandPlanSupport;
   DSL_BandPlanStatus_t              bandPlanStatus;
   DSL_EFM_MacConfig_t               efmMacConfig;
#endif /* #if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)*/
#ifdef INCLUDE_DSL_CPE_API_DANUBE
   DSL_ReTxStatistics_t              reTxStatistics;
#endif /* INCLUDE_DSL_CPE_API_DANUBE*/

   /* G997 args */
   DSL_G997_PowerManagementStateForcedTrigger_t g997PowerMgmtStateForcedTrigger;
   DSL_G997_PowerManagementStatus_t    g997PowerMgmtStatus;
   DSL_G997_FramingParameterStatus_t   g997FramingParamsStatus;
   DSL_G997_XTUSystemEnabling_t        g997XtuSystemEnabling;
   DSL_G997_LineFailures_t             g997LineFailures;
   DSL_G997_LineActivate_t             g997lineActivateConfig;
   DSL_G997_LineInventory_t            g997LineInvertory;
   DSL_G997_LineInventoryNe_t          g997LineInvertoryNe;
   DSL_G997_LineStatus_t               g997LineStatus;
   DSL_G997_LineInitStatus_t           g997LineInitStatus;
   DSL_G997_LineTransmissionStatus_t   g997LineTransmissionStatus;
   DSL_G997_LastStateTransmitted_t     g997LastStateTransmitted;
   DSL_G997_ChannelDataRateThreshold_t g997ChannelDataRateThreshold;
#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
   DSL_G997_LineStatusPerBand_t        g997LineStatusPerBand;
   DSL_G997_RateAdaptationConfig_t     g997RateAdaptation;
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */
   DSL_G997_ChannelStatus_t            g997ChannelStatus;
   DSL_G997_DataPathFailures_t         g997DataPathFailures;
   DSL_G997_BitAllocationNsc_t         g997BitAllocationNsc;
   DSL_G997_SnrAllocationNsc_t         g997SnrAllocationNsc;
   DSL_G997_GainAllocationNsc_t        g997GainAllocationNsc;
   DSL_G997_DeltHlin_t                 g997DeltHlin;
   DSL_G997_DeltHlinScale_t            g997DeltHlinScale;
   DSL_G997_DeltHlog_t                 g997DeltHlog;
   DSL_G997_DeltQln_t                  g997DeltQln;
#if defined(INCLUDE_DSL_CEOC)
   DSL_G997_Snmp_t                     g997SnmpMsg;
#endif

#ifdef INCLUDE_DSL_BONDING
   DSL_BND_HwInit_t                    bndHwInit;
   DSL_BND_ConfigSet_t                 bndConfigSet;
   DSL_BND_ConfigGet_t                 bndConfigGet;
   DSL_BND_HsStatusGet_t               bndHsStatusGet;
   DSL_BND_HsContinue_t                bndHsContinue;
   DSL_BND_EthDbgCounters_t            bndEthDbgCounters;
   DSL_BND_EthCounters_t               bndEthCounters;
   DSL_BND_PortModeSync_t              portModeSync;
#endif /* INCLUDE_DSL_BONDING*/

#ifdef INCLUDE_PILOT_TONES_STATUS
   DSL_PilotTonesStatus_t              pilotTonesStatus;
#endif /* #ifdef INCLUDE_PILOT_TONES_STATUS*/

/* PM args */
#if defined(INCLUDE_DSL_PM)
#ifdef INCLUDE_DSL_CPE_PM_CONFIG
   DSL_PM_Config_t                  pmConfig;
#endif /* INCLUDE_DSL_CPE_PM_CONFIG */
#ifdef INCLUDE_DSL_CPE_PM_HISTORY
   #ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS
   DSL_PM_ElapsedTimeReset_t        pmElapsedTimeReset;
   DSL_PM_BurninMode_t              pmBurninMode;
   DSL_PM_SyncMode_t                pmSyncMode;
   DSL_PM_ElapsedExtTrigger_t       pmExtTrigger;
   #endif /* #ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS */
   DSL_PM_Reset_t                   pmReset;
#endif /* #ifdef INCLUDE_DSL_CPE_PM_HISTORY */

#ifdef INCLUDE_DSL_CPE_PM_CHANNEL_COUNTERS
   DSL_PM_ChannelCounters_t         pmChannelCounters;
   #ifdef INCLUDE_DSL_CPE_PM_TOTAL_COUNTERS
   DSL_PM_ChannelCountersTotal_t    pmChannelCountersTotal;
   #endif
   #ifdef INCLUDE_DSL_CPE_PM_HISTORY
   #ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS
   DSL_PM_HistoryStatsChDir_t       pmChannelHistoryStats;
   #ifdef INCLUDE_DSL_CPE_PM_CHANNEL_THRESHOLDS
   DSL_PM_ChannelThreshold_t        pmChannelThresholds;
   #endif /* #ifdef INCLUDE_DSL_CPE_PM_CHANNEL_THRESHOLDS */
   #endif /* #ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS */
   #endif /* #ifdef INCLUDE_DSL_CPE_PM_HISTORY */
#endif

#ifdef INCLUDE_DSL_CPE_PM_DATA_PATH_COUNTERS
   DSL_PM_DataPathCounters_t        pmDataPathCounters;
   #ifdef INCLUDE_DSL_CPE_PM_TOTAL_COUNTERS
   DSL_PM_DataPathCountersTotal_t   pmDataPathCountersTotal;
   #endif
   #ifdef INCLUDE_DSL_CPE_PM_HISTORY
   #ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS
   DSL_PM_HistoryStatsChDir_t       pmDataPathHistoryStats;
   #ifdef INCLUDE_DSL_CPE_PM_DATA_PATH_THRESHOLDS
   DSL_PM_DataPathThreshold_t       pmDataPathThresholds;
   #endif /* #ifdef INCLUDE_DSL_CPE_PM_DATA_PATH_THRESHOLDS */
   #endif /* #ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS */
   #endif /* #ifdef INCLUDE_DSL_CPE_PM_HISTORY */
#endif

#ifdef INCLUDE_DSL_CPE_PM_DATA_PATH_FAILURE_COUNTERS
   DSL_PM_DataPathFailureCounters_t      pmDataPathFailureCounters;
   #ifdef INCLUDE_DSL_CPE_PM_TOTAL_COUNTERS
   DSL_PM_DataPathFailureCountersTotal_t pmDataPathFailureCountersTotal;
   #endif
   #ifdef INCLUDE_DSL_CPE_PM_HISTORY
   #ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS
   DSL_PM_HistoryStatsChDir_t       pmDataPathFailureHistoryStats;
   #endif /* #ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS */
   #endif /* #ifdef INCLUDE_DSL_CPE_PM_HISTORY */
#endif /* INCLUDE_DSL_CPE_PM_DATA_PATH_FAILURE_COUNTERS */

#ifdef INCLUDE_DSL_CPE_PM_LINE_COUNTERS
   DSL_PM_LineSecCounters_t         pmLineSecCounters;
   #ifdef INCLUDE_DSL_CPE_PM_TOTAL_COUNTERS
   DSL_PM_LineSecCountersTotal_t    pmLineSecCountersTotal;
   DSL_PM_LineInitCountersTotal_t   pmLineInitCountersTotal;
   #endif /* #ifdef INCLUDE_DSL_CPE_PM_TOTAL_COUNTERS */

   #ifdef INCLUDE_DSL_CPE_PM_HISTORY
   #ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS
   DSL_PM_HistoryStatsDir_t         pmLineSecHistoryStats;
   DSL_PM_HistoryStats_t            pmLineInitHistoryStats;
   #endif /* #ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS */
   DSL_PM_LineInitCounters_t        pmLineInitCounters;
   #ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS
   #ifdef INCLUDE_DSL_CPE_PM_LINE_THRESHOLDS
   DSL_PM_LineSecThreshold_t        pmLineSecThresholds;
   DSL_PM_LineInitThreshold_t       pmLineInitThresholds;
   #endif /* #ifdef INCLUDE_DSL_CPE_PM_LINE_THRESHOLDS */
   #endif /* #ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS */
   #endif /* #ifdef INCLUDE_DSL_CPE_PM_HISTORY */
#endif /* #ifdef INCLUDE_DSL_CPE_PM_LINE_COUNTERS */

#ifdef INCLUDE_DSL_CPE_PM_LINE_EVENT_SHOWTIME_COUNTERS
   DSL_PM_LineEventShowtimeCounters_t pmLineEventShowtimeCounters;
   #ifdef INCLUDE_DSL_CPE_PM_TOTAL_COUNTERS
   DSL_PM_LineEventShowtimeCountersTotal_t pmLineEventShowtimeCountersTotal;
   #endif
   #ifdef INCLUDE_DSL_CPE_PM_HISTORY
   #ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS
   DSL_PM_HistoryStatsChDir_t        pmLineEventShowtimeHistoryStats;
   #endif /* #ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS */
   #endif /* #ifdef INCLUDE_DSL_CPE_PM_HISTORY */
#endif /* INCLUDE_DSL_CPE_PM_LINE_EVENT_SHOWTIME_COUNTERS */

#ifdef INCLUDE_DSL_CPE_PM_RETX_COUNTERS
   DSL_PM_ReTxCounters_t             pmReTxCounters;
   #ifdef INCLUDE_DSL_CPE_PM_TOTAL_COUNTERS
   DSL_PM_ReTxCountersTotal_t        pmReTxCountersTotal;
   #endif
   #ifdef INCLUDE_DSL_CPE_PM_HISTORY
   #ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS
   DSL_PM_HistoryStatsChDir_t        pmReTxHistoryStats;
   #ifdef INCLUDE_DSL_CPE_PM_RETX_THRESHOLDS
   DSL_PM_ReTxThreshold_t            pmReTxThresholds;
   #endif /* INCLUDE_DSL_CPE_PM_RETX_THRESHOLDS */
   #endif /* INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS */
   #endif /* INCLUDE_DSL_CPE_PM_HISTORY */
#endif /* INCLUDE_DSL_CPE_PM_RETX_COUNTERS */

#endif /* defined(INCLUDE_DSL_PM) */

/*RTT args*/
#ifdef INCLUDE_REAL_TIME_TRACE
   DSL_RTT_Init_t                   rttInit;
   DSL_RTT_Config_t                 rttConfig;
   DSL_RTT_Status_t                 rttStatus;
   DSL_RTT_Control_t                rttControl;
   DSL_RTT_Statistics_t             rttStatistics;
#endif /*#ifdef INCLUDE_REAL_TIME_TRACE*/

   DSL_OlrStatistics_t              olrStatistics;

#ifdef INCLUDE_DSL_CPE_API_VRX
   DSL_G997_LowPowerModeConfig_t    lpmConfig;
#endif /* INCLUDE_DSL_CPE_API_VRX*/

   DSL_FirmwareDownloadStatus_t     fwDwnlStatus;
} DSL_IOCTL_arg_t;


/* ************************************************************************** */
/* *** ioctl commands                                                     *** */
/* ************************************************************************** */

/**
   Magic number for common DSL CPE_API ioctls. */
#define DSL_IOC_MAGIC_CPE_API          'r'

/**
   Magic number for G.997.1 related DSL CPE_API ioctls. */
#define DSL_IOC_MAGIC_CPE_API_G997     'g'

/**
   Magic number for Performance Monitoring related DSL CPE_API ioctls. */
#define DSL_IOC_MAGIC_CPE_API_PM       'p'

/**
   Magic number for bonding (PAF - PMA Aggregation Function) related DSL CPE_API
   ioctls.
   \note This part is only available for VDSL capable build type and if the
         bonding functionality is included within build. */
#define DSL_IOC_MAGIC_CPE_API_BND      'b'

/**
   Magic number for deprecated ioctl's of the DSL CPE_API.
   \note This ioctl's will be removed on mid term. */
#define DSL_IOC_MAGIC_CPE_API_DEP      'd'

/**
   Magic number for MIB related ioctls of DSL CPE_API.
   \note This part is only available for ADSL only capable platforms. */
#define DSL_IOC_MAGIC_MIB              'm'

/**
   Magic number for Real Time Trace related DSL CPE_API ioctls.
   \note This part is currently only available for ADSL only capable
         platforms. */
#define DSL_IOC_MAGIC_CPE_API_RTT      't'

/* ************************************************************************** */
/* * Ioctl interface definitions for common functions                       * */
/* ************************************************************************** */

/**
   This ioctl initializes the DSL CPE_API.

   CLI
   - n/a
   - Note: This ioctl is called within context of starting up the control
           application in case of using '-i' option, also refer to UMPR
           "Command line arguments for the DSL CPE Control Application"

   \param DSL_Init_t*
      The parameter points to a \ref DSL_Init_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_Init_t init;
      DSL_int_t ret = 0;

      memset(&init, 0x00, sizeof(DSL_Init_t));
      // Initialize the init structure here before calling the ioctl...
      // init. = ;
      ret = ioctl(fd, DSL_FIO_INIT, &init);
   \endcode

   \ingroup DRV_DSL_CPE_INIT */
#define DSL_FIO_INIT \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 0, DSL_Init_t)

/**
   This ioctl downloads a firmware.

   CLI
   - long command: AutobootLoadFirmware
   - short command: alf

   \param DSL_AutobootLoadFirmware_t*
      The parameter points to a \ref DSL_AutobootLoadFirmware_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_AutobootLoadFirmware_t fw;
      DSL_int_t ret = 0;

      memset(&fw, 0x00, sizeof(DSL_AutobootLoadFirmware_t));
      // Initialize the firmware structure here before calling the ioctl...
      // fw. = ;
      ret = ioctl(fd, DSL_FIO_AUTOBOOT_LOAD_FIRMWARE, &fw);
   \endcode

   \ingroup DRV_DSL_CPE_INIT */
#define DSL_FIO_AUTOBOOT_LOAD_FIRMWARE \
   _IOWR( DSL_IOC_MAGIC_CPE_API, 1, DSL_AutobootLoadFirmware_t )

/**
   This ioctl makes it possible to control the autoboot handling.

   CLI
   - long command: AutobootControlSet
   - short command: acs

   \param DSL_AutobootControl_t*
      The parameter points to a \ref DSL_AutobootControl_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_AutobootControl_t ctrl;
      DSL_int_t ret = 0;

      memset(&ctrl, 0x00, sizeof(DSL_AutobootControl_t));
      // Initialize the autoboot control structure here before calling the
      // ioctl...
      // ctrl. = ;
      ret = ioctl(fd, DSL_FIO_AUTOBOOT_CONTROL_SET, &ctrl);
   \endcode

   \ingroup DRV_DSL_CPE_INIT */
#define DSL_FIO_AUTOBOOT_CONTROL_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 2, DSL_AutobootControl_t)

/**
   This ioctl makes it possible to get the current state of the autoboot
   handling.

   CLI
   - long command: AutobootStatusGet
   - short command: asg

   \param DSL_AutobootStatus_t*
      The parameter points to a \ref DSL_AutobootStatus_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_AutobootStatus_t status;
      DSL_int_t ret = 0;

      memset(&status, 0x00, sizeof(DSL_AutobootStatus_t));
      ret = ioctl(fd, DSL_FIO_AUTOBOOT_STATUS_GET, &status);
   \endcode

   \ingroup DRV_DSL_CPE_INIT */
#define DSL_FIO_AUTOBOOT_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 3, DSL_AutobootStatus_t)

/**
   This ioctl requests the version information of all used DSL software and
   hardware components below Software CPE_API level.

   CLI
   - long command: VersionInformationGet
   - short command: vig

   \param DSL_VersionInformation_t*
      The parameter points to a \ref DSL_VersionInformation_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_VersionInformation_t version;
      DSL_int_t ret = 0;

      memset(&version, 0x00, sizeof(DSL_VersionInformation_t));
      ret = ioctl(fd, DSL_FIO_VERSION_INFORMATION_GET, &version);
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_VERSION_INFORMATION_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 4, DSL_VersionInformation_t)

/**
   This ioctl requests information about the current line state.

   CLI
   - long command: LineStateGet
   - short command: lsg

   \param DSL_LineState_t*
      The parameter points to a \ref DSL_LineState_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_LineState_t lineState;
      DSL_int_t ret = 0;

      memset(&lineState, 0x00, sizeof(DSL_LineState_t));
      ret = ioctl(fd, DSL_FIO_LINE_STATE_GET, &lineState);
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_LINE_STATE_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 5, DSL_LineState_t)

/**
   This ioctl configures various common line specific features.

   CLI
   - long command: LineFeatureConfigSet
   - short command: lfcs

   \param DSL_LineFeature_t*
      The parameter points to a \ref DSL_LineFeature_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   - Supported by all platforms.
   - Please note that a change of this configuration setting(s) will be only
     effective after restarting the autoboot handling using ioctl
     \ref DSL_FIO_AUTOBOOT_CONTROL_SET with command \ref DSL_AUTOBOOT_CTRL_RESTART

   \code
      DSL_LineFeature_t lineFeatureConfig;
      DSL_int_t ret = 0;

      memset(&lineFeatureConfig, 0x00, sizeof(DSL_LineFeature_t));
      ret = ioctl(fd, DSL_FIO_LINE_FEATURE_CONFIG_SET, &lineFeatureConfig);
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_LINE_FEATURE_CONFIG_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 6, DSL_LineFeature_t)

/**
   This ioctl returns the current configurations for various common line specific
   features.

   CLI
   - long command: LineFeatureConfigGet
   - short command: lfcg

   \param DSL_LineFeature_t*
      The parameter points to a \ref DSL_LineFeature_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_LineFeature_t lineFeatureConfig;
      DSL_int_t ret = 0;

      memset(&lineFeatureConfig, 0x00, sizeof(DSL_LineFeature_t));
      ret = ioctl(fd, DSL_FIO_LINE_FEATURE_CONFIG_GET, &lineFeatureConfig);
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_LINE_FEATURE_CONFIG_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 7, DSL_LineFeature_t)

/**
   This ioctl returns the current status of various common line specific features.

   CLI
   - long command: LineFeatureStatusGet
   - short command: lfsg

   \param DSL_LineFeature_t*
      The parameter points to a \ref DSL_LineFeature_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Please note that the state of this status values are only updated if showtime
   is reached otherwise they might reflect inconsistent states.
   To check if the line is in showtime please use ioctl
   \ref DSL_FIO_LINE_STATE_GET before using this functionality if you are not
   sure about the line state.
   Supported by all platforms.

   \code
      DSL_LineFeatureStatusGet_t lineFeatureConfig;
      DSL_int_t ret = 0;

      memset(&lineFeatureConfig, 0x00, sizeof(DSL_LineFeature_t));
      ret = ioctl(fd, DSL_FIO_LINE_FEATURE_STATUS_GET, &lineFeatureConfig);
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_LINE_FEATURE_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 8, DSL_LineFeature_t)

/**
   This ioctl requests information about the current status for various common
   framing parameters.

   CLI
   - long command: FramingParameterStatusGet
   - short command: fpsg

   \param DSL_FramingParameterStatus_t*
      The parameter points to a \ref DSL_FramingParameterStatus_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Please note that the state of this status values are only updated if showtime
   is reached otherwise they might reflect inconsistent values.
   To check if the line is in showtime please use ioctl
   \ref DSL_FIO_LINE_STATE_GET before using this functionality if you are not
   sure about the line state.
   Supported by all platforms.

   \code
      DSL_FramingParameterStatus_t framingParam;
      DSL_int_t ret = 0;

      memset(&framingParam, 0x00, sizeof(DSL_LineFeatureStatusGet_t));
      ret = ioctl(fd, DSL_FIO_FRAMING_PARAMETER_STATUS_GET, &framingParam);
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_FRAMING_PARAMETER_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 9, DSL_FramingParameterStatus_t)

/**
   This debug function sets a the debug level of the given module number.

   CLI
   - long command: DBG_ModuleLevelSet
   - short command: dbgmls

   \note Implementation only available if preprocessor directive
         'DSL_DEBUG_DISABLE' is not set.

   \param DSL_DBG_ModuleLevel_t*
      The parameter points to a \ref DSL_DBG_ModuleLevel_t structure
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \return
      0 if successful and -1 in case of an error/warning

   \remarks
   Supported by all platforms.

   \code
      DSL_DBG_ModuleLevel_t dbgModuleLevel;
      DSL_int_t ret = 0;

      memset(&dbgModuleLevel, 0x00, sizeof(DSL_DBG_ModuleLevel_t));
      ret = ioctl(fd, DSL_FIO_DBG_MODULE_LEVEL_SET, &dbgModuleLevel);
   \endcode

   \ingroup DRV_DSL_CPE_DEBUG */
#define DSL_FIO_DBG_MODULE_LEVEL_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 10, DSL_DBG_ModuleLevel_t)

/**
   This debug function returns the debug level from the given module number.

   CLI
   - long command: DBG_ModuleLevelGet
   - short command: dbgmlg

   \note Implementation only available if preprocessor directive
         'DSL_DEBUG_DISABLE' is not set.

   \param DSL_DBG_ModuleLevel_t*
      The parameter points to a \ref DSL_DBG_ModuleLevel_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_DBG_ModuleLevel_t dbgModuleLevel;
      DSL_int_t ret = 0;

      memset(&dbgModuleLevel, 0x00, sizeof(DSL_DBG_ModuleLevel_t));
      ret = ioctl(fd, DSL_FIO_DBG_MODULE_LEVEL_GET, &dbgModuleLevel);
   \endcode

   \ingroup DRV_DSL_CPE_DEBUG */
#define DSL_FIO_DBG_MODULE_LEVEL_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 11, DSL_DBG_ModuleLevel_t)

/**
   This debug function makes it possible to send a low level device message.

   CLI
   - long command: DeviceMessageSend
   - short command: dms

   \param DSL_DeviceMessage_t*
      The parameter points to a \ref DSL_DeviceMessage_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_DeviceMessage_t devMsg;
      DSL_int_t ret = 0;

      memset(&devMsg, 0x00, sizeof(DSL_DeviceMessage_t));
      // Initialize the device message here before calling the ioctl...
      ret = ioctl(fd, DSL_FIO_DBG_DEVICE_MESSAGE_SEND, &devMsg);
   \endcode

   \ingroup DRV_DSL_CPE_DEBUG */
#define DSL_FIO_DBG_DEVICE_MESSAGE_SEND \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 12, DSL_DeviceMessage_t)

/**
   This debug function makes it possible to send a low level device message.
   In addition to the data word a mask word is used that makes it possible to
   write single bits or bit groups instead of the complete data word.

   <tt>
      Example\n
      --------\n
      Act. Msg value.: 0xFF00 -> 1111 1111 0000 0000\n
      data word......: 0xF0F0 -> 1111 0000 1111 0000\n
      mask word......: 0xAAAA -> 1010 1010 1010 1010\n
      ------------------------------------------------\n
      Res. Msg value.: 0xF5A0 -> 1111 0101 1010 0000\n
      ================================================
   </tt>

   CLI
   - long command: DeviceMessageModifySend
   - short command: dmms

   \param DSL_DeviceMessageModify_t*
      The parameter points to a \ref DSL_DeviceMessageModify_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_DeviceMessageModify_t devMsg;

      // Initialize the device message here before calling the ioctl...
      memset(&devMsg, 0x00, sizeof(DSL_DeviceMessageModify_t));
      // Set message data here (data and mask)
      ret = ioctl(fd, DSL_FIO_DBG_DEVICE_MESSAGE_MODIFY_SEND, &devMsg)
   \endcode

   \ingroup DRV_DSL_CPE_DEBUG */
#define DSL_FIO_DBG_DEVICE_MESSAGE_MODIFY_SEND \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 61, DSL_DeviceMessageModify_t)

/**
   This function provides the possibility to activate a testmode.

   CLI
   - long command: TestModeControlSet
   - short command: tmcs

   \param DSL_TestModeControl_t*
      The parameter points to a \ref DSL_TestModeControl_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_TestModeControl_t testModeCtrl;
      DSL_int_t ret = 0;

      memset(&testModeCtrl, 0x00, sizeof(DSL_TestModeControl_t));
      // Set testmode here before calling the ioctl...
      testModeCtrl.data,nTestMode = DSL_TESTMODE_SHOWTIME_LOCK;
      ret = ioctl(fd, DSL_FIO_TEST_MODE_CONTROL_SET, &testModeCtrl);
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_TEST_MODE_CONTROL_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 13, DSL_TestModeControl_t)

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   This function retrieves the auxiliary line inventory information according
   to G.993.2 (see chapter 11.2.3 EOC commands and responses) directly from the
   device/CO. As this field is defined without a limit in size, it is not stored
   inside the DSL CPE API.
   The auxiliary inventory includes the VTU equipment ID auxiliary inventory
   information and self-test results.
   To set this information use \ref DSL_FIO_INIT or
   \ref DSL_FIO_G997_LINE_INVENTORY_SET.

   CLI
   - long command: AuxLineInventoryGet
   - short command: alig

   \param DSL_AuxLineInventory_t*
      The parameter points to a \ref DSL_AuxLineInventory_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - XWAY(TM) VRX200: xDSL-CPE
   - XWAY(TM) VRX300: xDSL-CPE

   \code
      DSL_AuxLineInventory_t auxLineInventory;
      DSL_int_t ret = 0;

      memset(&auxLineInventory, 0x00, sizeof(DSL_AuxLineInventory_t));
      ret = ioctl(fd, DSL_FIO_AUX_LINE_INVENTORY_GET, &auxLineInventory);
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_AUX_LINE_INVENTORY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 14, DSL_AuxLineInventory_t)
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */

/**
   This function returns the measured loop length for different line types
   in meters.
   The implementation of this function is device specific!

   CLI
   - long command: LoopLengthStatusGet
   - short command: llsg

   \param DSL_LoopLengthStatus_t*
      The parameter points to a \ref DSL_LoopLengthStatus_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_LoopLengthStatus_t loopLength;
      DSL_int_t ret = 0;

      memset(&loopLength, 0x00, sizeof(DSL_LoopLengthStatus_t));
      ret = ioctl(fd, DSL_FIO_LOOP_LENGTH_STATUS_GET, &loopLength);
      // Read and/or process results here...
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_LOOP_LENGTH_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 15, DSL_LoopLengthStatus_t)

/**
   This function configures the System Interface.

   CLI
   - long command: SystemInterfaceConfigSet
   - short command: sics

   \param DSL_SystemInterfaceConfig_t*
      The parameter points to a \ref DSL_SystemInterfaceConfig_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   - Supported by all platforms.
   - Please note that a change of this configuration setting(s) will be only
     effective after restarting the autoboot handling using ioctl
     \ref DSL_FIO_AUTOBOOT_CONTROL_SET with command \ref DSL_AUTOBOOT_CTRL_RESTART

   \code
      DSL_SystemInterfaceConfig_t ifCfg;
      DSL_int_t ret = 0;

      memset(&ifCfg, 0x00, sizeof(DSL_SystemInterfaceConfig_t));
      // Set configuration settings here before calling the ioctl...
      ret = ioctl(fd, DSL_FIO_SYSTEM_INTERFACE_CONFIG_SET, &ifCfg);
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_SYSTEM_INTERFACE_CONFIG_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 22, DSL_SystemInterfaceConfig_t)

/**
   This function returns the selected System Interface mode.

   CLI
   - long command: SystemInterfaceConfigGet
   - short command: sicg

   \param DSL_SystemInterfaceConfig_t*
      The parameter points to a \ref DSL_SystemInterfaceConfig_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_SystemInterfaceConfig_t ifCfg;
      DSL_int_t ret = 0;

      memset(&ifCfg, 0x00, sizeof(DSL_SystemInterfaceConfig_t));
      ret = ioctl(fd, DSL_FIO_SYSTEM_INTERFACE_CONFIG_GET, &ifCfg);
      // Read and/or process configuration settings here...
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_SYSTEM_INTERFACE_CONFIG_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 23, DSL_SystemInterfaceConfig_t)

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   This function reads back the selected Bandplan.

   CLI
   - n/a

   \param DSL_BandPlanStatus_t*
      The parameter points to a \ref DSL_BandPlanStatus_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Additional Band Plan information is provided by the the following ioctl
   \ref DSL_FIO_BAND_BORDER_STATUS_GET
   Supported by
   - XWAY(TM) VRX200: xDSL-CPE
   - XWAY(TM) VRX300: xDSL-CPE

   \code
      DSL_BandPlanStatus_t bpCfg;
      DSL_int_t ret = 0;

      memset(&bpCfg, 0x00, sizeof(DSL_BandPlanStatus_t));
      ret = ioctl(fd, DSL_FIO_BAND_PLAN_STATUS_GET, &bpCfg);
      // Read and/or process configuration settings here...
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_BAND_PLAN_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 24, DSL_BandPlanStatus_t)
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   This function returns all VDSL2 bandplan/profile combinations supported by
   the device.

   CLI
   - long command: BandPlanSupportGet
   - short command: bpsg

   \param DSL_BandPlanSupport_t*
      The parameter points to a \ref DSL_BandPlanSupport_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - XWAY(TM) VRX200: xDSL-CPE
   - XWAY(TM) VRX300: xDSL-CPE

   \code
      DSL_BandPlanSupport_t bpSupport;
      DSL_int_t ret = 0;

      memset(&bpSupport, 0x00, sizeof(DSL_BandPlanSupport_t));
      ret = ioctl(fd, DSL_FIO_BAND_PLAN_SUPPORT_GET, &bpSupport);
      // Read and/or process configuration settings here...
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_BAND_PLAN_SUPPORT_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 25, DSL_BandPlanSupport_t)
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */

/**
   This ioctl reads the showtime event logging data.

   CLI
   - Not applicable for CLI interface
   - Note: This ioctl is called within context of exception handling in case of
           ADSL only platform build. Also refer to the UMPR,
           - configure option: --enable-adsl-trace
           - event type: DSL_EVENT_S_SHOWTIME_LOGGING

   \param DSL_ShowtimeLogging_t*
      The parameter points to a \ref DSL_ShowtimeLogging_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - ADSL only platforms (XWAY(TM) ARX100 and XWAY(TM) ARX300)

   \code
      DSL_ShowtimeLogging_t data;
      DSL_int_t ret = 0;

      memset(&data, 0x00, sizeof(DSL_ShowtimeLogging_t));
      fw. = ;
      ret = ioctl(fd, DSL_FIO_SHOWTIME_LOGGING_DATA_GET, &data);
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_SHOWTIME_LOGGING_DATA_GET \
   _IOWR( DSL_IOC_MAGIC_CPE_API, 28, DSL_ShowtimeLogging_t )



/**
   This function configures the mask for one given the status event.
   For a description on status events please refer to the description of
   \ref DSL_EventType_t.

   CLI
   - long command: EventStatusMaskConfigSet
   - short command: esmcs

   \param DSL_EventStatusMask_t*
      The parameter points to a \ref DSL_EventStatusMask_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \note
   The function call provides the possibility to set a line specific mask for a
   given event type.
   If events are direction specific both directions are set together.
   Some events does not provide to possibility to set this mask line specific.
   Therefore please take the following restrictions into account.
   DSL_CBS_INIT_READY
      This is a common event which is available one ONCE per DSL API instance.
      If this setting will be changed for one line the settings from ALL other
      available lines will be set automatically to the same value.
   DSL_CBS_SYSTEM_STATUS
      This is a device specific event which is available one per device.
      If this setting will be changed for one line the settings from ALL other
      lines from the same device will be set automatically to the same value.

   \remarks
   Supported by all platforms.

   \code
      DSL_EventStatusMask_t statusEventMaskCfgSet;
      DSL_int_t ret = 0;

      memset(&statusEventMaskCfgSet, 0x00, sizeof(DSL_EventStatusMask_t));
      ret = ioctl(fd, DSL_FIO_EVENT_STATUS_MASK_CONFIG_SET, &statusEventMaskCfgSet);
   \endcode

   \ingroup DRV_DSL_CPE_EVENT */
#define DSL_FIO_EVENT_STATUS_MASK_CONFIG_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 29, DSL_EventStatusMask_t)

/**
   This function returns current configuration of the mask for one given status
   event. For a description on status events please refer to the description of
   \ref DSL_EventType_t.

   CLI
   - long command: EventStatusMaskConfigGet
   - short command: esmcg

   \param DSL_EventStatusMask_t*
      The parameter points to a \ref DSL_EventStatusMask_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_EventStatusMask_t statusEventMaskCfgGet;
      DSL_int_t ret = 0;

      memset(&statusEventMaskCfgGet, 0x00, sizeof(DSL_EventStatusMask_t));
      ret = ioctl(fd, DSL_FIO_EVENT_STATUS_MASK_CONFIG_GET, &statusEventMaskCfgGet);
   \endcode

   \ingroup DRV_DSL_CPE_EVENT */
#define DSL_FIO_EVENT_STATUS_MASK_CONFIG_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 30, DSL_EventStatusMask_t)

/**
   This ioctl gets device specific low level configuration.

   CLI
   - long command: LowLevelConfigurationGet
   - short command: llcg

   \param DSL_DeviceLowLevelConfig_t*
      The parameter points to a \ref DSL_LowLevelConfiguration_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_LowLevelConfiguration_t data;
      DSL_int_t ret = 0;

      memset(&data, 0x00, sizeof(DSL_LowLevelConfiguration_t));
      data. = ;
      ret = ioctl(fd, DSL_FIO_LOW_LEVEL_CONFIGURATION_GET, &data);
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_LOW_LEVEL_CONFIGURATION_GET \
    _IOWR(DSL_IOC_MAGIC_CPE_API, 31, DSL_LowLevelConfiguration_t)

/**
   This ioctl sets device specific low level configuration.

   CLI
   - long command: LowLevelConfigurationSet
   - short command: llcs

   \param DSL_LowLevelConfiguration_t*
      The parameter points to a \ref DSL_LowLevelConfiguration_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_LowLevelConfiguration_t data;
      DSL_int_t ret = 0;

      memset(&data, 0x00, sizeof(DSL_LowLevelConfiguration_t));
      data. = ;
      ret = ioctl(fd, DSL_FIO_LOW_LEVEL_CONFIGURATION_GET, &data);
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_LOW_LEVEL_CONFIGURATION_SET \
    _IOWR(DSL_IOC_MAGIC_CPE_API, 32, DSL_LowLevelConfiguration_t)

/**
   This function provides the possibility to get the status for the currently
   activated testmode.

   CLI
   - long command: TestModeStatusGet
   - short command: tmsg

   \param DSL_TestModeStatus_t*
      The parameter points to a \ref DSL_TestModeStatus_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_TestModeStatus_t testModeStatus;
      DSL_int_t ret = 0;

      memset(&testModeStatus, 0x00, sizeof(DSL_TestModeStatus_t));
      ret = ioctl(fd, DSL_FIO_TEST_MODE_STATUS_GET, &testModeStatus);
      // Read and/or process testmode here...
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_TEST_MODE_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 33, DSL_TestModeStatus_t)

/**
   This function returns the currently activated System Interface mode.

   CLI
   - long command: SystemInterfaceStatusGet
   - short command: sisg

   \param DSL_SystemInterfaceStatus_t*
      The parameter points to a \ref DSL_SystemInterfaceStatus_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_SystemInterfaceStatus_t systemInterfaceStatus;
      DSL_int_t ret = 0;

      memset(&systemInterfaceStatus, 0x00, sizeof(DSL_SystemInterfaceStatus_t));
      ret = ioctl(fd, DSL_FIO_SYSTEM_INTERFACE_STATUS_GET, &systemInterfaceStatus);
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_SYSTEM_INTERFACE_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 34, DSL_SystemInterfaceStatus_t)

/**
   This function has to be used to get information on an event that has been
   occurred. The function is designed to be called by the upper layer software
   in case of using event (interrupt) handling and in case of a wakeup of the
   upper layer software by the DSL CPE API.
   There is always an event identifier included within the returned information.
   According the event type there might be also additional event data included.

   CLI
   - Not applicable for CLI interface
   - Note: This ioctl is called within context of event handling within control
           application. Also refer to the UMPR "Event Handling"

   \param DSL_EventStatus_t*
      The parameter points to a \ref DSL_EventStatus_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_EventStatus_t eventStatus;
      DSL_int_t ret = 0;

      memset(&eventStatus, 0x00, sizeof(DSL_EventStatus_t));
      ret = ioctl(fd, DSL_FIO_EVENT_STATUS_GET, &eventStatus);
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_EVENT_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 35, DSL_EventStatus_t)

/**
   This function has to be used to initialize instance specific handling.
   Usually this function shall be called directly after using \ref DSL_FIO_INIT
   ioctl.

   CLI
   - long command: InstanceControlSet
   - short command: ics

   \note After opening an instance (and using \ref DSL_FIO_INIT ioctl) the
         default configuration of instance control parameters is as follows
         - event handling is OFF
         - NO additional event specific resources are allocated
         This configuration ensures that a minimum of runtime memory is used for
         according functionality.

   \param DSL_InstanceControl_t*
      The parameter points to a \ref DSL_InstanceControl_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_InstanceControl_t instanceControl;
      DSL_int_t ret = 0;

      memset(&instanceControl, 0x00, sizeof(DSL_InstanceControl_t));
      instanceControl.data.bEventActivation = DSL_TRUE;
      instanceControl.data.nResourceActivationMask = DSL_RESOURCE_ACTIVATION_SNMP;
      ret = ioctl(fd, DSL_FIO_INSTANCE_CONTROL_SET, &instanceControl);
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_INSTANCE_CONTROL_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 36, DSL_InstanceControl_t)

/**
   This function has to be used to get information about current instance
   specific configurations.

   CLI
   - long command: InstanceStatusGet
   - short command: isg

   \param DSL_InstanceStatus_t*
      The parameter points to a \ref DSL_InstanceStatus_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_InstanceStatus_t instanceStatus;
      DSL_int_t ret = 0;

      memset(&instanceStatus, 0x00, sizeof(DSL_InstanceStatus_t));
      ret = ioctl(fd, DSL_FIO_EVENT_ACTIVATION_STATUS_GET, &instanceStatus);
      // instanceStatus.data... includes current status
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_INSTANCE_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 37, DSL_InstanceStatus_t)

/**
   This function has to be used to get the autoboot configuration besides the
   init configuration at runtime

   CLI
   - long command: AutobootConfigOptionGet
   - short command: acog
   - NOTE: The implementation within driver part only covers the first three
           parameters from CLI command. Parameter four to six are implemented
           application specific only!

   \param DSL_AutobootConfig_t*
      The parameter points to a \ref DSL_AutobootConfig_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_AutobootConfig_t autobootConfig;
      DSL_int_t ret = 0;

      memset(&autobootConfig, 0x00, sizeof(DSL_AutobootConfig_t));
      ret = ioctl(fd, DSL_FIO_AUTOBOOT_CONFIG_SET, &autobootConfig);
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_AUTOBOOT_CONFIG_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 39, DSL_AutobootConfig_t)

/**
   This function has to be used to change the autoboot configuration besides the
   init configuration at runtime

   CLI
   - long command: AutobootConfigOptionSet
   - short command: acos
   - NOTE: The implementation within driver part only covers the first three
           parameters from CLI command. Parameter four to six are implemented
           application specific only!

   \param DSL_AutobootConfig_t*
      The parameter points to a \ref DSL_AutobootConfig_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_AutobootConfig_t autobootConfig;
      DSL_int_t ret = 0;

      memset(&autobootConfig, 0x00, sizeof(DSL_AutobootConfig_t));
      autobootConfig.data.nStateMachineOptions = DSL_TRUE;
      ret = ioctl(fd, DSL_FIO_AUTOBOOT_CONFIG_GET, &autobootConfig);
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_AUTOBOOT_CONFIG_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 40, DSL_AutobootConfig_t)

/**
   This function has to be used to get the current configuration options that
   will be used for interoperability issues.

   CLI
   - long command: InteropFeatureConfigGet
   - short command: ifcg

   \param DSL_InteropFeatureConfig_t*
      The parameter points to a \ref DSL_InteropFeatureConfig_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - ADSL only platforms (XWAY(TM) ARX100, XWAY(TM) ARX300)

   \code
      DSL_InteropFeatureConfig_t interopFeatureConfig;
      DSL_int_t ret = 0;

      memset(&interopFeatureConfig, 0x00, sizeof(DSL_InteropFeatureConfig_t));
      ret = ioctl(fd, DSL_FIO_INTEROP_FEATURE_CONFIG_GET, &interopFeatureConfig);
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_INTEROP_FEATURE_CONFIG_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 41, DSL_InteropFeatureConfig_t)

/**
   This function has to be used to set the current configuration options that
   will be used for interoperability issues.

   CLI
   - long command: InteropFeatureConfigSet
   - short command: ifcs

   \param DSL_InteropFeatureConfig_t*
      The parameter points to a \ref DSL_InteropFeatureConfig_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - ADSL only platforms (XWAY(TM) ARX100, XWAY(TM) ARX300)

   \code
      DSL_InteropFeatureConfig_t interopFeatureConfig;
      DSL_int_t ret = 0;

      memset(&interopFeatureConfig, 0x00, sizeof(DSL_InteropFeatureConfig_t));
      interopFeatureConfig.data.nStateMachineOptions = DSL_TRUE;
      ret = ioctl(fd, DSL_FIO_INTEROP_FEATURE_CONFIG_SET, &interopFeatureConfig);
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_INTEROP_FEATURE_CONFIG_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 42, DSL_InteropFeatureConfig_t)

/**
   This function returns DSL CPE API memory consumption statistics for both
   statically and dynamically allocated memory.

   CLI
   - long command: ResourceUsageStatisticsGet
   - short command: rusg

   \param DSL_ResourceUsageStatistics_t*
   The parameter points to a \ref DSL_ResourceUsageStatistics_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_ResourceUsageStatistics_t resourceStat;
      DSL_int_t ret = 0;

      memset(&resourceStat, 0x00, sizeof(DSL_ResourceUsageStatistics_t));
      ret = ioctl(fd, DSL_FIO_RESOURCE_USAGE_STATISTICS_GET, &resourceStat);
      // resourceStat includes statistics values to process
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_RESOURCE_USAGE_STATISTICS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 44, DSL_ResourceUsageStatistics_t)

/**
   This function returns miscellaneous line status values.

   CLI
   - long command: MiscLineStatusGet
   - short command: mlsg

   \param DSL_MiscLineStatus_t*
   The parameter points to a \ref DSL_MiscLineStatus_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - ADSL only platforms (XWAY(TM) ARX100, XWAY(TM) ARX300)

   \code
      DSL_MiscLineStatus_t miscLineStatus;
      DSL_int_t ret = 0;

      memset(&miscLineStatus, 0x00, sizeof(DSL_MiscLineStatus_t));
      ret = ioctl(fd, DSL_FIO_MISC_LINE_STATUS_GET, &miscLineStatus);
      // miscLineStatus includes statistics values to process
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_MISC_LINE_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 45, DSL_MiscLineStatus_t)

/**
   This function returns information about used bands (frequency ranges that are
   used respective are intended to be used).

   CLI
   - long command: BandBorderStatusGet
   - short command: bbsg

   \param DSL_BandBorderStatus_t*
   The parameter points to a \ref DSL_BandBorderStatus_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_BandBorderStatus_t bandBoarderStatus;
      DSL_int_t ret = 0;

      memset(&bandBoarderStatus, 0x00, sizeof(DSL_BandBorderStatus_t));
      ret = ioctl(fd, DSL_FIO_BAND_BORDER_STATUS_GET, &bandBoarderStatus);
      // bandBoarderStatus includes statistics values to process
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_BAND_BORDER_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 46, DSL_BandBorderStatus_t)

/**
   This function sets line specific options.

   CLI
   - long command: LineOptionsConfigSet
   - short command: locs

   \param DSL_LineOptionsConfig_t*
   The parameter points to a \ref DSL_LineOptionsConfig_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   - Supported by ADSL only platforms (XWAY(TM) ARX100, XWAY(TM) ARX300)
   - Please note that a change of this configuration setting(s) will be only
     effective after restarting the autoboot handling using ioctl
     \ref DSL_FIO_AUTOBOOT_CONTROL_SET with command \ref DSL_AUTOBOOT_CTRL_RESTART

   \code
      DSL_LineOptionsConfig_t lineOptionsConfig;
      DSL_int_t ret = 0;

      memset(&lineOptionsConfig, 0x00, sizeof(DSL_LineOptionsConfig_t));
      // Set configuration settings here before calling the ioctl...
      ret = ioctl(fd, DSL_FIO_LINE_OPTIONS_CONFIG_SET, &lineOptionsConfig);
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_LINE_OPTIONS_CONFIG_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 47, DSL_LineOptionsConfig_t)

/**
   This function returns line specific options.

   CLI
   - long command: LineOptionsConfigGet
   - short command: locg

   \param DSL_LineOptionsConfig_t*
   The parameter points to a \ref DSL_LineOptionsConfig_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - ADSL only platforms (XWAY(TM) ARX100, XWAY(TM) ARX300)

   \code
      DSL_LineOptionsConfig_t lineOptionsConfig;
      DSL_int_t ret = 0;

      memset(&lineOptionsConfig, 0x00, sizeof(DSL_LineOptionsConfig_t));
      // Set configuration settings here before calling the ioctl...
      ret = ioctl(fd, DSL_FIO_LINE_OPTIONS_CONFIG_SET, &lineOptionsConfig);
      // lineOptionsConfig includes configuration values to process
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_LINE_OPTIONS_CONFIG_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 48, DSL_LineOptionsConfig_t)

#ifdef INCLUDE_DEVICE_EXCEPTION_CODES
/**
   This function returns the last device specific exception codes.

   CLI
   - long command: LastExceptionCodesGet
   - short command: lecg

   \param DSL_LastExceptionCodes_t* The parameter points to a
          \ref DSL_LastExceptionCodes_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_LastExceptionCodes_t lastExceptionCodes;
      DSL_int_t ret = 0;

      memset(&lastExceptionCodes, 0x00, sizeof(DSL_LastExceptionCodes_t));
      ret = ioctl(fd, DSL_FIO_LAST_EXCEPTION_CODES_GET, &lastExceptionCodes);
      // Read and/or process exception codes here...
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_LAST_EXCEPTION_CODES_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 49, DSL_LastExceptionCodes_t)
#endif /* INCLUDE_DEVICE_EXCEPTION_CODES*/

#ifndef DSL_DEBUG_DISABLE
#ifdef INCLUDE_DSL_CPE_API_DANUBE
/**
   This function sets common debug features options

   CLI
   - long command: DebugFeatureConfigSet
   - short command: dfcs

   \param DSL_DBG_DebugFeatureConfig_t*
   The parameter points to a \ref DSL_DBG_DebugFeatureConfig_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - ADSL only platforms (XWAY(TM) ARX100, XWAY(TM) ARX300)

   \code
      DSL_DBG_DebugFeatureConfig_t dbgDebugFeatureConfig;
      DSL_int_t ret = 0;

      memset(&dbgDebugFeatureConfig, 0x00, sizeof(DSL_DBG_DebugFeatureConfig_t));
      Set dbgDebugFeatureControl structure data here
      ...
      ret = ioctl(fd, DSL_FIO_DBG_DEBUG_FEATURE_CONFIG_SET, &dbgDebugFeatureConfig);

   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_DBG_DEBUG_FEATURE_CONFIG_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 50, DSL_DBG_DebugFeatureConfig_t)
#endif /* INCLUDE_DSL_CPE_API_DANUBE*/
#endif /* DSL_DEBUG_DISABLE*/

#ifndef DSL_DEBUG_DISABLE
#ifdef INCLUDE_DSL_CPE_API_DANUBE
/**
   This function gets common debug features options

   CLI
   - long command: DebugFeatureConfigGet
   - short command: dfcg

   \param DSL_DBG_DebugFeatureConfig_t*
   The parameter points to a \ref DSL_DBG_DebugFeatureConfig_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - ADSL only platforms (XWAY(TM) ARX100, XWAY(TM) ARX300)

   \code
      DSL_DBG_DebugFeatureConfig_t dbgDebugFeatureConfig;
      DSL_int_t ret = 0;

      memset(&dbgDebugFeatureConfig, 0x00, sizeof(DSL_DBG_DebugFeatureConfig_t));
      ret = ioctl(fd, DSL_FIO_DBG_DEBUG_FEATURE_CONFIG_SET, &dbgDebugFeatureConfig);

   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_DBG_DEBUG_FEATURE_CONFIG_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 51, DSL_DBG_DebugFeatureConfig_t)
#endif /* INCLUDE_DSL_CPE_API_DANUBE*/
#endif /* DSL_DEBUG_DISABLE*/

/**
   This function returns a list of pilot tones that are used from the FW during
   showtime.

   CLI
   - long command: PilotTonesStatusGet
   - short command: ptsg

   \param DSL_PilotTonesStatus_t*
   The parameter points to a \ref DSL_PilotTonesStatus_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PilotTonesStatus_t pilotTonesStatus;
      DSL_int_t ret = 0;

      memset(&pilotTonesStatus, 0x00, sizeof(DSL_PilotTonesStatus_t));
      ret = ioctl(fd, DSL_FIO_PILOT_TONES_STATUS_GET, &pilotTonesStatus);
      // Read pilot tones status here...
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_PILOT_TONES_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 52, DSL_PilotTonesStatus_t)

/**
   This function configures the criterias that should be used for restarting
   the link.

   CLI
   - long command: RebootCriteriaConfigSet
   - short command: rccs

   \param DSL_RebootCriteriaConfig_t*
   The parameter points to a \ref DSL_RebootCriteriaConfig_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   - Supported by all platforms.
   - Please note that a change of this configuration setting(s) will be only
     effective after restarting the autoboot handling using ioctl
     \ref DSL_FIO_AUTOBOOT_CONTROL_SET with command \ref DSL_AUTOBOOT_CTRL_RESTART

   \code
      DSL_RebootCriteriaConfig_t rebootCriteriaConfig;
      DSL_int_t ret = 0;

      memset(&rebootCriteriaConfig, 0x00, sizeof(DSL_RebootCriteriaConfig_t));
      // Set configuration settings here before calling the ioctl...
      ret = ioctl(fd, DSL_FIO_REBOOT_CRITERIA_CONFIG_SET, &rebootCriteriaConfig);
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_REBOOT_CRITERIA_CONFIG_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 53, DSL_RebootCriteriaConfig_t)

/**
   This function return the criterias that are currently used for restarting
   the link.

   CLI
   - long command: RebootCriteriaConfigGet
   - short command: rccg

   \param DSL_RebootCriteriaConfig_t*
   The parameter points to a \ref DSL_RebootCriteriaConfig_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_RebootCriteriaConfig_t rebootCriteriaConfig;
      DSL_int_t ret = 0;

      memset(&rebootCriteriaConfig, 0x00, sizeof(DSL_RebootCriteriaConfig_t));
      // Set configuration settings here before calling the ioctl...
      ret = ioctl(fd, DSL_FIO_REBOOT_CRITERIA_CONFIG_GET, &rebootCriteriaConfig);
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_REBOOT_CRITERIA_CONFIG_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 54, DSL_RebootCriteriaConfig_t)

#ifdef INCLUDE_DSL_CPE_PM_RETX_COUNTERS
#ifdef INCLUDE_DSL_CPE_PM_RETX_THRESHOLDS
/**
   This function returns statistics counter that are related to retransmission
   functionality.

   CLI
   - long command: ReTxStatisticsGet
   - short command: rtsg

   \param DSL_ReTxStatistics_t*
   The parameter points to a \ref DSL_ReTxStatistics_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_ReTxStatistics_t ReTxStatistics;
      DSL_int_t ret = 0;

      memset(&ReTxStatistics, 0x00, sizeof(DSL_ReTxStatistics_t));
      ret = ioctl(fd, DSL_FIO_RETX_STATISTICS_GET, &ReTxStatistics);
      // Read statistics here...
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_RETX_STATISTICS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 58, DSL_ReTxStatistics_t)
#endif /* INCLUDE_DSL_CPE_PM_RETX_THRESHOLDS */
#endif /* INCLUDE_DSL_CPE_PM_RETX_COUNTERS */

#ifdef INCLUDE_DSL_FILTER_DETECTION
/**
   This function returns Filter Detection data.

   \param DSL_FilterDetection_t*
   The parameter points to a \ref DSL_FilterDetection_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_FilterDetection_t filterDetection;
      DSL_int_t ret = 0;

      memset(&filterDetection, 0x00, sizeof(DSL_FilterDetection_t));
      ret = ioctl(fd, DSL_FIO_FILTER_DETECTION_DATA_GET, &filterDetection);
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_FILTER_DETECTION_DATA_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 59, \
   DSL_FilterDetection_t)
#endif /* INCLUDE_DSL_FILTER_DETECTION */

/**
   This function returns AFE hybrid selection data.

   \param DSL_HybridSelection_t*
   The parameter points to a \ref DSL_HybridSelection_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_HybridSelection_t hybridSelection;
      DSL_int_t ret = 0;

      memset(&hybridSelection, 0x00, sizeof(DSL_HybridSelection_t));
      ret = ioctl(fd, DSL_FIO_HYBRID_SELECTION_DATA_GET, &hybridSelection);
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_HYBRID_SELECTION_DATA_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 60, \
   DSL_HybridSelection_t)

/**
   This function returns counters that are related to OLR statistics.
   It is direction specific.

   CLI
   - long command: OlrStatisticsGet
   - short command: osg

   \param DSL_OlrStatistics_t*
      The parameter points to a \ref DSL_OlrStatistics_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_OlrStatistics_t OlrStatistics;
      DSL_int_t ret = 0;

      memset(&OlrStatistics, 0x00, sizeof(DSL_OlrStatistics_t));
      ret = ioctl(fd, DSL_FIO_OLR_STATISTICS_GET, &OlrStatistics);
      // Read statistics here...
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_OLR_STATISTICS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 61, DSL_OlrStatistics_t)

/**
   This function returns the Signal to Noise Ratio (SNR) per subcarrier group
   measured during diagnostic or showtime (including Virtual Noise).

   CLI
   - long command: DeltSNRGet
   - short command: dsnrg

   \param DSL_G997_DeltSnr_t*
      The parameter points to a \ref DSL_G997_DeltSnr_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms (excluding UPSTREAM for ADSL-CPE).

   \code
      DSL_G997_DeltSnr_t deltSnrGet;
      DSL_int_t ret = 0;

      memset(&deltSnrGet, 0x00, sizeof(DSL_G997_DeltSnr_t));
      ret = ioctl(fd, DSL_FIO_DELT_SNR_GET, &deltSnrGet);
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_DELT_SNR_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 62, DSL_G997_DeltSnr_t)

/**
   This ioctl makes it possible to get the current firmware download.

   CLI
   - long command: FirmwareDownloadStatusGet
   - short command: fdsg

   \param DSL_FirmwareDownloadStatus_t*
      The parameter points to a \ref DSL_FirmwareDownloadStatus_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_FirmwareDownloadStatus_t status;
      DSL_int_t ret = 0;

      memset(&status, 0x00, sizeof(DSL_FirmwareDownloadStatus_t));
      ret = ioctl(fd, DSL_FIO_FIRMWARE_DOWNLOAD_STATUS_GET, &status);
   \endcode

   \ingroup DRV_DSL_CPE_INIT */
#define DSL_FIO_FIRMWARE_DOWNLOAD_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 63, DSL_FirmwareDownloadStatus_t)

/**
   This function configures the VDSL profile selection.

   CLI
   - long command: VdslProfileConfigSet
   - short command: vpcs

   \note
   Please note that a change of this configuration setting will be only
   effective after restarting the autoboot handling using ioctl
   \ref DSL_FIO_AUTOBOOT_CONTROL_SET with command \ref DSL_AUTOBOOT_CTRL_RESTART

   \param DSL_VdslProfileConfig_t*
   The parameter points to a \ref DSL_VdslProfileConfig_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - XWAY(TM) VRX200: xDSL-CPE
   - XWAY(TM) VRX300: xDSL-CPE

   \code
      DSL_VdslProfileConfig_t vdslProfileConfig;
      DSL_int_t ret = 0;

      memset(&vdslProfileConfig, 0x00, sizeof(DSL_VdslProfileConfig_t));
      // Set configuration settings here before calling the ioctl...
      ret = ioctl(fd, DSL_FIO_VDSL_PROFILE_CONFIG_SET, &vdslProfileConfig);
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_VDSL_PROFILE_CONFIG_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 64, DSL_VdslProfileConfig_t)

/**
   This function returns the VDSL profile selection that is currently used.

   CLI
   - long command: VdslProfileConfigGet
   - short command: vpcg

   \param DSL_VdslProfileConfig_t*
   The parameter points to a \ref DSL_VdslProfileConfig_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - XWAY(TM) VRX200: xDSL-CPE
   - XWAY(TM) VRX300: xDSL-CPE

   \code
      DSL_VdslProfileConfig_t vdslProfileConfig;
      DSL_int_t ret = 0;

      memset(&vdslProfileConfig, 0x00, sizeof(DSL_VdslProfileConfig_t));
      ret = ioctl(fd, DSL_FIO_VDSL_PROFILE_CONFIG_GET, &vdslProfileConfig); //
      Process returned values after ioctl call here ...
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_VDSL_PROFILE_CONFIG_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, 65, DSL_VdslProfileConfig_t)

#ifdef INCLUDE_DSL_BONDING
/* ************************************************************************** */
/* * Ioctl interface definitions for Bonding                                * */
/* ************************************************************************** */
/**
   This ioctl initializes the bonding hardware and should be called at start up.

   CLI
   - n/a
   - Note: This ioctl is called within context of starting up the control
           application in case of using '-i' option, also refer to UMPR
           "Command line arguments for the DSL CPE Control Application"

   \param DSL_BND_HwInit_t*
      The parameter points to a \ref DSL_BND_HwInit_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - XWAY(TM) VRX200: xDSL-CPE

   \note This ioctl is reserved for future usage on XWAY(TM) VRX200. This means
         there is currently no functionality included.

   \code
      DSL_BND_HwInit_t BND_HwInit;
      DSL_int_t ret = 0;

      memset(&BND_HwInit, 0x00, sizeof(DSL_BND_HwInit_t));
      BND_HwInit = ;
      ret = ioctl(fd, DSL_FIO_BND_HW_INIT, &BND_HwInit);
   \endcode

   \ingroup DRV_DSL_CPE_BND */
#define DSL_FIO_BND_HW_INIT \
   _IOWR(DSL_IOC_MAGIC_CPE_API_BND, 0, DSL_BND_HwInit_t)

/**
   This ioctl configures the bonding handshake configuration.

   CLI
   - long command: BND_ConfigSet
   - short command: bndcs

   \param DSL_BND_ConfigSet_t*
      The parameter points to a \ref DSL_BND_ConfigSet_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - XWAY(TM) VRX200: xDSL-CPE
   - XWAY(TM) VRX300: xDSL-CPE
   \remarks
   - Please note that a change of this configuration setting(s) will be only
     effective after restarting the autoboot handling using ioctl
     \ref DSL_FIO_AUTOBOOT_CONTROL_SET with command \ref DSL_AUTOBOOT_CTRL_RESTART

   \code
      DSL_BND_ConfigSet_t BND_ConfigSet;
      DSL_int_t ret = 0;

      memset(&BND_ConfigSet, 0x00, sizeof(DSL_BND_ConfigSet_t));
      BND_ConfigSet = ;
      ret = ioctl(fd, DSL_FIO_BND_CONFIG_SET, &BND_ConfigSet);
   \endcode

   \ingroup DRV_DSL_CPE_BND */
#define DSL_FIO_BND_CONFIG_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_BND, 2, DSL_BND_ConfigSet_t)

/**
   This ioctl retrieves the the bonding handshake configuration.

   CLI
   - long command: BND_ConfigGet
   - short command: bndcg

   \param DSL_BND_ConfigGet_t*
      The parameter points to a \ref DSL_BND_ConfigGet_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - XWAY(TM) VRX200: xDSL-CPE
   - XWAY(TM) VRX300: xDSL-CPE

   \code
      DSL_BND_ConfigGet_t BND_ConfigGet;
      DSL_int_t ret = 0;

      memset(&BND_ConfigGet, 0x00, sizeof(DSL_BND_ConfigGet_t));
      BND_ConfigGet = ;
      ret = ioctl(fd, DSL_FIO_BND_CONFIG_GET, &BND_ConfigGet);
   \endcode

   \ingroup DRV_DSL_CPE_BND */
#define DSL_FIO_BND_CONFIG_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_BND, 3, DSL_BND_ConfigGet_t)

/**
   This ioctl retrieves the bonding handshake status.

   CLI
   - long command: BND_HsStatusGet
   - short command: bndhsg

   \param DSL_BND_HsStatusGet_t*
      The parameter points to a \ref DSL_BND_HsStatusGet_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - XWAY(TM) VRX200: xDSL-CPE
   - XWAY(TM) VRX300: xDSL-CPE

   \code
      DSL_BND_HsStatusGet_t BND_HsStatusGet;
      DSL_int_t ret = 0;

      memset(&BND_HsStatusGet, 0x00, sizeof(DSL_BND_HsStatusGet_t));
      BND_HsStatusGet = ;
      ret = ioctl(fd, DSL_FIO_BND_HS_STATUS_GET, &BND_HsStatusGet);
   \endcode

   \ingroup DRV_DSL_CPE_BND */
#define DSL_FIO_BND_HS_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_BND, 4, DSL_BND_HsStatusGet_t)

/**
   This ioctl configures the remote_discovery_register (maintained by the
   network processor and common to all bonded ports) and notifies the
   DSP CPE API that the discovery exchange may continue.

   CLI
   - n/a
   - Note: This ioctl is called automatically within context of bonding
           related autoboot handling.
           Please note that the handling that is related to this ioctl is
           timing critical (the complete SW related handshake part has to be
           done within less than 500ms). Therefore do not use it outside the
           bonding state machine which is implemented within DSL CPE Control
           Application.

   \param DSL_BND_HsContinue_t*
      The parameter points to a \ref DSL_BND_HsContinue_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - XWAY(TM) VRX200: xDSL-CPE
   - XWAY(TM) VRX300: xDSL-CPE

   \code
      DSL_BND_HsContinue_t BND_HsContinue;
      DSL_int_t ret = 0;

      memset(&BND_HsContinue, 0x00, sizeof(DSL_BND_HsContinue_t));
      BND_HsContinue = ;
      ret = ioctl(fd, DSL_FIO_BND_HS_CONTINUE, &BND_HsContinue);
   \endcode

   \ingroup DRV_DSL_CPE_BND */
#define DSL_FIO_BND_HS_CONTINUE \
   _IOWR(DSL_IOC_MAGIC_CPE_API_BND, 5, DSL_BND_HsContinue_t)

/**
   This ioctl retrieves the bonding control register, fragment counts,
   and packet counts.

   CLI
   - long command: BND_ETH_DBG_CountersGet
   - short command: bndethdbgcg

   \note This ioctl includes non standardized counters.

   \param DSL_BND_EthDbgCounters_t*
      The parameter points to a \ref DSL_BND_EthDbgCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Not supported so far

   \code
      DSL_BND_EthDbgCounters_t BND_EthDbgCounters;
      DSL_int_t ret = 0;

      memset(&BND_EthDbgCounters, 0x00, sizeof(DSL_BND_EthDbgCounters_t));
      BND_EthDbgCounters = ;
      ret = ioctl(fd, DSL_FIO_BND_ETH_DBG_COUNTERS_GET, &BND_EthDbgCounters);
   \endcode

   \ingroup DRV_DSL_CPE_BND */
#define DSL_FIO_BND_ETH_DBG_COUNTERS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_BND, 7, DSL_BND_EthDbgCounters_t)

/**
   This ioctl retrieves the bonding error counters

   Please note the following important issues
   - In general there is the possibility within a VDSL capable system that
     supports bonding to access this ioctl for each device (from application
     layer you have multiple file descriptors, at least one for each of the two
     devices that are currently supported).
   - Independently from the device which is used to read these counters the API
     always returns the accumulated counter values for both PHY's (devices).
   - The counters are defined as reset on read!
     Therefore it is highly recommended to use only one of both available
     devices to read these counters.
   - The counters are defined to stay at saturation value instead of wrap
     around.

   CLI
   - long command: BND_ETH_CountersGet
   - short command: bndethcg

   \note This ioctl includes the standardized counters.

   \param DSL_BND_EthCounters_t*
      The parameter points to a \ref DSL_BND_EthCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Not supported so far

   \code
      DSL_BND_EthCounters_t BND_EthCountersGet;
      DSL_int_t ret = 0;

      memset(&BND_EthCountersGet, 0x00, sizeof(DSL_BND_EthCounters_t));
      BND_EthCountersGet = ;
      ret = ioctl(fd, DSL_FIO_BND_ETH_COUNTERS_GET, &BND_EthCountersGet);
   \endcode

   \ingroup DRV_DSL_CPE_BND */
#define DSL_FIO_BND_ETH_COUNTERS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_BND, 8, DSL_BND_EthCounters_t)

/**
   This ioctl specifies line Port Mode which will be used after triggering
   a hard reset.

   CLI
   - long command: BND_PortModeSyncSet
   - short command: bndpmss

   \note This ioctl should be called once before running a hard reset to
         force line into the specified Port Mode.

   \param DSL_BND_PortModeSync_t*
      The parameter points to a \ref DSL_BND_PortModeSync_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - XWAY(TM) VRX200: xDSL-CPE

   \code
      DSL_BND_PortModeSync_t BND_PortModeSync;
      DSL_int_t ret = 0;

      memset(&BND_PortModeSync, 0x00, sizeof(DSL_BND_PortModeSync_t));
      BND_PortModeSync. = ;
      ret = ioctl(fd, DSL_FIO_BND_PORT_MODE_SYNC_SET, &BND_PortModeSync);
   \endcode

   \ingroup DRV_DSL_CPE_BND */
#define DSL_FIO_BND_PORT_MODE_SYNC_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_BND, 9, DSL_BND_PortModeSync_t)

#endif /* INCLUDE_DSL_BONDING */

/* ************************************************************************** */
/* * Ioctl interface definitions for G.997.1                                * */
/* ************************************************************************** */

/**
   This function configures special line activation settings.

   CLI
   - long command: G997_LineActivateConfigSet
   - short command: g997lacs

   \attention All configurations that are done within context of this function
              are persistent!
              This means in case of activating the diagnostic mode (DELT) for
              example this configuration will be used for consecutive line
              activations by the autoboot handling until this configuration
              option is disabled again.

   \param DSL_G997_LineActivate_t*
      The parameter points to a \ref DSL_G997_LineActivate_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   - Supported by all platforms.
   - Please note that a change of this configuration setting(s) will be only
     effective after restarting the autoboot handling using ioctl
     \ref DSL_FIO_AUTOBOOT_CONTROL_SET with command \ref DSL_AUTOBOOT_CTRL_RESTART

   \code
      DSL_G997_LineActivate_t lineActivate;
      DSL_int_t ret = 0;

      memset(&lineActivate, 0x00, sizeof(DSL_G997_LineActivate_t));
      ret = ioctl(fd, DSL_FIO_G997_LINE_ACTIVATE_CONFIG_SET, &lineActivate);
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_LINE_ACTIVATE_CONFIG_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 0, DSL_G997_LineActivate_t)

/**
   This function returns the current configuration for special line activation
   settings.

   CLI
   - long command: G997_LineActivateConfigGet
   - short command: g997lacg

   \param DSL_G997_LineActivate_t*
      The parameter points to a \ref DSL_G997_LineActivate_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_G997_LineActivate_t lineActivate;
      DSL_int_t ret = 0;

      memset(&lineActivate, 0x00, sizeof(DSL_G997_LineActivate_t));
      ret = ioctl(fd, DSL_FIO_G997_LINE_ACTIVATE_CONFIG_GET, &lineActivate);
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_LINE_ACTIVATE_CONFIG_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 1, DSL_G997_LineActivate_t)

/**
   This function allows to change the configuration of the xTU transmission
   system modes that are supported by the CPE.
   It is usual to set any combination of bits that are allowed by the current
   hardware/firmware/software combination.

   CLI
   - long command: G997_XTUSystemEnablingConfigSet
   - short command: g997xtusecs

   \param DSL_G997_XTUSystemEnabling_t*
      The parameter points to a \ref DSL_G997_XTUSystemEnabling_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   - Supported by all platforms.
   - Please note that a change of this configuration setting(s) will be only
     effective after restarting the autoboot handling using ioctl
     \ref DSL_FIO_AUTOBOOT_CONTROL_SET with command \ref DSL_AUTOBOOT_CTRL_RESTART

   \code
      DSL_G997_XTUSystemEnabling_t xtuSystemEnabling;
      DSL_int_t ret = 0;

      memset(&xtuSystemEnabling, 0x00, sizeof(DSL_G997_XTUSystemEnabling_t));
      ret = ioctl(fd, DSL_FIO_G997_XTU_SYSTEM_ENABLING_CONFIG_SET, &xtuSystemEnabling);
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_XTU_SYSTEM_ENABLING_CONFIG_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 2, DSL_G997_XTUSystemEnabling_t)

/**
   This function returns the configuration of the xTU transmission system modes
   that are currently activated to be used by the CPE.

   CLI
   - long command: G997_XTUSystemEnablingConfigGet
   - short command: g997xtusecg

   \param DSL_G997_XTUSystemEnabling_t*
      The parameter points to a \ref DSL_G997_XTUSystemEnabling_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_G997_XTUSystemEnabling_t xtuSystemEnabling;
      DSL_int_t ret = 0;

      memset(&xtuSystemEnabling, 0x00, sizeof(DSL_G997_XTUSystemEnabling_t));
      ret = ioctl(fd, DSL_FIO_G997_XTU_SYSTEM_ENABLING_CONFIG_GET, &xtuSystemEnabling);
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_XTU_SYSTEM_ENABLING_CONFIG_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 3, DSL_G997_XTUSystemEnabling_t)

/**
   This function returns the status of the currently used xTU transmission
   system mode. In opposite to the functionality provided by
   \ref DSL_FIO_G997_XTU_SYSTEM_ENABLING_CONFIG_SET only ONE bit will be set
   at a time.

   CLI
   - long command: G997_XTUSystemEnablingStatusGet
   - short command: g997xtusesg

   \param DSL_G997_XTUSystemEnabling_t*
      The parameter points to a \ref DSL_G997_XTUSystemEnabling_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Please note that the state of this status values are only updated if showtime
   is reached otherwise they might reflect inconsistent states.
   To check if the line is in showtime please use ioctl
   \ref DSL_FIO_LINE_STATE_GET before using this functionality if you are not
   sure about the line state.
   Supported by all platforms.

   \code
      DSL_G997_XTUSystemEnabling_t xtuSystemEnabling;
      DSL_int_t ret = 0;

      memset(&xtuSystemEnabling, 0x00, sizeof(DSL_G997_XTUSystemEnabling_t));
      ret = ioctl(fd, DSL_FIO_G997_XTU_SYSTEM_ENABLING_STATUS_GET, &xtuSystemEnabling);
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_XTU_SYSTEM_ENABLING_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 4, DSL_G997_XTUSystemEnabling_t)

/**
   This function allows to change the configuration of channel data rate
   thresholds.

   CLI
   - long command: G997_ChannelDataRateThresholdConfigSet
   - short command: g997cdrtcs

   \param DSL_G997_ChannelDataRateThreshold_t*
      The parameter points to a \ref DSL_G997_ChannelDataRateThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_G997_ChannelDataRateThreshold_t chDataRateThres;
      DSL_int_t ret = 0;

      memset(&chDataRateThres, 0x00, sizeof(DSL_G997_ChannelDataRateThreshold_t));
      ret = ioctl(fd, DSL_FIO_G997_CHANNEL_DATA_RATE_THRESHOLD_CONFIG_SET, &chDataRateThres);
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_CHANNEL_DATA_RATE_THRESHOLD_CONFIG_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 5, DSL_G997_ChannelDataRateThreshold_t)

/**
   This function returns the current configuration of channel data rate
   thresholds.

   CLI
   - long command: G997_ChannelDataRateThresholdConfigGet
   - short command: g997cdrtcg

   \param DSL_G997_ChannelDataRateThreshold_t*
      The parameter points to a \ref DSL_G997_ChannelDataRateThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_G997_ChannelDataRateThreshold_t chDataRateThres;
      DSL_int_t ret = 0;

      memset(&chDataRateThres, 0x00, sizeof(DSL_G997_ChannelDataRateThreshold_t));
      ret = ioctl(fd, DSL_FIO_G997_CHANNEL_DATA_RATE_THRESHOLD_CONFIG_GET, &chDataRateThres);
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_CHANNEL_DATA_RATE_THRESHOLD_CONFIG_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 6, DSL_G997_ChannelDataRateThreshold_t)

/**
   This function returns the current status for the line transmission status.

   CLI
   - long command: G997_LineTransmissionStatusGet
   - short command: g997ltsg

   \param DSL_G997_LineTransmissionStatus_t*
      The parameter points to a \ref DSL_G997_LineTransmissionStatus_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_G997_LineTransmissionStatus_t lineTransStatus;
      DSL_int_t ret = 0;

      memset(&lineTransStatus, 0x00, sizeof(DSL_G997_LineTransmissionStatus_t));
      ret = ioctl(fd, DSL_FIO_G997_LINE_TRANSMISSION_STATUS_GET, &lineTransStatus);
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_LINE_TRANSMISSION_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 7, DSL_G997_LineTransmissionStatus_t)

/**
   This function returns the current status for the line initialization status.

   CLI
   - long command: G997_LineInitStatusGet
   - short command: g997lisg

   \param DSL_G997_LineInitStatus_t*
      The parameter points to a \ref DSL_G997_LineInitStatus_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_G997_LineInitStatus_t lineInitStatus;
      DSL_int_t ret = 0;

      memset(&lineTransStatusGet, 0x00, sizeof(DSL_G997_LineInitStatus_t));
      ret = ioctl(fd, DSL_FIO_G997_LINE_INIT_STATUS_GET, &lineInitStatus);
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_LINE_INIT_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 8, DSL_G997_LineInitStatus_t)

/**
   This function returns the current status for the line during showtime or
   diagnostic.

   CLI
   - long command: G997_LineStatusGet
   - short command: g997lsg

   \param DSL_G997_LineStatus_t*
      The parameter points to a \ref DSL_G997_LineStatus_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_G997_LineStatus_t lineStatus;
      DSL_int_t ret = 0;

      memset(&lineStatus, 0x00, sizeof(DSL_G997_LineStatus_t));
      ret = ioctl(fd, DSL_FIO_G997_LINE_STATUS_GET, &lineStatus);
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_LINE_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 9, DSL_G997_LineStatus_t)

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   This function returns the current per band status for the line.

   CLI
   - long command: G997_LineStatusPerBandGet
   - short command: g997lspbg

   \param DSL_G997_LineStatusPerBand_t*
      The parameter points to a \ref DSL_G997_LineStatusPerBand_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - XWAY(TM) VRX200: xDSL-CPE
   - XWAY(TM) VRX300: xDSL-CPE

   \code
      DSL_G997_LineStatusPerBand_t lineStatusPerBand;
      DSL_int_t ret = 0;

      memset(&lineStatusPerBand, 0x00, sizeof(DSL_G997_LineStatusPerBand_t));
      ret = ioctl(fd, DSL_FIO_G997_LINE_STATUS_PER_BAND_GET, &lineStatusPerBand);
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_LINE_STATUS_PER_BAND_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 10, DSL_G997_LineStatusPerBand_t)
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */

/**
   This function returns the current status for the (bearer channel).

   CLI
   - long command: G997_ChannelStatusGet
   - short command: g997csg

   \param DSL_G997_ChannelStatus_t*
      The parameter points to a \ref DSL_G997_ChannelStatus_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Please note that the state of this status values are only updated if showtime
   is reached otherwise they might reflect inconsistent states.
   To check if the line is in showtime please use ioctl
   \ref DSL_FIO_LINE_STATE_GET before using this functionality if you are not
   sure about the line state.
   Supported by all platforms.

   \code
      DSL_G997_ChannelStatus_t channelStatus;
      DSL_int_t ret = 0;

      memset(&channelStatus, 0x00, sizeof(DSL_G997_ChannelStatus_t));
      ret = ioctl(fd, DSL_FIO_G997_CHANNEL_STATUS_GET, &channelStatus);
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_CHANNEL_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 11, DSL_G997_ChannelStatus_t)

/**
   This function triggers a forced power management state on the line.

   CLI
   - long command: G997_PowerManagementStateForcedTrigger
   - short command: g997pmsft

   \param DSL_G997_PowerManagementStateForcedTrigger_t*
      The parameter points to a \ref DSL_G997_PowerManagementStateForcedTrigger_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_G997_PowerManagementStateForcedTrigger_t pwrMngStateForcedTrigger;
      DSL_int_t ret = 0;

      memset(&pwrMngStateForcedTrigger, 0x00, sizeof(DSL_G997_PowerManagementStateForcedTrigger_t));
      ret = ioctl(fd, DSL_FIO_G997_POWER_MANAGEMENT_STATE_FORCED_TRIGGER, &pwrMngStateForcedTrigger);
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_POWER_MANAGEMENT_STATE_FORCED_TRIGGER \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 12, DSL_G997_PowerManagementStateForcedTrigger_t)

/**
   This function returns the current power management status for the line.

   CLI
   - long command: G997_PowerManagementStatusGet
   - short command: g997pmsg

   \param DSL_G997_PowerManagementStatus_t*
      The parameter points to a \ref DSL_G997_PowerManagementStatus_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_G997_PowerManagementStatus_t pwrMngStatus;
      DSL_int_t ret = 0;

      memset(&pwrMngStatus, 0x00, sizeof(DSL_G997_PowerManagementStatus_t));
      ret = ioctl(fd, DSL_FIO_G997_POWER_MANAGEMENT_STATUS_GET, &pwrMngStatus);
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_POWER_MANAGEMENT_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 13, DSL_G997_PowerManagementStatus_t)

/**
   This function returns the last successfully transmitted state of the line
   for the specified direction.

   CLI
   - long command: G997_LastStateTransmittedGet
   - short command: g997lstg

   \param DSL_G997_LastStateTransmitted_t*
      The parameter points to a \ref DSL_G997_LastStateTransmitted_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - ADSL only platforms (XWAY(TM) ARX100, XWAY(TM) ARX300)

   \code
      DSL_G997_LastStateTransmitted_t lastStateTransmitted;
      DSL_int_t ret = 0;

      memset(&lastStateTransmitted, 0x00, sizeof(DSL_G997_LastStateTransmitted_t));
      ret = ioctl(fd, DSL_FIO_G997_LAST_STATE_TRANSMITTED_GET, &lastStateTransmitted);
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_LAST_STATE_TRANSMITTED_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 14, DSL_G997_LastStateTransmitted_t)

/**
   This function returns the current bit allocation per subcarrier table of
   the line for the specified direction.

   CLI
   - long command: G997_BitAllocationNscGet
   - short command: g997bang

   \param DSL_G997_BitAllocationNsc_t*
      The parameter points to a \ref DSL_G997_BitAllocationNsc_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_G997_BitAllocationNsc_t bitAllocationNsc;
      DSL_int_t ret = 0;

      memset(&bitAllocationNsc, 0x00, sizeof(DSL_G997_BitAllocationNsc_t));
      ret = ioctl(fd, DSL_FIO_G997_BIT_ALLOCATION_NSC_GET, &bitAllocationNsc);
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_BIT_ALLOCATION_NSC_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 15, DSL_G997_BitAllocationNsc_t)

/**
   This function returns the current gain allocation per subcarrier table of
   the line for the specified direction.

   CLI
   - long command: G997_GainAllocationNscGet
   - short command: g997gang

   \param DSL_G997_GainAllocationNsc_t*
      The parameter points to a \ref DSL_G997_GainAllocationNsc_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_G997_GainAllocationNsc_t gainAllocationNsc;
      DSL_int_t ret = 0;

      memset(&gainAllocationNsc, 0x00, sizeof(DSL_G997_GainAllocationNsc_t));
      ret = ioctl(fd, DSL_FIO_G997_BIT_ALLOCATION_NSC_GET, &gainAllocationNsc);
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
/* Usage of _IO instead of _IOWR is forced here because of a limited data type
   size for the _IOWR macro*/
#define DSL_FIO_G997_GAIN_ALLOCATION_NSC_GET \
   _IO(DSL_IOC_MAGIC_CPE_API_G997, 16)
/* Usage of _IO instead of _IOWR is forced here because of a limited data type
   size for the _IOWR macro*/

/**
   This function returns the current SNR allocation per subcarrier table of
   the line for the specified direction (exluding Virtual Noise).

   CLI
   - long command: G997_SnrAllocationNscGet
   - short command: g997sang

   \param DSL_G997_SnrAllocationNsc_t*
      The parameter points to a \ref DSL_G997_SnrAllocationNsc_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_G997_SnrAllocationNsc_t snrAllocationNsc;
      DSL_int_t ret = 0;

      memset(&snrAllocationNsc, 0x00, sizeof(DSL_G997_SnrAllocationNsc_t));
      ret = ioctl(fd, DSL_FIO_G997_SNR_ALLOCATION_NSC_GET, &snrAllocationNsc);
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_SNR_ALLOCATION_NSC_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 17, DSL_G997_SnrAllocationNsc_t)

/**
   This function configures the mask for the autonomous line failure event
   signalling.

   CLI
   - long command: G997_AlarmMaskLineFailuresConfigSet
   - short command: g997amlfcs

   \param DSL_G997_LineFailures_t*
      The parameter points to a \ref DSL_G997_LineFailures_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_G997_LineFailures_t alarmMaskLineFailuresCfgSet;
      DSL_int_t ret = 0;

      memset(&alarmMaskLineFailuresCfgSet, 0x00, sizeof(DSL_G997_LineFailures_t));
      ret = ioctl(fd, DSL_FIO_G997_ALARM_MASK_LINE_FAILURES_CONFIG_SET, &alarmMaskLineFailuresCfgSet);
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_ALARM_MASK_LINE_FAILURES_CONFIG_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 18, DSL_G997_LineFailures_t)

/**
   This function returns the current configuration of the mask for the
   autonomous line failure event signalling.

   CLI
   - long command: G997_AlarmMaskLineFailuresConfigGet
   - short command: g997amlfcg

   \param DSL_G997_LineFailures_t*
      The parameter points to a \ref DSL_G997_LineFailures_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_G997_LineFailures_t alarmMaskLineFailuresCfgGet;
      DSL_int_t ret = 0;

      memset(&alarmMaskLineFailuresCfgGet, 0x00, sizeof(DSL_G997_LineFailures_t));
      ret = ioctl(fd, DSL_FIO_G997_ALARM_MASK_LINE_FAILURES_CONFIG_GET, &alarmMaskLineFailuresCfgGet);
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_ALARM_MASK_LINE_FAILURES_CONFIG_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 19, DSL_G997_LineFailures_t)

/**
   This function returns the current status of the line failures.

   CLI
   - long command: G997_LineFailuresStatusSet
   - short command: g997lfsg

   \param DSL_G997_LineFailures_t*
      The parameter points to a \ref DSL_G997_LineFailures_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_G997_LineFailures_t lineFailuresStatusGet;
      DSL_int_t ret = 0;

      memset(&lineFailuresStatusGet, 0x00, sizeof(DSL_G997_LineFailures_t));
      ret = ioctl(fd, DSL_FIO_G997_LINE_FAILURES_STATUS_GET, &lineFailuresStatusGet);
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_LINE_FAILURES_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 20, DSL_G997_LineFailures_t)


/**
   This function configures the mask for the autonomous data path failure event
   signalling.

   CLI
   - long command: G997_AlarmMaskDataPathFailuresConfigSet
   - short command: g997amdpfcs

   \param DSL_G997_DataPathFailures_t*
      The parameter points to a \ref DSL_G997_DataPathFailures_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_G997_DataPathFailures_t alarmMaskDataPathFailuresCfgSet;
      DSL_int_t ret = 0;

      memset(&alarmMaskDataPathFailuresCfgSet, 0x00, sizeof(DSL_G997_DataPathFailures_t));
      ret = ioctl(fd, DSL_FIO_G997_ALARM_MASK_DATA_PATH_FAILURES_CONFIG_SET,;
         &alarmMaskDataPathFailuresCfgSet)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_ALARM_MASK_DATA_PATH_FAILURES_CONFIG_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 21, DSL_G997_DataPathFailures_t)

/**
   This function returns the current configuration of the mask for the
   autonomous data path failure event signalling.

   CLI
   - long command: G997_AlarmMaskDataPathFailuresConfigGet
   - short command: g997amdpfcg

   \param DSL_G997_DataPathFailures_t*
      The parameter points to a \ref DSL_G997_DataPathFailures_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_G997_DataPathFailures_t alarmMaskDataPathFailuresCfgGet;
      DSL_int_t ret = 0;

      memset(&alarmMaskDataPathFailuresCfgGet, 0x00, sizeof(DSL_G997_DataPathFailures_t));
      ret = ioctl(fd, DSL_FIO_G997_ALARM_MASK_DATA_PATH_FAILURES_CONFIG_GET,;
         &alarmMaskDataPathFailuresCfgGet)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_ALARM_MASK_DATA_PATH_FAILURES_CONFIG_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 22, DSL_G997_DataPathFailures_t)

/**
   This function returns the current status of the data path failures.

   CLI
   - long command: G997_DataPathFailuresStatusGet
   - short command: g997dpfsg

   \param DSL_G997_DataPathFailures_t*
      The parameter points to a \ref DSL_G997_DataPathFailures_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_G997_DataPathFailures_t dataPathFailuresStatusGet;
      DSL_int_t ret = 0;

      memset(&dataPathFailuresStatusGet, 0x00, sizeof(DSL_G997_DataPathFailures_t));
      ret = ioctl(fd, DSL_FIO_G997_DATA_PATH_FAILURES_STATUS_GET, &dataPathFailuresStatusGet);
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_DATA_PATH_FAILURES_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 23, DSL_G997_DataPathFailures_t)

/**
   This function reads framing parameters according to ITU G997.1

   CLI
   - long command: G997_FramingParameterStatusGet
   - short command: g997fpsg

   \param DSL_G997_FramingParameterStatus_t*
      The parameter points to a \ref DSL_G997_FramingParameterStatus_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Please note that the state of this status values are only updated if showtime
   is reached otherwise they might reflect inconsistent states.
   To check if the line is in showtime please use ioctl
   \ref DSL_FIO_LINE_STATE_GET before using this functionality if you are not
   sure about the line state.
   Supported by all platforms.

   \code
      DSL_G997_FramingParameterStatus_t g997FramingParamStatusGet;
      DSL_int_t ret = 0;

      memset(&g997FramingParamStatusGet, 0x00, sizeof(DSL_G997_FramingParameterStatus_t));
      ret = ioctl(fd, DSL_FIO_G997_FRAMING_PARAMETER_STATUS_GET, &g997FramingParamStatusGet);
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_FRAMING_PARAMETER_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 24, DSL_G997_FramingParameterStatus_t)

/**
   This function returns the line specific inventory information for the
   NearEnd or FarEnd.

   CLI
   - long command: G997_LineInventoryGet
   - short command: g997lig

   \attention The line inventory information of the far end side will be
              requested via EOC/OHC handling. This is only possible in showtime
              state and may take several seconds.
              The callback event DSL_EVENT_S_FE_INVENTORY_AVAILABLE will be called
              after ALL far end inventory data has been received.

   \param DSL_G997_LineInventory_t*
      The parameter points to a \ref DSL_G997_LineInventory_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_G997_LineInventory_t lineInventoryGet;
      DSL_int_t ret = 0;

      memset(&lineInventoryGet, 0x00, sizeof(DSL_G997_LineInventory_t));
      ret = ioctl(fd, DSL_FIO_G997_LINE_INVENTORY_GET, &lineInventoryGet);
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_LINE_INVENTORY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 25, DSL_G997_LineInventory_t)

/**
   This function modifies the line specific inventory information for the
   NearEnd.

   CLI
   - long command: G997_LineInventorySet
   - short command: g997lis

   \param DSL_G997_LineInventoryNe_t*
      The parameter points to a \ref DSL_G997_LineInventoryNe_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_G997_LineInventoryNe_t lineInventorySet;
      DSL_int_t ret = 0;

      memset(&lineInventorySet, 0x00, sizeof(DSL_G997_LineInventoryNe_t));
      ret = ioctl(fd, DSL_FIO_G997_LINE_INVENTORY_SET, &lineInventorySet);
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_LINE_INVENTORY_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 26, DSL_G997_LineInventoryNe_t)

/**
   This function returns the Hlin scale value measured during diagnostic or
   showtime.

   CLI
   - long command: G997_DeltHLINScaleGet
   - short command: g997dhlinsg

   \param DSL_G997_DeltHlinScale_t*
      The parameter points to a \ref DSL_G997_DeltHlinScale_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_G997_DeltHlinScale_t deltHlinScaleGet;
      DSL_int_t ret = 0;

      memset(&deltHlinScaleGet, 0x00, sizeof(DSL_G997_DeltHlinScale_t));
      ret = ioctl(fd, DSL_FIO_G997_DELT_HLIN_SCALE_GET, &deltHlinScaleGet);
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_DELT_HLIN_SCALE_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 27, DSL_G997_DeltHlinScale_t)

/**
   This function returns the Hlin per subcarrier data measured during diagnostic
   or showtime.

   CLI
   - long command: G997_DeltHLINGet
   - short command: g997dhling

   \param DSL_G997_DeltHlin_t*
      The parameter points to a \ref DSL_G997_DeltHlin_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_G997_DeltHlin_t deltHlinGet;
      DSL_int_t ret = 0;

      memset(&deltHlinGet, 0x00, sizeof(DSL_G997_DeltHlin_t));
      ret = ioctl(fd, DSL_FIO_G997_DELT_HLIN_GET, &deltHlinGet);
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_DELT_HLIN_GET \
   _IO(DSL_IOC_MAGIC_CPE_API_G997, 28)
/* Usage of _IO instead of _IOWR is forced here because of a limited data type
   size for the _IOWR macro*/

/**
   This function returns the Hlog per subcarrier measured during diagnostic or
   training (in case of selecting showtime values
   nDeltDataType=DSL_DELT_DATA_SHOWTIME).

   CLI
   - long command: G997_DeltHLOGGet
   - short command: g997dhlogg

   \param DSL_G997_DeltHlog_t*
      The parameter points to a \ref DSL_G997_DeltHlog_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_G997_DeltHlog_t deltHlogGet;
      DSL_int_t ret = 0;

      memset(&deltHlogGet, 0x00, sizeof(DSL_G997_DeltHlog_t));
      ret = ioctl(fd, DSL_FIO_G997_DELT_HLOG_GET, &deltHlogGet);
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_DELT_HLOG_GET \
   _IO(DSL_IOC_MAGIC_CPE_API_G997, 29)
/* Usage of _IO instead of _IOWR is forced here because of a limited data type
   size for the _IOWR macro*/

/**
   This function returns the Signal to Noise Ratio (SNR) per subcarrier group
   measured during diagnostic or showtime (excluding Virtual Noise).

   CLI
   - long command: G997_DeltSNRGet
   - short command: g997dsnrg

   \param DSL_G997_DeltSnr_t*
      The parameter points to a \ref DSL_G997_DeltSnr_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms (excluding UPSTREAM for ADSL-CPE).

   \code
      DSL_G997_DeltSnr_t deltSnrGet;
      DSL_int_t ret = 0;

      memset(&deltSnrGet, 0x00, sizeof(DSL_G997_DeltSnr_t));
      ret = ioctl(fd, DSL_FIO_G997_DELT_SNR_GET, &deltSnrGet);
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_DELT_SNR_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 30, DSL_G997_DeltSnr_t)

/**
   This function returns the Quiet Line Noise (QLN) per subcarrier measured
   during diagnostic or training (in case of selecting showtime values
   nDeltDataType=DSL_DELT_DATA_SHOWTIME).

   CLI
   - long command: G997_DeltQLNGet
   - short command: g997dqlng

   \param DSL_G997_DeltQln_t*
      The parameter points to a \ref DSL_G997_DeltQln_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_G997_DeltQln_t deltQlnGet;
      DSL_int_t ret = 0;

      memset(&deltQlnGet, 0x00, sizeof(DSL_G997_DeltQln_t));
      ret = ioctl(fd, DSL_FIO_G997_DELT_QLN_GET, &deltQlnGet);
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_DELT_QLN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 31, DSL_G997_DeltQln_t)

/**
   This function releases memory allocated for DELT before.

   CLI
   - long command: G997_DeltFreeResources
   - short command: g997dfr
   - Note: This ioctl usage only makes sense in case of using dynamically
           allocated diagnostic data (DELT) memory. Also refer to the UMPR
           configure option: "--enable-dsl-delt-static=no".
           In that case and after a DELT measurement has taken place the
           API internal memory could be freed by using this function.

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      ret = ioctl(fd, DSL_FIO_G997_DELT_FREE_RESOURCES, 0);
      DSL_int_t ret = 0;
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_DELT_FREE_RESOURCES \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 32, DSL_G997_DeltFreeResources_t)

/**
   This function will copy the SNMP message to an internal buffer.
   This buffer will be transmitted asynchronous.

   CLI
   - long command: G997_SnmpMessageSend
   - short command: g997sms

   \param DSL_G997_Snmp_t*
      The parameter points to a \ref DSL_G997_Snmp_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - ADSL only platforms (XWAY(TM) ARX100, XWAY(TM) ARX300)

   \code
      DSL_G997_Snmp_t snmpData;
      DSL_int_t ret = 0;

      memset(&snmpData, 0x00, sizeof(DSL_G997_Snmp_t));
      // Set data within SNMP structure here...

      ret = ioctl(fd, DSL_FIO_G997_SNMP_MESSAGE_SEND, &snmpData);
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_SNMP_MESSAGE_SEND \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 33, DSL_G997_Snmp_t)

/**
   This function will retrieve the last received SNMP message.

   CLI
   - long command: G997_SnmpMessageReceive
   - short command: g997smr

   \param DSL_G997_Snmp_t*
      The parameter points to a \ref DSL_G997_Snmp_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - ADSL only platforms (XWAY(TM) ARX100, XWAY(TM) ARX300)

   \code
      DSL_G997_Snmp_t snmpData;
      DSL_int_t ret = 0;

      memset(&snmpData, 0x00, sizeof(DSL_G997_Snmp_t));

      ret = ioctl(fd, DSL_FIO_G997_SNMP_MESSAGE_RECEIVE, &snmpData);
      // Read data from SNMP structure here...
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_SNMP_MESSAGE_RECEIVE \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 34, DSL_G997_Snmp_t)

/**
   This function writes the Rate Adaptation (SRA) Configuration settings.

   CLI
   - long command: G997_RateAdaptationConfigSet
   - short command: g997racs

   \param DSL_G997_RateAdaptationConfig_t*
      The parameter points to a \ref DSL_G997_RateAdaptationConfig_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   - Supported by all platforms.
   - Please note that a change of this configuration setting(s) will be only
     effective after restarting the autoboot handling using ioctl
     \ref DSL_FIO_AUTOBOOT_CONTROL_SET with command \ref DSL_AUTOBOOT_CTRL_RESTART

   \code
      DSL_G997_RateAdaptationConfig_t sraCfgSet;
      DSL_int_t ret = 0;

      memset(&sraCfgSet, 0, sizeof(DSL_G997_RateAdaptationConfig_t));
      // switch off the automatic SRA handling
      sraCfgSet.data.RA_MODE = DSL_G997_RA_MODE_AT_INIT;
      ret = ioctl(fd, DSL_FIO_G997_RATE_ADAPTATION_CONFIG_SET, &sraCfgSet);
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_RATE_ADAPTATION_CONFIG_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 35, DSL_G997_RateAdaptationConfig_t)

/**
   This function reads the Rate Adaptation (SRA) Configuration settings.

   CLI
   - long command: G997_RateAdaptationConfigGet
   - short command: g997racg

   \param DSL_G997_RateAdaptationConfig_t*
      The parameter points to a \ref DSL_G997_RateAdaptationConfig_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_G997_RateAdaptationConfig_t sraCfgGet;
      DSL_int_t ret = 0;

      memset(&sraCfgGet, 0, sizeof(DSL_G997_RateAdaptationConfig_t));
      ret = ioctl(fd, DSL_FIO_G997_RATE_ADAPTATION_CONFIG_GET, &sraCfgGet);
      // sraCfgGet includes the current SRA configuration
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_RATE_ADAPTATION_CONFIG_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 36, DSL_G997_RateAdaptationConfig_t)

/**
   This function returns the actual active downstream/upstream RA mode
   (ACT-RA-MODEds/us) during showtime.

   CLI
   - long command: G997_RateAdaptationStatusGet
   - short command: g997rasg

   \param DSL_G997_RateAdaptationStatus_t*
   The parameter points to a \ref DSL_G997_RateAdaptationStatus_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - ADSL only platforms (XWAY(TM) ARX100, XWAY(TM) ARX300)

   \code
      DSL_G997_RateAdaptationStatus_t raStatGet;
      DSL_int_t ret = 0;

      memset(&raStatGet, 0x00, sizeof(DSL_G997_RateAdaptationStatus_t));
      ret = ioctl(fd, DSL_FIO_G997_RATE_ADAPTATION_STATUS_GET, &raStatGet);
      // Read pilot tones status here...
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_RATE_ADAPTATION_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 37, DSL_G997_RateAdaptationStatus_t)

/**
   This function returns Upstream Power Back-Off (UPBO) related status
   parameters for the alternative electrical length estimation method
   (AELEM).

   CLI
   - long command: G997_UsPowerBackOffStatusGet
   - short command: g997upbosg

   \param DSL_G997_UsPowerBackOffStatus_t*
   The parameter points to a \ref DSL_G997_UsPowerBackOffStatus_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - XWAY(TM) VRX200: xDSL-CPE
   - XWAY(TM) VRX300: xDSL-CPE

   \code
      DSL_G997_UsPowerBackOffStatus_t upboStatGet;
      DSL_int_t ret = 0;

      memset(&upboStatGet, 0x00, sizeof(DSL_G997_UsPowerBackOffStatus_t));
      ret = ioctl(fd, DSL_FIO_G997_US_POWER_BACK_OFF_STATUS_GET, &upboStatGet);
      // Read UPBO related AELE-Mode status here...
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_US_POWER_BACK_OFF_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 38, DSL_G997_UsPowerBackOffStatus_t)

/* ************************************************************************** */
/* * Ioctl interface definitions for PM                                     * */
/* ************************************************************************** */

/**
   This function returns DSL line 15-minute second counter history statistics.

   CLI
   - long command: PM_LineSecHistoryStats15MinGet
   - short command: pmlshs15mg

   \param DSL_PM_HistoryStatsDir_t*
      The parameter points to a \ref DSL_PM_HistoryStatsDir_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_HistoryStatsDir_t pmHistoryStats;
      DSL_int_t ret = 0;

      memset(&pmHistoryStats, 0x00, sizeof(DSL_PM_HistoryStatsDir_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_SEC_HISTORY_STATS_15MIN_GET, &pmHistoryStats);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_SEC_HISTORY_STATS_15MIN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 0, DSL_PM_HistoryStatsDir_t)

/**
   This function returns DSL line 1-day second counter history statistics.

   CLI
   - long command: PM_LineSecHistoryStats1DayGet
   - short command: pmlshs1dg

   \param DSL_PM_HistoryStatsDir_t*
      The parameter points to a \ref DSL_PM_HistoryStatsDir_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_HistoryStatsDir_t pmHistoryStats;
      DSL_int_t ret = 0;

      memset(&pmHistoryStats, 0x00, sizeof(DSL_PM_HistoryStatsDir_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_SEC_HISTORY_STATS_1DAY_GET, &pmHistoryStats);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_SEC_HISTORY_STATS_1DAY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 1, DSL_PM_HistoryStatsDir_t)

/**
   This function returns DSL line 15-minute second counters of specified history
   interval.

   CLI
   - long command: PM_LineSecCounters15MinGet
   - short command: pmlsc15mg

   \param DSL_PM_LineSecCounters_t*
      The parameter points to a \ref DSL_PM_LineSecCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_LineSecCounters_t pmLineSecCounter;
      DSL_int_t ret = 0;

      memset(&pmLineSecCounter, 0x00, sizeof(DSL_PM_LineSecCounters_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_SEC_COUNTERS_15MIN_GET, &pmLineSecCounter);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_SEC_COUNTERS_15MIN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 2, DSL_PM_LineSecCounters_t)

/**
   This function returns DSL line 1-day second counters of specified history
   interval.

   CLI
   - long command: PM_LineSecCounters1DayGet
   - short command: pmlsc1dg

   \param DSL_PM_LineSecCounters_t*
      The parameter points to a \ref DSL_PM_LineSecCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_LineSecCounters_t pmLineSecCounters;
      DSL_int_t ret = 0;

      memset(&pmLineSecCounters, 0x00, sizeof(DSL_PM_LineSecCounters_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_SEC_COUNTERS_1DAY_GET, &pmLineSecCounters);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_SEC_COUNTERS_1DAY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 3, DSL_PM_LineSecCounters_t)

/**
   This function returns DSL line total second counters accumulated since system
   startup.

   CLI
   - long command: PM_LineSecCountersTotalGet
   - short command: pmlsctg

   \param DSL_PM_LineSecCountersTotal_t*
      The parameter points to a \ref DSL_PM_LineSecCountersTotal_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_LineSecCountersTotal_t pmLineSecCountersTotal;
      DSL_int_t ret = 0;

      memset(&pmLineSecCountersTotal, 0x00, sizeof(DSL_PM_LineSecCountersTotal_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_SEC_COUNTERS_TOTAL_GET, &pmLineSecCountersTotal);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_SEC_COUNTERS_TOTAL_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 4, DSL_PM_LineSecCountersTotal_t)

/**
   This function sets DSL line 15-minute second counter thresholds.

   CLI
   - long command: PM_LineSecThresholds15MinSet
   - short command: pmlst15ms

   \param DSL_PM_LineSecThreshold_t*
      The parameter points to a \ref DSL_PM_LineSecThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_LineSecThreshold_t pmLineSecThresholds;
      DSL_int_t ret = 0;

      memset(&pmLineSecThresholds, 0x00, sizeof(DSL_PM_LineSecThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_SEC_THRESHOLDS_15MIN_SET, &pmLineSecThresholds);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_SEC_THRESHOLDS_15MIN_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 5, DSL_PM_LineSecThreshold_t)

/**
   This function sets DSL line 1-day second counter thresholds.

   CLI
   - long command: PM_LineSecThresholds1DaySet
   - short command: pmlst1ds

   \param DSL_PM_LineSecThreshold_t*
      The parameter points to a \ref DSL_PM_LineSecThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_LineSecThreshold_t pmLineSecThresholds;
      DSL_int_t ret = 0;

      memset(&pmLineSecThresholds, 0x00, sizeof(DSL_PM_LineSecThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_SEC_THRESHOLDS_1DAY_SET, &pmLineSecThresholds);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_SEC_THRESHOLDS_1DAY_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 6, DSL_PM_LineSecThreshold_t)

/**
   This function returns DSL line 15-minute second counter thresholds.

   CLI
   - long command: PM_LineSecThresholds15MinGet
   - short command: pmlst15mg

   \param DSL_PM_LineSecThreshold_t*
      The parameter points to a \ref DSL_PM_LineSecThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_LineSecThreshold_t pmLineSecThresholds;
      DSL_int_t ret = 0;

      memset(&pmLineSecThresholds, 0x00, sizeof(DSL_PM_LineSecThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_SEC_THRESHOLDS_15MIN_GET, &pmLineSecThresholds);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_SEC_THRESHOLDS_15MIN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 7, DSL_PM_LineSecThreshold_t)

/**
   This function returns DSL line 1-day second counter thresholds.

   CLI
   - long command: PM_LineSecThresholds1DayGet
   - short command: pmlst1dg

   \param DSL_PM_LineSecThreshold_t*
      The parameter points to a \ref DSL_PM_LineSecThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_LineSecThreshold_t pmLineSecThresholds;
      DSL_int_t ret = 0;

      memset(&pmLineSecThresholds, 0x00, sizeof(DSL_PM_LineSecThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_SEC_THRESHOLDS_1DAY_GET, &pmLineSecThresholds);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_SEC_THRESHOLDS_1DAY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 8, DSL_PM_LineSecThreshold_t)

/**
   This function returns DSL line 15-minute initialization counter history
   statistics.

   CLI
   - long command: PM_LineInitHistoryStats15MinGet
   - short command: pmlihs15mg

   \param DSL_PM_HistoryStats_t*
      The parameter points to a \ref DSL_PM_HistoryStats_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_HistoryStats_t pmHistoryStats;
      DSL_int_t ret = 0;

      memset(&pmHistoryStats, 0x00, sizeof(DSL_PM_HistoryStats_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_INIT_HISTORY_STATS_15MIN_GET, &pmHistoryStats);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_INIT_HISTORY_STATS_15MIN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 9, DSL_PM_HistoryStats_t)

/**
   This function returns DSL line 1 day initialization counter history
   statistics.

   CLI
   - long command: PM_LineInitHistoryStats1DayGet
   - short command: pmlihs1dg

   \param DSL_PM_HistoryStats_t*
      The parameter points to a \ref DSL_PM_HistoryStats_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_HistoryStats_t pmHistoryStats;
      DSL_int_t ret = 0;

      memset(&pmHistoryStats, 0x00, sizeof(DSL_PM_HistoryStats_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_INIT_HISTORY_STATS_1DAY_GET, &pmHistoryStats);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_INIT_HISTORY_STATS_1DAY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 10, DSL_PM_HistoryStats_t)

/**
   This function returns DSL line 15-minute initialization counters of specified
   history interval.

   CLI
   - long command: PM_LineInitCounters15MinGet
   - short command: pmlic15mg

   \param DSL_PM_LineInitCounters_t*
      The parameter points to a \ref DSL_PM_LineInitCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_LineInitCounters_t pmLineInitCounters;
      DSL_int_t ret = 0;

      memset(&pmLineInitCounters, 0x00, sizeof(DSL_PM_LineInitCounters_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_INIT_COUNTERS_15MIN_GET, &pmLineInitCounters);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_INIT_COUNTERS_15MIN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 11, DSL_PM_LineInitCounters_t)

/**
   This function returns DSL line 1-day initialization counters of specified
   history interval.

   CLI
   - long command: PM_LineInitCounters1DayGet
   - short command: pmlic1dg

   \param DSL_PM_LineInitCounters_t*
      The parameter points to a \ref DSL_PM_LineInitCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_LineInitCounters_t pmLineInitCounters;
      DSL_int_t ret = 0;

      memset(&pmLineInitCounters, 0x00, sizeof(DSL_PM_LineInitCounters_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_INIT_COUNTERS_1DAY_GET, &pmLineInitCounters);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_INIT_COUNTERS_1DAY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 12, DSL_PM_LineInitCounters_t)

/**
   This function returns DSL line total initialization counters accumulated
   since system startup.

   CLI
   - long command: PM_LineInitCountersTotalGet
   - short command: pmlictg

   \param DSL_PM_LineInitCountersTotal_t*
      The parameter points to a \ref DSL_PM_LineInitCountersTotal_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_LineInitCountersTotal_t pmLineInitCountersTotal;
      DSL_int_t ret = 0;

      memset(&pmLineInitCountersTotal, 0x00, sizeof(DSL_PM_LineInitCountersTotal_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_INIT_COUNTERS_TOTAL_GET, &pmLineInitCountersTotal);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_INIT_COUNTERS_TOTAL_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 13, DSL_PM_LineInitCountersTotal_t)

/**
   This function sets DSL line 15-minute initialization counter thresholds.

   CLI
   - long command: PM_LineInitThresholds15MinSet
   - short command: pmlit15ms

   \param DSL_PM_LineInitThreshold_t*
      The parameter points to a \ref DSL_PM_LineInitThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_LineInitThreshold_t pmLineInitThresholds;
      DSL_int_t ret = 0;

      memset(&pmLineInitThresholds, 0x00, sizeof(DSL_PM_LineInitThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_INIT_THRESHOLDS_15MIN_SET, &pmLineInitThresholds);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_INIT_THRESHOLDS_15MIN_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 14, DSL_PM_LineInitThreshold_t)

/**
   This function sets DSL line 1-day initialization counter thresholds.

   CLI
   - long command: PM_LineInitThresholds1DaySet
   - short command: pmlit1ds

   \param DSL_PM_LineInitThreshold_t*
      The parameter points to a \ref DSL_PM_LineInitThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_LineInitThreshold_t pmLineInitThresholds;
      DSL_int_t ret = 0;

      memset(&pmLineInitThresholds, 0x00, sizeof(DSL_PM_LineInitThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_INIT_THRESHOLDS_1DAY_SET, &pmLineInitThresholds);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_INIT_THRESHOLDS_1DAY_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 15, DSL_PM_LineInitThreshold_t)

/**
   This function returns DSL line 15-minute initialization counter thresholds.

   CLI
   - long command: PM_LineInitThresholds15MinGet
   - short command: pmlit15mg

   \param DSL_PM_LineInitThreshold_t*
      The parameter points to a \ref DSL_PM_LineInitThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_LineInitThreshold_t pmLineInitThresholds;
      DSL_int_t ret = 0;

      memset(&pmLineInitThresholds, 0x00, sizeof(DSL_PM_LineInitThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_INIT_THRESHOLDS_15MIN_GET, &pmLineInitThresholds);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_INIT_THRESHOLDS_15MIN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 16, DSL_PM_LineInitThreshold_t)

/**
   This function returns DSL line 1-day initialization counter thresholds.

   CLI
   - long command: PM_LineInitThresholds1DayGet
   - short command: pmlit1dg

   \param DSL_PM_LineInitThreshold_t*
      The parameter points to a \ref DSL_PM_LineInitThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_LineInitThreshold_t pmLineInitThresholds;
      DSL_int_t ret = 0;

      memset(&pmLineInitThresholds, 0x00, sizeof(DSL_PM_LineInitThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_INIT_THRESHOLDS_1DAY_GET, &pmLineInitThresholds);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_INIT_THRESHOLDS_1DAY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 17, DSL_PM_LineInitThreshold_t)

/**
   This function returns DSL channel 15-minute counter history statistics.

   CLI
   - long command: PM_ChannelHistoryStats15MinGet
   - short command: pmchs15mg

   \param DSL_PM_HistoryStatsChDir_t*
      The parameter points to a \ref DSL_PM_HistoryStatsChDir_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_HistoryStatsChDir_t pmHistoryStats;
      DSL_int_t ret = 0;

      memset(&pmHistoryStats, 0x00, sizeof(DSL_PM_HistoryStatsChDir_t));
      ret = ioctl(fd, DSL_FIO_PM_CHANNEL_HISTORY_STATS_15MIN_GET, &pmHistoryStats);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_CHANNEL_HISTORY_STATS_15MIN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 18, DSL_PM_HistoryStatsChDir_t)

/**
   This function returns DSL channel 1-day counter history statistics.

   CLI
   - long command: PM_ChannelHistoryStats1DayGet
   - short command: pmchs1dg

   \param DSL_PM_HistoryStatsChDir_t*
      The parameter points to a \ref DSL_PM_HistoryStatsChDir_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_HistoryStatsChDir_t pmHistoryStats;
      DSL_int_t ret = 0;

      memset(&pmHistoryStats, 0x00, sizeof(DSL_PM_HistoryStatsChDir_t));
      ret = ioctl(fd, DSL_FIO_PM_CHANNEL_HISTORY_STATS_1DAY_GET, &pmHistoryStats);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_CHANNEL_HISTORY_STATS_1DAY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 19, DSL_PM_HistoryStatsChDir_t)

/**
   This function returns DSL channel 15-minute counters of specified history interval.

   CLI
   - long command: PM_ChannelCounters15MinGet
   - short command: pmcc15mg

   \param DSL_PM_ChannelCounters_t*
      The parameter points to a \ref DSL_PM_ChannelCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_ChannelCounters_t pmChannelCounters;
      DSL_int_t ret = 0;

      memset(&pmChannelCounters, 0x00, sizeof(DSL_PM_ChannelCounters_t));
      ret = ioctl(fd, DSL_FIO_PM_CHANNEL_COUNTERS_15MIN_GET, &pmChannelCounters);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_CHANNEL_COUNTERS_15MIN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 20, DSL_PM_ChannelCounters_t)

/**
   This function returns DSL channel 1-day counters of specified history interval.

   CLI
   - long command: PM_ChannelCounters1DayGet
   - short command: pmcc1dg

   \param DSL_PM_ChannelCounters_t*
      The parameter points to a \ref DSL_PM_ChannelCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_ChannelCounters_t pmChannelCounters;
      DSL_int_t ret = 0;

      memset(&pmChannelCounters, 0x00, sizeof(DSL_PM_ChannelCounters_t));
      ret = ioctl(fd, DSL_FIO_PM_CHANNEL_COUNTERS_1DAY_GET, &pmChannelCounters);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_CHANNEL_COUNTERS_1DAY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 21, DSL_PM_ChannelCounters_t)

/**
   This function returns DSL channel total counters accumulated since system startup.

   CLI
   - long command: PM_ChannelCountersTotalGet
   - short command: pmcctg

   \param DSL_PM_ChannelCountersTotal_t*
      The parameter points to a \ref DSL_PM_ChannelCountersTotal_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_ChannelCountersTotal_t pmChannelCountersTotal;
      DSL_int_t ret = 0;

      memset(&pmChannelCountersTotal, 0x00, sizeof(DSL_PM_ChannelCountersTotal_t));
      ret = ioctl(fd, DSL_FIO_PM_CHANNEL_COUNTERS_TOTAL_GET, &pmChannelCountersTotal);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_CHANNEL_COUNTERS_TOTAL_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 22, DSL_PM_ChannelCountersTotal_t)

/**
   This function sets DSL channel 15-minute counter thresholds.

   CLI
   - long command: PM_ChannelThresholds15MinSet
   - short command: pmct15ms

   \param DSL_PM_ChannelThreshold_t*
      The parameter points to a \ref DSL_PM_ChannelThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_ChannelThreshold_t pmChannelThresholds;
      DSL_int_t ret = 0;

      memset(&pmChannelThresholds, 0x00, sizeof(DSL_PM_ChannelThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_CHANNEL_THRESHOLDS_15MIN_SET, &pmChannelThresholds);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_CHANNEL_THRESHOLDS_15MIN_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 23, DSL_PM_ChannelThreshold_t)

/**
   This function sets DSL channel 1-day counter thresholds.

   CLI
   - long command: PM_ChannelThresholds1DaySet
   - short command: pmct1ds

   \param DSL_PM_ChannelThreshold_t*
      The parameter points to a \ref DSL_PM_ChannelThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_ChannelThreshold_t pmChannelThresholds;
      DSL_int_t ret = 0;

      memset(&pmChannelThresholds, 0x00, sizeof(DSL_PM_ChannelThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_CHANNEL_THRESHOLDS_1DAY_SET, &pmChannelThresholds);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_CHANNEL_THRESHOLDS_1DAY_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 24, DSL_PM_ChannelThreshold_t)

/**
   This function returns DSL channel 15-minute counter thresholds.

   CLI
   - long command: PM_ChannelThresholds15MinGet
   - short command: pmct15mg

   \param DSL_PM_ChannelThreshold_t*
      The parameter points to a \ref DSL_PM_ChannelThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_ChannelThreshold_t pmChannelThresholds;
      DSL_int_t ret = 0;

      memset(&pmChannelThresholds, 0x00, sizeof(DSL_PM_ChannelThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_CHANNEL_THRESHOLDS_15MIN_GET, &pmChannelThresholds);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_CHANNEL_THRESHOLDS_15MIN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 25, DSL_PM_ChannelThreshold_t)

/**
   This function returns DSL channel 1-day counter thresholds.

   CLI
   - long command: PM_ChannelThresholds1DayGet
   - short command: pmct1dg

   \param DSL_PM_ChannelThreshold_t*
      The parameter points to a \ref DSL_PM_ChannelThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_ChannelThreshold_t pmChannelThresholds;
      DSL_int_t ret = 0;

      memset(&pmChannelThresholds, 0x00, sizeof(DSL_PM_ChannelThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_CHANNEL_THRESHOLDS_1DAY_GET, &pmChannelThresholds);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_CHANNEL_THRESHOLDS_1DAY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 26, DSL_PM_ChannelThreshold_t)

/**
   This function returns DSL data path 15-minute counter history statistics.

   CLI
   - long command: PM_DataPathHistoryStats15MinGet
   - short command: pmdphs15mg

   \param DSL_PM_HistoryStatsChDir_t*
      The parameter points to a \ref DSL_PM_HistoryStatsChDir_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_HistoryStatsChDir_t pmDataPathHistoryStats;
      DSL_int_t ret = 0;

      memset(&pmDataPathHistoryStats, 0x00, sizeof(DSL_PM_HistoryStatsChDir_t));
      ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_HISTORY_STATS_15MIN_GET, &pmDataPathHistoryStats);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_DATA_PATH_HISTORY_STATS_15MIN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 27, DSL_PM_HistoryStatsChDir_t)

/**
   This function returns DSL data path 1-day counter history statistics.

   CLI
   - long command: PM_DataPathHistoryStats1DayGet
   - short command: pmdphs1dg

   \param DSL_PM_HistoryStatsChDir_t*
      The parameter points to a \ref DSL_PM_HistoryStatsChDir_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_HistoryStatsChDir_t pmDataPathHistoryStats;
      DSL_int_t ret = 0;

      memset(&pmDataPathHistoryStats, 0x00, sizeof(DSL_PM_HistoryStatsChDir_t));
      ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_HISTORY_STATS_1DAY_GET, &pmDataPathHistoryStats);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_DATA_PATH_HISTORY_STATS_1DAY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 28, DSL_PM_HistoryStatsChDir_t)

/**
   This function returns DSL data path 15-minute counters of specified history
   interval.

   CLI
   - long command: PM_DataPathCounters15MinGet
   - short command: pmdpc15mg

   \param DSL_PM_DataPathCounters_t*
      The parameter points to a \ref DSL_PM_DataPathCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_DataPathCounters_t pmDataPathCounters;
      DSL_int_t ret = 0;

      memset(&pmDataPathCounters, 0x00, sizeof(DSL_PM_DataPathCounters_t));
      ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_COUNTERS_15MIN_GET, &pmDataPathCounters);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_DATA_PATH_COUNTERS_15MIN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 29, DSL_PM_DataPathCounters_t)

/**
   This function returns DSL data path 1-day counters of specified history
   interval.

   CLI
   - long command: PM_DataPathCounters1DayGet
   - short command: pmdpc1dg

   \param DSL_PM_DataPathCounters_t*
      The parameter points to a \ref DSL_PM_DataPathCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_DataPathCounters_t pmDataPathCounters;
      DSL_int_t ret = 0;

      memset(&pmDataPathCounters, 0x00, sizeof(DSL_PM_DataPathCounters_t));
      ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_COUNTERS_1DAY_GET, &pmDataPathCounters);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_DATA_PATH_COUNTERS_1DAY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 30, DSL_PM_DataPathCounters_t)

/**
   This function returns DSL data path total counters accumulated since system
   startup.

   CLI
   - long command: PM_DataPathCountersTotalGet
   - short command: pmdpctg

   \param DSL_PM_DataPathCountersTotal_t*
      The parameter points to a \ref DSL_PM_DataPathCountersTotal_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_DataPathCountersTotal_t pmDataPathCountersTotal;
      DSL_int_t ret = 0;

      memset(&pmDataPathCountersTotal, 0x00, sizeof(DSL_PM_DataPathCountersTotal_t));
      ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_COUNTERS_TOTAL_GET, &pmDataPathCountersTotal);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_DATA_PATH_COUNTERS_TOTAL_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 31, DSL_PM_DataPathCountersTotal_t)

/**
   This function sets DSL data path 15-minute counter thresholds.

   CLI
   - long command: PM_DataPathThresholds15MinSet
   - short command: pmdpt15ms

   \param DSL_PM_DataPathThreshold_t*
      The parameter points to a \ref DSL_PM_DataPathThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_DataPathThreshold_t pmDataPathThresholds;
      DSL_int_t ret = 0;

      memset(&pmDataPathThresholds, 0x00, sizeof(DSL_PM_DataPathThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_THRESHOLDS_15MIN_SET, &pmDataPathThresholds);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_DATA_PATH_THRESHOLDS_15MIN_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 32, DSL_PM_DataPathThreshold_t)

/**
   This function sets DSL data path 1-day counter thresholds.

   CLI
   - long command: PM_DataPathThresholds1DaySet
   - short command: pmdpt1ds

   \param DSL_PM_DataPathThreshold_t*
      The parameter points to a \ref DSL_PM_DataPathThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_DataPathThreshold_t pmDataPathThresholds;
      DSL_int_t ret = 0;

      memset(&pmDataPathThresholds, 0x00, sizeof(DSL_PM_DataPathThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_THRESHOLDS_1DAY_SET, &pmDataPathThresholds);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_DATA_PATH_THRESHOLDS_1DAY_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 33, DSL_PM_DataPathThreshold_t)

/**
   This function returns DSL data path 15-minute counter thresholds.

   CLI
   - long command: PM_DataPathThresholds15MinGet
   - short command: pmdpt15mg

   \param DSL_PM_DataPathThreshold_t*
      The parameter points to a \ref DSL_PM_DataPathThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_DataPathThreshold_t pmDataPathThresholds;
      DSL_int_t ret = 0;

      memset(&pmDataPathThresholds, 0x00, sizeof(DSL_PM_DataPathThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_THRESHOLDS_15MIN_GET, &pmDataPathThresholds);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_DATA_PATH_THRESHOLDS_15MIN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 34, DSL_PM_DataPathThreshold_t)

/**
   This function returns DSL data path 1-day counter thresholds.

   CLI
   - long command: PM_DataPathThresholds1DayGet
   - short command: pmdpt1dg

   \param DSL_PM_DataPathThreshold_t*
      The parameter points to a \ref DSL_PM_DataPathThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_DataPathThreshold_t pmDataPathThresholds;
      DSL_int_t ret = 0;

      memset(&pmDataPathThresholds, 0x00, sizeof(DSL_PM_DataPathThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_THRESHOLDS_1DAY_GET, &pmDataPathThresholds);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_DATA_PATH_THRESHOLDS_1DAY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 35, DSL_PM_DataPathThreshold_t)

/**
   This function reset ALL PM data including history values except the total
   counters.

   CLI
   - long command: PM_Reset
   - short command: pmr

   \param DSL_PM_Reset_t*
      The parameter points to a \ref DSL_PM_Reset_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_Reset_t pmReset;
      DSL_int_t ret = 0;

      pmReset = DSL_PM_RESET_ALL;
      memset(&pmReset, 0x00, sizeof(DSL_PM_Reset_t));
      ret = ioctl(fd, DSL_FIO_PM_RESET, &pmReset);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_RESET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 36, DSL_PM_Reset_t)

/**
   Setup a trigger event to signal that the 15 minutes timer has been elapsed.
   In addition the elapsed day interval can be marked.

   The trigger should be used with care. After completion of the update of the
   PM data base the event DSL_CBS_PM_SYNC is send to the user application.
   The next synchronization trigger should be issued at least after receiving
   of the previous DSL_CBS_PM_SYNC event.

   CLI
   - long command: PM_15MinElapsedExtTrigger
   - short command: pm15meet

   \param DSL_PM_ElapsedExtTrigger_t*
      The parameter points to a \ref DSL_PM_ElapsedExtTrigger_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_ElapsedExtTrigger_t pmTrigger;
      DSL_int_t ret = 0;

      pmTrigger->bOneDayElapsed = DSL_FALSE;
      memset(&pmTrigger, 0x00, sizeof(DSL_PM_ElapsedExtTrigger_t));
      ret = ioctl(fd, DSL_FIO_PM_15MIN_ELAPSED_EXT_TRIGGER, &pmTrigger);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_15MIN_ELAPSED_EXT_TRIGGER \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 37, DSL_PM_ElapsedExtTrigger_t)

/**
   This function resets PM elapsed 15 min, 1 day, showtime time of the current
   interval and the total time reported with the
   DSL_FIO_PM_XXX_COUNTERS_TOTAL_GET. This command has no effect in
   \ref DSL_PM_SyncMode_t DSL_PM_SYNC_MODE_SYS_TIME on the current 15min and
   1day time, as they depend directly on the system time but the total time
   will be reset to 0 in this case.

   CLI
   - long command: PM_ElapsedTimeReset
   - short command: pmetr

   \param DSL_PM_ElapsedTimeReset_t*
      The parameter points to a \ref DSL_PM_ElapsedTimeReset_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_ElapsedTimeReset_t pmReset;
      DSL_int_t ret = 0;

      memset(&pmReset, 0x00, sizeof(DSL_PM_ElapsedTimeReset_t));
      ret = ioctl(fd, DSL_FIO_PM_ELAPSED_TIME_RESET, &pmReset);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_ELAPSED_TIME_RESET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 38, DSL_PM_ElapsedTimeReset_t)

/**
   This function sets PM synchronization mode.

   CLI
   - long command: PM_SyncModeSet
   - short command: pmsms

   \param DSL_PM_SyncMode_t*
      The parameter points to a \ref DSL_PM_SyncMode_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_SyncMode_t pmSyncMode;
      DSL_int_t ret = 0;

      pmSyncMode->nMode = DSL_PM_SYNC_MODE_EXTERNAL;
      memset(&pmSyncMode, 0x00, sizeof(DSL_PM_SyncMode_t));
      ret = ioctl(fd, DSL_FIO_PM_SYNC_MODE_SET, &pmSyncMode);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_SYNC_MODE_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 39, DSL_PM_SyncMode_t)

/**
   This function enables the PM burnin mode.
   This function can be used for test purposes by changing the default
   time base settings of the PM.

   CLI
   - long command: PM_BurninModeSet
   - short command: pmbms

   \param DSL_PM_BurninMode_t*
      The parameter points to a \ref DSL_PM_BurninMode_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_BurninMode_t pmBurninMode;
      DSL_int_t ret = 0;

      pmBurninMode->nMode->nPmTick = 10;
      pmBurninMode->nMode->nPm15Min = 100;
      pmBurninMode->nMode->nPm15MinPerDay = 2;
      memset(&pmBurninMode, 0x00, sizeof(DSL_PM_BurninMode_t));
      ret = ioctl(fd, DSL_FIO_PM_BURNIN_MODE_SET, &pmBurninMode);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_BURNIN_MODE_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 40, DSL_PM_BurninMode_t)

#if defined(DSL_PM_DEBUG_MODE_ENABLE) && (DSL_PM_DEBUG_MODE_ENABLE > 0)
/**
   This function print the PM data of all lines to the console.
   To show the data of a dedicated line please use the function DSL_PM_LineDump.
   This function is available if the compile switch DSL_PM_DEBUG_MODE_ENABLE
   is set to 1 (default mode).

   CLI command: n/a

   \param DSL_PM_Dump_t*
      The parameter points to a \ref DSL_PM_Dump_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.
   - XWAY(TM) VRX200: xDSL-CPE
   - XWAY(TM) VRX300: xDSL-CPE

   \code
      DSL_PM_Dump_t pmDump;
      DSL_int_t ret = 0;

      pmDump->dumpConfig->bPrintMemStatOnly = DSL_TRUE;
      memset(&pmDump, 0x00, sizeof(DSL_PM_Dump_t));
      ret = ioctl(fd, DSL_FIO_PM_DUMP, &pmDump);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_DUMP \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 41, DSL_PM_Dump_t)
#endif /* defined(DSL_PM_DEBUG_MODE_ENABLE) && (DSL_PM_DEBUG_MODE_ENABLE > 0) */

/**
   This function returns returns DSL second counter statistics accumulated since
   the specified showtime interval.
   Implementation is done according to specification within TR-98.

   CLI
   - long command: PM_LineSecCountersShowtimeGet
   - short command: pmlscsg

   \param DSL_PM_LineSecCounters_t*
      The parameter points to a \ref DSL_PM_LineSecCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_LineSecCounters_t pmLineSecCounter;
      DSL_int_t ret = 0;

      memset(&pmLineSecCounter, 0x00, sizeof(DSL_PM_LineSecCounters_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_SEC_COUNTERS_15MIN_GET, &pmLineSecCounter);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_SEC_COUNTERS_SHOWTIME_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 42, DSL_PM_LineSecCounters_t)

/**
   This function returns returns DSL initialization counter statistics
   accumulated since the specified showtime interval.
   Implementation is done according to specification within TR-98.

   CLI
   - long command: PM_LineInitCountersShowtimeGet
   - short command: pmlicsg

   \param DSL_PM_LineInitCounters_t*
      The parameter points to a \ref DSL_PM_LineInitCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_LineInitCounters_t pmLineInitCounters;
      DSL_int_t ret = 0;

      memset(&pmLineInitCounters, 0x00, sizeof(DSL_PM_LineInitCounters_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_INIT_COUNTERS_15MIN_GET, &pmLineInitCounters);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_INIT_COUNTERS_SHOWTIME_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 43, DSL_PM_LineInitCounters_t)

/**
   This function returns returns DSL channel counter statistics accumulated
   since the specified showtime interval.
   Implementation is done according to specification within TR-98.

   CLI
   - long command: PM_ChannelCountersShowtimeGet
   - short command: pmccsg

   \param DSL_PM_ChannelCounters_t*
      The parameter points to a \ref DSL_PM_ChannelCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_ChannelCounters_t pmChannelCounters;
      DSL_int_t ret = 0;

      memset(&pmChannelCounters, 0x00, sizeof(DSL_PM_ChannelCounters_t));
      ret = ioctl(fd, DSL_FIO_PM_CHANNEL_COUNTERS_15MIN_GET, &pmChannelCounters);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_CHANNEL_COUNTERS_SHOWTIME_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 44, DSL_PM_ChannelCounters_t)

/**
   This function returns returns DSL data path counter statistics accumulated
   since the specified showtime interval.
   Implementation is done according to specification within TR-98.

   CLI
   - long command: PM_DataPathCountersShowtimeGet
   - short command: pmdpcsg

   \param DSL_PM_DataPathCounters_t*
      The parameter points to a \ref DSL_PM_DataPathCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_DataPathCounters_t pmDataPathCounters;
      DSL_int_t ret = 0;

      memset(&pmDataPathCounters, 0x00, sizeof(DSL_PM_DataPathCounters_t));
      ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_COUNTERS_15MIN_GET, &pmDataPathCounters);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_DATA_PATH_COUNTERS_SHOWTIME_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 45, DSL_PM_DataPathCounters_t)

/**
   This function provides information about the currently active, PM related,
   basic configuration options.

   CLI
   - long command: PM_ConfigGet
   - short command: pmcg

   \param DSL_PM_Config_t*
      The parameter points to a \ref DSL_PM_Config_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_Config_t pmConfig;
      DSL_int_t ret = 0;

      memset(&pmConfig, 0x00, sizeof(DSL_PM_Config_t));
      ret = ioctl(fd, DSL_FIO_PM_CONFIG_GET, &pmConfig);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_CONFIG_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 46, DSL_PM_Config_t)


/**
   This function provides the possibility to change the PM related, basic
   configuration options.

   CLI
   - long command: PM_ConfigSet
   - short command: pmcs

   \param DSL_PM_Config_t*
      The parameter points to a \ref DSL_PM_Config_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   - Supported by all platforms.
   - Please note that a change of this configuration setting(s) will be only
     effective after restarting the autoboot handling using ioctl
     \ref DSL_FIO_AUTOBOOT_CONTROL_SET with command \ref DSL_AUTOBOOT_CTRL_RESTART

   \code
      DSL_PM_Config_t pmConfig;
      DSL_int_t ret = 0;

      memset(&pmConfig, 0x00, sizeof(DSL_PM_Config_t));
      pmConfig.bFePollingOff = DSL_TRUE;
      ret = ioctl(fd, DSL_FIO_PM_CONFIG_SET, &pmConfig);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_CONFIG_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 47, DSL_PM_Config_t)

/**
   This function returns returns DSL extended channel counter statistics accumulated
   since the specified showtime interval.
   Implementation is done according to specification within TR-98.

   CLI
   - long command: PM_ChannelCountersShowtimeExtGet
   - short command: pmccseg

   \param DSL_PM_ChannelCountersExt_t*
      The parameter points to a \ref DSL_PM_ChannelCountersExt_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - ADSL only platforms (XWAY(TM) ARX100, XWAY(TM) ARX300)

   \code
      DSL_PM_ChannelCountersExt_t pmChannelCountersExt;
      DSL_int_t ret = 0;

      memset(&pmChannelCountersExt, 0x00, sizeof(DSL_PM_ChannelCountersExt_t));
      ret = ioctl(fd, DSL_FIO_PM_CHANNEL_COUNTERS_SHOWTIME_EXT_GET, &pmChannelCountersExt);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_CHANNEL_COUNTERS_SHOWTIME_EXT_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 48, DSL_PM_ChannelCountersExt_t)

/**
   This function returns DSL line event showtime 15-minute counters of specified history
   interval.

   CLI
   - long command: PM_LineEventShowtimeCounters15MinGet
   - short command: pmlesc15mg

   \param DSL_PM_LineEventShowtimeCounters_t*
      The parameter points to a \ref DSL_PM_LineEventShowtimeCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_LineEventShowtimeCounters_t pmLineEventShowtimeCounter;
      DSL_int_t ret = 0;

      memset(&pmLineEventShowtimeCounter, 0x00, sizeof(DSL_PM_LineEventShowtimeCounters_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_EVENT_SHOWTIME_COUNTERS_15MIN_GET, &pmLineEventShowtimeCounter);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_EVENT_SHOWTIME_COUNTERS_15MIN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 49, DSL_PM_LineEventShowtimeCounters_t)

/**
   This function returns DSL line event showtime 1-day counters of specified history
   interval.

   CLI
   - long command: PM_LineEventShowtimeCounters1DayGet
   - short command: pmlesc1dg

   \param DSL_PM_LineEventShowtimeCounters_t*
      The parameter points to a \ref DSL_PM_LineEventShowtimeCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_LineEventShowtimeCounters_t pmLineEventShowtimeCounter;
      DSL_int_t ret = 0;

      memset(&pmLineEventShowtimeCounter, 0x00, sizeof(DSL_PM_LineEventShowtimeCounters_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_EVENT_SHOWTIME_COUNTERS_1DAY_GET, &pmLineEventShowtimeCounter);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_EVENT_SHOWTIME_COUNTERS_1DAY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 50, DSL_PM_LineEventShowtimeCounters_t)

/**
   This function returns returns DSL line event showtime counter statistics accumulated
   since the specified showtime interval.
   Implementation is done according to specification within TR-98.

   CLI
   - long command: PM_LineEventShowtimeCountersShowtimeGet
   - short command: pmlescsg

   \param DSL_PM_LineEventShowtimeCounters_t*
      The parameter points to a \ref DSL_PM_LineEventShowtimeCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_LineEventShowtimeCounters_t pmLineEventShowtimeCounter;
      DSL_int_t ret = 0;

      memset(&pmLineEventShowtimeCounter, 0x00, sizeof(DSL_PM_LineEventShowtimeCounters_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_EVENT_SHOWTIME_COUNTERS_SHOWTIME_GET, &pmLineEventShowtimeCounter);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_EVENT_SHOWTIME_COUNTERS_SHOWTIME_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 51, DSL_PM_LineEventShowtimeCounters_t)

/**
   This function returns DSL line event showtime total counters accumulated since system
   startup.

   CLI
   - long command: PM_LineEventShowtimeCountersTotalGet
   - short command: pmlesctg

   \param DSL_PM_LineEventShowtimeCountersTotal_t*
      The parameter points to a \ref DSL_PM_LineEventShowtimeCountersTotal_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_LineEventShowtimeCountersTotal_t pmLineEventShowtimeCountersTotal;
      DSL_int_t ret = 0;

      memset(&pmLineEventShowtimeCountersTotal, 0x00, sizeof(DSL_PM_LineEventShowtimeCountersTotal_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_EVENT_SHOWTIME_COUNTERS_TOTAL_GET, &pmLineEventShowtimeCountersTotal);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_EVENT_SHOWTIME_COUNTERS_TOTAL_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 52, DSL_PM_LineEventShowtimeCountersTotal_t)

/**
   This function returns DSL line event showtime 15-minute counter history statistics.

   CLI
   - long command: PM_LineEventShowtimeHistoryStats15MinGet
   - short command: pmleshs15mg

   \param DSL_PM_HistoryStatsChDir_t*
      The parameter points to a \ref DSL_PM_HistoryStatsChDir_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_HistoryStatsChDir_t pmLineEventShowtimeHistoryStats;
      DSL_int_t ret = 0;

      memset(&pmLineEventShowtimeHistoryStats, 0x00, sizeof(DSL_PM_HistoryStatsChDir_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_EVENT_SHOWTIME_HISTORY_STATS_15MIN_GET, &pmLineEventShowtimeHistoryStats);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_EVENT_SHOWTIME_HISTORY_STATS_15MIN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 53, DSL_PM_HistoryStatsChDir_t)

/**
   This function returns DSL line event showtime 1-day counter history statistics.

   CLI
   - long command: PM_LineEventShowtimeHistoryStats1DayGet
   - short command: pmleshs1dg

   \param DSL_PM_HistoryStatsChDir_t*
      The parameter points to a \ref DSL_PM_HistoryStatsChDir_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_HistoryStatsChDir_t pmLineEventShowtimeHistoryStats;
      DSL_int_t ret = 0;

      memset(&pmLineEventShowtimeHistoryStats, 0x00, sizeof(DSL_PM_HistoryStatsChDir_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_EVENT_SHOWTIME_HISTORY_STATS_1DAY_GET, &pmLineEventShowtimeHistoryStats);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_EVENT_SHOWTIME_HISTORY_STATS_1DAY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 54, DSL_PM_HistoryStatsChDir_t)

/**
   This function returns DSL data path failure 15-minute counters of specified history
   interval.

   CLI
   - long command: PM_DataPathFailureCounters15MinGet
   - short command: pmdpfc15mg

   \param DSL_PM_DataPathFailureCounters_t*
      The parameter points to a \ref DSL_PM_DataPathFailureCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_DataPathFailureCounters_t pmDataPathFailureCounter;
      DSL_int_t ret = 0;

      memset(&pmDataPathFailureCounter, 0x00, sizeof(DSL_PM_DataPathFailureCounters_t));
      ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_FAILURE_COUNTERS_15MIN_GET, &pmDataPathFailureCounter);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_DATA_PATH_FAILURE_COUNTERS_15MIN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 55, DSL_PM_DataPathFailureCounters_t)

/**
   This function returns DSL data path 1-day counters of specified history
   interval.

   CLI
   - long command: PM_DataPathFailureCounters1DayGet
   - short command: pmdpfc1dg

   \param DSL_PM_DataPathFailureCounters_t*
      The parameter points to a \ref DSL_PM_DataPathFailureCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_DataPathFailureCounters_t pmDataPathFailureCounter;
      DSL_int_t ret = 0;

      memset(&pmDataPathFailureCounter, 0x00, sizeof(DSL_PM_DataPathFailureCounters_t));
      ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_FAILURE_COUNTERS_1DAY_GET, &pmDataPathFailureCounter);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_DATA_PATH_FAILURE_COUNTERS_1DAY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 56, DSL_PM_DataPathFailureCounters_t)

/**
   This function returns returns DSL data path failure counter statistics accumulated
   since the specified showtime interval.
   Implementation is done according to specification within TR-98.

   CLI
   - long command: PM_DataPathFailureCountersShowtimeGet
   - short command: pmdpfcsg

   \param DSL_PM_DataPathFailureCounters_t*
      The parameter points to a \ref DSL_PM_DataPathFailureCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_DataPathFailureCounters_t pmDataPathFailureCounter;
      DSL_int_t ret = 0;

      memset(&pmDataPathFailureCounter, 0x00, sizeof(DSL_PM_DataPathFailureCounters_t));
      ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_FAILURE_COUNTERS_SHOWTIME_GET, &pmDataPathFailureCounter);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_DATA_PATH_FAILURE_COUNTERS_SHOWTIME_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 57, DSL_PM_DataPathFailureCounters_t)

/**
   This function returns DSL data path failure total counters accumulated since system
   startup.

   CLI
   - long command: PM_DataPathFailureCountersTotalGet
   - short command: pmdpfctg

   \param DSL_PM_DataPathFailureCountersTotal_t*
      The parameter points to a \ref DSL_PM_DataPathFailureCountersTotal_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_DataPathFailureCountersTotal_t pmDataPathFailureCountersTotal;
      DSL_int_t ret = 0;

      memset(&pmDataPathFailureCountersTotal, 0x00, sizeof(DSL_PM_DataPathFailureCountersTotal_t));
      ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_FAILURE_COUNTERS_TOTAL_GET, &pmDataPathFailureCountersTotal);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_DATA_PATH_FAILURE_COUNTERS_TOTAL_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 58, DSL_PM_DataPathFailureCountersTotal_t)

/**
   This function returns DSL data path failure 15-minute counter history statistics.

   CLI
   - long command: PM_DataPathFailureHistoryStats15MinGet
   - short command: pmdpfhs15mg

   \param DSL_PM_HistoryStatsChDir_t*
      The parameter points to a \ref DSL_PM_HistoryStatsChDir_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_HistoryStatsChDir_t pmDataPathFailureHistoryStats;
      DSL_int_t ret = 0;

      memset(&pmDataPathFailureHistoryStats, 0x00, sizeof(DSL_PM_HistoryStatsChDir_t));
      ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_FAILURE_HISTORY_STATS_15MIN_GET, &pmDataPathFailureHistoryStats);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_DATA_PATH_FAILURE_HISTORY_STATS_15MIN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 59, DSL_PM_HistoryStatsChDir_t)

/**
   This function returns DSL data path failure 1-day counter history statistics.

   CLI
   - long command: PM_DataPathFailureHistoryStats1DayGet
   - short command: pmdpfhs1dg

   \param DSL_PM_HistoryStatsChDir_t*
      The parameter points to a \ref DSL_PM_HistoryStatsChDir_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_HistoryStatsChDir_t pmDataPathFailureHistoryStats;
      DSL_int_t ret = 0;

      memset(&pmDataPathFailureHistoryStats, 0x00, sizeof(DSL_PM_HistoryStatsChDir_t));
      ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_FAILURE_HISTORY_STATS_1DAY_GET, &pmDataPathFailureHistoryStats);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_DATA_PATH_FAILURE_HISTORY_STATS_1DAY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 60, DSL_PM_HistoryStatsChDir_t)

/**
   This function returns DSL Retransmission 15-minute counters of specified history
   interval.

   CLI
   - long command: PM_ReTxCounters15MinGet
   - short command: pmrtc15mg

   \param DSL_PM_ReTxCounters_t*
      The parameter points to a \ref DSL_PM_ReTxCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Please not that the implementation is preliminary. Other components within
   system (PHY and PPE FW) has a basic influence to the ReTx counters. Current
   implementation expects that the FW ReTx counters are total and never reset
   during FW reboot.
   Supported by all platforms.

   \code
      DSL_PM_ReTxCounters_t pmReTxCounter;
      DSL_int_t ret = 0;

      memset(&pmReTxCounter, 0x00, sizeof(DSL_PM_ReTxCounters_t));
      ret = ioctl(fd, DSL_FIO_PM_RETX_COUNTERS_15MIN_GET, &pmReTxCounter);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_RETX_COUNTERS_15MIN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 61, DSL_PM_ReTxCounters_t)

/**
   This function returns DSL Retransmission 1-day counters of specified history
   interval.

   CLI
   - long command: PM_ReTxCounters1DayGet
   - short command: pmrtc1dg

   \param DSL_PM_ReTxCounters_t*
      The parameter points to a \ref DSL_PM_ReTxCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Please not that the implementation is preliminary. Other components within
   system (PHY and PPE FW) has a basic influence to the ReTx counters. Current
   implementation expects that the FW ReTx counters are total and never reset
   during FW reboot.
   Supported by all platforms.

   \code
      DSL_PM_ReTxCounters_t pmReTxCounter;
      DSL_int_t ret = 0;

      memset(&pmReTxCounter, 0x00, sizeof(DSL_PM_ReTxCounters_t));
      ret = ioctl(fd, DSL_FIO_PM_RETX_COUNTERS_1DAY_GET, &pmReTxCounter);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_RETX_COUNTERS_1DAY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 62, DSL_PM_ReTxCounters_t)

/**
   This function returns DSL Retransmission showtime counters of specified history
   interval.

   CLI
   - long command: PM_ReTxCountersShowtimeGet
   - short command: pmrtcsg

   \param DSL_PM_ReTxCounters_t*
      The parameter points to a \ref DSL_PM_ReTxCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_ReTxCounters_t pmReTxCounter;
      DSL_int_t ret = 0;

      memset(&pmReTxCounter, 0x00, sizeof(DSL_PM_ReTxCounters_t));
      ret = ioctl(fd, DSL_FIO_PM_RETX_COUNTERS_SHOWTIME_GET, &pmReTxCounter);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_RETX_COUNTERS_SHOWTIME_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 63, DSL_PM_ReTxCounters_t)

/**
   This function returns DSL Retransmission total counters of specified history
   interval.

   CLI
   - long command: PM_ReTxCountersTotalGet
   - short command: pmrtctg

   \param DSL_PM_ReTxCounters_t*
      The parameter points to a \ref DSL_PM_ReTxCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_ReTxCounters_t pmReTxCounter;
      DSL_int_t ret = 0;

      memset(&pmReTxCounter, 0x00, sizeof(DSL_PM_ReTxCounters_t));
      ret = ioctl(fd, DSL_FIO_PM_RETX_COUNTERS_TOTAL_GET, &pmReTxCounter);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_RETX_COUNTERS_TOTAL_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 64, DSL_PM_ReTxCountersTotal_t)

/**
   This function returns DSL Retransmission 15-minute counter history statistics.

   CLI
   - long command: PM_ReTxHistoryStats15MinGet
   - short command: pmrths15mg

   \param DSL_PM_HistoryStatsDir_t*
      The parameter points to a \ref DSL_PM_HistoryStatsDir_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_HistoryStatsDir_t pmHistoryStats;
      DSL_int_t ret = 0;

      memset(&pmHistoryStats, 0x00, sizeof(DSL_PM_HistoryStatsDir_t));
      ret = ioctl(fd, DSL_FIO_PM_RETX_HISTORY_STATS_15MIN_GET, &pmHistoryStats);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_RETX_HISTORY_STATS_15MIN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 65, DSL_PM_HistoryStatsDir_t)

/**
   This function returns DSL Retransmission 1-day counter history statistics.

   CLI
   - long command: PM_ReTxHistoryStats1DayGet
   - short command: pmrths1dg

   \param DSL_PM_HistoryStatsDir_t*
      The parameter points to a \ref DSL_PM_HistoryStatsDir_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_HistoryStatsDir_t pmHistoryStats;
      DSL_int_t ret = 0;

      memset(&pmHistoryStats, 0x00, sizeof(DSL_PM_HistoryStatsDir_t));
      ret = ioctl(fd, DSL_FIO_PM_RETX_HISTORY_STATS_1DAY_GET, &pmHistoryStats);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_RETX_HISTORY_STATS_1DAY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 66, DSL_PM_HistoryStatsDir_t)

/**
   This function returns DSL Retransmission 15-minute counter thresholds.

   CLI
   - long command: PM_ReTxThresholds15MinGet
   - short command: pmrtt15mg

   \param DSL_PM_ReTxThreshold_t*
      The parameter points to a \ref DSL_PM_ReTxThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_ReTxThreshold_t pmReTxThresholds;
      DSL_int_t ret = 0;

      memset(&pmReTxThresholds, 0x00, sizeof(DSL_PM_ReTxThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_RETX_THRESHOLDS_15MIN_GET, &pmReTxThresholds);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_RETX_THRESHOLDS_15MIN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 67, DSL_PM_ReTxThreshold_t)

/**
   This function sets DSL Retransmission 15-minute counter thresholds.

   CLI
   - long command: PM_ReTxThresholds15MinSet
   - short command: pmrtt15ms

   \param DSL_PM_ReTxThreshold_t*
      The parameter points to a \ref DSL_PM_ReTxThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_ReTxThreshold_t pmReTxThresholds;
      DSL_int_t ret = 0;

      memset(&pmReTxThresholds, 0x00, sizeof(DSL_PM_ReTxThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_RETX_THRESHOLDS_15MIN_SET, &pmReTxThresholds);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_RETX_THRESHOLDS_15MIN_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 68, DSL_PM_ReTxThreshold_t)

/**
   This function returns DSL Retransmission 1-day counter thresholds.

   CLI
   - long command: PM_ReTxThresholds1DayGet
   - short command: pmrtt1dg

   \param DSL_PM_ReTxThreshold_t*
      The parameter points to a \ref DSL_PM_ReTxThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_ReTxThreshold_t pmReTxThresholds;
      DSL_int_t ret = 0;

      memset(&pmReTxThresholds, 0x00, sizeof(DSL_PM_ReTxThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_RETX_THRESHOLDS_1DAY_GET, &pmReTxThresholds);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_RETX_THRESHOLDS_1DAY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 69, DSL_PM_ReTxThreshold_t)

/**
   This function sets DSL Retransmission 1-day counter thresholds.

   CLI
   - long command: PM_ReTxThresholds1DaySet
   - short command: pmrtt1ds

   \param DSL_PM_ReTxThreshold_t*
      The parameter points to a \ref DSL_PM_ReTxThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_ReTxThreshold_t pmReTxThresholds;
      DSL_int_t ret = 0;

      memset(&pmReTxThresholds, 0x00, sizeof(DSL_PM_ReTxThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_RETX_THRESHOLDS_1DAY_SET, &pmReTxThresholds);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_RETX_THRESHOLDS_1DAY_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 70, DSL_PM_ReTxThreshold_t)

/**
   This function gets PM synchronization mode.

   CLI
   - long command: PM_SyncModeGet
   - short command: pmsmg

   \param DSL_PM_SyncMode_t*
      The parameter points to a \ref DSL_PM_SyncMode_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by all platforms.

   \code
      DSL_PM_SyncMode_t pmSyncMode;
      DSL_int_t ret = 0;

      memset(&pmSyncMode, 0x00, sizeof(DSL_PM_SyncMode_t));
      ret = ioctl(fd, DSL_FIO_PM_SYNC_MODE_GET, &pmSyncMode);
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_SYNC_MODE_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_PM, 71, DSL_PM_SyncMode_t)

#ifdef INCLUDE_DEPRECATED

/* ************************************************************************** */
/* * Ioctl interface for deprecated functions                               * */
/* ************************************************************************** */

  /**
   \deprecated This function is deprecated and will be removed in the future.

   \attention Please only use the new function
              \ref DSL_FIO_PM_LINE_EVENT_SHOWTIME_COUNTERS_15MIN_GET.

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_FAILURE_COUNTERS_15MIN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_DEP, 0, DSL_PM_LineFailureCounters_t)

/**
   \deprecated This function is deprecated and will be removed in the future.

   \attention Please only use the new function
              \ref DSL_FIO_PM_LINE_EVENT_SHOWTIME_COUNTERS_1DAY_GET.

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_FAILURE_COUNTERS_1DAY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_DEP, 1, DSL_PM_LineFailureCounters_t)

/**
   \deprecated This function is deprecated and will be removed in the future.

   \attention Please only use the new function
              \ref DSL_FIO_PM_LINE_EVENT_SHOWTIME_COUNTERS_SHOWTIME_GET.

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_FAILURE_COUNTERS_SHOWTIME_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_DEP, 2, DSL_PM_LineFailureCounters_t)

/**
   \deprecated This function is deprecated and will be removed in the future.

   \attention Please only use the new function
              \ref DSL_FIO_PM_LINE_EVENT_SHOWTIME_COUNTERS_TOTAL_GET.

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_FAILURE_COUNTERS_TOTAL_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_DEP, 3, DSL_PM_LineFailureCountersTotal_t)

/**
   \deprecated This function is deprecated and will be removed in the future.

   \attention Please only use the new function
              \ref DSL_FIO_PM_LINE_EVENT_SHOWTIME_HISTORY_STATS_15MIN_GET.

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_FAILURE_HISTORY_STATS_15MIN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_DEP, 4, DSL_PM_HistoryStatsChDir_t)

/**
   \deprecated This function is deprecated and will be removed in the future.

   \attention Please only use the new function
              \ref DSL_FIO_PM_LINE_EVENT_SHOWTIME_HISTORY_STATS_1DAY_GET.

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_FAILURE_HISTORY_STATS_1DAY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_DEP, 5, DSL_PM_HistoryStatsChDir_t)

#endif /* INCLUDE_DEPRECATED */

#ifdef INCLUDE_REAL_TIME_TRACE

/**
   This ioctl initializes the Real Time Trace module of the DSL CPE API.

   CLI
   - long command: RTT_Init
   - short command: rtti

   \param DSL_RTT_Init_t*
      The parameter points to a \ref DSL_RTT_Init_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - ADSL only platforms (XWAY(TM) ARX100, XWAY(TM) ARX300)

   \code
      DSL_RTT_Init_t RTT_Init;
      DSL_int_t ret = 0;

      memset(&RTT_Init, 0x00, sizeof(DSL_RTT_Init_t));
      ret = ioctl(fd, DSL_FIO_RTT_INIT, &RTT_Init);
   \endcode

   \ingroup DRV_DSL_CPE_DEBUG_RTT */
#define DSL_FIO_RTT_INIT \
   _IOWR(DSL_IOC_MAGIC_CPE_API_RTT, 0, DSL_RTT_Init_t)

/**
   This ioctl configures the Real Time Trace (RTT) functionality of the
   DSL CPE API Driver.

   CLI
   - long command: RTT_ConfigSet
   - short command: rttcs

   \param DSL_RTT_Config_t*
      The parameter points to a \ref DSL_RTT_Config_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - ADSL only platforms (XWAY(TM) ARX100, XWAY(TM) ARX300)
   \remarks
   - Please note that a change of this configuration setting(s) will be only
     effective after restarting the autoboot handling using ioctl
     \ref DSL_FIO_AUTOBOOT_CONTROL_SET with command \ref DSL_AUTOBOOT_CTRL_RESTART

   \code
      DSL_RTT_Config_t RTT_Config;
      DSL_int_t ret = 0;

      memset(&RTT_Config, 0x00, sizeof(DSL_RTT_Config_t));
      // Set configuration parameter
      RTT_Config = ;
      ret = ioctl(fd, DSL_FIO_RTT_CONFIG_SET, &RTT_Config);
      // Check return value and do exception handling if necessary
   \endcode

   \ingroup DRV_DSL_CPE_DEBUG_RTT */
#define DSL_FIO_RTT_CONFIG_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_RTT, 1, DSL_RTT_Config_t)

/**
   This ioctl returns current configuration for the Real Time Trace (RTT)
   functionality of the DSL CPE API Driver.

   CLI
   - long command: RTT_ConfigGet
   - short command: rttcg

   \param DSL_RTT_Config_t*
      The parameter points to a \ref DSL_RTT_Config_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - ADSL only platforms (XWAY(TM) ARX100, XWAY(TM) ARX300)

   \code
      DSL_RTT_Config_t RTT_Config;
      DSL_int_t ret = 0;

      memset(&RTT_Config, 0x00, sizeof(DSL_RTT_Config_t));
      ret = ioctl(fd, DSL_FIO_RTT_CONFIG_GET, &RTT_Config);
      // Process configuration parameter
   \endcode

   \ingroup DRV_DSL_CPE_DEBUG_RTT */
#define DSL_FIO_RTT_CONFIG_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_RTT, 2, DSL_RTT_Config_t)

/**
   This ioctl returns the status of the Real Time Trace (RTT) processing.

   CLI
   - long command: RTT_StatusGet
   - short command: rttsg

   \param DSL_RTT_Status_t*
      The parameter points to a \ref DSL_RTT_Status_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - ADSL only platforms (XWAY(TM) ARX100, XWAY(TM) ARX300)

   \code
      DSL_RTT_Status_t DSL_RTT_Status;
      DSL_int_t ret = 0;

      memset(&DSL_RTT_Status, 0x00, sizeof(DSL_RTT_Status_t));
      ret = ioctl(fd, DSL_FIO_RTT_STATUS_GET, &DSL_RTT_Status);
      // Process status parameter
   \endcode

   \ingroup DRV_DSL_CPE_DEBUG_RTT */
#define DSL_FIO_RTT_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_RTT, 3, DSL_RTT_Status_t)

/**
   This ioctl returns statistic counter of the Real Time Trace (RTT)
   functionality within MEI Driver and firmware.

   CLI
   - long command: RTT_ConTRoLSet
   - short command: rttctrls

   \param DSL_RTT_Control_t*
      The parameter points to a \ref DSL_RTT_Control_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - ADSL only platforms (XWAY(TM) ARX100, XWAY(TM) ARX300)

   \code
      DSL_RTT_Control_t RTT_Control;
      DSL_int_t ret = 0;

      memset(&RTT_Control, 0x00, sizeof(DSL_RTT_Control_t));
      // Set control parameter
      RTT_Config = ;
      ret = ioctl(fd, DSL_FIO_RTT_CONTROL_SET, &RTT_Control);
      // Check return value and do exception handling if necessary
   \endcode

   \ingroup DRV_DSL_CPE_DEBUG_RTT */
#define DSL_FIO_RTT_CONTROL_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_RTT, 4, DSL_RTT_Control_t)

/**
   This ioctl returns statistic counter of the Real Time Trace (RTT)
   functionality within MEI Driver and firmware.

   CLI
   - long command: RTT_STATisticsGet
   - short command: rttstatg

   \param DSL_RTT_Statistics_t*
      The parameter points to a \ref DSL_RTT_Statistics_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - ADSL only platforms (XWAY(TM) ARX100, XWAY(TM) ARX300)

   \code
      DSL_RTT_Statistics_t RTT_Config;
      DSL_int_t ret = 0;

      memset(&RTT_Config, 0x00, sizeof(DSL_RTT_Config_t));
      // Set configuration parameter
      RTT_Config = ;
      ret = ioctl(fd, DSL_FIO_RTT_CONFIG, &RTT_Config);
      // Check return value and do exception handling if necessary
   \endcode

   \ingroup DRV_DSL_CPE_DEBUG_RTT */
#define DSL_FIO_RTT_STATISTICS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_RTT, 5, DSL_RTT_Statistics_t)

#endif /* INCLUDE_REAL_TIME_TRACE */

#ifdef INCLUDE_DSL_CPE_API_VRX
/**
   This function writes the Low Power Mode configuration settings.

   CLI
   - long command: G997_LowPowerModeConfigSet
   - short command: g997lpmcs

   \param DSL_G997_LowPowerModeConfig_t*
      The parameter points to a \ref DSL_G997_LowPowerModeConfig_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - XWAY(TM) VRX200: xDSL-CPE
   - XWAY(TM) VRX300: xDSL-CPE

   \code
      DSL_G997_LowPowerModeConfig_t lpmCfgSet;
      DSL_int_t ret = 0;

      memset(&lpmCfgSet, 0, sizeof(DSL_G997_LowPowerModeConfig_t));
      // enable the autonomous L2 transition
      lpmCfgSet.data.PMMode = DSL_G997_PMMODE_BIT_L2_STATE;
      ret = ioctl(fd, DSL_FIO_G997_LOW_POWER_MODE_CONFIG_SET, &lpmCfgSet);
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_LOW_POWER_MODE_CONFIG_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 39, DSL_G997_LowPowerModeConfig_t)

/**
   This function reads the Low Power Mode configuration settings.

   CLI
   - long command: G997_LowPowerModeConfigGet
   - short command: g997lpmcg

   \param DSL_G997_LowPowerModeConfig_t*
      The parameter points to a \ref DSL_G997_LowPowerModeConfig_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - XWAY(TM) VRX200: xDSL-CPE
   - XWAY(TM) VRX300: xDSL-CPE

   \code
      DSL_G997_LowPowerModeConfig_t lpmCfgGet;
      DSL_int_t ret = 0;

      memset(&lpmCfgGet, 0, sizeof(DSL_G997_LowPowerModeConfig_t));
      ret = ioctl(fd, DSL_FIO_G997_LOW_POWER_MODE_CONFIG_GET, &lpmCfgGet);
      // lpmCfgGet includes the current low power mode configuration
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_LOW_POWER_MODE_CONFIG_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API_G997, 40, DSL_G997_LowPowerModeConfig_t)
#endif /* INCLUDE_DSL_CPE_API_VRX*/

#endif /* DSL_CPE_API_USE_KERNEL_INTERFACE */

#ifdef __cplusplus
}
#endif

#endif /* _DRV_DSL_CPE_API_IOCTL_H */
