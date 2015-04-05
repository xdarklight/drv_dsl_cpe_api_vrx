/******************************************************************************

                               Copyright (c) 2011
                            Lantiq Deutschland GmbH
                     Am Campeon 3; 85579 Neubiberg, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#define DSL_INTERN

#include "drv_dsl_cpe_api.h"

#undef DSL_DBG_BLOCK
#define DSL_DBG_BLOCK DSL_DBG_SAR

#ifdef INCLUDE_DSL_CPE_API_SAR_SUPPORT

/** \file
   Segmentation and Reassembly (SAR) implementation
*/

#undef DSL_DBG_BLOCK
#define DSL_DBG_BLOCK DSL_DBG_SAR

DSL_Error_t DSL_DRV_SAR_ConfigCheck(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_SAR_ConfigData_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   /* Check VCI range*/
   if (pData->nDefVci < 32)
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - nDefVci parameter range check error!"
         " Valid range 32...65535."DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));

      nErrCode = DSL_ERR_INVALID_PARAMETER;
   }

   /* Check VPI range*/
   if (pData->nDefVpi > 255)
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - nDefVpi parameter range check error!"
         " Valid range 0...255."DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));

      nErrCode = DSL_ERR_INVALID_PARAMETER;
   }

   /* Check nSegClassFilter range*/
   if ((pData->nSegClassFilter < DSL_SAR_FILTER_MAC_DEST_ADDR) ||
       (pData->nSegClassFilter >= DSL_SAR_FILTER_LAST))
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Invalid nSegClassFilter(%d) specified!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), pData->nSegClassFilter));

      nErrCode = DSL_ERR_INVALID_PARAMETER;
   }

   /* Check nMuxType range*/
   if ((pData->nMuxType < DSL_SAR_MUX_VC_MUX) ||
       (pData->nMuxType >= DSL_SAR_MUX_LAST))
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Invalid nMuxType(%d) specified!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), pData->nMuxType));

      nErrCode = DSL_ERR_INVALID_PARAMETER;
   }

   return (nErrCode);
}

/*
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_SAR_CONFIG_SET
*/
DSL_Error_t DSL_DRV_SAR_ConfigSet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_SAR_Config_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_CHANNEL_RANGE(pData->nChannel);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_SAR_ConfigSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   nErrCode = DSL_DRV_SAR_ConfigCheck(pContext, &(pData->data));

   if (nErrCode == DSL_SUCCESS)
   {
      /* Copy SAR configuration data to internal VINAX device context*/
      DSL_CTX_WRITE(pContext, nErrCode, pDevCtx->data.deviceCfg.SARcfg, pData->data);
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_SAR_ConfigSet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

/*
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_SAR_CONFIG_GET
*/
#ifdef INCLUDE_DSL_CONFIG_GET
DSL_Error_t DSL_DRV_SAR_ConfigGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_SAR_Config_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();
   DSL_CHECK_CHANNEL_RANGE(pData->nChannel);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_SAR_ConfigSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Copy SAR configuration data from the internal VINAX device context*/
   DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.deviceCfg.SARcfg, pData->data);

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_SAR_ConfigSet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}
#endif /* INCLUDE_DSL_CONFIG_GET*/

