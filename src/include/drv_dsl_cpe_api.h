/******************************************************************************

                               Copyright (c) 2011
                            Lantiq Deutschland GmbH
                     Am Campeon 3; 85579 Neubiberg, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef _DRV_DSL_CPE_API_H
#define _DRV_DSL_CPE_API_H

#ifdef __cplusplus
   extern "C" {
#endif

/*
   For doxygen generation only!
   Take care to avoid excluding any code sections if not used for generating of
   documentation.
*/
#ifndef SWIG
#undef DSL_DOC_GENERATION_EXCLUDE_ADSL_MIB
#endif

#ifndef _lint
#ifdef WIN32
#include "dsl_cpe_config_win32.h"
#else
#ifdef DSL_INTERN

#include "drv_dsl_cpe_api_config.h"

#endif /* #ifdef DSL_INTERN*/
#endif /* WIN32*/
#endif /*#ifndef _lint*/

#ifndef DSL_CPE_API_PACKAGE_VERSION
#define DSL_CPE_API_PACKAGE_VERSION  PACKAGE_VERSION
#endif

#include "drv_dsl_cpe_api_types.h"
#include "drv_dsl_cpe_api_error.h"

/*#define HAS_TO_BE_TESTED*/

#define DSL_MAX_TIMEOUT_NUM   2

/** \todo decide whether this mechanism has to be configurable or should be
          always included */
#define INCLUDE_CALLBACK_SUPPORT

/** \file drv_dsl_cpe_api.h
   DSL CPE API interface file for user applications.
*/

/** \defgroup DRV_DSL_CPE_API Lantiq DSL CPE API
    Lists the entire modules to the DSL CPE_API.
*/

/**
   \defgroup DRV_DSL_CPE_INIT Initialization Service
   This module does the default initialization of DSL devices including firmware
   download.
   \ingroup DRV_DSL_CPE_API
*/

/**
   \defgroup DRV_DSL_CPE_COMMON Common Functionality for all Modules
   This module includes all DSL library functionality which does not belong to
   any other module.
   \ingroup DRV_DSL_CPE_API
*/

/**
   \defgroup DRV_DSL_CPE_G997 Implementation of ITU standard G.997.1
   This module implements the Physical Layer OAM specification G.PLOAM .
   \ingroup DRV_DSL_CPE_API
*/

/**
   \defgroup DRV_DSL_CPE_PM Implementation of Performance Monitoring of ITU standard
   G.997.1.
   This module implements the Performance Monitoring specification G.PLOAM .
   \ingroup DRV_DSL_CPE_API
*/

#ifdef INCLUDE_DSL_CPE_API_VDSL_SUPPORT
#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
#if defined(INCLUDE_DSL_CPE_API_VINAX)
/**
   \defgroup DRV_DSL_CPE_SAR Implementation of Segmentation and Reassembly API
   This module contains the API of the Segmentation and Reassembly functionality,
   also called interworking between packet- and cell-oriented data interfaces.
   \ingroup DRV_DSL_CPE_API
*/

/**
   \defgroup DRV_DSL_CPE_BND Implementation of Bonding functions.
   This module contains the API of the Bonding functionality.
   \ingroup DRV_DSL_CPE_API
*/

#endif /* defined(INCLUDE_DSL_CPE_API_VINAX) */
#endif
#endif

#ifdef INCLUDE_REAL_TIME_TRACE
/**
   \defgroup DRV_DSL_CPE_DEBUG_RTT Implementation of Real Time Trace (RTT) functions.
   This module contains the API of the Real Time Trace functionality.
   \ingroup DRV_DSL_CPE_API
*/

#endif /* #ifdef INCLUDE_REAL_TIME_TRACE */

/**
   \defgroup DRV_DSL_CPE_DEBUG Debug Code
   Includes all debug related coding
   \ingroup DRV_DSL_CPE_API
*/

/**
   \defgroup DRV_DSL_CPE_EVENT Event Implementation
   Includes all definitions and functions used for the event handling
   (autonomous messages) and a detailed description of event
   implementation/handling.
   \ingroup DRV_DSL_CPE_API
*/

/**
   \defgroup DRV_DSL_CPE_ERROR_CODES Error Codes
   Defines all possible error codes returned by the DSL API library
   \ingroup DRV_DSL_CPE_API
*/

/**
   \defgroup DRV_DSL_CPE_TYPES Basic Data Types
   Basic data types.
   \ingroup DRV_DSL_CPE_API
*/

#ifdef INCLUDE_DSL_CPE_API_VDSL_SUPPORT
#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   \defgroup DRV_DSL_DEVICE Device specific implementation
   Includes all device specific implementations
   \ingroup DRV_DSL_CPE_API
*/
#endif
#endif

/**
   This define specifies to use the exported interface functions instead of
   the ioctl interface. This is only necessary if the access should be done
   from within another driver module within the kernel space.
   The default configuration is to undefine this and use the driver from an
   user space application by ioctl.
   \attention This is included at this place temporarily because it shall be
              done by configure process later on.
*/
/*#define DSL_CPE_API_USE_KERNEL_INTERFACE*/
#undef DSL_CPE_API_USE_KERNEL_INTERFACE

/**
   This define specifies the maximum number of device instances
*/
#if defined (INCLUDE_DSL_CPE_API_VINAX) || defined (INCLUDE_DSL_CPE_API_VRX)
#if !defined(DSL_DRV_MAX_DEVICE_NUMBER)
   #define DSL_DRV_MAX_DEVICE_NUMBER 1
#elif (DSL_DRV_MAX_DEVICE_NUMBER == 0)
   #error "DSL_DRV_MAX_DEVICE_NUMBER should be greater than 0!!!"
#endif /* !defined(DSL_DRV_MAX_DEVICE_NUMBER)*/
#elif defined (INCLUDE_DSL_CPE_API_DANUBE)
#ifdef DSL_DRV_MAX_DEVICE_NUMBER
   #if (DSL_DRV_MAX_DEVICE_NUMBER > 1) || (DSL_DRV_MAX_DEVICE_NUMBER == 0)
   #error "DSL_DRV_MAX_DEVICE_NUMBER should be 1!!!"
   #endif
#else
   /* Danube low level driver support only single instance */
   #define DSL_DRV_MAX_DEVICE_NUMBER 1
#endif /* DSL_DRV_MAX_DEVICE_NUMBER*/
#endif /* defined (INCLUDE_DSL_CPE_API_VINAX) || defined (INCLUDE_DSL_CPE_API_VRX)*/

/**
   This define specifies the number of event elements in the FIFO
   \attention This is included at this place temporarily because it shall be
              done by configure process later on.
*/
#define DSL_EVENT_FIFO_ELEMENT_COUNT 8

/**
   If number of used bearer channels is not set choose default value of '1'
*/
#ifndef DSL_CHANNELS_PER_LINE
   #define DSL_CHANNELS_PER_LINE 1
      #ifdef _MSC_VER
         #pragma message("WARNING: DSL_CHANNELS_PER_LINE not specified, chossing default '1'")
      #else
         #warning "WARNING: DSL_CHANNELS_PER_LINE not specified, chossing default '1'"
      #endif
#else
   #if (DSL_CHANNELS_PER_LINE > 2)
      #error "ERROR: DSL_CHANNELS_PER_LINE has to be in range of '0..1'"
   #elif (DSL_CHANNELS_PER_LINE > 1)
      #error "DSL_CHANNELS_PER_LINE > 1 feature not released yet!!!"
   #endif
#endif

#if defined (INCLUDE_DSL_CPE_API_DANUBE)
   #define DSL_MAX_NUMBER_OF_BANDS    1
#elif defined (INCLUDE_DSL_CPE_API_VINAX) || defined (INCLUDE_DSL_CPE_API_VRX)
   #define DSL_MAX_NUMBER_OF_BANDS    9
#else
   #error "Device is not defined!"
#endif

#ifdef INCLUDE_DEPRECATED
   #ifdef INCLUDE_DSL_CPE_PM_LINE_FAILURE_COUNTERS
      #ifndef INCLUDE_DSL_CPE_PM_LINE_EVENT_SHOWTIME_COUNTERS
         #define INCLUDE_DSL_CPE_PM_LINE_EVENT_SHOWTIME_COUNTERS
      #endif
   #endif /* INCLUDE_DSL_CPE_PM_LINE_FAILURE_COUNTERS*/
#else
   #ifdef INCLUDE_DSL_CPE_PM_LINE_FAILURE_COUNTERS
      #error "PM Line Failure counters are deprecated! Please use --enable-deprecated configure option"
   #endif /* INCLUDE_DSL_CPE_PM_LINE_FAILURE_COUNTERS*/

   #ifdef INCLUDE_ADSL_LED
      #error "LED module is deprecated! Please use --enable-deprecated configure option"
   #endif /* INCLUDE_ADSL_LED*/
#endif /* INCLUDE_DEPRECATED */

/** \addtogroup DRV_DSL_CPE_G997
 @{ */

/** Maximum length of xTU System Enabling Octets */
#define DSL_G997_NUM_XTSE_OCTETS       8

/** Maximum DRV_DSL_CPE_INIT length of line inventory VendorID */
#define DSL_G997_LI_MAXLEN_VENDOR_ID   8
/** Maximum length of line inventory Version Number */
#define DSL_G997_LI_MAXLEN_VERSION     16
/** Maximum length of line inventory Serial Number */
#define DSL_G997_LI_MAXLEN_SERIAL      32
/** Maximum length of auxiliary inventory information */
#define DSL_G993_LI_MAXLEN_AUX         64
/** @} DRV_DSL_CPE_G997 */

/** \addtogroup DRV_DSL_CPE_COMMON
 @{ */

#define DSL_MAC_ADDRESS_OCTETS   6

#ifdef INCLUDE_DSL_CPE_API_DANUBE
   #if defined(INCLUDE_DEPRECATED) && defined(INCLUDE_ADSL_LED)
   /* Internal definition. Move to configure process */
   /*#define INCLUDE_DSL_DATA_LED_SIMULATOR*/
   #endif /* defined(INCLUDE_DEPRECATED) && defined(INCLUDE_ADSL_LED)*/
#endif /* INCLUDE_DSL_CPE_API_DANUBE*/

/**
   The driver context contains global information.
*/
typedef struct DSL_Context DSL_Context_t;

#ifdef DSL_INTERN
   /* Operating-system related stuff */
   #include "drv_dsl_cpe_os.h"
   #include "drv_dsl_cpe_timeout.h"
#endif

/* define the supported standards */
#ifdef INCLUDE_DSL_CPE_API_DANUBE
   #define INCLUDE_DSL_CPE_API_ADSL_SUPPORT         1
   #define INCLUDE_DSL_CPE_API_VDSL_SUPPORT         0
#endif

#ifdef INCLUDE_DSL_CPE_API_VINAX
   #define INCLUDE_DSL_CPE_API_ADSL_SUPPORT         1
   #define INCLUDE_DSL_CPE_API_VDSL_SUPPORT         1
#endif

#ifdef INCLUDE_DSL_CPE_API_VRX
   #define INCLUDE_DSL_CPE_API_ADSL_SUPPORT         1
   #define INCLUDE_DSL_CPE_API_VDSL_SUPPORT         1
#endif

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
   /** Defines the maximum number of carrier bands in one direction */
   #define DSL_G997_MAX_NUMBER_OF_BANDS         5
#else
   /** Defines the maximum number of carrier bands in one direction */
   #define DSL_G997_MAX_NUMBER_OF_BANDS         1
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */


#ifndef DSL_MAX_NSC
   #if defined (INCLUDE_DSL_CPE_API_DANUBE)
      /** maximum number of data elements for subcarrier related data */
      #define DSL_MAX_NSC           512
   #elif defined (INCLUDE_DSL_CPE_API_VINAX) || defined (INCLUDE_DSL_CPE_API_VRX)
      /** maximum number of data elements for subcarrier related data */
      #define DSL_MAX_NSC           4096
      /** maximum number of data elements for subcarrier group related data */
      #define VNX_MAX_SCGROUPS      512
   #else
      #error "Device is not defined!"
   #endif
#endif /* #ifndef DSL_MAX_NSC */


/** Maximal length of version information strings */
#define MAX_INFO_STRING_LEN            32

/** G994 Vendor ID constant */
#define DSL_G994_VENDOR_ID 0xB5, 0x00, 0x49, 0x46, 0x54, 0x4E, 0x00, 0x00

/** @} DRV_DSL_CPE_COMMON */

/** \addtogroup DRV_DSL_CPE_DEBUG
 @{ */

/** no output */
#define DSL_DBGLVL_NONE    0x00
/** used for common (statistics) information that will be always printed
    in case of compiled-in and activated debug output. */
#define DSL_DBGLVL_PRN     0x01
/** errors will be printed */
#define DSL_DBGLVL_ERR     0x02
/** warnings and errors are printed */
#define DSL_DBGLVL_WRN     0x40
/** messages, warnings and errors are printed */
#define DSL_DBGLVL_MSG     0x80
/** verbose output (low level debug) */
#define DSL_DBGLVL_LOCAL   0xFF

/** Definitions for defining the debug level.
    The smaller the value the less debug output will be printed
*/
typedef enum
{
   /** no output */
   DSL_DBG_NONE = DSL_DBGLVL_NONE,
   /** used for common (statistics) information that will be always printed
       in case of compiled-in and activated debug output. */
   DSL_DBG_PRN = DSL_DBGLVL_PRN,
   /** errors will be printed */
   DSL_DBG_ERR = DSL_DBGLVL_ERR,
   /** warnings and errors are printed */
   DSL_DBG_WRN = DSL_DBGLVL_WRN,
   /** messages, warnings and errors are printed */
   DSL_DBG_MSG = DSL_DBGLVL_MSG,
   /** verbose output (low level debug) */
   DSL_DBG_LOCAL = DSL_DBGLVL_LOCAL
} DSL_debugLevels_t;

/**
   Defines all available debug modules.
*/
typedef enum
{
   /** no block selected.
       In case of using this values within \ref DSL_FIO_DBG_MODULE_LEVEL_SET
       it will set the debug levels of *all* modules as defined. */
   DSL_DBG_NO_BLOCK = 0,
   /** API function block */
   DSL_DBG_CPE_API = 1,
   /** Internal function block */
   DSL_DBG_G997 = 2,
   /** Performance monitoring */
   DSL_DBG_PM = 3,
   /** MIB specific block */
   DSL_DBG_MIB = 4,
   /** CEOC specific block */
   DSL_DBG_CEOC = 5,
   /** LED specific block */
   DSL_DBG_LED = 6,
   /** SAR specific block */
   DSL_DBG_SAR = 7,
   /** Device specific block */
   DSL_DBG_DEVICE = 8,
   /** Control thread */
   DSL_DBG_AUTOBOOT_THREAD = 9,
   /** Operating system block */
   DSL_DBG_OS = 10,
   /** Callback event handling */
   DSL_DBG_CALLBACK = 11,
   /** Message dump enable. */
   DSL_DBG_MESSAGE_DUMP = 12,
   /** Driver debug level */
   DSL_DBG_LOW_LEVEL_DRIVER = 13,
   /** Multimode debug messages */
   DSL_DBG_MULTIMODE = 14,
   /** Last entry for debug blocks - only used as delimiter! */
   DSL_DBG_LAST_BLOCK
} DSL_debugModules_t;

/** @} DRV_DSL_CPE_DEBUG */

#if defined(DSL_INTERN)
/* include DSL CPE API debug header */
#include "drv_dsl_cpe_debug.h"
#endif

/** \addtogroup DRV_DSL_CPE_COMMON
 @{ */

/**
   Empty define used as information direction hint.
   Defines if a parameter has been used as an input parameter */
#ifndef SWIG
#define DSL_IN
#else
#define DSL_IN const
#endif /* SWIG */

/**
   Empty define used as information direction hint
   Defines if a parameter has been used as an output parameter */
#define DSL_OUT

/**
   Empty define used as information direction hint
   Defines if a parameter has been used as a reference which means it is used
   as an input and output parameter */
#define DSL_IN_OUT

/**
   Empty define used as information direction hint
   Defines if a parameter has been used whether as an output or an input
   parameter according to the driver access function as follows
   - ConfigSet -> input
   - ConfigGet -> output
   - StatusGet -> input */
#define DSL_CFG


/** Definition for special value of (bearer) channel if the value is not
    applicable for the actual context handling.
    \remarks Channel definition in API functions is always DSL_uint8_t */
#define DSL_CHANNEL_NA     0xFF


/**
   Defines the direction for which a function call should apply.
*/
typedef enum
{
   /** Direction information not applicable */
   DSL_ACCESSDIR_NA = -1,
   /** Upstream access mode. This is the direction xTU-R -> xTU-C.
      It depends on the desired information whether this means a
      near end or far end access. */
   DSL_UPSTREAM = 0,
   /** Downstream access mode. This is the direction xTU-C -> xTU-R.
      It depends on the desired information whether this means a
      near end or far end access. */
   DSL_DOWNSTREAM = 1,
   /* Delimiter only */
   DSL_ACCESSDIR_LAST
} DSL_AccessDir_t;


/**
   Defines the DELT data type for which a function call should apply.
*/
typedef enum
{
   /**
      DELT diagnostics data access mode. */
   DSL_DELT_DATA_DIAGNOSTICS = 0,
   /**
      DELT diagnostics data access mode. */
   DSL_DELT_DATA_SHOWTIME = 1,
   /* Delimiter only */
   DSL_DELT_DATA_LAST
} DSL_DeltDataType_t;


/**
   Defines the point of view for which a parameter should apply.
*/
typedef enum
{
   /** Direction information not applicable */
   DSL_XTUDIR_NA = -1,
   /** Near-End access mode. This is always the local site. */
   DSL_NEAR_END = 0,
   /** Far-End access mode. This is the foreign connection
      partner of the line. */
   DSL_FAR_END = 1
} DSL_XTUDir_t;


/**
   Defines all possible line init states of the device.
   dependant on the used device some states might not be used.
*/
typedef enum
{
   /* ******************************** */
   /* *** Common line states !     *** */
   /* ******************************** */

   /** Line State is not initialized! */
   DSL_LINESTATE_NOT_INITIALIZED   = 0x00000000,

   /** Line State: EXCEPTION.
       corresponds to the VINAX device state FAIL_STATE */
   DSL_LINESTATE_EXCEPTION         = 0x00000001,

   /** Line State: NOT_UPDATED.
       internal line state that indicates stopped autoboot thread */
   DSL_LINESTATE_NOT_UPDATED       = 0x00000010,

   /** Line State: IDLE_REQUEST.
       Interim state between deactivation of line and the time this user request
       is acknowledged by the firmware. */
   DSL_LINESTATE_IDLE_REQUEST      = 0x000000FF,
   /** Line State: IDLE.
       Corresponds to the following device specific states
       - Danube Family: Modem Status RESET
       - VINAX: Device state RESET_STATE */
   DSL_LINESTATE_IDLE              = 0x00000100,
   /** Line State: SILENT_REQUEST.
       Interim state between activation of line and the time this user request
       is acknowledged by the firmware. */
   DSL_LINESTATE_SILENT_REQUEST    = 0x000001FF,
   /** Line State: SILENT.
       Corresponds to the following device specific states
       - Danube Family: Modem status READY
       - VINAX: Device state READY */
   DSL_LINESTATE_SILENT            = 0x00000200,
   /** Line State: HANDSHAKE.
       Corresponds to the following device specific states
       - Danube Family: Modem status GHS
       - VINAX: Device state GHS */
   DSL_LINESTATE_HANDSHAKE         = 0x00000300,
   /** Line State: HANDSHAKE.
       Corresponds to the following device specific states
       - VINAX: Device state BONDING_CLR
       - Danube Family: Not supported */
   DSL_LINESTATE_BONDING_CLR       = 0x00000310,
   /** Line State: T1413.
       Entered upon detection of a far-end ANSI T1.413 signal. */
   DSL_LINESTATE_T1413             = 0x00000370,
   /** Line State: FULL_INIT.
       - Danube Family: Modem status FULL_INIT
       - VINAX: Device state FULL_INIT */
   DSL_LINESTATE_FULL_INIT         = 0x00000380,
   /** Line State: SHORT INIT
       not supported by VINAX*/
   DSL_LINESTATE_SHORT_INIT_ENTRY  = 0x000003C0,
   /** Line State: DISCOVERY.
       for Danube Family and VINAX this is a substate of FULL_INIT and is not
       reported */
   DSL_LINESTATE_DISCOVERY         = 0x00000400,
   /** Line State: TRAINING.
       for Danube Family and VINAX this is a substate of FULL_INIT and is not
       reported */
   DSL_LINESTATE_TRAINING          = 0x00000500,
   /** Line State: ANALYSIS.
       for Danube Family and VINAX this is a substate of FULL_INIT and is not
       reported */
   DSL_LINESTATE_ANALYSIS          = 0x00000600,
   /** Line State: EXCHANGE.
       for Danube Family and VINAX this is a substate of FULL_INIT and is not
       reported */
   DSL_LINESTATE_EXCHANGE          = 0x00000700,
   /** Line State: SHOWTIME.
       for Danube Family and VINAX this is a substate of FULL_INIT and is not
       reported */
   DSL_LINESTATE_SHOWTIME_NO_SYNC  = 0x00000800,
   /** Line State: SHOWTIME_TC_SYNC.
       for Danube Family and VINAX this is a substate of FULL_INIT and is not
       reported */
   DSL_LINESTATE_SHOWTIME_TC_SYNC  = 0x00000801,
   /** Line State: FASTRETRAIN.
       Currently not supported by Danube Family and VINAX. */
   DSL_LINESTATE_FASTRETRAIN       = 0x00000900,
   /** Line State: LOWPOWER_L2.
       Not supported by VINAX */
   DSL_LINESTATE_LOWPOWER_L2       = 0x00000A00,
   /** Line State: DIAGNOSTIC ACTIVE. */
   DSL_LINESTATE_LOOPDIAGNOSTIC_ACTIVE   = 0x00000B00,
   /** Line State: DIAGNOSTIC_DATA_EXCHANGE. */
   DSL_LINESTATE_LOOPDIAGNOSTIC_DATA_EXCHANGE = 0x00000B10,
   /** This status is used if the DELT data is already available within the
       firmware but has to be updated within the DSL API data elements. If
       the line is within this state the data within a DELT element is NOT
       consistent and shall be NOT read out by the upper layer software.
       \note This state is currently used for the GEMINAX-D device only */
   DSL_LINESTATE_LOOPDIAGNOSTIC_DATA_REQUEST = 0x00000B20,
   /** Line State: DIAGNOSTIC COMPLETE */
   DSL_LINESTATE_LOOPDIAGNOSTIC_COMPLETE = 0x00000C00,
   /** Line State: RESYNC
       not supported by VINAX */
   DSL_LINESTATE_RESYNC            = 0x00000D00,

   /* *********************************************************************** */
   /* *** Line States that may bundle various sates that are not handled  *** */
   /* *** in detail at the moment                                         *** */
   /* *********************************************************************** */

   /** Line State: TEST.
       Common test status may include various test states */
   DSL_LINESTATE_TEST              = 0x01000000,
   /** Line State: any loop activated. */
   DSL_LINESTATE_TEST_LOOP         = 0x01000001,

   /** Line State: TEST_REVERB. */
   DSL_LINESTATE_TEST_REVERB       = 0x01000010,
   /** Line State: TEST_MEDLEY. */
   DSL_LINESTATE_TEST_MEDLEY       = 0x01000020,
   /** Line State: TEST_SHOWTIME_LOCK. */
   DSL_LINESTATE_TEST_SHOWTIME_LOCK= 0x01000030,
   /** Line State: TEST_QUIET. */
   DSL_LINESTATE_TEST_QUIET        = 0x01000040,
   /** Line State: FILTERDETECTION_ACTIVE.
       Not supported by VINAX and VRx*/
   DSL_LINESTATE_TEST_FILTERDETECTION_ACTIVE = 0x01000050,
   /** Line State: FILTERDETECTION_COMPLETE.
       Not supported by VINAX and VRx*/
   DSL_LINESTATE_TEST_FILTERDETECTION_COMPLETE = 0x01000060,

   /** Line State: LOWPOWER_L3.
       Not supported by VINAX */
   DSL_LINESTATE_LOWPOWER_L3       = 0x02000000,
   /** Line State: All line states that are not assigned at the moment */
   DSL_LINESTATE_UNKNOWN           = 0x03000000

} DSL_LineStateValue_t;

