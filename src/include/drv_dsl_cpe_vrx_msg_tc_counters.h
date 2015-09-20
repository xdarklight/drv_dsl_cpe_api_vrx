/******************************************************************************

                              Copyright (c) 2014
                            Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/


#ifndef _DRV_DSL_CPE_VRX_MSG_TC_COUNTERS_H_
#define _DRV_DSL_CPE_VRX_MSG_TC_COUNTERS_H_

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


/** @defgroup _TC_COUNTERS_
 *  @{
 */

#ifdef __cplusplus
   extern "C" {
#endif

/* ----- Message Specific Constants Definition section ----- */
#define ACK_TC_StatusGet_UNKNOWN_TC 0x0
#define ACK_TC_StatusGet_EFM_TC 0x1
#define ACK_TC_StatusGet_ATM_TC 0x2
#define CMD_TC_FW_InfoSet_EFM_TC 0x1
#define CMD_TC_FW_InfoSet_ATM_TC 0x2
#define ACK_VersionInfoGet_HW_VER11 0x1
#define ACK_VersionInfoGet_VRX 0x05
#define ACK_VersionInfoGet_RELEASE 0
#define ACK_VersionInfoGet_PRERELEASE 1
#define ACK_VersionInfoGet_DEVELOP 2
#define ACK_VersionInfoGet_ADSL_A 0x1
#define ACK_VersionInfoGet_ADSL_BJ 0x2
#define ACK_VersionInfoGet_VDSL2 0x6
#define CMD_PAF_HS_Continue_WAIT 0x0
#define CMD_PAF_HS_Continue_CONTINUE 0x1
/* ----- Message Specific Constants Definition section (End) ----- */

/** Message ID for CMD_TC_StatusGet */
#define CMD_TC_STATUSGET 0x0E22

/**
   The message reads the used TC.In ADSL the TC type is selected during G.HS and
   thus known when entering FULL_INIT. In VDSL it is selected later, during
   FULL_INIT. Before this point the FW will report the TC as "unknown".
*/
typedef struct CMD_TC_StatusGet CMD_TC_StatusGet_t;

/** Message ID for ACK_TC_StatusGet */
#define ACK_TC_STATUSGET 0x0E22

/**
   Reporting of the used TC mode.
*/
typedef struct ACK_TC_StatusGet ACK_TC_StatusGet_t;

/** Message ID for CMD_TC_FW_InfoSet */
#define CMD_TC_FW_INFOSET 0x1762

/**
   The message informs the DSL-FW about the currently loaded TC FW driver.
*/
typedef struct CMD_TC_FW_InfoSet CMD_TC_FW_InfoSet_t;

/** Message ID for ACK_TC_FW_InfoSet */
#define ACK_TC_FW_INFOSET 0x1762

/**
   Acknowledgement to CMD_TC_FW_InfoSet.
*/
typedef struct ACK_TC_FW_InfoSet ACK_TC_FW_InfoSet_t;

/** Message ID for CMD_MAC_FrameConfigure */
#define CMD_MAC_FRAMECONFIGURE 0x5348

/**
   The message configures Ethernet properties, currently the MAC address of the
   device only.The MAC address is needed in the FW as "MAC source address"for
   PDUs not built by the host, like e.g. for the Ethernet encapsulated
   Backchannel Data ERB in Vectoring.
*/
typedef struct CMD_MAC_FrameConfigure CMD_MAC_FrameConfigure_t;

/** Message ID for ACK_MAC_FrameConfigure */
#define ACK_MAC_FRAMECONFIGURE 0x5348

/**
   Acknowledgement to CMD_MAC_FrameConfigure.
*/
typedef struct ACK_MAC_FrameConfigure ACK_MAC_FrameConfigure_t;

/** Message ID for CMD_DataPathFailuresAlarmConfigure */
#define CMD_DATAPATHFAILURESALARMCONFIGURE 0x1249

/**
   Enables/Disables the generation of ALARM messages (ALM) for specific data-
   path failures. If the corresponding Enable bit for a failure is set, then the
   modem firmware will send an autonomous message ALM_DataPathFailuresGet if
   this failure occurs.
*/
typedef struct CMD_DataPathFailuresAlarmConfigure CMD_DataPathFailuresAlarmConfigure_t;

/** Message ID for ACK_DataPathFailuresAlarmConfigure */
#define ACK_DATAPATHFAILURESALARMCONFIGURE 0x1249

/**
   Acknowledgement to CMD_DataPathFailuresAlarmConfigure.
*/
typedef struct ACK_DataPathFailuresAlarmConfigure ACK_DataPathFailuresAlarmConfigure_t;

/** Message ID for CMD_DataPathFailuresGet */
#define CMD_DATAPATHFAILURESGET 0x0507

/**
   Requests the data-path failure status.
*/
typedef struct CMD_DataPathFailuresGet CMD_DataPathFailuresGet_t;

/** Message ID for ACK_DataPathFailuresGet */
#define ACK_DATAPATHFAILURESGET 0x0507

/**
   Provides the data path failure status as requested by the command
   CMD_DataPathFailuresGet.
*/
typedef struct ACK_DataPathFailuresGet ACK_DataPathFailuresGet_t;

/** Message ID for ALM_DataPathFailuresGet */
#define ALM_DATAPATHFAILURESGET 0x0507

/**
   This autonomous message reports the data-path failure status. It is sent only
   if the message generation was enabled for the corresponding failure with
   CMD_DataPathFailuresAlarmConfigure.
*/
typedef struct ALM_DataPathFailuresGet ALM_DataPathFailuresGet_t;

/** Message ID for CMD_PTM_BC0_StatsNE_Get */
#define CMD_PTM_BC0_STATSNE_GET 0x2D0A

/**
   Requests the values of the near-end PTM data path performance counters for
   bearer channel 0 (BC0) as defined in G.997.1 (Section 7.2.5.1. of [11]).The
   counters are total counters.
*/
typedef struct CMD_PTM_BC0_StatsNE_Get CMD_PTM_BC0_StatsNE_Get_t;

/** Message ID for ACK_PTM_BC0_StatsNE_Get */
#define ACK_PTM_BC0_STATSNE_GET 0x2D0A

/**
   Delivers the near-end total TPS-TC counter values for bearer channel 0 (BC0)
   as requested by CMD_PTM_BC0_StatsNE_Get. (Section 7.2.5.1. of [11])
*/
typedef struct ACK_PTM_BC0_StatsNE_Get ACK_PTM_BC0_StatsNE_Get_t;

/** Message ID for CMD_PTM_BC0_StatsNE_Set */
#define CMD_PTM_BC0_STATSNE_SET 0x2D4A

/**
   Writes the TPS-TC CRC error and TPS-TC coding violation counters (Section
   7.2.5.1. of G.997.1 [11]).Why writing these counters? For "automoding",
   meaning modems configured to support both ADSL and VDLS with automatic mode
   selection. The first retrain is always done with an ADSL FW independent of
   what was loaded before. Therefore, the counters must be saved by the host
   before destroying them by the FW download and are written back before the
   next link start of a FW that supports PTM.
*/
typedef struct CMD_PTM_BC0_StatsNE_Set CMD_PTM_BC0_StatsNE_Set_t;

/** Message ID for ACK_PTM_BC0_StatsNE_Set */
#define ACK_PTM_BC0_STATSNE_SET 0x2D4A

/**
   Acknowledgement for the message CMD_PTM_BC0_StatsNE_Set.
*/
typedef struct ACK_PTM_BC0_StatsNE_Set ACK_PTM_BC0_StatsNE_Set_t;

/** Message ID for CMD_ATM_BC0_StatsNE_Get */
#define CMD_ATM_BC0_STATSNE_GET 0x2E0A

/**
   This command is used to request the near-end data-path performance counters
   for BC0 of an ATM-TC link as defined in Section 7.2.4. of G.997.1 [11].
*/
typedef struct CMD_ATM_BC0_StatsNE_Get CMD_ATM_BC0_StatsNE_Get_t;

/** Message ID for ACK_ATM_BC0_StatsNE_Get */
#define ACK_ATM_BC0_STATSNE_GET 0x2E0A

/**
   Provides the total near-end ATM-TC performance counters for bearer channel 0
   (BC0) as requested by the command CMD_ATM_BC0_StatsNE_Get.ADSL1 Fast Path
   reports its ATM -TC counters in this message too.
*/
typedef struct ACK_ATM_BC0_StatsNE_Get ACK_ATM_BC0_StatsNE_Get_t;

/** Message ID for CMD_ATM_BC0_StatsFE_Get */
#define CMD_ATM_BC0_STATSFE_GET 0x330A

/**
   This command is used to request the far-end data-path performance counters
   for BC0 of an ATM-TC link as defined in Section 7.2.4.2 of G.997.1 [11].
*/
typedef struct CMD_ATM_BC0_StatsFE_Get CMD_ATM_BC0_StatsFE_Get_t;

/** Message ID for ACK_ATM_BC0_StatsFE_Get */
#define ACK_ATM_BC0_STATSFE_GET 0x330A

/**
   Provides the total near-end ATM-TC performance counters for bearer channel 0
   (BC0) as requested by the command CMD_ATM_BC0_StatsFE_Get.
*/
typedef struct ACK_ATM_BC0_StatsFE_Get ACK_ATM_BC0_StatsFE_Get_t;

/** Message ID for CMD_ATM_BC0_StatsNE_Set */
#define CMD_ATM_BC0_STATSNE_SET 0x2E4A

/**
   Writes the data-path performance counters for bearer channel 0 (BC0) of an
   ATM-TC link (Section 7.2.4. of G.997.1 [11]).Why writing counters? The ADSL
   FW of the VRX destroys the performance counters at the beginning of a re-
   initialization since it starts with a FW download. Therefore the host has to
   save the counters in FAIL state before triggering a re-init. When
   reconfiguring the system in RESET state, the host writes the saved values as
   start values back into the counters. To do this for the ATM-TC counters, this
   message is used.
*/
typedef struct CMD_ATM_BC0_StatsNE_Set CMD_ATM_BC0_StatsNE_Set_t;

/** Message ID for ACK_ATM_BC0_StatsNE_Set */
#define ACK_ATM_BC0_STATSNE_SET 0x2E4A

/**
   Acknowledgement for the message CMD_ATM_BC0_StatsNE_Set.
*/
typedef struct ACK_ATM_BC0_StatsNE_Set ACK_ATM_BC0_StatsNE_Set_t;

/** Message ID for CMD_ATM_BC0_TxStatsNE_Get */
#define CMD_ATM_BC0_TXSTATSNE_GET 0x350A

/**
   Requests near-end ATM datapath performance transmit counters for bearer
   channel 0 (BC0).
*/
typedef struct CMD_ATM_BC0_TxStatsNE_Get CMD_ATM_BC0_TxStatsNE_Get_t;

/** Message ID for ACK_ATM_BC0_TxStatsNE_Get */
#define ACK_ATM_BC0_TXSTATSNE_GET 0x350A

/**
   Provides the ATM-TC transmit counters for beare channel 0 (BC0) as requested
   by the command CMD_ATM_BC0_TxStatsNE_Get.ADSL1 Fast Path reports its ATM -TC
   counters in this message too.
*/
typedef struct ACK_ATM_BC0_TxStatsNE_Get ACK_ATM_BC0_TxStatsNE_Get_t;

/** Message ID for CMD_ATM_BC0_TxStatsNE_Set */
#define CMD_ATM_BC0_TXSTATSNE_SET 0x354A

/**
   Writes the transmit data-path counters for BC0 of an ATM-TC link.For "Why
   writing performance counters" see Page 649.
*/
typedef struct CMD_ATM_BC0_TxStatsNE_Set CMD_ATM_BC0_TxStatsNE_Set_t;

/** Message ID for ACK_ATM_BC0_TxStatsNE_Set */
#define ACK_ATM_BC0_TXSTATSNE_SET 0x354A

/**
   Acknowledgement for the message CMD_ATM_BC0_TxStatsNE_Set.
*/
typedef struct ACK_ATM_BC0_TxStatsNE_Set ACK_ATM_BC0_TxStatsNE_Set_t;

/** Message ID for CMD_VersionInfoGet */
#define CMD_VERSIONINFOGET 0x0010

/**
   Requests version information from the VDSL2-Modem.
*/
typedef struct CMD_VersionInfoGet CMD_VersionInfoGet_t;

/** Message ID for ACK_VersionInfoGet */
#define ACK_VERSIONINFOGET 0x0010

/**
   Provides version information about hardware and firmware. The FW version
   numbering scheme has a long and a short form (Table 353).
*/
typedef struct ACK_VersionInfoGet ACK_VersionInfoGet_t;

/** Message ID for CMD_ADSL_FeatureMapGet */
#define CMD_ADSL_FEATUREMAPGET 0x1322

/**
   Requests the feature map indicating the ADSL FW features supported by this
   binary.
*/
typedef struct CMD_ADSL_FeatureMapGet CMD_ADSL_FeatureMapGet_t;

/** Message ID for ACK_ADSL_FeatureMapGet */
#define ACK_ADSL_FEATUREMAPGET 0x1322

/**
   Reports the feature map indicating the ADSL FW features supported by this
   binary.
*/
typedef struct ACK_ADSL_FeatureMapGet ACK_ADSL_FeatureMapGet_t;

/** Message ID for CMD_VDSL_FeatureMapGet */
#define CMD_VDSL_FEATUREMAPGET 0x1422

/**
   Requests the feature map indicating the VDSL FW features supported by this
   binary.
*/
typedef struct CMD_VDSL_FeatureMapGet CMD_VDSL_FeatureMapGet_t;

/** Message ID for ACK_VDSL_FeatureMapGet */
#define ACK_VDSL_FEATUREMAPGET 0x1422

/**
   Reports the feature map indicating the VDSL FW features supported by this
   binary.
*/
typedef struct ACK_VDSL_FeatureMapGet ACK_VDSL_FeatureMapGet_t;

/** Message ID for CMD_PAF_HS_Control */
#define CMD_PAF_HS_CONTROL 0x1148

/**
   The message is used to enable bonding (PAF) and to control the GHS
   aggregation discovery and aggregation processes. The register numbers
   referenced refer to IEEE 802.3ah [14], Clause 45.
*/
typedef struct CMD_PAF_HS_Control CMD_PAF_HS_Control_t;

/** Message ID for ACK_PAF_HS_Control */
#define ACK_PAF_HS_CONTROL 0x1148

/**
   The message acknowledges CMD_PAF_HS_Control.
*/
typedef struct ACK_PAF_HS_Control ACK_PAF_HS_Control_t;

/** Message ID for CMD_PAF_HS_ControlGet */
#define CMD_PAF_HS_CONTROLGET 0x1108

/**
   The message reads back settings for bonding configured with
   CMD_PAF_HS_Control.
*/
typedef struct CMD_PAF_HS_ControlGet CMD_PAF_HS_ControlGet_t;

/** Message ID for ACK_PAF_HS_ControlGet */
#define ACK_PAF_HS_CONTROLGET 0x1108

/**
   The message acknowledges CMD_PAF_HS_ControlGet providing PAF bonding
   parameters used in handshake as configured with CMD_PAF_HS_Control.
*/
typedef struct ACK_PAF_HS_ControlGet ACK_PAF_HS_ControlGet_t;

/** Message ID for CMD_PAF_HS_StatusGet */
#define CMD_PAF_HS_STATUSGET 0xDD03

/**
   The message requests aggregation discovery and aggregation status information
   for EFM bonding.
*/
typedef struct CMD_PAF_HS_StatusGet CMD_PAF_HS_StatusGet_t;

/** Message ID for ACK_PAF_HS_StatusGet */
#define ACK_PAF_HS_STATUSGET 0xDD03

/**
   The message reports discovery and aggregation status information for EFM
   bonding after request by CMD_PAF_HS_StatusGet.At the CPE side, the Host SW
   concludes on a GET request if none of the following 4 bits are set:
   discoveryClearIfSame, discoverySetIfClear, aggregClear , aggregSet. It does
   not distinguish between "discovery GET" and "aggregation GET"; in case of any
   "GET" both the discovery and the aggregation code are sent to the CO with the
   CLR.
*/
typedef struct ACK_PAF_HS_StatusGet ACK_PAF_HS_StatusGet_t;

/** Message ID for CMD_PAF_HS_Continue */
#define CMD_PAF_HS_CONTINUE 0x0C62

/**
   The message writes the aggregation discovery code and the aggregation data
   down to the FW and is used to trigger the CPE-FW to proceed in handshake
   (sending CLR) after it had been waiting in GHS_BONDING_CLR state for the
   necessary host interactions to be finished. These actions depend on the
   request type (see ACK_PAF_HS_StatusGet): the processing of the remote
   discovery register (for an aggregation discovery "Set If Clear" or "Clear If
   Same" command) or PME Aggregate register (for an aggregation "Set" or "Clear"
   command) in the SW and -in any case including a "Get" command - then updating
   the local representations of both in the FW (which are sent to the CO in a
   CLR).
*/
typedef struct CMD_PAF_HS_Continue CMD_PAF_HS_Continue_t;

/** Message ID for ACK_PAF_HS_Continue */
#define ACK_PAF_HS_CONTINUE 0x0C62

/**
   The message acknowledges CMD_PAF_HS_Continue.
*/
typedef struct ACK_PAF_HS_Continue ACK_PAF_HS_Continue_t;

/**
   The message reads the used TC.In ADSL the TC type is selected during G.HS and
   thus known when entering FULL_INIT. In VDSL it is selected later, during
   FULL_INIT. Before this point the FW will report the TC as "unknown".
*/
struct CMD_TC_StatusGet
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
   Reporting of the used TC mode.
*/
struct ACK_TC_StatusGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** TC Used */
   DSL_uint16_t TC;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** TC Used */
   DSL_uint16_t TC;
#endif
} __PACKED__ ;


