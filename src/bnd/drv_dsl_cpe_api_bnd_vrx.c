/******************************************************************************

                              Copyright (c) 2014
                            Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/
#define DSL_INTERN

#include "drv_dsl_cpe_api.h"

#ifdef __cplusplus
   extern "C" {
#endif

#if defined(INCLUDE_DSL_BONDING) && defined(INCLUDE_DSL_CPE_API_VRX)

#include "drv_dsl_cpe_api_bnd_vrx.h"

#undef DSL_DBG_BLOCK
#define DSL_DBG_BLOCK DSL_DBG_DEVICE

/*
   This function writes PAF Handshake configuration to the firmware.

   \param pContext - Pointer to dsl cpe library context structure, [I/O]
   \param pData    - Pointer to the PAF control data [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
static DSL_Error_t DSL_DRV_BND_VRX_PafHsControlSet(
   DSL_Context_t *pContext,
   DSL_BND_VRX_PafHsControl_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   CMD_PAF_HS_Control_t sCmd;
   ACK_PAF_HS_Control_t sAck;

   DSL_DEBUG(DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_BND_VRX_PafHsControlSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   memset(&sCmd, 0x0, sizeof(sCmd));

   sCmd.Length     = 0x1;
   sCmd.PAF_Enable = pData->nPafEnable ? VRX_ENABLE : VRX_DISABLE;

   nErrCode =  DSL_DRV_VRX_SendMessage( pContext, CMD_PAF_HS_CONTROL,
                   sizeof(CMD_PAF_HS_Control_t), (DSL_uint8_t *)&sCmd,
                   sizeof(ACK_PAF_HS_Control_t), (DSL_uint8_t *)&sAck );

   DSL_DEBUG(DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_BND_VRX_PafHsControlSet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

/*
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
   DSL_BND_VRX_PafHsStatus_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   CMD_PAF_HS_StatusGet_t sCmd;
   ACK_PAF_HS_StatusGet_t sAck;

   DSL_DEBUG(DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_BND_VRX_PafHsStatusGet" DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext)));

#if (DSL_DRV_LINES_PER_DEVICE == 2)
   if (DSL_DRV_DEV_FirmwareFeatureCheck(pContext,
      DSL_FW_XDSLFEATURE_DUALPORT) == DSL_FALSE)
   {
      DSL_DEBUG(DSL_DBG_ERR, (pContext, SYS_DBG_ERR
         "DSL[%02d]: ERROR - Bonding is not supported with the current FW!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERR_NOT_SUPPORTED_BY_FIRMWARE;
   }
#endif /* (DSL_DRV_LINES_PER_DEVICE == 2) */

   memset(&sCmd, 0x00, sizeof(sCmd));

   sCmd.Length = 0x6;

   nErrCode =  DSL_DRV_VRX_SendMessage( pContext, CMD_PAF_HS_STATUSGET,
                   sizeof(CMD_PAF_HS_StatusGet_t), (DSL_uint8_t *)&sCmd,
                   sizeof(ACK_PAF_HS_StatusGet_t), (DSL_uint8_t *)&sAck );

   if (nErrCode >= DSL_SUCCESS)
   {
      pData->discoveryClearIfSame = (DSL_uint8_t)sAck.discoveryClearIfSame;
      pData->discoverySetIfClear  = (DSL_uint8_t)sAck.discoverySetIfClear;
      pData->aggregClear          = (DSL_uint8_t)sAck.aggregClear;
      pData->aggregSet            = (DSL_uint8_t)sAck.aggregSet;
      pData->bPafEnable           = (DSL_boolean_t)sAck.PAF_Enable;
      pData->discoveryCode.discoveryCode1 = sAck.discoveryCode1;
      pData->discoveryCode.discoveryCode2 = sAck.discoveryCode2;
      pData->discoveryCode.discoveryCode3 = sAck.discoveryCode3;
      pData->aggregateData        = (sAck.aggregateData1 << 16) | sAck.aggregateData2;
   }
   else
   {
      memset(pData, 0x00, sizeof(DSL_BND_VRX_PafHsStatus_t));
   }

   DSL_DEBUG(DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_BND_VRX_PafHsStatusGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

/*
   This function triggers PAF Handshake continue in the firmware.

   \param pContext - Pointer to dsl cpe library context structure, [I/O]
   \param pData    - Pointer to the PAF HS continue data [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
static DSL_Error_t DSL_DRV_BND_VRX_PafHsContinue(
   DSL_Context_t *pContext,
   DSL_BND_VRX_PafHsContinue_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   CMD_PAF_HS_Continue_t sCmd;
   ACK_PAF_HS_Continue_t sAck;

   DSL_DEBUG(DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_BND_VRX_PafHsContinue" DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext)));

#if (DSL_DRV_LINES_PER_DEVICE == 2)
   if (DSL_DRV_DEV_FirmwareFeatureCheck(pContext,
      DSL_FW_XDSLFEATURE_DUALPORT) == DSL_FALSE)
   {
      DSL_DEBUG(DSL_DBG_ERR, (pContext, SYS_DBG_ERR
         "DSL[%02d]: ERROR - Bonding is not supported with the current FW!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERR_NOT_SUPPORTED_BY_FIRMWARE;
   }
#endif /* (DSL_DRV_LINES_PER_DEVICE == 2) */

   memset(&sCmd, 0x00, sizeof(sCmd));

   sCmd.Length = 0x6;

   sCmd.trigger = CMD_PAF_HS_Continue_CONTINUE;

   sCmd.discoveryCode1 = pData->discoveryCode.discoveryCode1;
   sCmd.discoveryCode2 = pData->discoveryCode.discoveryCode2;
   sCmd.discoveryCode3 = pData->discoveryCode.discoveryCode3;
   sCmd.aggregateData1  = (pData->aggregateData >> 16) & 0xFFFF;
   sCmd.aggregateData2  = pData->aggregateData & 0xFFFF;

   nErrCode =  DSL_DRV_VRX_SendMessage( pContext, CMD_PAF_HS_CONTINUE,
                   sizeof(CMD_PAF_HS_Continue_t), (DSL_uint8_t *)&sCmd,
                   sizeof(ACK_PAF_HS_Continue_t), (DSL_uint8_t *)&sAck );

   DSL_DEBUG(DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_BND_VRX_PafHsContinue, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_bnd_vrx.h'
*/
DSL_Error_t DSL_DRV_BND_VRX_ConfigWrite(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_BND_ConfigData_t BndConfig;
#if (DSL_DRV_LINES_PER_DEVICE == 2)
   DSL_PortMode_t nPortMode;
#endif /* DSL_DRV_LINES_PER_DEVICE */
   DSL_BND_VRX_PafHsControl_t PafHsControl;

   DSL_DEBUG(DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_BND_VRX_ConfigWrite" DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext)));

#if (DSL_DRV_LINES_PER_DEVICE == 2)
   /* get port mode type*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->data.nPortMode, nPortMode);
   if ((nPortMode == DSL_PORT_MODE_DUAL) &&
       (DSL_DRV_DEV_FirmwareFeatureCheck(pContext, DSL_FW_XDSLFEATURE_DUALPORT)
                                                                 == DSL_FALSE))
   {
      DSL_DEBUG(DSL_DBG_ERR, (pContext, SYS_DBG_ERR
         "DSL[%02d]: ERROR - Bonding is not supported with the current FW!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERR_NOT_SUPPORTED_BY_FIRMWARE;
   }
#endif /* (DSL_DRV_LINES_PER_DEVICE == 2) */

   /* Get Bonding configuration from the CPE API Context*/
   DSL_CTX_READ(pContext, nErrCode, BndConfig, BndConfig);
#if (DSL_DRV_LINES_PER_DEVICE == 2)
   if (nPortMode != DSL_PORT_MODE_DUAL)
   {
      DSL_DEBUG(DSL_DBG_WRN, (pContext,
         SYS_DBG_MSG"DSL[%02d]: Ignore PAF settings, disable PAF" DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));
      PafHsControl.nPafEnable = VRX_DISABLE;
   }
   else
   {
      PafHsControl.nPafEnable = BndConfig.bPafEnable ? VRX_ENABLE : VRX_DISABLE;
   }
#else
   PafHsControl.nPafEnable = BndConfig.bPafEnable ? VRX_ENABLE : VRX_DISABLE;
#endif

   /* Set PAF control*/
   nErrCode = DSL_DRV_BND_VRX_PafHsControlSet(pContext, &PafHsControl);

   DSL_DEBUG(DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_BND_VRX_ConfigWrite, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_bnd_vrx.h'
*/
DSL_Error_t DSL_DRV_BND_DEV_HwInit(
   DSL_Context_t *pContext,
   DSL_BND_HwInit_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_DEBUG(DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_BND_DEV_HwInit" DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext)));

   /** \todo crosscheck if we need to handle something here.
   */

   DSL_DEBUG(DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_BND_DEV_HwInit, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_bnd_vrx.h'
*/
DSL_Error_t DSL_DRV_BND_DEV_HsStatusGet(
   DSL_Context_t *pContext,
   DSL_BND_HsStatusGet_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_BND_VRX_PafHsStatus_t pafHsStatus;
   DSL_int_t i;

   DSL_DEBUG(DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_BND_DEV_HsStatusGet" DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext)));

   /* Get VRX PAF handshake status*/
   nErrCode = DSL_DRV_BND_VRX_PafHsStatusGet(pContext, &pafHsStatus);

   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG(DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - VRX PAF HS status get failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }
   else
   {
      pData->data.nRemotePafSupported =
         pafHsStatus.bPafEnable ? DSL_BND_ENABLE_ON : DSL_BND_ENABLE_OFF;

      pData->data.nAggregateData = pafHsStatus.aggregateData;

      pData->data.nActivationMode = DSL_BND_NO_COMMAND;

      pData->data.nActivationMode |=
         pafHsStatus.discoveryClearIfSame ? DSL_BND_DISCOVERY_CLEAR_IF_SAME : 0x0;
      pData->data.nActivationMode |=
         pafHsStatus.discoverySetIfClear ? DSL_BND_DISCOVERY_SET_IF_CLEAR : 0x0;
      pData->data.nActivationMode |=
         pafHsStatus.aggregClear ? DSL_BND_AGGREGATE_CLR : 0x0;
      pData->data.nActivationMode |=
         pafHsStatus.aggregSet ? DSL_BND_AGGREGATE_SET : 0x0;

      /* Copy 6 octets of the Discovery code*/
      for (i = 0; i < 6; i++)
      {
         if (i < 2)
            pData->data.nDiscoveryCode[i] = (pafHsStatus.discoveryCode.discoveryCode3 >> (8*(i))) & 0xFF;
         else if (i < 4)
            pData->data.nDiscoveryCode[i] = (pafHsStatus.discoveryCode.discoveryCode2 >> (8*(i-2))) & 0xFF;
         else
            pData->data.nDiscoveryCode[i] = (pafHsStatus.discoveryCode.discoveryCode1 >> (8*(i-4))) & 0xFF;
      }
   }

   DSL_DEBUG(DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_BND_DEV_HsStatusGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_bnd_vrx.h'
*/
DSL_Error_t DSL_DRV_BND_DEV_HsContinue(
   DSL_Context_t *pContext,
   DSL_BND_HsContinue_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_BND_VRX_PafHsContinue_t pafHsContinue = {{0,0},0};
   DSL_int_t i;

   DSL_DEBUG(DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_BND_DEV_HsContinue" DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext)));

   /* Copy 6 octets of the Discovery code*/
   for (i = 0; i < 6; i++)
   {
      if (i < 2)
         pafHsContinue.discoveryCode.discoveryCode3 |= pData->data.nDiscoveryCode[i] << (8*i);
      else if (i < 4)
         pafHsContinue.discoveryCode.discoveryCode2 |= pData->data.nDiscoveryCode[i] << (8*(i-2));
      else
         pafHsContinue.discoveryCode.discoveryCode1 |= pData->data.nDiscoveryCode[i] << (8*(i-4));
   }

   /* Set Aggregate data*/
   pafHsContinue.aggregateData = pData->data.nAggregateData;

   nErrCode = DSL_DRV_BND_VRX_PafHsContinue(pContext, &pafHsContinue);

   DSL_DEBUG(DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_BND_DEV_HsContinue, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_bnd_vrx.h'
*/
DSL_Error_t DSL_DRV_BND_DEV_EthDbgCountersGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_OUT DSL_BND_EthDbgCounters_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_DEBUG(DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_BND_DEV_EthDbgCountersGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   DSL_DEBUG(DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_BND_DEV_EthDbgCountersGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_bnd_vrx.h'
*/
DSL_Error_t DSL_DRV_BND_DEV_EthCountersGet(
   DSL_Context_t *pContext,
   DSL_BND_EthCounters_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_DEBUG(DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_BND_DEV_EthCountersGet" DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext)));

   DSL_DEBUG(DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_BND_DEV_EthCountersGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

#endif /* #if defined(INCLUDE_DSL_BONDING) && defined(INCLUDE_DSL_CPE_API_VRX)*/

#ifdef __cplusplus
}
#endif