/*
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_SAR_SEGMENTATION_TABLE_SET
*/
DSL_Error_t DSL_DRV_SAR_SegmentationTableSet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_SAR_SegmentationTable_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint16_t nArrayIdx = 0, i = 0;
   DSL_SAR_SegmentationTableEntry_t const *pEntry = DSL_NULL;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_CHANNEL_RANGE(pData->nChannel);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_SAR_SegmentationTableSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Check for the nNumEntries parameter*/
   if ( pData->data.nNumEntries > DSL_SAR_MAX_SEG_TABLE_ENTRIES)
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Invalid nNumEntries=%d specified!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), pData->data.nNumEntries));

      return DSL_ERR_PARAM_RANGE;
   }

   if(DSL_DRV_MUTEX_LOCK(pContext->dataMutex))
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - data mutex lock failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERROR;
   }

   /* Configure Segmentation Table*/
   nArrayIdx = 0;

   for(nArrayIdx = 0; nArrayIdx < pData->data.nNumEntries; nArrayIdx++)
   {
      pEntry = pData->data.pTableEntry + nArrayIdx;

      for( i = 0; i < 6; i++ )
      {
         pContext->pDevCtx->data.deviceCfg.SegmentationTable.pTableEntry[nArrayIdx].pMacAddress[i] =
            pEntry->pMacAddress[i];
      }

      /* Check for the 0 < nVlanId < 4095*/
      if ((pEntry->nVlanId == 0) || (pEntry->nVlanId > 4095))
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Invalid nVlanId(%d)=%d specified!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nArrayIdx, pEntry->nVlanId));
         nErrCode = DSL_ERR_PARAM_RANGE;
         break;
      }

      /* Check for the 0 <= nVlanPrio <= 7*/
      if (pEntry->nVlanPrio > 7)
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Invalid nVlanPrio(%d)=%d specified!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nArrayIdx, pEntry->nVlanPrio));
         nErrCode = DSL_ERR_PARAM_RANGE;
         break;
      }

      /* Check for the 0 <= nVpi <=255*/
      if (pEntry->nVpi > 255)
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Invalid nVpi(%d)=%d specified!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nArrayIdx, pEntry->nVpi));
         nErrCode = DSL_ERR_PARAM_RANGE;
         break;
      }

      /* Check for the nVci > 31*/
      if ((pEntry->nVci <= 31) )
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Invalid nVci(%d)=%d specified!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nArrayIdx, pEntry->nVci));
         nErrCode = DSL_ERR_PARAM_RANGE;
         break;
      }

      /* Set nVlanId parameter*/
      pContext->pDevCtx->data.deviceCfg.SegmentationTable.pTableEntry[nArrayIdx].nVlanId   = pEntry->nVlanId;
      /* Set nVlanPrio parameter*/
      pContext->pDevCtx->data.deviceCfg.SegmentationTable.pTableEntry[nArrayIdx].nVlanPrio = pEntry->nVlanPrio;
      /* Set nVpi parameter*/
      pContext->pDevCtx->data.deviceCfg.SegmentationTable.pTableEntry[nArrayIdx].nVpi      = pEntry->nVpi;
      /* Set nVci parameter*/
      pContext->pDevCtx->data.deviceCfg.SegmentationTable.pTableEntry[nArrayIdx].nVci      = pEntry->nVci;
   }

   if (nErrCode == DSL_SUCCESS)
   {
      pContext->pDevCtx->data.deviceCfg.SegmentationTable.nNumEntries = pData->data.nNumEntries;
   }

   DSL_DRV_MUTEX_UNLOCK(pContext->dataMutex);

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_SAR_SegmentationTableSet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

/*
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_SAR_SEGMENTATION_TABLE_GET
*/
DSL_Error_t DSL_DRV_SAR_SegmentationTableGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_SAR_SegmentationTable_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint16_t nArrayIdx = 0, i = 0;
   DSL_SAR_SegmentationTableEntry_t const *pEntry = DSL_NULL;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_CHANNEL_RANGE(pData->nChannel);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_SAR_SegmentationTableGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   if(DSL_DRV_MUTEX_LOCK(pContext->dataMutex))
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - data mutex lock failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERROR;
   }

   /* Get Segmentation Table from the VINAX device context*/
   nArrayIdx = 0;
   pData->data.nNumEntries = pContext->pDevCtx->data.deviceCfg.SegmentationTable.nNumEntries;
   for(nArrayIdx = 0; (nArrayIdx < pData->data.nNumEntries) &&
                      (nArrayIdx < DSL_SAR_MAX_SEG_TABLE_ENTRIES); nArrayIdx++)
   {
      pEntry = pContext->pDevCtx->data.deviceCfg.SegmentationTable.pTableEntry + nArrayIdx;
      for( i = 0; i < 6; i++ )
      {
         pData->data.pTableEntry[nArrayIdx].pMacAddress[i] = pEntry->pMacAddress[i];
      }

      pData->data.pTableEntry[nArrayIdx].nVlanId   = pEntry->nVlanId;
      pData->data.pTableEntry[nArrayIdx].nVlanPrio = pEntry->nVlanPrio;
      pData->data.pTableEntry[nArrayIdx].nVpi      = pEntry->nVpi;
      pData->data.pTableEntry[nArrayIdx].nVci      = pEntry->nVci;
   }

   DSL_DRV_MUTEX_UNLOCK(pContext->dataMutex);

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_SAR_SegmentationTableGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