/**
   This structure handles control and/or management information for common
   driver access.
*/
typedef struct
{
   /**
   Returns the DSL CPE_API driver internal error/return code according to the
   definition of \ref DSL_Error_t */
   DSL_OUT DSL_Error_t nReturn;
} DSL_AccessCtl_t;

/**
   Defines the possible hybrid types.

   \note The hybrid type is derived from a hardware related coding which is
         processed by the firmware and forwarded via a firmware message to the
         API. This handling requires that the hardware related functionality is
         foreseen in every hybrid design!
         - This functionality is only available on VRX platforms.
         - For Danube Family platforms refer to the low level configuration
           instead (drv_dsl_cpe_danube_ctx.h:DSL_DEV_Hybrid_t()).
         - For Vinax platform please refer to the low level configuration
           instead (drv_dsl_cpe_vinax_ctx.h:DSL_DEV_Hybrid_t()).
*/
typedef enum
{
   /**
   Hybrid information is not available.
   This state should usually not occur and it points to a problem within
   context of hardware related hybrid detection. */
   DSL_HYBRID_TYPE_UNKNOWN = -1,
   /**
   Hybrid information is not applicable on the hardware platform that is
   currently used. This is the default value for Danube, Amazon-SE, ARX100
   and Vinax. */
   DSL_HYBRID_TYPE_NA = 0,
   /**
   POTS hybrid detected. */
   DSL_HYBRID_TYPE_POTS = 1,
   /**
   ISDN hybrid detected. */
   DSL_HYBRID_TYPE_ISDN = 2,
   /**
   Delimiter only! */
   DSL_HYBRID_TYPE_LAST = 3
} DSL_HybridType_t;

/**
   This structure includes information about different DSL PHY Subsystem related
   hardware, software and firmware versions respective informations.
*/
typedef struct
{
   /**
   DSL CPE_API driver version. This is the version of the DSL Driver related
   layer which provides G.997.1 and ADSL/VDSL MIB functionality.

   Consists of major.minor.build.relchar:
   - major version (major): logical number, incremented for new versions.
   - minor version (minor): logical number, incremented for new minor changes
     in versions.
   - build step (build): logical number, incremented for each new version build.
   - optional release character (relchar): character to distinguish minor
     functional changes
     or special build for one version (starting with "a", "b", "c" etc.) */
   DSL_OUT DSL_char_t DSL_DriverVersionApi[MAX_INFO_STRING_LEN];

   /**
   Firmware version.

   Consists of
   hw_platform.feature_set.major_ver.minor_ver.rel_indication.application:

   hardware platform (hw_platform)
   - 0x01: Amazon
   - 0x02: Danube
   - 0x03: Amazon-SE
   - 0x04: ARX100
   - 0x05: VRX200
   - 0x09: VINAX1X (Vinax Revision 1.1 - 1.3)

   feature set (feature_set)
   - indicates the major feature set implementation for firmware (please refer
     to firmware documentation and/or contact Lantiq for detailed informations).

   major version (major_ver)
   - logical number (incremented for major updates of firmware)

   minor version (minor_ver)
   - logical number (incremented for minor updates of firmware)

   release indication (rel_indication)
   - 0x00: RELEASE_STATUS_RELEASED, The firmware has been tested and released.
   - 0x01: RELEASE_STATUS_PRE_RELEASE, The firmware is undergoing the release
           process; the release has not been completed.
   - 0x02: DEVELOP Development version

   application type (application) also includes information about the annex
   type. The following application types are defined.
   For Danube Family (Danube/Amazon-SE/ARX100) and VRX Family
   - 0x01: ADSL Annex A
   - 0x02: ADSL Annex B
   - 0x06: VDSL
   For Vinax
   - 0x02: VDSL1_2 FW for VDSL1 + VDSL2 (CO + CPE with-host)
   - 0x05: ADSL_A FW for ADSL Annex A only (CO + CPE with-host)
   - 0x06: ADSL_B FW for ADSL Annex B only (CO + CPE with-host)
   \attention The list of application types for the Vinax only includes the
              important CPE related values. For further details please refer to
              firmware documentation (Firmware Programmers Reference) and/or
              contact Lantiq. */
   DSL_OUT DSL_char_t DSL_ChipSetFWVersion[MAX_INFO_STRING_LEN];

   /**
   Device chip version */
   DSL_OUT DSL_char_t DSL_ChipSetHWVersion[MAX_INFO_STRING_LEN];

   /**
   Chip set type string which specifies the chip name.

   The following names are defined
   - Lantiq_Danube
   - Lantiq_Amazon      (not supported)
   - Lantiq_Amazon_SE
   - Lantiq_ARX100
   - Lantiq_Vinax
   - Lantiq_VRX200 */
   DSL_OUT DSL_char_t DSL_ChipSetType[MAX_INFO_STRING_LEN];

   /**
   MEI BSP Driver version. This is the version of the low level driver
   which provides basic access to the according device/firmware interface.

   Consists of major.minor.build.rel:
   - major version (major): logical number, incremented for new versions
   - minor version (minor): logical number, incremented for minor changes in versions
   - build step (build): logical number, incremented for each new version build
   - optional release indictor (rel): specifies different Lantiq internal indications */
   DSL_OUT DSL_char_t DSL_DriverVersionMeiBsp[MAX_INFO_STRING_LEN];

   /**
   Hybrid type.

   \attention This value is only available for VRX family platform. */
   DSL_OUT DSL_HybridType_t nHybridType;
} DSL_VersionInformationData_t;

/**
   This structure has to be used for ioctl \ref DSL_FIO_VERSION_INFORMATION_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains all necessary version information data */
   DSL_OUT DSL_VersionInformationData_t data;
} DSL_VersionInformation_t;

/**
   Defines the the possible types of firmware requests.
*/
typedef enum
{
   /**
   No dedicated information available for firmware request type.
   \note This values should be never given with event by the driver itself.
         If set, it normally indicates a problem within the driver. */
   DSL_FW_REQUEST_NA = 0,
   /**
   Firmware request is related to an autoboot reboot processing that will start
   in ADSL after successful firmware download.
   \note For Danube, Amazon-SE and ARX100 this will be the only value that is
         signaled. */
   DSL_FW_REQUEST_ADSL = 1,
   /**
   Firmware request is related to an autoboot reboot processing that will start
   in VDSL after successful firmware download.
   \note This value is only available within Vinax versions of DSL CPE API. */
   DSL_FW_REQUEST_VDSL = 2,
   /**
   Firmware request is related to an autoboot reboot processing for the VRX
   platform that will start in an xDSL mode that is supported by the provided
   firmware binary.
   \note This value is only available within VRX versions of DSL CPE API. */
   DSL_FW_REQUEST_XDSL = 3,
   /**
   Delimiter only! */
   DSL_FW_REQUEST_LAST = 4
} DSL_FirmwareRequestType_t;

/**
   Defines the the possible types of the downloaded firmware.
*/
typedef enum
{
   /**
   No dedicated information available for the firmware status.
   \note This values should be never given with event by the driver itself.
         If set, it normally indicates a problem within the driver. */
   DSL_FW_STATUS_NA = 0,
   /**
   Firmware status indicates ADSL successful firmware download.
   \note For Danube, Amazon-SE and ARX100 this will be the only value that is
         signaled. */
   DSL_FW_STATUS_ADSL = 1,
   /**
   Firmware status indicates VDSL successful firmware download.
   \note This value is only available within Vinax and VRX versions
         of DSL CPE API. */
   DSL_FW_STATUS_VDSL = 2,
   /**
   Delimiter only! */
   DSL_FW_STATUS_LAST = 3
} DSL_FirmwareStatusType_t;

/**
   Structure to return the current type of firmware request.
   It defines which kind of firmware is needed for the next reboot sequence.
*/
typedef struct
{
   /**
   Current line state. */
   DSL_OUT DSL_FirmwareRequestType_t nFirmwareRequestType;
} DSL_FirmwareRequestData_t;

/**
   Structure to return the current line state (according to appropriate standard,
   e.g. G.992.3)
*/
typedef struct
{
   /**
   Current line state. */
   DSL_OUT DSL_LineStateValue_t nLineState;
} DSL_LineStateData_t;

/**
   Indicates the success/failure status of a previous firmware download.
*/
typedef enum
{
   /**
   Firmware download was finished successfully. */
   DSL_FW_LOAD_SUCCESS = 0,
   /**
   There was an error during firmware download. */
   DSL_FW_LOAD_ERROR = 1
} DSL_FwError_t;

/**
   This structure is used for the event
   DSL_EventType_t::DSL_EVENT_S_FIRMWARE_DOWNLOAD_STATUS that indicates the
   success/failure status of a previous firmware download.
*/
typedef struct
{
   /* Error code*/
   DSL_FwError_t nError;
   /* Indicates which FW request was handled*/
   DSL_FirmwareStatusType_t nFwModeStatus;
} DSL_FwDownloadStatusData_t;

/**
   Defines the control functionalities of the autoboot handling.
*/
typedef enum
{
   /**
   Autoboot handling is currently stopped. */
   DSL_AUTOBOOT_STATUS_STOPPED = 0,
   /**
   Autoboot handling is currently started. */
   DSL_AUTOBOOT_STATUS_STARTING = 1,
   /**
   Autobooot handling is currently running. */
   DSL_AUTOBOOT_STATUS_RUNNING = 2,
   /**
   Autobooot handling is currently waiting for the FW. */
   DSL_AUTOBOOT_STATUS_FW_WAIT = 3,
   /**
   Autobooot handling is currently waiting for writing the configurations. */
   DSL_AUTOBOOT_STATUS_CONFIG_WRITE_WAIT = 4,
   /**
   Autobooot handling is currently waiting for the link activation. */
   DSL_AUTOBOOT_STATUS_LINK_ACTIVATE_WAIT = 5,
   /**
   Autobooot handling is currently waiting for restart. */
   DSL_AUTOBOOT_STATUS_RESTART_WAIT = 6,
   /**
   Delimiter only */
   DSL_AUTOBOOT_STATUS_LAST = 7
   /* Might be extended if necessary */
} DSL_AutobootStatGet_t;

/**
   Structure used for controlling the autoboot functionality.
   \remark This handling is necessary in any case if not supported by firmware
           automatically.
*/
typedef struct
{
   /**
   Command for autoboot handling. */
   DSL_IN_OUT DSL_AutobootStatGet_t nStatus;
   /**
   This value is only valid in case of nStatus is equal to
   \ref DSL_AUTOBOOT_STATUS_FW_WAIT. In this case it specifies the desired
   mode for the next startup of autoboot handling.
   Otherwise the value shall be always equal to \ref DSL_FW_REQUEST_NA */
   DSL_IN_OUT DSL_FirmwareRequestType_t nFirmwareRequestType;
} DSL_AutobootStatusData_t;

/**
   Structure used for controlling the autoboot functionality.
   This structure has to be used for ioctl \ref DSL_FIO_INIT
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains autoboot control data */
   DSL_IN_OUT DSL_AutobootStatusData_t data;
} DSL_AutobootStatus_t;

/**
   This structure has to be used for ioctl \ref DSL_FIO_LINE_STATE_GET.
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains all necessary status data */
   DSL_OUT DSL_LineStateData_t data;
} DSL_LineState_t;

/**
   Defines the direction for which a function call should apply.
*/
typedef enum
{
   /**
   Feature is not available respectively not applicable */
   DSL_FEATURE_NA = -1,
   /**
   Feature is enabled. */
   DSL_FEATURE_DISABLED = 0,
   /**
   Feature is disabled. */
   DSL_FEATURE_ENABLED = 1,
   /*
   Delimiter only */
   DSL_FEATURE_LAST
} DSL_FeatureSupport_t;

/**
   Structure to write (SET) or read (GET) the configuration or status for the
   lines features.
*/
typedef struct
{
   /**
   Trellis config/status value.

   Some common standard related notes about the Trellis configuration option
   - In case of ADSL1 Trellis could be only configured for both directions
     (US/DS) together. This is done by exchanging the information within
     handshake procedure between CO<->CPE, means both sides are indicating
     whether they support Trellis or not. If both sides supports it, it will be
     done without any additional condition. If one side does not support it, it
     will be NOT done.
   - In case of ADSL2/2+ and VDSL2 Trellis could be configured for both
     directions independently but the responsibility for each direction is
     assigned as follows
     - Downstream can be configured by ATU-R only
     - Upstream can be configured by ATU-C only

   Resulting handling of Trellis configuration within CPE API context:
   - Trellis configuration can be only changed for DS.
     In case of using ioctl \ref DSL_FIO_LINE_FEATURE_CONFIG_SET and nDirection
     equals DSL_UPSTREAM the configuration value for trellis is ignored.
   - In case of ADSL1 it depends on the CO configuration whether trellis is
     activated or not according to above description. This could lead to the
     effect that trellis(ds) is enabled but not used if status is requested
     via ioctl \ref DSL_FIO_LINE_FEATURE_STATUS_GET.
   - In case of ADSL2/2+ and VDSL2 the configuration value that is used for
     trellis(ds) should be directly reflected within according downstream status
     value provided by ioctl \ref DSL_FIO_LINE_FEATURE_STATUS_GET. */
   DSL_CFG DSL_boolean_t bTrellisEnable;
   /**
   Bitswap config/status value.

   \note In case of status request (using ioctl DSL_FIO_LINE_FEATURE_STATUS_GET)
         the bitswap enable status ALWAYS reflects the bitswap configuration
         value istelf.
    */
   DSL_CFG DSL_boolean_t bBitswapEnable;
   /**
   Retransmission (ReTx) config/status value.

   \note Currently the ReTx feature is only supported for downstream direction.
         Therefore configurations are only possible for downstream and will be
         ignored for upstream. Additionally a warning
         \ref DSL_WRN_CONFIG_PARAM_IGNORED is returned in that case.
   \note In case of using VRX200 the configuration is only valid in case of using
         VDSL link activation mode. */
   DSL_CFG DSL_boolean_t bReTxEnable;
   /**
   Virtual noise config/status value.

   \note The status value for virtual noise is directly derived by the
         configuration value (configuration value only) for the Danube device.
   \note For the Vinax device virtual noise config/status value is only valid in
         the VDSL mode.*/
   DSL_CFG DSL_boolean_t bVirtualNoiseSupport;
   /**
   20 bit constellation config/status value.
   - DSL_FALSE: 20 bit support is disabled (not indicated in G.Hs)
   - DSL_TRUE: 20 bit support is enabled (indicated in G.Hs)

   \note The configuration and status of this feature is only available for
         downstream direction. */
   DSL_CFG DSL_FeatureSupport_t b20BitSupport;
} DSL_LineFeatureData_t;

/**
   Structure used to configure line features.
   This structure has to be used for ioctl
   - \ref DSL_FIO_LINE_FEATURE_CONFIG_SET
   - \ref DSL_FIO_LINE_FEATURE_CONFIG_GET
   - \ref DSL_FIO_LINE_FEATURE_STATUS_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Specifies for which direction (upstream/downstream) the function will
   apply */
   DSL_IN DSL_AccessDir_t nDirection;
   /**
   Structure that contains all necessary configuration data */
   DSL_CFG DSL_LineFeatureData_t data;
} DSL_LineFeature_t;

/**
   Structure to return the framing parameter status values.
*/
typedef struct
{
   /**
   Ratio of Number of Mux Data Frames to Sync Octet (TP).
   The ratio of the number of mux data frames to the number of sync octets.
   Valid values are 1 .. 64.
   G993.2: The number of MDFs in an OH sub-frame;
   Tp = k Mp, where k is an integer. The value of Tp shall not exceed 64.
   This parameter is valid only for G.992.3/4/5 and G993.2. */
   DSL_OUT DSL_uint16_t nTp;
   /**
   Frames Per Codeword (S / MP).
   For 992.1 and 992.2, SM denotes the number of data-frames per DMTsymbol.
   This corresponds to parameter S. For the fast path, this
   parameter is always 1; for the interleaved path values from 0...16 are
   possible (0 for S = 1/2 only).
   For 992.3/4/5, SM denotes the number of mux data frames per
   codeword in the particular latency path P. This corresponds to
   parameter MP.
   This parameter is not valid for G.993.1.
   G993.2: The number of MDFs in an RS codeword. Only values of 1, 2, 4, 8,
   and 16 shall be supported.*/
   DSL_OUT DSL_uint16_t nMp;
   /**
   Length of the sync octet sequence of latency path function.
   Each latency path that is enabled carries an overhead channel to be
   transported in the sync octets. Generally, each overhead channel can contain
   a CRC portion, a bit oriented portion, and a message oriented portion over a
   repeating sequence of sync octets of length SEQp.
   This parameter is only defined for ADSL2 (G.992.3/4), ADSL2+ (G.992.5) and
   VDSL2 (G993.2).
   and might be used for implementation(s)regarding RFC2662
   (for example: adslAtucChanReceivedBlks). */
   DSL_OUT DSL_uint16_t nSEQ;
   /**
   Framing parameter nBp has a xDSL mode specific meaning.
   For G.992.1 and G.992.2 it represents the number of payload bytes per frame
   (raw frame size without overhead, without Reed-Solomon check bytes).
   According to table 6.3/G.992.1, the total data rate including RS-coding
   overhead for the fast path is (1 + Frame_Size + RS)  32 kbit/s
   For G.992.3, G.992.4, G.992.5 it represents the number of payload octets for
   this particular bearer channel and latency path. Corresponds to parameter
   BPN in G.992.3/4/5.
   For G993.2 it shows the number of octets from bearer channel n per MDF.
   The range of values is from 0 to 254. When Gp/Tp is not an integer, the
   number of octets from the bearer channel #0 varies between Bp0 and Bp0+1.
   In order to obtain the number of payload bytes per mux
   data frame, the parameter from all contributing bearer channels has to be
   added. */
   DSL_OUT DSL_uint16_t nBP;
   /**
   Number of Octets in Message Based Portion of Overhead Structure (MSGC).
   This parameter corresponds to MSGC in G.992.3 and G.992.4. It is
   undefined for G.992.1 and G.992.2 */
   DSL_OUT DSL_uint16_t nMSGC;
   /**
   This parameter defines the latency path which carries the message oriented
   portion of the overhead channel. This parameter has to be ignored for
   G.992.1 and G.992.2.
   The valid values are 0, 1, 2, and 3. */
   DSL_OUT DSL_uint16_t nMSGLP;
} DSL_FramingParameterStatusData_t;

/**
   Structure to return the actual status of various framing parameter.
   This structure has to be used for ioctl
   \ref DSL_FIO_FRAMING_PARAMETER_STATUS_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Specifies for which (bearer) channel the function will apply */
   DSL_IN DSL_uint8_t nChannel;
   /**
   Specifies for which direction (upstream/downstream) the function will
   apply */
   DSL_IN DSL_AccessDir_t nDirection;
   /**
   Structure that contains all necessary status data */
   DSL_OUT DSL_FramingParameterStatusData_t data;
} DSL_FramingParameterStatus_t;

/**
   Defines the testmodes that might be used for testing purpose.
*/
typedef enum
{
   /**
   Used as command: Disable all Test Modes.
   Used for status request: No testmode is currently active
   \attention When using this command to switch off any testmode that has been
              activated before the autoboot state machine will be reset and the
              firmware will be restarted! */
   DSL_TESTMODE_DISABLE = 0,
   /**
   The testmode 'showtime lock' avoids that the autoboot handling will reboot
   the firmware in special conditions such as
   - LOS has been occurred
   - Current SNR margin is below defined minimum value from CO.
   Activating this mode triggers the Autoboot restart sequence if the current
   test mode not equals to DSL_TESTMODE_SHOWTIME_LOCK. */
   DSL_TESTMODE_SHOWTIME_LOCK = 1,
   /**
   The testmode 'quiet' is used to completely switch off the TX signals for
   NearEnd and to avoid reboot as defined for testmode 'showtime lock'.
   Activating this mode triggers the Autoboot restart sequence.*/
   DSL_TESTMODE_QUIET = 2,
   /**
   The testmode 'training lock' is a special mode that will be used by debug
   tools. It includes lock functionality for
   - showtime state as defined within \ref DSL_TESTMODE_SHOWTIME_LOCK
   - training state - line state (\ref DSL_LineStateValue_t) <= 0x380.
   Activating this mode triggers the Autoboot restart sequence if the current
   test mode not equals to DSL_TESTMODE_TRAINING_LOCK.*/
   DSL_TESTMODE_TRAINING_LOCK = 3,
   /**
   The testmode 'lock' will "freeze" the autoboot state machine at any state
   that is currently active. In opposite to the other lock test modes the moment
   when this command is executed directly influences the behavior. */
   DSL_TESTMODE_LOCK = 4,
   /**
   This command can be used for unlocking any testmode.
   Compared to the command \ref DSL_TESTMODE_DISABLE this will NOT reset the
   autoboot state machine and does NOT restart the firmware. Instead of the
   autoboot state will be 'synchronized' with the current firmware state.
   \note This is a command only. If requesting the testmode state after
         successful execution of this command \ref DSL_TESTMODE_DISABLE will be
         returned. */
   DSL_TESTMODE_UNLOCK = 5,
   /**
   Sleep Mode (also known as Signal Test Mode) is entered.
   It triggers in a swap of the test mode page. This test mode can be used
   to force the ATU-R to transmit test signal like Reverb, Medley and
   single-tone signals.
   \attention It is NOT part of the API internal handling to force mentioned
              test signals itself. */
   DSL_TESTMODE_SLEEP = 6,
   DSL_TESTMODE_LAST
} DSL_TestModeControlSet_t;

