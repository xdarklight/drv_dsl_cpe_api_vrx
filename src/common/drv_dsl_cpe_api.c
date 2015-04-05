/******************************************************************************

                              Copyright (c) 2013
                            Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#define DSL_INTERN

#include "drv_dsl_cpe_api.h"
#include "drv_dsl_cpe_api_ioctl.h"

#ifdef __cplusplus
   extern "C" {
#endif

#undef DSL_DBG_BLOCK
#define DSL_DBG_BLOCK DSL_DBG_CPE_API

/* handles to the low-level BSP driver */
DSL_devCtx_t ifxDevices[DSL_DRV_MAX_ENTITIES] = { {DSL_NULL} };

#if defined(INCLUDE_DSL_BONDING) && (DSL_DRV_LINES_PER_DEVICE == 1)
DSL_DRV_Mutex_t bndLineLockMutex;
DSL_int_t       nLineLocked = -1;
#endif

#ifdef INCLUDE_DSL_DELT
#ifdef DSL_CPE_STATIC_DELT_DATA
   DSL_G997_DeltData_t deltData;
   #ifdef INCLUDE_DSL_DELT_SHOWTIME
   DSL_G997_DeltShowtimeData_t deltShowtimeData;
   #endif /* INCLUDE_DSL_DELT_SHOWTIME*/
#endif /* DSL_CPE_STATIC_DELT_DATA*/
#endif /* INCLUDE_DSL_DELT*/

DSL_OpenContext_t *pOpenContextList = DSL_NULL;

/* Possible XTSE configuration*/
#if defined(INCLUDE_DSL_CPE_API_VRX)
DSL_uint8_t g_XTSEposs[DSL_G997_NUM_XTSE_OCTETS] =
   {0x15, 0x01, 0x14, 0x51, 0x4C, 0x45, 0x05, 0x07};
#else
DSL_uint8_t g_XTSEposs[DSL_G997_NUM_XTSE_OCTETS] =
   {0x17, 0x01, 0x14, 0x51, 0x4C, 0x45, 0x05, 0x00};
#endif

/* G994 Vendor ID */
static const DSL_uint8_t g_G994VendorID[DSL_G997_LI_MAXLEN_VENDOR_ID] = {DSL_G994_VENDOR_ID};

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_EventGenerate(
   DSL_Context_t *pContext,
   DSL_uint8_t nChannel,
   DSL_AccessDir_t nAccessDir,
   DSL_XTUDir_t nXtuDir,
   DSL_EventType_t nEventType,
   DSL_EventData_Union_t *pData,
   DSL_uint16_t nDataSize)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_EventStatusData_t evData;

   DSL_DEBUG( DSL_DBG_MSG,
       (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_EventGenerate"
       DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Generate event*/
   DSL_DRV_MemSet(&evData, 0, sizeof(DSL_EventStatusData_t));
   evData.nEventType = nEventType;
   evData.nXtuDir    = nXtuDir;
   evData.nAccessDir = nAccessDir;
   evData.nChannel   = nChannel;

   if( pData != DSL_NULL && nDataSize != 0)
   {
      evData.pData = (DSL_EventData_Union_t*)DSL_DRV_Malloc(nDataSize);

      if( evData.pData != DSL_NULL)
      {
         memcpy( evData.pData, pData, nDataSize);
      }
   }
   else
   {
      evData.pData = DSL_NULL;
   }

   /* Queue Event*/
   nErrCode = DSL_DRV_EventQueue(pContext, &evData, nDataSize);

   DSL_DEBUG( DSL_DBG_MSG,
       (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_EventGenerate, retCode=%d"
       DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

#if defined(INCLUDE_DSL_G997_STATUS) || defined(INCLUDE_DSL_G997_ALARM) || defined(INCLUDE_DSL_LINIT_STATUS)
/*
   This function hadles an LINIT event.

   \param pContext - Pointer to dsl cpe library context structure, [I]
   \param nLinit   - Specifies the LINIT failure definition, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_HandleLinitValue(
   DSL_Context_t *pContext,
   const DSL_G997_LineInit_t nLinit,
   const DSL_G997_LineInitSubStatus_t nSub)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_G997_LineInitStatusData_t lineInitStatus;
   DSL_uint32_t nCount = 0;

   lineInitStatus.nLineInitStatus    = nLinit;
   lineInitStatus.nLineInitSubStatus = nSub;

   DSL_CTX_WRITE(pContext, nErrCode, lineInitStatus, lineInitStatus);

   if (nLinit != LINIT_SUCCESSFUL)
   {
      /* Get LINIT Retry count*/
      DSL_CTX_READ_SCALAR(pContext, nErrCode, lineInitRetryCount, nCount);

      if( nCount < DSL_G997_LINIT_RETRIES )
      {
         nCount++;
      }

#ifdef INCLUDE_DSL_G997_ALARM
      if( (nCount >= DSL_G997_LINIT_RETRIES) || (nLinit == LINIT_NO_PEER_XTU) )
      {
         nErrCode = DSL_DRV_EventGenerate(
                       pContext, 0, DSL_ACCESSDIR_NA, DSL_XTUDIR_NA,
                       DSL_EVENT_S_LINIT_FAILURE,
                       (DSL_EventData_Union_t*)&lineInitStatus,
                       sizeof(DSL_G997_LineInitStatusData_t));

         if( nErrCode != DSL_SUCCESS )
         {
            DSL_DEBUG( DSL_DBG_ERR,
               (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Event(%d) generate failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext), DSL_EVENT_S_LINIT_FAILURE));
         }
      }
#endif /* INCLUDE_DSL_G997_ALARM*/
   }
   else
   {
      /* Reset LINIT retry counter in case of success */
      nCount = 0;
   }

   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, lineInitRetryCount, nCount);

   return nErrCode;
}
#endif /* defined(INCLUDE_DSL_G997_STATUS) || defined(INCLUDE_DSL_G997_ALARM) || defined(INCLUDE_DSL_LINIT_STATUS)*/

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
#ifdef INCLUDE_DSL_TIMEOUT
DSL_Error_t DSL_DRV_OnTimeoutEvent(
   DSL_Context_t *pContext,
   DSL_int_t nEventType,
   DSL_uint32_t nTimeoutID)
{
   DSL_Error_t errorCode = DSL_SUCCESS;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: DSL_DRV_OnTimeoutEvent: "
      "nEventType=%d, nTimeoutID=0x%08X" DSL_DRV_CRLF ,
      DSL_DEV_NUM(pContext), nEventType, nTimeoutID));

   errorCode = DSL_DRV_DEV_OnTimeoutEvent(pContext,nEventType,nTimeoutID);

   return (errorCode);
}
#endif /* INCLUDE_DSL_TIMEOUT*/

#ifndef INCLUDE_FW_REQUEST_SUPPORT
static DSL_Error_t DSL_DRV_FwCopy(
   DSL_Context_t *pContext,
   DSL_boolean_t bIsInKernel,
   const DSL_uint8_t *pFirmware,
   const DSL_uint32_t nFirmwareSize,
   DSL_uint8_t **ppFirmware,
   DSL_uint32_t *pFirmwareSize)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CHECK_POINTER(pContext, ppFirmware);
   DSL_CHECK_POINTER(pContext, pFirmwareSize);
   DSL_CHECK_ERR_CODE();

#ifdef INCLUDE_DSL_DRV_STATIC_LINKED_FIRMWARE
   /* Statically linked FW supported only under the RTEMS OS build*/
   *ppFirmware    = pFirmware;
   *pFirmwareSize = nFirmwareSize;
#else
   if ( (pFirmware != DSL_NULL) && (nFirmwareSize != 0) )
   {
      if (*ppFirmware != DSL_NULL)
      {
         DSL_DRV_VFree(*ppFirmware);
      }

      *ppFirmware = (DSL_uint8_t*)DSL_DRV_VMalloc(nFirmwareSize);
      if (*ppFirmware != DSL_NULL)
      {
         *pFirmwareSize = nFirmwareSize;
         /* Copy FW binary*/
         DSL_IoctlMemCpyFrom(bIsInKernel, *ppFirmware, (DSL_void_t*)pFirmware,
            nFirmwareSize);
      }
      else
      {
         *pFirmwareSize = 0;
         nErrCode = DSL_ERROR;
         DSL_DEBUG( DSL_DBG_ERR, (pContext,
            SYS_DBG_ERR"DSL[%02d]: Memory allocation for firmware binary failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      }
   }
#endif /* INCLUDE_DSL_DRV_STATIC_LINKED_FIRMWARE*/

   return (nErrCode);
}

/*
   This function stores the FW binaries in the DSL CPE API internal memory

   \param pContext      Pointer to dsl library context structure, [I]
   \param pFirmware1    1st firmware pointer, [I]
   \param nSize1        Size of 1st firmware, [I]
   \param pFirmware2    2nd firmware pointer, [I]
   \param nSize2        Size of 2nd firmware, [I]

   \return  Return values are defined within the \ref DSL_Error_t definition
    - DSL_SUCCESS in case of success
    - DSL_ERROR if operation failed
    - or any other defined specific error code
*/
static DSL_Error_t DSL_DRV_FwStore(
   DSL_Context_t *pContext,
   DSL_boolean_t bIsInKernel,
   const DSL_char_t *pFirmware1,
   DSL_uint32_t nSize1,
   const DSL_char_t *pFirmware2,
   DSL_uint32_t nSize2)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_FwStore"DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext)));

   /* Proceed 1'st FW binary*/
   if (DSL_DRV_FwCopy(pContext, bIsInKernel, (DSL_uint8_t*)pFirmware1, nSize1,
      &pContext->pFirmware, &pContext->nFirmwareSize) != DSL_SUCCESS)
   {
      return DSL_ERROR;
   }

#ifdef INCLUDE_DSL_2ND_FIRMWARE_SUPPORT
   /* Proceed 2'nd FW binary*/
   if( DSL_DRV_FwCopy(pContext, bIsInKernel, (DSL_uint8_t*)pFirmware2, nSize2,
          &pContext->pFirmware2, &pContext->nFirmwareSize2) != DSL_SUCCESS)
   {
      return DSL_ERROR;
   }
#endif /* #ifdef INCLUDE_DSL_2ND_FIRMWARE_SUPPORT*/

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_FwStore"DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext)));

   return nErrCode;
}
#endif /* INCLUDE_FW_REQUEST_SUPPORT*/

static DSL_Error_t DSL_DRV_InstanceStatusGet(
   DSL_OpenContext_t *pOpenCtx,
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_InstanceStatus_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CHECK_CTX_POINTER(pContext);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_InstanceStatusGet"DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext)));

   if(DSL_DRV_MUTEX_LOCK(pOpenCtx->eventMutex))
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]:Couldn't lock event mutex"DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));
      return DSL_ERROR;
   }

   /* Get status data from the instance specific context*/
   pData->data.bEventActivation        = pOpenCtx->bEventActivation;
   pData->data.nResourceActivationMask = pOpenCtx->nResourceActivationMask;

   DSL_DRV_MUTEX_UNLOCK(pOpenCtx->eventMutex);

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_InstanceStatusGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return(nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_HandleInit(
   DSL_IN     DSL_int_t nNum,
   DSL_IN_OUT DSL_OpenContext_t **pRefOpenContext)
{
   if (nNum >= DSL_DRV_MAX_ENTITIES || pRefOpenContext == DSL_NULL)
   {
      return DSL_ERR_INVALID_PARAMETER;
   }

   /* the global context is NULL, initialize it */
   if (ifxDevices[nNum].pContext == DSL_NULL)
   {
      ifxDevices[nNum].pContext = DSL_DRV_VMalloc(sizeof(DSL_Context_t));
      if (ifxDevices[nNum].pContext == DSL_NULL)
      {
         return DSL_ERR_MEMORY;
      }

      DSL_DRV_MemSet(ifxDevices[nNum].pContext, 0, sizeof(DSL_Context_t));

      memcpy(&ifxDevices[nNum].pContext->lineInventoryNe.G994VendorID, g_G994VendorID,
         sizeof(DSL_uint8_t) * DSL_G997_LI_MAXLEN_VENDOR_ID);
      memcpy(&ifxDevices[nNum].pContext->lineInventoryNe.SystemVendorID, g_G994VendorID,
         sizeof(DSL_uint8_t) * DSL_G997_LI_MAXLEN_VENDOR_ID);

      if (ifxDevices[nNum].nUsageCount != 0)
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (DSL_NULL, SYS_DBG_ERR"DSL[%02d]: Internal error in the DSL "
            "CPE API. Uninitialized device context has non-null "
            "usage count."DSL_DRV_CRLF, nNum));
         return DSL_ERR_INTERNAL;
      }

      ifxDevices[nNum].pContext->pDevCtx = &ifxDevices[nNum];
      ifxDevices[nNum].pContext->pXDev = ifxDevices;

      /* Set device number*/
      ifxDevices[nNum].nNum = nNum;
      /* init device mutex */
      DSL_DRV_MUTEX_INIT(ifxDevices[nNum].pContext->bspMutex);

      /* init data mutex */
      DSL_DRV_MUTEX_INIT(ifxDevices[nNum].pContext->dataMutex);

      /* init initialization mutex */
      DSL_DRV_MUTEX_INIT(ifxDevices[nNum].pContext->initMutex);

      /* init Open Context List mutex */
      DSL_DRV_MUTEX_INIT(ifxDevices[nNum].openContextListMutex);

#if defined(INCLUDE_DSL_CPE_API_DANUBE)
      /* init HDLC mutex */
      DSL_DRV_MUTEX_INIT(ifxDevices[nNum].pContext->hdlcMutex);
#endif /* defined(INCLUDE_DSL_CPE_API_DANUBE)*/
#ifdef INCLUDE_DSL_TIMEOUT
      if (DSL_DRV_Timeout_Init(ifxDevices[nNum].pContext, DSL_MAX_TIMEOUT_NUM,
         DSL_FALSE) != DSL_SUCCESS)
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (DSL_NULL, SYS_DBG_ERR"DSL[%02d]: Internal error in the DSL "
            "CPE API. Could not prepare timeout context."DSL_DRV_CRLF, nNum));
         return DSL_ERR_INTERNAL;
      }
#endif /* INCLUDE_DSL_TIMEOUT*/
   }

   if (ifxDevices[nNum].nUsageCount > 0 && ifxDevices[nNum].pContext == DSL_NULL)
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (DSL_NULL, SYS_DBG_ERR"DSL[%02d]: Internal error in the DSL "
         "CPE API. Used device context has null pointer "
         "to DSL Context."DSL_DRV_CRLF, nNum));
      return DSL_ERR_INTERNAL;
   }

   /* Lock Open Context List*/
   if (DSL_DRV_MUTEX_LOCK(ifxDevices[nNum].openContextListMutex))
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (DSL_NULL, SYS_DBG_ERR"DSL[%02d]: ERROR - Couldn't lock Context List mutex!"
         DSL_DRV_CRLF, nNum));

      return DSL_ERR_SEMAPHORE_GET;
   }

   *pRefOpenContext = DSL_DRV_VMalloc(sizeof(DSL_OpenContext_t));
   if (*pRefOpenContext == DSL_NULL)
   {
      DSL_DEBUG(DSL_DBG_ERR, (DSL_NULL, SYS_DBG_ERR"DSL[%02d]: Open failed, "
         "could not allocate memory..."DSL_DRV_CRLF, nNum));

      /* Unlock Open Context List*/
      DSL_DRV_MUTEX_UNLOCK(ifxDevices[nNum].openContextListMutex);

      return DSL_ERR_MEMORY;
   }
   else
   {
      DSL_DRV_MemSet(*pRefOpenContext, 0, sizeof(DSL_OpenContext_t));

      /* init event queue */
      DSL_DRV_INIT_WAKELIST("evntque", (*pRefOpenContext)->eventWaitQueue);

      /* init event mutex */
      DSL_DRV_MUTEX_INIT((*pRefOpenContext)->eventMutex);

      #ifdef INCLUDE_DSL_CEOC
      /* init Rx SNMP FIFO mutex*/
      DSL_DRV_MUTEX_INIT((*pRefOpenContext)->rxSnmpFifoMutex);
      #endif /* #ifdef INCLUDE_DSL_CEOC*/

      /* Disable event handling*/
      (*pRefOpenContext)->bEventActivation = DSL_FALSE;
      /* Mask (disable) all events*/
      (*pRefOpenContext)->nEventMask       = 0xFFFFFFFF;
      /* KAv: Unmask all indication threshold crossing events by default.
         It not completely defined how to handle these events for instance specific
         processing.*/
      (*pRefOpenContext)->nEventMask &=
         ~(DSL_EVENT2MASK(DSL_EVENT_I_LINE_THRESHOLD_CROSSING) |
           DSL_EVENT2MASK(DSL_EVENT_I_CHANNEL_THRESHOLD_CROSSING) |
           DSL_EVENT2MASK(DSL_EVENT_I_DATA_PATH_THRESHOLD_CROSSING) |
           DSL_EVENT2MASK(DSL_EVENT_I_RETX_THRESHOLD_CROSSING) |
           DSL_EVENT2MASK(DSL_EVENT_I_CHANNEL_DATARATE_SHIFT_THRESHOLD_CROSSING));

      (*pRefOpenContext)->nResourceActivationMask = (DSL_BF_ResourceActivationType_t)0xFFFFFFFF;

      /* add to list */
      if (pOpenContextList == DSL_NULL)
      {
         /* first element */
         pOpenContextList = *pRefOpenContext;
      }
      else
      {
         /* add to head */
         (*pRefOpenContext)->pNext = pOpenContextList;
         pOpenContextList = *pRefOpenContext;
      }

      (*pRefOpenContext)->pDevCtx = &ifxDevices[nNum];

      ifxDevices[nNum].nUsageCount++;
   }

   /* Unlock Open Context List*/
   DSL_DRV_MUTEX_UNLOCK(ifxDevices[nNum].openContextListMutex);

#if defined(INCLUDE_DSL_BONDING) && (DSL_DRV_LINES_PER_DEVICE == 1)
   DSL_DRV_MUTEX_INIT(bndLineLockMutex);
#endif

   DSL_DEBUG( DSL_DBG_MSG,
      (DSL_NULL, SYS_DBG_MSG"DSL[%02d]: Open: Use count: %d"DSL_DRV_CRLF,
      nNum, ifxDevices[nNum].nUsageCount));

   DSL_DRV_OS_ModUseCountIncrement();

   return DSL_SUCCESS;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_HandleCleanup(
   DSL_IN DSL_devCtx_t *pDevContext,
   DSL_IN DSL_boolean_t bForce)
{
   if (pDevContext == DSL_NULL)
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (DSL_NULL, SYS_DBG_ERR"DSL: DSL_DRV_HandleCleanup: "
         "Device context is NULL"DSL_DRV_CRLF));
      return DSL_ERROR;
   }

   if (pDevContext->nUsageCount > 1 && bForce == DSL_FALSE)
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (DSL_NULL, SYS_DBG_ERR"DSL[%02d]: DSL_DRV_HandleCleanup: "
         "device is still used by another process"DSL_DRV_CRLF,
         pDevContext->nNum));
      return DSL_ERROR;
   }

   if (pDevContext->nUsageCount > 1 && bForce == DSL_TRUE)
   {
      DSL_DEBUG( DSL_DBG_MSG,
         (DSL_NULL, SYS_DBG_MSG"DSL[%02d]: DSL_DRV_HandleCleanup: "
         "clean up is forced"DSL_DRV_CRLF, pDevContext->nNum));
   }

   if (pDevContext->nUsageCount > 1 || bForce == DSL_TRUE)
   {

      while (pDevContext->nUsageCount-- > 1)
      {
         DSL_DRV_OS_ModUseCountDecrement();
      }

      if (pDevContext->pContext != DSL_NULL)
      {
         DSL_DEBUG( DSL_DBG_MSG,
            (DSL_NULL, SYS_DBG_MSG"DSL[%02d]: DSL_DRV_HandleCleanup: "
            "Autoboot thread stopping..."DSL_DRV_CRLF,
            pDevContext->nNum));

         DSL_DRV_AutobootThreadStop(pDevContext->pContext);

         DSL_DEBUG( DSL_DBG_MSG,
            (DSL_NULL, SYS_DBG_MSG"DSL[%02d]: DSL_DRV_HandleCleanup: "
            "Autoboot thread has been stopped..."DSL_DRV_CRLF,
            pDevContext->nNum));

         DSL_DRV_Free(pDevContext->pContext);
         DSL_DRV_VFree(pDevContext->pContext);
         pDevContext->pContext = DSL_NULL;
      }
   }

   return DSL_SUCCESS;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_HandleDelete(
   DSL_IN DSL_OpenContext_t *pOpenContext)
{
   DSL_devCtx_t *pDevContext;
   DSL_EventStatusData_t event;
   DSL_OpenContext_t *pPrev, *pCurr;
   DSL_void_t *pPtr;
   DSL_Error_t nErrCode = DSL_SUCCESS;

   if (pOpenContext == DSL_NULL)
   {
      return DSL_ERR_INVALID_PARAMETER;
   }
   else
   {
      pDevContext = pOpenContext->pDevCtx;

      if (pDevContext == DSL_NULL)
      {
         return DSL_ERROR;
      }
      else
      {
         if (pDevContext->nUsageCount > 0)
         {
            pDevContext->nUsageCount--;
         }

         /* Lock Open Context list mutex*/
         if (DSL_DRV_MUTEX_LOCK(pDevContext->openContextListMutex))
         {
            DSL_DEBUG( DSL_DBG_ERR,
               (DSL_NULL, SYS_DBG_ERR"DSL[%02d]: ERROR - Couldn't lock Context List mutex!"
               DSL_DRV_CRLF, pDevContext->nNum));

            return DSL_ERR_SEMAPHORE_GET;
         }

         if ((pOpenContext->nEventMask & DSL_EVENT2MASK(DSL_EVENT_S_FIRMWARE_REQUEST)) == 0
            && pOpenContext->pDevCtx != DSL_NULL
            && pOpenContext->pDevCtx->pContext != DSL_NULL)
         {
            DSL_CTX_WRITE_SCALAR(pOpenContext->pDevCtx->pContext,
               nErrCode, bFirmwareEventAssigned, DSL_FALSE);
         }

         if (pOpenContext->eventFifo != DSL_NULL
            && pOpenContext->eventFifoBuf != DSL_NULL)
         {
            while (!DSL_Fifo_isEmpty(pOpenContext->eventFifo))
            {
               if (DSL_DRV_EventUnqueue(pOpenContext, &event)
                  >= DSL_SUCCESS)
               {
                  if (event.pData != DSL_NULL)
                  {
                     DSL_DRV_VFree(event.pData);
                  }
               }
            }
         }

         /* release event structures */
         if (pOpenContext->eventFifo != DSL_NULL)
         {
            pPtr = pOpenContext->eventFifo;
            pOpenContext->eventFifo = DSL_NULL;

            DSL_DRV_VFree(pPtr);
         }

         /* release event structures */
         if (pOpenContext->eventFifoBuf != DSL_NULL)
         {
            pPtr = pOpenContext->eventFifoBuf;
            pOpenContext->eventFifoBuf = DSL_NULL;
            DSL_DRV_VFree(pPtr);
         }

         /* remove from list */
         if (pOpenContextList != DSL_NULL)
         {
            if (pOpenContext == pOpenContextList)
            {
               /* it is a head */
               pOpenContextList = pOpenContext->pNext;
            }
            else
            {
               for ( pPrev = pOpenContextList,
                     pCurr = pOpenContextList->pNext;
                     pCurr != pOpenContext && pCurr != DSL_NULL;
                     pPrev = pCurr,
                     pCurr = pCurr->pNext) ;

               if (pCurr != DSL_NULL)
               {
                  pPrev->pNext = pCurr->pNext;
               }
            }
         }

         DSL_DRV_VFree(pOpenContext);

         /* Unlock Open Context list mutex*/
         DSL_DRV_MUTEX_UNLOCK(pDevContext->openContextListMutex);
      }
   }

   DSL_DEBUG( DSL_DBG_MSG,
      (DSL_NULL, SYS_DBG_MSG"DSL[%02d]: Close: Use count: %d, retCode=%d"
      DSL_DRV_CRLF, pDevContext->nNum, pDevContext->nUsageCount, nErrCode));

   DSL_DRV_OS_ModUseCountDecrement();

   return nErrCode;
}

DSL_void_t DSL_DRV_Cleanup(DSL_void_t)
{
   DSL_int_t i;

   for (i = 0; i < DSL_DRV_MAX_ENTITIES; i++)
   {
      DSL_DRV_HandleCleanup(&ifxDevices[i], DSL_TRUE);

      DSL_DRV_DEV_DriverHandleDelete(ifxDevices[i].lowHandle);

#ifdef INCLUDE_DSL_NFC_HANDLE
      DSL_DRV_DEV_DriverHandleDelete(ifxDevices[i].nfc_lowHandle);
#endif /* INCLUDE_DSL_NFC_HANDLE*/

      /* this is redundant code but in case of some our
         threads leave active it will prevent momentary fault */
      ifxDevices[i].lowHandle = DSL_NULL;
#ifdef INCLUDE_DSL_NFC_HANDLE
      ifxDevices[i].nfc_lowHandle = DSL_NULL;
#endif /* INCLUDE_DSL_NFC_HANDLE*/
      ifxDevices[i].nUsageCount = 0;
   }

   while (pOpenContextList != DSL_NULL)
   {
      DSL_DRV_HandleDelete(pOpenContextList);
   }
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_LinkReset(
   DSL_IN DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_LinkReset"DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext)));

   nErrCode = DSL_DRV_DEV_LinkReset(pContext);
   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: Modem couldn't be reset"DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));
   }
   else
   {
      DSL_DEBUG( DSL_DBG_MSG,
         (pContext, SYS_DBG_MSG"DSL[%02d]: Modem was reseted..."DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));

      nErrCode = DSL_SUCCESS;
   }

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_LinkReset"DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext)));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_LinkFreeze(
   DSL_IN DSL_Context_t *pContext,
   DSL_uint8_t *pXTSE)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_LinkFreeze"DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext)));

   nErrCode = DSL_DRV_DEV_LinkFreeze(pContext, pXTSE);
   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: Modem couldn't be freeze"DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));
   }
   else
   {
      DSL_DEBUG( DSL_DBG_MSG,
         (pContext, SYS_DBG_MSG"DSL[%02d]: Modem was frozen..."DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));

      nErrCode = DSL_SUCCESS;
   }

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_LinkFreeze"DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext)));

   return nErrCode;
}

DSL_Error_t DSL_DRV_ModulesInit(
   DSL_IN DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_boolean_t bFirmwareReady = DSL_FALSE;

   DSL_CHECK_CTX_POINTER(pContext);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_ModulesInit"DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext)));

   DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->bFirmwareReady, bFirmwareReady);

   /* Check FW availability*/
   if( !bFirmwareReady )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW not loaded yet!"DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));

      return DSL_ERROR;
   }

   /* Lock DSL CPE API data mutex*/
   if(DSL_DRV_MUTEX_LOCK(pContext->initMutex))
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: Couldn't lock data mutex"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERROR;
   }

#if defined(INCLUDE_DSL_ADSL_MIB)
   if ( DSL_DRV_MIB_ModuleInit(pContext) != DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - MIB module initialization failed"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      nErrCode = DSL_ERROR;
   }
#endif /* defined(INCLUDE_DSL_ADSL_MIB)*/

#if defined(INCLUDE_DSL_CEOC)
   if ( DSL_CEOC_Start(pContext) != DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - CEOC module initialization failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      nErrCode = DSL_ERROR;
   }
#endif /* defined(INCLUDE_DSL_CEOC)*/

#if defined(INCLUDE_DSL_PM)
   if ( DSL_DRV_PM_Start(pContext) != DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - PM module initialization failed"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      nErrCode = DSL_ERROR;
   }