/**
   The message informs the DSL-FW about the currently loaded TC FW driver.
*/
struct CMD_TC_FW_InfoSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** TC Loaded */
   DSL_uint16_t loadedTC;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** TC Loaded */
   DSL_uint16_t loadedTC;
#endif
} __PACKED__ ;


/**
   Acknowledgement to CMD_TC_FW_InfoSet.
*/
struct ACK_TC_FW_InfoSet
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
   The message configures Ethernet properties, currently the MAC address of the
   device only.The MAC address is needed in the FW as "MAC source address"for
   PDUs not built by the host, like e.g. for the Ethernet encapsulated
   Backchannel Data ERB in Vectoring.
*/
struct CMD_MAC_FrameConfigure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Source MAC Address Octets 0 to 1 */
   DSL_uint16_t SrcMacAddrB0_1;
   /** Source MAC Address Octets 2 to 3 */
   DSL_uint16_t SrcMacAddrB2_3;
   /** Source MAC Address Octets 4 to 5 */
   DSL_uint16_t SrcMacAddrB4_5;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Source MAC Address Octets 0 to 1 */
   DSL_uint16_t SrcMacAddrB0_1;
   /** Source MAC Address Octets 2 to 3 */
   DSL_uint16_t SrcMacAddrB2_3;
   /** Source MAC Address Octets 4 to 5 */
   DSL_uint16_t SrcMacAddrB4_5;
