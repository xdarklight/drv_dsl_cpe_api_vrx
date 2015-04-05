/******************************************************************************

                              Copyright (c) 2012
                            Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/


#ifndef _DRV_DSL_CPE_VRX_MSG_PERF_PMD_H_
#define _DRV_DSL_CPE_VRX_MSG_PERF_PMD_H_

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


/** @defgroup _PERF_PMD_
 *  @{
 */

#ifdef __cplusplus
   extern "C" {
#endif

/* ----- Message Specific Constants Definition section ----- */
/* ----- Message Specific Constants Definition section (End) ----- */

/** Message ID for CMD_CounterControlSet */
#define CMD_COUNTERCONTROLSET 0x004A

/**
   Controls the behaviour of the performance counters.
*/
typedef struct CMD_CounterControlSet CMD_CounterControlSet_t;

/** Message ID for ACK_CounterControlSet */
#define ACK_COUNTERCONTROLSET 0x004A

/**
   Acknowledgement for message CMD_CounterControlSet.
*/
typedef struct ACK_CounterControlSet ACK_CounterControlSet_t;

/** Message ID for ALM_LineFailureNE_Get */
#define ALM_LINEFAILURENE_GET 0x0007

/**
   This autonomous message indicates near-end line failures.
*/
typedef struct ALM_LineFailureNE_Get ALM_LineFailureNE_Get_t;

/** Message ID for CMD_LineFailureNE_Get */
#define CMD_LINEFAILURENE_GET 0x0007

/**
   Requests the near-end line failure status.
*/
typedef struct CMD_LineFailureNE_Get CMD_LineFailureNE_Get_t;

/** Message ID for ACK_LineFailureNE_Get */
#define ACK_LINEFAILURENE_GET 0x0007

/**
   Provides the near-end line failure status requested by the command
   CMD_LineFailureNE_Get.
*/
typedef struct ACK_LineFailureNE_Get ACK_LineFailureNE_Get_t;

/** Message ID for ALM_LineFailureFE_Get */
#define ALM_LINEFAILUREFE_GET 0x0107

/**
   This autonomous message indicates far-end line failures.
*/
typedef struct ALM_LineFailureFE_Get ALM_LineFailureFE_Get_t;

/** Message ID for CMD_LineFailureFE_Get */
#define CMD_LINEFAILUREFE_GET 0x0107

/**
   Requests the far-end line failure status.
*/
typedef struct CMD_LineFailureFE_Get CMD_LineFailureFE_Get_t;

/** Message ID for ACK_LineFailureFE_Get */
#define ACK_LINEFAILUREFE_GET 0x0107

/**
   Provides the far-end line failure status requested by the command
   CMD_LineFailureFE_Get.
*/
typedef struct ACK_LineFailureFE_Get ACK_LineFailureFE_Get_t;

/** Message ID for CMD_Failure_Trigger */
#define CMD_FAILURE_TRIGGER 0x0562

/**
   Host initiated "loss of power". Triggers indicating an lpr primitive to the
   remote side via the indicator bits ("IB bits").The host writes the message to
   the FW e.g. after detecting LPR by an alternative detection scheme.
*/
typedef struct CMD_Failure_Trigger CMD_Failure_Trigger_t;

/** Message ID for ACK_Failure_Trigger */
#define ACK_FAILURE_TRIGGER 0x0562

/**
   Acknowledgement for CMD_Failure_Trigger.
*/
typedef struct ACK_Failure_Trigger ACK_Failure_Trigger_t;

/** Message ID for CMD_LinePerfCountNE_Get */
#define CMD_LINEPERFCOUNTNE_GET 0x020A

/**
   Requests near-end line performance counter values (Section 7.2.1.1 of
   G.997.1). These are wrap-around counters which are not affected by the
   TR1-period.
*/
typedef struct CMD_LinePerfCountNE_Get CMD_LinePerfCountNE_Get_t;

/** Message ID for ACK_LinePerfCountNE_Get */
#define ACK_LINEPERFCOUNTNE_GET 0x020A

/**
   Provides near-end line performance counter values requested by
   CMD_LinePerfCountNE_Get (Section 7.2.1.1 of G.997.1). These are wrap-around
   counters which are not affected by the TR1-period.
*/
typedef struct ACK_LinePerfCountNE_Get ACK_LinePerfCountNE_Get_t;

/** Message ID for CMD_LinePerfCountFE_Get */
#define CMD_LINEPERFCOUNTFE_GET 0x040A

/**
   Requests far-end line performance counter values (Section 7.2.1.2 of
   G.997.1). These are wrap-around counters which are not affected by the
   TR1-period.
*/
typedef struct CMD_LinePerfCountFE_Get CMD_LinePerfCountFE_Get_t;

/** Message ID for ACK_LinePerfCountFE_Get */
#define ACK_LINEPERFCOUNTFE_GET 0x040A

/**
   Provides far-end line performance counter values requested by
   CMD_LinePerfCountFE_Get (Section 7.2.1.2 of G.997.1). These are wrap-around
   counters which are not affected by the TR1-period.
*/
typedef struct ACK_LinePerfCountFE_Get ACK_LinePerfCountFE_Get_t;

/** Message ID for CMD_CRC_StatsNE_Get */
#define CMD_CRC_STATSNE_GET 0x080A

/**
   Requests near-end CRC counter values. These are wrap-around counters which
   are not affected by the TR1-period.
*/
typedef struct CMD_CRC_StatsNE_Get CMD_CRC_StatsNE_Get_t;

/** Message ID for ACK_CRC_StatsNE_Get */
#define ACK_CRC_STATSNE_GET 0x080A

/**
   Returns near-end CRC counter values. (Section 7.2.2.1.1 of G997.1). These are
   wrap-around counters which are not affected by the TR1-period.The "LP1/Fast
   Path" parameters are fed only in ADSL1 mode with selected fast path.
*/
typedef struct ACK_CRC_StatsNE_Get ACK_CRC_StatsNE_Get_t;

/** Message ID for CMD_CRC_StatsFE_Get */
#define CMD_CRC_STATSFE_GET 0x0C0A

/**
   Requests far-end CRC counter values. These are wrap-around counters which are
   not affected by the TR1-period.
*/
typedef struct CMD_CRC_StatsFE_Get CMD_CRC_StatsFE_Get_t;

/** Message ID for ACK_CRC_StatsFE_Get */
#define ACK_CRC_STATSFE_GET 0x0C0A

/**
   Returns far-end CRC counter values. (Section 7.2.2.2.1 of G997.1). These are
   wrap-around counters which are not affected by the TR1-period.The "LP1/Fast
   Path" parameters are fed only in ADSL1 mode with selected fast path.
*/
typedef struct ACK_CRC_StatsFE_Get ACK_CRC_StatsFE_Get_t;

/** Message ID for CMD_FEC_StatsNE_Get */
#define CMD_FEC_STATSNE_GET 0x0A0A

/**
   Requests near-end FEC counter values. These are wrap-around counters which
   are not affected by the TR1-period.
*/
typedef struct CMD_FEC_StatsNE_Get CMD_FEC_StatsNE_Get_t;

/** Message ID for ACK_FEC_StatsNE_Get */
#define ACK_FEC_STATSNE_GET 0x0A0A

/**
   Returns near-end FEC counter values (Section 7.2.2.1.2 of G997.1). These are
   wrap-around counters which are not affected by the TR1-period.The "LP1/Fast
   Path" parameters are fed only in ADSL1 mode with selected fast path.
*/
typedef struct ACK_FEC_StatsNE_Get ACK_FEC_StatsNE_Get_t;

/** Message ID for CMD_FEC_StatsFE_Get */
#define CMD_FEC_STATSFE_GET 0x0E0A

/**
   Requests far-end FEC counter values. These are wrap-around counters which are
   not affected by the TR1-period.
*/
typedef struct CMD_FEC_StatsFE_Get CMD_FEC_StatsFE_Get_t;

/** Message ID for ACK_FEC_StatsFE_Get */
#define ACK_FEC_STATSFE_GET 0x0E0A

/**
   Returns far-end FEC counter values (Section 7.2.2.2.2 of G997.1). These are
   wrap-around counters which are not affected by the TR1-period.The "LP1/Fast
   Path" parameters are fed only in ADSL1 mode with selected fast path.
*/
typedef struct ACK_FEC_StatsFE_Get ACK_FEC_StatsFE_Get_t;

/** Message ID for CMD_SOS_StatsNE_Get */
#define CMD_SOS_STATSNE_GET 0x290A

/**
   Requests the near-end 15 minutes SOS counter values.
*/
typedef struct CMD_SOS_StatsNE_Get CMD_SOS_StatsNE_Get_t;

/** Message ID for ACK_SOS_StatsNE_Get */
#define ACK_SOS_STATSNE_GET 0x290A

/**
   Returns near-end SOS counter values. (Section 7.2.1.6.1 of G997.1)
*/
typedef struct ACK_SOS_StatsNE_Get ACK_SOS_StatsNE_Get_t;

/** Message ID for CMD_SOS_StatsFE_Get */
#define CMD_SOS_STATSFE_GET 0x2A0A

/**
   Requests the far-end 15 minutes SOS counter values. These are wrap-around
   counters which are not affected by the TR1-period.
*/
typedef struct CMD_SOS_StatsFE_Get CMD_SOS_StatsFE_Get_t;

/** Message ID for ACK_SOS_StatsFE_Get */
#define ACK_SOS_STATSFE_GET 0x2A0A

/**
   Returns far-end SOS counter values. (Section 7.2.1.7.1 of G997.1)
*/
typedef struct ACK_SOS_StatsFE_Get ACK_SOS_StatsFE_Get_t;

/** Message ID for CMD_LinePerfCountNE_Set */
#define CMD_LINEPERFCOUNTNE_SET 0x024A

/**
   Writes near-end line performance counter values (total wrap-around)(Section
   7.2.1.1 of G.997.1).
*/
typedef struct CMD_LinePerfCountNE_Set CMD_LinePerfCountNE_Set_t;

/** Message ID for ACK_LinePerfCountNE_Set */
#define ACK_LINEPERFCOUNTNE_SET 0x024A

/**
   Acknowledgement for CMD_LinePerfCountNE_Set.
*/
typedef struct ACK_LinePerfCountNE_Set ACK_LinePerfCountNE_Set_t;

/** Message ID for CMD_CRC_StatsNE_Set */
#define CMD_CRC_STATSNE_SET 0x084A

/**
   Writes the near-end CRC counter values (Section 7.2.2.1.1 of G997.1)The
   "LP1/Fast Path" parameters are used only in ADSL1 mode with selected "Fast
   Path".
*/
typedef struct CMD_CRC_StatsNE_Set CMD_CRC_StatsNE_Set_t;

/** Message ID for ACK_CRC_StatsNE_Set */
#define ACK_CRC_STATSNE_SET 0x094A

/**
   Acknowledgement for the message CMD_CRC_StatsNE_Set.
*/
typedef struct ACK_CRC_StatsNE_Set ACK_CRC_StatsNE_Set_t;

/** Message ID for CMD_FEC_StatsNE_Set */
#define CMD_FEC_STATSNE_SET 0x0A4A

/**
   Writes near-end FEC counter values. (Section 7.2.2.1.2 of G997.1)The
   "LP1/Fast Path" parameters are used only in ADSL1 mode with selected "Fast
   Path".
*/
typedef struct CMD_FEC_StatsNE_Set CMD_FEC_StatsNE_Set_t;

/** Message ID for ACK_FEC_StatsNE_Set */
#define ACK_FEC_STATSNE_SET 0x0A4A

/**
   Acknowledgement for the message CMD_FEC_StatsNE_Set.
*/
typedef struct ACK_FEC_StatsNE_Set ACK_FEC_StatsNE_Set_t;

/** Message ID for  CMD_SOS_StatsNE_Set */
#define  CMD_SOS_STATSNE_SET 0x294A

/**
   Writes near-end SOS counter values. ("SOS-SUCCESS-NE", Section 7.2.1.6.1 of
   G997.1).
*/
typedef struct  CMD_SOS_StatsNE_Set CMD_SOS_StatsNE_Set_t;

/** Message ID for  ACK_SOS_StatsNE_Set */
#define  ACK_SOS_STATSNE_SET 0x294A

/**
   Acknowledgement for CMD_SOS_StatsNE_Set.
*/
typedef struct  ACK_SOS_StatsNE_Set ACK_SOS_StatsNE_Set_t;

/** Message ID for CMD_TimeSet */
#define CMD_TIMESET 0x6143

/**
   Sets the running time since midnightModifying the Time does not affect the
   TR1 supervision period and the TR1-counters.
*/
typedef struct CMD_TimeSet CMD_TimeSet_t;

/** Message ID for ACK_TimeSet */
#define ACK_TIMESET 0x6143

/**
   Acknowledgement for message CMD_TimeSet.
*/
typedef struct ACK_TimeSet ACK_TimeSet_t;

/** Message ID for CMD_TimeGet */
#define CMD_TIMEGET 0x6103

/**
   Requests the status of the "Set Time and Read Time" EOC messages.
*/
typedef struct CMD_TimeGet CMD_TimeGet_t;

/** Message ID for ACK_TimeGet */
#define ACK_TIMEGET 0x6103

/**
   Provides the status of the "Set Time and Read Time" EOC messages.
*/
typedef struct ACK_TimeGet ACK_TimeGet_t;

/**
   Controls the behaviour of the performance counters.
*/
struct CMD_CounterControlSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 13;
   /** Counter Inhibiting Enable */
   DSL_uint16_t CntInhibitEnable : 1;
   /** Fixed value: VRX_DISABLE */
   DSL_uint16_t Res1 : 2;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Fixed value: VRX_DISABLE */
   DSL_uint16_t Res1 : 2;
   /** Counter Inhibiting Enable */
   DSL_uint16_t CntInhibitEnable : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 13;
#endif
} __PACKED__ ;