#endif /* defined(INCLUDE_DSL_PM)*/

   /* Unock DSL CPE API data mutex*/
   DSL_DRV_MUTEX_UNLOCK(pContext->initMutex);

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_ModulesInit"DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext)));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_XtseSettingsCheck(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN DSL_uint8_t *pXTSE)
{
   DSL_Error_t nErrCode = DSL_SUCCESS, nRet = DSL_SUCCESS;
   DSL_int32_t i = 0, emptyXseCnt = 0;
   DSL_uint8_t xtsePoss = 0;
   DSL_boolean_t bXtseChanged = DSL_FALSE, bXtseUnsupported = DSL_FALSE;

   DSL_CHECK_POINTER(pContext, pXTSE);
   DSL_CHECK_ERR_CODE();

   /* Validation checks for configuration data */
   for (i = 0; i < DSL_G997_NUM_XTSE_OCTETS; i++)
   {
      DSL_CTX_READ(pContext, nErrCode, xtsePoss[i], xtsePoss);

      /* Check for unsupported bits*/
      if ( (~(xtsePoss) & pXTSE[i]) > 0)
      {
         /* Check if there is at least one valid bit*/
         if ((pXTSE[i] & xtsePoss) > 0)
         {
            /* Change specified configuration*/
            pXTSE[i] &= xtsePoss;

            bXtseChanged = DSL_TRUE;
         }
         else
         {
            /* No valid bits found at all*/
            bXtseUnsupported = DSL_TRUE;
         }
      }

      if (!pXTSE[i])
      {
         emptyXseCnt++;
      }
   }

   /* Check if no valid bits specified at all*/
   if (emptyXseCnt >= DSL_G997_NUM_XTSE_OCTETS)
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Empty XTSE settings, emptyXseCnt=%d!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), emptyXseCnt));

      nErrCode = DSL_ERROR;
   }
   else
   {
      if (bXtseUnsupported && !bXtseChanged)
      {
         /* There are bits set within the configuration that are not supported
            by G.997.1 or at least by this version of the DSL library */
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - No valid XTSE settings!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         nErrCode = DSL_ERROR;
      }
      else if (bXtseChanged)
      {
         DSL_DEBUG(DSL_DBG_WRN,
            (pContext, SYS_DBG_WRN"DSL[%02d]: WARNING - Unsupported bits removed from the current XTSE settings!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         nErrCode = DSL_WRN_INCONSISTENT_XTSE_CONFIGURATION;
      }
   }

   if (nErrCode >= DSL_SUCCESS)
   {
      /* Check device specific XTSE settings*/
      nRet = DSL_DRV_DEV_XtseSettingsCheck(pContext, pXTSE);
      if (nRet != DSL_SUCCESS)
      {
         nErrCode = nRet;
      }
   }

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_Init(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_Init_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_int32_t nLoff = 0, nCurrOffset = 0;
   DSL_boolean_t bFirmwareReady = DSL_FALSE;
#ifdef INCLUDE_FW_REQUEST_SUPPORT
   DSL_FirmwareRequestData_t fwReqData = { DSL_FW_REQUEST_NA };
   DSL_boolean_t bFwRequest = DSL_FALSE;
#endif /* INCLUDE_FW_REQUEST_SUPPORT*/
   DSL_FirmwareRequestType_t nFwType = DSL_FW_REQUEST_NA;
#ifndef DSL_DEBUG_DISABLE
   #if defined(INCLUDE_DSL_CPE_API_VRX)
      DSL_DBG_ModuleLevel_t dbgModLev;
   #endif /* #ifndef DSL_DEBUG_DISABLE*/
#endif /* INCLUDE_DSL_CPE_API_VRX */

   DSL_CHECK_CTX_POINTER(pContext);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_POINTER(pContext, pContext->pDevCtx);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_Init"DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext)));

   if (pContext->bInitComplete == DSL_TRUE)
   {
      DSL_DEBUG( DSL_DBG_WRN,
         (pContext, SYS_DBG_WRN"DSL[%02d]: DSL CPE API is already initialized!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_WRN_ALREADY_INITIALIZED;
   }

#ifndef DSL_DEBUG_DISABLE
   #if defined(INCLUDE_DSL_CPE_API_VRX)
      /* Take care that initial value for low level (MEI) driver are configured */
      dbgModLev.data.nDbgModule = DSL_DBG_LOW_LEVEL_DRIVER;
      dbgModLev.data.nDbgLevel = DSL_g_dbgLvl[DSL_DBG_LOW_LEVEL_DRIVER].nDbgLvl;
      DSL_DRV_DBG_ModuleLevelSet(pContext, &dbgModLev);

      dbgModLev.data.nDbgModule = DSL_DBG_MESSAGE_DUMP;
      dbgModLev.data.nDbgLevel = DSL_g_dbgLvl[DSL_DBG_MESSAGE_DUMP].nDbgLvl;
      DSL_DRV_DBG_ModuleLevelSet(pContext, &dbgModLev);
   #endif /* #ifndef DSL_DEBUG_DISABLE*/
#endif /* INCLUDE_DSL_CPE_API_VRX */


#ifdef INCLUDE_DSL_DELT
#ifdef DSL_CPE_STATIC_DELT_DATA
   DSL_DRV_MemSet(&deltData, 0, sizeof(DSL_G997_DeltData_t));
   pContext->DELT = &deltData;
   #ifdef INCLUDE_DSL_DELT_SHOWTIME
   DSL_DRV_MemSet(&deltShowtimeData, 0, sizeof(DSL_G997_DeltShowtimeData_t));
   pContext->DELT_SHOWTIME = &deltShowtimeData;
   #endif /* INCLUDE_DSL_DELT_SHOWTIME*/
#else
   pContext->DELT = DSL_NULL;
   #ifdef INCLUDE_DSL_DELT_SHOWTIME
   pContext->DELT_SHOWTIME = DSL_NULL;
   #endif /* INCLUDE_DSL_DELT_SHOWTIME*/
#endif
#endif /* #ifdef INCLUDE_DSL_DELT*/

#if defined (INCLUDE_DSL_CPE_API_DANUBE)
   /* Set platform ID */
   pContext->pDevCtx->data.version.fwVersion.nMajorVersion =
        pData->data.nFirmwareFeatures.nPlatformId;
   DSL_DEBUG(DSL_DBG_MSG, (pContext, SYS_DBG_MSG
      "DSL[%02d]: Initial platform ID (FW major version): %d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
      pContext->pDevCtx->data.version.fwVersion.nMajorVersion));
#endif

   /* Initialize possible XTSE configurations*/
   memcpy(pContext->xtsePoss, g_XTSEposs, DSL_G997_NUM_XTSE_OCTETS);

   /* Initialize SystemVendorId */
   if (pData->data.pInventory != DSL_NULL)
   {
      memcpy(pContext->lineInventoryNe.SystemVendorID, &pData->data.pInventory->SystemVendorID,
         sizeof(DSL_uint8_t) * DSL_G997_LI_MAXLEN_VENDOR_ID);
   }

   /* Check XTSE setings*/
   nErrCode = DSL_DRV_XtseSettingsCheck(pContext, pData->data.nXtseCfg.XTSE);
   if (nErrCode < DSL_SUCCESS)
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Wrong XTSE settings!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      DSL_DRV_MemSet(pData->data.nXtseCfg.XTSE, 0x0, sizeof(pData->data.nXtseCfg.XTSE));
   }

   if(DSL_DRV_MUTEX_LOCK(pContext->dataMutex))
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: Couldn't lock data mutex"DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));
      return DSL_ERROR;
   }

   /* Set XTSE configuration*/
   memcpy( pContext->xtseCfg, pData->data.nXtseCfg.XTSE, sizeof(pContext->xtseCfg) );

   /* Get Test Mode Control options and copy them to the DSL CPE context*/
   memcpy(&(pContext->nTestModeControl),
          &(pData->data.nTestModeControl), sizeof(DSL_TestModeControlSet_t));

   /* Get Line Activate Control options and copy them to the DSL CPE context*/
   memcpy(&(pContext->lineActivateConfig),
          &(pData->data.nLineActivateConfig), sizeof(DSL_G997_LineActivateData_t));

   /* Set SerialNumber*/
   memcpy(
      pContext->lineInventoryNe.SerialNumber,
      pData->data.pInventory->SerialNumber,
      DSL_G997_LI_MAXLEN_SERIAL);

   /* Set VersionNumber*/
   memcpy(
      pContext->lineInventoryNe.VersionNumber,
      pData->data.pInventory->VersionNumber,
      DSL_G997_LI_MAXLEN_VERSION);
#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
   /* Set VersionNumber*/
   memcpy(
      &pContext->auxInventoryNe,
      &(pData->data.pInventory->Auxiliary),
      sizeof(DSL_AuxInventoryNe_t));
#endif

   /* Set Autoboot configuration options*/
   pContext->nAutobootStartupMode = pData->data.nAutobootStartupMode;
   memcpy(
      &(pContext->nAutobootConfig),
      &(pData->data.nAutobootConfig),
      sizeof(DSL_AutobootConfigData_t));

   /* *********************************************************************** */
   /*              Initiliatize API default configurations                    */
   /* *********************************************************************** */
   /* Note: Values with default value zero/false does not need to be set
            necessarily because the complete pContext structure is initialized
            with zero at the beginning. */
   pContext->lineFeatureDataCfg[DSL_UPSTREAM].bTrellisEnable = DSL_TRUE;
   pContext->lineFeatureDataCfg[DSL_DOWNSTREAM].bTrellisEnable = DSL_TRUE;

   pContext->lineFeatureDataCfg[DSL_UPSTREAM].bBitswapEnable = DSL_TRUE;
   pContext->lineFeatureDataCfg[DSL_DOWNSTREAM].bBitswapEnable = DSL_TRUE;

   /* ReTx is only valid for downstream direction */
   pContext->lineFeatureDataCfg[DSL_DOWNSTREAM].bReTxEnable = DSL_TRUE;

#ifdef INCLUDE_DSL_CPE_API_VRX
   /* Virtual Noise support is only enabled for VRX */
   pContext->lineFeatureDataCfg[DSL_UPSTREAM].bVirtualNoiseSupport = DSL_TRUE;
   pContext->lineFeatureDataCfg[DSL_DOWNSTREAM].bVirtualNoiseSupport = DSL_TRUE;
#endif /* INCLUDE_DSL_CPE_API_VRX*/

   pContext->lineFeatureDataCfg[DSL_UPSTREAM].b20BitSupport = DSL_FEATURE_NA;
   pContext->lineFeatureDataCfg[DSL_DOWNSTREAM].b20BitSupport = DSL_FEATURE_DISABLED;

   pContext->powerMgmtStatus.nPowerManagementStatus = DSL_G997_PMS_L3;

   /* Set default reboot criteria*/
   pContext->RebootCriteriaNeConfigData =
      DSL_REBOOT_CRITERIA_ESE | DSL_REBOOT_CRITERIA_LOM | DSL_REBOOT_CRITERIA_LOF |
      DSL_REBOOT_CRITERIA_LOS | DSL_REBOOT_CRITERIA_NEGATIVE_MARGIN;

   /* Enhanced upstream framing shall be enabled by default */
   pContext->lineOptionsConfig[DSL_ENHANCED_FRAMING_US] = (DSL_int32_t)DSL_TRUE;

#ifdef INCLUDE_DSL_CPE_API_VRX
   /* Set initialization default power mode*/
   pContext->PMMode = DSL_G997_PMMODE_BIT_L3_STATE;
#endif /* INCLUDE_DSL_CPE_API_VRX*/

   /* *********************************************************************** */

   DSL_DRV_MUTEX_UNLOCK(pContext->dataMutex);

   /* Initialize device specific parameters*/
   nErrCode = DSL_DRV_DEV_DeviceInit(pContext, pData);
   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Device Init failed!"DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   memcpy(&(pContext->nFwFeatures), &(pData->data.nFirmwareFeatures),
      sizeof(DSL_FirmwareFeatures_t));

   /* Download FW if available and Start Autoboot handling.
       If no FW specified, do it later with appropriate IOCTLs*/
   if( pData->data.pFirmware != DSL_NULL || pData->data.pFirmware2 != DSL_NULL )
   {
      /* Load FW if available*/
      nErrCode = DSL_DRV_FwDownload(
                    pContext,
                    (DSL_char_t*)pData->data.pFirmware,
                    pData->data.nFirmwareSize,
                    (DSL_char_t*)pData->data.pFirmware2,
                    pData->data.nFirmwareSize2,
                    &nLoff, &nCurrOffset,
                    DSL_TRUE);

      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW Download failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         return nErrCode;
      }

      /* Get FW ready flag*/
      DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->bFirmwareReady, bFirmwareReady);

      /* Here the complete FW binary should be downloaded to the device, no chunks allowed*/
      if(!bFirmwareReady)
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Expecting complete FW binary!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return DSL_ERROR;
      }

      nErrCode = DSL_DRV_DEV_G994VendorIdFirmwareUpdate(pContext);
      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - could not initialize G994 Vendor ID!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return nErrCode;
      }

      /* Start Autoboot handling if allowed*/
      if( pData->data.nAutobootStartupMode != DSL_AUTOBOOT_CTRL_STOP )
      {
         if( DSL_DRV_AutobootThreadStart(pContext) != DSL_SUCCESS )
         {
            DSL_DEBUG( DSL_DBG_ERR,
               (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Autoboot thread start failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            return DSL_ERROR;
         }
      }

      /* Initialize additional modules PM, CEOC,...*/
      nErrCode = DSL_DRV_ModulesInit(pContext);
      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - additional modules init failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return nErrCode;
      }
   }
   else
   {
#if defined(INCLUDE_DSL_CPE_API_VRX)
      nFwType = DSL_FW_REQUEST_XDSL;
#else
      nFwType = DSL_FW_REQUEST_ADSL;
#endif /* defined(INCLUDE_DSL_CPE_API_VRX) */
#ifdef INCLUDE_FW_REQUEST_SUPPORT
      bFwRequest = DSL_TRUE;
#endif /* INCLUDE_FW_REQUEST_SUPPORT*/
      /* Set pending flag to start automatically Autoboot handling if allowed*/
      if( pData->data.nAutobootStartupMode != DSL_AUTOBOOT_CTRL_STOP )
      {
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, bAutobootStartPending, DSL_TRUE);
      }

      /* Set Autoboot Status*/
      nErrCode = DSL_DRV_AutobootStatusSet(pContext, DSL_AUTOBOOT_STATUS_FW_WAIT, nFwType);
      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Autoboot Status set failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return nErrCode;
      }
   }

   /* Set LINIT value to LINIT_UNKNOWN*/
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, lineInitStatus.nLineInitStatus, LINIT_UNKNOWN);

   pContext->bInitComplete = DSL_TRUE;

   /* Generate DSL_EVENT_S_INIT_READY event*/
   DSL_DRV_EventGenerate(pContext, 0, DSL_ACCESSDIR_NA, DSL_XTUDIR_NA,
      DSL_EVENT_S_INIT_READY, DSL_NULL, 0);

#ifdef INCLUDE_FW_REQUEST_SUPPORT
   if (bFwRequest)
   {
#if defined(INCLUDE_DSL_CPE_API_VRX)
      DSL_PortMode_t nPortMode;

      /* get port mode type*/
      DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->data.nPortMode, nPortMode);
      fwReqData.nPortMode = nPortMode;
#endif

      fwReqData.nFirmwareRequestType = nFwType;

      /* Generate DSL_EVENT_S_FIRMWARE_REQUEST event*/
      DSL_DRV_EventGenerate( pContext, 0, DSL_ACCESSDIR_NA,
         DSL_XTUDIR_NA, DSL_EVENT_S_FIRMWARE_REQUEST,
         (DSL_EventData_Union_t*)&fwReqData, sizeof(DSL_FirmwareRequestData_t));
   }