/**
   Structure used for configuration of a testmode.
*/
typedef struct
{
   /**
   Testmode to activate */
   DSL_IN DSL_TestModeControlSet_t nTestMode;
} DSL_TestModeControlData_t;

/**
   Structure used for configuration of a testmode.
   This structure has to be used for ioctl \ref DSL_FIO_TEST_MODE_CONTROL_SET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Testmode to activate */
   DSL_IN DSL_TestModeControlData_t data;
} DSL_TestModeControl_t;

/**
   Structure used to return the currently active testmode.
*/
typedef struct
{
   /**
   Testmode to activate */
   DSL_OUT DSL_TestModeControlSet_t nTestMode;
} DSL_TestModeStatusData_t;

/**
   Structure used for getting the current testmode state.
   This structure has to be used for ioctl \ref DSL_FIO_TEST_MODE_STATUS_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Returns the testmode state that is currently active. */
   DSL_OUT DSL_TestModeStatusData_t data;
} DSL_TestModeStatus_t;

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   Structure to retrieve Auxiliary Line Inventory information according
   to G.993.2
*/
typedef struct
{
   /**
   Size of the buffer \ref pData in bytes.
   Pass here the size of the buffer to the function.
   In return the function will indicate in this parameter the number of
   bytes retrieved and written to the buffer successfully. */
   DSL_IN_OUT DSL_uint32_t nLength;
   /**
   Auxiliary Inventory bytes.
   Though G.993.2 does not define an upper limit of this field, it is
   limited to \ref DSL_G993_LI_MAXLEN_AUX in DSL-API for the near end. */
   DSL_OUT DSL_uint8_t pData[DSL_G993_LI_MAXLEN_AUX];
} DSL_AuxLineInventoryData_t;

/**
   Structure to retrieve Auxiliary Line Inventory information according
   to G.993.2.
   This structure has to be used for ioctl \ref DSL_FIO_AUX_LINE_INVENTORY_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Specifies for which direction (near-end/far-end) the function will
   apply */
   DSL_IN DSL_XTUDir_t nDirection;
   /**
   Structure that contains all necessary auxiliary version information data */
   DSL_IN_OUT DSL_AuxLineInventoryData_t data;
} DSL_AuxLineInventory_t;
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */

/**
   This structure contains user defined line parameters and results
   for the loop length calculation.
   This values are updated within the API
   - directly after showtime entry
   - in exception state after failed training and in case of availibility
     within firmware.
*/
typedef struct
{
   /**
   Loop length estimation for AWG26 (0.4mm diameter) loops.
   Unit depends on used unit selector and will be feet or meter. */
   DSL_OUT DSL_uint32_t nLength_Awg26;
   /**
   Loop length estimation for AWG24 (0.5mm diameter) loops.
   Unit depends on used unit selector and will be feet or meter. */
   DSL_OUT DSL_uint32_t nLength_Awg24;
} DSL_LoopLengthStatusData_t;

/**
   Defines the possible units that can be used within interface.
*/
typedef enum
{
   /**
   Units are set to feet. */
   DSL_UNIT_FEET = 0,
   /**
   Units are set to meter. */
   DSL_UNIT_METER = 1,
   /* Delimiter only */
   DSL_UNIT_LAST
} DSL_UnitSelector_t;

/**
   This structure contains user defined line parameters and results
   for the loop length calculation.
   This structure has to be used for ioctl \ref DSL_FIO_LOOP_LENGTH_STATUS_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Unit selector for loop length parameters.
   It can be selected if the loop length will be returned in feet or in meter. */
   DSL_IN DSL_UnitSelector_t nUnit;
   /**
   Structure that contains all necessary loop length status data */
   DSL_IN_OUT DSL_LoopLengthStatusData_t data;
} DSL_LoopLengthStatus_t;

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   Structure used for configuration access of the assignment from the logical
   DSL line to the used UTOPIA/POS PHY Address.
 */
typedef struct
{
   /**
   UTOPIA/POS PHY Address number */
   DSL_CFG DSL_uint8_t nPhyAdr;
} DSL_PhyAddressConfigData_t;

/**
   Structure used for configuration access of the assignment from the logical
   DSL line to the used UTOPIA/POS PHY Address.
   This structure has to be used for ioctl
   - \ref DSL_FIO_UTOPIA_ADDRESS_CONFIG_SET
   - \ref DSL_FIO_UTOPIA_ADDRESS_CONFIG_GET
   - \ref DSL_FIO_POSPHY_ADDRESS_CONFIG_SET
   - \ref DSL_FIO_POSPHY_ADDRESS_CONFIG_GET
 */
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Specifies for which (bearer) channel the function will apply */
   DSL_IN DSL_uint8_t nChannel;
   /**
   Structure that contains configuration data */
   DSL_CFG DSL_PhyAddressConfigData_t data;
} DSL_PhyAddressConfig_t;

/**
   Definitions for possible UTOPIA bus width.
*/
typedef enum
{
   /**
   select the UTOPIA default bus width (16bit)*/
   DSL_BUS_WIDTH_NA = -1,
   /**
   select the UTOPIA 8bit bus width */
   DSL_BUS_WIDTH_8B = 1,
   /**
   select the UTOPIA 16bit bus width */
   DSL_BUS_WIDTH_16B = 2,
   /*
   Delimiter */
   DSL_BUS_WIDTH_LAST
} DSL_UtopiaBusWidth_t;

/**
   Structure used for configuration access of the assignment from the logical
   DSL line to the used UTOPIA bus width.
 */
typedef struct
{
   /**
   UTOPIA bus width*/
   DSL_CFG DSL_UtopiaBusWidth_t nDataWidth;
} DSL_UtopiaBusWidthConfigData_t;

/**
   Structure used for configuration access of the assignment from the logical
   DSL line to the used UTOPIA data bus width.
   This structure has to be used for ioctl
   - \ref DSL_FIO_UTOPIA_BUS_WIDTH_CONFIG_SET
   - \ref DSL_FIO_UTOPIA_BUS_WIDTH_CONFIG_GET
 */
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains configuration data */
   DSL_CFG DSL_UtopiaBusWidthConfigData_t data;
} DSL_UtopiaBusWidthConfig_t;
#endif /* #if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)*/

/**
   Definitions for possible Customer Interfaces.
*/
typedef enum
{
   /**
   select the UTOPIA - System Interface
   \note This configuration/status is only valid for Vinax */
   DSL_SYSTEMIF_UNKNOWN = 0,
   /**
   select the UTOPIA - System Interface
   \note This configuration/status is only valid for Vinax */
   DSL_SYSTEMIF_UTOPIA = 1,
   /**
   select the POSPHY - System Interface
   \note This configuration/status is only valid for Vinax */
   DSL_SYSTEMIF_POSPHY = 2,
   /**
   select the MII - System Interface
   \note For Danube this configuration/status is the only valid one */
   DSL_SYSTEMIF_MII = 3,
   /**
   System Interface was not yet configured
   \note This configuration/status is only valid for Vinax */
   DSL_SYSTEMIF_UNDEFINED = 4,
   /*
   Delimiter */
   DSL_SYSTEMIF_LAST
} DSL_SystemInterfaceSelection_t;

/**
   Definitions for possible TC-Layers.

   \attention In case of VRX200 and configuration access this value is only
              valid for ADSL link activation sequence. For VDSL link activation
              sequence the fixed value (DSL_TC_EFM) is used within DSL CPE API
              internal configuration. The same value will be also returned
              within status request respective event handling.
*/
typedef enum
{
   /**
   Indicates that the TC-layer configuration is unknown at the current system
   state. This initialization value in only valid for status request (not for
   configuration) and should be only set after first startup of DSL CPE API
   and before first full init line state is passed. */
   DSL_TC_UNKNOWN = 0,
   /**
   select the TC-layer - ATM */
   DSL_TC_ATM = 1,
   /**
   select the TC-layer - EFM (PTM) */
   DSL_TC_EFM = 2,
   /**
   select the TC-layer - HDLC
   \note This configuration is only valid for Vinax */
   DSL_TC_HDLC = 3,
   /**
   select the TC-layer - Autonegotiation
   \note This configuration is valid for Danube and VRX200
   \note In case of a status request this value should be never returned */
   DSL_TC_AUTO = 4,
   /**
   forces the TC-layer - EFM (PTM)
   \note This configuration is only valid for Danube
   \attention This selection should be only used in case of problems with using
   \ref DSL_TC_EFM configuration setting because it activates a workaround for
   special DSLAMs */
   DSL_TC_EFM_FORCED = 5,
   /*
   Delimiter */
   DSL_TC_LAST
} DSL_TcLayerSelection_t;

/**
   Bitfield definitions for possible operation modes for TC-Layer in case of
   selecting EFM as current used TC-Layer selection.
   \note In case of writing or reading the current configuration setting any
         combination might be used. In case of status request only one bit is
         used.
*/
typedef enum
{
   /* Cleaned. */
   DSL_EMF_TC_CLEANED = 0x00000000,
   /**
   64/65-octet encapsulation supported
   \note For Vinax this is the only valid configuration option
   \note For Danube downstream this is the only valid configuration option */
   DSL_EMF_TC_NORMAL = 0x00000001,
   /**
   64/65-octet encapsulation with pre-emption
   \note This configuration is only valid for upstream configuration of Danube */
   DSL_EMF_TC_PRE_EMPTION = 0x00000002,
   /**
   64/65-octet encapsulation with short packets
   \note This configuration is for future usage and not supported at the moment */
   DSL_EMF_TC_SHORT_PACKETS = 0x00000004
} DSL_BF_EfmTcConfig_t;

/**
   Structure used for System Interface configuration.
*/
typedef struct
{
   /**
   Specifies the TC-Layer */
   DSL_CFG DSL_TcLayerSelection_t nTcLayer;
   /**
   Specifies the operation mode for upstream direction in case of using of EFM
   TC-layer */
   DSL_CFG DSL_BF_EfmTcConfig_t nEfmTcConfigUs;
   /**
   Specifies the operation mode for downstream direction in case of using of EFM
   TC-layer */
   DSL_CFG DSL_BF_EfmTcConfig_t nEfmTcConfigDs;
   /**
   Specifies the System Interface
   \note For the Danube version of the DSL CPE API the only possible mode
         is DSL_SYSTEMIF_MII. */
   DSL_CFG DSL_SystemInterfaceSelection_t nSystemIf;
} DSL_SystemInterfaceConfigData_t;

/**
   Structure used for System Interface configuration.
   This structure has to be used for ioctl
   - \ref DSL_FIO_SYSTEM_INTERFACE_CONFIG_SET
   - \ref DSL_FIO_SYSTEM_INTERFACE_CONFIG_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains system interface configuration settings */
   DSL_CFG DSL_SystemInterfaceConfigData_t data;
} DSL_SystemInterfaceConfig_t;

/**
   Structure used for System Interface configuration.
   This structure has to be used for ioctl
   \ref DSL_FIO_SYSTEM_INTERFACE_STATUS_GET
   and event
   DSL_EventType_t::DSL_EVENT_S_SYSTEM_INTERFACE_STATUS
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains system interface configuration settings */
   DSL_CFG DSL_SystemInterfaceConfigData_t data;
} DSL_SystemInterfaceStatus_t;

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   Band Plan Selection.
   Several bandplans from different standards are listed here.
   The bandplan defines the usage of tones grouped to bands
   for upstream and downstream transmission. Number width and position
   of these bands as well as the PSD shape over the spectrum is defined
   by the bandplan.
*/
typedef enum
{
   DSL_BANDPLAN_NA                                 = -1,
   /** ITU G993.2 Annex A Table A-2 Mask 1 */
   DSL_BANDPLAN_ITU_ANNEX_A_M1_EU32                = 0,
   /** ITU G993.2 Annex A Table A-2 Mask 9 */
   DSL_BANDPLAN_ITU_ANNEX_A_M9_EU64                = 1,
   /** ITU G993.2 Annex A Table A-4 Mask 1 */
   DSL_BANDPLAN_ITU_ANNEX_A_M1_ADLU32              = 2,
   /** ITU G993.2 Annex A Table A-4 Mask 9 */
   DSL_BANDPLAN_ITU_ANNEX_A_M9_ADLU64              = 3,
   /** ITU G993.2 Annex B
      Table B-2, B-3, B-4: B7-1 997-M1c-A-7 */
   DSL_BANDPLAN_ITU_ANNEX_B_B01                    = 4,
   /** ITU G993.2 Annex B
      Table B-2, B-3, B-4: B7-2 997-M1x-M-8 */
   DSL_BANDPLAN_ITU_ANNEX_B_B02                    = 5,
   /** ITU G993.2 Annex B
      Table B-2, B-3, B-4: B7-3 997-M1x-M */
   DSL_BANDPLAN_ITU_ANNEX_B_B03                    = 6,
   /** ITU G993.2 Annex B
      Table B-2, B-3, B-4: B7-4 997-M2x-M-8 */
   DSL_BANDPLAN_ITU_ANNEX_B_B04                    = 7,
   /** ITU G993.2 Annex B
      Table B-2, B-3, B-4: B7-5 997-M2x-A */
   DSL_BANDPLAN_ITU_ANNEX_B_B05                    = 8,
   /** ITU G993.2 Annex B
      Table B-2, B-3, B-4: B7-6 997-M2x-M */
   DSL_BANDPLAN_ITU_ANNEX_B_B06                    = 9,
   /** ITU G993.2 Annex B
      Table B-2, B-3, B-4: B8-1 998-M1x-A */
   DSL_BANDPLAN_ITU_ANNEX_B_B07                    = 10,
   /** ITU G993.2 Annex B
      Table B-2, B-3, B-4: B8-2 998-M1x-B */
   DSL_BANDPLAN_ITU_ANNEX_B_B08                    = 11,
   /** ITU G993.2 Annex B
      Table B-2, B-3, B-4: B8-3 998-M1x-NUS0 */
   DSL_BANDPLAN_ITU_ANNEX_B_B09                    = 12,
   /** ITU G993.2 Annex B
      Table B-2, B-3, B-4: B8-4 998-M2x-A */
   DSL_BANDPLAN_ITU_ANNEX_B_B10                    = 13,
   /** ITU G993.2 Annex B
      Table B-2, B-3, B-4: B8-5 998-M2x-M */
   DSL_BANDPLAN_ITU_ANNEX_B_B11                    = 14,
   /** ITU G993.2 Annex B
       998-M2x-M 17MHz */
   DSL_BANDPLAN_ITU_ANNEX_B_B11_17                 = 15,
   /** ITU G993.2 Annex B
      Table B-2, B-3, B-4: B8-6 998-M2x-B */
   DSL_BANDPLAN_ITU_ANNEX_B_B12                    = 16,
   /** ITU G993.2 Annex B
       998-M2x-B 17MHz */
   DSL_BANDPLAN_ITU_ANNEX_B_B12_17                 = 17,
   /** ITU G993.2 Annex B
      Table B-2, B-3, B-4: B8-7 998-M2x-NUS0 */
   DSL_BANDPLAN_ITU_ANNEX_B_B13                    = 18,
   /** ITU G993.2 Annex B
       998-M2x-NUS0 17MHz */
   DSL_BANDPLAN_ITU_ANNEX_B_B13_17                 = 19,
   /** ITU G993.2 Annex C
        */
   DSL_BANDPLAN_ITU_ANNEX_C                        = 20,
   /** ITU G993.2 Annex C  8kHz spacing */
   DSL_BANDPLAN_ITU_ANNEX_C_8K                     = 21,
   /** ITU G993.2 Annex B  997E30 M2x NUS0
      Table B-2, B-3, B-4:  B7-10 997E30 M2x NUS0*/
   DSL_BANDPLAN_ITU_ANNEX_B_997E30_M2X_NUS0        = 22,
   /** ITU G993.2 Annex C, starting at 1.1MHz
         */
   DSL_BANDPLAN_ITU_ANNEX_C_1M1                    = 23,
   /** ITU G993.2 Annex C  8kHz spacing, starting at 1.1MHz
        */
   DSL_BANDPLAN_ITU_ANNEX_C_8K_1M1                 = 24,
   /** ITU G993.2 Annex B
       998E17-M2x-A */
   DSL_BANDPLAN_ITU_ANNEX_B_998E17_M2X_A           = 25,
   /** ITU G993.2 Annex B
      Table B-2, B-3, B-4: B8-8 998E17-M2x-NUS0 */
   DSL_BANDPLAN_ITU_ANNEX_B_998E17_M2X_NUS0        = 26,
   /** ANFP Cal0 long */
   DSL_BANDPLAN_ANFP_CAL0_LONG                     = 27,
   /** ANFP Cal0 medium */
   DSL_BANDPLAN_ANFP_CAL0_MEDIUM                   = 28,
   /** ANFP Cal0 short */
   DSL_BANDPLAN_ANFP_CAL0_SHORT                    = 29,
   /** ANFP Cal0 extra short */
   DSL_BANDPLAN_ANFP_CAL0_ESHORT                   = 30,
   /**  */
   DSL_BANDPLAN_KOREA_FTTCAB                       = 31,
   /**  */
   DSL_BANDPLAN_KOREA_FTTCAB_8K                    = 32,
   /**  */
   DSL_BANDPLAN_HANARO_FTTCAB_8K                   = 33,
   /**  */
   DSL_BANDPLAN_ITU_ANNEX_C_FTTEX_A                = 34,
   /**  */
   DSL_BANDPLAN_ITU_ANNEX_C_FTTEX_M                = 35,
   /**  */
   DSL_BANDPLAN_ITU_ANNEX_C_FTTCAB_A               = 36,
   /**  */
   DSL_BANDPLAN_ITU_ANNEX_C_FTTCAB_A_8K            = 37,
   /**  */
   DSL_BANDPLAN_ITU_ANNEX_C_FTTCAB_M               = 38,
   /**  */
   DSL_BANDPLAN_ITU_ANNEX_C_FTTCAB_M_8K            = 39,
   /**  */
   DSL_BANDPLAN_ITU_ANNEX_A_M10_EU128              = 40,

   /** delimiter for the old names */
   DSL_BANDPLAN_LAST_OLD,


   /** New Bandplans according to the revised VDSL2 standard */
   DSL_BANDPLAN_FIRST_NEW                                =  50 ,
   DSL_ITU_VDSL2_ANNEXA_FIRST                            =  50 ,

   /* Annex A           */
   DSL_ITU_VDSL2_ANNEXA_M1_EU32                          =  50 ,
   DSL_ITU_VDSL2_ANNEXA_M2_EU36                          =  51 ,
   DSL_ITU_VDSL2_ANNEXA_M3_EU40                          =  52 ,
   DSL_ITU_VDSL2_ANNEXA_M4_EU44                          =  53 ,
   DSL_ITU_VDSL2_ANNEXA_M5_EU48                          =  54 ,
   DSL_ITU_VDSL2_ANNEXA_M6_EU52                          =  55 ,
   DSL_ITU_VDSL2_ANNEXA_M7_EU56                          =  56 ,
   DSL_ITU_VDSL2_ANNEXA_M8_EU60                          =  57 ,
   DSL_ITU_VDSL2_ANNEXA_M9_EU64                          =  58 ,
   DSL_ITU_VDSL2_ANNEXA_EU128                            =  59 ,

   DSL_ITU_VDSL2_ANNEXA_M1_ADLU32                        =  70 ,
   DSL_ITU_VDSL2_ANNEXA_M2_ADLU36                        =  71 ,
   DSL_ITU_VDSL2_ANNEXA_M3_ADLU40                        =  72 ,
   DSL_ITU_VDSL2_ANNEXA_M4_ADLU44                        =  73 ,
   DSL_ITU_VDSL2_ANNEXA_M5_ADLU48                        =  74 ,
   DSL_ITU_VDSL2_ANNEXA_M6_ADLU52                        =  75 ,
   DSL_ITU_VDSL2_ANNEXA_M7_ADLU56                        =  76 ,
   DSL_ITU_VDSL2_ANNEXA_M8_ADLU60                        =  77 ,
   DSL_ITU_VDSL2_ANNEXA_M9_ADLU64                        =  78 ,
   DSL_ITU_VDSL2_ANNEXA_ADLU128                          =  79 ,

   /** delimiter for Annex A type */
   DSL_ITU_VDSL2_ANNEXA_LAST,
   DSL_ITU_VDSL2_ANNEXB_997_FIRST                        =  100,

   /* ANNEX B         */
   DSL_ITU_VDSL2_ANNEXB_B7_1_997_M1C_A_7                 =  100,
   DSL_ITU_VDSL2_ANNEXB_B7_2_997_M1X_M_8                 =  101,
   DSL_ITU_VDSL2_ANNEXB_B7_3_997_M1X_M                   =  102,
   DSL_ITU_VDSL2_ANNEXB_B7_4_997_M2X_M_8                 =  103,
   DSL_ITU_VDSL2_ANNEXB_B7_5_997_M2X_A                   =  104,
   DSL_ITU_VDSL2_ANNEXB_B7_6_997_M2X_M                   =  105,
   DSL_ITU_VDSL2_ANNEXB_B7_7_HPE17_M1_NUS0               =  106,
   DSL_ITU_VDSL2_ANNEXB_B7_8_HPE30_M1_NUS0               =  107,
   DSL_ITU_VDSL2_ANNEXB_B7_9_997E17_M2X_NUS0             =  108,
   DSL_ITU_VDSL2_ANNEXB_B7_10_997E30_M2X_NUS0            =  109,

   DSL_ITU_VDSL2_ANNEXB_997_LAST,
   DSL_ITU_VDSL2_ANNEXB_998_FIRST                        =  120,

   DSL_ITU_VDSL2_ANNEXB_B8_1_998_M1X_A                   =  120,
   DSL_ITU_VDSL2_ANNEXB_B8_2_998_M1X_B                   =  121,
   DSL_ITU_VDSL2_ANNEXB_B8_3_998_M1X_NUS0                =  122,
   DSL_ITU_VDSL2_ANNEXB_B8_4_998_M2X_A                   =  123,
   DSL_ITU_VDSL2_ANNEXB_B8_5_998_M2X_M                   =  124,
   DSL_ITU_VDSL2_ANNEXB_B8_6_998_M2X_B                   =  125,
   DSL_ITU_VDSL2_ANNEXB_B8_7_998_M2X_NUS0                =  126,
   DSL_ITU_VDSL2_ANNEXB_B8_8_998E17_M2X_NUS0             =  127,
   DSL_ITU_VDSL2_ANNEXB_B8_9_998E17_M2X_NUS0_M           =  128,
   DSL_ITU_VDSL2_ANNEXB_B8_10_998ADE17_M2X_NUS0_M        =  129,
   DSL_ITU_VDSL2_ANNEXB_B8_11_998ADE17_M2X_A             =  130,
   DSL_ITU_VDSL2_ANNEXB_B8_12_998ADE17_M2X_B             =  131,
   DSL_ITU_VDSL2_ANNEXB_B8_13_998E30_M2X_NUS0            =  132,
   DSL_ITU_VDSL2_ANNEXB_B8_14_998E30_M2X_NUS0_M          =  133,
   DSL_ITU_VDSL2_ANNEXB_B8_15_998ADE30_M2X_NUS0_M        =  134,
   DSL_ITU_VDSL2_ANNEXB_B8_16_998ADE30_M2X_NUS0_A        =  135,

   DSL_ITU_VDSL2_ANNEXB_998ADE17_M2X_M                   =  140,
   DSL_ITU_VDSL2_ANNEXB_998E17_M2X_A                     =  141,

   /** delimiter for Annex B type */
   DSL_ITU_VDSL2_ANNEXB_998_LAST,
   DSL_ITU_VDSL2_ANNEXC_FIRST                            =  160,

   /* ANNEX C       */
   DSL_ITU_VDSL2_ANNEXC_FTTCAB_A                         =  160,
   DSL_ITU_VDSL2_ANNEXC_FTTCAB_M                         =  161,
   DSL_ITU_VDSL2_ANNEXC_FTTEX_A                          =  162,
   DSL_ITU_VDSL2_ANNEXC_FTTEX_M                          =  163,
   DSL_ITU_VDSL2_ANNEXC_O_ADSL                           =  164,
   DSL_ITU_VDSL2_ANNEXC_O_TCMISDN                        =  165,

   /** delimiter for Annex C type*/
   DSL_ITU_VDSL2_ANNEXC_LAST,
   DSL_VDSL1_FIRST                                       =  180,

   /* VDSL1         */
   DSL_ANSI_FTTCAB_M1                                    =  180,
   DSL_ANSI_FTTCAB_M2                                    =  181,
   DSL_ANSI_FTTEX_M1                                     =  182,
   DSL_ANSI_FTTEX_M2                                     =  183,
   DSL_ETSI_FTTCAB_PCAB_M1                               =  184,
   DSL_ETSI_FTTCAB_PCAB_M2                               =  185,
   DSL_ETSI_FTTEX_P1_M1_O_ISDN                           =  186,
   DSL_ETSI_FTTEX_P1_M2_O_ISDN                           =  187,
   DSL_ETSI_FTTEX_P2_M1_O_POTS                           =  188,
   DSL_ETSI_FTTEX_P2_M2_O_POTS                           =  189,
   DSL_ITU_VDSL1_ANNEXE_E2_PCABA_M1                      =  190,

   /** delimiter for VDSL1 type */
   DSL_VDSL1_LAST,

   /* Others  */
   DSL_HANARO_FTTCAB                                     =  200,
   DSL_ANFP_CAL0                                         =  201,

   /** delimiter only */
   DSL_BANDPLAN_LAST
} DSL_BandPlanType_t;