/*
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_SAR_REASSEMBLY_TABLE_SET
*/
DSL_Error_t DSL_DRV_SAR_ReassemblyTableSet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_SAR_ReassemblyTable_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint16_t nArrayIdx = 0;
   DSL_SAR_ReassemblyTableEntry_t const *pEntry = DSL_NULL;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_CHANNEL_RANGE(pData->nChannel);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_SAR_ReassemblyTableSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Check for the nNumEntries parameter*/
   if ( pData->data.nNumEntries > DSL_SAR_MAX_REA_TABLE_ENTRIES)
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Invalid nNumEntries=%d specified!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), pData->data.nNumEntries));

      return DSL_ERR_PARAM_RANGE;
   }

   if(DSL_DRV_MUTEX_LOCK(pContext->dataMutex))
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - data mutex lock failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERROR;
   }

   nArrayIdx = 0;

   for( nArrayIdx = 0; nArrayIdx < pData->data.nNumEntries; nArrayIdx++)
   {
      pEntry = pData->data.pTableEntry + nArrayIdx;

      if (pEntry->nVci <= 31)
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Invalid nVci(%d)=%d specified!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nArrayIdx, pEntry->nVci));
         nErrCode = DSL_ERR_PARAM_RANGE;
         break;
      }

      if (pEntry->nVpi > 255)
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Invalid nVpi(%d)=%d specified!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nArrayIdx, pEntry->nVpi));
         nErrCode = DSL_ERR_PARAM_RANGE;
         break;
      }

      pContext->pDevCtx->data.deviceCfg.ReassemblyTable.pTableEntry[nArrayIdx].nVci = pEntry->nVci;
      pContext->pDevCtx->data.deviceCfg.ReassemblyTable.pTableEntry[nArrayIdx].nVpi = pEntry->nVpi;
   }

   if (nErrCode == DSL_SUCCESS)
   {
      pContext->pDevCtx->data.deviceCfg.ReassemblyTable.nNumEntries = pData->data.nNumEntries;
   }

   DSL_DRV_MUTEX_UNLOCK(pContext->dataMutex);

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_SAR_ReassemblyTableSet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

/*
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_SAR_REASSEMBLY_TABLE_GET
*/
DSL_Error_t DSL_DRV_SAR_ReassemblyTableGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_SAR_ReassemblyTable_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint16_t nArrayIdx = 0;
   DSL_SAR_ReassemblyTableEntry_t const *pEntry = DSL_NULL;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_CHANNEL_RANGE(pData->nChannel);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_SAR_ReassemblyTableGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   if(DSL_DRV_MUTEX_LOCK(pContext->dataMutex))
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - data mutex lock failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERROR;
   }

   nArrayIdx = 0;
   pData->data.nNumEntries = pContext->pDevCtx->data.deviceCfg.ReassemblyTable.nNumEntries;
   for( nArrayIdx = 0; (nArrayIdx < pData->data.nNumEntries) &&
                       (nArrayIdx < DSL_SAR_MAX_REA_TABLE_ENTRIES); nArrayIdx++)
   {
      pEntry = pContext->pDevCtx->data.deviceCfg.ReassemblyTable.pTableEntry + nArrayIdx;

      pData->data.pTableEntry[nArrayIdx].nVci = pEntry->nVci;
      pData->data.pTableEntry[nArrayIdx].nVpi = pEntry->nVpi;
   }

   DSL_DRV_MUTEX_UNLOCK(pContext->dataMutex);

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_SAR_ReassemblyTableGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

/*
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_SAR_SEGMENTATION_COUNTERS_GET
*/
DSL_Error_t DSL_DRV_SAR_SegmentationCountersGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_SAR_SegmentationCounters_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_LineStateValue_t   nCurrentState = DSL_LINESTATE_UNKNOWN;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_CHANNEL_RANGE(pData->nChannel);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_SAR_SegmentationCountersGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Get current line state*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineState, nCurrentState);

   /* Only proceed if the line is in SHOWTIME or IDLE state.*/
   if ((nCurrentState == DSL_LINESTATE_SHOWTIME_TC_SYNC) ||
       (nCurrentState == DSL_LINESTATE_IDLE))
   {
      /* Call VINAX device specific implementation*/
      nErrCode = DSL_DRV_DEV_SegmentationCountersGet(pContext, &(pData->data));
   }
   else
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Segmentation Counters only available in SHOWTIME or IDLE states!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      nErrCode = DSL_ERROR;
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_SAR_SegmentationCountersGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

/*
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_SAR_REASSEMBLY_COUNTERS_GET
*/
DSL_Error_t DSL_DRV_SAR_ReassemblyCountersGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_SAR_ReassemblyCounters_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_LineStateValue_t   nCurrentState = DSL_LINESTATE_UNKNOWN;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_CHANNEL_RANGE(pData->nChannel);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_SAR_ReassemblyCountersGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Get current line state*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineState, nCurrentState);

   /* Only proceed if the line is in SHOWTIME or IDLE state.*/
   if ((nCurrentState == DSL_LINESTATE_SHOWTIME_TC_SYNC) ||
       (nCurrentState == DSL_LINESTATE_IDLE))
   {
      /* Call VINAX device specific implementation*/
      nErrCode = DSL_DRV_DEV_ReassemblyCountersGet(pContext, &(pData->data));
   }
   else
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Reassembly Counters only available in SHOWTIME or IDLE states!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      nErrCode = DSL_ERROR;
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_SAR_ReassemblyCountersGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

/** \addtogroup DRV_DSL_CPE_SAR
 @{ */


/** @} LIB_DSL_SAR */

#endif /* #ifdef INCLUDE_DSL_CPE_API_SAR_SUPPORT */
