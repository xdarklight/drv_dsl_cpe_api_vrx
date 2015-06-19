/******************************************************************************

                              Copyright (c) 2014
                            Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/


#ifndef _DRV_DSL_CPE_VRX_MSG_CONST_H_
#define _DRV_DSL_CPE_VRX_MSG_CONST_H_

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


/** @defgroup _CONST_
 *  @{
 */

#ifdef __cplusplus
   extern "C" {
#endif

/* ----- Common Constants Definition section ----- */
/** Disable a functionality or state */
#define VRX_DISABLE 0
/** Enable a functionality or state */
#define VRX_ENABLE 1
/** Disable a functionality or state */
#define VRX_DISABLE_N 1
/** Enable a functionality or state */
#define VRX_ENABLE_N 0
/** Logical value TRUE */
#define VRX_TRUE 1
/** Logical value FALSE */
#define VRX_FALSE 0
/** Functionality is supported */
#define SUPPORTED 1
/** Functionality is not supported */
#define NOT_SUPPORTED 0
/** Counter value is below threshold */
#define CNTR_OK 0
/** Counter threshold met or exceeded */
#define CNTR_THRESH_HIT 1
/** Fixed Value of 0; Used for numbering parameters (like Array Index number or Length number) */
#define FV0 0
/** Fixed Value of 1; Used for numbering parameters (like Array Index number or Length number) */
#define FV1 1
/** Fixed Value of 2; Used for numbering parameters (like Array Index number or Length number) */
#define FV2 2
/** Fixed Value of 3; Used for numbering parameters (like Array Index number or Length number) */
#define FV3 3
/** Fixed Value of 4; Used for numbering parameters (like Array Index number or Length number) */
#define FV4 4
/** Fixed Value of 5; Used for numbering parameters (like Array Index number or Length number) */
#define FV5 5
/** Fixed Value of 6; Used for numbering parameters (like Array Index number or Length number) */
#define FV6 6
/** Fixed Value of 7; Used for numbering parameters (like Array Index number or Length number) */
#define FV7 7
/** Fixed Value of 8; Used for numbering parameters (like Array Index number or Length number) */
#define FV8 8
/** Fixed Value of 9; Used for numbering parameters (like Array Index number or Length number) */
#define FV9 9
/** Fixed Value of 10; Used for numbering parameters (like Array Index number or Length number) */
#define FV10 10
/** Fixed Value of 11; Used for numbering parameters (like Array Index number or Length number) */
#define FV11 11
/** Fixed Value of 12; Used for numbering parameters (like Array Index number or Length number) */
#define FV12 12
/** Fixed Value of 13; Used for numbering parameters (like Array Index number or Length number) */
#define FV13 13
/** Fixed Value of 15; Used for numbering parameters (like Array Index number or Length number) */
#define FV15 15
/** Fixed Value of 16; Used for numbering parameters (like Array Index number or Length number) */
#define FV16 16
/** Fixed Value of 17; Used for numbering parameters (like Array Index number or Length number) */
#define FV17 17
/** Fixed Value of 20; Used for numbering parameters (like Array Index number or Length number) */
#define FV20 20
/** Fixed Value of 32; Used for numbering parameters (like Array Index number or Length number) */
#define FV32 32
/** Fixed Value of 127; Used for numbering parameters (like Array Index number or Length number) */
#define FV127 127
/** Fixed Value of 128; Used for numbering parameters (like Array Index number or Length number) */
#define FV128 128
/** Fixed Value of 256; Used for numbering parameters (like Array Index number or Length number) */
#define FV256 256
/** Link 0 is selected */
#define LINK0 0x0
/** Link 1 is selected */
#define LINK1 0x1
/** No Failure */
#define VRX_OK 0x0
/** Failure */
#define VRX_FAIL 0x1
/** EOC Failure */
#define VRX_EOCFAIL 0x2
/** Request was rejected */
#define VRX_REJECTED 0x1
/* ----- Common Constants Definition section (End) ----- */
/* ----- Message Specific Constants Definition section ----- */
/* ----- Message Specific Constants Definition section (End) ----- */
/**

*/
typedef struct VRX_BAT_TableEntry VRX_BAT_TableEntry_t;

/**

*/
typedef struct VRX_ToneIndex VRX_ToneIndex_t;

/**

*/
typedef struct VRX_PSDbreak VRX_PSDbreak_t;

/**

*/
typedef struct VRX_PSD_TxDS VRX_PSD_TxDS_t;

/**

*/
typedef struct VRX_RxRef2PSD VRX_RxRef2PSD_t;

/**

*/
typedef struct VRX_LineStatus_pb VRX_LineStatus_pb_t;

/**

*/
typedef struct VRX_TSSIbreak VRX_TSSIbreak_t;

/**

*/
typedef struct VRX_TSSI VRX_TSSI_t;

/**

*/
typedef struct VRX_QLN_NE VRX_QLN_NE_t;

/**

*/
typedef struct VRX_TestParam VRX_TestParam_t;

/**

*/
typedef struct VRX_SNR VRX_SNR_t;

/**

*/
typedef struct VRX_HLIN VRX_HLIN_t;

/**

*/
typedef struct VRX_ClearEOC VRX_ClearEOC_t;

/**

*/
typedef struct VRX_UER VRX_UER_t;

/**

*/
typedef struct VRX_TIME VRX_TIME_t;

/**

*/
typedef struct VRX_ATMcell VRX_ATMcell_t;

/**

*/
struct VRX_BAT_TableEntry
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Bit Allocation */
   DSL_uint8_t data_01;
   /** Bit Allocation */
   DSL_uint8_t data_00;
#else
   /** Bit Allocation */
   DSL_uint8_t data_00;
   /** Bit Allocation */
   DSL_uint8_t data_01;
#endif
} __PACKED__ ;