#endif /* INCLUDE_FW_REQUEST_SUPPORT*/

   nErrCode = DSL_DRV_HybridContextReset(pContext);
   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG(DSL_DBG_ERR, (pContext, "DSL[%02d]: ERROR - HybridContextReset "
         "init failed!"DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      /* Continue with activation sequence anyhow */
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_Init, retCode=%d"DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext) ,nErrCode));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_void_t DSL_DRV_Free(
   DSL_IN DSL_Context_t *pContext)
{
   if (pContext == DSL_NULL)
   {
      DSL_DEBUG(DSL_DBG_ERR, (pContext, SYS_DBG_ERR"Invalid context pointer!"DSL_DRV_CRLF));
      return;
   }

   if (pContext->pFirmware != DSL_NULL)
   {
      DSL_DRV_VFree(pContext->pFirmware);
      pContext->pFirmware = DSL_NULL;
   }

   if (pContext->pFirmware2 != DSL_NULL)
   {
      DSL_DRV_VFree(pContext->pFirmware2);
      pContext->pFirmware2 = DSL_NULL;
   }

   pContext->bInitComplete = DSL_FALSE;

#ifdef INCLUDE_DSL_TIMEOUT
   DSL_DRV_Timeout_Shutdown(pContext);
#endif /* INCLUDE_DSL_TIMEOUT*/

#ifdef INCLUDE_DSL_PM
   /* Stop PM module processing*/
   if( DSL_DRV_PM_Stop(pContext) != DSL_SUCCESS )
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: PM Module deinitialization failed"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }
#endif /* #ifdef INCLUDE_DSL_PM*/

#ifdef INCLUDE_DSL_CEOC
   /* Stop CEOC module processing*/
   if( DSL_CEOC_Stop(pContext) != DSL_SUCCESS )
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: CEOC Module deinitialization failed"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }
#endif /* #ifdef INCLUDE_DSL_CEOC*/

#ifdef INCLUDE_DSL_DELT
#ifndef DSL_CPE_STATIC_DELT_DATA
   /* Release DELT memory */
   if (pContext->DELT != DSL_NULL)
   {
      DSL_DRV_MemFree(pContext->DELT);
   }
   #ifdef INCLUDE_DSL_DELT_SHOWTIME
   if (pContext->DELT_SHOWTIME != DSL_NULL)
   {
      DSL_DRV_MemFree(pContext->DELT_SHOWTIME);
   }
   #endif /* INCLUDE_DSL_DELT_SHOWTIME*/
#endif /* DSL_CPE_STATIC_DELT_DATA*/
#endif /* INCLUDE_DSL_DELT*/
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_AutobootLoadFirmware(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_AutobootLoadFirmware_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_int32_t nOffset = 0;
   DSL_boolean_t bFirmwareReady = DSL_FALSE;
   DSL_boolean_t bAutobootThreadStarted = DSL_FALSE,
                 bAutobootFwLoadPending = DSL_FALSE,
                 bAutobootStartPending  = DSL_FALSE;
   DSL_AutobootControl_t AutobootCtrl;
   DSL_int32_t nWaitCount = 0;
   DSL_Autoboot_State_t nState = DSL_AUTOBOOTSTATE_UNKNOWN;
   DSL_uint8_t *pFirmware = DSL_NULL, *pFirmware2 = DSL_NULL;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_POINTER(pContext, pContext->pDevCtx);
   DSL_CHECK_ERR_CODE();

   DSL_CTX_READ_SCALAR(pContext, nErrCode, bAutobootThreadStarted, bAutobootThreadStarted);
   DSL_CTX_READ_SCALAR(pContext, nErrCode, bAutobootFwLoadPending, bAutobootFwLoadPending);

   if( (pData->data.pFirmware  != DSL_NULL && pData->data.nFirmwareSize) ||
       (pData->data.pFirmware2 != DSL_NULL && pData->data.nFirmwareSize2) )
   {
      DSL_DEBUG(DSL_DBG_MSG,
         (pContext, SYS_DBG_MSG"DSL[%02d]: Preparing to download FW..."
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      DSL_CTX_WRITE(pContext, nErrCode, nFwFeatures, pData->data.firmwareFeatures);
   }
   else
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - No FW specified!"DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));

      return DSL_ERROR;
   }

   if( bAutobootThreadStarted && !bAutobootFwLoadPending)
   {
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, bAutobootFwLoadPending, DSL_TRUE);

      /* Get Autoboot State*/
      DSL_CTX_READ_SCALAR(pContext, nErrCode, nAutobootState, nState);

      if( nState != DSL_AUTOBOOTSTATE_FIRMWARE_WAIT )
      {
         /* Restart Autoboot*/
         AutobootCtrl.data.nCommand = DSL_AUTOBOOT_CTRL_RESTART;
         nErrCode = DSL_DRV_AutobootControlSet(pContext,&AutobootCtrl);
         if( nErrCode != DSL_SUCCESS )
         {
            /* Clear bAutobootFwLoadPending flag*/
            DSL_CTX_WRITE_SCALAR(pContext, nErrCode, bAutobootFwLoadPending, DSL_FALSE);

            DSL_DEBUG(DSL_DBG_ERR,
               (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Autoboot Control set failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
            return nErrCode;
         }
      }

      /* wait for "DSL_AUTOBOOTSTATE_FIRMWARE_WAIT" max 4 sec */
      for (nWaitCount = 0; nWaitCount < 40; nWaitCount++)
      {
         /* Get Autoboot State*/
         DSL_CTX_READ_SCALAR(pContext, nErrCode, nAutobootState, nState);
         if( nErrCode != DSL_SUCCESS )
         {
            /* Clear bAutobootFwLoadPending flag*/
            DSL_CTX_WRITE_SCALAR(pContext, nErrCode, bAutobootFwLoadPending, DSL_FALSE);

            DSL_DEBUG(DSL_DBG_ERR,
               (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Autoboot State get failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
            return nErrCode;
         }

         if( nState == DSL_AUTOBOOTSTATE_FIRMWARE_WAIT )
         {
            break;
         }

         DSL_DRV_MSecSleep(100);
      }

      if( nState != DSL_AUTOBOOTSTATE_FIRMWARE_WAIT )
      {
         /* Clear bAutobootFwLoadPending flag*/
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, bAutobootFwLoadPending, DSL_FALSE);
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Timeout while waiting the "
            "DSL_AUTOBOOTSTATE_FIRMWARE_WAIT state!"DSL_DRV_CRLF,
            DSL_DEV_NUM(pContext)));
         return DSL_ERROR;
      }
   }

   /* Deactivate Autoboot timeout for the Firmware Wait state*/
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, nAutobootTimeoutLimit, 1800);

   /* Copy FW binary to the kernel space*/
   if (pData->data.pFirmware != DSL_NULL)
   {
      pFirmware = (DSL_uint8_t*)DSL_DRV_VMalloc(pData->data.nFirmwareSize);

      if (pFirmware != DSL_NULL)
      {
         DSL_IoctlMemCpyFrom(
            DSL_FALSE, pFirmware, pData->data.pFirmware, pData->data.nFirmwareSize);
      }
      else
      {
         pFirmware = DSL_NULL;
         nErrCode = DSL_ERROR;
         DSL_DEBUG( DSL_DBG_ERR, (pContext,
            SYS_DBG_ERR"DSL[%02d]: ERROR - Memory allocation for firmware binary 1 failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      }
   }

   /* Copy FW binary 2 to the kernel space*/
   if ((pData->data.pFirmware2 != DSL_NULL) && (nErrCode == DSL_SUCCESS))
   {
      pFirmware2 = (DSL_uint8_t*)DSL_DRV_VMalloc(pData->data.nFirmwareSize2);

      if (pFirmware2 != DSL_NULL)
      {
         DSL_IoctlMemCpyFrom(
            DSL_FALSE, pFirmware2, pData->data.pFirmware2, pData->data.nFirmwareSize2);
      }
      else
      {
         pFirmware2 = DSL_NULL;
         nErrCode = DSL_ERROR;
         DSL_DEBUG( DSL_DBG_ERR, (pContext,
            SYS_DBG_ERR"DSL[%02d]: ERROR - Memory allocation for firmware binary 2 failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      }
   }

   if (nErrCode == DSL_SUCCESS)
   {
#if defined(INCLUDE_DSL_CPE_API_VRX)
      /* Download FW*/
      nErrCode = DSL_DRV_FwDownload(
                    pContext,
                    (DSL_char_t*)pFirmware, pData->data.nFirmwareSize,
                    (DSL_char_t*)pFirmware2, pData->data.nFirmwareSize2,
                    (DSL_int32_t*)&pData->data.nFirmwareOffset, &nOffset,
                    pData->data.bLastChunk);
#else
      /*
      KAv: If FW binary data is in kernel space, the Amazon-Se driver FW
           sequence crashes. This issue needs further clarification.
      */
      /* Download FW*/
      nErrCode = DSL_DRV_FwDownload(
                    pContext,
                    (DSL_char_t*)pData->data.pFirmware, pData->data.nFirmwareSize,
                    (DSL_char_t*)pData->data.pFirmware2, pData->data.nFirmwareSize2,
                    (DSL_int32_t*)&pData->data.nFirmwareOffset, &nOffset,
                    pData->data.bLastChunk);
#endif
   }

   if (pFirmware != DSL_NULL)
   {
      /* Free allocated FW binary 1 resources*/
      DSL_DRV_VFree(pFirmware);
   }

   if (pFirmware2 != DSL_NULL)
   {
      /* Free allocated FW binary 2 resources*/
      DSL_DRV_VFree(pFirmware2);
   }

   if( nErrCode >= DSL_SUCCESS )
   {
      /* Get bFirmwareReady flag*/
      DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->bFirmwareReady, bFirmwareReady);
      if( bFirmwareReady )
      {
         nErrCode = DSL_DRV_DEV_G994VendorIdFirmwareUpdate(pContext);
         if( nErrCode != DSL_SUCCESS )
         {
            DSL_DEBUG( DSL_DBG_ERR,
               (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - could not initialize G994 Vendor ID!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            return nErrCode;
         }

         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, bAutobootFwLoadPending, DSL_FALSE);
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, bFwRequestHandled, DSL_TRUE);

#ifndef INCLUDE_FW_REQUEST_SUPPORT
         /* Check if the Chunk download is disabled*/
         if (!(pData->data.bChunkDonwloadEnabled))
         {
            /* Store FW binaries*/
            nErrCode = DSL_DRV_FwStore(
                       pContext, DSL_FALSE,
                       (DSL_char_t*)pData->data.pFirmware,  pData->data.nFirmwareSize,
                       (DSL_char_t*)pData->data.pFirmware2, pData->data.nFirmwareSize2);
            if (nErrCode != DSL_SUCCESS)
            {
               DSL_DEBUG(DSL_DBG_ERR,
                 (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW store failed!"
                 DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
            }
         }
#endif /* INCLUDE_FW_REQUEST_SUPPORT*/

         /* Get Autoboot pending start flag*/
         DSL_CTX_READ(pContext, nErrCode, bAutobootStartPending, bAutobootStartPending);

         /* Check if there is a pending Autoboot start*/
         if (bAutobootStartPending)
         {
            /* Reset Autoboot pending start flag*/
            DSL_CTX_WRITE_SCALAR(pContext, nErrCode, bAutobootStartPending, DSL_FALSE);

            nErrCode = DSL_DRV_AutobootThreadStart(pContext);
            if (nErrCode != DSL_SUCCESS)
            {
               DSL_DEBUG(DSL_DBG_ERR,
                 (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Autoboot Thread pending start failed!"
                 DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

               return nErrCode;
            }

            /* Initialize additional modules PM, CEOC,...*/
            nErrCode = DSL_DRV_ModulesInit(pContext);
            if (nErrCode != DSL_SUCCESS)
            {
               DSL_DEBUG( DSL_DBG_ERR,
                  (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - additional modules init failed!"
                  DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

               return nErrCode;
            }
         }
      }
   }
   else
   {
      DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW download failed, "
            "Autoboot will stay in the DSL_AUTOBOOTSTATE_FIRMWARE_WAIT state!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_AutobootControlSet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_AutobootControl_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
#if defined(INCLUDE_DSL_DELT) || defined(INCLUDE_DSL_FILTER_DETECTION)
   DSL_G997_LDSF_t nLoopMode = DSL_G997_INHIBIT_LDSF;
#endif /* defined(INCLUDE_DSL_DELT) || defined(INCLUDE_DSL_FILTER_DETECTION) */
   DSL_boolean_t bFirmwareReady = DSL_FALSE, bInitComplete = DSL_FALSE, bAutobootDisable = DSL_FALSE;
   DSL_Autoboot_State_t nState = DSL_AUTOBOOTSTATE_UNKNOWN;
#if defined(INCLUDE_DSL_BONDING) && (DSL_DRV_LINES_PER_DEVICE == 2)
   DSL_PortMode_t nPortMode;
#endif
#if defined(INCLUDE_DSL_BONDING) && (DSL_DRV_LINES_PER_DEVICE == 1)
   DSL_uint32_t nOppositeLine;
   DSL_uint16_t nRemotePafAvailable = 0;
#endif

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_AutobootControlSet(%d)"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), pData->data.nCommand));

   DSL_CHECK_CTX_POINTER(pContext);
   DSL_CHECK_ERR_CODE();

   DSL_CTX_READ_SCALAR(pContext, nErrCode, bInitComplete, bInitComplete);
   if (bInitComplete == DSL_FALSE)
   {
      nErrCode = DSL_ERR_NOT_INITIALIZED;
   }

   DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->bFirmwareReady,
      bFirmwareReady);

   if (nErrCode == DSL_SUCCESS)
   {
      DSL_CTX_READ(pContext, nErrCode, bAutobootDisable, bAutobootDisable);

#if defined(INCLUDE_DSL_BONDING) && (DSL_DRV_LINES_PER_DEVICE == 1)
      /* Ignore for locked line */
      if (nLineLocked == DSL_DEV_NUM(pContext))
      {
         DSL_DEBUG(DSL_DBG_WRN, (pContext,
            SYS_DBG_WRN"DSL[%02d]: WARNING - PAF not available, line disabled!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         nErrCode = DSL_ERR_BND_REMOTE_PAF_DISABLED;
      }
      else
#endif
      {
         switch (pData->data.nCommand)
         {
            case DSL_AUTOBOOT_CTRL_START:
               if (bFirmwareReady == DSL_FALSE)
               {
                  DSL_DEBUG(DSL_DBG_WRN,
                     (pContext, SYS_DBG_ERR"DSL[%02d]: WARNING - Autoboot Start failed, no FW loaded!"
                     DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
                  nErrCode = DSL_WRN_CONFIG_PARAM_IGNORED;
                  break;
               }

               nErrCode = DSL_DRV_AutobootThreadStart(pContext);
               if( nErrCode != DSL_SUCCESS )
               {
                  DSL_DEBUG(DSL_DBG_ERR,
                    (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Autoboot Thread start failed!"
                    DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
                  break;
               }

               /* Initialize additional modules PM, CEOC,...*/
               nErrCode = DSL_DRV_ModulesInit(pContext);
               if( nErrCode != DSL_SUCCESS )
               {
                  DSL_DEBUG( DSL_DBG_ERR,
                     (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - additional modules init failed!"
                     DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
                  break;
               }

#if defined(INCLUDE_DSL_BONDING) && (DSL_DRV_LINES_PER_DEVICE == 1)
               DSL_DRV_MUTEX_LOCK(bndLineLockMutex);
               /* Start for unlocked line */
               if ((nLineLocked != -1) && (nLineLocked != DSL_DEV_NUM(pContext)))
               {
                  /* No line is lock now */
                  nLineLocked = -1;
               }
               DSL_DRV_MUTEX_UNLOCK(bndLineLockMutex);
#endif
               break;

            case DSL_AUTOBOOT_CTRL_STOP:
               /* Stop Autoboot thread*/
               DSL_DRV_AutobootThreadStop(pContext);
#ifdef INCLUDE_DSL_PM
               /* Suspend PM module*/
               nErrCode = DSL_DRV_PM_Suspend(pContext);

               if (nErrCode != DSL_SUCCESS)
               {
                  DSL_DEBUG( DSL_DBG_ERR,
                     (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - PM module suspend failed!"
                     DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

                  return nErrCode;
               }
#endif /* #ifdef INCLUDE_DSL_PM*/
               /* Trigger restart sequence*/
               DSL_CTX_WRITE_SCALAR(pContext, nErrCode, bAutobootRestart, DSL_TRUE);
               break;

            case DSL_AUTOBOOT_CTRL_ENABLE:
            case DSL_AUTOBOOT_CTRL_DISABLE:
#if defined(INCLUDE_DSL_BONDING) && (DSL_DRV_LINES_PER_DEVICE == 1)
               if (!bAutobootDisable && pData->data.nCommand == DSL_AUTOBOOT_CTRL_DISABLE)
               {
                  nOppositeLine = (DSL_DEV_NUM(pContext) + 1) % DSL_DRV_MAX_ENTITIES;

                  /* Check other line state FULL_INIT or SHOWTIME */
                  if (ifxDevices[nOppositeLine].pContext->bGotFullInit ||
                      ifxDevices[nOppositeLine].pContext->bGotShowtime)
                  {
                     /* Check current line PAF support */
                     nErrCode = DSL_DRV_BND_RemotePafAvailableCheck(pContext, &nRemotePafAvailable);
                     if (nErrCode < DSL_SUCCESS)
                     {
                        DSL_DEBUG( DSL_DBG_ERR, (pContext,
                           SYS_DBG_ERR"DSL[%02d]: ERROR - PAF check failed!"
                           DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

                        return DSL_ERROR;
                     }

                     /* If current line support PAF */
                     if (nRemotePafAvailable)
                     {
                        /* If opposite line disabled */
                        if (ifxDevices[nOppositeLine].pContext->bAutobootDisable)
                        {
                           nRemotePafAvailable = 0;
                        }
                     }

                     /* If current line support PAF and opposite line not disabled */
                     if (nRemotePafAvailable)
                     {
                        /* Check opposite line PAF support */
                        nErrCode = DSL_DRV_BND_RemotePafAvailableCheck(ifxDevices[nOppositeLine].pContext, &nRemotePafAvailable);
                        if (nErrCode < DSL_SUCCESS)
                        {
                           DSL_DEBUG( DSL_DBG_ERR, (pContext,
                              SYS_DBG_ERR"DSL[%02d]: ERROR - PAF check failed!"
                              DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

                           return DSL_ERROR;
                        }
                     }

                     /* remote PAF disabled (for at least for one line)*/
                     if (!nRemotePafAvailable)
                     {
                        DSL_DRV_MUTEX_LOCK(bndLineLockMutex);
                        /* If no locked lines => lock current line */
                        if (nLineLocked == -1)
                        {
                           nLineLocked = DSL_DEV_NUM(pContext);
                        }
                        DSL_DRV_MUTEX_UNLOCK(bndLineLockMutex);
                     }
                  }
               }
#endif

               if (bAutobootDisable && pData->data.nCommand == DSL_AUTOBOOT_CTRL_DISABLE)
               {
                  return DSL_SUCCESS;
               }

               if (!bAutobootDisable && pData->data.nCommand == DSL_AUTOBOOT_CTRL_ENABLE)
               {
                  return DSL_SUCCESS;
               }

               bAutobootDisable = pData->data.nCommand == DSL_AUTOBOOT_CTRL_DISABLE ?
                                    DSL_TRUE : DSL_FALSE;

               if (pContext->bAutobootThreadStarted)
               {
                  DSL_CTX_WRITE(pContext, nErrCode, bAutobootDisable, bAutobootDisable);
               }

               /* ... pass to restart*/

            case DSL_AUTOBOOT_CTRL_RESTART:
            case DSL_AUTOBOOT_CTRL_RESTART_FULL:
               if (bAutobootDisable && pData->data.nCommand == DSL_AUTOBOOT_CTRL_RESTART)
               {
                  return DSL_ERR_AUTOBOOT_DISABLED;
               }
#if defined(INCLUDE_DSL_DELT) || defined(INCLUDE_DSL_FILTER_DETECTION)
               DSL_CTX_READ_SCALAR(pContext, nErrCode, lineActivateConfig.nLDSF, nLoopMode);
#endif /* defined(INCLUDE_DSL_DELT) || defined(INCLUDE_DSL_FILTER_DETECTION) */

#ifdef INCLUDE_DSL_DELT
               if (nLoopMode == DSL_G997_AUTO_LDSF)
               {
                  DSL_CTX_WRITE_SCALAR(pContext, nErrCode, nLoopAutoCount, 1);
               }
#endif /* #ifdef INCLUDE_DSL_DELT*/

               nErrCode = DSL_DRV_HybridContextInit(pContext);
               if (nErrCode != DSL_SUCCESS)
               {
                  DSL_DEBUG(DSL_DBG_ERR, (pContext, "DSL[%02d]: ERROR - HybridContextInit "
                     "init failed!"DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
                  /* Continue with activation sequence anyhow */
               }

#ifdef INCLUDE_DSL_FILTER_DETECTION
               if (nLoopMode == DSL_G997_AUTO_FILTER_DETECTION)
               {
                  DSL_CTX_WRITE_SCALAR(pContext, nErrCode, showtimeMeasurement.bFilterDetectionActive, DSL_TRUE);
               }
#endif
               if (pData->data.nCommand == DSL_AUTOBOOT_CTRL_RESTART_FULL)
               {
                  DSL_CTX_WRITE_SCALAR(pContext, nErrCode, bAutobootDisable, DSL_FALSE);
                  DSL_CTX_WRITE_SCALAR(pContext, nErrCode, bSoftRestart, DSL_FALSE);
               }

#if defined(INCLUDE_DSL_BONDING) && (DSL_DRV_LINES_PER_DEVICE == 2)
               if (pData->data.nCommand == DSL_AUTOBOOT_CTRL_RESTART)
               {
                  /* get port mode type*/
                  DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->data.nPortMode, nPortMode);
                  if (nPortMode == DSL_PORT_MODE_DUAL)
                  {
                     DSL_CTX_WRITE_SCALAR(pContext, nErrCode, bSoftRestart, DSL_TRUE);
                  }
               }
#endif
               DSL_CTX_WRITE_SCALAR(pContext, nErrCode, bAutobootRestart, DSL_TRUE);

               if (pContext->bAutobootThreadStarted)
               {
                  /* Change Autoboot Status*/
                  nErrCode = DSL_DRV_AutobootStatusSet(pContext,
                                DSL_AUTOBOOT_STATUS_RUNNING, DSL_FW_REQUEST_NA);
               }

#if defined(INCLUDE_DSL_BONDING) && (DSL_DRV_LINES_PER_DEVICE == 1)
               if ((pData->data.nCommand == DSL_AUTOBOOT_CTRL_RESTART) ||
                  (pData->data.nCommand == DSL_AUTOBOOT_CTRL_RESTART_FULL))
               {
                  DSL_DRV_MUTEX_LOCK(bndLineLockMutex);
                  /* Restart for unlocked line */
                  if ((nLineLocked != -1) && (nLineLocked != DSL_DEV_NUM(pContext)))
                  {
                     /* No line is lock now */
                     nLineLocked = -1;
                  }
                  DSL_DRV_MUTEX_UNLOCK(bndLineLockMutex);
               }
#endif
               break;

            case DSL_AUTOBOOT_CTRL_CONTINUE:
               DSL_CTX_READ_SCALAR(pContext, nErrCode, nAutobootState, nState);
               if ((nState != DSL_AUTOBOOTSTATE_LINK_ACTIVATE_WAIT) &&
                   (nState != DSL_AUTOBOOTSTATE_CONFIG_WRITE_WAIT) &&
                   (nState != DSL_AUTOBOOTSTATE_RESTART_WAIT))
               {
                  nErrCode = DSL_WRN_NOT_ALLOWED_IN_CURRENT_STATE;
               }
               else
               {
                  /* Trigger to continue Autoboot handling*/
                  DSL_CTX_WRITE_SCALAR(pContext, nErrCode, bAutobootContinue, DSL_TRUE);
               }
               break;
            default:
               nErrCode = DSL_ERR_INVALID_PARAMETER;
               break;
         }
      }
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_AutobootControlSet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
#ifdef INCLUDE_DSL_CONFIG_GET
DSL_Error_t DSL_DRV_AutobootConfigGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_OUT DSL_AutobootConfig_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_AutobootConfigGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Get Autoboot Configuration from the DSL CPE context*/
   DSL_CTX_READ(pContext, nErrCode, nAutobootConfig, pData->data);

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_AutobootConfigGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}
#endif /* INCLUDE_DSL_CONFIG_GET*/

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_AutobootConfigSet(
   DSL_IN DSL_Context_t *pContext,
   DSL_OUT DSL_AutobootConfig_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_AutobootConfigSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Set Autoboot Configuration from the DSL CPE context*/
   DSL_CTX_WRITE(pContext, nErrCode, nAutobootConfig, pData->data);

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_AutobootConfigSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_AutobootStatusGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_OUT DSL_AutobootStatus_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_AutobootStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Get Autoboot Status from the DSL CPE context*/
   DSL_CTX_READ(pContext, nErrCode, nAutobootStatus.nStatus, pData->data.nStatus);

   DSL_CTX_READ(pContext, nErrCode, nAutobootStatus.nFirmwareRequestType,
      pData->data.nFirmwareRequestType);

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_AutobootStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_AutobootStatusSet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN DSL_AutobootStatGet_t nStatus,
   DSL_IN DSL_FirmwareRequestType_t nFirmwareRequestType)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_AutobootStatGet_t nOldStatus = DSL_AUTOBOOT_STATUS_LAST;
   DSL_AutobootStatusData_t autobootStatus;

   DSL_CHECK_CTX_POINTER(pContext);;
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_AutobootStatusSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   DSL_CTX_READ_SCALAR(pContext, nErrCode, nAutobootStatus.nStatus, nOldStatus);

   if (nStatus != nOldStatus)
   {
      DSL_DEBUG(DSL_DBG_MSG,
         (pContext, SYS_DBG_MSG"DSL[%02d]: Autoboot status has changed: "
         "%s(%d) -> %s(%d)" DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
         DSL_DBG_PRN_AUTOBOOT_STATUS[nOldStatus], nOldStatus,
         DSL_DBG_PRN_AUTOBOOT_STATUS[nStatus], nStatus));

      autobootStatus.nStatus = nStatus;
      autobootStatus.nFirmwareRequestType = nFirmwareRequestType;

      /* Set Autoboot Status in the DSL CPE context*/
      DSL_CTX_WRITE(pContext, nErrCode, nAutobootStatus, autobootStatus);

      nErrCode =  DSL_DRV_EventGenerate(
         pContext, 0, DSL_ACCESSDIR_NA, DSL_XTUDIR_NA,
         DSL_EVENT_S_AUTOBOOT_STATUS,
         (DSL_EventData_Union_t*)&autobootStatus,
         sizeof(DSL_AutobootStatusData_t));

      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Event(%d) generate failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), DSL_EVENT_S_AUTOBOOT_STATUS));
      }
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_AutobootStatusSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
#ifdef INCLUDE_DSL_RESOURCE_STATISTICS
DSL_Error_t DSL_DRV_ResourceUsageStatisticsGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_OUT DSL_ResourceUsageStatistics_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint32_t staticMemUsageTotal = 0, dynamicMemUsageTotal = 0,
                eventFifoTotal = 0, eventFifoBuffTotal = 0;
#ifdef INCLUDE_DSL_CEOC
   DSL_uint32_t snmpFifoTotal = 0, snmpFifoBuffTotal = 0;
#endif /* INCLUDE_DSL_CEOC*/
   DSL_OpenContext_t *pOpenContext = DSL_NULL;
   DSL_ResourceUsageStatisticsData_t devResourceUsageStatisticsData;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_POINTER(pContext, pContext->pDevCtx);
   DSL_CHECK_ERR_CODE();

   nErrCode = DSL_DRV_DEV_ResourceUsageStatisticsGet(
                 pContext, &devResourceUsageStatisticsData);

   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Device resource usage statistics get failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return nErrCode;
   }

   /* *********************************************************************** */
   /* *** Print statistic on memory usage of DSL CPE API                  *** */
   /* *********************************************************************** */
   DSL_DEBUG( DSL_DBG_PRN, (pContext, SYS_DBG_PRN"DSL: " DSL_DRV_CRLF ));
   DSL_DEBUG( DSL_DBG_PRN, (pContext,
      SYS_DBG_PRN"DSL: *****************************************************" DSL_DRV_CRLF ));
   DSL_DEBUG( DSL_DBG_PRN, (pContext,
      SYS_DBG_PRN"DSL: DSL CPE library version: %s" DSL_DRV_CRLF, DSL_CPE_API_PACKAGE_VERSION));
   DSL_DEBUG( DSL_DBG_PRN, (pContext,
      SYS_DBG_PRN"DSL: *****************************************************" DSL_DRV_CRLF ));
   DSL_DEBUG( DSL_DBG_PRN, (pContext,
      SYS_DBG_PRN"DSL: Device number              : %d" DSL_DRV_CRLF, pContext->pDevCtx->nNum));
   DSL_DEBUG( DSL_DBG_PRN, (pContext,
      SYS_DBG_PRN"DSL: Number of opened instances : %d" DSL_DRV_CRLF, pContext->pDevCtx->nUsageCount));
   DSL_DEBUG( DSL_DBG_PRN, (pContext,
      SYS_DBG_PRN"DSL: Number of channels/line    : %d" DSL_DRV_CRLF , DSL_CHANNELS_PER_LINE));
   DSL_DEBUG( DSL_DBG_PRN, (pContext, SYS_DBG_PRN"DSL: " DSL_DRV_CRLF ));

   /*
      Global Context static memory usage
   */
   DSL_DEBUG( DSL_DBG_PRN, (pContext, SYS_DBG_PRN"DSL: Global Context static memory usage" DSL_DRV_CRLF ));
   DSL_DEBUG( DSL_DBG_PRN, (pContext,
      SYS_DBG_PRN"DSL:   context structure            : %10d bytes" DSL_DRV_CRLF, (int)sizeof(DSL_Context_t)));
   /* Update total static memory usage*/
   staticMemUsageTotal += sizeof(DSL_Context_t);
#ifdef INCLUDE_DSL_TIMEOUT
   DSL_DEBUG( DSL_DBG_PRN, (pContext,
      SYS_DBG_PRN"DSL:   timeout event lists          : %10d bytes" DSL_DRV_CRLF,
      pContext->TimeoutListsContext.nNrOfElements * sizeof(DSL_TimeoutElement_t) * DSL_MAX_TIMEOUT_NUM));
   staticMemUsageTotal += pContext->TimeoutListsContext.nNrOfElements * sizeof(DSL_TimeoutElement_t) * DSL_MAX_TIMEOUT_NUM;
#endif /* INCLUDE_DSL_TIMEOUT*/
#ifdef INCLUDE_DSL_DELT
#ifdef DSL_CPE_STATIC_DELT_DATA
   DSL_DEBUG( DSL_DBG_PRN, (pContext,
      SYS_DBG_PRN"DSL:   DELT diagnostic data         : %10d bytes" DSL_DRV_CRLF, (int)sizeof(DSL_G997_DeltData_t)));
   staticMemUsageTotal += sizeof(DSL_G997_DeltData_t);
   #ifdef INCLUDE_DSL_DELT_SHOWTIME
   DSL_DEBUG( DSL_DBG_PRN, (pContext,
      SYS_DBG_PRN"DSL:   DELT showtime data           : %10d bytes" DSL_DRV_CRLF, (int)sizeof(DSL_G997_DeltShowtimeData_t)));
   staticMemUsageTotal += sizeof(DSL_G997_DeltShowtimeData_t);
   #endif /* INCLUDE_DSL_DELT_SHOWTIME*/
#endif /* DSL_CPE_STATIC_DELT_DATA*/
#endif /* INCLUDE_DSL_DELT*/

   /*
       Device specific static memory usage
   */
   DSL_DEBUG( DSL_DBG_PRN, (pContext, SYS_DBG_PRN"DSL: Device specific static memory usage" DSL_DRV_CRLF ));
   DSL_DEBUG( DSL_DBG_PRN, (pContext,
      SYS_DBG_PRN"DSL:   device context structure     : %10d bytes" DSL_DRV_CRLF, (int)(sizeof(DSL_devCtx_t)*DSL_DRV_MAX_ENTITIES)));
   staticMemUsageTotal += sizeof(DSL_devCtx_t);
   DSL_DEBUG( DSL_DBG_PRN, (pContext,
      SYS_DBG_PRN"DSL:   device auxiliary data        : %10d bytes" DSL_DRV_CRLF, devResourceUsageStatisticsData.staticMemUsage));
   staticMemUsageTotal += devResourceUsageStatisticsData.staticMemUsage;
   DSL_DEBUG( DSL_DBG_PRN, (pContext,
      SYS_DBG_PRN"DSL:                                 --------------" DSL_DRV_CRLF ));
   DSL_DEBUG( DSL_DBG_PRN, (pContext,
      SYS_DBG_PRN"DSL: Total static memory usage      : %10d bytes" DSL_DRV_CRLF, staticMemUsageTotal));
   DSL_DEBUG( DSL_DBG_PRN, (pContext, SYS_DBG_PRN"DSL: " DSL_DRV_CRLF ));
   /*
      Global Context dynamic memory usage
   */
   DSL_DEBUG( DSL_DBG_PRN, (pContext, SYS_DBG_PRN"DSL: Global Context dynamic memory usage" DSL_DRV_CRLF ));
#ifndef INCLUDE_FW_REQUEST_SUPPORT
   if( pContext->pFirmware != DSL_NULL && pContext->nFirmwareSize)
   {
      dynamicMemUsageTotal += pContext->nFirmwareSize;
      DSL_DEBUG( DSL_DBG_PRN, (pContext,
      SYS_DBG_PRN"DSL:   1st firmware                 : %10d bytes" DSL_DRV_CRLF,
         pContext->nFirmwareSize));
   }

   if( pContext->pFirmware2 != DSL_NULL && pContext->nFirmwareSize2)
   {
      dynamicMemUsageTotal += pContext->nFirmwareSize2;
      DSL_DEBUG( DSL_DBG_PRN, (pContext,
      SYS_DBG_PRN"DSL:   2nd firmware                 : %10d bytes" DSL_DRV_CRLF,
         pContext->nFirmwareSize2));
   }
#endif /* #ifdef INCLUDE_FW_REQUEST_SUPPORT*/

#ifdef INCLUDE_DSL_DELT
#ifndef DSL_CPE_STATIC_DELT_DATA
   if (pContext->DELT)
   {
      dynamicMemUsageTotal += sizeof(DSL_G997_DeltData_t);
   }
   DSL_DEBUG( DSL_DBG_PRN, (pContext,
      SYS_DBG_PRN"DSL:   DELT diagnostic data         : %10d bytes" DSL_DRV_CRLF,
      pContext->DELT ? (int)sizeof(DSL_G997_DeltData_t) : 0));
   #ifdef INCLUDE_DSL_DELT_SHOWTIME
   if (pContext->DELT_SHOWTIME)
   {
      dynamicMemUsageTotal += sizeof(DSL_G997_DeltShowtimeData_t);
   }
   DSL_DEBUG( DSL_DBG_PRN, (pContext,
      SYS_DBG_PRN"DSL:   DELT showtime data           : %10d bytes" DSL_DRV_CRLF,
      pContext->DELT_SHOWTIME ? (int)sizeof(DSL_G997_DeltShowtimeData_t) : 0));
   #endif /* INCLUDE_DSL_DELT_SHOWTIME*/
#endif /* DSL_CPE_STATIC_DELT_DATA*/
#endif /* INCLUDE_DSL_DELT*/

#ifdef INCLUDE_DSL_PM
   if (pContext->PM)
   {
      dynamicMemUsageTotal += sizeof(DSL_PM_Context);
      DSL_DEBUG( DSL_DBG_PRN, (pContext,
         SYS_DBG_PRN"DSL:   PM context structure         : %10d bytes" DSL_DRV_CRLF,
         pContext->PM ? (int)sizeof(DSL_PM_Context) : 0));

      if ( (((DSL_PM_Context*)pContext->PM)->pCounters))
      {
         dynamicMemUsageTotal += sizeof(DSL_PM_CountersData_t);
      }

      DSL_DEBUG( DSL_DBG_PRN, (pContext,
         SYS_DBG_PRN"DSL:   PM counters                  : %10d bytes" DSL_DRV_CRLF,
         ((DSL_PM_Context*)pContext->PM)->pCounters ?
         (int)sizeof(DSL_PM_CountersData_t) : 0));

      if (((DSL_PM_Context*)pContext->PM)->pCountersDump)
      {
         dynamicMemUsageTotal += sizeof(DSL_PM_CountersDump_t);
      }
      DSL_DEBUG( DSL_DBG_PRN, (pContext,
         SYS_DBG_PRN"DSL:   PM counters dump             : %10d bytes" DSL_DRV_CRLF,
         ((DSL_PM_Context*)pContext->PM)->pCountersDump ?
         (int)sizeof(DSL_PM_CountersDump_t) : 0));
   }
   else
   {
      DSL_DEBUG( DSL_DBG_PRN, (pContext,
         SYS_DBG_PRN"DSL:   PM context structure         : %10d bytes" DSL_DRV_CRLF, 0));
      DSL_DEBUG( DSL_DBG_PRN, (pContext,
         SYS_DBG_PRN"DSL:   PM counters                  : %10d bytes" DSL_DRV_CRLF, 0));
      DSL_DEBUG( DSL_DBG_PRN, (pContext,
         SYS_DBG_PRN"DSL:   PM counters dump             : %10d bytes" DSL_DRV_CRLF, 0));
   }
#endif /* INCLUDE_DSL_PM*/

#ifdef INCLUDE_DSL_CEOC
   if (pContext->CEOC)
   {
      dynamicMemUsageTotal += sizeof(DSL_CEOC_Context_t);
   }
   DSL_DEBUG( DSL_DBG_PRN, (pContext,
      SYS_DBG_PRN"DSL:   CEOC context structure       : %10d bytes" DSL_DRV_CRLF,
      pContext->CEOC ? (int)sizeof(DSL_CEOC_Context_t) : 0));
#endif /* INCLUDE_DSL_CEOC*/
   /*
      Instance(s) Context dynamic memory usage
   */
   DSL_DEBUG( DSL_DBG_PRN, (pContext, SYS_DBG_PRN"DSL: Instance(s) Context dynamic memory usage" DSL_DRV_CRLF ));
   DSL_DEBUG( DSL_DBG_PRN, (pContext,
      SYS_DBG_PRN"DSL:   opened context structure     : %10d bytes" DSL_DRV_CRLF,
      (int)(pContext->pDevCtx->nUsageCount * sizeof(DSL_OpenContext_t))));
   dynamicMemUsageTotal += (pContext->pDevCtx->nUsageCount * sizeof(DSL_OpenContext_t));

   /* Lock Open Context list*/
   if (DSL_DRV_MUTEX_LOCK(pContext->pDevCtx->openContextListMutex))
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (DSL_NULL, SYS_DBG_PRN"DSL[%02d]: ERROR - Couldn't lock Context List mutex!"DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));

      return DSL_ERR_SEMAPHORE_GET;
   }

   /* Proceess dynamic memory allocated by the opened instances*/
   if (pOpenContextList != DSL_NULL)
   {
      pOpenContext = pOpenContextList;
      while(pOpenContext != DSL_NULL)
      {
         eventFifoTotal     += (pOpenContext->eventFifo ? sizeof(DSL_FIFO) : 0);
         eventFifoBuffTotal += (pOpenContext->eventFifoBuf ?
                               DSL_EVENT_FIFO_ELEMENT_COUNT * sizeof(DSL_EventStatusData_t) : 0);
#ifdef INCLUDE_DSL_CEOC
         snmpFifoTotal     += (pOpenContext->rxSnmpFifo ? sizeof(DSL_FIFO) : 0);
         snmpFifoBuffTotal += (pOpenContext->rxSnmpFifoBuf ?
                              DSL_CEOC_RX_FIFO_ELEMENT_COUNT * sizeof(DSL_G997_SnmpData_t) : 0);
#endif /* INCLUDE_DSL_CEOC*/
         pOpenContext = (DSL_OpenContext_t*)pOpenContext->pNext;
      }
   }

   /* Unlock Open Context list*/
   DSL_DRV_MUTEX_UNLOCK(pContext->pDevCtx->openContextListMutex);

   dynamicMemUsageTotal += eventFifoTotal;
   dynamicMemUsageTotal += eventFifoBuffTotal;
   DSL_DEBUG( DSL_DBG_PRN, (pContext,
      SYS_DBG_PRN"DSL:   event FIFO                   : %10d bytes" DSL_DRV_CRLF,
      (int)(eventFifoTotal)));
   DSL_DEBUG( DSL_DBG_PRN, (pContext,
      SYS_DBG_PRN"DSL:   event FIFO buffer            : %10d bytes" DSL_DRV_CRLF,
      (int)(eventFifoBuffTotal)));
#ifdef INCLUDE_DSL_CEOC
   dynamicMemUsageTotal += snmpFifoTotal;
   dynamicMemUsageTotal += snmpFifoBuffTotal;
   DSL_DEBUG( DSL_DBG_PRN, (pContext,
      SYS_DBG_PRN"DSL:   SNMP FIFO                    : %10d bytes" DSL_DRV_CRLF,
      (int)(eventFifoTotal)));
   DSL_DEBUG( DSL_DBG_PRN, (pContext,
      SYS_DBG_PRN"DSL:   SNMP FIFO buffer             : %10d bytes" DSL_DRV_CRLF,
      (int)(eventFifoBuffTotal)));
#endif /* INCLUDE_DSL_CEOC*/
   DSL_DEBUG( DSL_DBG_PRN, (pContext,
      SYS_DBG_PRN"DSL:                                 --------------" DSL_DRV_CRLF ));
   DSL_DEBUG( DSL_DBG_PRN, (pContext,
      SYS_DBG_PRN"DSL: Total dynamic memory usage     : %10d bytes" DSL_DRV_CRLF, dynamicMemUsageTotal));
   DSL_DEBUG( DSL_DBG_PRN, (pContext, SYS_DBG_PRN"DSL: " DSL_DRV_CRLF ));

   /* Fill Output structure*/
   pData->data.staticMemUsage  = staticMemUsageTotal;
   pData->data.dynamicMemUsage = dynamicMemUsageTotal;

   return nErrCode;
}
#endif /* INCLUDE_DSL_RESOURCE_STATISTICS*/

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_VersionInformationGet(
    DSL_IN DSL_Context_t *pContext,
    DSL_IN_OUT DSL_VersionInformation_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_boolean_t bFirmwareReady = DSL_FALSE;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DRV_MemSet(pData, 0, sizeof(DSL_VersionInformation_t));
   strncpy(pData->data.DSL_DriverVersionApi, DSL_CPE_API_PACKAGE_VERSION,
           MAX_INFO_STRING_LEN);

   nErrCode = DSL_DRV_DEV_DriverVersionGet(pContext, pData->data.DSL_DriverVersionMeiBsp);
   if (nErrCode == DSL_SUCCESS)
   {
      nErrCode = DSL_DRV_DEV_ChipSetTypeGet(pContext, pData->data.DSL_ChipSetType);
   }

   if (nErrCode >= DSL_SUCCESS)
   {
      DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->bFirmwareReady, bFirmwareReady);

      if (bFirmwareReady != DSL_FALSE)
      {
         nErrCode = DSL_DRV_DEV_FirmwareVersionGet(pContext,
            pData->data.DSL_ChipSetFWVersion);

         if (nErrCode >= DSL_SUCCESS)
         {
            nErrCode = DSL_DRV_DEV_ChipHardwareVersionGet(pContext,
               pData->data.DSL_ChipSetHWVersion);
         }

         if (nErrCode >= DSL_SUCCESS)
         {
            nErrCode = DSL_DRV_DEV_HybridTypeGet(pContext,
               &pData->data.nHybridType);
         }
      }
      else
      {
         strcpy(pData->data.DSL_ChipSetFWVersion, "n/a");
         strcpy(pData->data.DSL_ChipSetHWVersion, "n/a");
         pData->data.nHybridType = DSL_HYBRID_TYPE_NA;
      }
   }

   return (nErrCode != DSL_SUCCESS ? DSL_WRN_INCOMPLETE_RETURN_VALUES : nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_LineStateGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_LineState_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_LineStateGet"DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext)));

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineState, pData->data.nLineState);

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_LineStateGet,"
      " nLineState=%08X, retCode=%d"DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
      pData->data.nLineState,nErrCode));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_LineFeatureConfigSet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_LineFeature_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS, nRet = DSL_SUCCESS;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_LineFeatureConfigSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();
   DSL_CHECK_DIRECTION(pData->nDirection);
   DSL_CHECK_ERR_CODE();

   if (pData->nDirection == DSL_UPSTREAM)
   {
      if(pData->data.bReTxEnable == DSL_TRUE)
      {
         /* Retransmission mode not supported for US */
         pData->data.bReTxEnable = DSL_FALSE;
         nRet = DSL_WRN_CONFIG_PARAM_IGNORED;
      }
   }

   if ((pData->nDirection == DSL_UPSTREAM) &&
       (pData->data.bTrellisEnable == DSL_FALSE))
   {
      /* Fixed default value for the US direction*/
      pData->data.bTrellisEnable = DSL_TRUE;
      nRet = DSL_WRN_CONFIG_PARAM_IGNORED;
   }

   DSL_CTX_WRITE(pContext, nErrCode, lineFeatureDataCfg[pData->nDirection], pData->data);

#if defined(INCLUDE_DSL_CPE_API_DANUBE)
   if ( (pData->nDirection == DSL_UPSTREAM) &&
        (pData->data.b20BitSupport == DSL_FEATURE_ENABLED))
   {
      nRet = DSL_WRN_CONFIG_PARAM_IGNORED;
   }
#endif

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_LineFeatureConfigSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nRet;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
#ifdef INCLUDE_DSL_CONFIG_GET
DSL_Error_t DSL_DRV_LineFeatureConfigGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_LineFeature_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_LineFeatureConfigGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();
   DSL_CHECK_DIRECTION(pData->nDirection);
   DSL_CHECK_ERR_CODE();

   DSL_CTX_READ(pContext, nErrCode, lineFeatureDataCfg[pData->nDirection], pData->data);

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_LineFeatureConfigGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}
#endif /* INCLUDE_DSL_CONFIG_GET*/

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_LineFeatureStatusGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_LineFeature_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_LineStateValue_t nCurrentState = DSL_LINESTATE_UNKNOWN;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_LineFeatureStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();
   DSL_CHECK_DIRECTION(pData->nDirection);
   DSL_CHECK_ERR_CODE();

   /* Get current line state*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineState, nCurrentState);

   /* Only proceed if the line is in SHOWTIME state.*/
   if ( (nCurrentState != DSL_LINESTATE_SHOWTIME_TC_SYNC) &&
        (nCurrentState != DSL_LINESTATE_SHOWTIME_NO_SYNC) )
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Function is only available in the SHOWTIME!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      nErrCode = DSL_ERR_ONLY_AVAILABLE_IN_SHOWTIME;
   } else
   {
      DSL_CTX_READ(pContext, nErrCode, lineFeatureDataSts[pData->nDirection], pData->data);
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_LineFeatureStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

#ifdef INCLUDE_DSL_CPE_API_DANUBE
/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_InteropFeatureConfigSet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_InteropFeatureConfig_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_InteropFeatureConfigSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   if ((pData->data.nSnrMarginRebootCfg.nSnrMarginRebootMode < DSL_SNRM_REBOOT_AUTOMODE_API) ||
       (pData->data.nSnrMarginRebootCfg.nSnrMarginRebootMode > DSL_SNRM_REBOOT_LAST) ||
       (pData->data.nSnrMarginRebootCfg.nUserMinSnrMargin < -320) ||
       (pData->data.nSnrMarginRebootCfg.nUserMinSnrMargin > 310))
   {
      nErrCode = DSL_ERROR;
   }

   if (nErrCode != DSL_ERROR)
   {
      DSL_CTX_WRITE(pContext, nErrCode, interopFeatureConfigData, pData->data);
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_InteropFeatureConfigSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
#ifdef INCLUDE_DSL_CONFIG_GET
DSL_Error_t DSL_DRV_InteropFeatureConfigGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_InteropFeatureConfig_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_InteropFeatureConfigGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_CTX_READ(pContext, nErrCode, interopFeatureConfigData, pData->data);

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_InteropFeatureConfigGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}
#endif /* INCLUDE_DSL_CONFIG_GET*/
#endif /* INCLUDE_DSL_CPE_API_DANUBE*/

DSL_Error_t DSL_DRV_LoopLengthStatusGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_LoopLengthStatus_t *pData)
{
   DSL_LoopLengthStatusData_t loopLengthData;
   DSL_uint16_t intP = 0, frcP = 0;
   DSL_boolean_t bLoopLengthValid = DSL_FALSE;
   DSL_Error_t nLoopLengthErrCode = DSL_SUCCESS;
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_LoopLengthStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

#ifdef INCLUDE_DSL_CPE_API_VRX
   /* Only for ADSL mode */
   if (!DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSL))
   {
      DSL_DEBUG(DSL_DBG_ERR, (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - "
               "hybrid/filter/looplength statistics are not supported "
               "in current VDSL mode" DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERR_NOT_SUPPORTED_IN_CURRENT_VDSL_MODE;
   }
#endif /* INCLUDE_DSL_CPE_API_VRX */

   DSL_CTX_READ_SCALAR(pContext, nErrCode, showtimeMeasurement.bLoopLengthValid, bLoopLengthValid);

   if (bLoopLengthValid == DSL_TRUE)
   {
      DSL_CTX_READ(pContext, nErrCode, showtimeMeasurement.loopLengthStatusData, loopLengthData);

      if (pData->nUnit == DSL_UNIT_FEET)
      {
         pData->data.nLength_Awg26 = loopLengthData.nLength_Awg26;
         pData->data.nLength_Awg24 = loopLengthData.nLength_Awg24;
      }
      else
      {
         /* Convert AWG26 to meters */
         intP = (DSL_uint16_t)((loopLengthData.nLength_Awg26 * 3048) / 10000);
         frcP = (DSL_uint16_t)((((loopLengthData.nLength_Awg26 * 3048) - intP * 10000) * 65536) / 10000);
         frcP = frcP >= 32768 ? 1 : 0;
         pData->data.nLength_Awg26 = (DSL_uint32_t)(intP + frcP);


         /* Convert AWG24 to meters */
         intP = (DSL_uint16_t)((loopLengthData.nLength_Awg24 * 3048) / 10000);
         frcP = (DSL_uint16_t)((((loopLengthData.nLength_Awg24 * 3048) - intP * 10000) * 65536) / 10000);
         frcP = frcP >= 32768 ? 1 : 0;
         pData->data.nLength_Awg24 = (DSL_uint32_t)(intP + frcP);

      }
   }
   else
   {
      DSL_CTX_READ_SCALAR(pContext, nErrCode, showtimeMeasurement.nLoopLengthErrCode, nLoopLengthErrCode);

      pData->data.nLength_Awg24 = 0;
      pData->data.nLength_Awg26 = 0;
      nErrCode = nLoopLengthErrCode;
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_LoopLengthStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

DSL_Error_t DSL_DRV_RetxStatisticsGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_ReTxStatistics_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();
   DSL_CHECK_DIRECTION(pData->nDirection);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_RetxStatisticsGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   nErrCode = DSL_DRV_DEV_RetxStatisticsGet(pContext,pData);

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_RetxStatisticsGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

DSL_Error_t DSL_DRV_SystemInterfaceConfigCheck(
   DSL_Context_t *pContext,
   DSL_DslModeSelection_t nDslMode,
   DSL_SystemInterfaceConfigData_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
#if defined(INCLUDE_DSL_CPE_API_VRX) && defined(INCLUDE_DSL_BONDING)
   DSL_boolean_t bPafEnable = DSL_FALSE;
#endif

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

#if defined(INCLUDE_DSL_CPE_API_VRX) && defined(INCLUDE_DSL_BONDING)
   /* Bonding is always enabled for both lines/devices together so using the
      configuration from the current line/device is ok. */
   DSL_CTX_READ_SCALAR( pContext, nErrCode, BndConfig.bPafEnable, bPafEnable);
#endif

   if ((pData->nTcLayer <= DSL_TC_UNKNOWN) ||
       (pData->nTcLayer >= DSL_TC_LAST))
   {
      nErrCode = DSL_ERR_PARAM_RANGE;
   }
   else
   {
      switch(pData->nTcLayer)
      {
      case DSL_TC_ATM:
      case DSL_TC_EFM:
      case DSL_TC_AUTO:
#if defined(INCLUDE_DSL_CPE_API_DANUBE)
      case DSL_TC_EFM_FORCED:
#endif
         break;
      default:
         nErrCode = DSL_ERR_PARAM_RANGE;
         break;
      }
   }

   /* Set error/warning return codes in order of priority */
   if ((pData->nEfmTcConfigUs < DSL_EMF_TC_CLEANED) ||
       (pData->nEfmTcConfigUs >= (DSL_EMF_TC_SHORT_PACKETS << 1)))
   {
      nErrCode = DSL_ERR_PARAM_RANGE;
   }
   else if ((pData->nEfmTcConfigDs < DSL_EMF_TC_CLEANED) ||
            (pData->nEfmTcConfigDs >= (DSL_EMF_TC_SHORT_PACKETS << 1)))
   {
      nErrCode = DSL_ERR_PARAM_RANGE;
   }
   else if ((pData->nSystemIf < DSL_SYSTEMIF_UNKNOWN) ||
            (pData->nSystemIf >= DSL_SYSTEMIF_LAST))
   {
      nErrCode = DSL_ERR_PARAM_RANGE;
   }
#if defined(INCLUDE_DSL_CPE_API_VRX) && defined(INCLUDE_DSL_BONDING)
   else if ((bPafEnable == DSL_TRUE) && (nDslMode == DSL_MODE_VDSL))
   {
      if (pData->nTcLayer == DSL_TC_ATM)
      {
         nErrCode = DSL_ERR_CONFIG_BND_VS_TCLAYER;
      }
      else if (pData->nTcLayer == DSL_TC_AUTO)
      {
         nErrCode = DSL_WRN_CONFIG_BND_VS_TCLAYER;
      }
   }
#endif

   return (nErrCode);
}

/*
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_SYSTEM_INTERFACE_CONFIG_SET
*/
#ifdef INCLUDE_DSL_SYSTEM_INTERFACE
DSL_Error_t DSL_DRV_SystemInterfaceConfigSet(
   DSL_Context_t *pContext,
   DSL_SystemInterfaceConfig_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS, nRetCode = DSL_SUCCESS;
   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_DSLMODE(pData->nDslMode);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG, (pContext, SYS_DBG_MSG
      "DSL[%02d]: IN - DSL_SystemInterfaceConfigSet(nSystemIf=%#x, nTcLayer=%#x)"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
      pData->data.nSystemIf, pData->data.nTcLayer));

   nErrCode = DSL_DRV_SystemInterfaceConfigCheck(pContext, pData->nDslMode,
      &(pData->data));

   if (nErrCode >= DSL_SUCCESS)
   {
      nRetCode = DSL_DRV_DEV_SystemInterfaceConfigSet(pContext, pData);
      DSL_SET_ERROR_CODE(nRetCode, nErrCode);
   }

   DSL_DEBUG( DSL_DBG_MSG, (pContext, SYS_DBG_MSG
      "DSL[%02d]: OUT - DSL_SystemInterfaceConfigSet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return (nErrCode);
}
#endif /* INCLUDE_DSL_SYSTEM_INTERFACE*/

/*
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_SYSTEM_INTERFACE_CONFIG_GET
*/
#ifdef INCLUDE_DSL_CONFIG_GET
#ifdef INCLUDE_DSL_SYSTEM_INTERFACE
DSL_Error_t DSL_DRV_SystemInterfaceConfigGet(
   DSL_Context_t *pContext,
   DSL_SystemInterfaceConfig_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_DSLMODE(pData->nDslMode);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_SystemInterfaceConfigGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   nErrCode = DSL_DRV_DEV_SystemInterfaceConfigGet(pContext, pData);

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_SystemInterfaceConfigGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return (nErrCode);
}
#endif /* INCLUDE_DSL_SYSTEM_INTERFACE*/
#endif /* INCLUDE_DSL_CONFIG_GET*/

/*
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_SYSTEM_INTERFACE_STATUS_GET
*/
#ifdef INCLUDE_DSL_SYSTEM_INTERFACE
DSL_Error_t DSL_DRV_SystemInterfaceStatusGet(
   DSL_Context_t *pContext,
   DSL_SystemInterfaceStatus_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_SystemInterfaceStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   nErrCode = DSL_DRV_DEV_SystemInterfaceStatusGet(pContext, pData);

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_SystemInterfaceStatusGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return (nErrCode);
}
#endif /* INCLUDE_DSL_SYSTEM_INTERFACE*/

#if defined(INCLUDE_DSL_CPE_MISC_LINE_STATUS) || defined(INCLUDE_DSL_CPE_API_DANUBE)
DSL_Error_t DSL_DRV_AdslBandLimitsGet(
   DSL_Context_t *pContext,
   DSL_AccessDir_t nDirection,
   DSL_Band_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint8_t XTSE[DSL_G997_NUM_XTSE_OCTETS] = {0};
   DSL_uint16_t nFirstToneIndex = 0, nLastToneIndex = 0;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_DIRECTION(nDirection);
   DSL_CHECK_ERR_CODE();

   /*Get current xTSE octets*/
   DSL_CTX_READ(pContext, nErrCode, xtseCurr, XTSE);

   if (nDirection == DSL_DOWNSTREAM)
   {
      if ((XTSE[1-1] & (XTSE_1_03_A_1_NO | XTSE_1_01_A_T1_413)) ||
          (XTSE[4-1] & XTSE_4_05_I_3_NO))
      {
         nFirstToneIndex = 32;
         nLastToneIndex  = 255;
      }
      else if ((XTSE[1-1] & (XTSE_1_05_B_1_NO | XTSE_1_02_C_TS_101388)) ||
               (XTSE[3-1] & XTSE_3_05_B_3_NO) || (XTSE[5-1] & XTSE_5_07_M_3_NO) ||
               (XTSE[4-1] & XTSE_4_07_J_3_NO))
      {
         nFirstToneIndex = 64;
         nLastToneIndex  = 255;
      }
      else if ((XTSE[3-1] & XTSE_3_03_A_3_NO))
      {
         nFirstToneIndex = 32;
         nLastToneIndex  = 255;
      }
      else if ((XTSE[6-1] & (XTSE_6_01_A_5_NO | XTSE_6_07_I_5_NO)))
      {
         nFirstToneIndex = 32;
         nLastToneIndex  = 511;
      }
      else if ((XTSE[6-1] & XTSE_6_03_B_5_NO) ||
               (XTSE[7-1] & (XTSE_7_03_M_5_NO | XTSE_7_01_J_5_NO)))
      {
         nFirstToneIndex = 64;
         nLastToneIndex  = 511;
      }
      else if ((XTSE[5-1] & (XTSE_5_03_L_3_NO | XTSE_5_04_L_3_NO)) ||
               (XTSE[2-1] & XTSE_2_01_A_2_NO))
      {
         nFirstToneIndex = 32;
         nLastToneIndex  = 128;
      }
      else
      {
         nErrCode = DSL_ERROR;
      }
   }
   else
   {
      if ((XTSE[1-1] & (XTSE_1_03_A_1_NO | XTSE_1_01_A_T1_413)) ||
          (XTSE[6-1] & XTSE_6_01_A_5_NO) || (XTSE[2-1] & XTSE_2_01_A_2_NO) ||
          (XTSE[3-1] & XTSE_3_03_A_3_NO))
      {
         nFirstToneIndex = 6;
         nLastToneIndex  = 31;
      }
      else if ((XTSE[1-1] & (XTSE_1_05_B_1_NO | XTSE_1_02_C_TS_101388)) ||
               (XTSE[3-1] & XTSE_3_05_B_3_NO) || (XTSE[6-1] & XTSE_6_03_B_5_NO))
      {
         nFirstToneIndex = 28;
         nLastToneIndex  = 63;
      }
      else if ((XTSE[4-1] & XTSE_4_05_I_3_NO) || (XTSE[6-1] & XTSE_6_07_I_5_NO))
      {
         nFirstToneIndex = 1;
         nLastToneIndex  = 31;
      }
      else if ((XTSE[5-1] & XTSE_5_03_L_3_NO))
      {
         /* Mask1: 6..24; Mask2: 6..14.*/
         nFirstToneIndex = 6;
         nLastToneIndex  = 24;
      }
      else if ((XTSE[5-1] & XTSE_5_04_L_3_NO))
      {
         /* Mask2: 6..14.*/
         nFirstToneIndex = 6;
         nLastToneIndex  = 14;
      }
      else if ((XTSE[5-1] & XTSE_5_07_M_3_NO) || (XTSE[7-1] & XTSE_7_03_M_5_NO) ||
               (XTSE[4-1] & XTSE_4_07_J_3_NO) || (XTSE[7-1] & XTSE_7_01_J_5_NO))
      {
         DSL_Band_t AnnexMJBandData;

         /* Get Annex M/J mask specific Band Limits*/
         nErrCode = DSL_DRV_DEV_Annex_M_J_UsBandBordersStatusGet(
                       pContext, &AnnexMJBandData);

         if (nErrCode == DSL_SUCCESS)
         {
            nFirstToneIndex = AnnexMJBandData.nFirstToneIndex;
            nLastToneIndex  = AnnexMJBandData.nLastToneIndex;
         }
      }
      else
      {
         nErrCode = DSL_ERROR;
      }
   }

   pData->nFirstToneIndex = nFirstToneIndex;
   pData->nLastToneIndex  = nLastToneIndex;

   return nErrCode;
}
#endif /* defined(INCLUDE_DSL_CPE_MISC_LINE_STATUS) || defined(INCLUDE_DSL_CPE_API_DANUBE)*/

#ifdef INCLUDE_DSL_CPE_MISC_LINE_STATUS
/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_BandBorderStatusGet(
   DSL_Context_t *pContext,
   DSL_BandBorderStatus_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();
   DSL_CHECK_DIRECTION(pData->nDirection);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_BandBorderStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Call device specific implementation*/
   nErrCode = DSL_DRV_DEV_BandBorderStatusGet(pContext, pData);

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_BandBorderStatusGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_MiscLineStatusGet(
   DSL_Context_t *pContext,
   DSL_MiscLineStatus_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_MiscLineStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Call device specific implementation*/
   nErrCode = DSL_DRV_DEV_MiscLineStatusGet(pContext, pData);

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_MiscLineStatusGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return (nErrCode);
}
#endif /* INCLUDE_DSL_CPE_MISC_LINE_STATUS*/

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_LineOptionsConfigSet(
   DSL_Context_t *pContext,
   DSL_LineOptionsConfig_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS, nRet = DSL_SUCCESS;
#ifdef INCLUDE_DSL_CPE_API_DANUBE
   DSL_boolean_t bTrustMeBit;
#endif /* INCLUDE_DSL_CPE_API_DANUBE*/
   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_LineOptionsConfigSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Check configuration option validity*/
   switch (pData->data.nConfigSelector)
   {
   case DSL_OPT_NOISE_MARGIN_DELTA_DS:
      if ((pData->data.nConfigValue < -50) || (pData->data.nConfigValue > 50))
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Noise Margin DS out of range [-50..50]!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         nRet = DSL_ERR_INVALID_PARAMETER;
      }

      if (pData->data.nConfigValue < 0)
      {
         DSL_DEBUG( DSL_DBG_WRN,
            (pContext, SYS_DBG_WRN"DSL[%02d]: WARNING - Negative Noise Margin DS is not recommended!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         nRet = DSL_WRN_NOT_RECOMMENDED_CONFIG;
      }
      break;

   case DSL_ERASURE_DECODING_TYPE_DS:
#ifdef INCLUDE_DSL_CPE_API_DANUBE
      DSL_CTX_READ(pContext, nErrCode, lineOptionsConfig[DSL_TRUST_ME_BIT], bTrustMeBit);

      if ( bTrustMeBit == DSL_TRUE )
      {
         nRet = DSL_WRN_CONFIG_PARAM_IGNORED;
      }
#endif /* INCLUDE_DSL_CPE_API_DANUBE*/
      /*FALL THROUGH*/
   case DSL_TRUST_ME_BIT:
   case DSL_INBAND_SPECTRAL_SHAPING_US:
#ifdef INCLUDE_DSL_CPE_API_DANUBE
      if (pData->data.nConfigValue & 0xFFFFFFFE)
      {
         nRet = DSL_ERR_INVALID_PARAMETER;
      }
#else
      nRet = DSL_ERR_NOT_SUPPORTED_BY_DEVICE;
#endif /* INCLUDE_DSL_CPE_API_DANUBE*/
      break;

   case DSL_ENHANCED_FRAMING_US:
      if ((pData->data.nConfigValue != 0) && (pData->data.nConfigValue != 1))
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Enhanced framing US is out of range [0..1]!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         nRet = DSL_ERR_INVALID_PARAMETER;
      }
      break;

   default:
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Invalid nConfigSelector=%d specified!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), pData->data.nConfigSelector));
      nRet = DSL_ERR_INVALID_PARAMETER;
      break;
   }

   if (nRet >= DSL_SUCCESS)
   {
      /* Set Line Configuration Option*/
      DSL_CTX_WRITE(pContext, nErrCode,
         lineOptionsConfig[pData->data.nConfigSelector],
         pData->data.nConfigValue);
   }

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_LineOptionsConfigSet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nRet));

   return nRet;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
#ifdef INCLUDE_DSL_CONFIG_GET
DSL_Error_t DSL_DRV_LineOptionsConfigGet(
   DSL_Context_t *pContext,
   DSL_LineOptionsConfig_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_LineOptionsConfigGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   switch (pData->data.nConfigSelector)
   {
   case DSL_OPT_NOISE_MARGIN_DELTA_DS:
   case DSL_ENHANCED_FRAMING_US:
      break;

   case DSL_ERASURE_DECODING_TYPE_DS:
   case DSL_TRUST_ME_BIT:
   case DSL_INBAND_SPECTRAL_SHAPING_US:
#ifndef INCLUDE_DSL_CPE_API_DANUBE
      nErrCode = DSL_ERR_NOT_SUPPORTED_BY_DEVICE;
#endif /* INCLUDE_DSL_CPE_API_DANUBE*/
      break;

   default :
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Invalid nConfigSelector=%d specified!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), pData->data.nConfigSelector));

      nErrCode = DSL_ERR_INVALID_PARAMETER;
      break;
   }

   if (nErrCode >= DSL_SUCCESS)
   {
      /* Get Line Configuration Option*/
      DSL_CTX_READ(pContext, nErrCode,
         lineOptionsConfig[pData->data.nConfigSelector],
         pData->data.nConfigValue);
   }

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_LineOptionsConfigGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}
#endif /* INCLUDE_DSL_CONFIG_GET*/

#ifdef INCLUDE_DEVICE_EXCEPTION_CODES
DSL_Error_t DSL_LastExceptionCodesGet(
   DSL_Context_t *pContext,
   DSL_LastExceptionCodes_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_LastExceptionCodesGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Get Last Exception Codes*/
   DSL_CTX_READ(pContext, nErrCode, LastExceptionCodes, pData->data);

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_LastExceptionCodesGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}
#endif /* INCLUDE_DEVICE_EXCEPTION_CODES*/

/*
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_LOW_LEVEL_CONFIGURATION_SET
*/
DSL_Error_t DSL_DRV_LowLevelConfigurationSet(
   DSL_Context_t *pContext,
   DSL_LowLevelConfiguration_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_LowLevelConfigurationSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Call device specific implementation*/
   nErrCode = DSL_DRV_DEV_LowLevelConfigurationSet(pContext, &(pData->data));
#if defined(INCLUDE_DSL_CPE_API_VRX)
   if (nErrCode > DSL_SUCCESS)
   {
      nErrCode = DSL_WRN_CONFIG_PARAM_IGNORED;
   }
#endif

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_LowLevelConfigurationSet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return (nErrCode);
}

/*
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_LOW_LEVEL_CONFIGURATION_GET
*/
DSL_Error_t DSL_DRV_LowLevelConfigurationGet(
   DSL_Context_t *pContext,
   DSL_LowLevelConfiguration_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_LowLevelConfigurationGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Get Low Level Configuration from the device specific context*/
   DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.deviceCfg.cfg, pData->data);

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_LowLevelConfigurationGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return (nErrCode);
}

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_api.h'
*/
#ifdef INCLUDE_DSL_G997_LINE_INVENTORY
DSL_Error_t DSL_DRV_AuxLineInventoryGet(
   DSL_Context_t *pContext,
   DSL_AuxLineInventory_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();
   DSL_CHECK_ATU_DIRECTION(pData->nDirection);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_AuxLineInventoryGet(nDirection=%s)"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), (pData->nDirection==DSL_NEAR_END?"NE":"FE")));

   nErrCode = DSL_DRV_DEV_AuxLineInventoryGet(pContext,pData);

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_AuxLineInventoryGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return (nErrCode);
}
#endif /* INCLUDE_DSL_G997_LINE_INVENTORY*/

/*
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_BAND_PLAN_STATUS_GET
*/
DSL_Error_t DSL_DRV_BandPlanStatusGet(
   DSL_Context_t *pContext,
   DSL_BandPlanStatus_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_LineStateValue_t nCurrentState = DSL_LINESTATE_UNKNOWN;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_BandPlanStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Get current line state*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineState, nCurrentState);

   if ( (nCurrentState == DSL_LINESTATE_SHOWTIME_TC_SYNC) ||
        (nCurrentState == DSL_LINESTATE_SHOWTIME_NO_SYNC) )
   {
      /* Call device specific implementation*/
      nErrCode = DSL_DRV_VRX_BandPlanStatusGet(pContext,&(pData->data));

      /* This parameter is not applicable at the CPE side*/
      pData->data.nBandPlan = DSL_BANDPLAN_NA;
   }
   else
   {
      nErrCode = DSL_ERR_ONLY_AVAILABLE_IN_SHOWTIME;
   }

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_BandPlanStatusGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return (nErrCode);
}

/*
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_BAND_PLAN_SUPPORT_GET
*/
DSL_Error_t DSL_DRV_BandPlanSupportedGet(
   DSL_Context_t *pContext,
   DSL_BandPlanSupport_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_BandPlanSupportedGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Call VRX device specific implementation*/
   nErrCode = DSL_DRV_VRX_BandPlanSupportedGet(pContext, &(pData->data));

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_BandPlanSupportedGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return (nErrCode);
}

#endif /* #if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)*/

/*
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_EVENT_STATUS_MASK_CONFIG_SET
*/
DSL_Error_t DSL_DRV_EventStatusMaskConfigSet(
   DSL_OpenContext_t *pOpenCtx,
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_EventStatusMask_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS, nRet = DSL_SUCCESS;
   DSL_uint32_t nEventMask;
   DSL_boolean_t bFwEv = DSL_FALSE;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_EventStatusMaskConfigSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   if(DSL_DRV_MUTEX_LOCK(pOpenCtx->eventMutex))
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: Couldn't lock event mutex"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return DSL_ERROR;
   }

   /* Check if the event handling is activated for this instance*/
   if (pOpenCtx->bEventActivation)
   {
      /* Get current event mask*/
      nEventMask = pOpenCtx->nEventMask;

      switch(pData->data.nEventType)
      {
      case DSL_EVENT_S_FIRMWARE_REQUEST:
         DSL_CTX_READ_SCALAR(pContext, nErrCode, bFirmwareEventAssigned, bFwEv);

         if (pData->data.bMask)
         {
            /* Mask (disable) DSL_EVENT_S_FIRMWARE_REQUEST event*/
            nEventMask |= DSL_EVENT2MASK(DSL_EVENT_S_FIRMWARE_REQUEST);
            if (bFwEv != DSL_FALSE)
            {
               DSL_CTX_WRITE_SCALAR(pContext, nErrCode, bFirmwareEventAssigned, DSL_FALSE);
            }
         }
         else
         {
            if (bFwEv != DSL_FALSE)
            {
               DSL_DEBUG(DSL_DBG_ERR,
                  (DSL_NULL, SYS_DBG_ERR"DSL[%02d]: Could not assign more than 1 applications "
                             "which handle firmware request event! Event mask will "
                             "be reduced"DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
               /* Mask (disable) DSL_EVENT_S_FIRMWARE_REQUEST event*/
               nEventMask |= DSL_EVENT2MASK(DSL_EVENT_S_FIRMWARE_REQUEST);
               nRet = DSL_ERR_EVENTS_NOT_ACTIVE;
            }
            else
            {
               /* Unmask (enable) DSL_EVENT_S_FIRMWARE_REQUEST event*/
               nEventMask &= ~DSL_EVENT2MASK(DSL_EVENT_S_FIRMWARE_REQUEST);

               DSL_CTX_WRITE_SCALAR(pContext, nErrCode, bFirmwareEventAssigned, DSL_TRUE);
            }
         }
         break;
      case DSL_EVENT_S_LINIT_FAILURE:
      case DSL_EVENT_S_LINE_STATE:
      case DSL_EVENT_S_LINE_POWERMANAGEMENT_STATE:
      case DSL_EVENT_S_CHANNEL_DATARATE:
      case DSL_EVENT_S_FIRMWARE_ERROR:
      case DSL_EVENT_S_INIT_READY:
      case DSL_EVENT_S_FE_INVENTORY_AVAILABLE:
      case DSL_EVENT_S_SYSTEM_STATUS:
      case DSL_EVENT_S_PM_SYNC:
      case DSL_EVENT_S_LINE_TRANSMISSION_STATUS:
      case DSL_EVENT_S_SHOWTIME_LOGGING:
      case DSL_EVENT_S_SNMP_MESSAGE_AVAILABLE:
      case DSL_EVENT_S_SYSTEM_INTERFACE_STATUS:
      case DSL_EVENT_S_FIRMWARE_DOWNLOAD_STATUS:
      case DSL_EVENT_S_AUTOBOOT_STATUS:
      case DSL_EVENT_S_RTT_STATUS:
      case DSL_EVENT_S_FE_TESTPARAMS_AVAILABLE:
         if (pData->data.bMask)
         {
            nEventMask |= DSL_EVENT2MASK(pData->data.nEventType);
         }
         else
         {
            /* Unmask (enable) specified event*/
            nEventMask &= ~DSL_EVENT2MASK(pData->data.nEventType);
         }
         break;
      default:
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - unknown status event(0x%x)"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), pData->data.nEventType));
         nErrCode = DSL_ERROR;
         break;
      }

      if (nErrCode != DSL_ERROR)
      {
         /* Update event mask*/
         pOpenCtx->nEventMask = nEventMask;
      }
   }
   else
   {
      nErrCode = DSL_ERR_EVENTS_NOT_ACTIVE;
   }

   if (nErrCode == DSL_SUCCESS)
   {
      nErrCode = nRet;
   }

   DSL_DRV_MUTEX_UNLOCK(pOpenCtx->eventMutex);

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_EventStatusMaskConfigSet, "
      "retCode(%d)"DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

/*
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_EVENT_STATUS_MASK_CONFIG_GET
*/
#ifdef INCLUDE_DSL_CONFIG_GET
DSL_Error_t DSL_DRV_EventStatusMaskConfigGet(
   DSL_OpenContext_t *pOpenCtx,
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_EventStatusMask_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint32_t nEventMask;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_EventStatusMaskConfigGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   if(DSL_DRV_MUTEX_LOCK(pOpenCtx->eventMutex))
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: Couldn't lock event mutex"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return DSL_ERROR;
   }

   /* Check if the event handling is activated for this instance*/
   if (pOpenCtx->bEventActivation)
   {
      /* Get current event mask*/
      nEventMask = pOpenCtx->nEventMask;

      switch(pData->data.nEventType)
      {
         case DSL_EVENT_S_FIRMWARE_REQUEST:
         case DSL_EVENT_S_LINIT_FAILURE:
         case DSL_EVENT_S_LINE_STATE:
         case DSL_EVENT_S_LINE_POWERMANAGEMENT_STATE:
         case DSL_EVENT_S_CHANNEL_DATARATE:
         case DSL_EVENT_S_FIRMWARE_ERROR:
         case DSL_EVENT_S_INIT_READY:
         case DSL_EVENT_S_FE_INVENTORY_AVAILABLE:
         case DSL_EVENT_S_SYSTEM_STATUS:
         case DSL_EVENT_S_PM_SYNC:
         case DSL_EVENT_S_LINE_TRANSMISSION_STATUS:
         case DSL_EVENT_S_SHOWTIME_LOGGING:
         case DSL_EVENT_S_SNMP_MESSAGE_AVAILABLE:
         case DSL_EVENT_S_SYSTEM_INTERFACE_STATUS:
         case DSL_EVENT_S_FIRMWARE_DOWNLOAD_STATUS:
         case DSL_EVENT_S_AUTOBOOT_STATUS:
         case DSL_EVENT_S_RTT_STATUS:
         case DSL_EVENT_S_FE_TESTPARAMS_AVAILABLE:
            pData->data.bMask = nEventMask & DSL_EVENT2MASK(pData->data.nEventType) ?
                                DSL_TRUE : DSL_FALSE;
            break;
      default:
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - unknown status event(0x%x)"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), pData->data.nEventType));
         nErrCode = DSL_ERROR;
         break;
      }
   }
   else
   {
      nErrCode = DSL_ERR_EVENTS_NOT_ACTIVE;
   }

   DSL_DRV_MUTEX_UNLOCK(pOpenCtx->eventMutex);

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_EventStatusMaskConfigSet, "
      "retCode(%d)"DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}
#endif /* INCLUDE_DSL_CONFIG_GET*/

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
#ifdef INCLUDE_DSL_FRAMING_PARAMETERS
DSL_Error_t DSL_DRV_FramingParameterStatusGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_FramingParameterStatus_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_FramingParameterStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Call device specific stuff*/
   nErrCode = DSL_DRV_DEV_FramingParameterStatusGet(pContext, pData);

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_FramingParameterStatusGet, "
      "retCode(%d)"DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}
#endif /* INCLUDE_DSL_FRAMING_PARAMETERS*/


/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
#ifdef INCLUDE_PILOT_TONES_STATUS
DSL_Error_t DSL_DRV_PilotTonesStatusGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_PilotTonesStatus_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_LineStateValue_t nCurrentState = DSL_LINESTATE_UNKNOWN;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_PilotTonesStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Get current line state*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineState, nCurrentState);

   if ( (nCurrentState == DSL_LINESTATE_SHOWTIME_TC_SYNC) ||
        (nCurrentState == DSL_LINESTATE_SHOWTIME_NO_SYNC) )
   {
      nErrCode = DSL_DRV_DEV_PilotTonesStatusGet(pContext, pData);

      if ((nErrCode >= DSL_SUCCESS) && (pData->data.nNumData == 0))
      {
         nErrCode = DSL_ERR_NOT_SUPPORTED_BY_FIRMWARE;
      }
   }
   else
   {
      nErrCode = DSL_ERR_ONLY_AVAILABLE_IN_SHOWTIME;
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_PilotTonesStatusGet, "
      "retCode(%d)"DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}
#endif /* #ifdef INCLUDE_PILOT_TONES_STATUS*/


/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_RebootCriteriaConfigSet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_RebootCriteriaConfig_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_RebootCriteriaConfigSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   DSL_CTX_WRITE(pContext, nErrCode, RebootCriteriaNeConfigData, pData->data.nRebootCriteria);

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_RebootCriteriaConfigSet, "
      "retCode(%d)"DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_RebootCriteriaConfigGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_RebootCriteriaConfig_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_RebootCriteriaConfigGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   DSL_CTX_READ(pContext, nErrCode, RebootCriteriaNeConfigData, pData->data.nRebootCriteria);

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_RebootCriteriaConfigGet, "
      "retCode(%d)"DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
#ifndef DSL_DEBUG_DISABLE
DSL_Error_t DSL_DRV_DBG_ModuleLevelSet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN DSL_DBG_ModuleLevel_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_int_t i = 0, j = 0;
   DSL_boolean_t bModOk = DSL_FALSE, bLvlOk = DSL_FALSE;
   DSL_debugLevelEntry_t *pDbgEntry = DSL_NULL;
   DSL_debugLevelEntry_t *pDbgLvlNames = DSL_NULL;
#if defined(INCLUDE_DSL_CPE_API_VRX)
   DSL_DEV_Handle_t dev;
   IOCTL_MEI_dbgLevel_t fioVrxDrvDbgLevelSet;
#endif /* INCLUDE_DSL_CPE_API_VRX */

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   /* Check if debug module number is valid */
   pDbgEntry = &DSL_g_dbgLvl[1];
   if ( (pData->data.nDbgModule >= DSL_DBG_LAST_BLOCK) ||
        (pData->data.nDbgModule < DSL_DBG_NO_BLOCK) )
   {
      bModOk = DSL_FALSE;
      DSL_DEBUG_SET_ERROR(DSL_ERROR);
      DSL_DEBUG( DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: DSL_DRV_DBG_ModuleLevelSet - invalid debug module number specified:"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      DSL_DEBUG( DSL_DBG_PRN, (pContext,
         SYS_DBG_PRN"   nDbgModules:" DSL_DRV_CRLF ) );

      for (i = 1; i < (DSL_int_t)DSL_DBG_LAST_BLOCK; i++, pDbgEntry++)
      {
         DSL_DEBUG( DSL_DBG_PRN,
            (pContext, SYS_DBG_PRN"      %d: %s" DSL_DRV_CRLF , i, pDbgEntry->pcName) );
      }
   }
   else
   {
      bModOk = DSL_TRUE;
   }

   /* Check if debug level is valid */
   pDbgLvlNames = &DSL_g_dbgLvlNames[0];
   for (i = 0; i < DSL_g_dbgLvlNumber; i++, pDbgLvlNames++)
   {
      if (pDbgLvlNames->nDbgLvl == pData->data.nDbgLevel)
      {
         /* debug level found  */
         bLvlOk = DSL_TRUE;
         break;
      }
   }
   if ( (bLvlOk == DSL_FALSE) && (i == DSL_g_dbgLvlNumber) )
   {
      /* debug level NOT valid  */
      DSL_DEBUG_SET_ERROR(DSL_ERROR);
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: DSL_DRV_DBG_ModuleLevelSet - invalid debug level specified:"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      DSL_DEBUG( DSL_DBG_PRN,
         (pContext, SYS_DBG_PRN"   nDbgLevels:" DSL_DRV_CRLF ) );
      pDbgLvlNames = &DSL_g_dbgLvlNames[0];
      for (i = 0; i < DSL_g_dbgLvlNumber; i++, pDbgLvlNames++)
      {
         DSL_DEBUG( DSL_DBG_PRN,
            (pContext, SYS_DBG_PRN"      0x%02X: %s" DSL_DRV_CRLF ,
            pDbgLvlNames->nDbgLvl, pDbgLvlNames->pcName) );
      }
   }

   if ( (bModOk == DSL_TRUE) && (bLvlOk == DSL_TRUE) )
   {
      if (pData->data.nDbgModule == DSL_DBG_NO_BLOCK)
      {
         i = 1;
         j = (DSL_int_t)DSL_DBG_LAST_BLOCK;
      }
      else
      {
         i = (DSL_int_t)(pData->data.nDbgModule);
         j = i + 1;
      }

      for ( ; i < j; i++, pDbgEntry++)
      {
         /* Set new debug level for specified module(s) */
         DSL_g_dbgLvl[i].nDbgLvl = pData->data.nDbgLevel;
         DSL_DEBUG( DSL_DBG_LOCAL,
            (pContext, SYS_DBG_LOCAL"DSL[%02d]: new debug level: %s = %s - 0x%02X" DSL_DRV_CRLF ,
                 DSL_DEV_NUM(pContext), DSL_g_dbgLvl[i].pcName,
                 pDbgLvlNames->pcName,
                 pData->data.nDbgLevel) );
      }

#if defined(INCLUDE_DSL_CPE_API_VRX)
      if (pData->data.nDbgModule == DSL_DBG_MESSAGE_DUMP)
      {
         dev = DSL_DEVICE_LOWHANDLE(pContext);
         fioVrxDrvDbgLevelSet.eDbgModule = e_MEI_DBGMOD_MEI_MSG_DUMP_API;
         switch (pData->data.nDbgLevel)
         {
            case DSL_DBG_NONE:
            case DSL_DBG_PRN:
            default:
               fioVrxDrvDbgLevelSet.valLevel = MEI_DBG_LEVEL_OFF;
               break;

            case DSL_DBG_ERR:
            case DSL_DBG_WRN:
            case DSL_DBG_MSG:
               fioVrxDrvDbgLevelSet.valLevel = MEI_DBG_LEVEL_NORMAL;
               break;

            case DSL_DBG_LOCAL:
               fioVrxDrvDbgLevelSet.valLevel = MEI_DBG_LEVEL_LOW;
               break;
         }
         if (DSL_DRV_VRX_InternalDebugLevelSet((MEI_DYN_CNTRL_T*)dev, &fioVrxDrvDbgLevelSet) < 0)
         {
            DSL_DEBUG( DSL_DBG_ERR, (pContext,
               SYS_DBG_ERR"DSL[%02d]: MEI debug level set failed, error %d" DSL_DRV_CRLF,
               DSL_DEV_NUM(pContext), fioVrxDrvDbgLevelSet.ictl.retCode));

            return DSL_ERROR;
         }
      }
      if (pData->data.nDbgModule == DSL_DBG_LOW_LEVEL_DRIVER)
      {
         dev = DSL_DEVICE_LOWHANDLE(pContext);
         fioVrxDrvDbgLevelSet.eDbgModule = e_MEI_DBGMOD_MEI_DRV;
         switch (pData->data.nDbgLevel)
         {
            case DSL_DBG_NONE:
            case DSL_DBG_PRN:
            default:
               fioVrxDrvDbgLevelSet.valLevel = MEI_DBG_LEVEL_OFF;
               break;

            case DSL_DBG_ERR:
               fioVrxDrvDbgLevelSet.valLevel = MEI_DBG_LEVEL_HIGH;
               break;

            case DSL_DBG_WRN:
               fioVrxDrvDbgLevelSet.valLevel = MEI_DBG_LEVEL_NORMAL;
               break;

            case DSL_DBG_MSG:
            case DSL_DBG_LOCAL:
               fioVrxDrvDbgLevelSet.valLevel = MEI_DBG_LEVEL_LOW;
               break;
         }
         if (DSL_DRV_VRX_InternalDebugLevelSet((MEI_DYN_CNTRL_T*)dev, &fioVrxDrvDbgLevelSet) < 0)
         {
            DSL_DEBUG( DSL_DBG_ERR, (pContext,
               SYS_DBG_ERR"DSL[%02d]: MEI debug level set failed, error %d" DSL_DRV_CRLF,
               DSL_DEV_NUM(pContext), fioVrxDrvDbgLevelSet.ictl.retCode));

            return DSL_ERROR;
         }
      }
#endif /* INCLUDE_DSL_CPE_API_VRX */
   }
   else
   {
      nErrCode = DSL_ERROR;
   }

   return nErrCode;
}
#endif /* DSL_DEBUG_DISABLE */

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
#ifndef DSL_DEBUG_DISABLE
DSL_Error_t DSL_DRV_DBG_ModuleLevelGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_DBG_ModuleLevel_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_boolean_t bLvlOk = DSL_FALSE;
   DSL_debugLevelEntry_t *pDbgEntry = DSL_NULL;
   DSL_debugLevelEntry_t *pDbgLvlNames = DSL_NULL;
   DSL_int_t i = 0;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   /* Check if debug module number is valid */
   pDbgEntry = &DSL_g_dbgLvl[1];
   if ( (pData->data.nDbgModule >= DSL_DBG_LAST_BLOCK) ||
        (pData->data.nDbgModule == DSL_DBG_NO_BLOCK) )
   {
      nErrCode = DSL_ERROR;
      DSL_DEBUG_SET_ERROR(nErrCode);
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: DSL_DRV_DBG_ModuleLevelGet - "
         "invalid debug module number specified:" DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));
      DSL_DEBUG( DSL_DBG_PRN,
         (pContext, SYS_DBG_PRN"   nDbgModules:" DSL_DRV_CRLF ) );
      for (i = 1; i < (DSL_int_t)DSL_DBG_LAST_BLOCK; i++, pDbgEntry++)
      {
         DSL_DEBUG( DSL_DBG_PRN,
            (pContext, SYS_DBG_PRN"      %d: %s" DSL_DRV_CRLF , i, pDbgEntry->pcName) );
      }
   }
   else
   {
      pData->data.nDbgLevel = DSL_g_dbgLvl[pData->data.nDbgModule].nDbgLvl;

      pDbgLvlNames = &DSL_g_dbgLvlNames[0];
      for (i = 0; i < DSL_g_dbgLvlNumber; i++, pDbgLvlNames++)
      {
         if (pDbgLvlNames->nDbgLvl == pData->data.nDbgLevel)
         {
            /* debug level found  */
            bLvlOk = DSL_TRUE;
            DSL_DEBUG( DSL_DBG_LOCAL,
               (pContext, SYS_DBG_LOCAL"DSL[%02d]: actual debug level: %s = %s - 0x%02X" DSL_DRV_CRLF,
                    DSL_DEV_NUM(pContext), DSL_g_dbgLvl[pData->data.nDbgModule].pcName,
                    pDbgLvlNames->pcName,
                    pData->data.nDbgLevel) );
            break;
         }
      }

      if (bLvlOk == DSL_FALSE)
      {
         DSL_DEBUG( DSL_DBG_LOCAL,
            (pContext, SYS_DBG_LOCAL"DSL[%02d]: actual debug level: %s = 0x%02X" DSL_DRV_CRLF,
                 DSL_DEV_NUM(pContext), DSL_g_dbgLvl[pData->data.nDbgModule].pcName,
                 pData->data.nDbgLevel) );
      }
   }

   return nErrCode;
}
#endif /* DSL_DEBUG_DISABLE*/

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_DBG_DeviceMessageSend(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_DeviceMessage_t *pMsg)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CHECK_POINTER(pContext, pMsg);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DBG_DeviceMessageSend"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   nErrCode = DSL_DRV_DEV_DBG_DeviceMessageSend(pContext, &(pMsg->data));

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DBG_DeviceMessageSend, retCode(%d)"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

#ifndef DSL_DEBUG_DISABLE
#if defined(INCLUDE_DSL_CPE_API_DANUBE)
DSL_Error_t DSL_DRV_DBG_DebugFeatureConfigSet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN DSL_DBG_DebugFeatureConfig_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_WRN,
      (DSL_NULL, SYS_DBG_WRN"DSL[%02d]: WARNING - No debug features are defined."
      DSL_DRV_CRLF, pOpenContextList->pDevCtx->nNum));

   return (nErrCode);
}

#ifdef INCLUDE_DSL_CONFIG_GET
DSL_Error_t DSL_DRV_DBG_DebugFeatureConfigGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN DSL_DBG_DebugFeatureConfig_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_WRN,
      (DSL_NULL, SYS_DBG_WRN"DSL[%02d]: WARNING - No debug features are defined."
      DSL_DRV_CRLF, pOpenContextList->pDevCtx->nNum));

   return (nErrCode);
}
#endif /* INCLUDE_DSL_CONFIG_GET*/
#endif /* defined(INCLUDE_DSL_CPE_API_DANUBE)*/
#endif /* DSL_DEBUG_DISABLE*/

#ifdef INCLUDE_DSL_FILTER_DETECTION
DSL_Error_t DSL_DRV_FilterDetectionDataGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_OUT DSL_FilterDetection_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_boolean_t bFilterDetectionCompleted = DSL_FALSE;
   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, "DSL[%02d]: IN - DSL_DRV_FilterDetectionDataGet" DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext)));

