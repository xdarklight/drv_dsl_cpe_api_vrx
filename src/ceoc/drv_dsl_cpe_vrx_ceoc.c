/******************************************************************************

                              Copyright (c) 2014
                            Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#define DSL_INTERN

#include "drv_dsl_cpe_api.h"

#if defined(INCLUDE_DSL_CPE_API_VRX) && defined(INCLUDE_DSL_CEOC)

#include "drv_dsl_cpe_intern_ceoc.h"

/* MEI CPE driver interface header file*/
#include "drv_mei_cpe_api_intern.h"

#undef DSL_DBG_BLOCK
#define DSL_DBG_BLOCK DSL_DBG_CEOC

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device_ceoc.h'
*/
DSL_Error_t DSL_CEOC_DEV_Start(DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_DEV_Handle_t dev;
   IOCTL_VRX_reqCfg_t fioVrxDrvCfg;
   IOCTL_VRX_CEOC_init_t fioVrxDrvCeocInit;
   IOCTL_VRX_CEOC_cntrl_t fioVrxDrvCeocCntrl;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_CEOC_DEV_Start"DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext)));

   /* Open separate instance of the CIANX driver for the EOC handling*/
   dev = DSL_DRV_DEV_DriverHandleGet(0,0);

   if( dev == DSL_NULL )
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - VRX driver open failed!"DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));

      return DSL_ERROR;
   }

   memset(&fioVrxDrvCfg, 0x00, sizeof(IOCTL_VRX_reqCfg_t));
   /* Request the configuration of the VRX Driver*/
   VRX_InternalRequestConfig( (VRX_DYN_CNTRL_T*)dev, &fioVrxDrvCfg );

   /* Check current driver state*/
   if( fioVrxDrvCfg.currDrvState == 0 )
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - VRX driver not initialized!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERROR;
   }

   /* Setup of the ClearEOC feature */
   if( VRX_InternalCEocInit( (VRX_DYN_CNTRL_T*)dev, &fioVrxDrvCeocInit) != 0 )
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - VRX driver CEOC init failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERROR;
   }

   /* Apply control settings*/
   memset(&fioVrxDrvCeocCntrl, 0x00, sizeof(IOCTL_VRX_CEOC_cntrl_t));
   /* Select Auto mode for the CEOC handling*/
   fioVrxDrvCeocCntrl.opMode = VRX_CEOC_OPERATION_MODE_AUTO;
   if( VRX_InternalCEocCntrl((VRX_DYN_CNTRL_T*)dev, &fioVrxDrvCeocCntrl) != 0 )
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - VRX driver CEOC control failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERROR;
   }

   /* Copy VRX driver handle to the internal CEOC context*/
   DSL_CEOC_CONTEXT(pContext)->lowHandle = dev;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_CEOC_DEV_Start, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

DSL_Error_t DSL_CEOC_DEV_Stop(DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_CEOC_DEV_Stop"DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext)));

   if( DSL_CEOC_CONTEXT(pContext)->lowHandle != DSL_NULL )
   {
      /* Close VRX driver*/
      nErrCode = DSL_DRV_DEV_DriverHandleDelete( DSL_CEOC_CONTEXT(pContext)->lowHandle );
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_CEOC_DEV_Stop, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

DSL_Error_t DSL_CEOC_DEV_Restart(DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_CEOC_DEV_Restart"DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext)));

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_CEOC_DEV_Restart, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}


DSL_Error_t DSL_CEOC_DEV_MessageSend(
   DSL_Context_t *pContext,
   DSL_uint16_t protIdent,
   DSL_CEOC_Message_t *pMsg)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_DEV_Handle_t dev;
   IOCTL_VRX_CEOC_frame_t vrxCeocFrame;

   DSL_CHECK_POINTER(pContext, pMsg);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_CEOC_DEV_MessageSend"DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext)));

   if( DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2) )
   {
      /* Get VRX device driver handle*/
      dev = DSL_CEOC_CONTEXT(pContext)->lowHandle;

      if( dev == DSL_NULL )
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - VRX driver handle is NULL!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return DSL_ERROR;
      }

      vrxCeocFrame.protIdent     = protIdent;
      vrxCeocFrame.dataSize_byte = pMsg->length;
      vrxCeocFrame.pEocData      = pMsg->data;

      /* Write CEOC frame data*/
      if( VRX_InternalCEocFrameWr( (VRX_DYN_CNTRL_T*)dev, &vrxCeocFrame) != 0 )
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - CEOC Frame Write failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         nErrCode = DSL_ERROR;
      }
   }
   else
   {
      nErrCode = DSL_ERR_NOT_SUPPORTED_BY_FIRMWARE;
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - EOC handling is not supported by the ADSL FW!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_CEOC_DEV_MessageSend, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

DSL_Error_t DSL_CEOC_DEV_MessageReceive(
   DSL_Context_t *pContext,
   DSL_uint16_t *protIdent,
   DSL_CEOC_Message_t *pMsg)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   IOCTL_VRX_message_t sMsg;
   DSL_DEV_Handle_t dev;
   DSL_int32_t ret = -1;

   DSL_CHECK_POINTER(pContext, pMsg);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_POINTER(pContext, protIdent);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_CEOC_DEV_MessageReceive"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   if( DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2) )
   {
      /* Get VRX device driver handle*/
      dev = DSL_CEOC_CONTEXT(pContext)->lowHandle;

      if( dev == DSL_NULL )
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - VRX driver handle is NULL!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return DSL_ERROR;
      }

      sMsg.paylSize_byte = sizeof(pMsg->data);
      sMsg.pPayload      = pMsg->data;

      ret = VRX_InternalNfcMsgRead( (VRX_DYN_CNTRL_T*)dev, &sMsg);

      if( ret == (DSL_int32_t)e_VRX_ERR_INVAL_STATE )
      {
         DSL_DEBUG(DSL_DBG_WRN,
            (pContext, SYS_DBG_WRN"DSL[%02d]: WRN - NFC Msg read failed, invalid state!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return DSL_WRN_DEVICE_NO_DATA;
      }

      if( ret < 0 )
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - NFC Msg read failed!"DSL_DRV_CRLF,
            DSL_DEV_NUM(pContext)));

         return DSL_ERROR;
      }

      /* Otherwise, data is available*/
      pMsg->length = (DSL_uint16_t)sMsg.paylSize_byte;
#ifndef HAS_TO_BE_CLARIFIED
/* KA: Manfred will provide information concerning the protocol identifier field in the
        received CEOC message*/
/*   *protIdent = ???;*/
#endif
   }
   else
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - EOC handling is not supported by the ADSL FW!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_CEOC_DEV_MessageReceive, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

#endif /* INCLUDE_DSL_CPE_API_VRX INCLUDE_DSL_CEOC */