#endif
} __PACKED__ ;


/**
   Acknowledgement to CMD_MAC_FrameConfigure.
*/
struct ACK_MAC_FrameConfigure
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
   Enables/Disables the generation of ALARM messages (ALM) for specific data-
   path failures. If the corresponding Enable bit for a failure is set, then the
   modem firmware will send an autonomous message ALM_DataPathFailuresGet if
   this failure occurs.
*/
struct CMD_DataPathFailuresAlarmConfigure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 6;
   /** OOS Failure FE, BC0 */
   DSL_uint16_t OOS_FE_BC1 : 1;
   /** OOS Failure FE, BC0 */
   DSL_uint16_t OOS_FE_BC0 : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 6;
   /** OOS Failure NE, BC1 */
   DSL_uint16_t OOS_NE_BC1 : 1;
   /** OOS Failure NE, BC0 */
   DSL_uint16_t OOS_NE_BC0 : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 4;
   /** LCD Failure FE, BC1 */
   DSL_uint16_t LCD_FE_BC1 : 1;
   /** NCD Failure FE, BC1 */
   DSL_uint16_t NCD_FE_BC1 : 1;
   /** LCD Failure FE, BC0 */
   DSL_uint16_t LCD_FE_BC0 : 1;
   /** NCD Failure FE, BC0 */
   DSL_uint16_t NCD_FE_BC0 : 1;
   /** Reserved */
   DSL_uint16_t Res3 : 4;
   /** LCD Failure NE, BC1 */
   DSL_uint16_t LCD_NE_BC1 : 1;
   /** NCD Failure NE, BC1 */
   DSL_uint16_t NCD_NE_BC1 : 1;
   /** LCD Failure NE, BC0 */
   DSL_uint16_t LCD_NE_BC0 : 1;
   /** NCD Failure NE, BC0 */
   DSL_uint16_t NCD_NE_BC0 : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** OOS Failure NE, BC0 */
   DSL_uint16_t OOS_NE_BC0 : 1;
   /** OOS Failure NE, BC1 */
   DSL_uint16_t OOS_NE_BC1 : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 6;
   /** OOS Failure FE, BC0 */
   DSL_uint16_t OOS_FE_BC0 : 1;
   /** OOS Failure FE, BC0 */
   DSL_uint16_t OOS_FE_BC1 : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 6;
   /** NCD Failure NE, BC0 */
   DSL_uint16_t NCD_NE_BC0 : 1;
   /** LCD Failure NE, BC0 */
   DSL_uint16_t LCD_NE_BC0 : 1;
   /** NCD Failure NE, BC1 */
   DSL_uint16_t NCD_NE_BC1 : 1;
   /** LCD Failure NE, BC1 */
   DSL_uint16_t LCD_NE_BC1 : 1;
   /** Reserved */
   DSL_uint16_t Res3 : 4;
   /** NCD Failure FE, BC0 */
   DSL_uint16_t NCD_FE_BC0 : 1;
   /** LCD Failure FE, BC0 */
   DSL_uint16_t LCD_FE_BC0 : 1;
   /** NCD Failure FE, BC1 */
   DSL_uint16_t NCD_FE_BC1 : 1;
   /** LCD Failure FE, BC1 */
   DSL_uint16_t LCD_FE_BC1 : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 4;
#endif
} __PACKED__ ;


/**
   Acknowledgement to CMD_DataPathFailuresAlarmConfigure.
*/
struct ACK_DataPathFailuresAlarmConfigure
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
   Requests the data-path failure status.
*/
struct CMD_DataPathFailuresGet
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
   Provides the data path failure status as requested by the command
   CMD_DataPathFailuresGet.
*/
struct ACK_DataPathFailuresGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 6;
   /** OOS Failure FE, BC0 */
   DSL_uint16_t OOS_FE_BC1 : 1;
   /** OOS Failure FE, BC0 */
   DSL_uint16_t OOS_FE_BC0 : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 6;
   /** OOS Failure NE, BC1 */
   DSL_uint16_t OOS_NE_BC1 : 1;
   /** OOS Failure NE, BC0 */
   DSL_uint16_t OOS_NE_BC0 : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 4;
   /** LCD Failure FE, BC1 */
   DSL_uint16_t LCD_FE_BC1 : 1;
   /** NCD Failure FE, BC1 */
   DSL_uint16_t NCD_FE_BC1 : 1;
   /** LCD Failure FE, BC0 */
   DSL_uint16_t LCD_FE_BC0 : 1;
   /** NCD Failure FE, BC0 */
   DSL_uint16_t NCD_FE_BC0 : 1;
   /** Reserved */
   DSL_uint16_t Res3 : 4;
   /** LCD Failure NE, BC1 */
   DSL_uint16_t LCD_NE_BC1 : 1;
   /** NCD Failure NE, BC1 */
   DSL_uint16_t NCD_NE_BC1 : 1;
   /** LCD Failure NE, BC0 */
   DSL_uint16_t LCD_NE_BC0 : 1;
   /** NCD Failure NE, BC0 */
   DSL_uint16_t NCD_NE_BC0 : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** OOS Failure NE, BC0 */
   DSL_uint16_t OOS_NE_BC0 : 1;
   /** OOS Failure NE, BC1 */
   DSL_uint16_t OOS_NE_BC1 : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 6;
   /** OOS Failure FE, BC0 */
   DSL_uint16_t OOS_FE_BC0 : 1;
   /** OOS Failure FE, BC0 */
   DSL_uint16_t OOS_FE_BC1 : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 6;
   /** NCD Failure NE, BC0 */
   DSL_uint16_t NCD_NE_BC0 : 1;
   /** LCD Failure NE, BC0 */
   DSL_uint16_t LCD_NE_BC0 : 1;
   /** NCD Failure NE, BC1 */
   DSL_uint16_t NCD_NE_BC1 : 1;
   /** LCD Failure NE, BC1 */
   DSL_uint16_t LCD_NE_BC1 : 1;
   /** Reserved */
   DSL_uint16_t Res3 : 4;
   /** NCD Failure FE, BC0 */
   DSL_uint16_t NCD_FE_BC0 : 1;
   /** LCD Failure FE, BC0 */
   DSL_uint16_t LCD_FE_BC0 : 1;
   /** NCD Failure FE, BC1 */
   DSL_uint16_t NCD_FE_BC1 : 1;
   /** LCD Failure FE, BC1 */
   DSL_uint16_t LCD_FE_BC1 : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 4;
#endif
} __PACKED__ ;