#ifdef INCLUDE_DSL_CPE_API_VRX
   /* Only for ADSL mode */
   if (!DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSL))
   {
      DSL_DEBUG(DSL_DBG_ERR, (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - "
               "hybrid/filter/looplength statistics are not supported "
               "in current VDSL mode" DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERR_NOT_SUPPORTED_IN_CURRENT_VDSL_MODE;
   }
#endif /* INCLUDE_DSL_CPE_API_VRX */

   DSL_CTX_READ(pContext, nErrCode, showtimeMeasurement.bFilterDetectionCompleted, bFilterDetectionCompleted);

   if (bFilterDetectionCompleted)
   {
        DSL_CTX_READ(pContext, nErrCode, showtimeMeasurement.filterDetectionData, pData->data);

      /* If data is not completely available generate warning*/
      if (pData->data.nNonLinearEchoMetric2 == 128)
      {
         nErrCode = DSL_WRN_INCOMPLETE_RETURN_VALUES;
      }
   }
   else
   {
      nErrCode = DSL_ERR_DEVICE_NO_DATA;
   }

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, "DSL[%02d]: OUT - DSL_DRV_FilterDetectionDataGet, retCode=%d"DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}
#endif /* INCLUDE_DSL_FILTER_DETECTION */

DSL_Error_t DSL_DRV_HybridSelectionDataGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_OUT DSL_HybridSelection_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, "DSL[%02d]: IN - DSL_DRV_HybridSelectionDataGet" DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext)));

