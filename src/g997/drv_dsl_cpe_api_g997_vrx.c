/******************************************************************************

                              Copyright (c) 2014
                            Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#define DSL_INTERN

#include "drv_dsl_cpe_api.h"

#ifdef INCLUDE_DSL_CPE_API_VRX

#include "drv_dsl_cpe_device_vrx.h"

#ifdef __cplusplus
   extern "C" {
#endif

#undef DSL_DBG_BLOCK
#define DSL_DBG_BLOCK DSL_DBG_DEVICE

#ifdef INCLUDE_DSL_G997_PER_TONE
/*
   For a detailed description please refer to the drv_dsl_cpe_intern_g997.h
*/
DSL_Error_t DSL_DRV_DEV_G997_BitAllocationNSCGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_G997_BitAllocationNsc_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_DEBUG(DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_G997_DEV_BitAllocationNSCGet(nDirection=%d)"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), pData->nDirection));

   nErrCode = DSL_DRV_VRX_BitAllocationTableGet(pContext,
                  pData->nDirection, &(pData->data.bitAllocationNsc), DSL_NULL);

   DSL_DEBUG(DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_G997_DEV_BitAllocationNSCGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern_g997.h'
*/
DSL_Error_t DSL_DRV_DEV_G997_GainAllocationNscGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_G997_GainAllocationNsc_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_DEBUG(DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_G997_GainAllocationNscGet (nDirection=%d)"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), pData->nDirection));

   nErrCode = DSL_DRV_VRX_GainAllocationTableGet(pContext,
                  pData->nDirection,
                  &(pData->data.gainAllocationNsc));

   DSL_DEBUG(DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_G997_GainAllocationNscGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return (nErrCode);
}

/*
   For a detailed description please refer to the drv_dsl_cpe_intern_g997.h
*/
DSL_Error_t DSL_DRV_DEV_G997_SnrAllocationNscGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_G997_SnrAllocationNsc_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_LineStateValue_t nCurrentState = DSL_LINESTATE_UNKNOWN;

   DSL_DEBUG(DSL_DBG_MSG, (pContext,SYS_DBG_MSG
      "DSL[%02d]: IN - DSL_DRV_DEV_G997_SnrAllocationNscGet (nDirection=%d)"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), pData->nDirection));

   /* Get current line state*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineState, nCurrentState);

   /* Only proceed if the specified line is in SHOWTIME state.*/
   if ( (nCurrentState == DSL_LINESTATE_SHOWTIME_TC_SYNC  ||
         nCurrentState == DSL_LINESTATE_SHOWTIME_NO_SYNC))
   {

      if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2) &&
                                           (pData->nDirection == DSL_UPSTREAM))
      {
         nErrCode = DSL_DRV_VRX_TestParamsFeCheck(pContext);
         if (nErrCode >= DSL_SUCCESS)
         {
            if( pContext->DELT_SHOWTIME != DSL_NULL )
            {
               pData->data.snrAllocationNsc.nNumData
                     = pContext->DELT_SHOWTIME->snrDataUsVdsl.deltSnr.nNumData;
               /* copy data */
               memcpy(&pData->data.snrAllocationNsc.nNSCData,
                      pContext->DELT_SHOWTIME->snrDataUsVdsl.deltSnr.nSCGData,
                      pData->data.snrAllocationNsc.nNumData);
            }
            else
            {
               nErrCode = DSL_ERR_DELT_DATA_NOT_AVAILABLE;
            }
         }
      }
      else
      {
         nErrCode = DSL_DRV_VRX_SendMsgSnrGet(pContext,
                           pData->nDirection, &(pData->data.snrAllocationNsc));
      }
   }
   else
   {
      nErrCode = DSL_ERR_ONLY_AVAILABLE_IN_SHOWTIME;
   }

   DSL_DEBUG(DSL_DBG_MSG, (pContext,SYS_DBG_MSG
      "DSL[%02d]: OUT - DSL_DRV_DEV_G997_SnrAllocationNscGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return (nErrCode);
}
#endif /* INCLUDE_DSL_G997_PER_TONE*/

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern_g997.h'
*/
DSL_Error_t DSL_DRV_DEV_G997_LineStatusPerBandGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_G997_LineStatusPerBand_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_AccessDir_t nDirection = pData->nDirection;
   DSL_LineStateValue_t nCurrentState = DSL_LINESTATE_UNKNOWN;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_G997_LineStatusPerBandGet (nDirection=%d)"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nDirection));

   memset((void *) pData, 0, sizeof (DSL_G997_LineStatusPerBand_t));

   /* Show Line Status Common information only if the line is in
   * SHOWTIME state. In other cases reset Line Status Common data.
   */
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineState, nCurrentState);

   /* Only proceed if the specified line is in SHOWTIME state.
      In other cases set number of used data elements to zero. */
   if ((nCurrentState == DSL_LINESTATE_SHOWTIME_TC_SYNC) ||
       (nCurrentState == DSL_LINESTATE_SHOWTIME_NO_SYNC))
   {
      nErrCode = DSL_DRV_VRX_LineStatusPerBandGet(pContext, nDirection, &(pData->data));
   }
   else
   {
      nErrCode = DSL_ERR_ONLY_AVAILABLE_IN_SHOWTIME;

      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Function is only available in the SHOWTIME!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_G997_LineStatusPerBandGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern_g997.h'
*/
DSL_Error_t DSL_DRV_DEV_G997_UsPowerBackOffStatusGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_G997_UsPowerBackOffStatus_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_LineStateValue_t nCurrentState = DSL_LINESTATE_UNKNOWN;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_G997_UsPowerBackOffStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   memset((void *) pData, 0, sizeof (DSL_G997_UsPowerBackOffStatus_t));

   /* Show Line Status Common information only if the line is in
   * SHOWTIME state. In other cases reset Line Status Common data.
   */
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineState, nCurrentState);

   /* Only proceed if the specified line is in SHOWTIME state.
      In other cases set number of used data elements to zero. */
   if ((nCurrentState == DSL_LINESTATE_SHOWTIME_TC_SYNC) ||
       (nCurrentState == DSL_LINESTATE_SHOWTIME_NO_SYNC))
   {
      nErrCode = DSL_DRV_VRX_UsPowerBackOffStatusGet(pContext, &(pData->data));
   }
   else
   {
      nErrCode = DSL_ERR_ONLY_AVAILABLE_IN_SHOWTIME;

      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Function is only available in the SHOWTIME!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_G997_UsPowerBackOffStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return (nErrCode);
}

