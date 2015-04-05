/******************************************************************************

                              Copyright (c) 2012
                            Lantiq Deutschland GmbH
                     Am Campeon 3; 85579 Neubiberg, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/


#ifndef _DRV_DSL_CPE_VRX_MSG_DEBUG_H_
#define _DRV_DSL_CPE_VRX_MSG_DEBUG_H_

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


/** @defgroup _DEBUG_
 *  @{
 */

#ifdef __cplusplus
   extern "C" {
#endif

/* ----- Message Specific Constants Definition section ----- */
/* ----- Message Specific Constants Definition section (End) ----- */

/** Message ID for CMD_DBG_MemMapRead */
#define CMD_DBG_MEMMAPREAD 0xA033

/**
   This command requests the content of the address contained as parameter.
*/
typedef struct CMD_DBG_MemMapRead CMD_DBG_MemMapRead_t;

/** Message ID for ACK_DBG_MemMapRead */
#define ACK_DBG_MEMMAPREAD 0xA033

/**
   Acknowledgement carrying the info requested by CMD_DBG_MemMapRead (result
   value of one read access).
*/
typedef struct ACK_DBG_MemMapRead ACK_DBG_MemMapRead_t;

/** Message ID for CMD_DBG_MemMapWrite */
#define CMD_DBG_MEMMAPWRITE 0xA173

/**
   The command writes a configurable value to a single arbitrary address.
*/
typedef struct CMD_DBG_MemMapWrite CMD_DBG_MemMapWrite_t;

/** Message ID for ACK_DBG_MemMapWrite */
#define ACK_DBG_MEMMAPWRITE 0xA173

/**
   Acknowledgement to a CMD_DBG_MemMapWrite command.
*/
typedef struct ACK_DBG_MemMapWrite ACK_DBG_MemMapWrite_t;

/** Message ID for CMD_DBG_AFE_Read */
#define CMD_DBG_AFE_READ 0xA433

/**
   The command reads any AFE register.
*/
typedef struct CMD_DBG_AFE_Read CMD_DBG_AFE_Read_t;

/** Message ID for ACK_DBG_AFE_Read */
#define ACK_DBG_AFE_READ 0xA433

/**
   Delivers the content of the register requested by CMD_DBG_AFE_Read.
*/
typedef struct ACK_DBG_AFE_Read ACK_DBG_AFE_Read_t;

/** Message ID for CMD_DBG_AFE_Write */
#define CMD_DBG_AFE_WRITE 0xA573

/**
   This messages writes a register inside the AFE.
*/
typedef struct CMD_DBG_AFE_Write CMD_DBG_AFE_Write_t;

/** Message ID for ACK_DBG_AFE_Write */
#define ACK_DBG_AFE_WRITE 0xA573

/**
   Acknowledgement for .CMD_DBG_AFE_Write
*/
typedef struct ACK_DBG_AFE_Write ACK_DBG_AFE_Write_t;

/** Message ID for CMD_DBG_WatchdogControl */
#define CMD_DBG_WATCHDOGCONTROL 0xB373

/**
   Switches on/off the HW watchdog by affecting timer T2 of the processor
   subsystem.
*/
typedef struct CMD_DBG_WatchdogControl CMD_DBG_WatchdogControl_t;

/** Message ID for ACK_DBG_WatchdogControl */
#define ACK_DBG_WATCHDOGCONTROL 0xB373

/**
   Acknowledgement to a CMD_DBG_WatchdogControl command.
*/
typedef struct ACK_DBG_WatchdogControl ACK_DBG_WatchdogControl_t;

/** Message ID for CMD_DBG_ErrorGenerationTimeRxSet */
#define CMD_DBG_ERRORGENERATIONTIMERXSET 0xB173

/**
   Generates errors of several error types in receive direction for a
   configurable period of time. The error generation can be enabled/disabled
   individually.
*/
typedef struct CMD_DBG_ErrorGenerationTimeRxSet CMD_DBG_ErrorGenerationTimeRxSet_t;

/** Message ID for ACK_DBG_ErrorGenerationTimeRxSet */
#define ACK_DBG_ERRORGENERATIONTIMERXSET 0xB173

/**
   Generates errors of several error types in receive direction for a
   configurable period of time. The error generation can be enabled/disabled
   individually.
*/
typedef struct ACK_DBG_ErrorGenerationTimeRxSet ACK_DBG_ErrorGenerationTimeRxSet_t;

/**
   This command requests the content of the address contained as parameter.
*/
struct CMD_DBG_MemMapRead
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Address */
   DSL_uint32_t Address;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Address */
   DSL_uint32_t Address;
#endif
} __PACKED__ ;