/**
   Profile Selection.
   You may select among the 8 VDSL2 profiles
   defined in G993.2 chapter 6.1
   The profile defines some transmission parameters like
   maximum transmit power, subcarrier spacing ...
*/
typedef enum
{
   /** Profile 8a */
   DSL_PROFILE_8A = 0,
   /** Profile 8b */
   DSL_PROFILE_8B = 1,
   /** Profile 8c */
   DSL_PROFILE_8C = 2,
   /** Profile 8d */
   DSL_PROFILE_8D = 3,
   /** Profile 12a */
   DSL_PROFILE_12A = 4,
   /** Profile 12b */
   DSL_PROFILE_12B = 5,
   /** Profile 17a */
   DSL_PROFILE_17A = 6,
   /** Bandplan 30a */
   DSL_PROFILE_30A = 7,
   /** Profile 17b */
   DSL_PROFILE_17B = 8,

   DSL_PROFILE_LAST = 9
} DSL_ProfileType_t;

/**
   Structure to read the current band plan configuration.
   \note To get more band plan related information please also refer to the
   ioctl \ref DSL_FIO_BAND_BORDER_STATUS_GET.
*/
typedef struct
{
   /**
   Selection of the Bandplan
   \note This parameter is not applicable at the CPE side (deprecated)
   within the following ioctl \ref DSL_FIO_BAND_PLAN_STATUS_GET. */
   DSL_OUT DSL_BandPlanType_t nBandPlan;
   /** Selection of the Profile */
   DSL_OUT DSL_ProfileType_t nProfile;
} DSL_BandPlanConfigData_t;

/**
   Structure to read the current band plan configuration.
   This structure has to be used for ioctl \ref DSL_FIO_BAND_PLAN_STATUS_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains all necessary version information data */
   DSL_OUT DSL_BandPlanConfigData_t data;
} DSL_BandPlanStatus_t;

/**
   Matrix of all possible combinations of
   profile from \ref DSL_ProfileType_t and bandplan from \ref DSL_BandPlanType_t.
   Each supported combination is represented by DSL_TRUE.
*/
typedef struct
{
   /** Bandplan vs profile support matrix */
   DSL_OUT DSL_boolean_t bSupported[DSL_BANDPLAN_LAST][DSL_PROFILE_LAST];
} DSL_BandPlanSupportData_t;

/**
   This function returns all VDSL2 bandplan/profile combinations supported by
   the device.
   This structure has to be used for ioctl \ref DSL_FIO_BAND_PLAN_SUPPORT_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains all necessary version information data */
   DSL_OUT DSL_BandPlanSupportData_t data;
} DSL_BandPlanSupport_t;

/**
   EFM Speed */
typedef enum
{
   /**
      Speed is 100 Mbps */
   DSL_EFM_SPEED_100 = 0,
   /**
      Speed is 10 Mbps */
   DSL_EFM_SPEED_10 = 1,
   /**
      Delimiter only! */
   DSL_EFM_SPEED_LAST = 2
} DSL_EFM_Speed_t;

/**
   EFM Duplex Mode */
typedef enum
{
   /**
      EFM Duplex mode is: Full Duplex */
   DSL_EFM_DUPLEX_FULL = 0,
   /**
      EFM Duplex mode is: Half Duplex */
   DSL_EFM_DUPLEX_HALF = 1,
   /**
      Delimiter only! */
   DSL_EFM_DUPLEX_LAST = 2
} DSL_EFM_Duplex_t;

/**
   Flow Control by pause frames on the MII.
*/
typedef enum
{
   /**
      Flow Control is off */
   DSL_EFM_FLOWCTRL_OFF    = 0,
   /**
      Flow Control is on */
   DSL_EFM_FLOWCTRL_ON   = 1,
   /**
      Delimiter only! */
   DSL_EFM_FLOWCTRL_LAST
} DSL_EFM_FlowCtrl_t;

/**
   This type defines the autonegotiation mode for the MAC in the device.
   If the autonegotiation is enabled, then the device will poll via MDIO
   register access the status of an attached ethernet phy and configure
   the speed and the duplex mode of the MAC accordingly.
   This only represents the status of the MAC and not of an attached
   ethernet phy. The autonegotiation function in the ethernet phy has
   to be enabled with a separate MDIO access.
*/
typedef enum
{
  /**
     Autonegotiation is disabled.
  */
  DSL_EFM_AUTONEG_OFF = 0,
  /**
     Autonegotiation is enabled
  */
  DSL_EFM_AUTONEG_ON = 1,
   /**
      Delimiter only! */
  DSL_EFM_AUTONEG_LAST
} DSL_EFM_AutoNegotiation_t;

/**
   This struct represents the MAC configuration settings. This only represents
   the settings of the MAC in the device and not of a possibly attached
   ethernet phy.
*/
typedef struct{
   /**
      Speed setting of the MAC interface. This can be 10MBit/s or 100MBit/s
   */
   DSL_CFG DSL_EFM_Speed_t nEfmSpeed;
   /**
      Duplex mode of the MAC interface. This can be Full Duplex or Half Duplex.
   */
   DSL_CFG DSL_EFM_Duplex_t nEfmDuplex;
   /**
      Flow Control mode of the MAC interface. This can be on or off. When flow
      control is activated in full duplex mode, then PAUSE frame according to
      IEEE 802.3 Annex 31B are used for flow control. In half-duplex mode a
      jam pattern is generated.
   */
   DSL_CFG DSL_EFM_FlowCtrl_t nFlowControl;
   /**
      Autonegotiation mode of the MAC interface. This can be enabled or disabled.
      When enabled, the device will poll via MDIO register access the speed and
      duplex state of an attached ethernet phy. Note that the ethernet
      autonegotiation in the ethernet phy has to be activated separately via a
      MDIO access.
      \note VINAX supports this parameter for hostless CPE only, means
            for far end from CO side.
   */
   DSL_CFG DSL_EFM_AutoNegotiation_t nAutoNegotiation;
   /**
      MAC Address
   */
   DSL_CFG DSL_uint8_t nMacAddress[DSL_MAC_ADDRESS_OCTETS];

   /**
      Maximum Frame Size for Link 0
   */
   DSL_CFG DSL_uint16_t nMaxFrameSize;
   /**
      Enable Support of external Slow Protocol Handling
   */
   DSL_CFG DSL_boolean_t bExtEthernetOam;
} DSL_EFM_MacConfigData_t;

/**
   Structure used for configuration of MAC related network parameters.
   This structure has to be used for ioctl
   - \ref DSL_FIO_EFM_MAC_CONFIG_SET
   - \ref DSL_FIO_EFM_MAC_CONFIG_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Specifies the direction (DSL_NEAR_END / DSL_FAR_END) to which the function
   will apply */
   DSL_IN DSL_XTUDir_t nDirection;
   /**
   Structure that contains configuration data */
   DSL_CFG DSL_EFM_MacConfigData_t data;
} DSL_EFM_MacConfig_t;


/**
   This structure is used to get total data path counters of the
   system side.
*/
typedef struct
{
   /**
   Transmitted Units (Frames)
   This Value refers to aFramesTransmittedOK
   of IEEE 802.3 chapter 30.3.1.1.2

   APPROPRIATE SYNTAX:
      Generalized non resettable counter. This counter has a
      maximum increment rate of 16 000 counts
      per second at 10 Mb/s

   BEHAVIOR DEFINED AS:
      A count of frames that are successfully transmitted.
      This counter is incremented when the
      TransmitStatus is reported as transmitOK. The actual
      update occurs in the LayerMgmtTransmitCounters procedure
      (5.2.4.2).; */
   DSL_uint32_t UnitsTx;
   /**
   Received Units (Frames)
   This value refers to of IEEE 802.3 chapter 30.3.1.1.5 aFramesReceivedOK

   APPROPRIATE SYNTAX:
      Generalized non resettable counter. This counter has a maximum
      increment rate of 16 000 counts per second at 10 Mb/s

   BEHAVIOR DEFINED AS:
      A count of frames that are successfully received (receiveOK).
      This does not include frames
      received with frame-too-long, FCS, length or alignment errors,
      or frames lost due to internal MAC
      sublayer error. This counter is incremented when the
      ReceiveStatus is reported as receiveOK. The
      actual update occurs in the LayerMgmtReceiveCounters procedure
      (5.2.4.3).; */
   DSL_uint32_t UnitsRx;
   /**
   This value refers to of IEEE 802.3 chapter 5.2.2.1.6
   aFrameCheckSequenceErrors

   APPROPRIATE SYNTAX:
      Generalized non resettable counter. This counter has a maximum increment
      rate of 16 000 counts per second.

   BEHAVIOR DEFINED AS:
         A count of frames that are an integral number of octets in length
         and do not pass the FCS check. This counter is incremented when
         the ReceiveStatus is reported as frameCheckError.
         The actual update occurs in the LayerMgmtReceiveCounters procedure
         (5.2.4.3). */
   DSL_uint32_t FrameCheckSequenceErrors;
   /**
   This value refers to of IEEE 802.3 chapter 5.2.2.1.7
   aAlignmentErrors

   APPROPRIATE SYNTAX:
      Generalized non resettable counter. This counter has a maximum increment
      rate of 16 000 counts per second.

   BEHAVIOR DEFINED AS:
      A count of frames that are not an integral number of octets in length
      and do not pass the FCS check. This counter is incremented when the
      ReceiveStatus is reported as alignmentError. The actual update occurs in
      the LayerMgmtReceiveCounters procedure (5.2.4.3). */
   DSL_uint32_t AlignmentErrors;
   /**
   Transmitted Octets
   This value refers to of IEEE 802.3 chapter 30.3.1.1.8 aOctetsTransmittedOK

   APPROPRIATE SYNTAX:
      Generalized non resettable counter. This counter
      has a maximum increment rate of 1 230 000
      counts per second at 10 Mb/s

   BEHAVIOR DEFINED AS:
      A count of data and padding octets of frames
      that are successfully transmitted. This counter is
      incremented when the TransmitStatus is reported
      as transmitOK. The actual update occurs in the
      LayerMgmtTransmitCounters procedure (5.2.4.2).; */
   DSL_uint32_t OctetsTx;
   /**
   Received Octets
   This value refers to of IEEE 802.3 chapter 30.3.1.1.14 aOctetsReceivedOK

   APPROPRIATE SYNTAX:
      Generalized non resettable counter. This counter
      has a maximum increment rate of 1 230 000
      counts per second at 10 Mb/s

   BEHAVIOR DEFINED AS:
      A count of data and padding octets in frames
      that are successfully received. This does not include
      octets in frames received with frame-too-long, FCS,
      length or alignment errors, or frames lost due
      to internal MAC sublayer error. This counter is
      incremented when the result of a reception is
      reported as a receiveOK status. The actual
      update occurs in the LayerMgmtReceiveCounters
      procedure (5.2.4.3).; */
   DSL_uint32_t OctetsRx;
   /**
   Units with Tx Errors
   This value refers to of IEEE 802.3 chapter 30.3.1.1.12
   aFramesLostDueToIntMACXmitError

   APPROPRIATE SYNTAX:
      Generalized non resettable counter. This counter has a maximum increment
      rate of 75 000 counts per second at 10 Mb/s

   BEHAVIOR DEFINED AS:
      A count of frames that would otherwise be transmitted by the station,
      but could not be sent due to an internal MAC sublayer transmit error.
      If this counter is incremented, then none of the other counters in this
      section are incremented. The exact meaning and mechanism for
      incrementing this counter is implementation dependent.; */
   DSL_uint32_t UnitsTxErrors;
   /**
   Units with Rx Errors
   This value refers to of IEEE 802.3 chapter 30.3.1.1.15
   aFramesLostDueToIntMACRcvError

   APPROPRIATE SYNTAX:
      Generalized non resettable counter. This counter has a maximum increment
      rate of 16 000 counts per second at 10 Mb/s

   BEHAVIOR DEFINED AS:
      A count of frames that would otherwise be received by the station, but
      could not be accepted due to an internal MAC sublayer receive error.
      If this counter is incremented, then none of the other counters in this
      section are incremented. The exact meaning and mechanism for
      incrementing this counter is implementation dependent.; */
   DSL_uint32_t UnitsRxErrors;
   /**
   Pause Frames Tx
   This value refers to of IEEE 802.3 chapter 30.3.4.2
   aPAUSEMACCtrlFramesTransmitted

   APPROPRIATE SYNTAX:
      Generalized non resettable counter. This counter has a maximum increment
      rate of 16 000 counts per second at 10 Mb/s

   BEHAVIOR DEFINED AS:
      A count of PAUSE frames passed to the MAC sublayer for transmission.
      This counter is incremented when a MA_CONTROL.request primitive is
      generated within the MAC Control sublayer with an opcode indicating the
      PAUSE operation.; */
   DSL_uint32_t PauseFramesTx;
   /**
   Pause Frames Rx
   This value refers to of IEEE 802.3 chapter 30.3.4.3
   aPAUSEMACCtrlFramesReceived

   APPROPRIATE SYNTAX:
      Generalized non resettable counter. This counter has a maximum increment
      rate of 16 000 counts per second at 10 Mb/s

   BEHAVIOR DEFINED AS:
      A count of MAC Control frames passed by the MAC sublayer to the MAC
      Control sublayer. This counter is incremented when a ReceiveFrame
      function call returns a valid frame with: (1) a lengthOrType field
      value equal to the reserved Type for 802.3_MAC_Control as specified
      in 31.4.1.3, and (2) an opcode indicating the PAUSE operation.; */
   DSL_uint32_t PauseFramesRx;
   /**
   Frame too long errors
   This value refers to of IEEE 802.3 chapter 30.3.1.1.25 aFrameTooLongErrors

   APPROPRIATE SYNTAX:
      Generalized non resettable counter. This counter has a maximum increment
      rate of 815 counts per second at 10 Mb/s

   BEHAVIOR DEFINED AS:
      A count of frames received that exceed the maximum permitted frame
      size. This counter is incremented when the status of a frame reception
      is frameTooLong. The actual update occurs in the
      LayerMgmtReceiveCounters procedure (5.2.4.3).
      NOTE: Implementations may use either maxUntaggedFrameSize or
      (maxUntaggedFrameSize + qTagPre.xSize) (see 4.2.7.1 and 4.4.2) for the
      maximum permitted frame size, either as a constant or as a function of
      whether the frame received is a basic or tagged frame (see 3.2 and 3.5).
      In implementations that treat this as a constant it is recommended that
      the larger value be used. The use of any value other than the larger
      value in this case may result in the counter being incremented by valid
      tagged frames.; */
   DSL_uint32_t FramesTooLong;
   /**
   Frame too short errors
   This value refers to counter etherStatsUndersizePkts from RFC2819

   DESCRIPTION:
      "The total number of packets received that were
      less than 64 octets long (excluding framing bits,
      but including FCS octets) and were otherwise well
      formed." */
   DSL_uint32_t FramesTooShort;
} DSL_LinePathCounterTotalData_t;

/**
   This structure is used to get total data path counters of the
   line side of the device.
   It has to be used for ioctl
   \ref DSL_FIO_LINE_PATH_COUNTER_TOTAL_GET
 */
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Specifies for which (bearer) channel the function will apply */
   DSL_IN DSL_uint8_t nChannel;
   /**
   Structure that contains configuration data */
   DSL_CFG DSL_LinePathCounterTotalData_t data;
} DSL_LinePathCounterTotal_t;

/**
   This structure is used to get total data path counters of the
   system side of the device.
*/
typedef struct
{
   /** Transmitted Units (Cells or Frames)
      This Value refers to aFramesTransmittedOK
      of IEEE 802.3 chapter 30.3.1.1.2

      APPROPRIATE SYNTAX:
         Generalized non resettable counter. This counter has a
         maximum increment rate of 16 000 counts
         per second at 10 Mb/s

      BEHAVIOR DEFINED AS:
         A count of frames that are successfully transmitted.
         This counter is incremented when the
         TransmitStatus is reported as transmitOK. The actual
         update occurs in the LayerMgmtTransmitCounters procedure
         (5.2.4.2).;
      */
   DSL_uint32_t UnitsTx;

   /**
      This Value refers to aSingleCollisionFrames
      of IEEE 802.3 chapter 5.2.2.1.3

   APPROPRIATE SYNTAX:
      Generalized non resettable counter. This counter has a maximum increment
      rate of 13 000 counts per second.

   BEHAVIOR DEFINED AS:
      A count of frames that are involved in a single collision and are
      subsequently transmitted successfully. This counter is incremented when
      the result of a transmission is reported as transmitOK and the attempt
      value  is 2. The actual update occurs in the LayerMgmtTransmitCounters
      procedure (5.2.4.2).
   */
   DSL_uint32_t SingleCollisionFrames;

   /**
      This Value refers to aMultipleCollisionFrames
      of IEEE 802.3 chapter 5.2.2.1.4

   APPROPRIATE SYNTAX:
      Generalized non resettable counter. This counter has a maximum increment
      rate of 11 000 counts per second.

   BEHAVIOR DEFINED AS:
      A count of frames that are involved in more than one collision and are
      subsequently transmitted successfully. This counter is incremented when
      the TransmitStatus is reported as transmitOK and the value of the attempts
      variable is greater than 2 and less or equal to attemptLimit. The actual
      update occurs in the LayerMgmtTransmitCounters procedure (5.2.4.2).
   */
   DSL_uint32_t MultipleCollisionFrames;

   /** Received Units (Cells or Frames)
      This value refers to of IEEE 802.3 chapter 30.3.1.1.5 aFramesReceivedOK

      APPROPRIATE SYNTAX:
         Generalized non resettable counter. This counter has a maximum
         increment rate of 16 000 counts per second at 10 Mb/s

      BEHAVIOR DEFINED AS:
         A count of frames that are successfully received (receiveOK).
         This does not include frames
         received with frame-too-long, FCS, length or alignment errors,
         or frames lost due to internal MAC
         sublayer error. This counter is incremented when the
         ReceiveStatus is reported as receiveOK. The
         actual update occurs in the LayerMgmtReceiveCounters procedure
         (5.2.4.3).;
   */
   DSL_uint32_t UnitsRx;

   /**
      This value refers to of IEEE 802.3 chapter 5.2.2.1.6
      aFrameCheckSequenceErrors

      APPROPRIATE SYNTAX:
         Generalized non resettable counter. This counter has a maximum increment
         rate of 16 000 counts per second.

      BEHAVIOR DEFINED AS:
            A count of frames that are an integral number of octets in length
            and do not pass the FCS check. This counter is incremented when
            the ReceiveStatus is reported as frameCheckError.
            The actual update occurs in the LayerMgmtReceiveCounters procedure
            (5.2.4.3).
   */
   DSL_uint32_t FrameCheckSequenceErrors;

   /**
      This value refers to of IEEE 802.3 chapter 5.2.2.1.7
      aAlignmentErrors

   APPROPRIATE SYNTAX:
      Generalized non resettable counter. This counter has a maximum increment
      rate of 16 000 counts per second.

   BEHAVIOR DEFINED AS:
      A count of frames that are not an integral number of octets in length
      and do not pass the FCS check. This counter is incremented when the
      ReceiveStatus is reported as alignmentError. The actual update occurs in
      the LayerMgmtReceiveCounters procedure (5.2.4.3).
   */
   DSL_uint32_t AlignmentErrors;

   /** Transmitted Octets
      This value refers to of IEEE 802.3 chapter 30.3.1.1.8 aOctetsTransmittedOK

      APPROPRIATE SYNTAX:
         Generalized non resettable counter. This counter
         has a maximum increment rate of 1 230 000
         counts per second at 10 Mb/s

      BEHAVIOR DEFINED AS:
         A count of data and padding octets of frames
         that are successfully transmitted. This counter is
         incremented when the TransmitStatus is reported
         as transmitOK. The actual update occurs in the
         LayerMgmtTransmitCounters procedure (5.2.4.2).;   */
   DSL_uint32_t OctetsTx;

   /** Received Octets
      This value refers to of IEEE 802.3 chapter 30.3.1.1.14 aOctetsReceivedOK

      APPROPRIATE SYNTAX:
         Generalized non resettable counter. This counter
         has a maximum increment rate of 1 230 000
         counts per second at 10 Mb/s

      BEHAVIOR DEFINED AS:
         A count of data and padding octets in frames
         that are successfully received. This does not include
         octets in frames received with frame-too-long, FCS,
         length or alignment errors, or frames lost due
         to internal MAC sublayer error. This counter is
         incremented when the result of a reception is
         reported as a receiveOK status. The actual
         update occurs in the LayerMgmtReceiveCounters
         procedure (5.2.4.3).;
   */
   DSL_uint32_t OctetsRx;

   /** Units with Tx Errors
      This value refers to of IEEE 802.3 chapter 30.3.1.1.12
      aFramesLostDueToIntMACXmitError

      APPROPRIATE SYNTAX:
         Generalized non resettable counter. This counter has a maximum increment
         rate of 75 000 counts per second at 10 Mb/s

      BEHAVIOR DEFINED AS:
         A count of frames that would otherwise be transmitted by the station,
         but could not be sent due to an internal MAC sublayer transmit error.
         If this counter is incremented, then none of the other counters in this
         section are incremented. The exact meaning and mechanism for
         incrementing this counter is implementation dependent.; */
   DSL_uint32_t UnitsTxErrors;

   /** Units with Rx Errors
      This value refers to of IEEE 802.3 chapter 30.3.1.1.15
      aFramesLostDueToIntMACRcvError

      APPROPRIATE SYNTAX:
         Generalized non resettable counter. This counter has a maximum increment
         rate of 16 000 counts per second at 10 Mb/s

      BEHAVIOR DEFINED AS:
         A count of frames that would otherwise be received by the station, but
         could not be accepted due to an internal MAC sublayer receive error.
         If this counter is incremented, then none of the other counters in this
         section are incremented. The exact meaning and mechanism for
         incrementing this counter is implementation dependent.;
   */
   DSL_uint32_t UnitsRxErrors;

   /** Pause Frames Tx
      This value refers to of IEEE 802.3 chapter 30.3.4.2
      aPAUSEMACCtrlFramesTransmitted

      APPROPRIATE SYNTAX:
         Generalized non resettable counter. This counter has a maximum increment
         rate of 16 000 counts per second at 10 Mb/s

      BEHAVIOR DEFINED AS:
         A count of PAUSE frames passed to the MAC sublayer for transmission.
         This counter is incremented when a MA_CONTROL.request primitive is
         generated within the MAC Control sublayer with an opcode indicating the
         PAUSE operation.;
   */
   DSL_uint32_t PauseFramesTx;

   /** Pause Frames Rx
      This value refers to of IEEE 802.3 chapter 30.3.4.3
      aPAUSEMACCtrlFramesReceived

      APPROPRIATE SYNTAX:
         Generalized non resettable counter. This counter has a maximum increment
         rate of 16 000 counts per second at 10 Mb/s

      BEHAVIOR DEFINED AS:
         A count of MAC Control frames passed by the MAC sublayer to the MAC
         Control sublayer. This counter is incremented when a ReceiveFrame
         function call returns a valid frame with: (1) a lengthOrType field
         value equal to the reserved Type for 802.3_MAC_Control as specified
         in 31.4.1.3, and (2) an opcode indicating the PAUSE operation.;
   */
   DSL_uint32_t PauseFramesRx;

   /** Frame too long errors
      This value refers to of IEEE 802.3 chapter 30.3.1.1.25 aFrameTooLongErrors

      APPROPRIATE SYNTAX:
         Generalized non resettable counter. This counter has a maximum increment
         rate of 815 counts per second at 10 Mb/s

      BEHAVIOR DEFINED AS:
         A count of frames received that exceed the maximum permitted frame
         size. This counter is incremented when the status of a frame reception
         is frameTooLong. The actual update occurs in the
         LayerMgmtReceiveCounters procedure (5.2.4.3).
         NOTE: Implementations may use either maxUntaggedFrameSize or
         (maxUntaggedFrameSize + qTagPre.xSize) (see 4.2.7.1 and 4.4.2) for the
         maximum permitted frame size, either as a constant or as a function of
         whether the frame received is a basic or tagged frame (see 3.2 and 3.5).
         In implementations that treat this as a constant it is recommended that
         the larger value be used. The use of any value other than the larger
         value in this case may result in the counter being incremented by valid
         tagged frames.;
   */
   DSL_uint32_t FramesTooLong;

   /** Frame too short errors
      This value refers to counter etherStatsUndersizePkts from RFC2819

      DESCRIPTION:
         "The total number of packets received that were
         less than 64 octets long (excluding framing bits,
         but including FCS octets) and were otherwise well
         formed."
   */
   DSL_uint32_t FramesTooShort;

   /** Units with CRC error
      See IEEE 802.3 ah, 45.2.6.11 (Register 6.24)
   */
   DSL_uint32_t UnitsCrc;

   /** Units with Alignment error
      See IEEE 802.3 ah, 45.2.6.12 (Register 6.25)
   */
   DSL_uint32_t UnitsAlign;
} DSL_DataPathCounterTotalData_t;