/*
   For a detailed description please refer to the drv_dsl_cpe_intern_g997.h
*/
DSL_Error_t DSL_DRV_DEV_G997_LineStatusGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_G997_LineStatus_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_AccessDir_t nDirection = pData->nDirection;
   DSL_LineStateValue_t nCurrentState = DSL_LINESTATE_UNKNOWN;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_G997_LineStatusGet (nDirection=%d)"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nDirection));

   memset((void *) &(pData->data), 0, sizeof(DSL_G997_LineStatusData_t));

   if (pData->nDeltDataType == DSL_DELT_DATA_DIAGNOSTICS)
   {
#ifdef INCLUDE_DSL_DELT
      /* get the Line Status data from the internal buffer*/
      if( pContext->DELT != DSL_NULL )
      {
         memcpy(
            &(pData->data),
            &(pContext->DELT->lineStatus[nDirection]),
            sizeof(DSL_G997_LineStatusData_t));
      }
      else
      {
         nErrCode = DSL_ERR_DELT_DATA_NOT_AVAILABLE;
      }
#else
      nErrCode = DSL_ERR_DELT_DATA_NOT_AVAILABLE;
#endif /* INCLUDE_DSL_DELT*/
   }
   else if (pData->nDeltDataType == DSL_DELT_DATA_SHOWTIME)
   {
      DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineState, nCurrentState);

      /* Only proceed if the specified line is in SHOWTIME state.
         In other cases set number of used data elements to zero. */
      if ((nCurrentState == DSL_LINESTATE_SHOWTIME_TC_SYNC) ||
          (nCurrentState == DSL_LINESTATE_SHOWTIME_NO_SYNC))
      {
         nErrCode = DSL_DRV_VRX_LineStatusGet(pContext, nDirection, &(pData->data));
      }
      else
      {
         nErrCode = DSL_ERR_ONLY_AVAILABLE_IN_SHOWTIME;

         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Function is only available in the SHOWTIME!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      }
   }
   else
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Unknown DELT data type!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      nErrCode = DSL_ERROR;
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_G997_LineStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return (nErrCode);
}

/*
   For a detailed description please refer to the drv_dsl_cpe_intern_g997.h
*/
#ifdef INCLUDE_DSL_G997_LINE_INVENTORY
DSL_Error_t DSL_DRV_DEV_G997_LineInventoryGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_G997_LineInventory_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_XTUDir_t nDirection = pData->nDirection;

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_G997_LineInventoryGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Copy inventory data*/
   if( nDirection == DSL_NEAR_END )
   {
      DSL_CTX_READ(pContext, nErrCode, lineInventoryNe, pData->data);
   }
   else
   {
      DSL_CTX_READ(pContext, nErrCode, lineInventoryFe, pData->data);
   }

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_G997_LineInventoryGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}
#endif /* INCLUDE_DSL_G997_LINE_INVENTORY*/

