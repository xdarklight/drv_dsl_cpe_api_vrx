/* src/include/drv_dsl_cpe_api_config.h.  Generated from drv_dsl_cpe_api_config.h.in by configure.  */
/* src/include/drv_dsl_cpe_api_config.h.in.  Generated from configure.in by autoheader.  */

/* Channels per line to support */
#define DSL_CHANNELS_PER_LINE 1

/* Enable building only library file */
/* #undef DSL_CPE_API_LIBRARY_BUILD_2_6 */

/* Include DSL DELT static data support */
/* #undef DSL_CPE_STATIC_DELT_DATA */

/* Debug level = local */
#define DSL_DBG_MAX_LEVEL_PRE 0x80

/* Preselection of max. debug level = no (using default) */
#define DSL_DBG_MAX_LEVEL_SET /**/

/* Disabled debug prints */
/* #undef DSL_DEBUG_DISABLE */

/* Maximum lines per DSL device to support */
#define DSL_DRV_LINES_PER_DEVICE 1

/* Maximum DSL devices to support */
#define DSL_DRV_MAX_DEVICE_NUMBER 2

/* Add VR10 support */
#define DSL_VRX_DEVICE_VR10 /**/

/* Add VR10_320 support */
/* #undef DSL_VRX_DEVICE_VR10_320 */

/* Add VR9 support */
/* #undef DSL_VRX_DEVICE_VR9 */

/* Include deprecated functions */
#define INCLUDE_DEPRECATED /**/

/* Include Device Exception Codes */
#define INCLUDE_DEVICE_EXCEPTION_CODES /**/

/* Include ADSL MIB support */
/* #undef INCLUDE_DSL_ADSL_MIB */

/* Include DSL BONDING support */
#define INCLUDE_DSL_BONDING /**/

/* Include DSL CEOC support */
/* #undef INCLUDE_DSL_CEOC */

/* Include configuration get support */
#define INCLUDE_DSL_CONFIG_GET /**/

/* Add DANUBE support */
/* #undef INCLUDE_DSL_CPE_API_DANUBE */

/* Include IFXOS support */
#define INCLUDE_DSL_CPE_API_IFXOS_SUPPORT /**/

/* Add VRX support */
#define INCLUDE_DSL_CPE_API_VRX /**/

/* Include Misc Line Status support */
#define INCLUDE_DSL_CPE_MISC_LINE_STATUS /**/

/* Include DSL PM channel counters support */
#define INCLUDE_DSL_CPE_PM_CHANNEL_COUNTERS /**/

/* Include DSL PM channel extended counters support */
/* #undef INCLUDE_DSL_CPE_PM_CHANNEL_EXT_COUNTERS */

/* Include DSL PM channel thresholds support */
#define INCLUDE_DSL_CPE_PM_CHANNEL_THRESHOLDS /**/

/* Include PM config support */
#define INCLUDE_DSL_CPE_PM_CONFIG /**/

/* Include DSL PM datapath counters support */
#define INCLUDE_DSL_CPE_PM_DATA_PATH_COUNTERS /**/

/* Include DSL PM datapath failure counters support */
#define INCLUDE_DSL_CPE_PM_DATA_PATH_FAILURE_COUNTERS /**/

/* Include DSL PM datapath thresholds support */
#define INCLUDE_DSL_CPE_PM_DATA_PATH_THRESHOLDS /**/

/* Include DSL PM history support */
#define INCLUDE_DSL_CPE_PM_HISTORY /**/

/* Include DSL PM line counters support */
#define INCLUDE_DSL_CPE_PM_LINE_COUNTERS /**/

/* Include DSL PM line event showtime counters support */
#define INCLUDE_DSL_CPE_PM_LINE_EVENT_SHOWTIME_COUNTERS /**/

/* Include DSL PM line thresholds support */
#define INCLUDE_DSL_CPE_PM_LINE_THRESHOLDS /**/

/* Include DSL PM optional parameters support */
#define INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS /**/

/* Include DSL PM retransmission counters support */
#define INCLUDE_DSL_CPE_PM_RETX_COUNTERS /**/

/* Include DSL PM retransmission thresholds support */
#define INCLUDE_DSL_CPE_PM_RETX_THRESHOLDS /**/

/* Include DSL PM showtime counters support */
#define INCLUDE_DSL_CPE_PM_SHOWTIME_COUNTERS /**/

/* Include DSL PM total counters support */
#define INCLUDE_DSL_CPE_PM_TOTAL_COUNTERS /**/

/* Include ADSL showtime event logging support */
/* #undef INCLUDE_DSL_CPE_TRACE_BUFFER */

/* Include DELT support */
#define INCLUDE_DSL_DELT /**/

/* Include Filter Detection support */
#define INCLUDE_DSL_FILTER_DETECTION /**/

/* Include FW memory free support */
#define INCLUDE_DSL_FIRMWARE_MEMORY_FREE /**/

/* Include Framing Parameters support */
#define INCLUDE_DSL_FRAMING_PARAMETERS /**/

/* Include G997 alarm support */
#define INCLUDE_DSL_G997_ALARM /**/

/* Include G997 Framing Parameters support */
#define INCLUDE_DSL_G997_FRAMING_PARAMETERS /**/

/* Include G997 Line Inventory support */
#define INCLUDE_DSL_G997_LINE_INVENTORY /**/

/* Include G997 per tone values support */
#define INCLUDE_DSL_G997_PER_TONE /**/

/* Include G997 status support */
#define INCLUDE_DSL_G997_STATUS /**/

/* Include DSL PM support */
#define INCLUDE_DSL_PM /**/

/* Include Resource Statistics support */
#define INCLUDE_DSL_RESOURCE_STATISTICS /**/

/* Include System Interface configuration support */
#define INCLUDE_DSL_SYSTEM_INTERFACE /**/

/* Include FW request support */
#define INCLUDE_FW_REQUEST_SUPPORT /**/

/* Pilot Tones Status support */
#define INCLUDE_PILOT_TONES_STATUS /**/

/* Include real time trace support */
/* #undef INCLUDE_REAL_TIME_TRACE */

/* Define to 1 if your C compiler doesn't accept -c and -o together. */
/* #undef NO_MINUS_C_MINUS_O */

/* Name of package */
#define PACKAGE "drv_dsl_cpe_api"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME "DSL CPE API driver"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "DSL CPE API driver 4.16.5.4"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "drv_dsl_cpe_api"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "4.16.5.4"

/* Version number of package */
#define VERSION "4.16.5.4"