/**
   Acknowledgement for message CMD_CounterControlSet.
*/
struct ACK_CounterControlSet
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
   This autonomous message indicates near-end line failures.
*/
struct ALM_LineFailureNE_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 2;
   /** ESE Failure NE */
   DSL_uint16_t ESE_NE : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 9;
   /** LOM Failure NE */
   DSL_uint16_t LOM_NE : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 1;
   /** LOF Failure NE */
   DSL_uint16_t LOF_NE : 1;
   /** LOS Failure NE */
   DSL_uint16_t LOS_NE : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** LOS Failure NE */
   DSL_uint16_t LOS_NE : 1;
   /** LOF Failure NE */
   DSL_uint16_t LOF_NE : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 1;
   /** LOM Failure NE */
   DSL_uint16_t LOM_NE : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 9;
   /** ESE Failure NE */
   DSL_uint16_t ESE_NE : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 2;
#endif
} __PACKED__ ;


/**
   Requests the near-end line failure status.
*/
struct CMD_LineFailureNE_Get
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
   Provides the near-end line failure status requested by the command
   CMD_LineFailureNE_Get.
*/
struct ACK_LineFailureNE_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 2;
   /** ESE Failure NE */
   DSL_uint16_t ESE_NE : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 9;
   /** LOM Failure NE */
   DSL_uint16_t LOM_NE : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 1;
   /** LOF Failure NE */
   DSL_uint16_t LOF_NE : 1;
   /** LOS Failure NE */
   DSL_uint16_t LOS_NE : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** LOS Failure NE */
   DSL_uint16_t LOS_NE : 1;
   /** LOF Failure NE */
   DSL_uint16_t LOF_NE : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 1;
   /** LOM Failure NE */
   DSL_uint16_t LOM_NE : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 9;
   /** ESE Failure NE */
   DSL_uint16_t ESE_NE : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 2;