/*
   For a detailed description please refer to the drv_dsl_cpe_intern_g997.h
*/
#ifdef INCLUDE_DSL_G997_LINE_INVENTORY
DSL_Error_t DSL_DRV_DEV_G997_LineInventorySet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_G997_LineInventoryNe_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_G997_LineInventorySet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   DSL_DRV_MUTEX_LOCK(pContext->dataMutex);

   /* Set VersionNumber*/
   memcpy(
      pContext->lineInventoryNe.VersionNumber,
      pData->data.VersionNumber,
      DSL_G997_LI_MAXLEN_VERSION);

   /* Set SystemVendorID*/
   memcpy(
      pContext->lineInventoryNe.SystemVendorID,
      pData->data.SystemVendorID,
      DSL_G997_LI_MAXLEN_VENDOR_ID);

   /* Set SerialNumber*/
   memcpy(
      pContext->lineInventoryNe.SerialNumber,
      pData->data.SerialNumber,
      DSL_G997_LI_MAXLEN_SERIAL);

   /* Set auxInventoryNe*/
   memcpy(
      &(pContext->auxInventoryNe),
      &(pData->data.Auxiliary),
      sizeof(DSL_AuxInventoryNe_t));

   DSL_DRV_MUTEX_UNLOCK(pContext->dataMutex);

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_G997_LineInventorySet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}
#endif /* INCLUDE_DSL_G997_LINE_INVENTORY*/

#ifdef INCLUDE_DSL_G997_STATUS
/*
   For a detailed description please refer to the drv_dsl_cpe_intern_g997.h
*/
DSL_Error_t DSL_DRV_DEV_G997_LineTransmissionStatusGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_G997_LineTransmissionStatus_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_LineStateValue_t nCurrentState = DSL_LINESTATE_UNKNOWN;
   DSL_G997_BF_LineFailures_t nLineFailures = DSL_G997_LINEFAILURE_CLEANED;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_G997_LineTransmissionStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Get current line state*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineState, nCurrentState);
   /* Get Line Failures information for the Near-End*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineFailuresNe, nLineFailures);

   /* Only proceed if the specified line is in SHOWTIME state.*/
   if ( nCurrentState == DSL_LINESTATE_SHOWTIME_TC_SYNC &&
        (nLineFailures & DSL_G997_LINEFAILURE_ESE) == 0 )
   {
      pData->data.nLineTransmissionStatus = DSL_G997_LINE_TRANSMISSION_AVAILABLE;
   }
   else
   {
      pData->data.nLineTransmissionStatus = DSL_G997_LINE_TRANSMISSION_NOT_AVAILABLE;
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_G997_LineTransmissionStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}
#endif /* INCLUDE_DSL_G997_STATUS*/

/*
   For a detailed description please refer to the drv_dsl_cpe_intern_g997.h
*/
DSL_Error_t DSL_DRV_DEV_G997_ChannelStatusGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_G997_ChannelStatus_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_LineStateValue_t nCurrentState = DSL_LINESTATE_UNKNOWN;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_G997_ChannelStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Get current line state*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineState, nCurrentState);

   /* Only proceed if the line is in SHOWTIME state.*/
   if ( (nCurrentState == DSL_LINESTATE_SHOWTIME_TC_SYNC) ||
        (nCurrentState == DSL_LINESTATE_SHOWTIME_NO_SYNC) )
   {
      nErrCode = DSL_DRV_VRX_ChannelStatusGet(
                    pContext,
                    pData->nChannel,
                    pData->nDirection,
                    &(pData->data));
   }
   else
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Function is only available in the SHOWTIME!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      nErrCode = DSL_ERR_ONLY_AVAILABLE_IN_SHOWTIME;
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_G997_ChannelStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

#ifdef INCLUDE_DSL_G997_STATUS
/*
   For a detailed description please refer to the drv_dsl_cpe_intern_g997.h
*/
DSL_Error_t DSL_DRV_DEV_G997_LastStateTransmittedGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_G997_LastStateTransmitted_t *pData)
{
   DSL_Error_t nErrCode = DSL_ERR_NOT_SUPPORTED_BY_FIRMWARE;

   return nErrCode;
}
#endif /* INCLUDE_DSL_G997_STATUS*/