/**
   This structure is used to get total data path counters of the
   system side of the device.
   It has to be used for ioctl
   \ref DSL_FIO_DATA_PATH_COUNTER_TOTAL_GET
 */
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Specifies for which (bearer) channel the function will apply */
   DSL_IN DSL_uint8_t nChannel;
   /**
   Structure that contains configuration data */
   DSL_CFG DSL_DataPathCounterTotalData_t data;
} DSL_DataPathCounterTotal_t;
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */

/**
   This structure is used to get resource usage statistics data
*/
typedef struct
{
   /**
   Total memory allocated statically (bytes) */
   DSL_uint32_t staticMemUsage;
   /**
   Total memory allocated dynamically (bytes) */
   DSL_uint32_t dynamicMemUsage;
} DSL_ResourceUsageStatisticsData_t;

/**
   This structure is used to get resource usage statistics data.
   It has to be used for ioctl
   \ref DSL_FIO_RESOURCE_USAGE_STATISTICS_GET
 */
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains statistics data */
   DSL_ResourceUsageStatisticsData_t data;
} DSL_ResourceUsageStatistics_t;

/**
   Structure for showtime event logging data upload.
*/
typedef struct
{
   /**
   Pointer to logging data. */
   DSL_OUT DSL_uint8_t *pData;
   /**
   Size of logging data.
   This value is only valid if pData pointer is NOT equal DSL_NULL */
   DSL_IN DSL_uint32_t nDataSize;
   /**
   Offset of current chunk within complete logging data.
   This values has to be used in case of using chunks for logging data upload
   only.
   A value equal to 0 means that chunk are NOT used.
   A value >0 and smaller nDataSize indicates a chunk. In this case the ioctl
   has to be called multiple times until nDataOffset is equal to nDataSize
   which means the complete data has been transferred. */
   DSL_IN DSL_uint32_t nDataOffset;
} DSL_ShowtimeLoggingData_t;

/**
   Structure for showtime event logging data upload.
   This structure has to be used for ioctl
   \ref DSL_FIO_SHOWTIME_LOGGING_DATA_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains showtime event logging data */
   DSL_OUT DSL_ShowtimeLoggingData_t data;
} DSL_ShowtimeLogging_t;

#ifdef DSL_INTERN
/**
   Specifies all possible timeout event types.
*/
typedef enum
{
   /**
      Reserved event timeout identifier shall be NOT used! */
   DSL_TIMEOUTEVENT_RESERVED = 0,
   /**
      Event timeout identifier for initialization of API */
   DSL_TIMEOUTEVENT_API_INIT = 1,
   /**
      Event timeout identifier for far end information retrieval */
   DSL_TIMEOUTEVENT_FE_STATUS = 2,
   /**
      Event timeout identifier for EAPS end */
   DSL_TIMEOUTEVENT_EAPS = 3
} DSL_TimeoutEvent_t;

/**
   Defines all possible (ADSL) latency path configurations.
*/
typedef enum
{
   /**
   Zero has been chosen to indicate 'not initialized' after memset of context
   structure after startup for example */
   DSL_LATENCY_UNKNOWN = 0,
   /**
   Latency path is not used */
   DSL_LATENCY_DISABLED = 1,
   /**
   Latency path is interleaved (ADSL1) or LP0 for ADSL2/2+ */
   DSL_LATENCY_IP_LP0 = 2,
   /**
   Latency path is fast (ADSL1) or LP1 for ADSL2/2+ */
   DSL_LATENCY_FP_LP1 = 3,
   DSL_LATENCY_LAST
} DSL_LatencyPath_t;
#endif

/**
   This structure shall carry the complete PSD shape for all tones
*/
typedef struct
{
   /**
   Number of first tone index of the band */
   DSL_uint16_t nFirstToneIndex;
   /**
   Number of last tone index of the band */
   DSL_uint16_t nLastToneIndex;
} DSL_Band_t;

/**
   This structure is used to get band border status values.
*/
typedef struct
{
   /**
   Number of used data elements */
   DSL_uint16_t nNumData;
   /**
   Reports the lowest and the highest tones per band that will be considered for
   bitloading. */
   DSL_Band_t nBandLimits[DSL_MAX_NUMBER_OF_BANDS];
   /**
   Reports the lowest and the highest tones per band that are effectively used
   for bitloading. These values are derived from BAT itself and might change
   during showtime due to bitswaps and SRA. */
   DSL_Band_t nBandBorder[DSL_MAX_NUMBER_OF_BANDS];
} DSL_BandBorderStatusData_t;

/**
   This structure is used to get band border status values and has to be
   used for ioctl \ref DSL_FIO_BAND_BORDER_STATUS_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Specifies for which direction (upstream/downstream) the function will
   apply */
   DSL_IN DSL_AccessDir_t nDirection;
   /**
   Structure that contains status data */
   DSL_OUT DSL_BandBorderStatusData_t data;
} DSL_BandBorderStatus_t;

/**
   Definition of possible miscellaneous status values.
   According to the used selector the value of
   \ref DSL_MiscLineStatusData_t :: nStatusValue
   has the meaning as described within selector value.
*/
typedef enum
{
   /**
   DS Attainable Data Rate Estimate coding gain that is assumed in bitloading.
   The reported value is expressed in multiple of 0.1dB and has a valid range
   of 0dB (coded as 0) to 7.5dB (coded as 75).
   \note This value is only supported for ADSL1 and can be requested in showtime
   only. */
   DSL_MLS_ATTNDR_CODING_GAIN_DS = 0,
   /**
   DS Attainable Data Rate Estimate Max number of bits per symbol including
   overhead.
   \note This value is only supported for ADSL1 and can be requested in showtime
   only. */
   DSL_MLS_ATTNDR_MAX_BITS_PER_SYMBOL_DS = 1,
   /**
   Upstream latency status.
   The reported value is set to '1' if both fast and interleave path respective
   LP0 and LP1 carry user data in US. Otherwise it is set to '0'. */
   DSL_MLS_DUAL_LATENCY_ON_US = 2,
   /**
   Downstream latency status.
   The reported value is set to '1' if both fast and interleave path respective
   LP0 and LP1 carry user data in DS. Otherwise it is set to '0'. */
   DSL_MLS_DUAL_LATENCY_ON_DS = 3,
   /**
   Initial Downstream Signal-to-Noise Ratio Margin estimated at end of training
   for the current connection (see chapter 7.5.1.10/7.5.1.11 of G.997.1).
   The reported value is an integral value over the complete spectrum.
   The downstream SNR margin ranges from -64dB (coded as -640) to
   +63dB (coded as 630) with 0.1dB steps.
   A special value (-641) indicates the parameter is out of range to be
   represented. */
   DSL_MLS_INIT_SNR_DS = 4,
   /**
   Downstream Nominal PSD for Gi=0.
   The reported value is expressed in dBm/Hz and ranges from 0 dBm/Hz
   (coded as 0) to -97.5 dBm/Hz (coded as -975) within steps of 0.1 dBm/Hz.
   \note This value is only supported in ADSL2/2+ modes. */
   DSL_MLS_NOMPSD_US = 5,
   /**
   Upstream Nominal PSD for Gi=0.
   The reported value is expressed in dBm/Hz and ranges from 0 dBm/Hz
   (coded as 0) to -97.5 dBm/Hz (coded as -975) within steps of 0.1 dBm/Hz.
   \note This value is only supported in ADSL2/2+ modes. */
   DSL_MLS_NOMPSD_DS = 6,
   /**
   Upstream Line Rate
   The reported value is coded in bits per symbol.
   \note This value is currently only supported for ADSL1. */
   DSL_MLS_LINE_RATE_US = 7,
   /**
   Downstream Line Rate
   The reported value is coded in bits per symbol.
   \note This value is currently only supported for ADSL1. */
   DSL_MLS_LINE_RATE_DS = 8,
   /**
   Upstream Net (total) Rate
   The reported value is coded in bits per symbol.
   \note This value is only supported for ADSL1. */
   DSL_MLS_NET_RATE_US = 9,
   /**
   Downstream Net (total) Rate
   The reported value is coded in bits per symbol.
   \note This value is only supported for ADSL1. */
   DSL_MLS_NET_RATE_DS = 10,
   /**
   Upstream Total Data Rate
   The reported value is coded in bits per symbol.
   \note This value is currently only supported for ADSL1. */
   DSL_MLS_TOTAL_DATA_RATE_US = 11,
   /**
   Downstream Total Data Rate
   The reported value is coded in bits per symbol.
   \note This value is currently only supported for ADSL1. */
   DSL_MLS_TOTAL_DATA_RATE_DS = 12,
   /**
   Upstream Aggregate Data Rate
   The reported value is coded in bits per symbol.
   \note This value is currently only supported for ADSL1. */
   DSL_MLS_AGGREGATE_DATA_RATE_US = 13,
   /**
   Downstream Aggregate Data Rate
   The reported value is coded in bits per symbol.
   \note This value is currently only supported for ADSL1. */
   DSL_MLS_AGGREGATE_DATA_RATE_DS = 14,
   /**
   Required Minimum Impulse Noise Protection for frame bearer 0.
   The reported value is expressed in fraction of DMT symbols with steps
   of 0.5. */
   DSL_MLS_MIN_INP_IP_LP0_DS = 15,
   /**
   Required Minimum Impulse Noise Protection for frame bearer 1.
   The reported value is expressed in fraction of DMT symbols with steps
   of 0.5. */
   DSL_MLS_MIN_INP_FP_LP1_DS = 16,
   /**
   Upstream Attainable Aggregate Data Rate.
   The reported value is coded in bits per frame.*/
   DSL_MLS_ATT_AGGREGATE_DATA_RATE_US = 17,

   /**
   Downstream Attainable Aggregate Data Rate.
   The reported value is coded in bits per frame.*/
   DSL_MLS_ATT_AGGREGATE_DATA_RATE_DS = 18,

   /**
   Upstream Attainable Line Data Rate.
   The reported value is coded in bits per frame.*/
   DSL_MLS_ATT_LINE_DATA_RATE_US = 19,

   /**
   Downstream Attainable Line Data Rate.
   The reported value is coded in bits per frame.*/
   DSL_MLS_ATT_LINE_DATA_RATE_DS = 20,

   /**
   Upstream Attainable Total Data Rate.
   The reported value is coded in bits per frame.*/
   DSL_MLS_ATT_TOTAL_DATA_RATE_US = 21,

   /**
   Downstream Attainable Total Data Rate.
   The reported value is coded in bits per frame.*/
   DSL_MLS_ATT_TOTAL_DATA_RATE_DS = 22,

   /**
   Currently selected upstream mask.
   The reported value corresponds to the definition of the following enum
   - \ref DSL_MLS_MaskAnnexMJ_Us_t */
   DSL_MLS_MASK_ANNEX_M_J_US = 23,

   /**
   Delimiter only */
   DSL_MLS_LAST
} DSL_MiscLineStatusSelector_t;

/**
   Definition of possible upstream mask values.
   This configuration parameter defines which upstream PSD mask is enabled.
   This parameter is used only for Annexes J and M of ITU-T Recs G.992.3 and
   G.992.5. As only one selection parameter is defined in the MIB, the same
   selection value applies to all relevant modes enabled in the XTSE line
   configuration parameter.
   It ranges from 1 to 9 and selects the mask with the definitions of Table 7-5
   within G.997.1.*/
typedef enum

{
   /**
   The value for AnnexM/J upstream mask selection is not applicable.
   For example the currently activated xDSL mode is not AnnexM or AnnexJ. */
   DSL_MASK_ANNEXMJ_US_NA = 0,
   /**
   Upstream msk 1 selected.
   ADLU-32 for Annex J of G.992.3 and G.992.5
   EU-32 for Annex M of G.992.3 and G.992.5 */
   DSL_MASK_ANNEXMJ_US_1 = 1,
   /**
   Upstream msk 2 selected.
   ADLU-36 for Annex J of G.992.3 and G.992.5
   EU-36 for Annex M of G.992.3 and G.992.5 */
   DSL_MASK_ANNEXMJ_US_2 = 2,
   /**
   Upstream msk 3 selected.
   ADLU-40 for Annex J of G.992.3 and G.992.5
   EU-40 for Annex M of G.992.3 and G.992.5 */
   DSL_MASK_ANNEXMJ_US_3 = 3,
   /**
   Upstream msk 4 selected.
   ADLU-44 for Annex J of G.992.3 and G.992.5
   EU-44 for Annex M of G.992.3 and G.992.5 */
   DSL_MASK_ANNEXMJ_US_4 = 4,
   /**
   Upstream msk 5 selected.
   ADLU-48 for Annex J of G.992.3 and G.992.5
   EU-48 for Annex M of G.992.3 and G.992.5 */
   DSL_MASK_ANNEXMJ_US_5 = 5,
   /**
   Upstream msk 6 selected.
   ADLU-52 for Annex J of G.992.3 and G.992.5
   EU-52 for Annex M of G.992.3 and G.992.5 */
   DSL_MASK_ANNEXMJ_US_6 = 6,
   /**
   Upstream msk 7 selected.
   ADLU-56 for Annex J of G.992.3 and G.992.5
   EU-56 for Annex M of G.992.3 and G.992.5 */
   DSL_MASK_ANNEXMJ_US_7 = 7,
   /**
   Upstream msk 8 selected.
   ADLU-60 for Annex J of G.992.3 and G.992.5
   EU-60 for Annex M of G.992.3 and G.992.5 */
   DSL_MASK_ANNEXMJ_US_8 = 8,
   /**
   Upstream msk 9 selected.
   ADLU-64 for Annex J of G.992.3 and G.992.5
   EU-64 for Annex M of G.992.3 and G.992.5 */
   DSL_MASK_ANNEXMJ_US_9 = 9,
   /**
   Last entry - only used as delimiter! */
   DSL_MASK_ANNEXMJ_US_LAST
} DSL_MLS_MaskAnnexMJ_Us_t;


/**
   This structure is used to get miscellaneous line status values.
*/
typedef struct
{
   /**
   Specifies which line status value should be read. */
   DSL_IN DSL_MiscLineStatusSelector_t nStatusSelector;
   /**
   The meaning of the value depends on the selected feature
   \ref DSL_MiscLineStatusSelector_t. */
   DSL_OUT DSL_int32_t nStatusValue;
} DSL_MiscLineStatusData_t;

/**
   This structure is used to get miscellaneous line status values and has to be
   used for ioctl \ref DSL_FIO_MISC_LINE_STATUS_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains status data */
   DSL_IN_OUT DSL_MiscLineStatusData_t data;
} DSL_MiscLineStatus_t;

/**
   Definition of possible line options values.
   According to the used selector the value of
   \ref DSL_LineOptionsConfigData_t :: nConfigValue
   has the meaning as described within selector value.
*/
typedef enum

{
   /**
   Configuration of a target noise margin delta at the CPE, which is added to
   the target noise margin value configured at and received from the CO.
   The resulting value is then taken as target noise margin, e.g. for bit
   loading.
   The target noise margin delta ranges from -5 dB to +5 dB with 0.1 dB steps.
   The default value is 0 dB.
   \note API value -50 corresponds to -5 dB, 50 to 5 dB.
   \attention If this parameter will be changed from it's default value it is
              strictly recommended to use ONLY positive values. Negative values
              are likely to result in link instabilities. */
   DSL_OPT_NOISE_MARGIN_DELTA_DS = 0,
   /**
   Selection of erasure decoding algorithm.
   Valid values are
   - '0' for increased Impulse Noise Protection (INP)
   - '1' for increased data rate
   \note This configuration option is only valid for Danube Family.
   \note The erasure decoding functionality itself is enabled within firmware
         by default. */
   DSL_ERASURE_DECODING_TYPE_DS = 1,
   /**
   Usage of trust me bit within G.HS.
   To convey erasure decoding capability in G.Hs CLR message.
   Valid values are
   - '0' for disabling (do not transmit in G.HS)
   - '1' for enabling (transmit in G.HS)
   \note This configuration option is only valid for Danube Family. */
   DSL_TRUST_ME_BIT = 2,
   /**
   Configuration of G.HS option for spectral inband shaping.
   Valid values are
   - '0' for not support option during G.HS
   - '1' for support option during G.HS (in this case the CO might send Upstream
         PSD Mask). This is the default configuration.
   \note This configuration option is only valid for Danube Family. */
   DSL_INBAND_SPECTRAL_SHAPING_US = 3,
   /**
   Last entry - only used as delimiter! */
   DSL_OPT_LAST
} DSL_LineOptionsSelector_t;

/**
   This structure is used to set/get line options values.
*/
typedef struct
{
   /**
   Specifies line option value to set or get. */
   DSL_LineOptionsSelector_t nConfigSelector;
   /**
   The meaning of the value depends on the selected feature
   \ref DSL_LineOptionsSelector_t. */
   DSL_int32_t nConfigValue;
} DSL_LineOptionsConfigData_t;

/**
   This structure is used to get miscellaneous line status values and has to be
   used for ioctl
   - \ref DSL_FIO_LINE_OPTIONS_CONFIG_SET
   - \ref DSL_FIO_LINE_OPTIONS_CONFIG_GET

*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains status data */
   DSL_IN_OUT DSL_LineOptionsConfigData_t data;
} DSL_LineOptionsConfig_t;

#ifdef INCLUDE_DEVICE_EXCEPTION_CODES
/**
   This function returns the last device specific exception codes.
   \note This structure might be extended by additional values if necessary.
   \note The return values are transparently forwarded from the firmware and
         thus they are not described in detail with DSL CPE API context.
*/
typedef struct
{
   /**
   This structure includes device specific and optional error code 1.
   A value of 0 means that there is no error or the value is not used.
   It is mapped as follows
   - Danube Family: STAT 5 0  (Current link exception code)
   - Vinax : CMD_ModemFSM_FailReasonGet, Parameter 3 */
   DSL_uint32_t nError1;
   /**
   This structure includes device specific and optional error code 2
   (Previous link exception code).
   A value of 0 means that there is no error or the value is not used.
   This value is handled API internally. On a consecutive fail state entry the
   value from previous error code (nError1) is copied to this value (nError2)
   before nError1 is updated with the current (new) value */
   DSL_uint32_t nError2;
} DSL_DBG_LastExceptionCodesData_t;