#endif
} __PACKED__ ;


/**
   This autonomous message indicates far-end line failures.
*/
struct ALM_LineFailureFE_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 2;
   /** ESE Failure FE */
   DSL_uint16_t ESE_FE : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 9;
   /** LOM Failure FE */
   DSL_uint16_t LOM_FE : 1;
   /** LPR Failure FE */
   DSL_uint16_t LPR_FE : 1;
   /** LOF Failure FE */
   DSL_uint16_t LOF_FE : 1;
   /** LOS Failure FE */
   DSL_uint16_t LOS_FE : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** LOS Failure FE */
   DSL_uint16_t LOS_FE : 1;
   /** LOF Failure FE */
   DSL_uint16_t LOF_FE : 1;
   /** LPR Failure FE */
   DSL_uint16_t LPR_FE : 1;
   /** LOM Failure FE */
   DSL_uint16_t LOM_FE : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 9;
   /** ESE Failure FE */
   DSL_uint16_t ESE_FE : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 2;
#endif
} __PACKED__ ;


/**
   Requests the far-end line failure status.
*/
struct CMD_LineFailureFE_Get
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
   Provides the far-end line failure status requested by the command
   CMD_LineFailureFE_Get.
*/
struct ACK_LineFailureFE_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 2;
   /** ESE Failure FE */
   DSL_uint16_t ESE_FE : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 9;
   /** LOM Failure FE */
   DSL_uint16_t LOM_FE : 1;
   /** LPR Failure FE */
   DSL_uint16_t LPR_FE : 1;
   /** LOF Failure FE */
   DSL_uint16_t LOF_FE : 1;
   /** LOS Failure FE */
   DSL_uint16_t LOS_FE : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** LOS Failure FE */
   DSL_uint16_t LOS_FE : 1;
   /** LOF Failure FE */
   DSL_uint16_t LOF_FE : 1;
   /** LPR Failure FE */
   DSL_uint16_t LPR_FE : 1;
   /** LOM Failure FE */
   DSL_uint16_t LOM_FE : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 9;
   /** ESE Failure FE */
   DSL_uint16_t ESE_FE : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 2;