/*
   For a detailed description please refer to the drv_dsl_cpe_intern_g997.h
*/
#ifdef INCLUDE_DSL_G997_FRAMING_PARAMETERS
DSL_Error_t DSL_DRV_DEV_G997_FramingParameterStatusGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN const DSL_AccessDir_t nDirection,
   DSL_IN const DSL_uint8_t nChannel,
   DSL_OUT DSL_G997_FramingParameterStatusData_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_LineStateValue_t nCurrentState = DSL_LINESTATE_UNKNOWN;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_G997_FramingParameterStatusGet,"
      " (nDirection=%d, nChannel=%d)"DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext), nDirection, nChannel));

   /* Get current line state*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineState, nCurrentState);

   /* Only proceed if the line is in SHOWTIME state.*/
   if ( (nCurrentState != DSL_LINESTATE_SHOWTIME_TC_SYNC) &&
        (nCurrentState != DSL_LINESTATE_SHOWTIME_NO_SYNC) )
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Function is only available in the SHOWTIME!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERR_ONLY_AVAILABLE_IN_SHOWTIME;
   }

   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
   {
      /* VDSL2 mode */
      nErrCode = DSL_DRV_VRX_G997_FramingParameterStatusVdsl2Get(
                    pContext, nDirection, nChannel, pData);
   }
   else
   {
      /* ADSL mode */
      nErrCode = DSL_DRV_VRX_G997_FramingParameterStatusAdslGet(
                    pContext, nDirection, nChannel, pData);
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_G997_FramingParameterStatusGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}
#endif /* INCLUDE_DSL_G997_FRAMING_PARAMETERS*/

#ifdef INCLUDE_DSL_DELT
/*
   For a detailed description please refer to the drv_dsl_cpe_intern_g997.h
*/
DSL_Error_t DSL_DRV_DEV_G997_DeltHlinScaleGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN const DSL_AccessDir_t nDirection,
   DSL_IN DSL_DeltDataType_t nDeltDataType,
   DSL_OUT DSL_G997_DeltHlinScaleData_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_G997_DeltHlinScaleGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   if (nDeltDataType == DSL_DELT_DATA_SHOWTIME)
   {
      nErrCode = DSL_ERR_NOT_SUPPORTED_BY_FIRMWARE;
   }
   else if (nDeltDataType == DSL_DELT_DATA_DIAGNOSTICS)
   {
      if( pContext->DELT != DSL_NULL )
      {
         pData->nDeltHlinScale = nDirection == DSL_DOWNSTREAM ?
                                 pContext->DELT->hlinScaleDataDs.nDeltHlinScale :
                                 pContext->DELT->hlinScaleDataUs.nDeltHlinScale;
      }
      else
      {
         nErrCode = DSL_ERR_DEVICE_NO_DATA;
      }
   }
   else
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Unknown DELT data type!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      nErrCode = DSL_ERROR;
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_G997_DeltHlinScaleGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

/*
   For a detailed description please refer to the drv_dsl_cpe_intern_g997.h
*/
DSL_Error_t DSL_DRV_DEV_G997_DeltHlinGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN const DSL_AccessDir_t nDirection,
   DSL_IN DSL_DeltDataType_t nDeltDataType,
   DSL_OUT DSL_G997_DeltHlinData_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_G997_DeltHlinGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   if (nDeltDataType == DSL_DELT_DATA_SHOWTIME)
   {
      nErrCode = DSL_ERR_NOT_SUPPORTED_BY_DEFINITION;
   }
   else if (nDeltDataType == DSL_DELT_DATA_DIAGNOSTICS)
   {
      /* get the far end data from the internal buffer*/
      if( pContext->DELT != DSL_NULL )
      {
         memcpy(pData,
                nDirection == DSL_DOWNSTREAM ?
                (DSL_void_t*)&(pContext->DELT->hlinDataDs) :
                (DSL_void_t*)&(pContext->DELT->hlinDataUs),
                nDirection == DSL_DOWNSTREAM ? sizeof(pContext->DELT->hlinDataDs) :
                                               sizeof(pContext->DELT->hlinDataUs));
      }
      else
      {
         nErrCode = DSL_ERR_DELT_DATA_NOT_AVAILABLE;
      }
   }
   else
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Unknown DELT data type!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      nErrCode = DSL_ERROR;
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_G997_DeltHlinGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