/**
   This autonomous message reports the data-path failure status. It is sent only
   if the message generation was enabled for the corresponding failure with
   CMD_DataPathFailuresAlarmConfigure.
*/
struct ALM_DataPathFailuresGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 6;
   /** OOS Failure FE, BC0 */
   DSL_uint16_t OOS_FE_BC1 : 1;
   /** OOS Failure FE, BC0 */
   DSL_uint16_t OOS_FE_BC0 : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 6;
   /** OOS Failure NE, BC1 */
   DSL_uint16_t OOS_NE_BC1 : 1;
   /** OOS Failure NE, BC0 */
   DSL_uint16_t OOS_NE_BC0 : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 4;
   /** LCD Failure FE, BC1 */
   DSL_uint16_t LCD_FE_BC1 : 1;
   /** NCD Failure FE, BC1 */
   DSL_uint16_t NCD_FE_BC1 : 1;
   /** LCD Failure FE, BC0 */
   DSL_uint16_t LCD_FE_BC0 : 1;
   /** NCD Failure FE, BC0 */
   DSL_uint16_t NCD_FE_BC0 : 1;
   /** Reserved */
   DSL_uint16_t Res3 : 4;
   /** LCD Failure NE, BC1 */
   DSL_uint16_t LCD_NE_BC1 : 1;
   /** NCD Failure NE, BC1 */
   DSL_uint16_t NCD_NE_BC1 : 1;
   /** LCD Failure NE, BC0 */
   DSL_uint16_t LCD_NE_BC0 : 1;
   /** NCD Failure NE, BC0 */
   DSL_uint16_t NCD_NE_BC0 : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** OOS Failure NE, BC0 */
   DSL_uint16_t OOS_NE_BC0 : 1;
   /** OOS Failure NE, BC1 */
   DSL_uint16_t OOS_NE_BC1 : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 6;
   /** OOS Failure FE, BC0 */
   DSL_uint16_t OOS_FE_BC0 : 1;
   /** OOS Failure FE, BC0 */
   DSL_uint16_t OOS_FE_BC1 : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 6;
   /** NCD Failure NE, BC0 */
   DSL_uint16_t NCD_NE_BC0 : 1;
   /** LCD Failure NE, BC0 */
   DSL_uint16_t LCD_NE_BC0 : 1;
   /** NCD Failure NE, BC1 */
   DSL_uint16_t NCD_NE_BC1 : 1;
   /** LCD Failure NE, BC1 */
   DSL_uint16_t LCD_NE_BC1 : 1;
   /** Reserved */
   DSL_uint16_t Res3 : 4;
   /** NCD Failure FE, BC0 */
   DSL_uint16_t NCD_FE_BC0 : 1;
   /** LCD Failure FE, BC0 */
   DSL_uint16_t LCD_FE_BC0 : 1;
   /** NCD Failure FE, BC1 */
   DSL_uint16_t NCD_FE_BC1 : 1;
   /** LCD Failure FE, BC1 */
   DSL_uint16_t LCD_FE_BC1 : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 4;
#endif
} __PACKED__ ;


/**
   Requests the values of the near-end PTM data path performance counters for
   bearer channel 0 (BC0) as defined in G.997.1 (Section 7.2.5.1. of [11]).The
   counters are total counters.
*/
struct CMD_PTM_BC0_StatsNE_Get
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
   Delivers the near-end total TPS-TC counter values for bearer channel 0 (BC0)
   as requested by CMD_PTM_BC0_StatsNE_Get. (Section 7.2.5.1. of [11])
*/
struct ACK_PTM_BC0_StatsNE_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** TPS-TC CRC Counter (CRC-P), NE, BC0, LSW */
   DSL_uint16_t cntCRC_LSW;
   /** TPS-TC CRC Counter (CRC-P), NE, BC0, MSW */
   DSL_uint16_t cntCRC_MSW;
   /** TPS-TC CV Counter (CV-P), NE, BC0, LSW */
   DSL_uint16_t cntCV_LSW;
   /** TPS-TC CV Counter (CV-P), NE, BC0, MSW */
   DSL_uint16_t cntCV_MSW;
   /** TPS-TC CRC Counter (CRCP-P), NE, BC0, LSW */
   DSL_uint16_t cntCRCP_LSW;
   /** TPS-TC CRC Counter (CRCP-P), NE, BC0, MSW */
   DSL_uint16_t cntCRCP_MSW;
   /** TPS-TC CV Counter (CVP-P), NE, BC0, LSW */
   DSL_uint16_t cntCVP_LSW;
   /** TPS-TC CV Counter (CVP-P), NE, BC0, MSW */
   DSL_uint16_t cntCVP_MSW;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** TPS-TC CRC Counter (CRC-P), NE, BC0, LSW */
   DSL_uint16_t cntCRC_LSW;
   /** TPS-TC CRC Counter (CRC-P), NE, BC0, MSW */
   DSL_uint16_t cntCRC_MSW;
   /** TPS-TC CV Counter (CV-P), NE, BC0, LSW */
   DSL_uint16_t cntCV_LSW;
   /** TPS-TC CV Counter (CV-P), NE, BC0, MSW */
   DSL_uint16_t cntCV_MSW;
   /** TPS-TC CRC Counter (CRCP-P), NE, BC0, LSW */
   DSL_uint16_t cntCRCP_LSW;
   /** TPS-TC CRC Counter (CRCP-P), NE, BC0, MSW */
   DSL_uint16_t cntCRCP_MSW;
   /** TPS-TC CV Counter (CVP-P), NE, BC0, LSW */
   DSL_uint16_t cntCVP_LSW;
   /** TPS-TC CV Counter (CVP-P), NE, BC0, MSW */
   DSL_uint16_t cntCVP_MSW;
#endif
} __PACKED__ ;


/**
   Writes the TPS-TC CRC error and TPS-TC coding violation counters (Section
   7.2.5.1. of G.997.1 [11]).Why writing these counters? For "automoding",
   meaning modems configured to support both ADSL and VDLS with automatic mode
   selection. The first retrain is always done with an ADSL FW independent of
   what was loaded before. Therefore, the counters must be saved by the host
   before destroying them by the FW download and are written back before the
   next link start of a FW that supports PTM.
*/
struct CMD_PTM_BC0_StatsNE_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** TPS-TC CRC Counter (CRC-P), NE, BC0, LSW */
   DSL_uint16_t cntCRC_LSW;
   /** TPS-TC CRC Counter (CRC-P), NE, BC0, MSW */
   DSL_uint16_t cntCRC_MSW;
   /** TPS-TC CV Counter (CV-P), NE, BC0, LSW */
   DSL_uint16_t cntCV_LSW;
   /** TPS-TC CV Counter (CV-P), NE, BC0, MSW */
   DSL_uint16_t cntCV_MSW;
   /** TPS-TC CRC Counter (CRCP-P), NE, BC0, LSW */
   DSL_uint16_t cntCRCP_LSW;
   /** TPS-TC CRC Counter (CRCP-P), NE, BC0, MSW */
   DSL_uint16_t cntCRCP_MSW;
   /** TPS-TC CV Counter (CVP-P), NE, BC0, LSW */
   DSL_uint16_t cntCVP_LSW;
   /** TPS-TC CV Counter (CVP-P), NE, BC0, MSW */
   DSL_uint16_t cntCVP_MSW;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** TPS-TC CRC Counter (CRC-P), NE, BC0, LSW */
   DSL_uint16_t cntCRC_LSW;
   /** TPS-TC CRC Counter (CRC-P), NE, BC0, MSW */
   DSL_uint16_t cntCRC_MSW;
   /** TPS-TC CV Counter (CV-P), NE, BC0, LSW */
   DSL_uint16_t cntCV_LSW;
   /** TPS-TC CV Counter (CV-P), NE, BC0, MSW */
   DSL_uint16_t cntCV_MSW;
   /** TPS-TC CRC Counter (CRCP-P), NE, BC0, LSW */
   DSL_uint16_t cntCRCP_LSW;
   /** TPS-TC CRC Counter (CRCP-P), NE, BC0, MSW */
   DSL_uint16_t cntCRCP_MSW;
   /** TPS-TC CV Counter (CVP-P), NE, BC0, LSW */
   DSL_uint16_t cntCVP_LSW;
   /** TPS-TC CV Counter (CVP-P), NE, BC0, MSW */
   DSL_uint16_t cntCVP_MSW;
#endif
} __PACKED__ ;


/**
   Acknowledgement for the message CMD_PTM_BC0_StatsNE_Set.
*/
struct ACK_PTM_BC0_StatsNE_Set
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
   This command is used to request the near-end data-path performance counters
   for BC0 of an ATM-TC link as defined in Section 7.2.4. of G.997.1 [11].