#endif
} __PACKED__ ;


/**
   Host initiated "loss of power". Triggers indicating an lpr primitive to the
   remote side via the indicator bits ("IB bits").The host writes the message to
   the FW e.g. after detecting LPR by an alternative detection scheme.
*/
struct CMD_Failure_Trigger
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 15;
   /** LPR Trigger */
   DSL_uint16_t lpr_Trigger : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** LPR Trigger */
   DSL_uint16_t lpr_Trigger : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 15;
#endif
} __PACKED__ ;


/**
   Acknowledgement for CMD_Failure_Trigger.
*/
struct ACK_Failure_Trigger
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
   Requests near-end line performance counter values (Section 7.2.1.1 of
   G.997.1). These are wrap-around counters which are not affected by the
   TR1-period.
*/
struct CMD_LinePerfCountNE_Get
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
   Provides near-end line performance counter values requested by
   CMD_LinePerfCountNE_Get (Section 7.2.1.1 of G.997.1). These are wrap-around
   counters which are not affected by the TR1-period.
*/
struct ACK_LinePerfCountNE_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Near-End FECS Count, LSW */
   DSL_uint16_t  cntFECS_LSW;
   /** Near-End FECS Count, MSW */
   DSL_uint16_t  cntFECS_MSW;
   /** Near-End ES Count, LSW */
   DSL_uint16_t  cntES_LSW;
   /** Near-End ES Count, MSW */
   DSL_uint16_t  cntES_MSW;
   /** Near-End SES Count, LSW */
   DSL_uint16_t  cntSES_LSW;
   /** Near-End SES Count, MSW */
   DSL_uint16_t  cntSES_MSW;
   /** Near-End LOSS Count, LSW */
   DSL_uint16_t  cntLOSS_LSW;
   /** Near-End LOSS Count, MSW */
   DSL_uint16_t  cntLOSS_MSW;
   /** Near-End UAS Count, LSW */
   DSL_uint16_t  cntUAS_LSW;
   /** Near-End UAS Count, MSW */
   DSL_uint16_t  cntUAS_MSW;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Near-End FECS Count, LSW */
   DSL_uint16_t  cntFECS_LSW;
   /** Near-End FECS Count, MSW */
   DSL_uint16_t  cntFECS_MSW;
   /** Near-End ES Count, LSW */
   DSL_uint16_t  cntES_LSW;
   /** Near-End ES Count, MSW */
   DSL_uint16_t  cntES_MSW;
   /** Near-End SES Count, LSW */
   DSL_uint16_t  cntSES_LSW;
   /** Near-End SES Count, MSW */
   DSL_uint16_t  cntSES_MSW;
   /** Near-End LOSS Count, LSW */
   DSL_uint16_t  cntLOSS_LSW;
   /** Near-End LOSS Count, MSW */
   DSL_uint16_t  cntLOSS_MSW;
   /** Near-End UAS Count, LSW */
   DSL_uint16_t  cntUAS_LSW;
   /** Near-End UAS Count, MSW */
   DSL_uint16_t  cntUAS_MSW;