/*
   For a detailed description please refer to the drv_dsl_cpe_intern_g997.h
*/
DSL_Error_t DSL_DRV_DEV_G997_DeltHlogGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN const DSL_AccessDir_t nDirection,
   DSL_IN DSL_DeltDataType_t nDeltDataType,
   DSL_OUT DSL_G997_DeltHlogData_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_LineStateValue_t nCurrentState = DSL_LINESTATE_UNKNOWN;
   ACK_TestParamsAuxDS_Get_t  sAck;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_G997_DeltHlogGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   memset(&sAck, 0, sizeof(sAck));

   if (nDeltDataType == DSL_DELT_DATA_SHOWTIME)
   {
      /* Get current line state*/
      DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineState, nCurrentState);

      /* Only proceed if the specified line is in SHOWTIME state.*/
      if ( (nCurrentState == DSL_LINESTATE_SHOWTIME_TC_SYNC  ||
            nCurrentState == DSL_LINESTATE_SHOWTIME_NO_SYNC))
      {
         /* Get Aux Parameters directly from the FW*/
         nErrCode = DSL_DRV_VRX_SendMsgTestParamsAuxGet(
                             pContext, nDirection, (DSL_uint8_t*)&sAck);
         if (nErrCode != DSL_SUCCESS)
         {
            DSL_DEBUG(DSL_DBG_ERR,(pContext, SYS_DBG_ERR
               "DSL[%02d]: ERROR - Aux Test Parameters get failed (nDirection=%d)"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nDirection));
         }

         if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2)
                                               && (nDirection == DSL_UPSTREAM))
         {
            nErrCode = DSL_DRV_VRX_TestParamsFeCheck(pContext);
            if (nErrCode >= DSL_SUCCESS)
            {
               if(pContext->DELT_SHOWTIME != DSL_NULL)
               {
                  /* Get FE showtime parameters from the internal buffer*/
                  memcpy(pData,(DSL_void_t*)&(pContext->DELT_SHOWTIME->hlogDataUsVdsl),
                           sizeof(pContext->DELT_SHOWTIME->hlogDataUsVdsl));
               }
               else
               {
                  nErrCode = DSL_ERR_DELT_DATA_NOT_AVAILABLE;
               }
            }
         }
         else
         {
            /* Get SHOWTIME Hlog parameter directly from the FW*/
            nErrCode = DSL_DRV_VRX_SendMsgHlogGet(pContext, nDirection, &(pData->deltHlog));
            pData->nGroupSize       = (DSL_uint8_t)sAck.HLOGG;
            pData->nMeasurementTime = sAck.HLOGMT;
         }
      }
      else
      {
         nErrCode = DSL_ERR_ONLY_AVAILABLE_IN_SHOWTIME;
      }
   }
   else if (nDeltDataType == DSL_DELT_DATA_DIAGNOSTICS)
   {
      /* get the far end data from the internal buffer*/
      if( pContext->DELT != DSL_NULL )
      {
         memcpy(pData,
                nDirection == DSL_DOWNSTREAM ?
                (DSL_void_t*)&(pContext->DELT->hlogDataDs) :
                (DSL_void_t*)&(pContext->DELT->hlogDataUs),
                nDirection == DSL_DOWNSTREAM ? sizeof(pContext->DELT->hlogDataDs) :
                                               sizeof(pContext->DELT->hlogDataUs));
      }
      else
      {
         nErrCode = DSL_ERR_DELT_DATA_NOT_AVAILABLE;
      }
   }
   else
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Unknown DELT data type!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      nErrCode = DSL_ERROR;
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_G997_DeltHlogGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