/**
   This structure is used to get the last device specific exception codes.
   It is used for ioctl \ref DSL_FIO_DBG_LAST_EXCEPTION_CODES_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains status data */
   DSL_OUT DSL_DBG_LastExceptionCodesData_t data;
} DSL_DBG_LastExceptionCodes_t;
#endif /* INCLUDE_DEVICE_EXCEPTION_CODES*/

/** @} DRV_DSL_CPE_COMMON */

/** \addtogroup DRV_DSL_CPE_EVENT
 @{ */


/**
alias for first callback event */
#define DSL_EVENT_FIRST   DSL_EVENT_I_FIRST
/**
alias for first callback indication */
#define DSL_EVENT_I_FIRST   DSL_EVENT_I_LINE_FAILURES
/**
alias for last callback indication */
#define DSL_EVENT_I_LAST    DSL_EVENT_S_FIRST
/**
alias for first callback status event */
#define DSL_EVENT_S_FIRST   DSL_EVENT_S_LINIT_FAILURE
/**
alias for last callback status event */
#define DSL_EVENT_S_LAST    DSL_EVENT_LAST

/**
   Defines all possible classes of callback event types according to G.997.1
   standard (see figure 7-2).

   Meaning of group specifiers
   - prefix DSL_EVENT_  indicates a common event type specifier
   - prefix DSL_EVENT_I_ indicates an event for indication or alarm
   - prefix DSL_EVENT_S_ indicates a, event for status

*/
typedef enum
{

   /* *********************************************************************** */
   /* *** FAULT MONITORING EVENTS                                         *** */
   /* *********************************************************************** */
   /**
   (0) Direction (Near/Far End) specific line failure(s) occurred
   (e.g. LOS, LOF, LPR).
   For a detailed description of the information provided with this event
   please refer to the description of the following structure(s)
   - ref DSL_G997_LineFailures_t */
   DSL_EVENT_I_LINE_FAILURES = 0,
   /**
   (1) Direction (Near/Far End) specific data path failure(s) occurred
   (e.g. NCD, LCD).
   For a detailed description of the information provided with this event
   please refer to the description of the following structure(s)
   - ref DSL_G997_DataPathFailures_t */
   DSL_EVENT_I_DATA_PATH_FAILURES = 1,
   /**
   (2) Direction (Near/Far End) specific line threshold value exceeded */
   DSL_EVENT_I_LINE_THRESHOLD_CROSSING = 2,
   /**
   (3) Direction (Near/Far End) specific channel threshold value exceeded.
   For a detailed description of the information provided with this event
   please refer to the description of the following structure(s)
   - ref DSL_G997_ChannelDataRateThreshold_t */
   DSL_EVENT_I_CHANNEL_THRESHOLD_CROSSING = 3,
   /**
   (4) Direction (Near/Far End) specific data path threshold value exceeded */
   DSL_EVENT_I_DATA_PATH_THRESHOLD_CROSSING = 4,
   /**
   (5) Direction (Near/Far End) specific ReTx threshold value exceeded */
   DSL_EVENT_I_RETX_THRESHOLD_CROSSING = 5,
   /**
   (6) Data rate up/down-shift threshold crossing.
   For a detailed description of the information provided with this event
   please refer to the description of the following structure(s)
   - ref DSL_G997_DataRateShiftThresholdCrossingData_t */
   DSL_EVENT_I_CHANNEL_DATARATE_SHIFT_THRESHOLD_CROSSING = 6,

   /* *********************************************************************** */
   /* *** TEST, DIAGNOSTIC AND STATUS EVENTS                              *** */
   /* *********************************************************************** */
   /**
   (7) Line initialization failure (LINIT).
   For a detailed description of the information provided with this event
   please refer to the description of the following structure(s)
   - ref DSL_G997_LineInit_t */
   DSL_EVENT_S_LINIT_FAILURE = 7,
   /**
   (8) Line state change event.
   For a detailed description of the information provided with this event
   please refer to the description of the following structure(s)
   - ref DSL_LineState_t */
   DSL_EVENT_S_LINE_STATE = 8,
   /**
   (9) Line power management state change event.
   For a detailed description of the information provided with this event
   please refer to the description of the following structure(s)
   - ref DSL_G997_PowerManagementStatus_t */
   DSL_EVENT_S_LINE_POWERMANAGEMENT_STATE = 9,
   /**
   (10) Data rate change event.
   For a detailed description of the information provided with this event
   please refer to the description of the following structure(s)
   - ref DSL_G997_ChannelStatus_t */
   DSL_EVENT_S_CHANNEL_DATARATE = 10,
   /**
   (11) Firmware error event. */
   DSL_EVENT_S_FIRMWARE_ERROR = 11,
   /**
   (12) DSL API initialization ready event.
   \attention It is strongly recommended to check for this event before
   calling any configuration functions, because the DMS does not guarantee
   a proper functionality before. */
   DSL_EVENT_S_INIT_READY = 12,
   /**
   (13) FarEnd line inventory available. */
   DSL_EVENT_S_FE_INVENTORY_AVAILABLE = 13,
   /**
   (14) System status event.
   For a detailed description of the information provided with this event
   please refer to the description of the following structure(s)
   - ref DSL_SystemStatusEvent_t */
   DSL_EVENT_S_SYSTEM_STATUS = 14,
   /**
   (15) Performance Monitoring synchronization (PM sync) event.
   For a detailed description of the information provided with this event
   please refer to the description of the following structure(s)
   - ref DSL_PM_SyncEvent_t */
   DSL_EVENT_S_PM_SYNC = 15,
   /**
   (16) Line transmission status.
   For a detailed description of the information provided with this event
   please refer to the description of the following structure(s)
   - ref DSL_G997_LineTransmissionStatus_t */
   DSL_EVENT_S_LINE_TRANSMISSION_STATUS = 16,
   /**
   (17) Showtime Event Logging.
   InfoEvent which does not include any additional data beside event
   information itself.
   To get detailed data the ioctl \ref DSL_FIO_SHOWTIME_LOGGING_DATA_GET has
   to be used. */
   DSL_EVENT_S_SHOWTIME_LOGGING = 17,
   /**
   (18) FW binary request Event.
   InfoEvent which does not include any additional data beside event
   information itself.
   Event informs the upper layer application to provide the FW binary
   via the ioctl \ref DSL_FIO_AUTOBOOT_LOAD_FIRMWARE */
   DSL_EVENT_S_FIRMWARE_REQUEST = 18,
   /**
   (19) SNMP Message available Event.
   This is an InfoEvent which does not include any additional data beside event
   information itself up to now.
   Event informs the upper layer application that SNMP message has to be read
   out from driver using the ioctl \ref DSL_FIO_G997_SNMP_MESSAGE_RECEIVE
   \attention For future versions of the driver it might be necessary to extend
   the event by returning the SNMP data directly within this event. But this
   is not planned up to now. */
   DSL_EVENT_S_SNMP_MESSAGE_AVAILABLE = 19,
   /**
   (20) System Interface Status.
   For a detailed description of the information provided with this event
   please refer to the description of the following structure(s)
   - ref DSL_SystemInterfaceStatus_t */
   DSL_EVENT_S_SYSTEM_INTERFACE_STATUS = 20,
   /**
   (21) FW download status event.
   For a detailed description of the information provided with this event
   please refer to the description of the following structure(s)
   - ref DSL_FwDownloadStatusData_t */
   DSL_EVENT_S_FIRMWARE_DOWNLOAD_STATUS = 21,
   /**
   (22) Autoboot status event.
   For a detailed description of the information provided with this event
   please refer to the description of the following structure(s)
   - ref DSL_AutobootStatusData_t */
   DSL_EVENT_S_AUTOBOOT_STATUS = 22,
   /**
   (23) Multimode FSM status event (system reboot request).
   For a detailed description of the information provided with this event
   please refer to the description of the following structure(s)
   - ref DSL_MultimodeFsmStatus_t */
   DSL_EVENT_S_MULTIMODE_FSM_STATUS = 23,
   /**
   (24) RTT status event.
   For a detailed description of the information provided with this event
   please refer to the description of the following structure(s)
   - ref DSL_RTT_StatusData_t */
   DSL_EVENT_S_RTT_STATUS = 24,
   /**
   alias for last callback event */
   DSL_EVENT_LAST
} DSL_EventType_t;

#if defined(INCLUDE_DSL_CPE_API_VRX)
/**
   Defines the possible types of firmware requests.
*/
typedef enum
{
   /**
   No dedicated information is specified. It is used to activate a API internal
   multimode default handling. */
   DSL_FW_TYPE_NA = 0,
   /**
   ADSL firmware type is specified. */
   DSL_FW_TYPE_ADSL = 1,
   /**
   VDSL firmware type is specified. */
   DSL_FW_TYPE_VDSL = 2,
   /**
   Delimiter only! */
   DSL_FW_TYPE_LAST = 3
} DSL_FirmwareType_t;

/**
   Defines the possible types of activation sequence.
*/
typedef enum
{
   /**
   Standard G.HS activation sequence (using G.994.1 AnnexA) */
   DSL_ACT_SEQ_STD = 0,
   /**
   Non-standard ANSI-T1.413 activation sequence ("Telefonica switching mode") */
   DSL_ACT_SEQ_NON_STD = 1,
   /**
   Delimiter only! */
   DSL_ACT_SEQ_LAST = 2
} DSL_ActivationSequence_t;

/**
   Defines the possible types of activation mode.
*/
typedef enum
{
   /**
   No dedicated information is specified */
   DSL_ACT_MODE_NA = 0,
   /**
   Start with G.HS activation mode */
   DSL_ACT_MODE_GHS = 1,
   /**
   Start with ANSI-T1.413 activation mode */
   DSL_ACT_MODE_ANSI_T1413 = 2,
   /**
   Delimiter only! */
   DSL_ACT_MODE_LAST = 3
} DSL_ActivationMode_t;

/**
   Structure used to initialize the configuration of the
   autoboot FSM activation data.

   \note This config structure is only used within ADSL/VDSL multimode
      handling and in case of using VRX device.
*/
typedef struct
{
   /**
   Within context of ADSL/VDSL multimode handling it defines the ADSL
   activation sequence type. */
   DSL_CFG DSL_ActivationSequence_t nActivationSequence;
   /**
   Within context of ADSL/VDSL multimode handling it defines the ADSL
   activation mode type. This parameter is only effective in case of
   \ref nActivationSequence is set to \ref DSL_ACT_SEQ_NON_STD */
   DSL_CFG DSL_ActivationMode_t nActivationMode;
} DSL_ActivationFsmConfigData_t;

/**
   Structure used to write (SET) or read (GET) the configuration of the
   autoboot FSM.

   \note This config structure is only used within ADSL/VDSL multimode
      handling and in case of using VRX device.
*/
typedef struct
{
   /**
   Within context of ADSL/VDSL multimode handling it defines the xDSL
   transmission mode that is used for the next link activation sequence.
   It should have one of the following values
   - DSL_FW_TYPE_VDSL: VDSL only retry is activated
   - DSL_FW_TYPE_ADSL: ADSL only retry is activated

   \note Setting a value that is different from above listed ones (for example
      DSL_FW_TYPE_NA) results in selecting the default value DSL_FW_TYPE_ADSL. */
   DSL_CFG DSL_FirmwareType_t nNextMode;
} DSL_MultimodeFsmConfigData_t;

/**
   Structure used for getting status information of the autoboot FSM of the
   API.
   This structure has to be used for
   ioctls \ref DSL_FIO_MULTIMODE_FSM_CONFIG_SET ,
          \ref DSL_FIO_MULTIMODE_FSM_CONFIG_SET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains autoboot control data */
   DSL_OUT DSL_MultimodeFsmConfigData_t data;
} DSL_MultimodeFsmConfig_t;

/**
   Structure used for getting status information of the autoboot FSM.
   \note This status structure is only used within ADSL/VDSL multimode handling
         and for the VRX device.
*/
typedef struct
{
   /**
   This parameter indicates if a reboot is requested from the automode FSM.
   This request is signaled within context of switching between ADSL and VDSL
   or vice versa.

   \note Independently from the status of this parameter the nNextMode
         parameter always includes the current setting that is used within
         automode FSM of the API. However, due to that fact this value is
         changed within context of autoboot handling, it should be only taken
         into account if the bRebootRequested is DSL_TRUE. */
   DSL_OUT DSL_boolean_t bRebootRequested;
   /**
   Within context of ADSL/VDSL multimode handling it defines the xDSL transmission
   mode that will be used for the next link activation sequence, which will
   by default take place after reboot of the system.

   \note It is intended that this parameter will be stored within a reboot
         (power cycle) persistent memory before reboot and that it will be
         configured within API after system restart and within context of
         re-starting the DSL CPE Control Application and DSL CPE API. */
   DSL_OUT DSL_FirmwareType_t nNextMode;
} DSL_MultimodeFsmStatusData_t;

/**
   Structure used for getting status information of the autoboot FSM of the
   API.
   This structure has to be used for ioctl \ref DSL_FIO_MULTIMODE_FSM_STATUS_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains autoboot control data */
   DSL_OUT DSL_MultimodeFsmStatusData_t data;
} DSL_MultimodeFsmStatus_t;
#endif /* #if defined(INCLUDE_DSL_CPE_API_VRX)*/

#include "drv_dsl_cpe_api_g997.h"

#if defined(INCLUDE_DSL_PM)
   #include "drv_dsl_cpe_api_pm.h"
#endif

#if defined(INCLUDE_DSL_BONDING)
   #include "drv_dsl_cpe_api_bnd.h"
#endif

/**
   Structure for getting statistics counter that are related to retransmission
   functionality.
*/
typedef struct
{
   /**
   This anomaly occurs when a received DTU with detected errors is not corrected
   by the reception of a DTU, i.e. retransmission didn't correct the corrupted
   DTU. The corresponding anomaly counter counts all corrupt DTUs, also idle
   cells in case of ATM-TC.
   \note This parameter is only valid for NEAR-END direction. */
   DSL_OUT DSL_uint32_t nRxCorruptedTotal;
   /**
   This anomaly occurs when a received DTU, eligible for retransmission, with
   detected errors is not corrected by the reception of a DTU with the same
   SID, i.e. retransmission could not correct the to-be-protected corrupted DTU.
   The corresponding anomaly counter counts only corrupt DTUs which belong to
   the user data stream which shall be impulse noise protected by
   retransmission.
   \note This parameter is only valid for NEAR-END direction. */
   DSL_OUT DSL_uint32_t nRxUncorrectedProtected;
   /**
   This anomaly occurs when a received DTU (Data Transfer Unit) is detected to
   be a retransmission of a previous sent DTU.
   \note This parameter is only valid for NEAR-END direction and in case of
         NOT using VRX200. */
   DSL_OUT DSL_uint32_t nRxRetransmitted;
   /**
   This anomaly occurs when a received DTU with detected errors is corrected by
   the reception of a DTU with the same SID, i.e. a retransmission corrects the
   corrupted DTU.
   \note This parameter is only valid for NEAR-END direction. */
   DSL_OUT DSL_uint32_t nRxCorrected;
   /**
   Counter of retransmitted DTU by the transmitter (rtx-tx): this is a counter
   that is incremented each time a DTU has been retransmitted by the transmitter.
   Multiple retransmission of the same DTU is counted as many times as it has
   been retransmitted. (G.998.4 chapter 12).
   \note This parameter is only valid for FAR-END direction. */
   DSL_OUT DSL_uint32_t nTxRetransmitted;
   /**
   Counter for the number of bits which has been passed from the RTX receiver to
   the next sub-layer.
   \note This parameter is only valid for NEAR-END direction and in case of
         using VRX200. */
   DSL_OUT DSL_uint32_t nErrorFreeBits;
} DSL_ReTxStatisticsData_t;

/**
   Structure for getting statistics counter that are related to retransmission
   functionality.
   This structure has to be used for ioctl
   - \ref DSL_FIO_RETX_STATISTICS_GET

   \note The interface provides the direction parameter to select between
   near-end and far-end values. However there are certain restrictions in
   direction and device specific availability of ReTx counter values. For
   details please refer to the counter parameter description itself.
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Specifies the direction (DSL_NEAR_END / DSL_FAR_END) to which the function
   will apply */
   DSL_IN DSL_XTUDir_t nDirection;
   /**
   Structure that contains statistics data */
   DSL_OUT DSL_ReTxStatisticsData_t data;
} DSL_ReTxStatistics_t;

/**
   Structure for initialization of the DSL CPE API Driver related RTT functionality.
   This structure has to be used for ioctl \ref DSL_FIO_RTT_INIT
*/
typedef struct
{
   /**
   Network connection parameter: IP Address */
   DSL_uint8_t nIpAddress[4];
   /**
   Network connection parameter: Port number */
   DSL_uint16_t nPort;
} DSL_RTT_InitData_t;

/**
   Structure for initialization of DSL CPE_API Driver related RTT functionality.
   This structure has to be used for ioctl \ref DSL_FIO_RTT_INIT
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains initialization data */
   DSL_IN DSL_RTT_InitData_t data;
} DSL_RTT_Init_t;

/**
   Defines the RTT mode (start condition) for RTT-packet collection of the
   firmware.
*/
typedef enum
{
   /**
   Configures the RTT trace dump to be started with showtime entry.
   \note Currently there is only one configuration option but it is intended
         to extend this configuration option in future versions of the SW/FW. */
   DSL_RTT_MODE_SHOWTIME = 0,
   DSL_RTT_MODE_LAST
} DSL_RTT_ModeControlSet_t;


/**
   Structure for configuration of the MEI Driver related RTT functionality.

   \note This configuration parameters are used within API for configuration
         of the firmware using CMV's respective messages within link
         configuration phase and they are also forwarded to the MEI Driver
         for memory partitioning purpose.
*/
typedef struct
{
   /**
   Configures the first tone index to capture FT data. */
   DSL_uint16_t nStartIndex;
   /**
   Configures the number of FFT values to be captured. */
   DSL_uint16_t nSize;
   /**
   Selects the mode (start condition) for RTT-packet collection of the firmware. */
   DSL_RTT_ModeControlSet_t nRttMode;
   /**
   Number of RTT-packets after which data capture stops.
   - 0: Continue data capture (i.e. No stop criteria) (default)
   - N: Number of RTT-packets after which data capture stops.
        Valid values for "N" are 1 to 32767 */
   DSL_uint16_t nStopCriteria;

} DSL_RTT_ConfigData_t;

/**
   Structure for getting statistics counter that are related to RTT packet
   processing.
   This structure has to be used for ioctl
   - \ref DSL_FIO_RTT_CONFIG_SET
   - \ref DSL_FIO_RTT_CONFIG_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains configuration data */
   DSL_IN DSL_RTT_ConfigData_t data;
} DSL_RTT_Config_t;

/**
   Defines all possible firmware status values of RTT processing.
*/
typedef enum
{
   /**
   RTT idle. */
   DSL_RTT_FW_STATUS_IDLE = 0,
   /**
   RTT data capturing in progress. */
   DSL_RTT_FW_STATUS_IN_PROGRESS = 1,
   /**
   RTT data capturing in completed. */
   DSL_RTT_FW_STATUS_COMPLETED = 2,
   /**
   RTT data capturing in aborted. In this case please also refer to the value
   of nFwFailReason which includes a failure code. */
   DSL_RTT_FW_STATUS_ABORTED = 3,
   /**
   Delimiter only. */
   DSL_RTT_FW_STATUS_LAST
} DSL_RTT_FwStatus_t;

/**
   Defines all possible firmware failure reasons in case of aborted RTT
   data capturing processing.
*/
typedef enum
{
   /**
   No failure. */
   DSL_RTT_FW_FAIL_REASON_NONE = 0,
   /**
   Failure due to invalid configuration. */
   DSL_RTT_FW_FAIL_REASON_INVALID_CONFIG = 1,
   /**
   Failure due to not supported RTT functionality. */
   DSL_RTT_FW_FAIL_REASON_NOT_SUPPORTED = 2,
   /**
   Delimiter only. */
   DSL_RTT_FW_FAIL_REASON_LAST
} DSL_RTT_FwFailReason_t;

/**
   Structure that includes status information about the RTT processing within
   DSL CPE API, MEI Driver and firmware.
*/
typedef struct
{
   /**
   Returns information about the firmware RTT status. */
   DSL_RTT_FwStatus_t nFwStatus;
   /**
   Returns information about the firmware RTT failure reason. */
   DSL_RTT_FwFailReason_t nFwFailReason;
   /**
   Returns information about the software RTT status. */
   DSL_Error_t nSwErrorCode;

} DSL_RTT_StatusData_t;

/**
   Structure for getting the status of the Real Time Trace (RTT) processing.
   This structure has to be used for ioctl
   - \ref DSL_FIO_RTT_STATUS_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains configuration data */
   DSL_IN DSL_RTT_StatusData_t data;
} DSL_RTT_Status_t;

/**
   Structure for enabling or disabling (stopping) of the MEI Driver related RTT
   functionality.
*/
typedef struct
{
   /**
   Configures to enable or disable the capturing of RTT-packets within context
   of next link activation sequence respectively stops any running capturing.
   - DSL_TRUE: Activates the capturing of RTT data
     \note In case of using this ioctl after the link has been activated already
           (\ref DSL_LineStateValue_t equals or higher than 0x200) the autoboot
           handling has to be restarted (\ref DSL_FIO_AUTOBOOT_CONTROL_SET with
           selecting \ref  DSL_AutobootCtrlSet_t equals
           \ref DSL_AUTOBOOT_CTRL_RESTART (2)) to make this setting effective.
   - DSL_FALSE: Deactivates or stops the capturing of RTT data
     \note In case of currently active RTT capturing within firmware this
           capturing is immediately stopped.
           In case of NO active data capturing within firmware it is just not
           started. In this case it is also not activated within next link
           activation sequence. */
   DSL_boolean_t nRttEnable;

} DSL_RTT_ControlData_t;

/**
   Structure for enabling or disabling (stopping) of DSL CPE_API Driver related
   RTT functionality respective processing.
   This structure has to be used for ioctl
   - \ref DSL_FIO_RTT_CONTROL_SET_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains initialization data */
   DSL_IN DSL_RTT_ControlData_t data;
} DSL_RTT_Control_t;


/**
   Structure for getting statistics counter that are related to RTT packet
   processing.
*/
typedef struct
{
   /**
   Includes information about how many RTT-packets has been successfully
   processed (written to the first level shared circular buffer) within firmware.
   \note This is a running 32-bit wrap around counter. */
   DSL_uint32_t nFwPacketsSend;
   /**
   Includes information about how many RTT-packets has been dropped by the
   firmware. The firmware will for example drop a packet in case of
   - an internal firmware error within trace dump processing (due to DMA not
     over in a symbol period)
   - an overflow of the first level shared circular buffer
   \note This is a running 32-bit wrap around counter. */
   DSL_uint32_t nFwPacketsDropped;
   /**
   Includes information about how many RTT-packets has been successfully
   processed (send via UDP/TCP) within MEI Driver.
   \note This is a running 32-bit wrap around counter. */
   DSL_uint32_t nSwPacketsSend;
   /**
   Includes information about how many RTT-packets has been dropped by the
   software. The software will for example drop a packet in case of
   - an overflow of the second level FIFO buffer
   \note This is a running 32-bit wrap around counter. */
   DSL_uint32_t nSwPacketsDropped;
} DSL_RTT_StatisticsData_t;