#ifdef INCLUDE_DSL_CPE_API_VRX
   /* Only for ADSL mode */
   if (!DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSL))
   {
      DSL_DEBUG(DSL_DBG_ERR, (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - "
               "hybrid/filter/looplength statistics are not supported "
               "in current VDSL mode" DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERR_NOT_SUPPORTED_IN_CURRENT_VDSL_MODE;
   }
#endif /* INCLUDE_DSL_CPE_API_VRX */

   DSL_CTX_READ(pContext, nErrCode, showtimeMeasurement.hybridSelectionData, pData->data);

   /* If data is not completely available return a warning, if data is not
      available at all return an error. */
   if ((pData->data.actualSelection.nHybridIndex == -1) ||
       (pData->data.secondBestSelection.nHybridIndex == -1) ||
       (pData->data.fdActualSelection.nHybridIndex == -1) ||
       (pData->data.fdSecondBestSelection.nHybridIndex == -1))
   {
      if ((pData->data.actualSelection.nHybridIndex == -1) &&
          (pData->data.secondBestSelection.nHybridIndex == -1) &&
          (pData->data.fdActualSelection.nHybridIndex == -1) &&
          (pData->data.fdSecondBestSelection.nHybridIndex == -1))
      {
         nErrCode = DSL_ERR_DEVICE_NO_DATA;
      }
      else
      {
         nErrCode = DSL_WRN_INCOMPLETE_RETURN_VALUES;
      }
   }

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, "DSL[%02d]: OUT - DSL_DRV_HybridSelectionDataGet, retCode=%d"DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

DSL_Error_t DSL_DRV_OlrStatisticsGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_OUT DSL_OlrStatistics_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();
   DSL_CHECK_DIRECTION(pData->nDirection);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, "DSL[%02d]: IN - DSL_DRV_OlrStatisticsGet" DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext)));

   nErrCode = DSL_DRV_DEV_OlrStatisticsGet(pContext, pData);

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, "DSL[%02d]: OUT - DSL_DRV_OlrStatisticsGet, retCode=%d"DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_InitDataPrepare(
   DSL_Context_t *pContext,
   DSL_boolean_t bIsInKernel,
   DSL_Init_t *pInit)
{
   DSL_G997_LineInventoryNeData_t *pInv = DSL_NULL;
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CHECK_POINTER(pContext, pInit);
   DSL_CHECK_ERR_CODE();

#ifndef INCLUDE_FW_REQUEST_SUPPORT
   /* Store FW in the DSL CPE internal memory*/
   nErrCode = DSL_DRV_FwStore(
                 pContext, bIsInKernel,
                 (DSL_char_t*)pInit->data.pFirmware, pInit->data.nFirmwareSize,
                 (DSL_char_t*)pInit->data.pFirmware2, pInit->data.nFirmwareSize2);

   if( nErrCode != DSL_SUCCESS)
   {
      return nErrCode;
   }
#endif /* INCLUDE_FW_REQUEST_SUPPORT*/

   if (pInit->data.pInventory != DSL_NULL)
   {
      pInv = DSL_DRV_PMalloc(sizeof(DSL_G997_LineInventoryNeData_t));
      if (pInv == DSL_NULL)
      {
         nErrCode = DSL_ERR_MEMORY;
      }
      else
      {
         DSL_IoctlMemCpyFrom(bIsInKernel, pInv, pInit->data.pInventory,
            sizeof(DSL_G997_LineInventoryNeData_t));
         pInit->data.pInventory = pInv;
      }
   }

#ifdef INCLUDE_DSL_INIT_DATA_PREPARE
   if (nErrCode == DSL_SUCCESS)
   {
      nErrCode = DSL_DRV_DEV_InitDataPrepare(pContext, bIsInKernel, pInit);
   }
#endif /* INCLUDE_DSL_INIT_DATA_PREPARE*/

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_void_t DSL_DRV_InitDataFree(
   DSL_Context_t *pContext,
   DSL_Init_t *pInit)
{
   if (pInit == DSL_NULL)
   {
      return;
   }

   if (pInit->data.pInventory != DSL_NULL)
   {
      DSL_DRV_PFree(pInit->data.pInventory);
   }

#ifdef INCLUDE_FW_REQUEST_SUPPORT
#ifndef INCLUDE_DSL_DRV_STATIC_LINKED_FIRMWARE
   /* Remove stored FW from the DSL CPE context. The FW binary will
       be requested via the corresponding event further if necessary*/
   if( pContext->pFirmware != DSL_NULL )
   {
      DSL_DRV_VFree(pContext->pFirmware);
      pContext->pFirmware     = DSL_NULL;
      pContext->nFirmwareSize = 0;
   }

   if( pContext->pFirmware2 != DSL_NULL )
   {
      DSL_DRV_VFree(pContext->pFirmware2);
      pContext->pFirmware2     = DSL_NULL;
      pContext->nFirmwareSize2 = 0;
   }
#endif /* #ifndef INCLUDE_DSL_DRV_STATIC_LINKED_FIRMWARE*/
#endif /* #ifdef INCLUDE_FW_REQUEST_SUPPORT*/

#ifdef INCLUDE_DSL_INIT_DATA_PREPARE
   DSL_DRV_DEV_InitDataFree(pInit);
#endif /* INCLUDE_DSL_INIT_DATA_PREPARE*/
}

DSL_Error_t DSL_DRV_IoctlHandleHelperCall(
   DSL_OpenContext_t *pOpenCtx,
   DSL_Context_t *pContext,
   DSL_boolean_t bIsInKernel,
   DSL_boolean_t bInstanceCall,
   DSL_IoctlHandlerHelperType_t nType,
   DSL_void_t *pFunc,
   DSL_void_t *pArg,
   DSL_uint32_t nArgSz)
{
   DSL_Error_t nErrCode = DSL_ERR_MEMORY;
   DSL_IOCTL_arg_t *pIOCTL_arg = DSL_NULL;

   if (nType != DSL_IOCTL_HELPER_GET && nType != DSL_IOCTL_HELPER_SET)
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Invalid parameter was passed to "
         "the IOCTL helper!"DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return DSL_ERR_INVALID_PARAMETER;
   }

   if (pArg == DSL_NULL || nArgSz == 0)
   {
      nErrCode = DSL_ERR_INVALID_PARAMETER;
   }
   else
   {
      pIOCTL_arg = DSL_DRV_VMalloc(nArgSz);
      if(pIOCTL_arg == DSL_NULL)
      {
         DSL_IoctlMemCpyTo( bIsInKernel, pArg, &nErrCode, sizeof(DSL_Error_t));
         return nErrCode;
      }

      DSL_IoctlMemCpyFrom(bIsInKernel, pIOCTL_arg, pArg,
         nArgSz);

      if(bInstanceCall)
      {
         /* Lock Open Context list*/
         if (DSL_DRV_MUTEX_LOCK(pContext->pDevCtx->openContextListMutex))
         {
            DSL_DEBUG( DSL_DBG_ERR,
               (DSL_NULL, SYS_DBG_ERR"DSL[%02d]: ERROR - Couldn't lock Context List mutex!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            nErrCode = DSL_ERR_SEMAPHORE_GET;
         }
         else
         {
            /* Call Instance specific IOCTL*/
            nErrCode = ((DSL_DRV_IoctlInstanceHandlerHelperFunc_t)pFunc)(pOpenCtx, pContext, pIOCTL_arg);

            /* Unlock Open Context list*/
            DSL_DRV_MUTEX_UNLOCK(pContext->pDevCtx->openContextListMutex);
         }
      }
      else
      {
         nErrCode = ((DSL_DRV_IoctlHandlerHelperFunc_t)pFunc)(pContext, pIOCTL_arg);
      }

      if (nErrCode >= DSL_SUCCESS)
      {
         if (nType == DSL_IOCTL_HELPER_GET)
         {
            pIOCTL_arg->accessCtl.nReturn = nErrCode;

            DSL_IoctlMemCpyTo( bIsInKernel, pArg, pIOCTL_arg,
               nArgSz);
         }
         else
         {
            DSL_IoctlMemCpyTo( bIsInKernel, pArg, &nErrCode,
               sizeof(DSL_Error_t));
         }
      }
      else
      {
         DSL_IoctlMemCpyTo( bIsInKernel, pArg, &nErrCode, sizeof(DSL_Error_t));
      }

      DSL_DRV_VFree(pIOCTL_arg);
   }

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
#ifdef INCLUDE_REAL_TIME_TRACE
DSL_Error_t DSL_DRV_RTT_Init(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN DSL_RTT_Init_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_boolean_t bRttEnabled = DSL_FALSE;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_RTT_Init"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Check RTT functionality*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, bRttEnabled, bRttEnabled);

   if (bRttEnabled == DSL_FALSE)
   {
      nErrCode = DSL_ERR_RTT_NOT_AVAILABLE;
   }
   else
   {
      nErrCode = DSL_DRV_DEV_RTT_Init(pContext, &(pData->data));
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_RTT_Init"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_RTT_ConfigSet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN DSL_RTT_Config_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_boolean_t bRttEnabled = DSL_FALSE;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_RTT_ConfigSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Check RTT functionality*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, bRttEnabled, bRttEnabled);

   if (bRttEnabled == DSL_FALSE)
   {
      nErrCode = DSL_ERR_RTT_NOT_AVAILABLE;
   }
   else
   {
      nErrCode = DSL_DRV_DEV_RTT_ConfigSet(pContext, &(pData->data));
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_RTT_ConfigSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_RTT_ConfigGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_RTT_Config_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_boolean_t bRttEnabled = DSL_FALSE;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_RTT_ConfigGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Check RTT functionality*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, bRttEnabled, bRttEnabled);

   if (bRttEnabled == DSL_FALSE)
   {
      nErrCode = DSL_ERR_RTT_NOT_AVAILABLE;
   }
   else
   {
      nErrCode = DSL_DRV_DEV_RTT_ConfigGet(pContext, &(pData->data));
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_RTT_ConfigGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_RTT_StatusGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_RTT_Status_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_RTT_StatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   DSL_CTX_READ_SCALAR(pContext, nErrCode, nRttFwStatus, pData->data.nFwStatus);

   DSL_CTX_READ_SCALAR(pContext, nErrCode, nRttFwFailReason, pData->data.nFwFailReason);

   DSL_CTX_READ_SCALAR(pContext, nErrCode, nRttSwErrorCode, pData->data.nSwErrorCode);

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_RTT_StatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_RTT_ControlSet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN DSL_RTT_Control_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_boolean_t bRttEnabled = DSL_FALSE;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_RTT_ControlSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Check RTT functionality*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, bRttEnabled, bRttEnabled);

   if (bRttEnabled == DSL_FALSE)
   {
      nErrCode = DSL_ERR_RTT_NOT_AVAILABLE;
   }
   else
   {
      nErrCode = DSL_DRV_DEV_RTT_ControlSet(pContext, &(pData->data));
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_RTT_ControlSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_RTT_StatisticsGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_RTT_Statistics_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_boolean_t bRttEnabled = DSL_FALSE;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_RTT_StatisticsGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Check RTT functionality*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, bRttEnabled, bRttEnabled);

   if (bRttEnabled == DSL_FALSE)
   {
      nErrCode = DSL_ERR_RTT_NOT_AVAILABLE;
   }
   else
   {
      nErrCode = DSL_DRV_DEV_RTT_StatisticsGet(pContext, &(pData->data));
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_RTT_StatisticsGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}
#endif /* #ifdef INCLUDE_REAL_TIME_TRACE */

/*  DSL CPE API Ioctl Table */
DSL_IOCTL_Table_t ioctlTable[] = {
/* DSL_FIO_AUTOBOOT_LOAD_FIRMWARE */
DSL_IOCTL_REGISTER(DSL_FIO_AUTOBOOT_LOAD_FIRMWARE, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_AutobootLoadFirmware,
                   sizeof(DSL_AutobootLoadFirmware_t)),
/* DSL_FIO_AUTOBOOT_CONTROL_SET */
DSL_IOCTL_REGISTER(DSL_FIO_AUTOBOOT_CONTROL_SET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_AutobootControlSet,
                   sizeof(DSL_AutobootControl_t)),
/* DSL_FIO_AUTOBOOT_STATUS_GET */
DSL_IOCTL_REGISTER(DSL_FIO_AUTOBOOT_STATUS_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_AutobootStatusGet,
                   sizeof(DSL_AutobootStatus_t)),
/* DSL_FIO_LINE_STATE_GET */
DSL_IOCTL_REGISTER(DSL_FIO_LINE_STATE_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_LineStateGet,
                   sizeof(DSL_LineState_t)),
/* DSL_FIO_AUTOBOOT_CONFIG_SET */
DSL_IOCTL_REGISTER(DSL_FIO_AUTOBOOT_CONFIG_SET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_AutobootConfigSet,
                   sizeof(DSL_AutobootConfig_t)),
/* DSL_FIO_LINE_FEATURE_CONFIG_SET */
DSL_IOCTL_REGISTER(DSL_FIO_LINE_FEATURE_CONFIG_SET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_LineFeatureConfigSet,
                   sizeof(DSL_LineFeature_t)),
/* DSL_FIO_G997_XTU_SYSTEM_ENABLING_CONFIG_SET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_XTU_SYSTEM_ENABLING_CONFIG_SET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_G997_XTUSystemEnablingConfigSet,
                   sizeof(DSL_G997_XTUSystemEnabling_t)),
/* DSL_FIO_G997_XTU_SYSTEM_ENABLING_STATUS_GET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_XTU_SYSTEM_ENABLING_STATUS_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_G997_XTUSystemEnablingStatusGet,
                   sizeof(DSL_G997_XTUSystemEnabling_t)),

#ifdef INCLUDE_DSL_CONFIG_GET
/* DSL_FIO_G997_XTU_SYSTEM_ENABLING_CONFIG_GET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_XTU_SYSTEM_ENABLING_CONFIG_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_G997_XTUSystemEnablingConfigGet,
                   sizeof(DSL_G997_XTUSystemEnabling_t)),
/* DSL_FIO_AUTOBOOT_CONFIG_GET */
DSL_IOCTL_REGISTER(DSL_FIO_AUTOBOOT_CONFIG_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_AutobootConfigGet,
                   sizeof(DSL_AutobootConfig_t)),
/* DSL_FIO_LINE_FEATURE_CONFIG_GET */
DSL_IOCTL_REGISTER(DSL_FIO_LINE_FEATURE_CONFIG_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_LineFeatureConfigGet,
                   sizeof(DSL_LineFeature_t)),
#ifdef INCLUDE_DSL_CPE_API_DANUBE
/* DSL_FIO_INTEROP_FEATURE_CONFIG_GET */
DSL_IOCTL_REGISTER(DSL_FIO_INTEROP_FEATURE_CONFIG_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_InteropFeatureConfigGet,
                   sizeof(DSL_InteropFeatureConfig_t)),
#endif /* INCLUDE_DSL_CPE_API_DANUBE*/
/* DSL_FIO_LOOP_LENGTH_STATUS_GET */
DSL_IOCTL_REGISTER(DSL_FIO_LOOP_LENGTH_STATUS_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_LoopLengthStatusGet,
                   sizeof(DSL_LoopLengthStatus_t)),
/* DSL_FIO_EVENT_STATUS_MASK_CONFIG_GET */
DSL_IOCTL_REGISTER(DSL_FIO_EVENT_STATUS_MASK_CONFIG_GET, DSL_IOCTL_HELPER_GET,
                   DSL_TRUE, DSL_DRV_EventStatusMaskConfigGet,
                   sizeof(DSL_EventStatusMask_t)),
#endif /* INCLUDE_DSL_CONFIG_GET*/
/* DSL_FIO_EVENT_STATUS_MASK_CONFIG_SET */
DSL_IOCTL_REGISTER(DSL_FIO_EVENT_STATUS_MASK_CONFIG_SET, DSL_IOCTL_HELPER_SET,
                   DSL_TRUE, DSL_DRV_EventStatusMaskConfigSet,
                   sizeof(DSL_EventStatusMask_t)),
#ifdef INCLUDE_DSL_SYSTEM_INTERFACE

/* DSL_FIO_SYSTEM_INTERFACE_CONFIG_SET */
DSL_IOCTL_REGISTER(DSL_FIO_SYSTEM_INTERFACE_CONFIG_SET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_SystemInterfaceConfigSet,
                   sizeof(DSL_SystemInterfaceConfig_t)),
#ifdef INCLUDE_DSL_CONFIG_GET
/* DSL_FIO_SYSTEM_INTERFACE_CONFIG_GET */
DSL_IOCTL_REGISTER(DSL_FIO_SYSTEM_INTERFACE_CONFIG_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_SystemInterfaceConfigGet,
                   sizeof(DSL_SystemInterfaceConfig_t)),
#endif /* INCLUDE_DSL_CONFIG_GET*/
/* DSL_FIO_SYSTEM_INTERFACE_STATUS_GET */
DSL_IOCTL_REGISTER(DSL_FIO_SYSTEM_INTERFACE_STATUS_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_SystemInterfaceStatusGet,
                   sizeof(DSL_SystemInterfaceStatus_t)),
#endif /* INCLUDE_DSL_SYSTEM_INTERFACE*/

#ifdef INCLUDE_DSL_CPE_MISC_LINE_STATUS
/* DSL_FIO_BAND_BORDER_STATUS_GET */
DSL_IOCTL_REGISTER(DSL_FIO_BAND_BORDER_STATUS_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_BandBorderStatusGet,
                   sizeof(DSL_BandBorderStatus_t)),

/* DSL_FIO_MISC_LINE_STATUS_GET */
DSL_IOCTL_REGISTER(DSL_FIO_MISC_LINE_STATUS_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_MiscLineStatusGet,
                   sizeof(DSL_MiscLineStatus_t)),
#endif /* INCLUDE_DSL_CPE_MISC_LINE_STATUS*/

DSL_IOCTL_REGISTER(DSL_FIO_LINE_OPTIONS_CONFIG_SET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_LineOptionsConfigSet,
                   sizeof(DSL_LineOptionsConfig_t)),
#ifdef INCLUDE_DSL_CONFIG_GET
DSL_IOCTL_REGISTER(DSL_FIO_LINE_OPTIONS_CONFIG_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_LineOptionsConfigGet,
                   sizeof(DSL_LineOptionsConfig_t)),
#endif /* INCLUDE_DSL_CONFIG_GET*/

#ifdef INCLUDE_DEVICE_EXCEPTION_CODES
DSL_IOCTL_REGISTER(DSL_FIO_LAST_EXCEPTION_CODES_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_LastExceptionCodesGet,
                   sizeof(DSL_LastExceptionCodes_t)),
#endif /* INCLUDE_DEVICE_EXCEPTION_CODES*/

#ifdef INCLUDE_DSL_CPE_API_DANUBE
#ifndef DSL_DEBUG_DISABLE
DSL_IOCTL_REGISTER(DSL_FIO_DBG_DEBUG_FEATURE_CONFIG_SET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_DBG_DebugFeatureConfigSet,
                   sizeof(DSL_DBG_DebugFeatureConfig_t)),

#ifdef INCLUDE_DSL_CONFIG_GET
DSL_IOCTL_REGISTER(DSL_FIO_DBG_DEBUG_FEATURE_CONFIG_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_DBG_DebugFeatureConfigGet,
                   sizeof(DSL_DBG_DebugFeatureConfig_t)),
#endif /* INCLUDE_DSL_CONFIG_GET*/
#endif /* DSL_DEBUG_DISABLE*/
#endif /* INCLUDE_DSL_CPE_API_DANUBE*/

#ifdef INCLUDE_DSL_FILTER_DETECTION
DSL_IOCTL_REGISTER(DSL_FIO_FILTER_DETECTION_DATA_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_FilterDetectionDataGet,
                   sizeof(DSL_FilterDetection_t)),
#endif /* INCLUDE_DSL_FILTER_DETECTION */

DSL_IOCTL_REGISTER(DSL_FIO_HYBRID_SELECTION_DATA_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_HybridSelectionDataGet,
                   sizeof(DSL_HybridSelection_t)),

DSL_IOCTL_REGISTER(DSL_FIO_OLR_STATISTICS_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_OlrStatisticsGet,
                   sizeof(DSL_OlrStatistics_t)),

#ifdef INCLUDE_DSL_PM
#ifdef INCLUDE_DSL_CPE_PM_CONFIG
DSL_IOCTL_REGISTER(DSL_FIO_PM_CONFIG_SET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_PM_ConfigSet,
                   sizeof(DSL_PM_Config_t)),
#ifdef INCLUDE_DSL_CONFIG_GET
DSL_IOCTL_REGISTER(DSL_FIO_PM_CONFIG_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_ConfigGet,
                   sizeof(DSL_PM_Config_t)),
#endif /* INCLUDE_DSL_CONFIG_GET*/
#endif /* INCLUDE_DSL_CPE_PM_CONFIG*/

#ifdef INCLUDE_DSL_CPE_PM_HISTORY
#ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS
/* DSL_FIO_PM_ELAPSED_TIME_RESET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_ELAPSED_TIME_RESET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_PM_ElapsedTimeReset,
                   sizeof(DSL_PM_ElapsedTimeReset_t)),
/* DSL_FIO_PM_BURNIN_MODE_SET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_BURNIN_MODE_SET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_PM_BurninModeSet,
                   sizeof(DSL_PM_BurninMode_t)),
/* DSL_FIO_PM_SYNC_MODE_SET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_SYNC_MODE_SET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_PM_SyncModeSet,
                   sizeof(DSL_PM_SyncMode_t)),
#ifdef INCLUDE_DSL_CONFIG_GET
/* DSL_FIO_PM_SYNC_MODE_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_SYNC_MODE_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_SyncModeGet,
                   sizeof(DSL_PM_SyncMode_t)),
#endif /* INCLUDE_DSL_CONFIG_GET*/
/* DSL_FIO_PM_15MIN_ELAPSED_EXT_TRIGGER */
DSL_IOCTL_REGISTER(DSL_FIO_PM_15MIN_ELAPSED_EXT_TRIGGER, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_PM_15MinElapsedExtTrigger,
                   sizeof(DSL_PM_ElapsedExtTrigger_t)),
#endif /* #ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS*/
/* DSL_FIO_PM_RESET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_RESET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_PM_Reset,
                   sizeof(DSL_PM_Reset_t)),
#endif /* #ifdef INCLUDE_DSL_CPE_PM_HISTORY*/

#ifdef INCLUDE_DSL_CPE_PM_CHANNEL_COUNTERS
#ifdef INCLUDE_DSL_CPE_PM_HISTORY
/* DSL_FIO_PM_CHANNEL_COUNTERS_15MIN_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_CHANNEL_COUNTERS_15MIN_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_ChannelCounters15MinGet,
                   sizeof(DSL_PM_ChannelCounters_t)),
/* DSL_FIO_PM_CHANNEL_COUNTERS_1DAY_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_CHANNEL_COUNTERS_1DAY_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_ChannelCounters1DayGet,
                   sizeof(DSL_PM_ChannelCounters_t)),
#endif /* #ifdef INCLUDE_DSL_CPE_PM_HISTORY*/
#ifdef INCLUDE_DSL_CPE_PM_TOTAL_COUNTERS
/* DSL_FIO_PM_CHANNEL_COUNTERS_TOTAL_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_CHANNEL_COUNTERS_TOTAL_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_ChannelCountersTotalGet,
                   sizeof(DSL_PM_ChannelCountersTotal_t)),
#endif /* #ifdef INCLUDE_DSL_CPE_PM_TOTAL_COUNTERS*/

#ifdef INCLUDE_DSL_CPE_PM_SHOWTIME_COUNTERS
/* DSL_FIO_PM_CHANNEL_COUNTERS_SHOWTIME_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_CHANNEL_COUNTERS_SHOWTIME_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_ChannelCountersShowtimeGet,
                   sizeof(DSL_PM_ChannelCounters_t)),
#ifdef INCLUDE_DSL_CPE_PM_CHANNEL_EXT_COUNTERS
/* DSL_FIO_PM_CHANNEL_COUNTERS_SHOWTIME_EXT_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_CHANNEL_COUNTERS_SHOWTIME_EXT_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_ChannelCountersExtShowtimeGet,
                   sizeof(DSL_PM_ChannelCountersExt_t)),
#endif /* INCLUDE_DSL_CPE_PM_CHANNEL_EXT_COUNTERS*/
#endif /* #ifdef INCLUDE_DSL_CPE_PM_SHOWTIME_COUNTERS*/
#ifdef INCLUDE_DSL_CPE_PM_HISTORY
#ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS
/* DSL_FIO_PM_CHANNEL_HISTORY_STATS_15MIN_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_CHANNEL_HISTORY_STATS_15MIN_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_ChannelHistoryStats15MinGet,
                   sizeof(DSL_PM_HistoryStatsChDir_t)),
/* DSL_FIO_PM_CHANNEL_HISTORY_STATS_1DAY_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_CHANNEL_HISTORY_STATS_1DAY_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_ChannelHistoryStats1DayGet,
                   sizeof(DSL_PM_HistoryStatsChDir_t)),
#endif /* #ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS*/
#endif /* #ifdef INCLUDE_DSL_CPE_PM_HISTORY*/
#ifdef INCLUDE_DSL_CPE_PM_CHANNEL_THRESHOLDS
#ifdef INCLUDE_DSL_CPE_PM_HISTORY
#ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS
/* DSL_FIO_PM_CHANNEL_THRESHOLDS_15MIN_SET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_CHANNEL_THRESHOLDS_15MIN_SET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_PM_ChannelThresholds15MinSet,
                   sizeof(DSL_PM_ChannelThreshold_t)),
/* DSL_FIO_PM_CHANNEL_THRESHOLDS_1DAY_SET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_CHANNEL_THRESHOLDS_1DAY_SET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_PM_ChannelThresholds1DaySet,
                   sizeof(DSL_PM_ChannelThreshold_t)),
/* DSL_FIO_PM_CHANNEL_THRESHOLDS_15MIN_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_CHANNEL_THRESHOLDS_15MIN_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_ChannelThresholds15MinGet,
                   sizeof(DSL_PM_ChannelThreshold_t)),
/* DSL_FIO_PM_CHANNEL_THRESHOLDS_1DAY_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_CHANNEL_THRESHOLDS_1DAY_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_ChannelThresholds1DayGet,
                   sizeof(DSL_PM_ChannelThreshold_t)),
#endif /* INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS*/
#endif /* INCLUDE_DSL_CPE_PM_HISTORY*/
#endif /* INCLUDE_DSL_CPE_PM_CHANNEL_THRESHOLDS*/
#endif /* INCLUDE_DSL_CPE_PM_CHANNEL_COUNTERS*/

#ifdef INCLUDE_DSL_CPE_PM_DATA_PATH_COUNTERS
#ifdef INCLUDE_DSL_CPE_PM_HISTORY
/* DSL_FIO_PM_DATA_PATH_COUNTERS_15MIN_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_DATA_PATH_COUNTERS_15MIN_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_DataPathCounters15MinGet,
                   sizeof(DSL_PM_DataPathCounters_t)),
/* DSL_FIO_PM_DATA_PATH_COUNTERS_1DAY_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_DATA_PATH_COUNTERS_1DAY_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_DataPathCounters1DayGet,
                   sizeof(DSL_PM_DataPathCounters_t)),
#endif /* INCLUDE_DSL_CPE_PM_HISTORY*/
#ifdef INCLUDE_DSL_CPE_PM_TOTAL_COUNTERS
/* DSL_FIO_PM_DATA_PATH_COUNTERS_TOTAL_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_DATA_PATH_COUNTERS_TOTAL_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_DataPathCountersTotalGet,
                   sizeof(DSL_PM_DataPathCountersTotal_t)),
#endif /* INCLUDE_DSL_CPE_PM_TOTAL_COUNTERS*/

#ifdef INCLUDE_DSL_CPE_PM_SHOWTIME_COUNTERS
/* DSL_FIO_PM_DATA_PATH_COUNTERS_SHOWTIME_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_DATA_PATH_COUNTERS_SHOWTIME_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_DataPathCountersShowtimeGet,
                   sizeof(DSL_PM_DataPathCounters_t)),
#endif /* INCLUDE_DSL_CPE_PM_SHOWTIME_COUNTERS*/

#ifdef INCLUDE_DSL_CPE_PM_HISTORY
#ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS
/* DSL_FIO_PM_DATA_PATH_HISTORY_STATS_15MIN_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_DATA_PATH_HISTORY_STATS_15MIN_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_DataPathHistoryStats15MinGet,
                   sizeof(DSL_PM_HistoryStatsChDir_t)),
/* DSL_FIO_PM_DATA_PATH_HISTORY_STATS_1DAY_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_DATA_PATH_HISTORY_STATS_1DAY_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_DataPathHistoryStats1DayGet,
                   sizeof(DSL_PM_HistoryStatsChDir_t)),
#endif /* INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS*/
#endif /* INCLUDE_DSL_CPE_PM_HISTORY*/

#ifdef INCLUDE_DSL_CPE_PM_DATA_PATH_THRESHOLDS
#ifdef INCLUDE_DSL_CPE_PM_HISTORY
#ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS
/* DSL_FIO_PM_DATA_PATH_THRESHOLDS_15MIN_SET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_DATA_PATH_THRESHOLDS_15MIN_SET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_PM_DataPathThresholds15MinSet,
                   sizeof(DSL_PM_DataPathThreshold_t)),
/* DSL_FIO_PM_DATA_PATH_THRESHOLDS_1DAY_SET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_DATA_PATH_THRESHOLDS_1DAY_SET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_PM_DataPathThresholds1DaySet,
                   sizeof(DSL_PM_DataPathThreshold_t)),
/* DSL_FIO_PM_DATA_PATH_THRESHOLDS_15MIN_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_DATA_PATH_THRESHOLDS_15MIN_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_DataPathThresholds15MinGet,
                   sizeof(DSL_PM_DataPathThreshold_t)),
/* DSL_FIO_PM_DATA_PATH_THRESHOLDS_1DAY_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_DATA_PATH_THRESHOLDS_1DAY_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_DataPathThresholds1DayGet,
                   sizeof(DSL_PM_DataPathThreshold_t)),
#endif /* INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS*/
#endif /* INCLUDE_DSL_CPE_PM_HISTORY*/
#endif /* INCLUDE_DSL_CPE_PM_DATA_PATH_THRESHOLDS*/
#endif /* INCLUDE_DSL_CPE_PM_DATA_PATH_COUNTERS*/

#ifdef INCLUDE_DSL_CPE_PM_RETX_COUNTERS
#ifdef INCLUDE_DSL_CPE_PM_HISTORY
/* DSL_FIO_PM_RETX_COUNTERS_15MIN_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_RETX_COUNTERS_15MIN_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_ReTxCounters15MinGet,
                   sizeof(DSL_PM_ReTxCounters_t)),
/* DSL_FIO_PM_RETX_COUNTERS_1DAY_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_RETX_COUNTERS_1DAY_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_ReTxCounters1DayGet,
                   sizeof(DSL_PM_ReTxCounters_t)),
#endif /* INCLUDE_DSL_CPE_PM_HISTORY*/
#ifdef INCLUDE_DSL_CPE_PM_TOTAL_COUNTERS
/* DSL_FIO_PM_RETX_COUNTERS_TOTAL_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_RETX_COUNTERS_TOTAL_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_ReTxCountersTotalGet,
                   sizeof(DSL_PM_ReTxCountersTotal_t)),
#endif /* INCLUDE_DSL_CPE_PM_TOTAL_COUNTERS*/

#ifdef INCLUDE_DSL_CPE_PM_SHOWTIME_COUNTERS
/* DSL_FIO_PM_RETX_COUNTERS_SHOWTIME_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_RETX_COUNTERS_SHOWTIME_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_ReTxCountersShowtimeGet,
                   sizeof(DSL_PM_ReTxCounters_t)),
#endif /* INCLUDE_DSL_CPE_PM_SHOWTIME_COUNTERS*/

#ifdef INCLUDE_DSL_CPE_PM_HISTORY
#ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS
/* DSL_FIO_PM_RETX_HISTORY_STATS_15MIN_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_RETX_HISTORY_STATS_15MIN_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_ReTxHistoryStats15MinGet,
                   sizeof(DSL_PM_HistoryStatsChDir_t)),
/* DSL_FIO_PM_RETX_HISTORY_STATS_1DAY_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_RETX_HISTORY_STATS_1DAY_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_ReTxHistoryStats1DayGet,
                   sizeof(DSL_PM_HistoryStatsChDir_t)),
#endif /* INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS*/
#endif /* INCLUDE_DSL_CPE_PM_HISTORY*/

#ifdef INCLUDE_DSL_CPE_PM_RETX_THRESHOLDS
#ifdef INCLUDE_DSL_CPE_PM_HISTORY
#ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS
/* DSL_FIO_PM_RETX_THRESHOLDS_15MIN_SET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_RETX_THRESHOLDS_15MIN_SET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_PM_ReTxThresholds15MinSet,
                   sizeof(DSL_PM_ReTxThreshold_t)),
/* DSL_FIO_PM_RETX_THRESHOLDS_1DAY_SET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_RETX_THRESHOLDS_1DAY_SET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_PM_ReTxThresholds1DaySet,
                   sizeof(DSL_PM_ReTxThreshold_t)),
/* DSL_FIO_PM_RETX_THRESHOLDS_15MIN_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_RETX_THRESHOLDS_15MIN_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_ReTxThresholds15MinGet,
                   sizeof(DSL_PM_ReTxThreshold_t)),
/* DSL_FIO_PM_RETX_THRESHOLDS_1DAY_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_RETX_THRESHOLDS_1DAY_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_ReTxThresholds1DayGet,
                   sizeof(DSL_PM_ReTxThreshold_t)),
#endif /* INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS*/
#endif /* INCLUDE_DSL_CPE_PM_HISTORY*/
#endif /* INCLUDE_DSL_CPE_PM_RETX_THRESHOLDS*/
#endif /* INCLUDE_DSL_CPE_PM_RETX_COUNTERS*/

#ifdef INCLUDE_DSL_CPE_PM_DATA_PATH_FAILURE_COUNTERS
#ifdef INCLUDE_DSL_CPE_PM_HISTORY
/* DSL_FIO_PM_DATA_PATH_FAILURE_COUNTERS_15MIN_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_DATA_PATH_FAILURE_COUNTERS_15MIN_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_DataPathFailureCounters15MinGet,
                   sizeof(DSL_PM_DataPathFailureCounters_t)),
/* DSL_FIO_PM_DATA_PATH_FAILURE_COUNTERS_1DAY_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_DATA_PATH_FAILURE_COUNTERS_1DAY_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_DataPathFailureCounters1DayGet,
                   sizeof(DSL_PM_DataPathFailureCounters_t)),
#endif /* INCLUDE_DSL_CPE_PM_HISTORY*/
#ifdef INCLUDE_DSL_CPE_PM_TOTAL_COUNTERS
/* DSL_FIO_PM_DATA_PATH_FAILURE_COUNTERS_TOTAL_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_DATA_PATH_FAILURE_COUNTERS_TOTAL_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_DataPathFailureCountersTotalGet,
                   sizeof(DSL_PM_DataPathFailureCountersTotal_t)),
#endif /* INCLUDE_DSL_CPE_PM_TOTAL_COUNTERS*/

#ifdef INCLUDE_DSL_CPE_PM_SHOWTIME_COUNTERS
/* DSL_FIO_PM_DATA_PATH_FAILURE_COUNTERS_SHOWTIME_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_DATA_PATH_FAILURE_COUNTERS_SHOWTIME_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_DataPathFailureCountersShowtimeGet,
                   sizeof(DSL_PM_DataPathFailureCounters_t)),
#endif /* INCLUDE_DSL_CPE_PM_SHOWTIME_COUNTERS*/

#ifdef INCLUDE_DSL_CPE_PM_HISTORY
#ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS
/* DSL_FIO_PM_DATA_PATH_FAILURE_HISTORY_STATS_15MIN_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_DATA_PATH_FAILURE_HISTORY_STATS_15MIN_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_DataPathFailureHistoryStats15MinGet,
                   sizeof(DSL_PM_HistoryStatsChDir_t)),
/* DSL_FIO_PM_DATA_PATH_FAILURE_HISTORY_STATS_1DAY_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_DATA_PATH_FAILURE_HISTORY_STATS_1DAY_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_DataPathFailureHistoryStats1DayGet,
                   sizeof(DSL_PM_HistoryStatsChDir_t)),
#endif /* INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS*/
#endif /* INCLUDE_DSL_CPE_PM_HISTORY*/
#endif /* INCLUDE_DSL_CPE_PM_DATA_PATH_FAILURE_COUNTERS*/

#ifdef INCLUDE_DSL_CPE_PM_LINE_COUNTERS
#ifdef INCLUDE_DSL_CPE_PM_HISTORY
/* DSL_FIO_PM_LINE_SEC_COUNTERS_15MIN_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_LINE_SEC_COUNTERS_15MIN_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_LineSecCounters15MinGet,
                   sizeof(DSL_PM_LineSecCounters_t)),
/* DSL_FIO_PM_LINE_SEC_COUNTERS_1DAY_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_LINE_SEC_COUNTERS_1DAY_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_LineSecCounters1DayGet,
                   sizeof(DSL_PM_LineSecCounters_t)),
#ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS
/* DSL_FIO_PM_LINE_INIT_HISTORY_STATS_15MIN_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_LINE_INIT_HISTORY_STATS_15MIN_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_LineInitHistoryStats15MinGet,
                   sizeof(DSL_PM_HistoryStats_t)),
/* DSL_FIO_PM_LINE_INIT_HISTORY_STATS_1DAY_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_LINE_INIT_HISTORY_STATS_1DAY_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_LineInitHistoryStats1DayGet,
                   sizeof(DSL_PM_HistoryStats_t)),
/* DSL_FIO_PM_LINE_SEC_HISTORY_STATS_15MIN_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_LINE_SEC_HISTORY_STATS_15MIN_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_LineSecHistoryStats15MinGet,
                   sizeof(DSL_PM_HistoryStatsDir_t)),
/* DSL_FIO_PM_LINE_SEC_HISTORY_STATS_1DAY_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_LINE_SEC_HISTORY_STATS_1DAY_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_LineSecHistoryStats1DayGet,
                   sizeof(DSL_PM_HistoryStatsDir_t)),
#endif /* INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS*/
#endif /* INCLUDE_DSL_CPE_PM_HISTORY*/

#ifdef INCLUDE_DSL_CPE_PM_TOTAL_COUNTERS
/* DSL_FIO_PM_LINE_SEC_COUNTERS_TOTAL_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_LINE_SEC_COUNTERS_TOTAL_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_LineSecCountersTotalGet,
                   sizeof(DSL_PM_LineSecCountersTotal_t)),
#endif /* INCLUDE_DSL_CPE_PM_TOTAL_COUNTERS*/

#ifdef INCLUDE_DSL_CPE_PM_SHOWTIME_COUNTERS
/* DSL_FIO_PM_LINE_SEC_COUNTERS_SHOWTIME_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_LINE_SEC_COUNTERS_SHOWTIME_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_LineSecCountersShowtimeGet,
                   sizeof(DSL_PM_LineSecCounters_t)),
#endif /* INCLUDE_DSL_CPE_PM_SHOWTIME_COUNTERS*/

#ifdef INCLUDE_DSL_CPE_PM_LINE_THRESHOLDS
#ifdef INCLUDE_DSL_CPE_PM_HISTORY
#ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS
/* DSL_FIO_PM_LINE_SEC_THRESHOLDS_15MIN_SET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_LINE_SEC_THRESHOLDS_15MIN_SET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_PM_LineSecThresholds15MinSet,
                   sizeof(DSL_PM_LineSecThreshold_t)),
/* DSL_FIO_PM_LINE_SEC_THRESHOLDS_1DAY_SET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_LINE_SEC_THRESHOLDS_1DAY_SET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_PM_LineSecThresholds1DaySet,
                   sizeof(DSL_PM_LineSecThreshold_t)),
/* DSL_FIO_PM_LINE_SEC_THRESHOLDS_15MIN_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_LINE_SEC_THRESHOLDS_15MIN_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_LineSecThresholds15MinGet,
                   sizeof(DSL_PM_LineSecThreshold_t)),
/* DSL_FIO_PM_LINE_SEC_THRESHOLDS_1DAY_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_LINE_SEC_THRESHOLDS_1DAY_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_LineSecThresholds1DayGet,
                   sizeof(DSL_PM_LineSecThreshold_t)),
#endif /* INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS*/
#endif /* INCLUDE_DSL_CPE_PM_HISTORY*/
#endif /* INCLUDE_DSL_CPE_PM_LINE_THRESHOLDS*/

#ifdef INCLUDE_DSL_CPE_PM_HISTORY
/* DSL_FIO_PM_LINE_INIT_COUNTERS_15MIN_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_LINE_INIT_COUNTERS_15MIN_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_LineInitCounters15MinGet,
                   sizeof(DSL_PM_LineInitCounters_t)),
/* DSL_FIO_PM_LINE_INIT_COUNTERS_1DAY_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_LINE_INIT_COUNTERS_1DAY_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_LineInitCounters1DayGet,
                   sizeof(DSL_PM_LineInitCounters_t)),
#endif /* INCLUDE_DSL_CPE_PM_HISTORY*/

#ifdef INCLUDE_DSL_CPE_PM_TOTAL_COUNTERS
/* DSL_FIO_PM_LINE_INIT_COUNTERS_TOTAL_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_LINE_INIT_COUNTERS_TOTAL_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_LineInitCountersTotalGet,
                   sizeof(DSL_PM_LineInitCountersTotal_t)),
#endif /* INCLUDE_DSL_CPE_PM_TOTAL_COUNTERS*/

#ifdef INCLUDE_DSL_CPE_PM_SHOWTIME_COUNTERS
/* DSL_FIO_PM_LINE_INIT_COUNTERS_SHOWTIME_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_LINE_INIT_COUNTERS_SHOWTIME_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_LineInitCountersShowtimeGet,
                   sizeof(DSL_PM_LineInitCounters_t)),
#endif /* INCLUDE_DSL_CPE_PM_SHOWTIME_COUNTERS*/

#ifdef INCLUDE_DSL_CPE_PM_LINE_THRESHOLDS
#ifdef INCLUDE_DSL_CPE_PM_HISTORY
#ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS
/* DSL_FIO_PM_LINE_INIT_THRESHOLDS_15MIN_SET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_LINE_INIT_THRESHOLDS_15MIN_SET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_PM_LineInitThresholds15MinSet,
                   sizeof(DSL_PM_LineInitThreshold_t)),
/* DSL_FIO_PM_LINE_INIT_THRESHOLDS_1DAY_SET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_LINE_INIT_THRESHOLDS_1DAY_SET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_PM_LineInitThresholds1DaySet,
                   sizeof(DSL_PM_LineInitThreshold_t)),
/* DSL_FIO_PM_LINE_INIT_THRESHOLDS_15MIN_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_LINE_INIT_THRESHOLDS_15MIN_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_LineInitThresholds15MinGet,
                   sizeof(DSL_PM_LineInitThreshold_t)),
/* DSL_FIO_PM_LINE_INIT_THRESHOLDS_1DAY_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_LINE_INIT_THRESHOLDS_1DAY_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_LineInitThresholds1DayGet,
                   sizeof(DSL_PM_LineInitThreshold_t)),
#endif /* INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS*/
#endif /* INCLUDE_DSL_CPE_PM_HISTORY*/
#endif /* INCLUDE_DSL_CPE_PM_LINE_THRESHOLDS*/
#endif /* INCLUDE_DSL_CPE_PM_LINE_COUNTERS*/

#ifdef INCLUDE_DSL_CPE_PM_LINE_EVENT_SHOWTIME_COUNTERS
#ifdef INCLUDE_DSL_CPE_PM_HISTORY
/* DSL_FIO_PM_LINE_EVENT_SHOWTIME_COUNTERS_15MIN_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_LINE_EVENT_SHOWTIME_COUNTERS_15MIN_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_LineEventShowtimeCounters15MinGet,
                   sizeof(DSL_PM_LineEventShowtimeCounters_t)),
/* DSL_FIO_PM_LINE_EVENT_SHOWTIME_COUNTERS_1DAY_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_LINE_EVENT_SHOWTIME_COUNTERS_1DAY_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_LineEventShowtimeCounters1DayGet,
                   sizeof(DSL_PM_LineEventShowtimeCounters_t)),
#endif /* INCLUDE_DSL_CPE_PM_HISTORY*/
#ifdef INCLUDE_DSL_CPE_PM_TOTAL_COUNTERS
/* DSL_FIO_PM_LINE_EVENT_SHOWTIME_COUNTERS_TOTAL_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_LINE_EVENT_SHOWTIME_COUNTERS_TOTAL_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_LineEventShowtimeCountersTotalGet,
                   sizeof(DSL_PM_LineEventShowtimeCountersTotal_t)),
#endif /* INCLUDE_DSL_CPE_PM_TOTAL_COUNTERS*/

#ifdef INCLUDE_DSL_CPE_PM_SHOWTIME_COUNTERS
/* DSL_FIO_PM_LINE_EVENT_SHOWTIME_COUNTERS_SHOWTIME_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_LINE_EVENT_SHOWTIME_COUNTERS_SHOWTIME_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_LineEventShowtimeCountersShowtimeGet,
                   sizeof(DSL_PM_LineEventShowtimeCounters_t)),
#endif /* INCLUDE_DSL_CPE_PM_SHOWTIME_COUNTERS*/

#ifdef INCLUDE_DSL_CPE_PM_HISTORY
#ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS
/* DSL_FIO_PM_LINE_EVENT_SHOWTIME_HISTORY_STATS_15MIN_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_LINE_EVENT_SHOWTIME_HISTORY_STATS_15MIN_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_LineEventShowtimeHistoryStats15MinGet,
                   sizeof(DSL_PM_HistoryStatsChDir_t)),
/* DSL_FIO_PM_LINE_EVENT_SHOWTIME_HISTORY_STATS_1DAY_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PM_LINE_EVENT_SHOWTIME_HISTORY_STATS_1DAY_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PM_LineEventShowtimeHistoryStats1DayGet,
                   sizeof(DSL_PM_HistoryStatsChDir_t)),
#endif /* INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS*/
#endif /* INCLUDE_DSL_CPE_PM_HISTORY*/
#endif /* INCLUDE_DSL_CPE_PM_LINE_EVENT_SHOWTIME_COUNTERS*/
#endif /* INCLUDE_DSL_PM*/

#ifdef INCLUDE_DSL_CEOC
/* DSL_FIO_G997_SNMP_MESSAGE_SEND */
DSL_IOCTL_REGISTER(DSL_FIO_G997_SNMP_MESSAGE_SEND, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_G997_SnmpMessageSend,
                   sizeof(DSL_G997_Snmp_t)),
/* DSL_FIO_G997_SNMP_MESSAGE_RECEIVE */
DSL_IOCTL_REGISTER(DSL_FIO_G997_SNMP_MESSAGE_RECEIVE, DSL_IOCTL_HELPER_GET,
                   DSL_TRUE, DSL_DRV_G997_SnmpMessageReceive,
                   sizeof(DSL_G997_Snmp_t)),
#endif /* INCLUDE_DSL_CEOC*/

/* DSL_FIO_G997_RATE_ADAPTATION_CONFIG_SET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_RATE_ADAPTATION_CONFIG_SET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_G997_RateAdaptationConfigSet,
                   sizeof(DSL_G997_RateAdaptationConfig_t)),
/* DSL_FIO_G997_LINE_ACTIVATE_CONFIG_SET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_LINE_ACTIVATE_CONFIG_SET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_G997_LineActivateConfigSet,
                   sizeof(DSL_G997_LineActivate_t)),
/* DSL_FIO_G997_POWER_MANAGEMENT_STATE_FORCED_TRIGGER */
DSL_IOCTL_REGISTER(DSL_FIO_G997_POWER_MANAGEMENT_STATE_FORCED_TRIGGER, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_G997_PowerManagementStateForcedTrigger,
                   sizeof(DSL_G997_PowerManagementStateForcedTrigger_t)),
/* DSL_FIO_G997_POWER_MANAGEMENT_STATUS_GET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_POWER_MANAGEMENT_STATUS_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_G997_PowerManagementStatusGet,
                   sizeof(DSL_G997_PowerManagementStatus_t)),
/* DSL_FIO_G997_LINE_STATUS_GET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_LINE_STATUS_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_G997_LineStatusGet,
                   sizeof(DSL_G997_LineStatus_t)),
/* DSL_FIO_G997_CHANNEL_STATUS_GET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_CHANNEL_STATUS_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_G997_ChannelStatusGet,
                   sizeof(DSL_G997_ChannelStatus_t)),

#ifdef INCLUDE_DSL_G997_PER_TONE
/* DSL_FIO_G997_BIT_ALLOCATION_NSC_GET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_BIT_ALLOCATION_NSC_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_G997_BitAllocationNSCGet,
                   sizeof(DSL_G997_BitAllocationNsc_t)),
/* DSL_FIO_G997_SNR_ALLOCATION_NSC_GET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_SNR_ALLOCATION_NSC_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_G997_SnrAllocationNscGet,
                   sizeof(DSL_G997_SnrAllocationNsc_t)),
/* DSL_FIO_G997_GAIN_ALLOCATION_NSC_GET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_GAIN_ALLOCATION_NSC_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_G997_GainAllocationNscGet,
                   sizeof(DSL_G997_GainAllocationNsc_t)),
#endif /* INCLUDE_DSL_G997_PER_TONE*/

#ifdef INCLUDE_DSL_DELT
/* DSL_FIO_G997_DELT_HLIN_GET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_DELT_HLIN_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_G997_DeltHlinGet,
                   sizeof(DSL_G997_DeltHlin_t)),
/* DSL_FIO_G997_DELT_HLIN_SCALE_GET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_DELT_HLIN_SCALE_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_G997_DeltHlinScaleGet,
                   sizeof(DSL_G997_DeltHlinScale_t)),
/* DSL_FIO_G997_DELT_HLOG_GET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_DELT_HLOG_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_G997_DeltHlogGet,
                   sizeof(DSL_G997_DeltHlog_t)),
/* DSL_FIO_G997_DELT_QLN_GET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_DELT_QLN_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_G997_DeltQLNGet,
                   sizeof(DSL_G997_DeltQln_t)),
/* DSL_FIO_G997_DELT_SNR_GET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_DELT_SNR_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_G997_DeltSNRGet,
                   sizeof(DSL_G997_DeltSnr_t)),
/* DSL_FIO_DELT_SNR_GET */
DSL_IOCTL_REGISTER(DSL_FIO_DELT_SNR_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_DeltSNRGet,
                   sizeof(DSL_G997_DeltSnr_t)),
/* DSL_FIO_G997_DELT_FREE_RESOURCES */
DSL_IOCTL_REGISTER(DSL_FIO_G997_DELT_FREE_RESOURCES, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_G997_DeltFreeResources,
                   sizeof(DSL_G997_DeltFreeResources_t)),
#endif /* INCLUDE_DSL_DELT*/

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/* DSL_FIO_G997_LINE_STATUS_PER_BAND_GET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_LINE_STATUS_PER_BAND_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_G997_LineStatusPerBandGet,
                  sizeof(DSL_G997_LineStatusPerBand_t)),
/* DSL_FIO_G997_US_POWER_BACK_OFF_STATUS_GET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_US_POWER_BACK_OFF_STATUS_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_G997_UsPowerBackOffStatusGet,
                  sizeof(DSL_G997_UsPowerBackOffStatus_t)),
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */

#ifdef INCLUDE_DSL_G997_FRAMING_PARAMETERS
/* DSL_FIO_G997_FRAMING_PARAMETER_STATUS_GET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_FRAMING_PARAMETER_STATUS_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_G997_FramingParameterStatusGet,
                   sizeof(DSL_G997_FramingParameterStatus_t)),
#endif /* INCLUDE_DSL_G997_FRAMING_PARAMETERS*/

#ifdef INCLUDE_DSL_G997_ALARM
/* DSL_FIO_G997_ALARM_MASK_DATA_PATH_FAILURES_CONFIG_SET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_ALARM_MASK_DATA_PATH_FAILURES_CONFIG_SET, DSL_IOCTL_HELPER_SET,
                   DSL_TRUE, DSL_DRV_G997_AlarmMaskDataPathFailuresConfigSet,
                   sizeof(DSL_G997_DataPathFailures_t)),
/* DSL_FIO_G997_ALARM_MASK_LINE_FAILURES_CONFIG_SET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_ALARM_MASK_LINE_FAILURES_CONFIG_SET, DSL_IOCTL_HELPER_SET,
                   DSL_TRUE, DSL_DRV_G997_AlarmMaskLineFailuresConfigSet,
                   sizeof(DSL_G997_LineFailures_t)),
/* DSL_FIO_G997_CHANNEL_DATA_RATE_THRESHOLD_CONFIG_SET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_CHANNEL_DATA_RATE_THRESHOLD_CONFIG_SET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_G997_ChannelDataRateThresholdConfigSet,
                   sizeof(DSL_G997_ChannelDataRateThreshold_t)),

#ifdef INCLUDE_DSL_CONFIG_GET
/* DSL_FIO_G997_ALARM_MASK_DATA_PATH_FAILURES_CONFIG_GET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_ALARM_MASK_DATA_PATH_FAILURES_CONFIG_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_G997_AlarmMaskDataPathFailuresConfigGet,
                   sizeof(DSL_G997_DataPathFailures_t)),
/* DSL_FIO_G997_ALARM_MASK_LINE_FAILURES_CONFIG_GET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_ALARM_MASK_LINE_FAILURES_CONFIG_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_G997_AlarmMaskLineFailuresConfigGet,
                   sizeof(DSL_G997_LineFailures_t)),
/* DSL_FIO_G997_CHANNEL_DATA_RATE_THRESHOLD_CONFIG_GET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_CHANNEL_DATA_RATE_THRESHOLD_CONFIG_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_G997_ChannelDataRateThresholdConfigGet,
                   sizeof(DSL_G997_ChannelDataRateThreshold_t)),
#endif /* INCLUDE_DSL_CONFIG_GET*/
#endif /* INCLUDE_DSL_G997_ALARM*/

#ifdef INCLUDE_DSL_G997_STATUS
/* DSL_FIO_G997_LINE_INIT_STATUS_GET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_LINE_INIT_STATUS_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_G997_LineInitStatusGet,
                   sizeof(DSL_G997_LineInitStatus_t)),
/* DSL_FIO_G997_LINE_FAILURES_STATUS_GET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_LINE_FAILURES_STATUS_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_G997_LineFailuresStatusGet,
                   sizeof(DSL_G997_LineFailures_t)),
/* DSL_FIO_G997_LINE_TRANSMISSION_STATUS_GET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_LINE_TRANSMISSION_STATUS_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_G997_LineTransmissionStatusGet,
                   sizeof(DSL_G997_LineTransmissionStatus_t)),
/* DSL_FIO_G997_LAST_STATE_TRANSMITTED_GET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_LAST_STATE_TRANSMITTED_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_G997_LastStateTransmittedGet,
                   sizeof(DSL_G997_LastStateTransmitted_t)),
/* DSL_FIO_G997_DATA_PATH_FAILURES_STATUS_GET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_DATA_PATH_FAILURES_STATUS_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_G997_DataPathFailuresStatusGet,
                   sizeof(DSL_G997_DataPathFailures_t)),
#endif /* INCLUDE_DSL_G997_STATUS*/

#ifdef INCLUDE_DSL_G997_LINE_INVENTORY
/* DSL_FIO_G997_LINE_INVENTORY_GET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_LINE_INVENTORY_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_G997_LineInventoryGet,
                   sizeof(DSL_G997_LineInventory_t)),
/* DSL_FIO_G997_LINE_INVENTORY_SET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_LINE_INVENTORY_SET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_G997_LineInventorySet,
                   sizeof(DSL_G997_LineInventoryNe_t)),
#endif /* INCLUDE_DSL_G997_LINE_INVENTORY*/

#ifdef INCLUDE_DSL_CONFIG_GET
/* DSL_FIO_G997_LINE_ACTIVATE_CONFIG_GET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_LINE_ACTIVATE_CONFIG_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_G997_LineActivateConfigGet,
                   sizeof(DSL_G997_LineActivate_t)),
/* DSL_FIO_G997_RATE_ADAPTATION_CONFIG_GET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_RATE_ADAPTATION_CONFIG_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_G997_RateAdaptationConfigGet,
                   sizeof(DSL_G997_RateAdaptationConfig_t)),
#endif /* INCLUDE_DSL_CONFIG_GET*/

#ifdef INCLUDE_DSL_CPE_API_VRX
/* DSL_FIO_G997_LOW_POWER_MODE_CONFIG_SET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_LOW_POWER_MODE_CONFIG_SET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_G997_LowPowerModeConfigSet,
                   sizeof(DSL_G997_LowPowerModeConfig_t)),
#ifdef INCLUDE_DSL_CONFIG_GET
/* DSL_FIO_G997_LOW_POWER_MODE_CONFIG_GET */
DSL_IOCTL_REGISTER(DSL_FIO_G997_LOW_POWER_MODE_CONFIG_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_G997_LowPowerModeConfigGet,
                   sizeof(DSL_G997_LowPowerModeConfig_t)),
#endif /* INCLUDE_DSL_CONFIG_GET*/
#endif /* INCLUDE_DSL_CPE_API_VRX*/

#ifdef INCLUDE_DSL_BONDING
/* DSL_FIO_BND_HW_INIT */
DSL_IOCTL_REGISTER(DSL_FIO_BND_HW_INIT, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_BND_HwInit,
                   sizeof(DSL_BND_HwInit_t)),
/* DSL_FIO_BND_CONFIG_SET */
DSL_IOCTL_REGISTER(DSL_FIO_BND_CONFIG_SET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_BND_ConfigSet,
                   sizeof(DSL_BND_ConfigSet_t)),
#ifdef INCLUDE_DSL_CONFIG_GET
/* DSL_FIO_BND_CONFIG_GET */
DSL_IOCTL_REGISTER(DSL_FIO_BND_CONFIG_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_BND_ConfigGet,
                   sizeof(DSL_BND_ConfigGet_t)),
#endif /* INCLUDE_DSL_CONFIG_GET*/
/* DSL_FIO_BND_HS_STATUS_GET */
DSL_IOCTL_REGISTER(DSL_FIO_BND_HS_STATUS_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_BND_HsStatusGet,
                   sizeof(DSL_BND_HsStatusGet_t)),
/* DSL_FIO_BND_HS_CONTINUE */
DSL_IOCTL_REGISTER(DSL_FIO_BND_HS_CONTINUE, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_BND_HsContinue,
                   sizeof(DSL_BND_HsContinue_t)),
/* DSL_FIO_BND_ETH_DBG_COUNTERS_GET */
DSL_IOCTL_REGISTER(DSL_FIO_BND_ETH_DBG_COUNTERS_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_BND_EthDbgCountersGet,
                   sizeof(DSL_BND_EthDbgCounters_t)),
/* DSL_FIO_BND_ETH_COUNTERS_GET */
DSL_IOCTL_REGISTER(DSL_FIO_BND_ETH_COUNTERS_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_BND_EthCountersGet,
                   sizeof(DSL_BND_EthCounters_t)),
#if (DSL_DRV_LINES_PER_DEVICE == 2)
/* DSL_FIO_BND_PORT_MODE_SYNC_SET */
DSL_IOCTL_REGISTER(DSL_FIO_BND_PORT_MODE_SYNC_SET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_BND_PortModeSyncSet,
                   sizeof(DSL_BND_PortModeSync_t)),
#endif /* DSL_DRV_LINES_PER_DEVICE == 2*/
#endif /* INCLUDE_DSL_BONDING*/

/* DSL_FIO_VERSION_INFORMATION_GET */
DSL_IOCTL_REGISTER(DSL_FIO_VERSION_INFORMATION_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_VersionInformationGet,
                   sizeof(DSL_VersionInformation_t)),
#ifdef INCLUDE_DSL_CPE_API_DANUBE
/* DSL_FIO_INTEROP_FEATURE_CONFIG_SET */
DSL_IOCTL_REGISTER(DSL_FIO_INTEROP_FEATURE_CONFIG_SET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_InteropFeatureConfigSet,
                   sizeof(DSL_InteropFeatureConfig_t)),
#endif /* INCLUDE_DSL_CPE_API_DANUBE*/
/* DSL_FIO_INSTANCE_STATUS_GET */
DSL_IOCTL_REGISTER(DSL_FIO_INSTANCE_STATUS_GET, DSL_IOCTL_HELPER_GET,
                   DSL_TRUE, DSL_DRV_InstanceStatusGet,
                   sizeof(DSL_InstanceStatus_t)),

/* DSL_FIO_LOW_LEVEL_CONFIGURATION_SET */
DSL_IOCTL_REGISTER(DSL_FIO_LOW_LEVEL_CONFIGURATION_SET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_LowLevelConfigurationSet,
                   sizeof(DSL_LowLevelConfiguration_t)),
/* DSL_FIO_LOW_LEVEL_CONFIGURATION_GET */
DSL_IOCTL_REGISTER(DSL_FIO_LOW_LEVEL_CONFIGURATION_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_LowLevelConfigurationGet,
                   sizeof(DSL_LowLevelConfiguration_t)),

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
#ifdef INCLUDE_DSL_G997_LINE_INVENTORY
/* DSL_FIO_AUX_LINE_INVENTORY_GET */
DSL_IOCTL_REGISTER(DSL_FIO_AUX_LINE_INVENTORY_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_AuxLineInventoryGet,
                   sizeof(DSL_AuxLineInventory_t)),
#endif /* INCLUDE_DSL_G997_LINE_INVENTORY*/
/* DSL_FIO_BAND_PLAN_SUPPORT_GET */
DSL_IOCTL_REGISTER(DSL_FIO_BAND_PLAN_SUPPORT_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_BandPlanSupportedGet,
                   sizeof(DSL_BandPlanSupport_t)),
/* DSL_FIO_BAND_PLAN_STATUS_GET */
DSL_IOCTL_REGISTER(DSL_FIO_BAND_PLAN_STATUS_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_BandPlanStatusGet,
                   sizeof(DSL_BandPlanStatus_t)),
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)*/

#ifdef INCLUDE_DSL_RESOURCE_STATISTICS
/* DSL_FIO_RESOURCE_USAGE_STATISTICS_GET */
DSL_IOCTL_REGISTER(DSL_FIO_RESOURCE_USAGE_STATISTICS_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_ResourceUsageStatisticsGet,
                   sizeof(DSL_ResourceUsageStatistics_t)),
#endif /* INCLUDE_DSL_RESOURCE_STATISTICS*/

#ifdef INCLUDE_DSL_FRAMING_PARAMETERS
/* DSL_FIO_FRAMING_PARAMETER_STATUS_GET */
DSL_IOCTL_REGISTER(DSL_FIO_FRAMING_PARAMETER_STATUS_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_FramingParameterStatusGet,
                   sizeof(DSL_FramingParameterStatus_t)),
#endif /* INCLUDE_DSL_FRAMING_PARAMETERS*/

#ifdef INCLUDE_PILOT_TONES_STATUS
/* DSL_FIO_PILOT_TONES_STATUS_GET */
DSL_IOCTL_REGISTER(DSL_FIO_PILOT_TONES_STATUS_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_PilotTonesStatusGet,
                   sizeof(DSL_PilotTonesStatus_t)),
#endif /* INCLUDE_PILOT_TONES_STATUS*/

/* DSL_FIO_REBOOT_CRITERIA_CONFIG_SET */
DSL_IOCTL_REGISTER(DSL_FIO_REBOOT_CRITERIA_CONFIG_SET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_RebootCriteriaConfigSet,
                   sizeof(DSL_RebootCriteriaConfig_t)),
/* DSL_FIO_REBOOT_CRITERIA_CONFIG_GET */
DSL_IOCTL_REGISTER(DSL_FIO_REBOOT_CRITERIA_CONFIG_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_RebootCriteriaConfigGet,
                   sizeof(DSL_RebootCriteriaConfig_t)),
/* DSL_FIO_TEST_MODE_CONTROL_SET */
DSL_IOCTL_REGISTER(DSL_FIO_TEST_MODE_CONTROL_SET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_TestModeControlSet,
                   sizeof(DSL_TestModeControl_t)),
/* DSL_FIO_TEST_MODE_STATUS_GET */
DSL_IOCTL_REGISTER(DSL_FIO_TEST_MODE_STATUS_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_TestModeStatusGet,
                   sizeof(DSL_TestModeStatus_t)),
/* DSL_FIO_LINE_FEATURE_STATUS_GET */
DSL_IOCTL_REGISTER(DSL_FIO_LINE_FEATURE_STATUS_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_LineFeatureStatusGet,
                   sizeof(DSL_LineFeature_t)),
#ifdef INCLUDE_DSL_CPE_PM_RETX_COUNTERS
#ifdef INCLUDE_DSL_CPE_PM_RETX_THRESHOLDS
/* DSL_FIO_LINE_RETX_STATISTICS_GET */
DSL_IOCTL_REGISTER(DSL_FIO_RETX_STATISTICS_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_RetxStatisticsGet,
                   sizeof(DSL_ReTxStatistics_t)),
#endif /* INCLUDE_DSL_CPE_PM_RETX_THRESHOLDS */
#endif /* INCLUDE_DSL_CPE_PM_RETX_COUNTERS */
#ifndef DSL_DEBUG_DISABLE
/* DSL_FIO_DBG_MODULE_LEVEL_SET */
DSL_IOCTL_REGISTER(DSL_FIO_DBG_MODULE_LEVEL_SET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_DBG_ModuleLevelSet,
                   sizeof(DSL_DBG_ModuleLevel_t)),
/* DSL_FIO_DBG_MODULE_LEVEL_GET */
DSL_IOCTL_REGISTER(DSL_FIO_DBG_MODULE_LEVEL_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_DBG_ModuleLevelGet,
                   sizeof(DSL_DBG_ModuleLevel_t)),
#endif /* DSL_DEBUG_DISABLE*/
#ifdef INCLUDE_REAL_TIME_TRACE
/* DSL_FIO_RTT_INIT */
DSL_IOCTL_REGISTER(DSL_FIO_RTT_INIT, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_RTT_Init,
                   sizeof(DSL_RTT_Init_t)),
/* DSL_FIO_RTT_CONFIG_SET */
DSL_IOCTL_REGISTER(DSL_FIO_RTT_CONFIG_SET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_RTT_ConfigSet,
                   sizeof(DSL_RTT_Config_t)),
/* DSL_FIO_RTT_CONFIG_GET */
DSL_IOCTL_REGISTER(DSL_FIO_RTT_CONFIG_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_RTT_ConfigGet,
                   sizeof(DSL_RTT_Config_t)),
/* DSL_FIO_RTT_STATUS_GET */
DSL_IOCTL_REGISTER(DSL_FIO_RTT_STATUS_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_RTT_StatusGet,
                   sizeof(DSL_RTT_Status_t)),
/* DSL_FIO_RTT_CONTROL_SET */
DSL_IOCTL_REGISTER(DSL_FIO_RTT_CONTROL_SET, DSL_IOCTL_HELPER_SET,
                   DSL_FALSE, DSL_DRV_RTT_ControlSet,
                   sizeof(DSL_RTT_Control_t)),
/* DSL_FIO_RTT_STATISTICS_GET */
DSL_IOCTL_REGISTER(DSL_FIO_RTT_STATISTICS_GET, DSL_IOCTL_HELPER_GET,
                   DSL_FALSE, DSL_DRV_RTT_StatisticsGet,
                   sizeof(DSL_RTT_Statistics_t)),
#endif /*INCLUDE_REAL_TIME_TRACE*/

/* IOCTL: Dummy delimeter */
DSL_IOCTL_REGISTER(0, DSL_IOCTL_HELPER_UNKNOWN, DSL_FALSE, DSL_NULL, 0)
};

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_IoctlHandle(
   DSL_OpenContext_t *pOpenCtx,
   DSL_Context_t *pContext,
   DSL_boolean_t bIsInKernel,
   DSL_uint_t nCommand,
   DSL_uint32_t nArg)
{
   DSL_Error_t nErrCode = DSL_ERROR;
   DSL_IOCTL_arg_t *pIOCTL_arg = DSL_NULL;
   DSL_EventData_Union_t *pEventData;
   DSL_uint8_t *pMsgBuf, *pOldMsgBuf, *pMaskBuf, *pDataBuf;
   DSL_uint16_t *pMsgId;
   DSL_uint16_t i;
   DSL_boolean_t bEventActivation = DSL_FALSE;
   DSL_BF_ResourceActivationType_t nResourceActivationMask;
   DSL_EventStatusData_t event;
   DSL_IOCTL_Table_t *pIoctlTable = ioctlTable;
   DSL_boolean_t bIoctlFound = DSL_FALSE;

#ifndef DSL_DEBUG_DISABLE
   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_IoctlHandle, call %d - "
      "(%s)"DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
      nCommand, DSL_DBG_IoctlName(nCommand)));
#endif /* DSL_DEBUG_DISABLE*/

   switch (nCommand)
   {
      case DSL_FIO_INIT:
         pIOCTL_arg = DSL_DRV_VMalloc(sizeof(DSL_Init_t));

         if(pIOCTL_arg == DSL_NULL)
         {
            DSL_IoctlMemCpyTo( bIsInKernel, (DSL_void_t*)nArg, &nErrCode,
               sizeof(DSL_Error_t));
            break;
         }

         if (nArg == 0)
         {
            nErrCode = DSL_ERR_INVALID_PARAMETER;
         }
         else
         {
            DSL_IoctlMemCpyFrom(bIsInKernel, pIOCTL_arg, (DSL_void_t*)nArg,
                                 sizeof(DSL_Init_t));

            nErrCode = DSL_DRV_InitDataPrepare(pContext, bIsInKernel, &(pIOCTL_arg->init));
            if (nErrCode == DSL_SUCCESS)
            {
               nErrCode = DSL_DRV_Init(pContext, &(pIOCTL_arg->init));
            }

            DSL_DRV_InitDataFree(pContext, &(pIOCTL_arg->init));

            DSL_DEBUG(DSL_DBG_MSG,
               (pContext, SYS_DBG_MSG"DSL[%02d]: DSL_DRV_IoctlHandle - return"
               "(from DSL_FIO_INIT) %d"DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));
            DSL_IoctlMemCpyTo( bIsInKernel, (DSL_void_t*)nArg, &nErrCode,
               sizeof(DSL_Error_t));
         }
         break;

#ifdef INCLUDE_DSL_CPE_TRACE_BUFFER
      case DSL_FIO_SHOWTIME_LOGGING_DATA_GET:
         pIOCTL_arg = DSL_DRV_VMalloc(sizeof(DSL_ShowtimeLogging_t));

         if(pIOCTL_arg == DSL_NULL)
         {
            DSL_IoctlMemCpyTo( bIsInKernel, (DSL_void_t*)nArg, &nErrCode,
               sizeof(DSL_Error_t));
            break;
         }

         if (nArg == 0)
         {
            nErrCode = DSL_ERR_INVALID_PARAMETER;
         }
         else
         {
            DSL_IoctlMemCpyFrom(bIsInKernel, pIOCTL_arg, (DSL_void_t*)nArg,
                                 sizeof(DSL_ShowtimeLogging_t));

            DSL_DEBUG(DSL_DBG_MSG, (pContext,
               SYS_DBG_MSG"DSL[%02d]: DEBUG:2:: showtime event logging data (in the kernel space):",
               DSL_DEV_NUM(pContext)));
            for (i = 0; i < DSL_DEV_SHOWTIME_EVENT_LOGGING_BUFFER_LENGTH; i++)
            {
               if (i % 10 == 0) DSL_DEBUG(DSL_DBG_MSG, (pContext, SYS_DBG_MSG""DSL_DRV_CRLF));
               DSL_DEBUG(DSL_DBG_MSG,
                  (pContext, SYS_DBG_MSG"0x%04X ", pContext->loggingBuffer[i]));
            }
            DSL_DEBUG(DSL_DBG_MSG, (pContext, SYS_DBG_MSG""DSL_DRV_CRLF DSL_DRV_CRLF));

            if (pIOCTL_arg->showtimeLogging.data.pData == DSL_NULL)
            {
               /* in this case return data size only */
               pIOCTL_arg->showtimeLogging.data.nDataSize =
                  DSL_DEV_SHOWTIME_EVENT_LOGGING_BUFFER_LENGTH * 2;
               nErrCode = DSL_SUCCESS;
            }
            else
            {
               if ((pIOCTL_arg->showtimeLogging.data.nDataOffset >=
                  DSL_DEV_SHOWTIME_EVENT_LOGGING_BUFFER_LENGTH * 2) ||
                  pIOCTL_arg->showtimeLogging.data.nDataSize == 0)
               {
                  nErrCode = DSL_ERR_INVALID_PARAMETER;
               }
               else
               {
                  if (pIOCTL_arg->showtimeLogging.data.nDataSize >=
                      (DSL_DEV_SHOWTIME_EVENT_LOGGING_BUFFER_LENGTH * 2 -
                       pIOCTL_arg->showtimeLogging.data.nDataOffset))
                  {
                     DSL_IoctlMemCpyTo(bIsInKernel,
                        pIOCTL_arg->showtimeLogging.data.pData,
                        pContext->loggingBuffer,
                        DSL_DEV_SHOWTIME_EVENT_LOGGING_BUFFER_LENGTH * 2 -
                           pIOCTL_arg->showtimeLogging.data.nDataOffset);
                     pIOCTL_arg->showtimeLogging.data.nDataOffset = 0;
                     pIOCTL_arg->showtimeLogging.data.nDataSize =
                        DSL_DEV_SHOWTIME_EVENT_LOGGING_BUFFER_LENGTH * 2 -
                        pIOCTL_arg->showtimeLogging.data.nDataOffset;
                  }
                  else
                  {
                     DSL_IoctlMemCpyTo(bIsInKernel,
                        pIOCTL_arg->showtimeLogging.data.pData,
                        pContext->loggingBuffer,
                        pIOCTL_arg->showtimeLogging.data.nDataSize);
                     pIOCTL_arg->showtimeLogging.data.nDataOffset +=
                        pIOCTL_arg->showtimeLogging.data.nDataSize;
                  }
                  nErrCode = DSL_SUCCESS;
               }
            }

            if (pIOCTL_arg->showtimeLogging.data.pData)
            {
               DSL_DEBUG(DSL_DBG_MSG, (pContext,
                  SYS_DBG_MSG"DSL[%02d]: DEBUG:3:: showtime event logging data (in the user space):",
                  DSL_DEV_NUM(pContext)));
               for (i = 0; i < pIOCTL_arg->showtimeLogging.data.nDataSize / 2; i++)
               {
                  if (i % 10 == 0) DSL_DEBUG(DSL_DBG_MSG, (pContext, SYS_DBG_MSG""DSL_DRV_CRLF));
                  DSL_DEBUG(DSL_DBG_MSG, (pContext, SYS_DBG_MSG"0x%04X ",
                     ((DSL_uint16_t*)(pIOCTL_arg->showtimeLogging.data.pData))[i]));
               }
               DSL_DEBUG(DSL_DBG_MSG, (pContext, SYS_DBG_MSG""DSL_DRV_CRLF DSL_DRV_CRLF));
            }

            pIOCTL_arg->showtimeLogging.accessCtl.nReturn = nErrCode;

            if (nErrCode == DSL_SUCCESS)
            {
               DSL_IoctlMemCpyTo( bIsInKernel, (DSL_void_t*)nArg, pIOCTL_arg,
                  sizeof(DSL_ShowtimeLogging_t));
            }
            else
            {
               DSL_IoctlMemCpyTo( bIsInKernel, (DSL_void_t*)nArg, &nErrCode,
                  sizeof(DSL_Error_t));
            }
         }
         break;
#endif /* INCLUDE_DSL_CPE_TRACE_BUFFER*/

      case DSL_FIO_DBG_DEVICE_MESSAGE_SEND:
         pIOCTL_arg = DSL_DRV_VMalloc(sizeof(DSL_DeviceMessage_t));

         if(pIOCTL_arg == DSL_NULL)
         {
            DSL_IoctlMemCpyTo( bIsInKernel, (DSL_void_t*)nArg, &nErrCode,
               sizeof(DSL_Error_t));
            break;
         }

         if (nArg == 0)
         {
            nErrCode = DSL_ERR_INVALID_PARAMETER;
         }
         else
         {
            DSL_IoctlMemCpyFrom(bIsInKernel, pIOCTL_arg, (DSL_void_t*)nArg,
               sizeof(DSL_DeviceMessage_t));

            if ((pOldMsgBuf = pIOCTL_arg->dbgMsg.data.pMsg) == DSL_NULL)
            {
               nErrCode = DSL_ERR_INVALID_PARAMETER;
            }
            else
            {
               pMsgBuf = DSL_DRV_VMalloc(
                  (pIOCTL_arg->dbgMsg.data.nSizeRx
                     > pIOCTL_arg->dbgMsg.data.nSizeTx) ?
                     pIOCTL_arg->dbgMsg.data.nSizeRx :
                     pIOCTL_arg->dbgMsg.data.nSizeTx);
               if (pMsgBuf == DSL_NULL)
               {
                  nErrCode = DSL_ERR_MEMORY;
               }
               else
               {
                  DSL_IoctlMemCpyFrom(bIsInKernel, pMsgBuf, pOldMsgBuf,
                     pIOCTL_arg->dbgMsg.data.nSizeTx);

                  pIOCTL_arg->dbgMsg.data.pMsg = pMsgBuf;
                  nErrCode = DSL_DRV_DBG_DeviceMessageSend(pContext,
                     &pIOCTL_arg->dbgMsg);

                  if (nErrCode == DSL_SUCCESS)
                  {
                     if (pIOCTL_arg->dbgMsg.data.nSizeRx != 0)
                     {
                        DSL_IoctlMemCpyTo(bIsInKernel, pOldMsgBuf, pMsgBuf,
                           pIOCTL_arg->dbgMsg.data.nSizeRx);
                     }
                     pIOCTL_arg->dbgMsg.data.pMsg = pOldMsgBuf;
                     DSL_IoctlMemCpyTo( bIsInKernel, (DSL_void_t*)nArg, pIOCTL_arg,
                        sizeof(DSL_DeviceMessage_t));
                  }
                  DSL_DRV_VFree(pMsgBuf);
               }
            }

            DSL_IoctlMemCpyTo( bIsInKernel, (DSL_void_t*)nArg, &nErrCode,
               sizeof(DSL_Error_t));
         }
         break;

      case DSL_FIO_DBG_DEVICE_MESSAGE_MODIFY_SEND:
         pIOCTL_arg = DSL_DRV_VMalloc(sizeof(DSL_DeviceMessageModify_t));

         pMsgBuf = DSL_NULL;
         pMaskBuf = DSL_NULL;
         pDataBuf = DSL_NULL;

         if(pIOCTL_arg == DSL_NULL)
         {
            DSL_IoctlMemCpyTo( bIsInKernel, (DSL_void_t*)nArg, &nErrCode,
               sizeof(DSL_Error_t));
            break;
         }

         if (nArg == 0)
         {
            nErrCode = DSL_ERR_INVALID_PARAMETER;
         }
         else
         {
            DSL_IoctlMemCpyFrom(bIsInKernel, pIOCTL_arg, (DSL_void_t*)nArg,
               sizeof(DSL_DeviceMessageModify_t));

            if ((pOldMsgBuf = pIOCTL_arg->dbgMsgMod.data.pMsg) == DSL_NULL ||
                (pIOCTL_arg->dbgMsgMod.data.pMsgMask == NULL))
            {
               nErrCode = DSL_ERR_INVALID_PARAMETER;
            }
            else
            {
               i = (pIOCTL_arg->dbgMsgMod.data.nSizeRx > pIOCTL_arg->dbgMsgMod.data.nSizeTx) ?
                     pIOCTL_arg->dbgMsgMod.data.nSizeRx :
                     pIOCTL_arg->dbgMsgMod.data.nSizeTx;

               pMsgBuf = DSL_DRV_VMalloc( i );
               pMaskBuf = DSL_DRV_VMalloc( i );
               pDataBuf = DSL_DRV_VMalloc( i );

               nErrCode = DSL_SUCCESS;

               if ((pMsgBuf == DSL_NULL)||(pDataBuf == DSL_NULL)||(pMaskBuf == DSL_NULL))
               {
                  nErrCode = DSL_ERR_MEMORY;
               }
               else
               {
                  DSL_DeviceMessage_t msg;

                  pMsgId = (DSL_uint16_t*)pMsgBuf;

                  DSL_IoctlMemCpyFrom(bIsInKernel, pMsgBuf, pOldMsgBuf,
                     pIOCTL_arg->dbgMsgMod.data.nSizeRx);

                  DSL_IoctlMemCpyFrom(bIsInKernel, pDataBuf, pOldMsgBuf,
                     pIOCTL_arg->dbgMsgMod.data.nSizeRx);

                  DSL_IoctlMemCpyFrom(bIsInKernel, pMaskBuf, pIOCTL_arg->dbgMsgMod.data.pMsgMask,
                     pIOCTL_arg->dbgMsgMod.data.nSizeRx);

                  pIOCTL_arg->dbgMsgMod.data.pMsg = pMsgBuf;

                  msg.accessCtl = pIOCTL_arg->dbgMsgMod.accessCtl;
                  msg.data.pMsg = pIOCTL_arg->dbgMsgMod.data.pMsg;
                  msg.data.nSizeTx = 6;
                  msg.data.nSizeRx = pIOCTL_arg->dbgMsgMod.data.nSizeRx;

                  /*check command direction*/
                  if ((*pMsgId & 0x40) != 0)
                  { /*this is a WRITE command*/

                     /*read value, toggle direction bit*/
                     *pMsgId ^= 0x40;

                     nErrCode = DSL_DRV_DBG_DeviceMessageSend(pContext,&msg);

                     if (nErrCode == DSL_SUCCESS)
                     {  /*apply mask*/
                        DSL_uint8_t *pV = pMsgBuf + 6; /*skip message header*/
                        DSL_uint8_t *pD = pDataBuf + 6;
                        DSL_uint8_t *pM = pMaskBuf;

                        for (i = msg.data.nSizeRx-6; i > 0; i--, pV++, pM++, pD++)
                        {
                           *pV &= ~(*pM);
                           *pV |= *pD & *pM;
                        }

                        /*write value, set direction bit*/
                        *pMsgId |= 0x40;
                     }

                     msg.data.nSizeTx = pIOCTL_arg->dbgMsgMod.data.nSizeTx;

                  }

                  if (nErrCode == DSL_SUCCESS)
                  {
                     msg.data.nSizeRx = pIOCTL_arg->dbgMsgMod.data.nSizeRx;

                     nErrCode = DSL_DRV_DBG_DeviceMessageSend(pContext,&msg);

                     pIOCTL_arg->dbgMsgMod.data.nSizeRx = msg.data.nSizeRx > msg.data.nSizeTx ?
                        msg.data.nSizeRx : msg.data.nSizeTx;

                     if (nErrCode == DSL_SUCCESS)
                     {
                        if (pIOCTL_arg->dbgMsgMod.data.nSizeRx != 0)
                        {
                           DSL_IoctlMemCpyTo(bIsInKernel, pOldMsgBuf, pMsgBuf,
                              pIOCTL_arg->dbgMsgMod.data.nSizeRx);
                        }
                        pIOCTL_arg->dbgMsgMod.data.pMsg = pOldMsgBuf;
                        DSL_IoctlMemCpyTo( bIsInKernel, (DSL_void_t*)nArg, pIOCTL_arg,
                           sizeof(DSL_DeviceMessageModify_t));
                     }
                  }
               }
            }

            DSL_DRV_VFree(pMsgBuf);
            DSL_DRV_VFree(pDataBuf);
            DSL_DRV_VFree(pMaskBuf);

            DSL_IoctlMemCpyTo( bIsInKernel, (DSL_void_t*)nArg, &nErrCode,
               sizeof(DSL_Error_t));
         }
         break;

      case DSL_FIO_EVENT_STATUS_GET:
         pIOCTL_arg = DSL_DRV_VMalloc(sizeof(DSL_EventStatus_t));

         if(pIOCTL_arg == DSL_NULL)
         {
            DSL_IoctlMemCpyTo( bIsInKernel, (DSL_void_t*)nArg, &nErrCode,
               sizeof(DSL_Error_t));
            break;
         }

         if (nArg == 0)
         {
            nErrCode = DSL_ERR_INVALID_PARAMETER;
         }
         else
         {
            DSL_IoctlMemCpyFrom(bIsInKernel, pIOCTL_arg, (DSL_void_t*)nArg,
               sizeof(DSL_EventStatus_t));

            pEventData = pIOCTL_arg->event.data.pData;

            /* Lock Open Context list*/
            if (DSL_DRV_MUTEX_LOCK(pContext->pDevCtx->openContextListMutex))
            {
               DSL_DEBUG( DSL_DBG_ERR,
                  (DSL_NULL, SYS_DBG_ERR"DSL[%02d]: ERROR - Couldn't lock Context List mutex!"
                  DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

               nErrCode = DSL_ERR_SEMAPHORE_GET;
               break;
            }

            nErrCode = DSL_DRV_EventUnqueue(pOpenCtx, &pIOCTL_arg->event.data);

            /* Unlock Open Context list*/
            DSL_DRV_MUTEX_UNLOCK(pContext->pDevCtx->openContextListMutex);

            pIOCTL_arg->event.accessCtl.nReturn = nErrCode;

            if (nErrCode == DSL_SUCCESS)
            {
               if (pEventData != DSL_NULL && pIOCTL_arg->event.data.pData
                  != DSL_NULL)
               {
                  DSL_IoctlMemCpyTo( bIsInKernel, (DSL_void_t*)pEventData,
                     pIOCTL_arg->event.data.pData,
                     sizeof(DSL_EventData_Union_t));
               }

               if (pIOCTL_arg->event.data.pData != DSL_NULL)
               {
                  DSL_DRV_VFree(pIOCTL_arg->event.data.pData);
                  pIOCTL_arg->event.data.pData = pEventData;
               }
               else
               {
                  pIOCTL_arg->event.data.pData = pEventData;
               }

               DSL_IoctlMemCpyTo( bIsInKernel, (DSL_void_t*)nArg, pIOCTL_arg,
                  sizeof(DSL_EventStatus_t));
            }
            else
            {
               DSL_IoctlMemCpyTo( bIsInKernel, (DSL_void_t*)nArg, &nErrCode,
                  sizeof(DSL_Error_t));
            }

            if (nErrCode >= DSL_SUCCESS)
            {
               nErrCode = DSL_SUCCESS;
            }
         }

         break;

      case DSL_FIO_INSTANCE_CONTROL_SET:
         pIOCTL_arg = DSL_DRV_VMalloc(sizeof(DSL_InstanceControl_t));

         if(pIOCTL_arg == DSL_NULL)
         {
            DSL_IoctlMemCpyTo( bIsInKernel, (DSL_void_t*)nArg, &nErrCode,
               sizeof(DSL_Error_t));
            break;
         }

         if (nArg == 0)
         {
            nErrCode = DSL_ERR_INVALID_PARAMETER;
         }
         else
         {
            DSL_IoctlMemCpyFrom(bIsInKernel, pIOCTL_arg, (DSL_void_t*)nArg,
               sizeof(DSL_InstanceControl_t));

            nErrCode = DSL_SUCCESS;

            /* Get bEventActivation flag*/
            bEventActivation = (((DSL_InstanceControl_t*)nArg)->data.bEventActivation);
            /* Get nResourceActivationMask data*/
            nResourceActivationMask = (((DSL_InstanceControl_t*)nArg)->data.nResourceActivationMask);

            /* Lock Open Context list*/
            if (DSL_DRV_MUTEX_LOCK(pContext->pDevCtx->openContextListMutex))
            {
               DSL_DEBUG( DSL_DBG_ERR,
                  (DSL_NULL, SYS_DBG_ERR"DSL[%02d]: ERROR - Couldn't lock Context List mutex!"
                  DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

               nErrCode = DSL_ERR_SEMAPHORE_GET;
               break;
            }

            /*
               Proceed Event Activation Contriol settings
            */
            if(DSL_DRV_MUTEX_LOCK(pOpenCtx->eventMutex))
            {
               DSL_DEBUG( DSL_DBG_ERR,
                  (pContext, SYS_DBG_ERR"DSL[%02d]: Couldn't lock event mutex"
                  DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
               nErrCode = DSL_ERROR;
            }
            else
            {
               if (bEventActivation == DSL_FALSE)
               {
                  DSL_DRV_MUTEX_UNLOCK(pOpenCtx->eventMutex);
                  /** free event structures */
                  if (pOpenCtx->eventFifo != DSL_NULL && pOpenCtx->eventFifoBuf != DSL_NULL)
                  {
                     while (!DSL_Fifo_isEmpty(pOpenCtx->eventFifo))
                     {
                        if (DSL_DRV_EventUnqueue(pOpenCtx, &event) >= DSL_SUCCESS)
                        {
                           if (event.pData != DSL_NULL)
                           {
                              DSL_DRV_VFree(event.pData);
                           }
                        }
                     }
                  }

                  if(DSL_DRV_MUTEX_LOCK(pOpenCtx->eventMutex))
                  {
                     DSL_DEBUG( DSL_DBG_ERR,
                        (pContext, SYS_DBG_ERR"DSL[%02d]: Couldn't lock event mutex"
                        DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
                     nErrCode = DSL_ERROR;
                  }
                  else
                  {
                     /* release event structures */
                     if (pOpenCtx->eventFifo != DSL_NULL)
                     {
                        DSL_DRV_VFree(pOpenCtx->eventFifo);
                        pOpenCtx->eventFifo = DSL_NULL;
                     }

                     /* release event structures */
                     if (pOpenCtx->eventFifoBuf != DSL_NULL)
                     {
                        DSL_DRV_VFree(pOpenCtx->eventFifoBuf);
                        pOpenCtx->eventFifoBuf = DSL_NULL;
                     }
                  }
               }
               else
               {
                  /* initialize event structures */
                  pOpenCtx->eventFifo = DSL_DRV_VMalloc(sizeof(DSL_FIFO));
                  if (pOpenCtx->eventFifo == DSL_NULL)
                  {
                     DSL_DEBUG(DSL_DBG_ERR,
                        (DSL_NULL, SYS_DBG_ERR"DSL[%02d]: Open failed, "
                        "could not allocate memory for event FIFO..."DSL_DRV_CRLF,
                        DSL_DEV_NUM(pContext)));
                     nErrCode = DSL_ERR_MEMORY;
                  }
                  else
                  {
                     pOpenCtx->eventFifoBuf = DSL_DRV_VMalloc(DSL_EVENT_FIFO_ELEMENT_COUNT
                        * sizeof(DSL_EventStatusData_t));
                     if (pOpenCtx->eventFifoBuf == DSL_NULL)
                     {
                        DSL_DEBUG(DSL_DBG_ERR,
                           (DSL_NULL, SYS_DBG_ERR"DSL[%02d]: Open failed, "
                           "could not allocate memory for event FIFO..."DSL_DRV_CRLF,
                           DSL_DEV_NUM(pContext)));
                        DSL_DRV_VFree(pOpenCtx->eventFifo);
                        nErrCode = DSL_ERR_MEMORY;
                     }
                     else
                     {
                        /* init event fifo */
                        if (DSL_Fifo_Init(pOpenCtx->eventFifo,
                           pOpenCtx->eventFifoBuf,
                           pOpenCtx->eventFifoBuf +
                             (DSL_EVENT_FIFO_ELEMENT_COUNT - 1) *
                             sizeof(DSL_EventStatusData_t),
                           sizeof(DSL_EventStatusData_t)) != 0)
                        {
                           DSL_DEBUG(DSL_DBG_ERR,
                              (DSL_NULL, SYS_DBG_ERR"DSL[%02d]: Internal error in the DSL "
                              "CPE API. Could not prepare FIFO for the events."DSL_DRV_CRLF,
                              DSL_DEV_NUM(pContext)));
                           DSL_DRV_VFree(pOpenCtx->eventFifoBuf);
                           DSL_DRV_VFree(pOpenCtx->eventFifo);
                           nErrCode = DSL_ERR_INTERNAL;
                        }
                     }
                  }
               }

               pOpenCtx->bEventActivation = bEventActivation;
               pOpenCtx->nResourceActivationMask = nResourceActivationMask;
               DSL_DRV_MUTEX_UNLOCK(pOpenCtx->eventMutex);
            }

            /*
               Proceed Resource activation settings settings
            */
#ifdef INCLUDE_DSL_CEOC
            if(DSL_DRV_MUTEX_LOCK(pOpenCtx->rxSnmpFifoMutex))
            {
               DSL_DEBUG( DSL_DBG_ERR,
                  (pContext, SYS_DBG_ERR"DSL[%02d]: Couldn't lock Rx SNMP FIFO mutex"
                  DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
               nErrCode = DSL_ERR_SEMAPHORE_GET;
            }
            else
            {
               if (nResourceActivationMask & DSL_RESOURCE_ACTIVATION_SNMP)
               {
                  /* release event structures */
                  if (pOpenCtx->rxSnmpFifo != DSL_NULL)
                  {
                     DSL_DRV_VFree(pOpenCtx->rxSnmpFifo);
                     pOpenCtx->rxSnmpFifo = DSL_NULL;
                  }

                  /* release event structures */
                  if (pOpenCtx->rxSnmpFifoBuf != DSL_NULL)
                  {
                     DSL_DRV_VFree(pOpenCtx->rxSnmpFifoBuf);
                     pOpenCtx->rxSnmpFifoBuf = DSL_NULL;
                  }
               }
               else
               {
                  /* initialize event structures */
                  pOpenCtx->rxSnmpFifo = DSL_DRV_VMalloc(sizeof(DSL_FIFO));
                  if (pOpenCtx->rxSnmpFifo == DSL_NULL)
                  {
                     DSL_DEBUG(DSL_DBG_ERR,
                        (DSL_NULL, SYS_DBG_ERR"DSL[%02d]: Could not allocate memory for SNMP FIFO!!!"
                        DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
                     nErrCode = DSL_ERR_MEMORY;
                  }
                  else
                  {
                     pOpenCtx->rxSnmpFifoBuf =
                        DSL_DRV_VMalloc(DSL_CEOC_RX_FIFO_ELEMENT_COUNT * sizeof(DSL_G997_SnmpData_t));
                     if (pOpenCtx->rxSnmpFifoBuf == DSL_NULL)
                     {
                        DSL_DEBUG(DSL_DBG_ERR,
                           (DSL_NULL, SYS_DBG_ERR"DSL[%02d]: Could not allocate memory for SNMP FIFO buffer!!!"
                           DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
                        DSL_DRV_VFree(pOpenCtx->rxSnmpFifo);
                        nErrCode = DSL_ERR_MEMORY;
                     }
                     else
                     {
                        /* init event fifo */
                        if (DSL_Fifo_Init(
                               pOpenCtx->rxSnmpFifo,
                               pOpenCtx->rxSnmpFifoBuf,
                               pOpenCtx->rxSnmpFifoBuf +
                               sizeof(DSL_CEOC_Message_t)*(DSL_CEOC_RX_FIFO_ELEMENT_COUNT - 1),
                               sizeof(DSL_CEOC_Message_t)) != 0)
                        {
                           DSL_DEBUG(DSL_DBG_ERR,
                              (DSL_NULL, SYS_DBG_ERR"DSL[%02d]: Internal error in the DSL "
                              "CPE API. Could not prepare FIFO for the events."DSL_DRV_CRLF,
                              DSL_DEV_NUM(pContext)));
                           DSL_DRV_VFree(pOpenCtx->rxSnmpFifoBuf);
                           DSL_DRV_VFree(pOpenCtx->rxSnmpFifo);
                           nErrCode = DSL_ERR_INTERNAL;
                        }
                     }
                  }
               }
               pOpenCtx->nResourceActivationMask = nResourceActivationMask;
               DSL_DRV_MUTEX_UNLOCK(pOpenCtx->rxSnmpFifoMutex);
            }
#endif /* #ifdef INCLUDE_DSL_CEOC*/

            /* Unlock Open Context list*/
            DSL_DRV_MUTEX_UNLOCK(pContext->pDevCtx->openContextListMutex);

            DSL_IoctlMemCpyTo( bIsInKernel, (DSL_void_t*)nArg, &nErrCode,
               sizeof(DSL_Error_t));
         }
         break;

      default:
         /* Search for the available IOCTL*/
         while (pIoctlTable->pFunc)
         {
            if (pIoctlTable->nCommand != nCommand)
            {
               /* Move to the next Table entry*/
               pIoctlTable++;
               continue;
            }

            nErrCode = DSL_DRV_IoctlHandleHelperCall(pOpenCtx, pContext,
               bIsInKernel, pIoctlTable->bInstanceCall, pIoctlTable->accessType,
               pIoctlTable->pFunc, (DSL_void_t*)nArg, pIoctlTable->nArgSz);

            bIoctlFound = DSL_TRUE;
            break;
         }

         if (!bIoctlFound)
         {
            nErrCode = DSL_ERR_IOCTL_NOT_SUPPORTED;
            DSL_IoctlMemCpyTo( bIsInKernel, (DSL_void_t*)nArg, &nErrCode,
               sizeof(DSL_Error_t));
         }
         break;
   }

   if (pIOCTL_arg != DSL_NULL)
   {
      DSL_DRV_VFree(pIOCTL_arg);
   }

   DSL_DEBUG(DSL_DBG_MSG, (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_IoctlHandle,"
      " return(from %d - %s) %d"DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext), nCommand, DSL_DBG_IoctlName(nCommand),
      nErrCode));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_TestModeControlSet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_TestModeControl_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_TestModeControlSet_t nTestModeControl = DSL_TESTMODE_DISABLE;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_TestModeControlSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();


   if (pData->data.nTestMode >= DSL_TESTMODE_LAST ||
       pData->data.nTestMode < DSL_TESTMODE_DISABLE)
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Wrong test mode (%d) specified!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), pData->data.nTestMode));
     return DSL_ERR_INVALID_PARAMETER;
   }

   /* Get current settings for the Test mode*/
   DSL_CTX_READ(pContext, nErrCode, nTestModeControl, nTestModeControl);

   /* Call device specific stuff*/
   nErrCode = DSL_DRV_DEV_TestModeControlSet(pContext,pData);

   if ((pData->data.nTestMode == DSL_TESTMODE_DISABLE ||
        pData->data.nTestMode == DSL_TESTMODE_QUIET ||
        pData->data.nTestMode == DSL_TESTMODE_SLEEP ||
      ((pData->data.nTestMode == DSL_TESTMODE_SHOWTIME_LOCK) &&
        (nTestModeControl != DSL_TESTMODE_SHOWTIME_LOCK)) ||
      ((pData->data.nTestMode == DSL_TESTMODE_TRAINING_LOCK) &&
        (nTestModeControl != DSL_TESTMODE_TRAINING_LOCK))) && nErrCode == DSL_SUCCESS)
   {
      /* Trigger restart sequence*/
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, bAutobootRestart, DSL_TRUE);
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_TestModeControlSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_TestModeStatusGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_TestModeStatus_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_TestModeStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_CTX_READ(pContext, nErrCode, nTestModeControl, pData->data.nTestMode);

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_TestModeStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_LineStateUpdate(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_LineStateValue_t nLineState = DSL_LINESTATE_UNKNOWN,
                        nPrevLineState = DSL_LINESTATE_UNKNOWN;
#ifdef INCLUDE_REAL_TIME_TRACE
   DSL_boolean_t bRttProceed;
#endif /*#ifdef INCLUDE_REAL_TIME_TRACE*/

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_LineStateUpdate"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Get Line State from the DSL CPE internal memory*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineState, nPrevLineState);
   if(nErrCode != DSL_SUCCESS)
   {
      return nErrCode;
   }

#ifdef INCLUDE_REAL_TIME_TRACE
   DSL_CTX_READ_SCALAR(pContext, nErrCode, bRttProceed, bRttProceed);
   if(nErrCode != DSL_SUCCESS)
   {
      return nErrCode;
   }

   if (bRttProceed == DSL_TRUE)
   {
      DSL_DRV_DEV_RTT_StatusUpdate(pContext);
   }
#endif /*#ifdef INCLUDE_REAL_TIME_TRACE*/

   nErrCode = DSL_DRV_DEV_LineStateGet(pContext, &nLineState);
   if (nErrCode == DSL_SUCCESS)
   {
      DSL_DRV_LineStateSet(pContext, nLineState);
   }
   else
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Could not get line state "
         "from the device!"DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_LineStateUpdate, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

DSL_DEV_Handle_t DSL_DRV_LowDeviceGet(DSL_devCtx_t *pDevCtx)
{
   return pDevCtx->lowHandle;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_FwDownload(
   DSL_Context_t *pContext,
   const DSL_char_t *pFw1,
   DSL_uint32_t nSize1,
   const DSL_char_t *pFw2,
   DSL_uint32_t nSize2,
   DSL_int32_t *pLoff,
   DSL_int32_t *pCurrentOff,
   DSL_boolean_t bLastChunk)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CHECK_CTX_POINTER(pContext);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_POINTER(pContext, pContext->pDevCtx);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_POINTER(pContext, pLoff);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_POINTER(pContext, pCurrentOff);
   DSL_CHECK_ERR_CODE();

   DSL_DRV_LineStateSet(pContext, DSL_LINESTATE_IDLE_REQUEST);

   nErrCode = DSL_DRV_DEV_FwDownload(pContext,
         pFw1, nSize1, pFw2, nSize2, pLoff, pCurrentOff, bLastChunk);

   if (nErrCode < DSL_SUCCESS)
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - firmware download has failed "
         "(on %d byte)"DSL_DRV_CRLF, DSL_DEV_NUM(pContext), *pCurrentOff));
   }
   else
   {
      DSL_DEBUG( DSL_DBG_MSG,
         (pContext, SYS_DBG_MSG"DSL[%02d]: Firmware download finished "
         "(total %d bytes)"DSL_DRV_CRLF, DSL_DEV_NUM(pContext), *pCurrentOff));
   }

   return nErrCode;
}

DSL_Error_t DSL_DRV_LineStateSet(
   DSL_Context_t *pContext,
   DSL_LineStateValue_t nNewLineState)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_LineStateValue_t nOldLineState = DSL_LINESTATE_UNKNOWN;

   DSL_CHECK_CTX_POINTER(pContext);
   DSL_CHECK_ERR_CODE();

   DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineState, nOldLineState);

   if ((nNewLineState != nOldLineState) || (nNewLineState == DSL_LINESTATE_IDLE))
   {
      DSL_DEBUG(DSL_DBG_MSG, (pContext, SYS_DBG_MSG"DSL[%02d]: Line state has changed: "
         "%08X -> %08X"DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
         nOldLineState, nNewLineState));
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, nLineState, nNewLineState);

      /* There are special line states which needs to be signaled later on after
         some further action like copying data from firmware to API internal
         memory. In this cases the event will be not generated here. */
      switch (nNewLineState)
      {
#ifdef INCLUDE_DSL_FILTER_DETECTION
         /* These are the line states that will be signaled later on */
         case DSL_LINESTATE_TEST_FILTERDETECTION_COMPLETE:
            break;
#endif /*#ifdef INCLUDE_DSL_FILTER_DETECTION*/
         /* By default new line state is directly signaled */
         default:
            /* Showtime TC-Sync entry relevant handlings */
            if ((nOldLineState != DSL_LINESTATE_SHOWTIME_TC_SYNC) &&
                (nNewLineState == DSL_LINESTATE_SHOWTIME_TC_SYNC))
            {
               /* On showtime entry datarates are needed which are only available in
                  case of TC-Sync */
               DSL_DRV_DEV_MeiShowtimeSignaling(pContext, DSL_TRUE);
            }
            /* Showtime exit relevant handlings. In case of alternating TC-Sync and
               TC-No Sync NO showtime exit shall be signaled. */
            else if (((nOldLineState == DSL_LINESTATE_SHOWTIME_TC_SYNC) ||
                      (nOldLineState == DSL_LINESTATE_SHOWTIME_NO_SYNC)) &&
                      (nNewLineState != DSL_LINESTATE_SHOWTIME_TC_SYNC) &&
                      (nNewLineState != DSL_LINESTATE_SHOWTIME_NO_SYNC))
            {
               DSL_DRV_DEV_MeiShowtimeSignaling(pContext, DSL_FALSE);
            }

            nErrCode =  DSL_DRV_EventGenerate(
               pContext, 0, DSL_ACCESSDIR_NA, DSL_XTUDIR_NA,
               DSL_EVENT_S_LINE_STATE,
               (DSL_EventData_Union_t*)&nNewLineState,
               sizeof(DSL_LineStateData_t));

            if( nErrCode != DSL_SUCCESS )
            {
               DSL_DEBUG( DSL_DBG_ERR,
                  (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Event(%d) generate failed!"
                  DSL_DRV_CRLF, DSL_DEV_NUM(pContext), DSL_EVENT_S_LINE_STATE));
            }
         break;
      }
   }

   return nErrCode;
}

DSL_Error_t DSL_DRV_ShowtimeStatusUpdate(
   DSL_Context_t *pContext,
   DSL_boolean_t bInit)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_LineStateValue_t nLineState = DSL_LINESTATE_UNKNOWN;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_ShowtimeStatusUpdate"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   DSL_CHECK_CTX_POINTER(pContext);
   DSL_CHECK_ERR_CODE();

   DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineState, nLineState);

   if(nLineState != DSL_LINESTATE_SHOWTIME_TC_SYNC &&
      nLineState != DSL_LINESTATE_SHOWTIME_NO_SYNC &&
      nLineState != DSL_LINESTATE_EXCHANGE)
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - This function is applicable "
         "while line is in SHOWTIME state only"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      nErrCode = DSL_ERR_ONLY_AVAILABLE_IN_SHOWTIME;
   }
   else
   {
      nErrCode = DSL_DRV_DEV_ShowtimeStatusUpdate(pContext, bInit);
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_ShowtimeStatusUpdate, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_CtxDataUpdate(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint32_t tmp32 = 0;
   DSL_uint8_t i = 0;
   DSL_AccessDir_t nDir;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_CtxDataUpdate"DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext)));

   for (nDir = DSL_UPSTREAM; nDir < DSL_ACCESSDIR_LAST; nDir++)
   {
      for (i = 0 ; i < DSL_CHANNELS_PER_LINE; i++)
      {
         /* Update previous data rate values*/
         DSL_CTX_READ_SCALAR(pContext, nErrCode, nChannelActualDataRatePrev[nDir][i], tmp32);
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, nChannelPreviousDataRate[nDir][i], tmp32);

         /* Reset actual data rate values*/
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, nChannelActualDataRate[nDir][i], 0);
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, nChannelActualNetDataRate[nDir][i], 0);

         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, ActualInterleaveDelay[nDir][i], 0);
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, ActualImpulseNoiseProtection[nDir][i], 0);
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, ActualImpulseNoiseProtectionRein[nDir][i], 0);
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, ActualImpulseNoiseProtectionNoErasure[nDir][i], 0);
      }
   }

   /* Clean current XTSE status*/
   DSL_DRV_MemSet(pContext->xtseCurr, 0, DSL_G997_NUM_XTSE_OCTETS);