/*
   For a detailed description please refer to the drv_dsl_cpe_intern_g997.h
*/
DSL_Error_t DSL_DRV_DEV_G997_DeltQLNGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN const DSL_AccessDir_t nDirection,
   DSL_IN DSL_DeltDataType_t nDeltDataType,
   DSL_OUT DSL_G997_DeltQlnData_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_LineStateValue_t nCurrentState = DSL_LINESTATE_UNKNOWN;
   ACK_TestParamsAuxDS_Get_t  sAck;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_G997_DeltQLNGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   memset(&sAck, 0, sizeof(sAck));

   if (nDeltDataType == DSL_DELT_DATA_SHOWTIME)
   {
      /* Get current line state*/
      DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineState, nCurrentState);

      /* Only proceed if the specified line is in SHOWTIME state.*/
      if ( (nCurrentState == DSL_LINESTATE_SHOWTIME_TC_SYNC  ||
            nCurrentState == DSL_LINESTATE_SHOWTIME_NO_SYNC))
      {
         /* Get Aux Parameters directly from the FW*/
         nErrCode = DSL_DRV_VRX_SendMsgTestParamsAuxGet(
                             pContext, nDirection, (DSL_uint8_t*)&sAck);
         if (nErrCode != DSL_SUCCESS)
         {
            DSL_DEBUG(DSL_DBG_ERR,(pContext, SYS_DBG_ERR
               "DSL[%02d]: ERROR - Aux Test Parameters get failed (nDirection=%d)"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nDirection));
         }

         if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2)
                                               && (nDirection == DSL_UPSTREAM))
         {
            nErrCode = DSL_DRV_VRX_TestParamsFeCheck(pContext);
            if (nErrCode >= DSL_SUCCESS)
            {
               if(pContext->DELT_SHOWTIME != DSL_NULL)
               {
                  /* Get FE showtime parameters from the internal buffer*/
                  memcpy(pData,(DSL_void_t*)&(pContext->DELT_SHOWTIME->qlnDataUsVdsl),
                              sizeof(pContext->DELT_SHOWTIME->qlnDataUsVdsl));
               }
               else
               {
                  nErrCode = DSL_ERR_DELT_DATA_NOT_AVAILABLE;
               }
            }
         }
         else
         {
            /* Get SHOWTIME Qln parameter directly from the FW*/
            nErrCode = DSL_DRV_VRX_SendMsgQlnGet(pContext, nDirection, &(pData->deltQln));
            pData->nGroupSize       = (DSL_uint8_t)sAck.QLNG;
            pData->nMeasurementTime = sAck.QLNMT;
         }
      }
      else
      {
         nErrCode = DSL_ERR_ONLY_AVAILABLE_IN_SHOWTIME;
      }
   }
   else if (nDeltDataType == DSL_DELT_DATA_DIAGNOSTICS)
   {
      /* get the far end data from the internal buffer*/
      if( pContext->DELT != DSL_NULL )
      {
         memcpy(pData,
                nDirection == DSL_DOWNSTREAM ?
                (DSL_void_t*)&(pContext->DELT->qlnDataDs) :
                (DSL_void_t*)&(pContext->DELT->qlnDataUs),
                nDirection == DSL_DOWNSTREAM ? sizeof(pContext->DELT->qlnDataDs) :
                                               sizeof(pContext->DELT->qlnDataUs));
      }
      else
      {
         nErrCode = DSL_ERR_DELT_DATA_NOT_AVAILABLE;
      }
   }
   else
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Unknown DELT data type!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      nErrCode = DSL_ERROR;
   }


   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_G997_DeltQLNGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

/*
   For a detailed description please refer to the drv_dsl_cpe_intern_g997.h
*/
DSL_Error_t DSL_DRV_DEV_G997_DeltSNRGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN const DSL_AccessDir_t nDirection,
   DSL_IN DSL_DeltDataType_t nDeltDataType,
   DSL_OUT DSL_G997_DeltSnrData_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_LineStateValue_t nCurrentState = DSL_LINESTATE_UNKNOWN;
   DSL_VRX_FwVersion_t fwVersion = {0};
   ACK_TestParamsAuxDS_Get_t  sAck;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_G997_DeltSNRGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   memset(&sAck, 0, sizeof(sAck));

   if (nDeltDataType == DSL_DELT_DATA_SHOWTIME)
   {
      /* Get current line state*/
      DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineState, nCurrentState);

      /* Only proceed if the specified line is in SHOWTIME state.*/
      if ( (nCurrentState == DSL_LINESTATE_SHOWTIME_TC_SYNC  ||
            nCurrentState == DSL_LINESTATE_SHOWTIME_NO_SYNC))
      {
         /* Get Aux Parameters directly from the FW*/
         nErrCode = DSL_DRV_VRX_SendMsgTestParamsAuxGet(
                       pContext, nDirection, (DSL_uint8_t*)&sAck);

         if (nErrCode != DSL_SUCCESS)
         {
            DSL_DEBUG(DSL_DBG_ERR,(pContext, SYS_DBG_ERR
               "DSL[%02d]: ERROR - Aux Test Parameters get failed (nDirection=%d)"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nDirection));
         }

         if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2)
                                               && (nDirection == DSL_UPSTREAM))
         {
            nErrCode = DSL_DRV_VRX_TestParamsFeCheck(pContext);
            if (nErrCode >= DSL_SUCCESS)
            {
               if(pContext->DELT_SHOWTIME != DSL_NULL)
               {
                  /* Get FE showtime parameters from the internal buffer*/
                  memcpy(pData,(DSL_void_t*)&(pContext->DELT_SHOWTIME->snrDataUsVdsl),
                               sizeof(pContext->DELT_SHOWTIME->snrDataUsVdsl));
               }
               else
               {
                  nErrCode = DSL_ERR_DELT_DATA_NOT_AVAILABLE;
               }
            }
         }
         else
         {
            nErrCode = DSL_DRV_VRX_SendMsgSnrGet(pContext, nDirection, &(pData->deltSnr));

            pData->nGroupSize       = (DSL_uint8_t)sAck.SNRG;
            pData->nMeasurementTime = sAck.SNRMT;
         }
      }
      else
      {
         nErrCode = DSL_ERR_ONLY_AVAILABLE_IN_SHOWTIME;
      }
   }
   else if (nDeltDataType == DSL_DELT_DATA_DIAGNOSTICS)
   {
      /* get the far end data from the internal buffer*/
      if( pContext->DELT != DSL_NULL )
      {
         memcpy(pData,
                nDirection == DSL_DOWNSTREAM ?
                (DSL_void_t*)&(pContext->DELT->snrDataDs) :
                (DSL_void_t*)&(pContext->DELT->snrDataUs),
                nDirection == DSL_DOWNSTREAM ?
                                           sizeof(pContext->DELT->snrDataDs) :
                                           sizeof(pContext->DELT->snrDataUs));
      }
      else
      {
         nErrCode = DSL_ERR_DELT_DATA_NOT_AVAILABLE;
      }
   }
   else
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Unknown DELT data type!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      nErrCode = DSL_ERROR;
   }


   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_G997_DeltSNRGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}
