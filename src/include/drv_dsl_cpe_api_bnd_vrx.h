/******************************************************************************

                              Copyright (c) 2014
                            Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef _DRV_DSL_CPE_BND_VRX_H
#define _DRV_DSL_CPE_BND_VRX_H

#ifdef __cplusplus
   extern "C" {
#endif

/** \addtogroup DRV_DSL_DEVICE
 @{ */

#include "drv_dsl_cpe_api.h"

#define DSL_VRX_BND_MASTER_PORT_NUM 0
#define DSL_VRX_BND_SLAVE_PORT_NUM  1

/**
   Structure for the PAF HS control
*/
typedef struct
{
   /**
      PAF enable/disable*/
   DSL_uint8_t nPafEnable;
} DSL_BND_VRX_PafHsControl_t;

typedef struct
{
   /**
      Aggregation Discovery Code (Register 6.18)
      Bits 47:32 of the 48-bit aggregation discovery code.
      The PME aggregation discovery code received from the CO with a
      "Set if Clear" or "Clear if Same" command. */
   DSL_uint16_t discoveryCode1;
   /**
      Aggregation Discovery Code (Registers 6.19)
      Bits 31:16 of the 48-bit aggregation discovery code.
      See description at discoveryCode1.*/
   DSL_uint16_t discoveryCode2;
   /**
      Aggregation Discovery Code (Registers 6.20)
      Bits 15:0 of the 48-bit aggregation discovery code.
      See description at discoveryCode1. */
   DSL_uint16_t discoveryCode3;
} DSL_BND_VRX_DiscoveryCodes_t;

/**
   Structure for the PAF HS status
*/
typedef struct
{
   /**
      Aggregation Discovery CLEAR-IF-SAME command received.
      VRX_TRUE/VRX_FALSE*/
   DSL_uint8_t discoveryClearIfSame;
   /**
      Aggregation Discovery SET-IF-CLEAR command received.
      VRX_TRUE/VRX_FALSE*/
   DSL_uint8_t discoverySetIfClear;
   /** PME Aggregate SET command received with PME aggregate data equal
       to zero. VRX_TRUE/VRX_FALSE*/
   DSL_uint8_t aggregClear;
   /** PME Aggregate SET command received with non-zero PME aggregate data.
       VRX_TRUE/VRX_FALSE*/
   DSL_uint8_t aggregSet;
   /**
      Remote PAF supported. VRX_TRUE/VRX_FALSE */
   DSL_boolean_t bPafEnable;
   /**
      Aggregation Discovery Code*/
   DSL_BND_VRX_DiscoveryCodes_t discoveryCode;
   /**
      Link Partner PME Aggregate Data (Registers 6.22, 6.23)
      Data for the link partner aggregate operation.
      The data received from the CO together with a "Set" command. */
   DSL_uint32_t aggregateData;
} DSL_BND_VRX_PafHsStatus_t;

/**
   This function gets PAF Handshake status from the firmware.

   \param pContext - Pointer to dsl cpe library context structure, [I/O]
   \param pData    - Pointer to the PAF status data [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_BND_VRX_PafHsStatusGet(
   DSL_Context_t *pContext,
   DSL_BND_VRX_PafHsStatus_t *pData);

/**
   Structure for the PAF HS continue command
*/
typedef struct
{
   /**
      Aggregation Discovery Code*/
   DSL_BND_VRX_DiscoveryCodes_t discoveryCode;
   /**
      Partner PME Aggregate Data (Registers 6.22, 6.23)
      Actual aggregate data as held in the SW.*/
   DSL_uint32_t aggregateData;
}  DSL_BND_VRX_PafHsContinue_t;

/**
   This function writes Bonding configuration to the firmware.

   \param pContext - Pointer to dsl cpe library context structure, [I/O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_BND_VRX_ConfigWrite(
   DSL_Context_t *pContext);

/**
   XWAY(TM) VRX200 device specific Bonding HW initialization.

   \param pContext - Pointer to dsl cpe library context structure, [I/O]
   \param pData    - Pointer to the Bonding HW initialization data, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_BND_DEV_HwInit(
   DSL_Context_t *pContext,
   DSL_BND_HwInit_t *pData);

/**
   This function gets Bonding device specific handshake status.

   \param pContext - Pointer to dsl cpe library context structure, [I/O]
   \param pData    - Pointer to the Bonding HS Status data, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_BND_DEV_HsStatusGet(
   DSL_Context_t *pContext,
   DSL_BND_HsStatusGet_t *pData);

/**
   This function configures the remote_discovery_register (maintained by the
   network processor and common to all bonded ports)

   \param pContext - Pointer to dsl cpe library context structure, [I/O]
   \param pData    - Pointer to the Bonding HS continue data, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_BND_DEV_HsContinue(
   DSL_Context_t *pContext,
   DSL_BND_HsContinue_t *pData);

/**
   This function retrieves the bonding control register, fragment counts,
   and packet counts.

   \param pContext - Pointer to dsl cpe library context structure, [I/O]
   \param pData    - Pointer to the Bonding Eth Dbg counters data, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_BND_DEV_EthDbgCountersGet(
   DSL_Context_t *pContext,
   DSL_BND_EthDbgCounters_t *pData);

/**
   This function retrieves Bonding Ethernet counters.

   \param pContext - Pointer to dsl cpe library context structure, [I/O]
   \param pData    - Pointer to the Bonding Eth counters data, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_BND_DEV_EthCountersGet(
   DSL_Context_t *pContext,
   DSL_BND_EthCounters_t *pData);

/** @} DRV_DSL_DEVICE */

#ifdef __cplusplus
}
#endif

#endif /** _DRV_DSL_CPE_BND_VRX_H*/