/**
   Acknowledgement carrying the info requested by CMD_DBG_MemMapRead (result
   value of one read access).
*/
struct ACK_DBG_MemMapRead
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Address */
   DSL_uint32_t Address;
   /** ReadValue */
   DSL_uint32_t ReadValue;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Address */
   DSL_uint32_t Address;
   /** ReadValue */
   DSL_uint32_t ReadValue;
#endif
} __PACKED__ ;


/**
   The command writes a configurable value to a single arbitrary address.
*/
struct CMD_DBG_MemMapWrite
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Address */
   DSL_uint32_t Address;
   /** WriteValue */
   DSL_uint32_t WriteValue;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Address */
   DSL_uint32_t Address;
   /** WriteValue */
   DSL_uint32_t WriteValue;
#endif
} __PACKED__ ;


/**
   Acknowledgement to a CMD_DBG_MemMapWrite command.
*/
struct ACK_DBG_MemMapWrite
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Address */
   DSL_uint32_t Address;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Address */
   DSL_uint32_t Address;
#endif
} __PACKED__ ;


/**
   The command reads any AFE register.
*/
struct CMD_DBG_AFE_Read
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Address */
   DSL_uint32_t Address;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Address */
   DSL_uint32_t Address;
#endif
} __PACKED__ ;


/**
   Delivers the content of the register requested by CMD_DBG_AFE_Read.
*/
struct ACK_DBG_AFE_Read
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Address */
   DSL_uint32_t Address;
   /** ReadValue */
   DSL_uint32_t ReadValue;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Address */
   DSL_uint32_t Address;
   /** ReadValue */
   DSL_uint32_t ReadValue;
#endif
} __PACKED__ ;


/**
   This messages writes a register inside the AFE.
*/
struct CMD_DBG_AFE_Write
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Address */
   DSL_uint32_t Address;
   /** WriteValue */
   DSL_uint32_t WriteValue;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Address */
   DSL_uint32_t Address;
   /** WriteValue */
   DSL_uint32_t WriteValue;
#endif
} __PACKED__ ;


/**
   Acknowledgement for .CMD_DBG_AFE_Write
*/
struct ACK_DBG_AFE_Write
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Address */
   DSL_uint32_t Address;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Address */
   DSL_uint32_t Address;
#endif
} __PACKED__ ;


/**
   Switches on/off the HW watchdog by affecting timer T2 of the processor
   subsystem.
*/
struct CMD_DBG_WatchdogControl
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** HW Watchdog Control */
   DSL_uint32_t Control;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** HW Watchdog Control */
   DSL_uint32_t Control;
#endif
} __PACKED__ ;


/**
   Acknowledgement to a CMD_DBG_WatchdogControl command.
*/
struct ACK_DBG_WatchdogControl
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
   Generates errors of several error types in receive direction for a
   configurable period of time. The error generation can be enabled/disabled
   individually.
*/
struct CMD_DBG_ErrorGenerationTimeRxSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint32_t Res0 : 30;
   /** Enable Bit1 */
   DSL_uint32_t E1 : 1;
   /** Enable Bit0 */
   DSL_uint32_t E0 : 1;
   /** OOS Error Period for PTM, BC0 */
   DSL_uint32_t OOS_TimeBC0;
   /** OOS Error Period for PTM, BC1 */
   DSL_uint32_t OOS_TimeBC1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Enable Bit0 */
   DSL_uint32_t E0 : 1;
   /** Enable Bit1 */
   DSL_uint32_t E1 : 1;
   /** Reserved */
   DSL_uint32_t Res0 : 30;
   /** OOS Error Period for PTM, BC0 */
   DSL_uint32_t OOS_TimeBC0;
   /** OOS Error Period for PTM, BC1 */
   DSL_uint32_t OOS_TimeBC1;
#endif
} __PACKED__ ;


/**
   Generates errors of several error types in receive direction for a
   configurable period of time. The error generation can be enabled/disabled
   individually.
*/
struct ACK_DBG_ErrorGenerationTimeRxSet
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

#endif /** _DRV_DSL_CPE_VRX_MSG_DEBUG_H_*/