/**

*/
struct VRX_ToneIndex
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** First Tone Index */
   DSL_uint16_t first;
   /** Last Tone Index */
   DSL_uint16_t last;
#else
   /** First Tone Index */
   DSL_uint16_t first;
   /** Last Tone Index */
   DSL_uint16_t last;
#endif
} __PACKED__ ;


/**

*/
struct VRX_PSDbreak
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Tone Index of Breakpoint */
   DSL_uint16_t toneIndex;
   /** PSD Level of Breakpoint */
   DSL_uint16_t level;
   /** Reserved */
   DSL_uint16_t Res0;
#else
   /** Tone Index of Breakpoint */
   DSL_uint16_t toneIndex;
   /** PSD Level of Breakpoint */
   DSL_uint16_t level;
   /** Reserved */
   DSL_uint16_t Res0;
#endif
} __PACKED__ ;


/**

*/
struct VRX_PSD_TxDS
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Reserved */
   DSL_uint8_t Res0;
   /** Number of Breakpoints */
   DSL_uint8_t NumBreakPts;
   /** Breakpoint Descriptor TxPSDds */
   VRX_PSDbreak_t breakpoint[48];
#else
   /** Number of Breakpoints */
   DSL_uint8_t NumBreakPts;
   /** Reserved */
   DSL_uint8_t Res0;
   /** Breakpoint Descriptor TxPSDds */
   VRX_PSDbreak_t breakpoint[48];
#endif
} __PACKED__ ;


/**

*/
struct VRX_RxRef2PSD
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** UPBO RxRefPSD Parameter "a" */
   DSL_int16_t a;
   /** UPBO RxRefPSD Parameter "b" */
   DSL_int16_t b;
#else
   /** UPBO RxRefPSD Parameter "a" */
   DSL_int16_t a;
   /** UPBO RxRefPSD Parameter "b" */
   DSL_int16_t b;
#endif
} __PACKED__ ;


/**

*/
struct VRX_LineStatus_pb
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** LATN per Band */
   DSL_uint16_t LATNpb;
   /** SATN per Band */
   DSL_uint16_t SATNpb;
   /** SNRM per Band */
   DSL_uint16_t SNRMpb;
#else
   /** LATN per Band */
   DSL_uint16_t LATNpb;
   /** SATN per Band */
   DSL_uint16_t SATNpb;
   /** SNRM per Band */
   DSL_uint16_t SNRMpb;
#endif
} __PACKED__ ;


/**

*/
struct VRX_TSSIbreak
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Supported Set Status */
   DSL_uint16_t supset : 1;
   /** Tone Index of Breakpoint */
   DSL_uint16_t toneIndex : 15;
   /** Transmit Spectrum Shaping Value of Breakpoint */
   DSL_uint16_t level;
#else
   /** Tone Index of Breakpoint */
   DSL_uint16_t toneIndex : 15;
   /** Supported Set Status */
   DSL_uint16_t supset : 1;
   /** Transmit Spectrum Shaping Value of Breakpoint */
   DSL_uint16_t level;
#endif
} __PACKED__ ;


/**

*/
struct VRX_TSSI
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Number of breakpoints */
   DSL_uint16_t Num;
   /** TSSI breakpoints */
   VRX_TSSIbreak_t TSSIbrkp[64];
#else
   /** Number of breakpoints */
   DSL_uint16_t Num;
   /** TSSI breakpoints */
   VRX_TSSIbreak_t TSSIbrkp[64];