#ifdef INCLUDE_DSL_G997_LINE_INVENTORY
#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
   /* Clean current AUX line inventory information*/
   DSL_DRV_MemSet(&(pContext->auxInventoryFe), 0x0, sizeof(DSL_AuxLineInventoryData_t));
#endif

   /* Clean current FE line inventory information*/
   DSL_DRV_MemSet(&(pContext->lineInventoryFe), 0x0, sizeof(DSL_G997_LineInventoryData_t));
   /* Reset FE line inventory flag*/
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, bFeLineInventoryValid, DSL_FALSE);
   /* Reset FE line inventory incomplete flag*/
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, bFeLineInventoryIncomplete, DSL_FALSE);
#endif /* INCLUDE_DSL_G997_LINE_INVENTORY*/

   /* Clean line Feature status data, US and DS*/
   DSL_DRV_MemSet(&(pContext->lineFeatureDataSts[0]), 0x0, sizeof(DSL_LineFeatureData_t));
   DSL_DRV_MemSet(&(pContext->lineFeatureDataSts[1]), 0x0, sizeof(DSL_LineFeatureData_t));

#ifdef INCLUDE_DSL_CPE_API_DANUBE
   /* Clean FE line status data*/
   DSL_DRV_MemSet(&(pContext->lineStatusFe), 0x0, sizeof(DSL_G997_LineStatusBackupData_t));
   /* Clear ADSL1 mode indication*/
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.bAdsl1, DSL_FALSE);
   /* Clear ADSL2+ mode indication*/
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.bAdsl2p, DSL_FALSE);
#endif /* INCLUDE_DSL_CPE_API_DANUBE*/