#endif
} __PACKED__ ;


/**
   Requests far-end line performance counter values (Section 7.2.1.2 of
   G.997.1). These are wrap-around counters which are not affected by the
   TR1-period.
*/
struct CMD_LinePerfCountFE_Get
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
   Provides far-end line performance counter values requested by
   CMD_LinePerfCountFE_Get (Section 7.2.1.2 of G.997.1). These are wrap-around
   counters which are not affected by the TR1-period.
*/
struct ACK_LinePerfCountFE_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Far-End FECS Count, LSW */
   DSL_uint16_t  cntFECS_LSW;
   /** Far-End FECS Count, MSW */
   DSL_uint16_t  cntFECS_MSW;
   /** Far-End ES Count, LSW */
   DSL_uint16_t  cntES_LSW;
   /** Far-End ES Count, MSW */
   DSL_uint16_t  cntES_MSW;
   /** Far-End SES Count, LSW */
   DSL_uint16_t  cntSES_LSW;
   /** Far-End SES Count, MSW */
   DSL_uint16_t  cntSES_MSW;
   /** Far-End LOSS Count, LSW */
   DSL_uint16_t  cntLOSS_LSW;
   /** Far-End LOSS Count, MSW */
   DSL_uint16_t  cntLOSS_MSW;
   /** Near-End UAS Count, LSW */
   DSL_uint16_t  cntUAS_LSW;
   /** Near-End UAS Count, MSW */
   DSL_uint16_t  cntUAS_MSW;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Far-End FECS Count, LSW */
   DSL_uint16_t  cntFECS_LSW;
   /** Far-End FECS Count, MSW */
   DSL_uint16_t  cntFECS_MSW;
   /** Far-End ES Count, LSW */
   DSL_uint16_t  cntES_LSW;
   /** Far-End ES Count, MSW */
   DSL_uint16_t  cntES_MSW;
   /** Far-End SES Count, LSW */
   DSL_uint16_t  cntSES_LSW;
   /** Far-End SES Count, MSW */
   DSL_uint16_t  cntSES_MSW;
   /** Far-End LOSS Count, LSW */
   DSL_uint16_t  cntLOSS_LSW;
   /** Far-End LOSS Count, MSW */
   DSL_uint16_t  cntLOSS_MSW;
   /** Near-End UAS Count, LSW */
   DSL_uint16_t  cntUAS_LSW;
   /** Near-End UAS Count, MSW */
   DSL_uint16_t  cntUAS_MSW;
#endif
} __PACKED__ ;


/**
   Requests near-end CRC counter values. These are wrap-around counters which
   are not affected by the TR1-period.
*/
struct CMD_CRC_StatsNE_Get
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
   Returns near-end CRC counter values. (Section 7.2.2.1.1 of G997.1). These are
   wrap-around counters which are not affected by the TR1-period.The "LP1/Fast
   Path" parameters are fed only in ADSL1 mode with selected fast path.
*/
struct ACK_CRC_StatsNE_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Near-End CRC Count LP0, LSW */
   DSL_uint16_t cntCVI_LSW;
   /** Near-End CRC Count LP0, MSW */
   DSL_uint16_t cntCVI_MSW;
   /** Near-End CRC Count LP1, LSW */
   DSL_uint16_t cntCVF_LSW;
   /** Near-End CRC Count LP1, MSW */
   DSL_uint16_t cntCVF_MSW;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Near-End CRC Count LP0, LSW */
   DSL_uint16_t cntCVI_LSW;
   /** Near-End CRC Count LP0, MSW */
   DSL_uint16_t cntCVI_MSW;
   /** Near-End CRC Count LP1, LSW */
   DSL_uint16_t cntCVF_LSW;
   /** Near-End CRC Count LP1, MSW */
   DSL_uint16_t cntCVF_MSW;
#endif
} __PACKED__ ;


/**
   Requests far-end CRC counter values. These are wrap-around counters which are
   not affected by the TR1-period.
*/
struct CMD_CRC_StatsFE_Get
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
   Returns far-end CRC counter values. (Section 7.2.2.2.1 of G997.1). These are
   wrap-around counters which are not affected by the TR1-period.The "LP1/Fast
   Path" parameters are fed only in ADSL1 mode with selected fast path.
*/
struct ACK_CRC_StatsFE_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Far-End CRC Count LP0, LSW */
   DSL_uint16_t cntCVI_LSW;
   /** Far-End CRC Count LP0, MSW */
   DSL_uint16_t cntCVI_MSW;
   /** Far-End CRC Count LP1, LSW */
   DSL_uint16_t cntCVF_LSW;
   /** Far-End CRC Count LP1, MSW */
   DSL_uint16_t cntCVF_MSW;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Far-End CRC Count LP0, LSW */
   DSL_uint16_t cntCVI_LSW;
   /** Far-End CRC Count LP0, MSW */
   DSL_uint16_t cntCVI_MSW;
   /** Far-End CRC Count LP1, LSW */
   DSL_uint16_t cntCVF_LSW;
   /** Far-End CRC Count LP1, MSW */
   DSL_uint16_t cntCVF_MSW;
#endif
} __PACKED__ ;