/**
   Structure for getting statistics counter that are related to RTT packet
   processing.
   This structure has to be used for ioctl
   - \ref DSL_FIO_RTT_STATISTICS_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains statistics data */
   DSL_IN DSL_RTT_StatisticsData_t data;
} DSL_RTT_Statistics_t;

/**
   encapsulate all event data/status structures
*/
typedef union
{
   DSL_LineStateData_t                           lineStateData;
   DSL_FirmwareRequestData_t                     fwRequestData;
   DSL_SystemInterfaceConfigData_t               systemInterfaceStatusData;
   DSL_G997_ChannelStatusData_t                  channelStatusData;
   DSL_G997_LineInitStatusData_t                 lineInitStatusData;
   DSL_G997_DataPathFailuresData_t               dataPathFailuresData;
   DSL_G997_LineFailuresData_t                   lineFailuresData;
   DSL_G997_DataRateShiftThresholdCrossingData_t dataRateThresholdCrossing;
   DSL_G997_PowerManagementStatusData_t          powerMgmtStatusData;
   DSL_FwDownloadStatusData_t                    fwDownloadStatus;
   DSL_AutobootStatusData_t                      autobootStatus;

#if defined(INCLUDE_DSL_CPE_API_DANUBE)
   DSL_ReTxStatisticsData_t                      reTxStatistics;
#endif
#if defined(INCLUDE_DSL_CPE_API_VRX)
   DSL_MultimodeFsmStatusData_t                  multimodeFsmStatusData;
#endif
#if defined(INCLUDE_DSL_PM)
   #if defined(INCLUDE_DSL_CPE_PM_CHANNEL_THRESHOLDS)
   DSL_PM_ChannelThresholdCrossing_t             channelThresholdCrossing;
   #endif
   DSL_PM_LineThresholdCrossing_t                lineThresholdCrossing;
   #if defined(INCLUDE_DSL_CPE_PM_DATA_PATH_THRESHOLDS)
   DSL_PM_DataPathThresholdCrossing_t            dataPathThresholdCrossing;
   #endif
   #if defined(INCLUDE_DSL_CPE_PM_RETX_THRESHOLDS)
   DSL_PM_ReTxThresholdCrossing_t                reTxThresholdCrossing;
   #endif
   #if defined(INCLUDE_DSL_CPE_PM_HISTORY)
   DSL_PM_SyncEvent_t                            sync;
   #endif
#endif
#if defined(INCLUDE_REAL_TIME_TRACE)
   DSL_RTT_StatusData_t                          rttStatusData;
#endif /*#if defined(INCLUDE_REAL_TIME_TRACE)*/
} DSL_EventData_Union_t;


/**
   Defines all possible classes of resource activation types.
   Each resource activation type that is listed, belongs to an internal memory
   resource (FIFO) which is allocated on activation respectively freed on
   deactivation.
*/
typedef enum
{
   /* Cleaned. */
   DSL_RESOURCE_ACTIVATION_CLEANED = 0x00000000,
   /**
   Activates/deactivates SNMP resource buffer.
   Creates a FIFO on activation which includes a number of elements that is
   defined by DSL_CEOC_RX_FIFO_ELEMENT_COUNT (number of max. stored events).
   Each elements size is defined by size of structure DSL_CEOC_Message_t. */
   DSL_RESOURCE_ACTIVATION_SNMP = 0x00000001
} DSL_BF_ResourceActivationType_t;

/**
   Structure that is used to initialize the event handling of the DSL CPE API
   driver. Within this context all necessary FIFO memory will be allocated.
*/
typedef struct
{
   /**
   Specifies to activate or deactivate the event handling.
   Creates a FIFO on activation that includes a number of elements that is
   defined by \ref DSL_EVENT_FIFO_ELEMENT_COUNT (number of max. stored events).
   Each elements size is defined by size of union DSL_EventData_Union_t.
   \attention After activation of events for the current instance, the initial
              state for all following events is masked (not signaled)!
              To unmask all events that are intended to be handled by opening
              instance please use the following functions for status events
              - \ref DSL_FIO_EVENT_STATUS_MASK_CONFIG_SET
              and for indication events
              - \ref DSL_FIO_G997_ALARM_MASK_LINE_FAILURES_CONFIG_SET
              - \ref DSL_FIO_G997_ALARM_MASK_DATA_PATH_FAILURES_CONFIG_SET
   \attention Indication events for Line, Channel- and Datapath Threshold
              Crossing as well as Channel Datarate Shift Threshold Crossing can
              be controlled by setting the appropriate threshold value. */
   DSL_boolean_t bEventActivation;
   /**
   Specifies the resource activation mask. */
   DSL_BF_ResourceActivationType_t nResourceActivationMask;
} DSL_InstanceControlData_t;

/**
   Structure that is used to configure instance specific event and resource
   management settings.
   This structure has to be used for ioctl \ref DSL_FIO_INSTANCE_CONTROL_SET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains status event mask data */
   DSL_IN DSL_InstanceControlData_t data;
} DSL_InstanceControl_t;

/**
   Structure that is used to get information about current configuration of
   instance specific event and resource management settings.
   This structure has to be used for ioctl \ref DSL_FIO_INSTANCE_STATUS_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains status event mask data */
   DSL_OUT DSL_InstanceControlData_t data;
} DSL_InstanceStatus_t;

/**
   Structure that is used to provide extended internal event buffer related
   status information.
*/
typedef struct
{
   /**
   Returns the total size that is available for each instance that has
   activated the event handling. The size is given in number of elements where
   each element has the size of the union \ref DSL_IOCTL_arg_t. */
   DSL_OUT DSL_uint32_t nEventBufferSize;
   /**
   Returns the number of elements that are currently used from within total
   available number of elements. */
   DSL_OUT DSL_uint32_t nEventBufferFillStatus;
} DSL_EventStatusExt_t;

/**
   Structure that is used to inform the upper layer software about an event
   that has been occurred.
*/
typedef struct
{
   /**
   Specifies for which (bearer) channel the function will apply.
   This parameter is used for channel specific events only. If the parameter is
   not used it is set to \ref DSL_CHANNEL_NA . */
   DSL_OUT DSL_uint8_t nChannel;
   /**
   Specifies for which direction (upstream/downstream) the function will
   apply.
   This parameter is only used for direction specific call backs. If the
   parameter is not used it is set to \ref DSL_ACCESSDIR_NA . */
   DSL_OUT DSL_AccessDir_t nAccessDir;
   /**
   Specifies the direction (DSL_NEAR_END / DSL_FAR_END) to which the function
   will apply.
   If the parameter is not used it is set to \ref DSL_XTUDIR_NA . */
   DSL_OUT DSL_XTUDir_t nXtuDir;
   /**
   Specifies the event type (reason of event). Regrading to the setting of this
   value the data which is included in the following union has different
   meanings. Please refer to the description of the union to get information
   about the meaning of the included data.
   For some event (InfoEvents) there is only this event type set. The union
   does not include any data in this case because the upper layer software is
   responsible to read the according ioctl to retrieve according data if
   necessary. */
   DSL_OUT DSL_EventType_t nEventType;
   /**
   Provides extended internal event buffer related status information. */
   DSL_OUT DSL_EventStatusExt_t nEventStatusExt;
   /**
   Data according to \ref DSL_EventData_Union_t */
   DSL_OUT DSL_EventData_Union_t *pData;
} DSL_EventStatusData_t;

/**
   Structure that is used to inform the upper layer software about an event
   that has been occurred.
   This structure has to be used for ioctl \ref DSL_FIO_EVENT_STATUS_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains status event mask data */
   DSL_OUT DSL_EventStatusData_t data;
} DSL_EventStatus_t;


/**
   Structure that is used to read or write configuration for status event
   handling events. Events might be masked (not signaled) or unmasked
   (signaled).
*/
typedef struct
{
   /**
   Specifies the status events that should be handled. The value shall only be
   in the range of DSL_EVENT_S_FIRST...DSL_EVENT_S_LAST because this structure
   is used to handle status events only. */
   DSL_IN DSL_EventType_t nEventType;
   /**
   Specifies or returns the state of the event according to access type
   (get/set).
   - DSL_TRUE : Event masked (not signaled)
   - DSL_FALSE: Event unmasked (signaled) */
   DSL_CFG DSL_boolean_t bMask;
} DSL_EventStatusMaskData_t;

/**
   Structure that is used to read (GET) or write (SET) configuration for status
   event handling events. Events might be masked (not signaled) or unmasked
   (signaled).
   This structure has to be used for ioctl
   - \ref DSL_FIO_EVENT_STATUS_MASK_CONFIG_SET
   - \ref DSL_FIO_EVENT_STATUS_MASK_CONFIG_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains status event mask data */
   DSL_CFG DSL_EventStatusMaskData_t data;
} DSL_EventStatusMask_t;

/** @} DRV_DSL_CPE_EVENT */

/**
   Defines all possible Annex types as stored in the firmware
*/
typedef enum
{
   DSL_FW_ANNEX_NA = -1,
   DSL_FW_ANNEX_B_J = 0,
   DSL_FW_ANNEX_A = 1,
   DSL_FW_ANNEX_B = 2
} DSL_FirmwareAnnex_t;

/** \addtogroup DRV_DSL_CPE_INIT
 @{ */

/**
   Device specific initialization data.
*/
typedef struct DSL_DeviceConfig DSL_DeviceConfig_t;

#if defined (INCLUDE_DSL_CPE_API_DANUBE)
   #include "drv_dsl_cpe_danube_ctx.h"
#endif

#ifdef INCLUDE_DSL_CPE_API_VINAX
   #include "drv_dsl_cpe_api_sar.h"
   #include "drv_dsl_cpe_vinax_ctx.h"
#endif

#ifdef INCLUDE_DSL_CPE_API_VRX
   #include "drv_dsl_cpe_vrx_ctx.h"
#endif

/**
   Structure for Device Low level configuration.
   \if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
   This structure has to be used for ioctl
   - \ref DSL_FIO_LOW_LEVEL_CONFIGURATION_SET
   - \ref DSL_FIO_LOW_LEVEL_CONFIGURATION_GET
   \endif
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains low level configuration data */
   DSL_DeviceLowLevelConfig_t data;
} DSL_LowLevelConfiguration_t;

/**
   Defines the possible firmware feature sets.

   \note For VRX there will be usually two bits set according to the combination
         of firmware binaries that includes by default always VDSL2 plus ADSL
         AnnexA or AnnexB firmware.
*/
typedef enum
{
   /**
   Firmware includes ADSL Annex A feature set. */
   DSL_FW_FEATURES_CLEANED = 0x0000,
   /**
   Firmware includes ADSL Annex A feature set. */
   DSL_FW_FEATURES_ADSL_A  = 0x0001,
   /**
   Firmware includes ADSL Annex B feature set. */
   DSL_FW_FEATURES_ADSL_B  = 0x0002,
   /**
   Firmware includes VDSL2 feature set. */
   DSL_FW_FEATURES_VDSL2   = 0x0004,
   /**
   Delimiter only! */
   DSL_FW_FEATURES_LAST    = 0x0008
} DSL_FirmwareFeatures_t;

/**
   Structure for firmware download handling of DSL CPE_API Driver.

   It is possible to do the firmware download by using chunks of a defined size.
   In this case...
   - the data includes only a chunk part of the complete firmware binary
   - the "load firmware" functionality has to be called multiple times
   - The offset has to be used for consecutive function calls

   \attention For a detailed description of the parameters that specifies the
              firmware binaries please refer to the structure \ref DSL_Init_t
              which includes parameters that have the same meaning.
*/
typedef struct
{
   /**
   Pointer to firmware binary. */
   DSL_IN DSL_uint8_t *pFirmware;
   /**
   Size of firmware binary.
   This value is only valid if pFirmware pointer is NOT equal DSL_NULL */
   DSL_IN DSL_uint32_t nFirmwareSize;
   /**
   Defines the features that are supported by the currently used firmware binary.
   \note This information is mandatory in case of VRX multimode handling and
         informal in case of all other devices. */
   DSL_IN DSL_FirmwareFeatures_t nFirmwareFeatures;
   /**
   Pointer to 2nd firmware binary. */
   DSL_IN DSL_uint8_t *pFirmware2;
   /**
   Size of the 2nd firmware binary.
   This value is only valid if pFirmware2 pointer is NOT equal DSL_NULL*/
   DSL_IN DSL_uint32_t nFirmwareSize2;
   /**
   Offset within complete firmware binary.
   This values has to be used in case of using chunks for firmware download.
   In this case it has to be zero for the first call and
   (chunk size * index number of call) for consecutive calls. */
   DSL_IN DSL_uint32_t nFirmwareOffset;
   /**
   Firmware download via chunks enabled.
   This value should be always true in case of firmware download via chunks.*/
   DSL_IN DSL_boolean_t bChunkDonwloadEnabled;
   /**
   Last firmware chunk flag.
   This value should be always true in case of download whole firmware binary.
   In case of using chunks it should be true for last download call */
   DSL_IN DSL_boolean_t bLastChunk;
} DSL_AutobootLoadFirmwareData_t;

/**
   Structure for firmware download handling of DSL CPE_API Driver.
   This structure has to be used for ioctl \ref DSL_FIO_AUTOBOOT_LOAD_FIRMWARE
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains firmware data */
   DSL_IN DSL_AutobootLoadFirmwareData_t data;
} DSL_AutobootLoadFirmware_t;


/**
   Defines the control functionalities of the autoboot handling
*/
typedef enum
{
   /**
   Stops the autoboot handling. */
   DSL_AUTOBOOT_CTRL_STOP = 0,
   /**
   Starts the autoboot handling. */
   DSL_AUTOBOOT_CTRL_START = 1,
   /**
   Restarts the firmware and the DSL CPE API for usage of updated configurations. */
   DSL_AUTOBOOT_CTRL_RESTART = 2,
   /* Continues at any wait state of the autoboot handling.
      \note This control trigger is only used if autoboot status is
      DSL_AUTOBOOT_STATUS_xxx_WAIT
      otherwise it will be ignored and a special
      warning code DSL_WRN_NOT_ALLOWED_IN_CURRENT_STATE will be returned.
      \note Currently there is only one wait on link activation used.*/
   DSL_AUTOBOOT_CTRL_CONTINUE = 3
} DSL_AutobootCtrlSet_t;

/**
   Structure used for controlling the autoboot functionality.
   \remark This handling is necessary in any case if not supported by firmware
           automatically.
*/
typedef struct
{
   /**
   Command for autoboot handling. */
   DSL_IN DSL_AutobootCtrlSet_t nCommand;
} DSL_AutobootControlData_t;

/**
   Structure used for controlling the autoboot functionality.
   This structure has to be used for ioctl \ref DSL_FIO_INIT
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains autoboot control data */
   DSL_IN DSL_AutobootControlData_t data;
} DSL_AutobootControl_t;

/**
   Structure used for controlling of special features within the autoboot
   functionality.
*/
typedef struct
{
   /**
   Activates a special handling that uses an additional state within autoboot
   handling to wait after firmware download and BEFORE basic configuration and
   line/link activation.
   This handling is intended to be used for extended autoboot script handling
   and makes it possible to write additional configuration by using DSL CPE API
   ioctls that updates the internal database within DSL CPE API that will be
   directly used for current activation after continuing.
   If this option is activated (DSL_TRUE) the autoboot waits after firmware
   download and BEFORE basic configuration (autoboot state
   \ref DSL_AUTOBOOT_STATUS_CONFIG_WRITE_WAIT) and has to be resumed using
   autoboot control \ref DSL_AUTOBOOT_CTRL_CONTINUE.
   In case of working on event basis the line state 0x100
   (DSL_LINESTATE_IDLE) can be used to identify the "config write wait"
   autoboot state. After idle line state is reached the autoboot status has to
   be requested by using \ref DSL_FIO_AUTOBOOT_STATUS_GET ioctl in this case to
   decide which is the current wait condition. */
   DSL_IN_OUT DSL_boolean_t bWaitBeforeConfigWrite;
   /**
   Activates a special handling that uses an additional state within autoboot
   handling to wait AFTER firmware download and basic configuration (does not
   activate the line/link)  automatically).
   This handling is intended to be used for extended autoboot script handling
   and makes it possible to write additional device configuration by using
   tunnel messages (ioctl \ref DSL_FIO_DBG_DEVICE_MESSAGE_SEND) before the
   line/link activation.
   If this option is activated (DSL_TRUE) the autoboot waits AFTER firmware
   download and basic configuration (autoboot state
   \ref DSL_AUTOBOOT_STATUS_LINK_ACTIVATE_WAIT) and has to be resumed using
   autoboot control \ref DSL_AUTOBOOT_CTRL_CONTINUE.
   In case of working on event basis the line state 0x100
   (DSL_LINESTATE_IDLE) can be used to identify the "link activate wait"
   autoboot state. After idle line state is reached the autoboot status has to
   be requested by using \ref DSL_FIO_AUTOBOOT_STATUS_GET ioctl in this case to
   decide which is the current wait condition. */
   DSL_IN_OUT DSL_boolean_t bWaitBeforeLinkActivation;
   /**
   Activates a special handling that uses an additional state within autoboot
   handling to wait BEFORE firmware download in the device FAIL state.
   This handling is intended to be used for extended autoboot script handling
   and makes it possible to read device status before FW download  in FAIL state)
   by using tunnel messages (ioctl \ref DSL_FIO_DBG_DEVICE_MESSAGE_SEND).
   If this option is activated (DSL_TRUE) the autoboot waits BEFORE firmware
   download(autoboot state
   \ref DSL_AUTOBOOT_STATUS_RESTART_WAIT) and has to be resumed using
   autoboot control \ref DSL_AUTOBOOT_CTRL_CONTINUE.
   In case of working on event basis the line state 0x1
   (DSL_LINESTATE_EXCEPTION) can be used to identify the "restart wait"
   autoboot state. After exception line state is reached the autoboot status has to
   be requested by using \ref DSL_FIO_AUTOBOOT_STATUS_GET ioctl in this case to
   decide which is the current wait condition. */
   DSL_IN_OUT DSL_boolean_t bWaitBeforeRestart;
} DSL_AutobootStateMachineOptions_t;

/**
   Structure for configuration of autoboot state machine behavior.
*/
typedef struct
{
   /**
      Special options to configure the behavior of the autoboot state
      machine handling. */
   DSL_IN_OUT DSL_AutobootStateMachineOptions_t nStateMachineOptions;
} DSL_AutobootConfigData_t;

/**
   Structure for configuration of autoboot state machine behavior.
   This structure has to be used for ioctl
   - \ref DSL_FIO_AUTOBOOT_CONFIG_GET
   - \ref DSL_FIO_AUTOBOOT_CONFIG_SET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains autoboot configuration data */
   DSL_IN_OUT DSL_AutobootConfigData_t data;
} DSL_AutobootConfig_t;

/**
   Structure for initialization of DSL CPE_API Driver.

   \note It is possible to set the firmware pointer to DSL_NULL. In this case
         the following procedure has to be used separately
         - the firmware has to be downloaded later by using ioctl
           \ref DSL_FIO_AUTOBOOT_LOAD_FIRMWARE
         - the autoboot handling has to be started afterwards by using ioctl
           \ref DSL_FIO_AUTOBOOT_CONTROL_SET
         The advantage of this procedure is that the firmware can be downloaded
         using chunks of a defined size by using "load firmware" functionality
         multiple times. This reduces the amount of memory that has to be
         allocated for firmware download functionality.
*/
typedef struct
{
   /**
   Pointer to firmware binary.
   According to the device for which the DSL CPE API driver has been compiled
   this pointer is defined as follows
   - Danube: The firmware binary that will be provided with this pointer is the
     one and only firmware that will be used
   - Vinax: The firmware binary that will be provided has to have VDSL
     functionality included if DSL CPE API driver shall support VDSL
     transmission modes. */
   DSL_IN DSL_uint8_t *pFirmware;
   /**
   Size of firmware binary.
   This value is only valid if pFirmware pointer is NOT equal DSL_NULL */
   DSL_IN DSL_uint32_t nFirmwareSize;
   /**
   Defines the features that are supported by the currently used firmware binary.
   \note This information is mandatory in case of VRX multimode handling and
         informal in case of all other devices. */
   DSL_IN DSL_FirmwareFeatures_t nFirmwareFeatures;
   /**
   Pointer to 2nd firmware binary.
   According to the device for which the DSL CPE API driver has been compiled
   this pointer is defined as follows
   - Danube: This parameter is NOT used and has to be set to DSL_NULL
   - Vinax: The firmware binary that will be provided has to have ADSL
     functionality included if DSL CPE API driver shall support ADSL
     transmission modes.
   \note For future versions of Vinax firmware which includes both VDSL and
         ADSL functionality (as "real" standalone functionality) this parameter
         will be not necessary anymore and can be set to DSL_NULL. */
   DSL_IN DSL_uint8_t *pFirmware2;
   /**
   Size of the 2nd firmware binary.
   This value is only valid if pFirmware2 pointer is NOT equal DSL_NULL*/
   DSL_IN DSL_uint32_t nFirmwareSize2;
   /**
   Defines the transmission modes that shall be activated within autoboot
   handling after firmware download */
   DSL_IN DSL_G997_XTUSystemEnablingData_t nXtseCfg;
   /**
   Defines the near end line inventory information that will be programmed
   within initialization procedure between firmware download and start of
   autoboot handling */
   DSL_IN DSL_G997_LineInventoryNeData_t *pInventory;
   /**
   Defines the startup mode of the autoboot handling.
   In special cases it might be necessary that the autoboot handling will be
   not started automatically. In this case the value DSL_AUTOBOOT_CTRL_STOP (0)
   has to be used and the upper layer application is responsible to start the
   autoboot handling later on.
   All other values are starting the autoboot handling automatically */
   DSL_IN DSL_AutobootCtrlSet_t nAutobootStartupMode;
   /**
   Defines extended configuration options for autoboot behavior */
   DSL_IN DSL_AutobootConfigData_t nAutobootConfig;
   /** Testmode Control*/
   DSL_IN DSL_TestModeControlSet_t nTestModeControl;
   /** Line Activation Configuration options*/
   DSL_IN DSL_G997_LineActivateData_t nLineActivateConfig;
   /**
   Defines device configuration parameters*/
   DSL_IN DSL_DeviceConfig_t nDeviceCfg;
} DSL_InitData_t;

/**
   Structure for initialization of DSL CPE_API Driver.
   This structure has to be used for ioctl \ref DSL_FIO_INIT
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains initialization data */
   DSL_IN DSL_InitData_t data;
} DSL_Init_t;