*/
struct CMD_ATM_BC0_StatsNE_Get
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
   Provides the total near-end ATM-TC performance counters for bearer channel 0
   (BC0) as requested by the command CMD_ATM_BC0_StatsNE_Get.ADSL1 Fast Path
   reports its ATM -TC counters in this message too.
*/
struct ACK_ATM_BC0_StatsNE_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** NE HEC Violation Count (HEC-P), LSW */
   DSL_uint16_t hecp_LSW;
   /** NE HEC Violation Count (HEC-P), MSW */
   DSL_uint16_t hecp_MSW;
   /** NE Delineated Total Cell Count (CD-P), LSW */
   DSL_uint16_t cdp_LSW;
   /** NE Delineated Total Cell Count (CD-P), MSW */
   DSL_uint16_t cdp_MSW;
   /** NE User Total Cell Count (CU-P), LSW */
   DSL_uint16_t cup_LSW;
   /** NE User Total Cell Count (CU-P), MSW */
   DSL_uint16_t cup_MSW;
   /** NE Idle Cell Bit Error Count (IBE-P), LSW */
   DSL_uint16_t ibep_LSW;
   /** NE Idle Cell Bit Error Count (IBE-P), MSW */
   DSL_uint16_t ibep_MSW;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** NE HEC Violation Count (HEC-P), LSW */
   DSL_uint16_t hecp_LSW;
   /** NE HEC Violation Count (HEC-P), MSW */
   DSL_uint16_t hecp_MSW;
   /** NE Delineated Total Cell Count (CD-P), LSW */
   DSL_uint16_t cdp_LSW;
   /** NE Delineated Total Cell Count (CD-P), MSW */
   DSL_uint16_t cdp_MSW;
   /** NE User Total Cell Count (CU-P), LSW */
   DSL_uint16_t cup_LSW;
   /** NE User Total Cell Count (CU-P), MSW */
   DSL_uint16_t cup_MSW;
   /** NE Idle Cell Bit Error Count (IBE-P), LSW */
   DSL_uint16_t ibep_LSW;
   /** NE Idle Cell Bit Error Count (IBE-P), MSW */
   DSL_uint16_t ibep_MSW;
#endif
} __PACKED__ ;


/**
   This command is used to request the far-end data-path performance counters
   for BC0 of an ATM-TC link as defined in Section 7.2.4.2 of G.997.1 [11].
*/
struct CMD_ATM_BC0_StatsFE_Get
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
   Provides the total near-end ATM-TC performance counters for bearer channel 0
   (BC0) as requested by the command CMD_ATM_BC0_StatsFE_Get.
*/
struct ACK_ATM_BC0_StatsFE_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** FE HEC Violation Count (HEC-PFE), LSW */
   DSL_uint16_t hecpfe_LSW;
   /** FE HEC Violation Count (HEC-PFE), MSW */
   DSL_uint16_t hecpfe_MSW;
   /** FE Delineated Total Cell Count (CD-PFE), LSW */
   DSL_uint16_t cdpfe_LSW;
   /** FE Delineated Total Cell Count (CD-PFE), MSW */
   DSL_uint16_t cdpfe_MSW;
   /** FE User Total Cell Count (CU-PFE), LSW */
   DSL_uint16_t cupfe_LSW;
   /** FE User Total Cell Count (CU-PFE), MSW */
   DSL_uint16_t cupfe_MSW;
   /** FE Idle Cell Bit Error Count (IBE-PFE), LSW */
   DSL_uint16_t ibepfe_LSW;
   /** FE Idle Cell Bit Error Count (IBE-PFE), MSW */
   DSL_uint16_t ibepfe_MSW;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** FE HEC Violation Count (HEC-PFE), LSW */
   DSL_uint16_t hecpfe_LSW;
   /** FE HEC Violation Count (HEC-PFE), MSW */
   DSL_uint16_t hecpfe_MSW;
   /** FE Delineated Total Cell Count (CD-PFE), LSW */
   DSL_uint16_t cdpfe_LSW;
   /** FE Delineated Total Cell Count (CD-PFE), MSW */
   DSL_uint16_t cdpfe_MSW;
   /** FE User Total Cell Count (CU-PFE), LSW */
   DSL_uint16_t cupfe_LSW;
   /** FE User Total Cell Count (CU-PFE), MSW */
   DSL_uint16_t cupfe_MSW;
   /** FE Idle Cell Bit Error Count (IBE-PFE), LSW */
   DSL_uint16_t ibepfe_LSW;
   /** FE Idle Cell Bit Error Count (IBE-PFE), MSW */
   DSL_uint16_t ibepfe_MSW;
#endif
} __PACKED__ ;


/**
   Writes the data-path performance counters for bearer channel 0 (BC0) of an
   ATM-TC link (Section 7.2.4. of G.997.1 [11]).Why writing counters? The ADSL
   FW of the VRX destroys the performance counters at the beginning of a re-
   initialization since it starts with a FW download. Therefore the host has to
   save the counters in FAIL state before triggering a re-init. When
   reconfiguring the system in RESET state, the host writes the saved values as
   start values back into the counters. To do this for the ATM-TC counters, this
   message is used.
*/
struct CMD_ATM_BC0_StatsNE_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** NE HEC Violation Count (HEC-P), LSW */
   DSL_uint16_t hecp_LSW;
   /** NE HEC Violation Count (HEC-P), MSW */
   DSL_uint16_t hecp_MSW;
   /** NE Delineated Total Cell Count (CD-P), LSW */
   DSL_uint16_t cdp_LSW;
   /** NE Delineated Total Cell Count (CD-P), MSW */
   DSL_uint16_t cdp_MSW;
   /** NE User Total Cell Count (CU-P), LSW */
   DSL_uint16_t cup_LSW;
   /** NE User Total Cell Count (CU-P), MSW */
   DSL_uint16_t cup_MSW;
   /** NE Idle Cell Bit Error Count (IBE-P), LSW */
   DSL_uint16_t ibep_LSW;
   /** NE Idle Cell Bit Error Count (IBE-P), MSW */
   DSL_uint16_t ibep_MSW;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** NE HEC Violation Count (HEC-P), LSW */
   DSL_uint16_t hecp_LSW;
   /** NE HEC Violation Count (HEC-P), MSW */
   DSL_uint16_t hecp_MSW;
   /** NE Delineated Total Cell Count (CD-P), LSW */
   DSL_uint16_t cdp_LSW;
   /** NE Delineated Total Cell Count (CD-P), MSW */
   DSL_uint16_t cdp_MSW;
   /** NE User Total Cell Count (CU-P), LSW */
   DSL_uint16_t cup_LSW;
   /** NE User Total Cell Count (CU-P), MSW */
   DSL_uint16_t cup_MSW;
   /** NE Idle Cell Bit Error Count (IBE-P), LSW */
   DSL_uint16_t ibep_LSW;
   /** NE Idle Cell Bit Error Count (IBE-P), MSW */
   DSL_uint16_t ibep_MSW;
#endif
} __PACKED__ ;


/**
   Acknowledgement for the message CMD_ATM_BC0_StatsNE_Set.
*/
struct ACK_ATM_BC0_StatsNE_Set
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
   Requests near-end ATM datapath performance transmit counters for bearer
   channel 0 (BC0).
*/
struct CMD_ATM_BC0_TxStatsNE_Get
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
   Provides the ATM-TC transmit counters for beare channel 0 (BC0) as requested
   by the command CMD_ATM_BC0_TxStatsNE_Get.ADSL1 Fast Path reports its ATM -TC
   counters in this message too.
*/
struct ACK_ATM_BC0_TxStatsNE_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Near-end Tx User Cell Count (TxCU), Current Period, LSW */
   DSL_uint16_t TxCU_LSW;
   /** Near-end Tx User Cell Count (TxCU), Current Period, MSW */
   DSL_uint16_t TxCU_MSW;
   /** Near-end Tx Idle Cell Count (TxIdle), Current Period, LSW */
   DSL_uint16_t TxIdle_LSW;
   /** Near-end Tx Idle Cell Count (TxIdle), Current Period, MSW */
   DSL_uint16_t TxIdle_MSW;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Near-end Tx User Cell Count (TxCU), Current Period, LSW */
   DSL_uint16_t TxCU_LSW;
   /** Near-end Tx User Cell Count (TxCU), Current Period, MSW */
   DSL_uint16_t TxCU_MSW;
   /** Near-end Tx Idle Cell Count (TxIdle), Current Period, LSW */
   DSL_uint16_t TxIdle_LSW;
   /** Near-end Tx Idle Cell Count (TxIdle), Current Period, MSW */
   DSL_uint16_t TxIdle_MSW;
#endif
} __PACKED__ ;