/**
   Requests near-end FEC counter values. These are wrap-around counters which
   are not affected by the TR1-period.
*/
struct CMD_FEC_StatsNE_Get
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
   Returns near-end FEC counter values (Section 7.2.2.1.2 of G997.1). These are
   wrap-around counters which are not affected by the TR1-period.The "LP1/Fast
   Path" parameters are fed only in ADSL1 mode with selected fast path.
*/
struct ACK_FEC_StatsNE_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Near-End FEC Count LP0, LSW */
   DSL_uint16_t cntECI_LSW;
   /** Near-End FEC Count LP0, MSW */
   DSL_uint16_t cntECI_MSW;
   /** Near-End FEC Count LP1, LSW */
   DSL_uint16_t cntFEC_LSW;
   /** Near-End FEC Count LP1, MSW */
   DSL_uint16_t cntFEC_MSW;
   /** Near-End FEC Uncorrectable Count LP0, LSW */
   DSL_uint16_t cntECI_NoCorrLSW;
   /** Near-End FEC Uncorrectable Count LP0, MSW */
   DSL_uint16_t cntECI_NoCorrMSW;
   /** Near-End FEC Uncorrectable Count LP1, LSW */
   DSL_uint16_t cntECF_NoCorrLSW;
   /** Near-End FEC Uncorrectable Count LP1, MSW */
   DSL_uint16_t cntECF_NoCorrMSW;
   /** Near-End Total RS CW Count LP0, LSW */
   DSL_uint16_t cntECI_CW_LSW;
   /** Near-End Total RS CW Count LP0, MSW */
   DSL_uint16_t cntECI_CW_MSW;
   /** Near-End Total RS CW Count LP1, LSW */
   DSL_uint16_t cntECF_CW_LSW;
   /** Near-End Total RS CW Count LP1, MSW */
   DSL_uint16_t cntECF_CW_MSW;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Near-End FEC Count LP0, LSW */
   DSL_uint16_t cntECI_LSW;
   /** Near-End FEC Count LP0, MSW */
   DSL_uint16_t cntECI_MSW;
   /** Near-End FEC Count LP1, LSW */
   DSL_uint16_t cntFEC_LSW;
   /** Near-End FEC Count LP1, MSW */
   DSL_uint16_t cntFEC_MSW;
   /** Near-End FEC Uncorrectable Count LP0, LSW */
   DSL_uint16_t cntECI_NoCorrLSW;
   /** Near-End FEC Uncorrectable Count LP0, MSW */
   DSL_uint16_t cntECI_NoCorrMSW;
   /** Near-End FEC Uncorrectable Count LP1, LSW */
   DSL_uint16_t cntECF_NoCorrLSW;
   /** Near-End FEC Uncorrectable Count LP1, MSW */
   DSL_uint16_t cntECF_NoCorrMSW;
   /** Near-End Total RS CW Count LP0, LSW */
   DSL_uint16_t cntECI_CW_LSW;
   /** Near-End Total RS CW Count LP0, MSW */
   DSL_uint16_t cntECI_CW_MSW;
   /** Near-End Total RS CW Count LP1, LSW */
   DSL_uint16_t cntECF_CW_LSW;
   /** Near-End Total RS CW Count LP1, MSW */
   DSL_uint16_t cntECF_CW_MSW;
#endif
} __PACKED__ ;


/**
   Requests far-end FEC counter values. These are wrap-around counters which are
   not affected by the TR1-period.
*/
struct CMD_FEC_StatsFE_Get
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
   Returns far-end FEC counter values (Section 7.2.2.2.2 of G997.1). These are
   wrap-around counters which are not affected by the TR1-period.The "LP1/Fast
   Path" parameters are fed only in ADSL1 mode with selected fast path.
*/
struct ACK_FEC_StatsFE_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Far-End FEC Count LP0, LSW */
   DSL_uint16_t cntECI_LSW;
   /** Far-End FEC Count LP0, MSW */
   DSL_uint16_t cntECI_MSW;
   /** Far-End FEC Count LP1, LSW */
   DSL_uint16_t cntFEC_LSW;
   /** Far-End FEC Count LP1, MSW */
   DSL_uint16_t cntFEC_MSW;
   /** Reserved */
   DSL_uint16_t Res0[4];
   /** Far-End Total RS CW Count LP0, LSW */
   DSL_uint16_t cntECI_CW_LSW;
   /** Far-End Total RS CW Count LP0, MSW */
   DSL_uint16_t cntECI_CW_MSW;
   /** Far-End Total RS CW Count LP1, LSW */
   DSL_uint16_t cntECF_CW_LSW;
   /** Far-End Total RS CW Count LP1, MSW */
   DSL_uint16_t cntECF_CW_MSW;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Far-End FEC Count LP0, LSW */
   DSL_uint16_t cntECI_LSW;
   /** Far-End FEC Count LP0, MSW */
   DSL_uint16_t cntECI_MSW;
   /** Far-End FEC Count LP1, LSW */
   DSL_uint16_t cntFEC_LSW;
   /** Far-End FEC Count LP1, MSW */
   DSL_uint16_t cntFEC_MSW;
   /** Reserved */
   DSL_uint16_t Res0[4];
   /** Far-End Total RS CW Count LP0, LSW */
   DSL_uint16_t cntECI_CW_LSW;
   /** Far-End Total RS CW Count LP0, MSW */
   DSL_uint16_t cntECI_CW_MSW;
   /** Far-End Total RS CW Count LP1, LSW */
   DSL_uint16_t cntECF_CW_LSW;
   /** Far-End Total RS CW Count LP1, MSW */
   DSL_uint16_t cntECF_CW_MSW;