#endif /* INCLUDE_DSL_DELT*/

/*
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_G997_POWER_MANAGEMENT_STATE_FORCED_TRIGGER
*/
DSL_Error_t DSL_DRV_DEV_G997_PowerManagementStateForcedTrigger(
   DSL_IN DSL_Context_t *pContext,
   DSL_OUT DSL_G997_PowerManagementStateForcedTriggerData_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS, nRet = DSL_SUCCESS;
   DSL_int_t nAttempt = 0, i = 0;
   const DSL_int_t nMaxRetry = DSL_LX_TO_L3_TIMEOUT/DSL_L3_WAIT_INTERVAL;
   DSL_G997_PowerManagementStatusData_t PmStatus = {DSL_G997_PMS_NA};
   DSL_VRX_L3RequestStatus_t L3RequestStatus = DSL_VRX_L3_STATUS_NA;
   DSL_VRX_L3RequestFailReason_t L3FailReason = DSL_VRX_L3_FAIL_REASON_NA;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_G997_PowerManagementStateForcedTrigger"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Get Actual Power Management Status from the DSL CPE context*/
   DSL_CTX_READ(pContext, nErrCode, powerMgmtStatus, PmStatus);

   switch(pData->nPowerManagementState)
   {
   case DSL_G997_PMSF_L3_TO_L0:
      if(PmStatus.nPowerManagementStatus == DSL_G997_PMS_L3)
      {
         /* Reset Flag*/
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, bPowerManagementL3Forced, DSL_FALSE);
      }
      else
      {
         DSL_DEBUG( DSL_DBG_WRN,
            (pContext,
            SYS_DBG_WRN"DSL[%02d]: PMSF L3-L0 requested but not feasible (actual mode: L%d)"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
            (DSL_int_t)PmStatus.nPowerManagementStatus));
         nErrCode = DSL_ERROR;
      }
      break;

   case DSL_G997_PMSF_L0_TO_L2:
      DSL_DEBUG( DSL_DBG_WRN,
         (pContext,
         SYS_DBG_WRN"DSL[%02d]: PMSF L0-L2 requested but not supported."
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      nErrCode = DSL_ERR_NOT_SUPPORTED_BY_DEVICE;
      break;

   case DSL_G997_PMSF_LX_TO_L3:
      if (PmStatus.nPowerManagementStatus != DSL_G997_PMS_L3)
      {
         /* retry it DSL_LX_TO_L3_ATTEMPT_COUNT times */
         for (nAttempt = 0; nAttempt < DSL_LX_TO_L3_ATTEMPT_COUNT; nAttempt++)
         {
            /* L3 shutdown request*/
            nErrCode = DSL_DRV_VRX_SendMsgShutdownRequest(pContext);
            if (nErrCode != DSL_SUCCESS)
            {
               DSL_DEBUG( DSL_DBG_ERR,
                  (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - L3 shutdown request failed!"
                  DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
               nRet = nErrCode;
               break;
            }

            /* Poll for the L3 request status with the specified timeout*/
            for (i = 0; i < nMaxRetry; i++)
            {
               DSL_WAIT(DSL_L3_WAIT_INTERVAL);

               /* Get L3 request status*/
               nErrCode = DSL_DRV_VRX_SendMsgTxL3RequestStatusGet(pContext, &L3RequestStatus);
               if (nErrCode != DSL_SUCCESS)
               {
                  DSL_DEBUG( DSL_DBG_ERR,
                     (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - L3 shutdown request status get failed!"
                     DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
                  nRet = nErrCode;
                  break;
               }

               /* Check L3 status*/
               if (L3RequestStatus == DSL_VRX_L3_STATUS_PENDING)
               {
                  /* Poll status again until API timeout*/
                  continue;
               }
               else if (L3RequestStatus == DSL_VRX_L3_STATUS_ACCEPTED)
               {
                  /* L3 request accepted*/
                  break;
               }
               else if (L3RequestStatus == DSL_VRX_L3_STATUS_REJECTED)
               {
                  DSL_DEBUG( DSL_DBG_ERR,
                     (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - L3 request rejected!"
                     DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

                  nErrCode = DSL_ERR_L3_REJECTED;

                  break;
               }
               else if (L3RequestStatus == DSL_VRX_L3_STATUS_FAIL)
               {
                  /* Get L3 request fail reason*/
                  nErrCode = DSL_DRV_VRX_SendMsgTxL3RequestFailReasonGet(pContext, &L3FailReason);
                  if (nErrCode != DSL_SUCCESS)
                  {
                     DSL_DEBUG( DSL_DBG_ERR,
                        (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - L3 request fail reason get failed!"
                        DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
                     nRet = nErrCode;
                     break;
                  }

                  /* Check L3 request fail reason*/
                  if (L3FailReason == DSL_VRX_L3_FAIL_REASON_NOT_L0)
                  {
                     nErrCode = DSL_ERR_L3_NOT_IN_L0;
                  }
                  else if (L3FailReason == DSL_VRX_L3_FAIL_REASON_TIMEOUT)
                  {
                     DSL_DEBUG( DSL_DBG_ERR,
                        (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - L3 request FW timeout occured!"
                        DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

                     nErrCode = DSL_ERR_L3_TIMED_OUT;
                  }
                  else
                  {
                     DSL_DEBUG( DSL_DBG_ERR,
                         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Unknown L3 request fail reason (%d)"
                         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), L3FailReason));
                     nRet = DSL_ERROR;
                     break;
                  }
               }
               else
               {
                  DSL_DEBUG( DSL_DBG_ERR,
                     (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Unknown L3 request status (%d)"
                     DSL_DRV_CRLF, DSL_DEV_NUM(pContext), L3RequestStatus));
                  nRet = DSL_ERR_L3_UNKNOWN_FAILURE;
                  break;
               }
            }

            if (nRet != DSL_SUCCESS)
            {
               nErrCode = nRet;
               break;
            }

            if (L3RequestStatus == DSL_VRX_L3_STATUS_ACCEPTED)
            {
               /*Set flag*/
               DSL_CTX_WRITE_SCALAR(pContext, nErrCode, bPowerManagementL3Forced, DSL_TRUE);

               /* Set Power Management Status*/
               nErrCode = DSL_DRV_VRX_PowerManagementStatusSet(pContext, DSL_G997_PMS_L3);
               if( nErrCode != DSL_SUCCESS )
               {
                  DSL_DEBUG( DSL_DBG_ERR,
                     (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Power Managemnt Status set failed!"
                     DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
                  break;
               }

               /* Trigger restart sequence*/
               DSL_CTX_WRITE_SCALAR(pContext, nErrCode, bAutobootRestart, DSL_TRUE);

               /* L3 request accepted, exit any retries*/
               break;
            }
            else if (L3RequestStatus == DSL_VRX_L3_STATUS_PENDING)
            {
               DSL_DEBUG( DSL_DBG_ERR,
                  (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - L3 request API timeout occured!"
                  DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

               nErrCode = DSL_ERR_L3_TIMED_OUT;
            }
         }
      }
      else
      {
         DSL_DEBUG( DSL_DBG_WRN,
            (pContext,
            SYS_DBG_WRN"DSL[%02d]: PMSF Lx-L3 requested but not feasible (actual mode: L%d)"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
            (DSL_int_t)PmStatus.nPowerManagementStatus));
         nErrCode = DSL_ERROR;
      }
      break;
   default:
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext,
         SYS_DBG_ERR"DSL[%02d]: Unknown trigger specified!" DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));
      nErrCode = DSL_ERROR;
      break;
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_G997_PowerManagementStateForcedTrigger, "
      "retCode=%d"DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}


#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_DSL_CPE_API_VRX */