/**
   Writes the transmit data-path counters for BC0 of an ATM-TC link.For "Why
   writing performance counters" see Page 649.
*/
struct CMD_ATM_BC0_TxStatsNE_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Near-end Tx User Cell Count (TxCU), Current Period, LSW */
   DSL_uint16_t TxCU_LSW;
   /** Near-end Tx User Cell Count (TxCU), Current Period, MSW */
   DSL_uint16_t TxCU_MSW;
   /** Near-end Tx Idle Cell Count (TxIdle), Current Period, LSW */
   DSL_uint16_t TxIdle_LSW;
   /** Near-end Tx Idle Cell Count (TxIdle), Current Period, MSW */
   DSL_uint16_t TxIdle_MSW;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Near-end Tx User Cell Count (TxCU), Current Period, LSW */
   DSL_uint16_t TxCU_LSW;
   /** Near-end Tx User Cell Count (TxCU), Current Period, MSW */
   DSL_uint16_t TxCU_MSW;
   /** Near-end Tx Idle Cell Count (TxIdle), Current Period, LSW */
   DSL_uint16_t TxIdle_LSW;
   /** Near-end Tx Idle Cell Count (TxIdle), Current Period, MSW */
   DSL_uint16_t TxIdle_MSW;
#endif
} __PACKED__ ;


/**
   Acknowledgement for the message CMD_ATM_BC0_TxStatsNE_Set.
*/
struct ACK_ATM_BC0_TxStatsNE_Set
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
   Requests version information from the VDSL2-Modem.
*/
struct CMD_VersionInfoGet
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
   Provides version information about hardware and firmware. The FW version
   numbering scheme has a long and a short form (Table 353).
*/
struct ACK_VersionInfoGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Hardware Version Number */
   DSL_uint32_t HW_Version;
   /** Reserved */
   DSL_uint32_t Res0;
   /** Firmware Version Number (Long) */
   DSL_uint32_t FW_Version;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Hardware Version Number */
   DSL_uint32_t HW_Version;
   /** Reserved */
   DSL_uint32_t Res0;
   /** Firmware Version Number (Long) */
   DSL_uint32_t FW_Version;
#endif
} __PACKED__ ;


/**
   Requests the feature map indicating the ADSL FW features supported by this
   binary.
*/
struct CMD_ADSL_FeatureMapGet
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
   Reports the feature map indicating the ADSL FW features supported by this
   binary.
*/
struct ACK_ADSL_FeatureMapGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Feature-Bit15 of Word 0: ATM Off-chip Bonding */
   DSL_uint16_t W0F15 : 1;
   /** Feature-Bit14 of Word 0: ATM On-chip Bonding */
   DSL_uint16_t W0F14 : 1;
   /** Feature-Bit13 of Word 0: PTM Off-chip Bonding BACP */
   DSL_uint16_t W0F13 : 1;
   /** Feature-Bit12 of Word 0: PTM On-chip Bonding BACP */
   DSL_uint16_t W0F12 : 1;
   /** Feature-Bit11 of Word 0: PTM Off-chip bonding (GHS-based) */
   DSL_uint16_t W0F11 : 1;
   /** Feature-Bit10 of Word 0: PTM On-chip bonding (GHS-based) */
   DSL_uint16_t W0F10 : 1;
   /** Feature-Bit9 of Word 0: Not used in ADSL */
   DSL_uint16_t W0F09 : 1;
   /** Feature-Bit8 of Word 0: PTM-TC for ADSL2x  */
   DSL_uint16_t W0F08 : 1;
   /** Feature-Bit7 of Word 0: AnnexM/J US PSD Shaping (ADSL only) */
   DSL_uint16_t W0F07 : 1;
   /** Feature-Bit6 of Word 0: AnnexM/J US Mask Selection (ADSL only) */
   DSL_uint16_t W0F06 : 1;
   /** Feature-Bit5 of Word 0: Annex M (ADSL only) */
   DSL_uint16_t W0F05 : 1;
   /** Feature-Bit4 of Word 0: Annex J (ADSL only) */
   DSL_uint16_t W0F04 : 1;
   /** Feature-Bit3 of Word 0: Annex I (ADSL only) */
   DSL_uint16_t W0F03 : 1;
   /** Feature-Bit2 of Word 0: Annex B (ADSL only) */
   DSL_uint16_t W0F02 : 1;
   /** Feature-Bit1 of Word 0: T1.413 (ADSL only) */
   DSL_uint16_t W0F01 : 1;
   /** Feature-Bit0 of Word 0: Annex A (incl. Annex L) (ADSL only) */
   DSL_uint16_t W0F00 : 1;
   /** Feature-Bit15 of Word 1: NTR */
   DSL_uint16_t W1F15 : 1;
   /** Feature-Bit14 of Word 1: CIPolicy */
   DSL_uint16_t W1F14 : 1;
   /** Feature-Bit13 of Word 1: Pre-emption & Short Packets  */
   DSL_uint16_t W1F13 : 1;
   /** Feature-Bit12 of Word 1: DELT */
   DSL_uint16_t W1F12 : 1;
   /** Feature-Bit11 of Word 1: Dual Latency */
   DSL_uint16_t W1F11 : 1;
   /** Feature-Bit10 of Word 1: Short init (ADSL only) */
   DSL_uint16_t W1F10 : 1;
   /** Feature-Bit9 of Word 1: Virtual Noise */
   DSL_uint16_t W1F09 : 1;
   /** Feature-Bit8 of Word 1: Erasure Decoding /FORCEINP */
   DSL_uint16_t W1F08 : 1;
   /** Feature-Bit7 of Word 1: L2 (ADSL only) */
   DSL_uint16_t W1F07 : 1;
   /** Feature-Bit6 of Word 1: G.INP Retransmission US + DS + SRA */
   DSL_uint16_t W1F06 : 1;
   /** Feature-Bit5 of Word 1: G.INP Retransmission DS + SRA */
   DSL_uint16_t W1F05 : 1;
   /** Feature-Bit4 of Word 1: G.INP Retransmission US + DS */
   DSL_uint16_t W1F04 : 1;
   /** Feature-Bit3 of Word 1: G.INP Retransmission DS */
   DSL_uint16_t W1F03 : 1;
   /** Feature-Bit2 of Word 1: Not used in ADSL */
   DSL_uint16_t W1F02 : 1;
   /** Feature-Bit1 of Word 1: SRA */
   DSL_uint16_t W1F01 : 1;
   /** Feature-Bit0 of Word 1: Bitswap */
   DSL_uint16_t W1F00 : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 13;
   /** Feature-Bit2 of Word 2: Microfilter Detection and Hybrid Data */
   DSL_uint16_t W2F02 : 1;
   /** Feature-Bit1 of Word 2: Real Time Trace (RTT) */
   DSL_uint16_t W2F01 : 1;
   /** Feature-Bit0 of Word 2: Clear EOC */
   DSL_uint16_t W2F00 : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 16;
   /** Reserved */
   DSL_uint16_t Res2 : 16;
   /** Reserved */
   DSL_uint16_t Res3 : 16;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Feature-Bit0 of Word 0: Annex A (incl. Annex L) (ADSL only) */
   DSL_uint16_t W0F00 : 1;
   /** Feature-Bit1 of Word 0: T1.413 (ADSL only) */
   DSL_uint16_t W0F01 : 1;
   /** Feature-Bit2 of Word 0: Annex B (ADSL only) */
   DSL_uint16_t W0F02 : 1;
   /** Feature-Bit3 of Word 0: Annex I (ADSL only) */
   DSL_uint16_t W0F03 : 1;
   /** Feature-Bit4 of Word 0: Annex J (ADSL only) */
   DSL_uint16_t W0F04 : 1;
   /** Feature-Bit5 of Word 0: Annex M (ADSL only) */
   DSL_uint16_t W0F05 : 1;
   /** Feature-Bit6 of Word 0: AnnexM/J US Mask Selection (ADSL only) */
   DSL_uint16_t W0F06 : 1;
   /** Feature-Bit7 of Word 0: AnnexM/J US PSD Shaping (ADSL only) */
   DSL_uint16_t W0F07 : 1;
   /** Feature-Bit8 of Word 0: PTM-TC for ADSL2x  */
   DSL_uint16_t W0F08 : 1;
   /** Feature-Bit9 of Word 0: Not used in ADSL */
   DSL_uint16_t W0F09 : 1;
   /** Feature-Bit10 of Word 0: PTM On-chip bonding (GHS-based) */
   DSL_uint16_t W0F10 : 1;
   /** Feature-Bit11 of Word 0: PTM Off-chip bonding (GHS-based) */
   DSL_uint16_t W0F11 : 1;
   /** Feature-Bit12 of Word 0: PTM On-chip Bonding BACP */
   DSL_uint16_t W0F12 : 1;
   /** Feature-Bit13 of Word 0: PTM Off-chip Bonding BACP */
   DSL_uint16_t W0F13 : 1;
   /** Feature-Bit14 of Word 0: ATM On-chip Bonding */
   DSL_uint16_t W0F14 : 1;
   /** Feature-Bit15 of Word 0: ATM Off-chip Bonding */
   DSL_uint16_t W0F15 : 1;
   /** Feature-Bit0 of Word 1: Bitswap */
   DSL_uint16_t W1F00 : 1;
   /** Feature-Bit1 of Word 1: SRA */
   DSL_uint16_t W1F01 : 1;
   /** Feature-Bit2 of Word 1: Not used in ADSL */
   DSL_uint16_t W1F02 : 1;
   /** Feature-Bit3 of Word 1: G.INP Retransmission DS */
   DSL_uint16_t W1F03 : 1;
   /** Feature-Bit4 of Word 1: G.INP Retransmission US + DS */
   DSL_uint16_t W1F04 : 1;
   /** Feature-Bit5 of Word 1: G.INP Retransmission DS + SRA */
   DSL_uint16_t W1F05 : 1;
   /** Feature-Bit6 of Word 1: G.INP Retransmission US + DS + SRA */
   DSL_uint16_t W1F06 : 1;
   /** Feature-Bit7 of Word 1: L2 (ADSL only) */
   DSL_uint16_t W1F07 : 1;
   /** Feature-Bit8 of Word 1: Erasure Decoding /FORCEINP */
   DSL_uint16_t W1F08 : 1;
   /** Feature-Bit9 of Word 1: Virtual Noise */
   DSL_uint16_t W1F09 : 1;
   /** Feature-Bit10 of Word 1: Short init (ADSL only) */
   DSL_uint16_t W1F10 : 1;
   /** Feature-Bit11 of Word 1: Dual Latency */
   DSL_uint16_t W1F11 : 1;
   /** Feature-Bit12 of Word 1: DELT */
   DSL_uint16_t W1F12 : 1;
   /** Feature-Bit13 of Word 1: Pre-emption & Short Packets  */
   DSL_uint16_t W1F13 : 1;
   /** Feature-Bit14 of Word 1: CIPolicy */
   DSL_uint16_t W1F14 : 1;
   /** Feature-Bit15 of Word 1: NTR */
   DSL_uint16_t W1F15 : 1;
   /** Feature-Bit0 of Word 2: Clear EOC */
   DSL_uint16_t W2F00 : 1;
   /** Feature-Bit1 of Word 2: Real Time Trace (RTT) */
   DSL_uint16_t W2F01 : 1;
   /** Feature-Bit2 of Word 2: Microfilter Detection and Hybrid Data */
   DSL_uint16_t W2F02 : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 13;
   /** Reserved */
   DSL_uint16_t Res1 : 16;
   /** Reserved */
   DSL_uint16_t Res2 : 16;
   /** Reserved */
   DSL_uint16_t Res3 : 16;