/**
   Defines the control functionalities of the autoboot handling
   \note Please note that this configurations are only used for Danube,
         Amazon-SE and ARX100
*/
typedef enum
{
   /**
   Activates the DSL CPE API internal default handling of SNR Margin reboot
   criteria according to the following definition:
   - Reboot criteria will be ALWAYS handled automatically by the API
     (independently from the FW feature set!)
   - AnnexA(/I/L/M)
     SnrMinMargin that is provided by the CO will be ignored for ALL AnnexA
     modes (ADSL1/2/2+). Instead of a value of 0 dB will be assumed.
   - AnnexB(J/M)
     SnrMinMargin that is provided by the CO will be used for ALL AnnexB
     modes (ADSL1/2/2+). */
   DSL_SNRM_REBOOT_AUTOMODE_API = 0,
   /**
   If this mode is activated the SnrMinMargin that is provided by the CO
   will be always used for ALL annexes (A/I/L/M/B/J) and modes (ADSL1/2/2+).
   \attention Activating this mode might cause interoperability problems in
              in case of being connected to a ATU-C that provides a value
              for MinSnrMargin that does not make sense. Result can be that
              the autoboot will be always restarted after reaching showtime
              in case of using a negative value that has been provided by
              the ATU-C and which can not be fulfilled. */
   DSL_SNRM_REBOOT_AUTOMODE_FE = 1,
   /**
   This is the default configuration at startup.
   If this mode is activated it depends on the firmware feature set which
   handling is used in the DSL CPE API as follows
   - For feature set 2 and lower firmware versions the handling of
     \ref DSL_SNRM_REBOOT_AUTOMODE_API is used (DSL CPE API internal default
     handling is used)
   - For feature set 3 and higher firmware versions the firmware reboot
     functionality is used (NO DSL CPE API internal handling of reboot
     criteria for MinSnrMargin) */
   DSL_SNRM_REBOOT_AUTOMODE_FW = 2,
   /**
   If this mode is activated the SnrMinMargin that is provided by the CO
   will be ignored for ALL annexes (A/I/L/M/B/J) and modes (ADSL1/2/2+).
   Instead of a value of 0 dB will be assumed. */
   DSL_SNRM_REBOOT_MANUAL_OFF = 3,
   /**
   If this mode is activated it is possible to configure the value that should
   be used for SnrMinMargin reboot criteria. This value will be used instead of
   the CO provided one, for ALL annexes (A/I/L/M/B/J) and modes (ADSL1/2/2+). */
   DSL_SNRM_REBOOT_MANUAL_USER = 4,
   /**
   Delimiter only */
   DSL_SNRM_REBOOT_LAST = 5
} DSL_SnrMarginRebootMode_t;

/**
   Structure used for configuration of MinSnrMarging reboot criteria.
   \note Please note that this configurations are only used for Danube
         Amazon-SE and ARX100
*/
typedef struct
{
   /**
   Selects the handling for MinSnrMarging reboot criteria that should
   be used within DSL CPE API. */
   DSL_IN_OUT DSL_SnrMarginRebootMode_t nSnrMarginRebootMode;
   /**
   Specifies the user defined value for MinSnrMargin reboot criteria.
   The value ranges from  -32 dB to +31 dB with 0.1 dB steps (-320..310).
   The default configuration value is 0 (dB).
   \note Please note that this value will be only taken into account in case
         of user defined SnrMarginReboot mode (\ref DSL_SnrMarginRebootMode_t
         equals \ref DSL_SNRM_REBOOT_MANUAL_USER) */
   DSL_IN_OUT DSL_int16_t nUserMinSnrMargin;
} DSL_SnrMarginRebootCfg_t;

/**
   Structure for configuration of interoperability issues.
*/
typedef struct
{
   /**
   Select alternative Tx POTS highpass filter for France Telecom
   requirements.
   DSL_FALSE - Use default filter (Default)
   DSL_TRUE  - Use alternative filter for reduced Tx PSD */
   DSL_IN_OUT DSL_boolean_t bFtTxPotsHp;
   /**
   Selects the handling that is used for handling of Min. SNR margin
   reboot handling */
   DSL_IN_OUT DSL_SnrMarginRebootCfg_t nSnrMarginRebootCfg;
   /**
   Controls the disabling of the L2 mode.
   DSL_FALSE - L2 mode is enabled (Default)
   DSL_TRUE  - L2 mode is disabled */
   DSL_IN_OUT DSL_boolean_t bL2modeDisable;
   /**
   Controls the disabling of the L3 mode.
   DSL_FALSE - L3 mode is enabled (Default)
   DSL_TRUE  - L3 mode is disabled */
   DSL_IN_OUT DSL_boolean_t bL3modeDisable;
   /**
   Provides the possibility to switch off the far-end parameter request.
   If this option is activated the upstream parameters SNR, LATN and SATN within
   context of \ref DSL_FIO_G997_LINE_STATUS_GET with nDirection equals US are
   not requested via overhead channel on ioctl access. Instead of the values
   will be returned that are received by the xTU-R firmware during training
   procedure.
   - DSL_FALSE - DSL CPE API requests the line status values for upstream
     directly on ioctl call via overhead channel (blocking call).
     If this fails training values are returned instead.
     This is the DEFAULT configuration.
   - DSL_TRUE  - no request of line status values via overhead channel. The
     training values are always returned.
   \note This handling shall be only activated if there are problems with the
         receive processing of upstream parameter within context of overhead
         channel. For example in case the xTU-C side does not support this
         communication channel at all activating this option will speed up the
         request time for G.997.1 line status upstream parameters with no
         functional restriction (because there will be always a timeout on
         overhead channel in that case and the training values will be returned
         in any case). */
   DSL_IN_OUT DSL_boolean_t bFeRequestOff;
} DSL_InteropFeatureConfigData_t;

/**
   Structure for configuration of interoperability issues.
   This structure has to be used for ioctl
   - \ref DSL_FIO_INTEROP_FEATURE_CONFIG_GET
   - \ref DSL_FIO_INTEROP_FEATURE_CONFIG_SET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains interoperability configuration data */
   DSL_IN_OUT DSL_InteropFeatureConfigData_t data;
} DSL_InteropFeatureConfig_t;

/**
   This function will allocate memory and create all devices.
   This function is intended for VxWorks and eCos. It has to be called
   during system startup.
   Linux is using the module entry points instead.

   \return
   0 - in case of success
*/
int DSL_DRV_DeviceCreate(void);

/**
   This function will free memory and remove all devices.
   This function is intended for VxWorks and eCos.
   Linux is using the module entry points instead.
*/
void DSL_DRV_DeviceDelete(void);

/** @} DRV_DSL_CPE_INIT */


/** \addtogroup DRV_DSL_CPE_DEBUG
 @{ */

/**
   Structure used to read (GET) or write (SET) the configuration of a debug
   module level. This is used to activate/deactivate dedicated debug printout or
   get the current configuration status.
*/
typedef struct
{
   /**
   Specifies for which debug module the level should be get/set */
   DSL_IN DSL_debugModules_t nDbgModule;
   /**
   Specifies/returns the debug level. This value has to be one of the defined
   debug levels (\ref DSL_debugLevels_t).*/
   DSL_CFG DSL_debugLevels_t nDbgLevel;
} DSL_DBG_ModuleLevelData_t;

/**
   Structure used to read (GET) or write (SET) the configuration of a debug
   module level.
   This structure has to be used for ioctl
   - \ref DSL_FIO_DBG_MODULE_LEVEL_SET
   - \ref DSL_FIO_DBG_MODULE_LEVEL_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains debug configuration data */
   DSL_CFG DSL_DBG_ModuleLevelData_t data;
} DSL_DBG_ModuleLevel_t;

/**
   Structure used for sending plain byte data and low level device messages
*/
typedef struct
{
   /**
   Message data */
   DSL_IN_OUT DSL_uint8_t *pMsg;
   /**
   Size of message included in pMsg. The size of the message has to be given
   in bytes! */
   DSL_IN DSL_uint16_t nSizeTx;
   /**
   Maximal Size of pMsg buffer. This value will be checked if a possible answer
   to the command can be stored in the buffer. */
   DSL_IN_OUT DSL_uint16_t nSizeRx;
} DSL_DeviceMessageData_t;

/**
   Structure used for sending plain byte data and low level device messages
   This structure has to be used for ioctl
   - \ref DSL_FIO_DBG_DEVICE_MESSAGE_SEND
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains device message data */
   DSL_IN_OUT DSL_DeviceMessageData_t data;
} DSL_DeviceMessage_t;

#ifndef DSL_DEBUG_DISABLE
#ifdef INCLUDE_DSL_CPE_API_DANUBE
/**
   Definition of possible debug features.
   According to the used selector the value of
   \ref DSL_DebugFeatureConfigData_t :: nFeatureValue
   has the meaning as described within selector value.
*/
typedef enum
{
   /* Data LED behavior. Please refer to the - \ref DSL_DataLedBehavior_t
      structure for the valid selection*/
   DSL_DFC_DATA_LED_BEHAVIOR = 0,
   /* Timeout value expressed in [ms].
      Optional parameter to specify Data LED timeout before consecutive
      Data LED blink triggers. Used only in conjunction with DAL_DATA_LED_BLINK.
      Valid range is: 1...10000 with 1 ms steps,
      Default value is 1000 ms*/
   DSL_DFC_DATA_LED_BLINK_TIMEOUT = 1
} DSL_DebugFeatureSelector_t;

/**
   DATA LED Simulator control data
   This structure has to be used for ioctl
   - \ref DSL_FIO_DBG_DEBUG_FEATURE_CONFIG_SET
   - \ref DSL_FIO_DBG_DEBUG_FEATURE_CONFIG_SET
*/
typedef struct
{
   /**
   Specifies debug feature option to set or get. */
   DSL_DebugFeatureSelector_t nConfigSelector;
   /**
   The meaning of the value depends on the selected feature
   \ref DSL_DebugFeatureSelector_t. */
   DSL_int32_t nFeatureValue;
} DSL_DebugFeatureConfigData_t;

/**
   Structure used for controling debug features
   This structure has to be used for ioctl
   - \ref DSL_FIO_DBG_DEBUG_FEATURE_CONFIG_SET
   - \ref DSL_FIO_DBG_DEBUG_FEATURE_CONFIG_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains debug features data */
   DSL_IN DSL_DebugFeatureConfigData_t data;
} DSL_DBG_DebugFeatureConfig_t;
#endif /* INCLUDE_DSL_CPE_API_DANUBE*/
#endif /* DSL_DEBUG_DISABLE*/

/** @} DRV_DSL_CPE_DEBUG */

#if defined(INCLUDE_DSL_CPE_API_DANUBE) || defined(INCLUDE_DSL_CPE_API_VRX)
#ifdef INCLUDE_DSL_FILTER_DETECTION
/**
   Bridge Tap Flag (rough length Indication).
*/
typedef enum
{
   /**
   None.
   Not a bridge tap loop. */
   DSL_BRIDGE_TAP_NONE  = 0,
   /**
   Short bridge tap loop.
   Bridge tap length is less than 1 kft. */
   DSL_BRIDGE_TAP_SHORT = 1,
   /**
   Bridge tap length is around 1 kft. */
   DSL_BRIDGE_TAP_1KFT  = 2,
   /**
   Long bridge tap loop.
   Bridge tap length is more than 1 kft. */
   DSL_BRIDGE_TAP_LONG  = 3,
   /**
   delimeter only*/
   DSL_BRIDGE_TAP_LAST
} DSL_BridgeTapFlag_t;

/**
   Filter Detection Data.
   All values except nNonLinearEchoMetric2 are updated within API on filter
   detection complete state
    (\ref DSL_LINESTATE_TEST_FILTERDETECTION_COMPLETE = 0x01000060) only.
   This structure has to be used for ioctl
   - \ref DSL_FIO_FILTER_DETECTION_DATA_GET
*/
typedef struct
{
   /**
   Noise Level
   This value indicated the noise level range
   - 0 is lowest noise level
   - 7 is highest noise level */
   DSL_OUT DSL_uint8_t nNoiseLevel;
   /**
   Bridge Tap Flag (rough length Indication) */
   DSL_OUT DSL_BridgeTapFlag_t nBridgeTapFlag;
   /**
   No confidence bit.
   According to the state of this parameter the metrics has to be used as
   follows:
   - DSL_FALSE: Metric 1 has to be used for analysis.
   - DSL_TRUE: No confidence. Metric 2 has to be used together with data from
     \ref DSL_FIO_LOOP_LENGTH_STATUS_GET and DSL_FIO_HYBRID_SELECTION_DATA_GET
     which will be updated
     only during training. */
   DSL_OUT DSL_boolean_t bNoConfidence;
   /**
   Metric 1 includes the non linear echo power.
   The valid range of this value is -128 dB to +127 dB. */
   DSL_OUT DSL_int16_t nNonLinearEchoMetric1;
   /**
   Metric 2 includes average echo power.
   The valid range of this value is -128 dB to +127 dB.
   A special value +128 indicate that the parameter is not updated by the API.
   This value is updated within the API
   - directly after showtime entry
   - in exception state after failed training and in case of availability within
     firmware. */
   DSL_OUT DSL_int16_t nNonLinearEchoMetric2;
} DSL_FilterDetectionData_t;

/**
   Structure used for getting Filter Detection Data
   This structure has to be used for ioctl
   - \ref DSL_FIO_FILTER_DETECTION_DATA_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains filter detection data */
   DSL_IN_OUT DSL_FilterDetectionData_t data;
} DSL_FilterDetection_t;
#endif /* INCLUDE_DSL_FILTER_DETECTION */
#endif /* INCLUDE_DSL_CPE_API_DANUBE || INCLUDE_DSL_CPE_API_VRX */

/**
   AFE Hybrid Metric Data.
*/
typedef struct
{
   /**
   AFE hybrid index.
   This value indicate an array offset of the hybrids that are defined within
   firmware.
   A special value which is -1 indicate that both nHybridIndex as well as
   nHybridMetric are uninitialized respective not used. */
   DSL_OUT DSL_int16_t nHybridIndex;
   /**
   AFE hybrid metrics data.
   \note This value is only valid in case of nHybridIndex is equal or bigger
         than 1. */
   DSL_OUT DSL_uint16_t nHybridMetric;
} DSL_HybridMetricData_t;

/**
   AFE Hybrid Selection Data.
   This structure includes information about which hybrid is currently selected
   within firmware as well as which would be the second best hybrid selection.
   Both of these values are available for training/showtime state as well as for
   filter detection state.
   To make use of filter detection information the feature has to be included
   at compile time (INCLUDE_DSL_FILTER_DETECTION).
   This structure has to be used for ioctl
   - \ref DSL_FIO_HYBRID_SELECTION_DATA_GET
*/
typedef struct
{
   /**
   Includes information about which AFE hybrid type is currently selected
   within firmware.
   This values are updated within the API
   - directly after showtime entry
   - in exception state after failed training and in case of availibility
     within firmware. */
   DSL_OUT DSL_HybridMetricData_t actualSelection;
   /**
   Includes information about which AFE hybrid type would be the second best
   for selection within firmware.
   This values are updated within the API
   - directly after showtime entry
   - in exception state after failed training and in case of availibility
     within firmware. */
   DSL_OUT DSL_HybridMetricData_t secondBestSelection;
   /**
   Includes information about which AFE hybrid type is currently selected
   within firmware in case of filter detection measurement.
   This values are updated within API on filter detection complete state
   (\ref DSL_LINESTATE_TEST_FILTERDETECTION_COMPLETE = 0x01000060) only. */
   DSL_OUT DSL_HybridMetricData_t fdActualSelection;
   /**
   Includes information about which AFE hybrid type would be the second best
   for selection within firmware in case of filter detection measurement.
   This values are updated within API on filter detection complete state
   (\ref DSL_LINESTATE_TEST_FILTERDETECTION_COMPLETE = 0x01000060) only. */
   DSL_OUT DSL_HybridMetricData_t fdSecondBestSelection;
} DSL_HybridSelectionData_t;

/**
   Structure used for getting AFE Hybrid Selection Data
   This structure has to be used for ioctl
   - \ref DSL_FIO_HYBRID_SELECTION_DATA_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains AFE hybrid selection data */
   DSL_IN_OUT DSL_HybridSelectionData_t data;
} DSL_HybridSelection_t;

/** Maximum number of pilot tones */
#define MAX_NUMBER_OF_PILOT_TONES            16

/**
   Structure used for getting the pilot tones in showtime.
*/
typedef struct
{
   /**
   Number of used data elements */
   DSL_uint8_t nNumData;
   /**
   Data elements includes the currently used pilot tones.
   Not used values (indices >= nNumData) are set to zero. */
   DSL_uint16_t nPilotTone[MAX_NUMBER_OF_PILOT_TONES];
} DSL_PilotTonesStatusData_t;

/**
   Structure used for getting the pilot tones in showtime.
   This structure has to be used for ioctl
   \ref DSL_FIO_PILOT_TONES_STATUS_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains pilot tone status data */
   DSL_OUT DSL_PilotTonesStatusData_t data;
} DSL_PilotTonesStatus_t;

/**
   Reboot criterias bit field definitions.
   The meaning of all bits is as follows:
   0: Reboot criteria not selected (not active)
   1: Reboot criteria selected (active)
*/
typedef enum
{
   /* Cleaned. */
   DSL_REBOOT_CRITERIA_CLEANED = 0x00000000,
   /**
   Loss-of-Margin.

   \note Default is DSL_TRUE */
   DSL_REBOOT_CRITERIA_LOM = 0x00000001,
   /**
   Loss-of-Frame.

   \note This criteria is only used for VDSL-CPE and ADSL-CPE (Vinax/VRx)
   \note Default is DSL_TRUE */
   DSL_REBOOT_CRITERIA_LOF = 0x00000002,
   /**
   Loss-of-Signal.

   \note Default is DSL_TRUE */
   DSL_REBOOT_CRITERIA_LOS = 0x00000004,
   /**
   Excessive Severe Errors (ESE) Failure, (10 seconds SES).

   \note This criteria is only used for VDSL-CPE (Vinax/VRx)
   \note Default is DSL_TRUE */
   DSL_REBOOT_CRITERIA_ESE = 0x00000008,
   /**
   90 consecutive errored seconds (ES).

   \note This criteria is only used for ADSL-CPE (DanubeFamily) and
         ADSL-CPE (Vinax/VRx)
   \note Default is DSL_FALSE */
   DSL_REBOOT_CRITERIA_ES90 = 0x00000010,
   /**
   30 consecutive severely errored seconds (SES).

   \note This criteria is only used for ADSL-CPE (DanubeFamily) and
         ADSL-CPE (Vinax/VRx)
   \note Default is DSL_FALSE */
   DSL_REBOOT_CRITERIA_SES30 = 0x00000020,
   /**
   Negative SNR margin.

   \note This criteria is only used for ADSL-CPE
   \note Default is DSL_TRUE */
   DSL_REBOOT_CRITERIA_NEGATIVE_MARGIN = 0x00000040,
   /**
   Out-of-Sync (OOS) Failure (PTM only), bearer channel 0.

   \note Default is DSL_FALSE */
   DSL_REBOOT_CRITERIA_OOS_BC0 = 0x00000080,
   /**
   Out-of-Sync (OOS) Failure (PTM only), bearer channel 1.

   \note Default is DSL_FALSE */
   DSL_REBOOT_CRITERIA_OOS_BC1 = 0x00000100,
   /**
   No-Cell-Delineation (NCD) Failure (ATM only), bearer channel 0.

   \note Default is DSL_FALSE, not available yet */
   DSL_REBOOT_CRITERIA_NCD_BC0 = 0x00000200,
   /**
   No-Cell-Delineation (NCD) Failure (ATM only), bearer channel 1.

   \note Default is DSL_FALSE, not available yet */
   DSL_REBOOT_CRITERIA_NCD_BC1 = 0x00000400,
   /**
   Loss-of-Cell-Delineation (LCD) Failure (ATM only), bearer channel 0.

   \note Default is DSL_FALSE, not available yet */
   DSL_REBOOT_CRITERIA_LCD_BC0 = 0x00000800,
   /**
   Loss-of-Cell-Delineation (LCD) Failure (ATM only), bearer channel 1.

   \note Default is DSL_FALSE, not available yet */
   DSL_REBOOT_CRITERIA_LCD_BC1 = 0x00001000
} DSL_BF_RebootCriteriaConfigData_t;

/**
   Structure to write (SET) or read (GET) the configuration of reboot criteria
   for restarting the link.

   \note All bits within this bit field have the following meaning:
         0: Reboot criterion is not active
         1: Reboot criterion is active
*/
typedef struct {
   /**
   Reboot criteria */
   DSL_CFG DSL_BF_RebootCriteriaConfigData_t nRebootCriteria;
} DSL_RebootCriteriaConfigData_t;

/**
   This structure is used to configure reboot criteria for link restart.
   This structure has to be used for ioctl
   - \ref DSL_FIO_REBOOT_CRITERIA_CONFIG_SET
   - \ref DSL_FIO_REBOOT_CRITERIA_CONFIG_GET

*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains status data */
   DSL_IN_OUT DSL_RebootCriteriaConfigData_t data;
} DSL_RebootCriteriaConfig_t;

/**
   Structure used for sending plain byte data and low level device messages
*/
typedef struct
{
   /**
   Message data */
   DSL_IN_OUT DSL_uint8_t *pMsg;
   /**
   Message data mask */
   DSL_IN_OUT DSL_uint8_t *pMsgMask;
   /**
   Size of message included in pMsg. The size of the message has to be given
   in bytes! */
   DSL_IN DSL_uint16_t nSizeTx;
   /**
   Maximal Size of pMsg buffer. This value will be checked if a possible answer
   to the command can be stored in the buffer. */
   DSL_IN_OUT DSL_uint16_t nSizeRx;
} DSL_DeviceMessageModifyData_t;

/**
   Structure used for sending plain byte data and low level device messages
   This structure has to be used for ioctl
   - \ref DSL_FIO_DBG_DEVICE_MESSAGE_MODIFY_SEND
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Structure that contains device message data */
   DSL_IN_OUT DSL_DeviceMessageModifyData_t data;
} DSL_DeviceMessageModify_t;

#ifdef DSL_INTERN
   /* lower level device interface*/
   #include "drv_dsl_cpe_device.h"

   #if defined(INCLUDE_DSL_PM)
      #include "drv_dsl_cpe_pm_core.h"
   #endif

   #if defined(INCLUDE_DSL_BONDING)
   #if defined(INCLUDE_DSL_CPE_API_VINAX)
      #include "drv_dsl_cpe_api_bnd_vinax.h"
      #include "drv_dsl_cpe_api_bnd_vinax_fpga.h"
   #endif
   #endif
#endif

#if defined(SWIG) || defined(DSL_INTERN)
#include "drv_dsl_cpe_intern.h"
#include "drv_dsl_cpe_intern_g997.h"
#endif

#ifdef __cplusplus
}
#endif

#endif /* _DRV_DSL_CPE_API_H */