#ifdef INCLUDE_DSL_SYSTEM_INTERFACE
#if defined(INCLUDE_DSL_CPE_API_VRX) || defined(INCLUDE_DSL_CPE_API_DANUBE)
   /* Clear System Interface status flag*/
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.bSystemIfStatusValid, DSL_FALSE);
#endif /* INCLUDE_DSL_CPE_API_VRX INCLUDE_DSL_CPE_API_DANUBE */
#endif /* INCLUDE_DSL_SYSTEM_INTERFACE */

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_CtxDataUpdate, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

/* Events stuff */

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_EventQueue(
   DSL_Context_t *pContext,
   DSL_IN DSL_EventStatusData_t *pEvent,
   DSL_IN DSL_uint32_t nDataSize)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint8_t *buf;
   DSL_EventStatusData_t *pEvBuf;
   DSL_OpenContext_t *pCurr;

   DSL_CHECK_POINTER(pContext, pContext->pDevCtx);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (DSL_NULL, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_EventQueue"DSL_DRV_CRLF,
      pOpenContextList->pDevCtx->nNum));

   if (pEvent == DSL_NULL)
   {
      return DSL_ERR_INVALID_PARAMETER;
   }

   /* Lock Open Context list*/
   if (DSL_DRV_MUTEX_LOCK(pContext->pDevCtx->openContextListMutex))
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (DSL_NULL, SYS_DBG_ERR"DSL[%02d]: ERROR - Couldn't lock Context List mutex!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERR_SEMAPHORE_GET;
   }

   for (pCurr = pOpenContextList; pCurr != DSL_NULL; pCurr = pCurr->pNext)
   {
      /* Queue event only for a specified device*/
      if (pContext->pDevCtx->nNum != pCurr->pDevCtx->nNum)
      {
         continue;
      }

      if(DSL_DRV_MUTEX_LOCK(pCurr->eventMutex))
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (DSL_NULL, SYS_DBG_ERR"DSL[%02d]: ERROR - Couldn't lock event mutex!"
            DSL_DRV_CRLF, pOpenContextList->pDevCtx->nNum));
      }
      else
      {
         if ((pCurr->eventFifo != DSL_NULL) &&
             (pCurr->eventFifoBuf != DSL_NULL) &&
             (pCurr->bEventActivation == DSL_TRUE) &&
             (pCurr->nEventMask & DSL_EVENT2MASK(pEvent->nEventType)) == 0)
         {
            if ( DSL_Fifo_isFull( pCurr->eventFifo ) == (DSL_int8_t)DSL_TRUE )
            {
               /* Set FIFO overflow indication flag*/
               pCurr->bFifoFull = DSL_TRUE;
               DSL_DEBUG( DSL_DBG_WRN,
                  (DSL_NULL, SYS_DBG_WRN"DSL[%02d]: WARNING - The fifo is full!"
                  DSL_DRV_CRLF, pOpenContextList->pDevCtx->nNum));
            }
            else
            {
               if ((buf = DSL_Fifo_writeElement(pCurr->eventFifo)) == DSL_NULL)
               {
                  DSL_DEBUG( DSL_DBG_ERR,
                     (DSL_NULL, SYS_DBG_ERR"DSL[%02d]: ERROR - Event could not be added!"
                     DSL_DRV_CRLF, pOpenContextList->pDevCtx->nNum));
               }
               else
               {
                  /* copy an element */
                  memcpy(buf, pEvent, sizeof(DSL_EventStatusData_t));

                  if (pEvent->pData != DSL_NULL)
                  {
                     pEvBuf = (DSL_EventStatusData_t *)buf;
                     pEvBuf->pData = DSL_DRV_VMalloc(nDataSize);
                     if (pEvBuf->pData != DSL_NULL)
                     {
                        /* copy an element data */
                        memcpy(pEvBuf->pData, pEvent->pData, nDataSize);
                     }
                     else
                     {
                        DSL_DEBUG( DSL_DBG_WRN,
                           (DSL_NULL, SYS_DBG_WRN"DSL[%02d]: WRN - Could not allocate memory for an "
                           "event data, it will be lost!"DSL_DRV_CRLF,
                           pOpenContextList->pDevCtx->nNum));
                     }
                  }

                  /* wake up */
                  DSL_DRV_WAKEUP_WAKELIST(pCurr->eventWaitQueue);
               }
            }
         }

         DSL_DRV_MUTEX_UNLOCK(pCurr->eventMutex);
      }
   }

   /* Unlock Open Context List*/
   DSL_DRV_MUTEX_UNLOCK(pContext->pDevCtx->openContextListMutex);

   /* free event data */
   if (pEvent->pData != DSL_NULL)
   {
      DSL_DRV_VFree(pEvent->pData);
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (DSL_NULL, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_EventQueue, retCode=%d"
      DSL_DRV_CRLF, pOpenContextList->pDevCtx->nNum, nErrCode));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_intern.h'