#endif
} __PACKED__ ;


/**
   Requests the near-end 15 minutes SOS counter values.
*/
struct CMD_SOS_StatsNE_Get
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
   Returns near-end SOS counter values. (Section 7.2.1.6.1 of G997.1)
*/
struct ACK_SOS_StatsNE_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Near-End SOS Success Count LP0, LSW */
   DSL_uint16_t cntSOS_SUC_LSW;
   /** Near-End SOS Success Count LP0, MSW */
   DSL_uint16_t cntSOS_SUC_MSW;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Near-End SOS Success Count LP0, LSW */
   DSL_uint16_t cntSOS_SUC_LSW;
   /** Near-End SOS Success Count LP0, MSW */
   DSL_uint16_t cntSOS_SUC_MSW;
#endif
} __PACKED__ ;


/**
   Requests the far-end 15 minutes SOS counter values. These are wrap-around
   counters which are not affected by the TR1-period.
*/
struct CMD_SOS_StatsFE_Get
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
   Returns far-end SOS counter values. (Section 7.2.1.7.1 of G997.1)
*/
struct ACK_SOS_StatsFE_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Far-End SOS Success Count LP0, LSW */
   DSL_uint16_t cntSOS_SUC_LSW;
   /** Far-End SOS Success Count LP0, MSW */
   DSL_uint16_t cntSOS_SUC_MSW;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Far-End SOS Success Count LP0, LSW */
   DSL_uint16_t cntSOS_SUC_LSW;
   /** Far-End SOS Success Count LP0, MSW */
   DSL_uint16_t cntSOS_SUC_MSW;
#endif
} __PACKED__ ;


/**
   Writes near-end line performance counter values (total wrap-around)(Section
   7.2.1.1 of G.997.1).
*/
struct CMD_LinePerfCountNE_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Near-End FECS Count, LSW */
   DSL_uint16_t  cntFECS_LSW;
   /** Near-End FECS Count, MSW */
   DSL_uint16_t  cntFECS_MSW;
   /** Near-End ES Count, LSW */
   DSL_uint16_t  cntES_LSW;
   /** Near-End ES Count, MSW */
   DSL_uint16_t  cntES_MSW;
   /** Near-End SES Count, LSW */
   DSL_uint16_t  cntSES_LSW;
   /** Near-End SES Count, MSW */
   DSL_uint16_t  cntSES_MSW;
   /** Near-End LOSS Count, LSW */
   DSL_uint16_t  cntLOSS_LSW;
   /** Near-End LOSS Count, MSW */
   DSL_uint16_t  cntLOSS_MSW;
   /** Near-End UAS Count, LSW */
   DSL_uint16_t  cntUAS_LSW;
   /** Near-End UAS Count, MSW */
   DSL_uint16_t  cntUAS_MSW;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Near-End FECS Count, LSW */
   DSL_uint16_t  cntFECS_LSW;
   /** Near-End FECS Count, MSW */
   DSL_uint16_t  cntFECS_MSW;
   /** Near-End ES Count, LSW */
   DSL_uint16_t  cntES_LSW;
   /** Near-End ES Count, MSW */
   DSL_uint16_t  cntES_MSW;
   /** Near-End SES Count, LSW */
   DSL_uint16_t  cntSES_LSW;
   /** Near-End SES Count, MSW */
   DSL_uint16_t  cntSES_MSW;
   /** Near-End LOSS Count, LSW */
   DSL_uint16_t  cntLOSS_LSW;
   /** Near-End LOSS Count, MSW */
   DSL_uint16_t  cntLOSS_MSW;
   /** Near-End UAS Count, LSW */
   DSL_uint16_t  cntUAS_LSW;
   /** Near-End UAS Count, MSW */
   DSL_uint16_t  cntUAS_MSW;
#endif
} __PACKED__ ;


/**
   Acknowledgement for CMD_LinePerfCountNE_Set.
*/
struct ACK_LinePerfCountNE_Set
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
   Writes the near-end CRC counter values (Section 7.2.2.1.1 of G997.1)The
   "LP1/Fast Path" parameters are used only in ADSL1 mode with selected "Fast
   Path".
*/
struct CMD_CRC_StatsNE_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Near-End CRC Count LP0, LSW */
   DSL_uint16_t cntCVI_LSW;
   /** Near-End CRC Count LP0, MSW */
   DSL_uint16_t cntCVI_MSW;
   /** Near-End CRC Count LP1, LSW */
   DSL_uint16_t cntCVF_LSW;
   /** Near-End CRC Count LP1, MSW */
   DSL_uint16_t cntCVF_MSW;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Near-End CRC Count LP0, LSW */
   DSL_uint16_t cntCVI_LSW;
   /** Near-End CRC Count LP0, MSW */
   DSL_uint16_t cntCVI_MSW;
   /** Near-End CRC Count LP1, LSW */
   DSL_uint16_t cntCVF_LSW;
   /** Near-End CRC Count LP1, MSW */
   DSL_uint16_t cntCVF_MSW;