#endif
} __PACKED__ ;


/**

*/
struct VRX_QLN_NE
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** QLN per Subcarrier Group */
   DSL_uint8_t qln_01;
   /** QLN per Subcarrier Group */
   DSL_uint8_t qln_00;
#else
   /** QLN per Subcarrier Group */
   DSL_uint8_t qln_00;
   /** QLN per Subcarrier Group */
   DSL_uint8_t qln_01;
#endif
} __PACKED__ ;


/**

*/
struct VRX_TestParam
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** HLOG per Subcarrier Group */
   DSL_uint16_t hlog;
   /** SNR per Subcarrier Group */
   DSL_uint8_t snr;
   /** QLN per Subcarrier Group */
   DSL_uint8_t qln;
#else
   /** HLOG per Subcarrier Group */
   DSL_uint16_t hlog;
   /** QLN per Subcarrier Group */
   DSL_uint8_t qln;
   /** SNR per Subcarrier Group */
   DSL_uint8_t snr;
#endif
} __PACKED__ ;


/**

*/
struct VRX_SNR
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** SNR per Subcarrier Group */
   DSL_uint8_t snr_01;
   /** SNR per Subcarrier Group */
   DSL_uint8_t snr_00;
#else
   /** SNR per Subcarrier Group */
   DSL_uint8_t snr_00;
   /** SNR per Subcarrier Group */
   DSL_uint8_t snr_01;
#endif
} __PACKED__ ;


/**

*/
struct VRX_HLIN
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** HLIN per Subcarrier Group, Real Part */
   DSL_int16_t hlin_real;
   /** HLIN per Subcarrier Group, Imaginary Part */
   DSL_int16_t hlin_imag;
#else
   /** HLIN per Subcarrier Group, Real Part */
   DSL_int16_t hlin_real;
   /** HLIN per Subcarrier Group, Imaginary Part */
   DSL_int16_t hlin_imag;
#endif
} __PACKED__ ;


/**

*/
struct VRX_ClearEOC
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Reserved  */
   DSL_uint16_t Res0 : 6;
   /** Length of the Clear EOC Message  */
   DSL_uint16_t Length : 10;
   /** Protocol Identifier */
   DSL_uint16_t ProtIdent;
   /** Clear EOC data field */
   DSL_uint8_t Data[508];
#else
   /** Length of the Clear EOC Message  */
   DSL_uint16_t Length : 10;
   /** Reserved  */
   DSL_uint16_t Res0 : 6;
   /** Protocol Identifier */
   DSL_uint16_t ProtIdent;
   /** Clear EOC data field */
   DSL_uint8_t Data[508];
#endif
} __PACKED__ ;


/**

*/
struct VRX_UER
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** UERps Real Part, LSW */
   DSL_uint16_t UER_Real_LSW;
   /** UERps Real Part, MSW */
   DSL_uint16_t UER_Real_MSW;
   /** UERps Imaginary Part, LSW */
   DSL_uint16_t UER_Imag_LSW;
   /** UERps Imaginary Part, MSW */
   DSL_uint16_t UER_Imag_MSW;
#else
   /** UERps Real Part, LSW */
   DSL_uint16_t UER_Real_LSW;
   /** UERps Real Part, MSW */
   DSL_uint16_t UER_Real_MSW;
   /** UERps Imaginary Part, LSW */
   DSL_uint16_t UER_Imag_LSW;
   /** UERps Imaginary Part, MSW */
   DSL_uint16_t UER_Imag_MSW;
#endif
} __PACKED__ ;


/**

*/
struct VRX_TIME
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Hours */
   DSL_uint8_t hours;
   /** Minutes */
   DSL_uint8_t minutes;
   /** Seconds */
   DSL_uint8_t seconds;
   /** Reserved */
   DSL_uint8_t Res0;
#else
   /** Minutes */
   DSL_uint8_t minutes;
   /** Hours */
   DSL_uint8_t hours;
   /** Reserved */
   DSL_uint8_t Res0;
   /** Seconds */
   DSL_uint8_t seconds;
#endif
} __PACKED__ ;


/**

*/
struct VRX_ATMcell
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** ATM cell (53 bytes) */
   DSL_uint8_t ATMcell[53];
   /** Reserved */
   DSL_uint8_t Res0[3];
#else
   /** ATM cell (53 bytes) */
   DSL_uint8_t ATMcell[53];
   /** Reserved */
   DSL_uint8_t Res0[3];
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

#endif /** _DRV_DSL_CPE_VRX_MSG_CONST_H_*/