#endif
} __PACKED__ ;


/**
   Requests the feature map indicating the VDSL FW features supported by this
   binary.
*/
struct CMD_VDSL_FeatureMapGet
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
   Reports the feature map indicating the VDSL FW features supported by this
   binary.
*/
struct ACK_VDSL_FeatureMapGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Feature-Bit15 of Word 0: ATM Off-chip Bonding */
   DSL_uint16_t W0F15 : 1;
   /** Feature-Bit14 of Word 0: ATM On-chip Bonding */
   DSL_uint16_t W0F14 : 1;
   /** Feature-Bit13 of Word 0: PTM Off-chip Bonding BACP */
   DSL_uint16_t W0F13 : 1;
   /** Feature-Bit12 of Word 0: PTM On-chip Bonding BACP */
   DSL_uint16_t W0F12 : 1;
   /** Feature-Bit11 of Word 0: PTM Off-chip bonding (GHS-based) */
   DSL_uint16_t W0F11 : 1;
   /** Feature-Bit10 of Word 0: PTM On-chip bonding (GHS-based) */
   DSL_uint16_t W0F10 : 1;
   /** Feature-Bit9 of Word 0: Profile 30  (VDSL only) */
   DSL_uint16_t W0F09 : 1;
   /** Feature-Bits 8:0 of Word 0: Not used in VDSL */
   DSL_uint16_t Res0 : 9;
   /** Feature-Bit15 of Word 1: NTR */
   DSL_uint16_t W1F15 : 1;
   /** Feature-Bit14 of Word 1: CIPolicy */
   DSL_uint16_t W1F14 : 1;
   /** Feature-Bit13 of Word 1: Pre-emption & Short Packets  */
   DSL_uint16_t W1F13 : 1;
   /** Feature-Bit12 of Word 1: DELT */
   DSL_uint16_t W1F12 : 1;
   /** Feature-Bit11 of Word 1: Dual Latency */
   DSL_uint16_t W1F11 : 1;
   /** Feature-Bit10 of Word 1: Not used in VDSL */
   DSL_uint16_t Res1 : 1;
   /** Feature-Bit9 of Word 1: Virtual Noise */
   DSL_uint16_t W1F09 : 1;
   /** Feature-Bit8 of Word 1: Erasure Decoding /FORCEINP */
   DSL_uint16_t W1F08 : 1;
   /** Feature-Bit7 of Word 1: Not used in VDSL */
   DSL_uint16_t Res2 : 1;
   /** Feature-Bit6 of Word 1: G.INP Retransmission US + DS + SRA */
   DSL_uint16_t W1F06 : 1;
   /** Feature-Bit5 of Word 1: G.INP Retransmission DS + SRA */
   DSL_uint16_t W1F05 : 1;
   /** Feature-Bit4 of Word 1: G.INP Retransmission US + DS */
   DSL_uint16_t W1F04 : 1;
   /** Feature-Bit3 of Word 1: G.INP Retransmission DS */
   DSL_uint16_t W1F03 : 1;
   /** Feature-Bit2 of Word 1: SOS (VDSL only) */
   DSL_uint16_t W1F02 : 1;
   /** Feature-Bit1 of Word 1: SRA */
   DSL_uint16_t W1F01 : 1;
   /** Feature-Bit0 of Word 1: Bitswap */
   DSL_uint16_t W1F00 : 1;
   /** Reserved */
   DSL_uint16_t Res3 : 12;
   /** Feature-Bit3 of Word 2: AEC/2nd DAC */
   DSL_uint16_t W2F03 : 1;
   /** Feature-Bit2 of Word 2: Microfilter Detection and Hybrid Data */
   DSL_uint16_t W2F02 : 1;
   /** Feature-Bit1 of Word 2: Real Time Trace (RTT) */
   DSL_uint16_t W2F01 : 1;
   /** Feature-Bit0 of Word 2: Clear EOC */
   DSL_uint16_t W2F00 : 1;
   /** Reserved */
   DSL_uint16_t Res4 : 15;
   /** Feature-Bit0 of Word 3: Vectoring (fully standard compliant) */
   DSL_uint16_t W3F00 : 1;
   /** Reserved */
   DSL_uint16_t Res5 : 16;
   /** Reserved */
   DSL_uint16_t Res6 : 16;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Feature-Bits 8:0 of Word 0: Not used in VDSL */
   DSL_uint16_t Res0 : 9;
   /** Feature-Bit9 of Word 0: Profile 30  (VDSL only) */
   DSL_uint16_t W0F09 : 1;
   /** Feature-Bit10 of Word 0: PTM On-chip bonding (GHS-based) */
   DSL_uint16_t W0F10 : 1;
   /** Feature-Bit11 of Word 0: PTM Off-chip bonding (GHS-based) */
   DSL_uint16_t W0F11 : 1;
   /** Feature-Bit12 of Word 0: PTM On-chip Bonding BACP */
   DSL_uint16_t W0F12 : 1;
   /** Feature-Bit13 of Word 0: PTM Off-chip Bonding BACP */
   DSL_uint16_t W0F13 : 1;
   /** Feature-Bit14 of Word 0: ATM On-chip Bonding */
   DSL_uint16_t W0F14 : 1;
   /** Feature-Bit15 of Word 0: ATM Off-chip Bonding */
   DSL_uint16_t W0F15 : 1;
   /** Feature-Bit0 of Word 1: Bitswap */
   DSL_uint16_t W1F00 : 1;
   /** Feature-Bit1 of Word 1: SRA */
   DSL_uint16_t W1F01 : 1;
   /** Feature-Bit2 of Word 1: SOS (VDSL only) */
   DSL_uint16_t W1F02 : 1;
   /** Feature-Bit3 of Word 1: G.INP Retransmission DS */
   DSL_uint16_t W1F03 : 1;
   /** Feature-Bit4 of Word 1: G.INP Retransmission US + DS */
   DSL_uint16_t W1F04 : 1;
   /** Feature-Bit5 of Word 1: G.INP Retransmission DS + SRA */
   DSL_uint16_t W1F05 : 1;
   /** Feature-Bit6 of Word 1: G.INP Retransmission US + DS + SRA */
   DSL_uint16_t W1F06 : 1;
   /** Feature-Bit7 of Word 1: Not used in VDSL */
   DSL_uint16_t Res2 : 1;
   /** Feature-Bit8 of Word 1: Erasure Decoding /FORCEINP */
   DSL_uint16_t W1F08 : 1;
   /** Feature-Bit9 of Word 1: Virtual Noise */
   DSL_uint16_t W1F09 : 1;
   /** Feature-Bit10 of Word 1: Not used in VDSL */
   DSL_uint16_t Res1 : 1;
   /** Feature-Bit11 of Word 1: Dual Latency */
   DSL_uint16_t W1F11 : 1;
   /** Feature-Bit12 of Word 1: DELT */
   DSL_uint16_t W1F12 : 1;
   /** Feature-Bit13 of Word 1: Pre-emption & Short Packets  */
   DSL_uint16_t W1F13 : 1;
   /** Feature-Bit14 of Word 1: CIPolicy */
   DSL_uint16_t W1F14 : 1;
   /** Feature-Bit15 of Word 1: NTR */
   DSL_uint16_t W1F15 : 1;
   /** Feature-Bit0 of Word 2: Clear EOC */
   DSL_uint16_t W2F00 : 1;
   /** Feature-Bit1 of Word 2: Real Time Trace (RTT) */
   DSL_uint16_t W2F01 : 1;
   /** Feature-Bit2 of Word 2: Microfilter Detection and Hybrid Data */
   DSL_uint16_t W2F02 : 1;
   /** Feature-Bit3 of Word 2: AEC/2nd DAC */
   DSL_uint16_t W2F03 : 1;
   /** Reserved */
   DSL_uint16_t Res3 : 12;
   /** Feature-Bit0 of Word 3: Vectoring (fully standard compliant) */
   DSL_uint16_t W3F00 : 1;
   /** Reserved */
   DSL_uint16_t Res4 : 15;
   /** Reserved */
   DSL_uint16_t Res5 : 16;
   /** Reserved */
   DSL_uint16_t Res6 : 16;