*/
DSL_Error_t DSL_DRV_EventUnqueue(
   DSL_IN DSL_OpenContext_t *pOpenContext,
   DSL_IN DSL_EventStatusData_t *pEvent)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint8_t *buf;

   if (pOpenContext == DSL_NULL || pEvent == DSL_NULL)
   {
      return DSL_ERR_INVALID_PARAMETER;
   }

   if (pOpenContext->eventFifo == DSL_NULL ||
      pOpenContext->eventFifoBuf == DSL_NULL)
   {
      DSL_DEBUG( DSL_DBG_WRN,
         (DSL_NULL, SYS_DBG_WRN"DSL[%02d]: WRN - Call to event routine before it "
         "is configured!"DSL_DRV_CRLF, pOpenContext->pDevCtx->nNum));
      return DSL_ERROR;
   }

   if(DSL_DRV_MUTEX_LOCK(pOpenContext->eventMutex))
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (DSL_NULL, SYS_DBG_ERR"DSL[%02d]: ERROR - Couldn't lock event mutex!"
         DSL_DRV_CRLF, pOpenContext->pDevCtx->nNum));
      return DSL_ERROR;
   }

   if ( DSL_Fifo_isEmpty( pOpenContext->eventFifo ) == (DSL_int8_t)DSL_TRUE )
   {
      DSL_DEBUG( DSL_DBG_WRN,
         (DSL_NULL, SYS_DBG_WRN"DSL[%02d]: WARNING - The fifo is empty"
         DSL_DRV_CRLF, pOpenContext->pDevCtx->nNum));

      pEvent->pData = DSL_NULL;
      nErrCode = DSL_WRN_DEVICE_NO_DATA;
   }
   else
   {
      if ((buf = DSL_Fifo_readElement(pOpenContext->eventFifo)) == DSL_NULL)
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (DSL_NULL, SYS_DBG_ERR"DSL[%02d]: ERROR - Event could not be read!"
            DSL_DRV_CRLF, pOpenContext->pDevCtx->nNum));
         nErrCode = DSL_ERROR;
      }
      else
      {
         /* copy an element */
         memcpy(pEvent, buf, sizeof(DSL_EventStatusData_t));

         /* Get the total FIFO element count*/
         pEvent->nEventStatusExt.nEventBufferSize = DSL_EVENT_FIFO_ELEMENT_COUNT;
         /* Get currently used FIFO elementy count*/
         pEvent->nEventStatusExt.nEventBufferFillStatus =
            DSL_Fifo_getCount(pOpenContext->eventFifo);

         /* Check for the occured FIFO overflow condition*/
         if (pOpenContext->bFifoFull)
         {
            /* Clear FIFO overflow indication flag*/
            pOpenContext->bFifoFull = DSL_FALSE;
            /* Signal FIFO overflow condition*/
            nErrCode = DSL_WRN_EVENT_FIFO_OVERFLOW;
         }
      }
   }

   DSL_DRV_MUTEX_UNLOCK(pOpenContext->eventMutex);

   DSL_DEBUG(DSL_DBG_MSG,
      (DSL_NULL, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_EventUnqueue, retCode=%d"
      DSL_DRV_CRLF, pOpenContext->pDevCtx->nNum, nErrCode));

   return nErrCode;
}

/* Helpers stuff */

/*
   This function converts a given value in unsigned 16-bit value in 2'complement
   format to its according signed 16-bit format.
   The type of 2'complement can be specified, for example:
   nsVal16 = 0x200, nBits = 10 ==> nuVal16 = -512

   \param nsVal16
      Specifies the unsigned 16-bit value that shall be converted, [I]
   \param nBits
      Specifies how many bits are used for 2'complement conversion.
      The bit that includes the sign is included, [I]
      \note It makes only sense to use this function in a range of
            2 <= nBits <= 15
            In case of nBits = 16 a simple cast will do the right thing.
   \param nuVal16
      Returns the converted 16-bit int value, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_TwosComplement16_HexToInt(
   DSL_Context_t *pContext,
   DSL_uint16_t nuVal16,
   DSL_char_t nBits,
   DSL_int16_t *nsVal16)
{
   DSL_uint16_t nFullScale = 0, nFullScaleMask = 0;

   *nsVal16 = 0;

   if ((nBits < 2) || (nBits > 16))
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: Type of 2'complement (%d)"
         " conversion out of valid range (2...16)"DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext), nBits));

      return DSL_ERROR;
   }

   nFullScale = (DSL_uint16_t) (1 << (nBits - 1));
   nFullScaleMask = nFullScale - 1;

   /* If msb is '1' it is a negative value */
   if (nuVal16 > (nFullScale - 1))
   {
      *nsVal16 = (DSL_int16_t) (nFullScaleMask & nuVal16);
      *nsVal16 -= ((DSL_int16_t) nFullScale);
   }
   else
   {
      *nsVal16 = (DSL_int16_t) nuVal16;
   }

   return DSL_SUCCESS;
}

#ifndef DRV_DSL_CPE_FORCE_MACROS
DSL_Error_t
_DSL_CHECK_CTX_POINTER(
   DSL_Context_t *pContext)
{
   if (pContext == DSL_NULL)
   {
      DSL_DEBUG_SET_ERROR(DSL_ERR_POINTER);
      DSL_DEBUG_HDR(DSL_DBG_ERR, (pContext, SYS_DBG_ERR"Invalid context pointer!"));
      return DSL_ERR_POINTER;
   }

   return DSL_SUCCESS;
}

DSL_Error_t
_DSL_CHECK_POINTER(
   DSL_Context_t *pContext,
   DSL_void_t *pPtr)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CHECK_CTX_POINTER(pContext);
   DSL_CHECK_ERR_CODE();

   if (pPtr == DSL_NULL)
   {
      DSL_DEBUG_SET_ERROR(DSL_ERR_POINTER);
      DSL_DEBUG_HDR(DSL_DBG_ERR, (pContext, SYS_DBG_ERR"Invalid data pointer!"));
      return DSL_ERR_POINTER;
   }

   return DSL_SUCCESS;
}

DSL_Error_t
_DSL_CHECK_MODEM_IS_READY(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CHECK_POINTER(pContext, pContext->pDevCtx);
   DSL_CHECK_ERR_CODE();

   if (DSL_DRV_DEV_ModemIsReady(pContext) == DSL_FALSE)
   {
      pContext->nErrNo = DSL_ERR_MODEM_NOT_READY;
      DSL_DEBUG(DSL_DBG_ERR, (pContext, SYS_DBG_ERR"Modem is not ready!"DSL_DRV_CRLF));
      return DSL_ERR_MODEM_NOT_READY;
   }

   return DSL_SUCCESS;
}

DSL_Error_t
_DSL_CHECK_CHANNEL_RANGE(
   DSL_Context_t *pContext,
   DSL_uint16_t nChannel)
{
   if (nChannel >= DSL_CHANNELS_PER_LINE)
   {
      DSL_DEBUG_SET_ERROR(DSL_ERR_CHANNEL_RANGE);
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL: invalid bearer channel number %d! (only bearer "
         "channels in the range of 0..%d are supported)" DSL_DRV_CRLF,
         nChannel, (DSL_CHANNELS_PER_LINE - 1)));
      return DSL_ERR_CHANNEL_RANGE;
   }

   return DSL_SUCCESS;
}

DSL_Error_t
_DSL_CHECK_DSLMODE(
      DSL_Context_t *pContext,
      DSL_DslModeSelection_t nDslMode)
{
   if ((nDslMode < 0) || (nDslMode >= DSL_MODE_LAST))
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL: invalid DSL mode!" DSL_DRV_CRLF));
      return DSL_ERR_DSLMODE;
   }

   return DSL_SUCCESS;
}

DSL_Error_t
_DSL_CHECK_ATU_DIRECTION(
   DSL_Context_t *pContext,
   DSL_XTUDir_t nDirection)
{
   if ((nDirection != DSL_NEAR_END) && (nDirection != DSL_FAR_END))
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL: invalid xTU direction!" DSL_DRV_CRLF));
      return DSL_ERR_DIRECTION;
   }

   return DSL_SUCCESS;
}

DSL_Error_t
_DSL_CHECK_DIRECTION(
   DSL_Context_t *pContext,
   DSL_AccessDir_t nDirection)
{
   if ((nDirection != DSL_DOWNSTREAM) && (nDirection != DSL_UPSTREAM))
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL: invalid direction!" DSL_DRV_CRLF));
      return DSL_ERR_DIRECTION;
   }

   return DSL_SUCCESS;
}

DSL_Error_t
_DSL_SET_ERROR_CODE(
   DSL_Error_t nRetCode,
   DSL_Error_t nErrCode)
{
   if ((nErrCode >= 0) && (nRetCode >= 0))
   {
      nErrCode = (nRetCode > nErrCode) ? nRetCode : nErrCode;
   }
   else
   {
      nErrCode = (nRetCode < nErrCode) ? nRetCode : nErrCode;
   }
   return nErrCode;
}

DSL_Error_t
_DSL_CTX_ASSIGN_ANY(
   DSL_Context_t *pContext,
   DSL_void_t *pFrom,
   DSL_void_t *pTo,
   DSL_uint32_t nSize)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CHECK_POINTER(pContext, pFrom);
   DSL_CHECK_POINTER(pContext, pTo);
   if (nSize == 0)
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext, SYS_DBG_ERR"_DSL_CTX_ASSIGN_ANY: "
         "nSize = 0!!!"DSL_DRV_CRLF));
      nErrCode |= DSL_ERR_INVALID_PARAMETER;
   }
   DSL_CHECK_ERR_CODE();

   if(DSL_DRV_MUTEX_LOCK(pContext->dataMutex))
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext, SYS_DBG_ERR"Couldn't lock data mutex "
         "(pContext=%p, pFrom=%p, pTo=%p"DSL_DRV_CRLF, pContext, pFrom, pTo));
      return DSL_ERR_SEMAPHORE_GET;
   }

   memcpy(pTo, pFrom, nSize);

   DSL_DRV_MUTEX_UNLOCK(pContext->dataMutex);

   return DSL_SUCCESS;
}

DSL_Error_t
_DSL_CTX_ASSIGN_8(
   DSL_Context_t *pContext,
   DSL_uint8_t nFrom,
   DSL_void_t *pTo)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CHECK_POINTER(pContext, pTo);
   DSL_CHECK_ERR_CODE();

   if(DSL_DRV_MUTEX_LOCK(pContext->dataMutex))
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext, SYS_DBG_ERR"Couldn't lock data mutex "
         "(pContext=%p, nFrom=%02x, pTo=%p"DSL_DRV_CRLF, pContext, nFrom, pTo));
      return DSL_ERR_SEMAPHORE_GET;
   }

   *((DSL_uint8_t*)pTo) = nFrom;

   DSL_DRV_MUTEX_UNLOCK(pContext->dataMutex);

   return DSL_SUCCESS;
}

DSL_Error_t
_DSL_CTX_ASSIGN_16(
   DSL_Context_t *pContext,
   DSL_uint16_t nFrom,
   DSL_void_t *pTo)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CHECK_POINTER(pContext, pTo);
   DSL_CHECK_ERR_CODE();

   if(DSL_DRV_MUTEX_LOCK(pContext->dataMutex))
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext, SYS_DBG_ERR"Couldn't lock data mutex "
         "(pContext=%p, nFrom=%04x, pTo=%p"DSL_DRV_CRLF, pContext, nFrom, pTo));
      return DSL_ERR_SEMAPHORE_GET;
   }

   *((DSL_uint16_t*)pTo) = nFrom;

   DSL_DRV_MUTEX_UNLOCK(pContext->dataMutex);

   return DSL_SUCCESS;
}

DSL_Error_t
_DSL_CTX_ASSIGN_32(
   DSL_Context_t *pContext,
   DSL_uint32_t nFrom,
   DSL_void_t *pTo)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CHECK_POINTER(pContext, pTo);
   DSL_CHECK_ERR_CODE();

   if(DSL_DRV_MUTEX_LOCK(pContext->dataMutex))
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext, SYS_DBG_ERR"Couldn't lock data mutex "
         "(pContext=%p, nFrom=%08x, pTo=%p"DSL_DRV_CRLF, pContext, nFrom, pTo));
      return DSL_ERR_SEMAPHORE_GET;
   }

   *((DSL_uint32_t*)pTo) = nFrom;

   DSL_DRV_MUTEX_UNLOCK(pContext->dataMutex);

   return DSL_SUCCESS;
}

#endif /*#ifndef DRV_DSL_CPE_FORCE_MACROS*/

#ifdef INCLUDE_DSL_FILTER_DETECTION
DSL_Error_t DSL_DRV_FilterDetectionInit(DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_HybridSelectionData_t hybridSelectionData;

   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, showtimeMeasurement.bFilterDetectionCompleted, DSL_FALSE);

   /* Initialize the filter detection related hybrid data with special/init
      values (rd/mod/wr) */
   DSL_CTX_READ(pContext, nErrCode, showtimeMeasurement.hybridSelectionData, hybridSelectionData);

   hybridSelectionData.fdActualSelection.nHybridIndex = -1;
   hybridSelectionData.fdActualSelection.nHybridMetric = 0;
   hybridSelectionData.fdSecondBestSelection.nHybridIndex = -1;
   hybridSelectionData.fdSecondBestSelection.nHybridMetric = 0;

   DSL_CTX_WRITE(pContext, nErrCode, showtimeMeasurement.hybridSelectionData, hybridSelectionData);

   return nErrCode;
}
#endif /* INCLUDE_DSL_FILTER_DETECTION */

DSL_Error_t DSL_DRV_HybridSelectionInit(DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_HybridSelectionData_t hybridSelectionData;

   /* Initialize the filter detection related hybrid data with special/init
      values (rd/mod/wr) */
   DSL_CTX_READ(pContext, nErrCode, showtimeMeasurement.hybridSelectionData, hybridSelectionData);

   hybridSelectionData.actualSelection.nHybridIndex = -1;
   hybridSelectionData.actualSelection.nHybridMetric = 0;
   hybridSelectionData.secondBestSelection.nHybridIndex = -1;
   hybridSelectionData.secondBestSelection.nHybridMetric = 0;

   DSL_CTX_WRITE(pContext, nErrCode, showtimeMeasurement.hybridSelectionData, hybridSelectionData);

   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, showtimeMeasurement.bLoopLengthValid, DSL_FALSE);

   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, showtimeMeasurement.nLoopLengthErrCode, DSL_ERR_DEVICE_NO_DATA);

   return nErrCode;
}

DSL_Error_t DSL_DRV_HybridContextReset(DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   nErrCode = DSL_DRV_HybridSelectionInit(pContext);
#ifdef INCLUDE_DSL_FILTER_DETECTION
   if (nErrCode == DSL_SUCCESS)
   {
      nErrCode = DSL_DRV_FilterDetectionInit(pContext);
   }
#endif /* #ifdef INCLUDE_DSL_FILTER_DETECTION*/
   return nErrCode;
}

DSL_Error_t DSL_DRV_HybridContextInit(DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_G997_LDSF_t nLoopMode = DSL_G997_INHIBIT_LDSF;

   DSL_CTX_READ_SCALAR(pContext, nErrCode, lineActivateConfig.nLDSF, nLoopMode);

#ifdef INCLUDE_DSL_FILTER_DETECTION
   if ((nLoopMode == DSL_G997_AUTO_FILTER_DETECTION) ||
       (nLoopMode == DSL_G997_FORCE_FILTER_DETECTION))
   {
      nErrCode = DSL_DRV_HybridContextReset(pContext);
      if (nErrCode != DSL_SUCCESS)
      {
         DSL_DEBUG(DSL_DBG_ERR, (pContext, "DSL[%02d]: ERROR - HybridContextReset "
            "init failed!"DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         /* Continue with activation sequence anyhow */
      }
   }
   else
   {
#endif /* #ifdef INCLUDE_DSL_FILTER_DETECTION*/
      if (nLoopMode == DSL_G997_INHIBIT_LDSF)
      {
         nErrCode = DSL_DRV_HybridSelectionInit(pContext);
         if (nErrCode != DSL_SUCCESS)
         {
            DSL_DEBUG(DSL_DBG_ERR, (pContext, "DSL[%02d]: ERROR - HybridContextInit "
               "init failed!"DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
            /* Continue with activation sequence anyhow */
         }
      }
#ifdef INCLUDE_DSL_FILTER_DETECTION
   }
#endif /* #ifdef INCLUDE_DSL_FILTER_DETECTION*/

   return nErrCode;
}

#ifdef INCLUDE_DSL_DELT
/*
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_DELT_SNR_GET
*/
DSL_Error_t DSL_DRV_DeltSNRGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_G997_DeltSnr_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_DIRECTION(pData->nDirection);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DeltSNRGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Call device specific implementation*/
   nErrCode = DSL_DRV_DEV_DeltSNRGet(
                 pContext, pData->nDirection,
                 pData->nDeltDataType, &(pData->data));

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DeltSNRGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}
#endif /* INCLUDE_DSL_DELT */

#ifdef __cplusplus
}
#endif