#endif
} __PACKED__ ;


/**
   Acknowledgement for the message CMD_CRC_StatsNE_Set.
*/
struct ACK_CRC_StatsNE_Set
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
   Writes near-end FEC counter values. (Section 7.2.2.1.2 of G997.1)The
   "LP1/Fast Path" parameters are used only in ADSL1 mode with selected "Fast
   Path".
*/
struct CMD_FEC_StatsNE_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Near-End FEC Count LP0, LSW */
   DSL_uint16_t cntECI_LSW;
   /** Near-End FEC Count LP0, MSW */
   DSL_uint16_t cntECI_MSW;
   /** Near-End FEC Count LP1, LSW */
   DSL_uint16_t cntFEC_LSW;
   /** Near-End FEC Count LP1, MSW */
   DSL_uint16_t cntFEC_MSW;
   /** Near-End FEC Uncorrectable Count LP0, LSW */
   DSL_uint16_t cntECI_NoCorrLSW;
   /** Near-End FEC Uncorrectable Count LP0, MSW */
   DSL_uint16_t cntECI_NoCorrMSW;
   /** Near-End FEC Uncorrectable Count LP1, LSW */
   DSL_uint16_t cntECF_NoCorrLSW;
   /** Near-End FEC Uncorrectable Count LP1, MSW */
   DSL_uint16_t cntECF_NoCorrMSW;
   /** Near-End Total RS CW Count LP0, LSW */
   DSL_uint16_t cntECI_CW_LSW;
   /** Near-End Total RS CW Count LP0, MSW */
   DSL_uint16_t cntECI_CW_MSW;
   /** Near-End Total RS CW Count LP1, LSW */
   DSL_uint16_t cntECF_CW_LSW;
   /** Near-End Total RS CW Count LP1, MSW */
   DSL_uint16_t cntECF_CW_MSW;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Near-End FEC Count LP0, LSW */
   DSL_uint16_t cntECI_LSW;
   /** Near-End FEC Count LP0, MSW */
   DSL_uint16_t cntECI_MSW;
   /** Near-End FEC Count LP1, LSW */
   DSL_uint16_t cntFEC_LSW;
   /** Near-End FEC Count LP1, MSW */
   DSL_uint16_t cntFEC_MSW;
   /** Near-End FEC Uncorrectable Count LP0, LSW */
   DSL_uint16_t cntECI_NoCorrLSW;
   /** Near-End FEC Uncorrectable Count LP0, MSW */
   DSL_uint16_t cntECI_NoCorrMSW;
   /** Near-End FEC Uncorrectable Count LP1, LSW */
   DSL_uint16_t cntECF_NoCorrLSW;
   /** Near-End FEC Uncorrectable Count LP1, MSW */
   DSL_uint16_t cntECF_NoCorrMSW;
   /** Near-End Total RS CW Count LP0, LSW */
   DSL_uint16_t cntECI_CW_LSW;
   /** Near-End Total RS CW Count LP0, MSW */
   DSL_uint16_t cntECI_CW_MSW;
   /** Near-End Total RS CW Count LP1, LSW */
   DSL_uint16_t cntECF_CW_LSW;
   /** Near-End Total RS CW Count LP1, MSW */
   DSL_uint16_t cntECF_CW_MSW;
#endif
} __PACKED__ ;


/**
   Acknowledgement for the message CMD_FEC_StatsNE_Set.
*/
struct ACK_FEC_StatsNE_Set
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
   Writes near-end SOS counter values. ("SOS-SUCCESS-NE", Section 7.2.1.6.1 of
   G997.1).
*/
struct  CMD_SOS_StatsNE_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Near-End SOS Success Count LP0, LSW */
   DSL_uint16_t cntSOS_SUC_LSW;
   /** Near-End SOS Success Count LP0, MSW */
   DSL_uint16_t cntSOS_SUC_MSW;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Near-End SOS Success Count LP0, LSW */
   DSL_uint16_t cntSOS_SUC_LSW;
   /** Near-End SOS Success Count LP0, MSW */
   DSL_uint16_t cntSOS_SUC_MSW;
#endif
} __PACKED__ ;


/**
   Acknowledgement for CMD_SOS_StatsNE_Set.
*/
struct  ACK_SOS_StatsNE_Set
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
   Sets the running time since midnightModifying the Time does not affect the
   TR1 supervision period and the TR1-counters.
*/
struct CMD_TimeSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0[3];
   /** NE Time */
   VRX_TIME_t timeNE;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0[3];
   /** NE Time */
   VRX_TIME_t timeNE;
#endif
} __PACKED__ ;


/**
   Acknowledgement for message CMD_TimeSet.
*/
struct ACK_TimeSet
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
   Requests the status of the "Set Time and Read Time" EOC messages.
*/
struct CMD_TimeGet
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
   Provides the status of the "Set Time and Read Time" EOC messages.
*/
struct ACK_TimeGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0;
   /** FE Time */
   VRX_TIME_t timeFE;
   /** NE Time */
   VRX_TIME_t timeNE;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0;
   /** FE Time */
   VRX_TIME_t timeFE;
   /** NE Time */
   VRX_TIME_t timeNE;
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

#endif /** _DRV_DSL_CPE_VRX_MSG_PERF_PMD_H_*/