#endif
} __PACKED__ ;


/**
   The message is used to enable bonding (PAF) and to control the GHS
   aggregation discovery and aggregation processes. The register numbers
   referenced refer to IEEE 802.3ah [14], Clause 45.
*/
struct CMD_PAF_HS_Control
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 15;
   /** PCS Control Register (Register 3.61, Bit 0) */
   DSL_uint16_t PAF_Enable : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** PCS Control Register (Register 3.61, Bit 0) */
   DSL_uint16_t PAF_Enable : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 15;
#endif
} __PACKED__ ;


/**
   The message acknowledges CMD_PAF_HS_Control.
*/
struct ACK_PAF_HS_Control
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
   The message reads back settings for bonding configured with
   CMD_PAF_HS_Control.
*/
struct CMD_PAF_HS_ControlGet
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
   The message acknowledges CMD_PAF_HS_ControlGet providing PAF bonding
   parameters used in handshake as configured with CMD_PAF_HS_Control.
*/
struct ACK_PAF_HS_ControlGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 15;
   /** PCS Control Register (Register 3.61, Bit 0) */
   DSL_uint16_t PAF_Enable : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** PCS Control Register (Register 3.61, Bit 0) */
   DSL_uint16_t PAF_Enable : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 15;
#endif
} __PACKED__ ;


/**
   The message requests aggregation discovery and aggregation status information
   for EFM bonding.
*/
struct CMD_PAF_HS_StatusGet
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
   The message reports discovery and aggregation status information for EFM
   bonding after request by CMD_PAF_HS_StatusGet.At the CPE side, the Host SW
   concludes on a GET request if none of the following 4 bits are set:
   discoveryClearIfSame, discoverySetIfClear, aggregClear , aggregSet. It does
   not distinguish between "discovery GET" and "aggregation GET"; in case of any
   "GET" both the discovery and the aggregation code are sent to the CO with the
   CLR.
*/
struct ACK_PAF_HS_StatusGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 9;
   /** Discovery Clear-If-Same */
   DSL_uint16_t discoveryClearIfSame : 1;
   /** Discovery Set-If-Clear */
   DSL_uint16_t discoverySetIfClear : 1;
   /** Aggregation Clear */
   DSL_uint16_t aggregClear : 1;
   /** Aggregation Set */
   DSL_uint16_t aggregSet : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 2;
   /** PCS Control Register (Register 3.60, Bit 12) */
   DSL_uint16_t PAF_Enable : 1;
   /** Aggregation Discovery Code (Register 6.18) */
   DSL_uint16_t discoveryCode1;
   /** Aggregation Discovery Code (Registers 6.19) */
   DSL_uint16_t discoveryCode2;
   /** Aggregation Discovery Code (Registers 6.20) */
   DSL_uint16_t discoveryCode3;
   /** Partner PME Aggregate Data (Registers 6.22) */
   DSL_uint16_t aggregateData1;
   /** Partner PME Aggregate Data (Registers 6.23) */
   DSL_uint16_t aggregateData2;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** PCS Control Register (Register 3.60, Bit 12) */
   DSL_uint16_t PAF_Enable : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 2;
   /** Aggregation Set */
   DSL_uint16_t aggregSet : 1;
   /** Aggregation Clear */
   DSL_uint16_t aggregClear : 1;
   /** Discovery Set-If-Clear */
   DSL_uint16_t discoverySetIfClear : 1;
   /** Discovery Clear-If-Same */
   DSL_uint16_t discoveryClearIfSame : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 9;
   /** Aggregation Discovery Code (Register 6.18) */
   DSL_uint16_t discoveryCode1;
   /** Aggregation Discovery Code (Registers 6.19) */
   DSL_uint16_t discoveryCode2;
   /** Aggregation Discovery Code (Registers 6.20) */
   DSL_uint16_t discoveryCode3;
   /** Partner PME Aggregate Data (Registers 6.22) */
   DSL_uint16_t aggregateData1;
   /** Partner PME Aggregate Data (Registers 6.23) */
   DSL_uint16_t aggregateData2;
#endif
} __PACKED__ ;


/**
   The message writes the aggregation discovery code and the aggregation data
   down to the FW and is used to trigger the CPE-FW to proceed in handshake
   (sending CLR) after it had been waiting in GHS_BONDING_CLR state for the
   necessary host interactions to be finished. These actions depend on the
   request type (see ACK_PAF_HS_StatusGet): the processing of the remote
   discovery register (for an aggregation discovery "Set If Clear" or "Clear If
   Same" command) or PME Aggregate register (for an aggregation "Set" or "Clear"
   command) in the SW and -in any case including a "Get" command - then updating
   the local representations of both in the FW (which are sent to the CO in a
   CLR).
*/
struct CMD_PAF_HS_Continue
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 15;
   /** Continue Trigger */
   DSL_uint16_t trigger : 1;
   /** Aggregation Discovery Code (Register 6.18) */
   DSL_uint16_t discoveryCode1;
   /** Aggregation Discovery Code (Registers 6.19) */
   DSL_uint16_t discoveryCode2;
   /** Aggregation Discovery Code (Registers 6.20) */
   DSL_uint16_t discoveryCode3;
   /** Partner PME Aggregate Data (Registers 6.22) */
   DSL_uint16_t aggregateData1;
   /** Partner PME Aggregate Data (Registers 6.23) */
   DSL_uint16_t aggregateData2;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Continue Trigger */
   DSL_uint16_t trigger : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 15;
   /** Aggregation Discovery Code (Register 6.18) */
   DSL_uint16_t discoveryCode1;
   /** Aggregation Discovery Code (Registers 6.19) */
   DSL_uint16_t discoveryCode2;
   /** Aggregation Discovery Code (Registers 6.20) */
   DSL_uint16_t discoveryCode3;
   /** Partner PME Aggregate Data (Registers 6.22) */
   DSL_uint16_t aggregateData1;
   /** Partner PME Aggregate Data (Registers 6.23) */
   DSL_uint16_t aggregateData2;
#endif
} __PACKED__ ;


/**
   The message acknowledges CMD_PAF_HS_Continue.
*/
struct ACK_PAF_HS_Continue
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

#endif /** _DRV_DSL_CPE_VRX_MSG_TC_COUNTERS_H_*/
