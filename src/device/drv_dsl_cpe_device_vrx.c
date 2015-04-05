/******************************************************************************

                               Copyright (c) 2011
                            Lantiq Deutschland GmbH
                     Am Campeon 3; 85579 Neubiberg, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/
#define DSL_INTERN

#include "drv_dsl_cpe_api.h"

#ifdef INCLUDE_DSL_CPE_API_VRX

#include "drv_dsl_cpe_device_g997.h"

#undef DSL_DBG_BLOCK
#define DSL_DBG_BLOCK DSL_DBG_DEVICE

/*
  VRX chip strings and codes
  will be truncated to MAX_INFO_STRING_LEN
 */
static VNX_CodeString_t ChipString[]=
{
   {0x00000001, "VRX-DFE_V1.1"},
   {0x00000000, DSL_NULL}
};


/* supported modes in ADSL Annex A FW */
static const DSL_uint8_t DSL_XtseAdslA[DSL_G997_NUM_XTSE_OCTETS] =
{0x05, 0x00, 0x04, 0x00, 0x4C, 0x01, 0x04, 0x00};

/* supported modes in ADSL Annex B FW */
static const DSL_uint8_t DSL_XtseAdslB[DSL_G997_NUM_XTSE_OCTETS] =
{0x10, 0x00, 0x10, 0x40, 0x00, 0x04, 0x01, 0x00};

/* supported modes in VDSL2 FW */
static const DSL_uint8_t DSL_XtseVdsl2[DSL_G997_NUM_XTSE_OCTETS] =
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07};

static DSL_Error_t DSL_DRV_VRX_InitDeviceDrv(
   DSL_Context_t *pContext,
   DSL_DeviceLowLevelConfig_t *pLineCfg);

static DSL_Error_t DSL_DRV_VRX_LineFailuresNeStatusGet(
   DSL_Context_t *pContext);

static DSL_Error_t DSL_DRV_VRX_LineFailuresFeStatusGet(
   DSL_Context_t *pContext);

static DSL_Error_t DSL_DRV_VRX_ReTxStatusGet(
   DSL_Context_t *pContext);

static DSL_Error_t DSL_DRV_VRX_ReTxCountersGet(
   DSL_Context_t *pContext,
   DSL_ReTxCounters_t *pCounters);

#undef DSL_DBG_BLOCK
#define DSL_DBG_BLOCK DSL_DBG_MULTIMODE
/*CAM*/
/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device_vxx.h'
*/
DSL_Error_t DSL_DRV_VXX_CamFsmStateSet(
   DSL_Context_t *pContext,
   const DSL_DEV_CamStates_t nCamStateNew)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_DEV_CamStates_t nCamStateCurr;

   /* Get CAM state information*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->data.nCamState, nCamStateCurr);

   if (nCamStateCurr == DSL_CAM_DISABLED)
   {
      DSL_DEBUG(DSL_DBG_MSG, (pContext,
         SYS_DBG_MSG"DSL[%02d]: CAM - no state change, CAM disabled" DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));

      return DSL_SUCCESS;
   }

   if (nCamStateNew > DSL_CAM_LAST)
   {
      DSL_DEBUG(DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: CAM - state set failed, illegal state %d!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nCamStateNew));

      return DSL_ERROR;
   }

   /* no change */
   if (nCamStateCurr == nCamStateNew)
      return nErrCode;

   DSL_DEBUG(DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: CAM - state changed %d -> %d" DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext), nCamStateCurr, nCamStateNew));

   /* Set CAM state information*/
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.nCamState, nCamStateNew);
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.nCamReinits, 0);
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.nCamFwReinits, 0);

   return(nErrCode);
}

DSL_DEV_CamStates_t DSL_DRV_VXX_CamFsmStateGet(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_DEV_CamStates_t nCamStateCurr;

   /* Get CAM state information*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->data.nCamState, nCamStateCurr);

   return (nErrCode == DSL_SUCCESS ? nCamStateCurr : DSL_CAM_DISABLED);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device_vxx.h'
*/
DSL_Error_t DSL_DRV_VXX_CamExceptionHandle(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_DEV_CamStates_t nCurrentState = DSL_DRV_VXX_CamFsmStateGet(pContext);
   DSL_uint8_t nCamReinits = 0;
   DSL_uint8_t nCamFwReinits = 0;
   DSL_G997_LineInitStatusData_t lineInitStatus = {LINIT_UNKNOWN, LINIT_SUB_NONE};
   DSL_uint8_t nSpar1 = 0, nSpar2 = 0, nSpar3 = 0,
               nSpar4 = 0, nSpar5 = 0;
   DSL_MultimodeFsmConfigData_t nCamOpt;
   DSL_MultimodeFsmStatusData_t nCamStatus;
   DSL_boolean_t bFsmStatusChange = DSL_FALSE;
   DSL_boolean_t bVdsl = DSL_FALSE, bAdsl = DSL_FALSE;
   DSL_uint8_t nXTSE1 = 0;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VXX_CamExceptionHandle"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   if( DSL_DRV_VXX_CamFsmStateGet(pContext) != DSL_CAM_DISABLED )
   {
      /* Get reinit counters*/
      DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.nCamReinits, nCamReinits);
      DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.nCamFwReinits, nCamFwReinits);

      /* Read FSM status values*/
      DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.nCamStatus, nCamOpt);

      /* Get Line Init info*/
      DSL_CTX_READ(pContext, nErrCode, lineInitStatus, lineInitStatus);

      /* Update FW reinit counter*/
      if (lineInitStatus.nLineInitSubStatus == LINIT_SUB_FW_RETRY)
      {
         nCamFwReinits++;
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - nCamFwReinits=%u"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nCamFwReinits));
      }
      else
      {
         /* Update general reinit counter*/
         nCamReinits++;
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - nCamReinits=%u"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nCamReinits));
      }

      DSL_DEBUG(DSL_DBG_MSG,
         (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - nLineInitSubStatus=%u"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), lineInitStatus.nLineInitSubStatus));

      /* decide on the fail reasons state changes */
      if( (lineInitStatus.nLineInitSubStatus == LINIT_SUB_NO_COMMON_MODE) ||
          (lineInitStatus.nLineInitSubStatus == LINIT_SUB_OPP_MODE))
      {
         /* Get SPAR1 info*/
         DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->data.nSpar1, nSpar1);
         DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->data.nSpar2, nSpar2);
         DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->data.nSpar3, nSpar3);
         DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->data.nSpar4, nSpar4);
         DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->data.nSpar5, nSpar5);

         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - nSpar [0x%02X 0x%02X 0x%02X 0x%02X 0x%02X]"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nSpar1, nSpar2, nSpar3, nSpar4, nSpar5));

         /* check for VDSL mode */
         if( nSpar5 & 0x20 )
         {
            bVdsl = DSL_TRUE;
         }

         /* check for any ADSL mode */
         if( (nSpar1 & 0x7F) || (nSpar2 & 0x03) ||
             (nSpar3 & 0x7F) || (nSpar4 & 0x1F) || (nSpar5 & 0x15) )
         {
            bAdsl = DSL_TRUE;
         }
      }

      /* dependent on the current state
         retries or state changes have to be done.
         Counter resets are done as part of a state change */
      switch (nCurrentState)
      {
      case DSL_CAM_MULTI:
         /* Update XTSE status information*/
         nErrCode = DSL_DRV_VXX_SendMsgXtseStatusGet( pContext );
         if(nErrCode != DSL_SUCCESS)
         {
            DSL_DEBUG( DSL_DBG_ERR, (pContext,
               SYS_DBG_ERR"DSL[%02d]: ERROR - XTSE status get failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
            return nErrCode;
         }

         /* Read XTSE1 bits */
         DSL_CTX_READ(pContext, nErrCode, xtseCurr[0], nXTSE1);

         /* Update the current activation mode value */
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode,
            pDevCtx->data.nActivationCfg.nActivationMode,
            (nXTSE1 & XTSE_1_01_A_T1_413) ? DSL_ACT_MODE_ANSI_T1413 : DSL_ACT_MODE_GHS);

         nCamOpt.nNextMode = DSL_FW_TYPE_VDSL;
         bFsmStatusChange = bVdsl;
         break;

      case DSL_CAM_VDSL_FORCED:
         nCamOpt.nNextMode = DSL_FW_TYPE_ADSL;
         bFsmStatusChange = bAdsl;
         break;
      default:
         nCamOpt.nNextMode = DSL_FW_TYPE_VDSL;
         break;
      }

      /* Update reinit counters*/
      DSL_CTX_WRITE(pContext, nErrCode, pDevCtx->data.nCamReinits, nCamReinits);
      DSL_CTX_WRITE(pContext, nErrCode, pDevCtx->data.nCamFwReinits, nCamFwReinits);

      if( (nCamReinits >= DSL_CAM_MAX_REINITS) || (nCamFwReinits >= DSL_CAM_MAX_FW_REINITS) ||
               bFsmStatusChange )
      {
         /* Write FSM status values*/
         DSL_CTX_WRITE(pContext, nErrCode, pDevCtx->data.nCamStatus, nCamOpt);
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.bRebootRequested, DSL_TRUE);

         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - status nNextMode=%u"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nCamOpt.nNextMode));

         nErrCode = DSL_DRV_VXX_CamFsmStateSet(pContext, DSL_CAM_INIT);
         if( nErrCode != DSL_SUCCESS )
         {
            DSL_DEBUG( DSL_DBG_MSG, (pContext,
               SYS_DBG_MSG"DSL[%02d]: CAM - FSM state set failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            return nErrCode;
         }
         /* Set event structure*/
         nCamStatus.bRebootRequested = DSL_TRUE;
         nCamStatus.nNextMode = nCamOpt.nNextMode;

         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - event(%d) (nNextMode=%u)"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), DSL_EVENT_S_MULTIMODE_FSM_STATUS,
            nCamStatus.nNextMode));

         /* Send notification*/
         nErrCode = DSL_DRV_EventGenerate(
                        pContext, 0, DSL_ACCESSDIR_NA, DSL_XTUDIR_NA,
                        DSL_EVENT_S_MULTIMODE_FSM_STATUS,
                        (DSL_EventData_Union_t*)&nCamStatus,
                        sizeof(DSL_MultimodeFsmStatusData_t));
         if( nErrCode != DSL_SUCCESS )
         {
            DSL_DEBUG( DSL_DBG_ERR,
               (pContext, SYS_DBG_ERR"DSL[%02d]: CAM - event(%d) generate failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext), DSL_EVENT_S_MULTIMODE_FSM_STATUS));
            return nErrCode;
         }
      }
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VXX_CamExceptionHandle"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return(nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device_vxx.h'
*/
DSL_Error_t DSL_DRV_VXX_CamFwLoadNeededCheck(
   DSL_Context_t *pContext,
   DSL_boolean_t *bDownloadRequired,
   DSL_VNX_FwMode_t *nFwMode)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_DEV_CamStates_t nCamState = DSL_DRV_VXX_CamFsmStateGet(pContext);
   DSL_uint8_t i, nAtse = 0, nAtseCurr = 0, nVtse = 0;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VXX_CamFwLoadNeededCheck"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   *nFwMode = FWMODE_VDSL;
   *bDownloadRequired = DSL_FALSE;

   /* get the ADSL bits */
   for (i = 0; i < DSL_G997_NUM_XTSE_OCTETS - 1; i++)
   {
      DSL_CTX_READ_SCALAR(pContext, nErrCode, xtseCfg[i], nAtseCurr);
      nAtse |= nAtseCurr;
   }

   /* get the VDSL bits */
   DSL_CTX_READ_SCALAR(pContext, nErrCode, xtseCfg[7], nVtse);

   /* Check if both ADSL and VDSL setings are not configured*/
   if ((nAtse == 0) && (nVtse == 0))
   {
      DSL_DEBUG(DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: CAM - No valid XTSE configuration!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return nErrCode;
   }

   switch (nCamState)
   {
   case DSL_CAM_INIT:
   case DSL_CAM_DISABLED:
      /* prefer ADSL FW also in multi mode */
      if (nAtse != 0)
      {
         *nFwMode = FWMODE_ADSL;
      }
      else
      {
         *nFwMode = FWMODE_VDSL;
      }
      break;
   case DSL_CAM_MULTI:
      if (nAtse)
      {
         *nFwMode = FWMODE_ADSL;
      }
      else
      {
         /* XTSE configuration changed. Try VDSL mode.*/
         *nFwMode = FWMODE_VDSL;
         /* Reset CAM FSM to INIT state*/
         nCamState = DSL_CAM_INIT;
      }
      break;
   case DSL_CAM_VDSL_FORCED:
   case DSL_CAM_VDSL_RETRY:
      if (nVtse)
      {
         *nFwMode = FWMODE_VDSL;
      }
      else
      {
         /* XTSE configuration changed. Try ADSL mode.*/
         *nFwMode = FWMODE_ADSL;
         /* Reset CAM FSM to INIT state*/
         nCamState = DSL_CAM_INIT;
      }
      break;
   default:
      DSL_DEBUG(DSL_DBG_WRN, (pContext,
         SYS_DBG_WRN"DSL[%02d]: DSL_VXX_CamFwLoad unhandled state %d"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nCamState));

      break;
   }

   /* Update CAM state*/
   nErrCode = DSL_DRV_VXX_CamFsmStateSet(pContext, nCamState);
   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - CAM FSM state set failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERROR;
   }

   /* now check if we need a new download */
   if (*nFwMode == FWMODE_ADSL)
   {
      if (DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2))
      {
         *bDownloadRequired = DSL_TRUE;
      }
   }
   else
   {
      if (!DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2))
      {
         *bDownloadRequired = DSL_TRUE;
      }
   }

   DSL_DEBUG(DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: CAM - FW reload needed=%d, type=%s"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
      *bDownloadRequired, ((*nFwMode == FWMODE_VDSL)?"VDSL":"ADSL")));

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VXX_CamFwLoadNeededCheck"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return(nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device_vxx.h'
*/
DSL_Error_t DSL_DRV_VXX_CamFsmUpdate(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint8_t nXtse1 = 0, nVtse = 0, nAtse = 0, nAtseCurr = 0, i = 0;
   DSL_VNX_FwMode_t nFwType;
   DSL_boolean_t bDownload = DSL_FALSE;
   DSL_MultimodeFsmConfigData_t nCamOpt;
   DSL_DEV_CamStates_t nCamStateNew = DSL_CAM_MULTI;

   DSL_CHECK_CTX_POINTER(pContext);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VXX_CamFsmUpdate."
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* get the ADSL bits */
   for (i = 0; i < DSL_G997_NUM_XTSE_OCTETS - 1; i++)
   {
      DSL_CTX_READ_SCALAR(pContext, nErrCode, xtseCfg[i], nAtseCurr);
      nAtse |= nAtseCurr;
   }

   /* get the VDSL bits */
   DSL_CTX_READ_SCALAR(pContext, nErrCode, xtseCfg[7], nVtse);

   /* get the XTSE1 bits */
   DSL_CTX_READ_SCALAR(pContext, nErrCode, xtseCfg[0], nXtse1);

   if (!(nAtse && nVtse))
   {
      DSL_DRV_VXX_CamFsmStateSet(pContext, DSL_CAM_DISABLED);
      DSL_DEBUG(DSL_DBG_MSG,
         (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - no multimode XTSE settings detected, CAM disabled"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }
   else
   {
      if (DSL_DRV_VXX_CamFsmStateGet(pContext) == DSL_CAM_DISABLED)
      {
         DSL_DRV_VXX_CamFsmEnable(pContext, DSL_TRUE);
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - multimode XTSE settings detected, CAM enabled"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      }
   }

   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.bT1_413,
      nXtse1 & XTSE_1_01_A_T1_413 ? DSL_TRUE : DSL_FALSE);

   /* In INIT state modify the CPE Auto mode FSM
      according to the xTSE bits */
   if( DSL_DRV_VXX_CamFsmStateGet(pContext) == DSL_CAM_INIT )
   {
      /* Read FSM status values*/
      DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.nCamStatus, nCamOpt);

      if (nCamOpt.nNextMode == DSL_FW_TYPE_NA)
      {
         /* Read FSM configuration values*/
         DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.nCamConfig, nCamOpt);

         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - use config nNextMode=%d"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nCamOpt.nNextMode));
      }
      else
      {
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - use status nNextMode=%d"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nCamOpt.nNextMode));
      }

      /* Reset multimode operation flag*/
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.bMultimode, DSL_FALSE);

      if (nAtse && nVtse)
      {
         switch(nCamOpt.nNextMode)
         {
         default:
         case DSL_FW_TYPE_NA:
         case DSL_FW_TYPE_VDSL:
            /* Try again in VDSL mode*/
            nCamStateNew = DSL_CAM_VDSL_FORCED;
            /* Set Activation mode to GHS*/
            DSL_CTX_WRITE_SCALAR(pContext, nErrCode,
               pDevCtx->data.nActivationCfg.nActivationMode, DSL_ACT_MODE_GHS);

            /* Reset hybrid,looplength,mfd context */
            /* hybrid,looplength,mfd is not supported by VDSL firmware */
            DSL_DRV_HybridContextReset(pContext);
            break;

         case DSL_FW_TYPE_ADSL:
            nCamStateNew = DSL_CAM_MULTI;
            /* Set multimode operation flag*/
            DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.bMultimode, DSL_TRUE);
            break;
         }
      }
      else if (nAtse)
      {
         nCamStateNew = DSL_CAM_MULTI;
         /* Set multimode operation flag*/
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.bMultimode, DSL_TRUE);
      }
      else if (nVtse)
      {
         nCamStateNew = DSL_CAM_VDSL_FORCED;
         /* Clear hybrid,looplength,mfd context */
         /* hybrid,looplength,mfd is not supported by VDSL firmware */
         DSL_DRV_HybridContextReset(pContext);
      }

      /* Set New CAM state*/
      nErrCode = DSL_DRV_VXX_CamFsmStateSet(pContext, nCamStateNew);
      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: CAM - FSM state (%u) set failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nCamStateNew));

         return DSL_ERROR;
      }
   }

   /* check the loaded FW and load a different one if needed */
   nErrCode = DSL_DRV_VXX_CamFwLoadNeededCheck(pContext, &bDownload, &nFwType);
   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: CAM - FW check failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERROR;
   }

   /* Check if it is neccessary to reload the FW binary*/
   if( bDownload )
   {
      if( nFwType == FWMODE_VDSL )
      {
         /* Set VDSL FW image type*/
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.deviceCfg.nFwType, DSL_FW_REQUEST_VDSL);
         DSL_CHECK_ERR_CODE();
      }
      else {
         /* Set ADSL FW image type*/
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.deviceCfg.nFwType, DSL_FW_REQUEST_ADSL);
         DSL_CHECK_ERR_CODE();
      }

      /* Set bCamFwReload flag*/
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.bCamFwReload, DSL_TRUE);
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VXX_CamFsmUpdate"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return(nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device_vxx.h'
*/
DSL_Error_t DSL_DRV_VXX_CamFsmEnable(
   DSL_Context_t *pContext,
   const DSL_boolean_t bEnable)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   if (bEnable)
   {
      /* if already enabled, this causes a reset of the state machine */
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.nCamState, DSL_CAM_INIT);
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.nCamReinits, 0);
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.nCamFwReinits, 0);
      /* Reset FSM status values*/
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.nCamStatus.nNextMode, DSL_FW_TYPE_NA);
   }
   else
   {
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.nCamState, DSL_CAM_DISABLED);
   }

   DSL_DEBUG(DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: CAM - FSM %s" DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext), (bEnable?"enable":"disable")));

   return nErrCode;
}

DSL_Error_t DSL_DRV_VXX_CamTrainingTimeoutHandle(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_DEV_CamStates_t nCamState = DSL_DRV_VXX_CamFsmStateGet(pContext);
   DSL_boolean_t bFsmStatusChange = DSL_FALSE;
   DSL_MultimodeFsmConfigData_t nCamOpt;
   DSL_MultimodeFsmStatusData_t nCamStatus;
   DSL_boolean_t bT1_413 = DSL_FALSE;

   DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.bT1_413, bT1_413);

   if (bT1_413)
   {
      switch (nCamState)
      {
         case DSL_CAM_VDSL_FORCED:
            nCamOpt.nNextMode = DSL_FW_TYPE_ADSL;
            bFsmStatusChange = DSL_TRUE;
            /* Set Activation mode to ANSI-T1.413 */
            DSL_CTX_WRITE_SCALAR(pContext, nErrCode,
               pDevCtx->data.nActivationCfg.nActivationMode, DSL_ACT_MODE_ANSI_T1413);
            break;

         case DSL_CAM_MULTI:
            nCamOpt.nNextMode = DSL_FW_TYPE_VDSL;
            bFsmStatusChange = DSL_TRUE;
            break;

         default:
            break;
      }
   }

   if (bFsmStatusChange)
   {
      /* Write FSM status values*/
      DSL_CTX_WRITE(pContext, nErrCode, pDevCtx->data.nCamStatus, nCamOpt);
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.bRebootRequested, DSL_TRUE);

      DSL_DEBUG(DSL_DBG_MSG,
         (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - status nNextMode=%u"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nCamOpt.nNextMode));


      /* Set event structure*/
      nCamStatus.bRebootRequested = DSL_TRUE;
      nCamStatus.nNextMode = nCamOpt.nNextMode;

      DSL_DEBUG(DSL_DBG_MSG,
         (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - event(%d) (nNextMode=%u)"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), DSL_EVENT_S_MULTIMODE_FSM_STATUS,
         nCamStatus.nNextMode));

      /* Send notification*/
      nErrCode = DSL_DRV_EventGenerate(
                     pContext, 0, DSL_ACCESSDIR_NA, DSL_XTUDIR_NA,
                     DSL_EVENT_S_MULTIMODE_FSM_STATUS,
                     (DSL_EventData_Union_t*)&nCamStatus,
                     sizeof(DSL_MultimodeFsmStatusData_t));
      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: CAM - event(%d) generate failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), DSL_EVENT_S_MULTIMODE_FSM_STATUS));
         return nErrCode;
      }
   }

   nErrCode = DSL_DRV_VXX_CamFsmStateSet(pContext, DSL_CAM_INIT);
   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_MSG, (pContext,
         SYS_DBG_MSG"DSL[%02d]: CAM - FSM state set failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   return nErrCode;
}

#undef DSL_DBG_BLOCK
#define DSL_DBG_BLOCK DSL_DBG_DEVICE

DSL_boolean_t DSL_DRV_VXX_FwFeatureCheck(
   DSL_Context_t           *pContext,
   const DSL_VXX_Feature_t nFeature)
{
   DSL_VNX_ApplicationCode_t nAppType =
      (DSL_VNX_ApplicationCode_t)pContext->pDevCtx->data.fwFeatures.nApplication;
   DSL_boolean_t bReturn = DSL_FALSE;

   switch (nFeature)
   {
   case DSL_VXX_FW_VDSL2:
      if ((nAppType == DSL_APP_VDSL_POTS ) ||
          (nAppType == DSL_APP_VDSL_ISDN ) )
         bReturn = DSL_TRUE;
      break;
   case DSL_VXX_FW_ADSLA:
      if (nAppType == DSL_APP_ADSLA)
         bReturn = DSL_TRUE;
      break;
   case DSL_VXX_FW_ADSLB:
      if ((nAppType == DSL_APP_ADSLB_J) ||
          (nAppType == DSL_APP_ADSLB  ))
         bReturn = DSL_TRUE;
      break;
   case DSL_VXX_FW_ADSL:
      if ((nAppType == DSL_APP_ADSLA) ||
          (nAppType == DSL_APP_ADSLB_J) ||
          (nAppType == DSL_APP_ADSLB  ))
         bReturn = DSL_TRUE;
      break;
   }

   DSL_DEBUG( DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: DSL_DRV_VXX_FwFeatureCheck %d vs FW %d, result %d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nFeature, nAppType, bReturn));

   return (bReturn);
}

/*
   This function gets the NE/FE data path failures and sets them to the
   DSL CPE context.

   \param pContext - Pointer to dsl library context structure, [I/O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VXX_DataPathFailuresStatusGet(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   ACK_DataPathFailuresGet_t sAck;

   DSL_uint32_t nActDPF_NE = 0;
   DSL_uint32_t nActDPF_FE = 0;
   DSL_uint32_t nPreDPF_NE = 0;
   DSL_uint32_t nPreDPF_FE = 0;
#ifdef INCLUDE_DSL_G997_ALARM
   DSL_uint32_t nActDPFM_NE = 0;
   DSL_uint32_t nActDPFM_FE = 0 ;
   DSL_uint32_t nCfgDPFM_NE = 0;
   DSL_uint32_t nCfgDPFM_FE = 0;
#endif /* INCLUDE_DSL_G997_ALARM*/
   DSL_uint32_t nRelevantBitMask = 0;
   DSL_LineStateValue_t nLineState = DSL_LINESTATE_UNKNOWN;

   nRelevantBitMask = DSL_G997_DATAPATHFAILURE_NCD |
                      DSL_G997_DATAPATHFAILURE_LCD |
                      DSL_G997_DATAPATHFAILURE_OOS;

   nErrCode = DSL_DRV_VRX_SendMsgDataPathFailuresGet(
                 pContext, (DSL_uint8_t *)&sAck);

   if( nErrCode < 0 )
   {
      return nErrCode;
   }

   /* Get previous state of Data Path Failures */
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nDataPathFailuresFe[0], nPreDPF_FE);
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nDataPathFailuresNe[0], nPreDPF_NE);

#ifdef INCLUDE_DSL_G997_ALARM
   /* Process Near-End bits */
   DSL_FAILUREBIT_SET(OOS_NE_BC0, nPreDPF_NE, DSL_G997_DATAPATHFAILURE_OOS, nActDPF_NE, nActDPFM_NE);
   DSL_FAILUREBIT_SET(NCD_NE_BC0, nPreDPF_NE, DSL_G997_DATAPATHFAILURE_NCD, nActDPF_NE, nActDPFM_NE);
   DSL_FAILUREBIT_SET(LCD_NE_BC0, nPreDPF_NE, DSL_G997_DATAPATHFAILURE_LCD, nActDPF_NE, nActDPFM_NE);
   /* Process Far-End bits */
   DSL_FAILUREBIT_SET(NCD_FE_BC0, nPreDPF_FE, DSL_G997_DATAPATHFAILURE_NCD, nActDPF_FE, nActDPFM_FE);
   DSL_FAILUREBIT_SET(LCD_FE_BC0, nPreDPF_FE, DSL_G997_DATAPATHFAILURE_LCD, nActDPF_FE, nActDPFM_FE);
#else
   /* Process Near-End bits */
   DSL_FAILUREBIT_SET(OOS_NE_BC0, DSL_G997_DATAPATHFAILURE_OOS, nActDPF_NE);
   DSL_FAILUREBIT_SET(NCD_NE_BC0, DSL_G997_DATAPATHFAILURE_NCD, nActDPF_NE);
   DSL_FAILUREBIT_SET(LCD_NE_BC0, DSL_G997_DATAPATHFAILURE_LCD, nActDPF_NE);
   /* Process Far-End bits */
   DSL_FAILUREBIT_SET(NCD_FE_BC0, DSL_G997_DATAPATHFAILURE_NCD, nActDPF_FE);
   DSL_FAILUREBIT_SET(LCD_FE_BC0, DSL_G997_DATAPATHFAILURE_LCD, nActDPF_FE);
#endif /* INCLUDE_DSL_G997_ALARM*/

   /* Write actual settings for Data Path Failures in DSL context. Take care
      to set only failures bits that should be handled by this function
      (relevant bit mask).  */
   nActDPF_FE &= nRelevantBitMask;
   nActDPF_NE &= nRelevantBitMask;
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, nDataPathFailuresFe[0], nActDPF_FE);
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, nDataPathFailuresNe[0], nActDPF_NE);

   /* TODO: Add Failure counters handling*/

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: No Cell Delineation failure NE (NCD) = %d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
      (nActDPF_NE & DSL_G997_DATAPATHFAILURE_NCD) ? 1 : 0 ));

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: Loss of Cell Delineation failure NE (LCD) = %d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
      (nActDPF_NE & DSL_G997_DATAPATHFAILURE_LCD) ? 1 : 0 ));

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: Out of Sync failure NE (OOS) = %d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
      (nActDPF_NE & DSL_G997_DATAPATHFAILURE_OOS) ? 1 : 0 ));

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: No Cell Delineation failure FE (NCD) = %d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
      (nActDPF_FE & DSL_G997_DATAPATHFAILURE_NCD) ? 1 : 0 ));

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: Loss of Cell Delineation failure FE (LCD) = %d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
      (nActDPF_FE & DSL_G997_DATAPATHFAILURE_LCD) ? 1 : 0 ));

   /* Get line state*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineState, nLineState);

#ifdef INCLUDE_DSL_G997_ALARM
   /* Get (user) mask configuration for line failure event handling */
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nDataPathFailuresNeAlarmMask, nCfgDPFM_NE);
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nDataPathFailuresFeAlarmMask, nCfgDPFM_FE);

   nActDPFM_NE &= nCfgDPFM_NE;
   nActDPFM_FE &= nCfgDPFM_FE;

   /* Signal event only in case of transition from 0 to 1*/
   if (nActDPF_NE & nActDPFM_NE)
   {
      nErrCode = DSL_DRV_EventGenerate(
                     pContext, 0, DSL_ACCESSDIR_NA, DSL_NEAR_END,
                     DSL_EVENT_I_DATA_PATH_FAILURES,
                     (DSL_EventData_Union_t*)&nActDPF_NE,
                     sizeof(DSL_G997_DataPathFailuresData_t));
      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Event(%d) generate failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), DSL_EVENT_I_DATA_PATH_FAILURES));
         return nErrCode;
      }
   }

   /* Signal event only in case of transition from 0 to 1*/
   if (nActDPF_FE & nActDPFM_FE)
   {
      nErrCode = DSL_DRV_EventGenerate(
                     pContext, 0, DSL_ACCESSDIR_NA, DSL_FAR_END,
                     DSL_EVENT_I_DATA_PATH_FAILURES,
                     (DSL_EventData_Union_t*)&nActDPF_FE,
                     sizeof(DSL_G997_DataPathFailuresData_t));
      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Event(%d) generate failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), DSL_EVENT_I_DATA_PATH_FAILURES));

         return nErrCode;
      }
   }
#endif /* INCLUDE_DSL_G997_ALARM*/

   return nErrCode;
}

#ifdef INCLUDE_DSL_FRAMING_PARAMETERS
DSL_Error_t DSL_DRV_VXX_FramingParametersVdsl2StatusGet(
   DSL_Context_t *pContext,
   const DSL_uint8_t nChannel,
   const DSL_AccessDir_t nDir,
   DSL_FramingParameterStatusData_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   union
   {
      ACK_FrameDataExt2US_Get_t US;
      ACK_FrameDataExt2DS_Get_t DS;
   } sAck;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_VXX_FramingParametersVdsl2StatusGet,"
      " (nDirection=%d, nChannel=%d)"DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
      nDir, nChannel));

   /* Get VDSL2 framing parameters*/
   nErrCode = DSL_DRV_VRX_SendMsgFrameDataExt2Get(pContext, nDir, (DSL_uint8_t*)&(sAck.DS));
   if( nErrCode < 0 )
      return nErrCode;

   pData->nTp    = nDir == DSL_DOWNSTREAM ? sAck.DS.Tp_LP0ds : sAck.US.Tp_LP0us;
   pData->nBP    = nDir == DSL_DOWNSTREAM ? sAck.DS.B0_LP0ds : sAck.US.B0_LP0us;
   pData->nMp    = nDir == DSL_DOWNSTREAM ? sAck.DS.Mp_LP0ds : sAck.US.Mp_LP0us;
   pData->nMSGC  = nDir == DSL_DOWNSTREAM ? sAck.DS.MSGc_LP0ds : sAck.US.MSGc_LP0us;
   pData->nMSGLP = 0x0; /* not supported*/
   pData->nSEQ   = nDir == DSL_DOWNSTREAM ? sAck.DS.SEQp_LP0ds : sAck.US.SEQp_LP0us;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VXX_FramingParametersVdsl2StatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return (nErrCode);
}
#endif /* INCLUDE_DSL_FRAMING_PARAMETERS*/

#ifdef INCLUDE_DSL_FRAMING_PARAMETERS
DSL_Error_t DSL_DRV_VXX_FramingParametersAdslStatusGet(
   DSL_Context_t *pContext,
   const DSL_uint8_t nChannel,
   const DSL_AccessDir_t nDir,
   DSL_FramingParameterStatusData_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   ACK_ADSL_FrameDataDS_LP0Get_t sAck;
   DSL_uint8_t XTSE[DSL_G997_NUM_XTSE_OCTETS] = {0};

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VXX_FramingParametersAdslStatusGet,"
      " (nDirection=%d, nChannel=%d)"DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
      nDir, nChannel));

   /* Get ADSL framing parameters*/
   nErrCode = DSL_DRV_VRX_SendMsgAdslFrameDataGet(pContext, nDir, (DSL_uint8_t*)&sAck);
   if(nErrCode < 0)
      return nErrCode;

#if !defined(INCLUDE_DSL_CPE_API_VRX_FULL_MSG)
   if (nErrCode == DSL_WRN_NOT_SUPPORTED_IN_CURRENT_ADSL_MODE_OR_ANNEX)
   {
      return DSL_ERR_NOT_SUPPORTED_BY_FIRMWARE;
   }
#endif

   pData->nTp    = sAck.Tp;
   pData->nBP    = sAck.Kp ? sAck.Kp - 1 : 0;
   pData->nMp    = sAck.Mp;
   pData->nMSGLP = 0x0; /* not supported*/


   /*Get current xTSE octets*/
   DSL_CTX_READ(pContext, nErrCode, xtseCurr, XTSE);

   if ( (XTSE[1-1] & (XTSE_1_05_B_1_NO | XTSE_1_03_A_1_NO | XTSE_1_01_A_T1_413)) ||
        (XTSE[2-1] & (XTSE_2_01_A_2_NO)))
   {
      /* Not supported*/
      pData->nMSGC  = 0x0;
      pData->nSEQ   = 0x0;

      nErrCode = DSL_WRN_INCOMPLETE_RETURN_VALUES;
   }
   else
   {
      pData->nMSGC  = sAck.MSGc;
      pData->nSEQ   = sAck.SEQp;
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VXX_FramingParametersAdslStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return (nErrCode);

}
#endif /* INCLUDE_DSL_FRAMING_PARAMETERS*/

#ifdef INCLUDE_DSL_DELT
DSL_Error_t DSL_DRV_VXX_DiagnosticDeltDataGet(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   ACK_TestParamsAuxDS_Get_t  sAck;
   DSL_uint16_t i = 0;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VNX_AdslDeltAllDataGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   if( pContext->DELT == DSL_NULL )
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - No memory allocated to store DELT data!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERROR;
   }

   for(i = 0; i <= (DSL_uint16_t)DSL_DOWNSTREAM; i++)
   {
      /* Get Test Params Aux data, not available for VDSL in DIAG_COMPLETE state*/
         nErrCode = DSL_DRV_VRX_SendMsgTestParamsAuxGet(
                       pContext, (DSL_AccessDir_t)i, (DSL_uint8_t*)&sAck);

      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - DELT Aux data get failed, nDirection=%d!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), i));

         return DSL_ERROR;
      }

      /* Update Aux Parameters*/
      if (((DSL_AccessDir_t)i) == DSL_DOWNSTREAM)
      {
         pContext->DELT->hlinScaleDataDs.nDeltHlinScale = sAck.HLINSC;
         pContext->DELT->hlogDataDs.nGroupSize          = (DSL_uint8_t)sAck.HLOGG;
         pContext->DELT->hlogDataDs.nMeasurementTime    = sAck.HLOGMT;
         pContext->DELT->qlnDataDs.nGroupSize           = (DSL_uint8_t)sAck.QLNG;
         pContext->DELT->qlnDataDs.nMeasurementTime     = sAck.QLNMT;
         pContext->DELT->snrDataDs.nGroupSize           = (DSL_uint8_t)sAck.SNRG;
         pContext->DELT->snrDataDs.nMeasurementTime     = sAck.SNRMT;
         pContext->DELT->hlinDataDs.nGroupSize          = (DSL_uint8_t)sAck.HLING;
         /* Not supported yet*/
         pContext->DELT->hlinDataDs.nMeasurementTime    = 0x0;
      }
      else
      {
         pContext->DELT->hlinScaleDataUs.nDeltHlinScale = sAck.HLINSC;
         pContext->DELT->hlogDataUs.nGroupSize          = (DSL_uint8_t)sAck.HLOGG;
         pContext->DELT->hlogDataUs.nMeasurementTime    = sAck.HLOGMT;
         pContext->DELT->qlnDataUs.nGroupSize           = (DSL_uint8_t)sAck.QLNG;
         pContext->DELT->qlnDataUs.nMeasurementTime     = sAck.QLNMT;
         pContext->DELT->snrDataUs.nGroupSize           = (DSL_uint8_t)sAck.SNRG;
         pContext->DELT->snrDataUs.nMeasurementTime     = sAck.SNRMT;
         pContext->DELT->hlinDataUs.nGroupSize          = (DSL_uint8_t)sAck.HLING;
         /* Not supported yet*/
         pContext->DELT->hlinDataUs.nMeasurementTime    = 0x0;
      }

      /* Get Hlog values, not available for VDSL in DIAG_COMPLETE state*/
      nErrCode = DSL_DRV_VRX_SendMsgHlogGet(
              pContext,
              (DSL_AccessDir_t)i,
              ((DSL_AccessDir_t)i) == DSL_DOWNSTREAM ?
              (DSL_G997_NSCData16_t*)&(pContext->DELT->hlogDataDs.deltHlog) :
              (DSL_G997_NSCData16_t*)&(pContext->DELT->hlogDataUs.deltHlog));

      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - DELT Hlog data get failed, nDirection=%d!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), i));

         break;
      }

      /* Get Line Status data*/
      nErrCode = DSL_DRV_VRX_LineStatusGet(
                    pContext,
                    (DSL_AccessDir_t)i,
                    &(pContext->DELT->lineStatus[i]));

      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - DELT Line Status data get failed, nDirection=%d!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), i));

         break;
      }

      /* Get Hlin values*/
      nErrCode = DSL_DRV_VRX_SendMsgHlinGet(
                    pContext,
                    (DSL_AccessDir_t)i,
                   ((DSL_AccessDir_t)i) == DSL_DOWNSTREAM ?
                    (DSL_G997_NSCComplexData_t*)&(pContext->DELT->hlinDataDs.deltHlin) :
                    (DSL_G997_NSCComplexData_t*)&(pContext->DELT->hlinDataUs.deltHlin));

      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - DELT Hlin data get failed, nDirection=%d!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), i));

         break;
      }

      /* Get SNR values*/
      if (DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_ADSL) &&
                               (DSL_AccessDir_t)i == DSL_DOWNSTREAM)
      {

         nErrCode = DSL_DRV_VRX_SnrAllocationTableNeGet(
                       pContext, (DSL_G997_NSCData8_t*)&(pContext->DELT->snrDataDs.deltSnr));

         if( nErrCode != DSL_SUCCESS )
         {
            DSL_DEBUG(DSL_DBG_ERR,
               (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - DELT SNR data get failed, nDirection=%d!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext), i));

            break;
         }
      }
      else
      {
         nErrCode = DSL_DRV_VRX_SendMsgSnrGet(
                       pContext,
                       (DSL_AccessDir_t)i,
                     ((DSL_AccessDir_t)i) == DSL_DOWNSTREAM ?
                     (DSL_G997_NSCData8_t*)&(pContext->DELT->snrDataDs.deltSnr) :
                       (DSL_G997_NSCData8_t*)&(pContext->DELT->snrDataUs.deltSnr));

         if( nErrCode != DSL_SUCCESS )
         {
            DSL_DEBUG(DSL_DBG_ERR,
               (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - DELT SNR data get failed, nDirection=%d!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext), i));

            break;
         }
      }

      /* Get QLN values*/
      nErrCode = DSL_DRV_VRX_SendMsgQlnGet(
                    pContext,
                    (DSL_AccessDir_t)i,
                    ((DSL_AccessDir_t)i) == DSL_DOWNSTREAM ?
                    (DSL_G997_NSCData8_t*)&(pContext->DELT->qlnDataDs.deltQln) :
                    (DSL_G997_NSCData8_t*)&(pContext->DELT->qlnDataUs.deltQln));

      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - DELT QLN data get failed, nDirection=%d!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), i));

         break;
      }
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VNX_AdslDeltAllDataGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return(nErrCode);
}
#endif /* INCLUDE_DSL_DELT*/

DSL_Error_t DSL_DRV_VXX_LineFailuresStatusUpdate(
   DSL_Context_t *pContext)
{
   DSL_Error_t errCode = DSL_SUCCESS;

   /* Update Line failures information for Near-End*/
   errCode = DSL_DRV_VRX_LineFailuresNeStatusGet(pContext);
   if(errCode != DSL_SUCCESS)
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - Line Failures NE update failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

     return errCode;
   }

   /* Update Line failures information for Far-End*/
   errCode = DSL_DRV_VRX_LineFailuresFeStatusGet(pContext);
   if(errCode != DSL_SUCCESS)
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - Line Failures FE update failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return errCode;
   }

   return errCode;
}

/*
   This function returns the support state of a bandplan/profile
   combination.

   \param pContext Pointer to dsl cpe library context structure, [I]
   \param pData    Defines the bandplan from
                   \ref DSL_BandPlanConfigData_t, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_TRUE (1) if supported
   - DSL_FALSE (0) if not supported
*/
DSL_boolean_t DSL_DRV_VXX_BandPlanSupportedCheck(
   DSL_Context_t *pContext,
   const DSL_BandPlanConfigData_t *pData)
{
   typedef struct
   {
      DSL_BandPlanType_t nIdx;
      DSL_uint8_t        bSupported[DSL_PROFILE_LAST];
   } DSL_BP_SupportTableEntry_t;

   static const DSL_BP_SupportTableEntry_t sBpSupport[] =
   {
      /* only new IDs are supported from version 3.2 onwards */
      /* Annex A     */                                                           /*  8a,8b,8c,8d,12a,12b,17a,30a,17b */
      /*                      50  */  {DSL_ITU_VDSL2_ANNEXA_M1_EU32                  , {1, 1, 1, 1, 1, 1, 1, 1, 1,}},
      /*                      51  */  {DSL_ITU_VDSL2_ANNEXA_M2_EU36                  , {0, 0, 0, 0, 0, 0, 0, 0, 0,}},
      /*                      52  */  {DSL_ITU_VDSL2_ANNEXA_M3_EU40                  , {0, 0, 0, 0, 0, 0, 0, 0, 0,}},
      /*                      53  */  {DSL_ITU_VDSL2_ANNEXA_M4_EU44                  , {0, 0, 0, 0, 0, 0, 0, 0, 0,}},
      /*                      54  */  {DSL_ITU_VDSL2_ANNEXA_M5_EU48                  , {0, 0, 0, 0, 0, 0, 0, 0, 0,}},
      /*                      55  */  {DSL_ITU_VDSL2_ANNEXA_M6_EU52                  , {0, 0, 0, 0, 0, 0, 0, 0, 0,}},
      /*                      56  */  {DSL_ITU_VDSL2_ANNEXA_M7_EU56                  , {0, 0, 0, 0, 0, 0, 0, 0, 0,}},
      /*                      57  */  {DSL_ITU_VDSL2_ANNEXA_M8_EU60                  , {0, 0, 0, 0, 0, 0, 0, 0, 0,}},
      /*                      58  */  {DSL_ITU_VDSL2_ANNEXA_M9_EU64                  , {1, 1, 1, 1, 1, 1, 1, 0, 1,}},
      /*                      59  */  {DSL_ITU_VDSL2_ANNEXA_EU128                    , {1, 1, 1, 1, 1, 1, 1, 1, 1,}},
      /*                      70  */  {DSL_ITU_VDSL2_ANNEXA_M1_ADLU32                , {1, 1, 1, 1, 1, 0, 0, 1, 1,}},
      /*                      71  */  {DSL_ITU_VDSL2_ANNEXA_M2_ADLU36                , {0, 0, 0, 0, 0, 0, 0, 0, 0,}},
      /*                      72  */  {DSL_ITU_VDSL2_ANNEXA_M3_ADLU40                , {0, 0, 0, 0, 0, 0, 0, 0, 0,}},
      /*                      73  */  {DSL_ITU_VDSL2_ANNEXA_M4_ADLU44                , {0, 0, 0, 0, 0, 0, 0, 0, 0,}},
      /*                      74  */  {DSL_ITU_VDSL2_ANNEXA_M5_ADLU48                , {0, 0, 0, 0, 0, 0, 0, 0, 0,}},
      /*                      75  */  {DSL_ITU_VDSL2_ANNEXA_M6_ADLU52                , {0, 0, 0, 0, 0, 0, 0, 0, 0,}},
      /*                      76  */  {DSL_ITU_VDSL2_ANNEXA_M7_ADLU56                , {0, 0, 0, 0, 0, 0, 0, 0, 0,}},
      /*                      77  */  {DSL_ITU_VDSL2_ANNEXA_M8_ADLU60                , {0, 0, 0, 0, 0, 0, 0, 0, 0,}},
      /*                      78  */  {DSL_ITU_VDSL2_ANNEXA_M9_ADLU64                , {1, 1, 1, 1, 1, 0, 0, 1, 1,}},
      /*                      79  */  {DSL_ITU_VDSL2_ANNEXA_ADLU128                  , {1, 1, 1, 1, 1, 0, 0, 1, 1,}},
      /* ANNEX B     */
      /*                      100 */  {DSL_ITU_VDSL2_ANNEXB_B7_1_997_M1C_A_7         , {0, 0, 1, 0, 0, 0, 0, 0, 0,}},
      /*                      101 */  {DSL_ITU_VDSL2_ANNEXB_B7_2_997_M1X_M_8         , {1, 1, 1, 1, 0, 0, 0, 0, 0,}},
      /*                      102 */  {DSL_ITU_VDSL2_ANNEXB_B7_3_997_M1X_M           , {0, 0, 0, 0, 1, 1, 0, 0, 0,}},
      /*                      103 */  {DSL_ITU_VDSL2_ANNEXB_B7_4_997_M2X_M_8         , {1, 1, 1, 1, 0, 0, 0, 0, 0,}},
      /*                      104 */  {DSL_ITU_VDSL2_ANNEXB_B7_5_997_M2X_A           , {1, 1, 1, 1, 1, 1, 0, 0, 0,}},
      /*                      105 */  {DSL_ITU_VDSL2_ANNEXB_B7_6_997_M2X_M           , {0, 0, 0, 0, 1, 1, 0, 0, 0,}},
      /*                      106 */  {DSL_ITU_VDSL2_ANNEXB_B7_7_HPE17_M1_NUS0       , {0, 0, 0, 0, 0, 0, 1, 0, 0,}},
      /*                      107 */  {DSL_ITU_VDSL2_ANNEXB_B7_8_HPE30_M1_NUS0       , {0, 0, 0, 0, 0, 0, 0, 1, 0,}},
      /*                      108 */  {DSL_ITU_VDSL2_ANNEXB_B7_9_997E17_M2X_NUS0     , {0, 0, 0, 0, 0, 0, 1, 0, 0,}},
      /*                      109 */  {DSL_ITU_VDSL2_ANNEXB_B7_10_997E30_M2X_NUS0    , {0, 0, 0, 0, 0, 0, 0, 1, 0,}},
      /*                      120 */  {DSL_ITU_VDSL2_ANNEXB_B8_1_998_M1X_A           , {1, 1, 1, 1, 1, 0, 0, 0, 0,}},
      /*                      121 */  {DSL_ITU_VDSL2_ANNEXB_B8_2_998_M1X_B           , {1, 1, 1, 1, 1, 1, 0, 0, 0,}},
      /*                      122 */  {DSL_ITU_VDSL2_ANNEXB_B8_3_998_M1X_NUS0        , {1, 1, 1, 1, 0, 1, 0, 0, 0,}},
      /*                      123 */  {DSL_ITU_VDSL2_ANNEXB_B8_4_998_M2X_A           , {1, 1, 1, 1, 1, 0, 0, 0, 0,}},
      /*                      124 */  {DSL_ITU_VDSL2_ANNEXB_B8_5_998_M2X_M           , {1, 1, 1, 1, 1, 1, 0, 0, 0,}},
      /*                      125 */  {DSL_ITU_VDSL2_ANNEXB_B8_6_998_M2X_B           , {1, 1, 1, 1, 1, 0, 0, 0, 0,}},
      /*                      126 */  {DSL_ITU_VDSL2_ANNEXB_B8_7_998_M2X_NUS0        , {1, 1, 1, 1, 0, 1, 0, 0, 0,}},
      /*                      127 */  {DSL_ITU_VDSL2_ANNEXB_B8_8_998E17_M2X_NUS0     , {0, 0, 0, 0, 0, 0, 1, 0, 0,}},
      /*                      128 */  {DSL_ITU_VDSL2_ANNEXB_B8_9_998E17_M2X_NUS0_M   , {0, 0, 0, 0, 0, 0, 1, 0, 0,}},
      /*                      129 */  {DSL_ITU_VDSL2_ANNEXB_B8_10_998ADE17_M2X_NUS0_M, {0, 0, 0, 0, 0, 0, 1, 0, 0,}},
      /*                      130 */  {DSL_ITU_VDSL2_ANNEXB_B8_11_998ADE17_M2X_A     , {0, 0, 0, 0, 0, 0, 1, 0, 1,}},
      /*                      131 */  {DSL_ITU_VDSL2_ANNEXB_B8_12_998ADE17_M2X_B     , {0, 0, 0, 0, 0, 0, 0, 0, 1,}},
      /*                      132 */  {DSL_ITU_VDSL2_ANNEXB_B8_13_998E30_M2X_NUS0    , {0, 0, 0, 0, 0, 0, 0, 1, 0,}},
      /*                      133 */  {DSL_ITU_VDSL2_ANNEXB_B8_14_998E30_M2X_NUS0_M  , {0, 0, 0, 0, 0, 0, 0, 1, 0,}},
      /*                      134 */  {DSL_ITU_VDSL2_ANNEXB_B8_15_998ADE30_M2X_NUS0_M, {0, 0, 0, 0, 0, 0, 0, 1, 0,}},
      /*                      135 */  {DSL_ITU_VDSL2_ANNEXB_B8_16_998ADE30_M2X_NUS0_A, {0, 0, 0, 0, 0, 0, 1, 1, 0,}},
      /*                      140 */  {DSL_ITU_VDSL2_ANNEXB_998ADE17_M2X_M           , {0, 0, 0, 0, 0, 0, 0, 0, 1,}},
      /*                      141 */  {DSL_ITU_VDSL2_ANNEXB_998E17_M2X_A             , {0, 0, 0, 0, 0, 0, 0, 0, 1,}},
      /* ANNEX C      */
      /*                      160 */  {DSL_ITU_VDSL2_ANNEXC_FTTCAB_A                 , {0, 0, 1, 0, 1, 1, 1, 0, 1,}},
      /*                      161 */  {DSL_ITU_VDSL2_ANNEXC_FTTCAB_M                 , {0, 0, 1, 0, 1, 1, 1, 0, 1,}},
      /*                      162 */  {DSL_ITU_VDSL2_ANNEXC_FTTEX_A                  , {1, 1, 1, 1, 1, 1, 0, 0, 0,}},
      /*                      163 */  {DSL_ITU_VDSL2_ANNEXC_FTTEX_M                  , {1, 1, 1, 1, 1, 1, 0, 0, 0,}},
      /*                      164 */  {DSL_ITU_VDSL2_ANNEXC_O_ADSL                   , {0, 0, 1, 0, 0, 1, 1, 1, 0,}},
      /*                      165 */  {DSL_ITU_VDSL2_ANNEXC_O_TCMISDN                , {0, 0, 1, 0, 0, 1, 1, 1, 0,}},
      /* VDSL1       */
      /*                      180 */  {DSL_ANSI_FTTCAB_M1                            , {0, 0, 1, 0, 1, 1, 0, 0, 0,}},
      /*                      181 */  {DSL_ANSI_FTTCAB_M2                            , {0, 0, 1, 1, 1, 1, 0, 0, 0,}},
      /*                      182 */  {DSL_ANSI_FTTEX_M1                             , {1, 1, 1, 1, 1, 1, 0, 0, 0,}},
      /*                      183 */  {DSL_ANSI_FTTEX_M2                             , {1, 1, 1, 1, 1, 1, 0, 0, 0,}},
      /*                      184 */  {DSL_ETSI_FTTCAB_PCAB_M1                       , {0, 0, 1, 0, 1, 1, 0, 0, 0,}},
      /*                      185 */  {DSL_ETSI_FTTCAB_PCAB_M2                       , {0, 0, 1, 0, 1, 1, 0, 0, 0,}},
      /*                      186 */  {DSL_ETSI_FTTEX_P1_M1_O_ISDN                   , {1, 1, 1, 1, 1, 1, 0, 0, 0,}},
      /*                      187 */  {DSL_ETSI_FTTEX_P1_M2_O_ISDN                   , {1, 1, 1, 1, 1, 1, 0, 0, 0,}},
      /*                      188 */  {DSL_ETSI_FTTEX_P2_M1_O_POTS                   , {1, 1, 1, 1, 1, 1, 0, 0, 0,}},
      /*                      189 */  {DSL_ETSI_FTTEX_P2_M2_O_POTS                   , {1, 1, 1, 1, 1, 1, 0, 0, 0,}},
      /*                      190 */  {DSL_ITU_VDSL1_ANNEXE_E2_PCABA_M1              , {0, 0, 1, 0, 1, 1, 0, 0, 0,}},
      /* Others      */
      /*                      200 */  {DSL_HANARO_FTTCAB                             , {0, 0, 1, 0, 1, 1, 1, 1, 1,}},
      /*                      201 */  {DSL_ANFP_CAL0                                 , {0, 0, 1, 0, 0, 0, 0, 0, 0,}},

   };
   DSL_uint32_t i = 0;

   if  (pData == DSL_NULL)
      return DSL_FALSE;

   if ((pData->nBandPlan >= DSL_BANDPLAN_LAST) || (pData->nProfile >= DSL_PROFILE_LAST))
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - DSL_VNX_BandPlanSupportedCheck: (nBandplan=%d, "
         "nProfile=%d) out of range!" DSL_DRV_CRLF , DSL_DEV_NUM(pContext),
         pData->nBandPlan, pData->nProfile));

      return DSL_FALSE;
   }

   /* to speed up the search, break when search index gets greater than the
      bandplan number to search for,
     because the entries are sorted and unique so
     position <= BandplanId */
   for (i = 0;
      (i < DSL_ARRAY_LENGTH(sBpSupport)) &&
      (i <= (DSL_uint32_t)pData->nBandPlan);
      i++)
   {
      if (sBpSupport[i].nIdx == pData->nBandPlan)
         return (DSL_boolean_t)sBpSupport[i].bSupported[pData->nProfile];
   }

   /* Bandplan number not found */
   return DSL_FALSE;
}

DSL_Error_t DSL_DRV_VXX_FailReasonGet(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_G997_LineInit_t lineInitStatus = LINIT_UNKNOWN;
   DSL_G997_LineInitSubStatus_t lineInitSubStatus = LINIT_SUB_NONE;
   ACK_ModemFSM_FailReasonGet_t sAck;
#ifdef INCLUDE_DEVICE_EXCEPTION_CODES
   DSL_DBG_LastExceptionCodesData_t LastExceptionCodes;
#endif /* INCLUDE_DEVICE_EXCEPTION_CODES*/

   nErrCode = DSL_DRV_VXX_SendMsgModemFSMFailReasonGet(
                  pContext,(DSL_uint8_t*)&sAck);

   if( nErrCode == DSL_SUCCESS )
   {
#ifdef INCLUDE_DEVICE_EXCEPTION_CODES
      /* Read previous Last Exception Codes from the CPE API Context*/
      DSL_CTX_READ(pContext, nErrCode, LastExceptionCodes, LastExceptionCodes);

      /* Update Previous Exception Code value*/
      LastExceptionCodes.nError2 = LastExceptionCodes.nError1;

      /* Update Current Exception Code value*/
      LastExceptionCodes.nError1 = (DSL_uint32_t)sAck.FW_FailCode;

      DSL_CTX_WRITE(pContext, nErrCode, LastExceptionCodes, LastExceptionCodes);
#endif /* INCLUDE_DEVICE_EXCEPTION_CODES*/
      switch(sAck.ErrorCode)
      {
      case ALM_ModemFSM_FailReasonGet_E_CONFIG:
         lineInitStatus = LINIT_CONFIG_ERROR;
         break;
      case ALM_ModemFSM_FailReasonGet_E_NOTFEASIBLE:
         lineInitStatus = LINIT_CONFIG_NOT_FEASIBLE;
         break;
      case ALM_ModemFSM_FailReasonGet_E_COMM:
      case ALM_ModemFSM_FailReasonGet_E_COMMCRC:
         lineInitStatus = LINIT_COMMUNICATION_PROBLEM;
         break;
      case ALM_ModemFSM_FailReasonGet_E_NOPEER:
          lineInitStatus = LINIT_NO_PEER_XTU;
          break;
      case ALM_ModemFSM_FailReasonGet_E_UNKNOWN:
      case ALM_ModemFSM_FailReasonGet_E_CHIPSET:
      case ALM_ModemFSM_FailReasonGet_E_RESERVED:
         lineInitStatus = LINIT_UNKNOWN;
         break;
      default:
         lineInitStatus = LINIT_UNKNOWN;
         break;
      }
   }

   switch( sAck.SubErrorCode )
   {
   case ALM_ModemFSM_FailReasonGet_S_MODE:
      lineInitSubStatus = LINIT_SUB_NO_COMMON_MODE;
      break;
   case ALM_ModemFSM_FailReasonGet_S_XDSL_MODE:
      lineInitSubStatus = LINIT_SUB_OPP_MODE;
      break;
   case ALM_ModemFSM_FailReasonGet_S_FW_RETRY:
      lineInitSubStatus = LINIT_SUB_FW_RETRY;
      break;
   case ALM_ModemFSM_FailReasonGet_S_FW_HYBRID:
      lineInitSubStatus = LINIT_SUB_FW_HYBRID;
      break;
   case ALM_ModemFSM_FailReasonGet_S_OK:
      lineInitSubStatus = LINIT_SUB_NONE;

   default:
      lineInitSubStatus = LINIT_SUB_UNKNOWN;
      break;
   }

   DSL_DRV_HandleLinitValue(pContext, lineInitStatus, lineInitSubStatus);

   return (nErrCode);
}

/*
   Gets out of the 32bit version number the detailled information
   of the Firmware
   release and separates it in its logical parts.

   \param pContext   Pointer to dsl cpe library context structure, [I]
   \param nVersion   Fw Version Number to decode, [I]
   \param pData      pointer to detailled version information, [O]

   \return
   None
*/
static DSL_Error_t DSL_DRV_VRX_FirmwareVersionDecode(
   DSL_Context_t *pContext,
   const DSL_uint32_t nVersion,
   DSL_VNX_FwVersion_t *pData)
{
   pData->nPlatform        = (nVersion & 0xFF000000) >> 24;
   pData->nFeatureSet      = (nVersion & 0x00FF0000) >> 16;
   pData->nMajor           = (nVersion & 0x0000F000) >> 12;
   pData->nMinor           = (nVersion & 0x00000F00) >> 8;
   pData->nReleaseStatus   = (nVersion & 0x000000C0) >> 6;
   pData->nApplication     = (nVersion & 0x0000003F);

   return DSL_SUCCESS;
}

static DSL_Error_t DSL_DRV_VRX_HardwareVersionDecode(
   DSL_Context_t *pContext,
   const DSL_uint32_t nVersion,
   DSL_char_t *pData)
{
   DSL_int32_t i;
   VNX_CodeString_t *pCS;

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: Chip set version: 0x%04X" DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext), nVersion));

   pCS = ChipString;
   for (i = 0; pCS[i].string; i++)
   {
      if (pCS[i].code == nVersion)
      {
         strncpy(pData, pCS[i].string, MAX_INFO_STRING_LEN);
         break;
      }
   }

   if (pCS[i].string == DSL_NULL)
      strncpy(pData, "Unknown", MAX_INFO_STRING_LEN);

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: Chip set string : %s" DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext), pData));

   DSL_DEBUG( DSL_DBG_MSG, (pContext, SYS_DBG_MSG"DSL[%02d]: Chip set type: %s"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), DSL_CHIPSET_TYPE_STRING));

   return DSL_SUCCESS;
}

/*
   This functions makes the driver low level initialization.
   Therefore the current driver state will be check and the
   if necessary the low level config values will be set.

   \param pContext - Pointer to DSL CPE library context structure, [I]
   \param pLineCfg - Pointer to Line configuration parameters, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
static DSL_Error_t DSL_DRV_VRX_InitDeviceDrv(
   DSL_Context_t *pContext,
   DSL_DeviceLowLevelConfig_t *pLowCfg)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_DEV_Handle_t dev;
   IOCTL_MEI_reqCfg_t  fioVrxDrvCfg;
   IOCTL_MEI_devInit_t fioVrxDrvInit;

   DSL_CHECK_POINTER(pContext, pLowCfg);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_POINTER(pContext, pContext->pDevCtx);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_POINTER(pContext, pContext->pDevCtx->lowHandle);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_POINTER(pContext, pContext->pDevCtx->nfc_lowHandle);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_InitDeviceDrv()"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Get device handle*/
   dev = DSL_DEVICE_LOWHANDLE(pContext);

   memset(&fioVrxDrvCfg, 0x00, sizeof(IOCTL_MEI_reqCfg_t));
   /* Request the configuration of the MEI CPE Driver*/
   DSL_DRV_VRX_InternalRequestConfig( (MEI_DYN_CNTRL_T*)dev, &fioVrxDrvCfg );

   /* Check current driver state*/
   if (fioVrxDrvCfg.currDrvState != 0)
   {
      DSL_DEBUG( DSL_DBG_MSG, (pContext,
         SYS_DBG_MSG"DSL[%02d]: MEI CPE init, already initialized"DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));
      DSL_DEBUG( DSL_DBG_MSG, (pContext,
         SYS_DBG_MSG"DSL[%02d]: MEI CPE driver configuration:"DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));
      DSL_DEBUG( DSL_DBG_MSG, (pContext,
         SYS_DBG_MSG"  devNum            : %d"DSL_DRV_CRLF,fioVrxDrvCfg.devNum));
      DSL_DEBUG( DSL_DBG_MSG, (pContext,
         SYS_DBG_MSG"  phyBaseAddr       : %x"DSL_DRV_CRLF,fioVrxDrvCfg.phyBaseAddr));
      DSL_DEBUG( DSL_DBG_MSG, (pContext,
         SYS_DBG_MSG"  usedIRQ           : %d"DSL_DRV_CRLF,fioVrxDrvCfg.usedIRQ));
      DSL_DEBUG( DSL_DBG_MSG, (pContext,
         SYS_DBG_MSG"  bootMode          : %d"DSL_DRV_CRLF,fioVrxDrvCfg.bootMode));
      DSL_DEBUG( DSL_DBG_MSG, (pContext,
         SYS_DBG_MSG"  currDrvState      : %d"DSL_DRV_CRLF,fioVrxDrvCfg.currDrvState));
      DSL_DEBUG( DSL_DBG_MSG, (pContext,
         SYS_DBG_MSG"  currModemFsmState : %d"DSL_DRV_CRLF,fioVrxDrvCfg.currModemFsmState));
   }
   else
   {
      /*...device not initialized, do the initialization */
      if ( (pLowCfg->nBaseAddr != 0xFFFFFFFF) &&
           (pLowCfg->nBaseAddr != 0x0) &&
           (pLowCfg->nIrqNum != -1) )
      {
         fioVrxDrvInit.meiBaseAddr = pLowCfg->nBaseAddr;
         fioVrxDrvInit.usedIRQ     = (unsigned int)((int)(pLowCfg->nIrqNum));

         /* Do the basic init of the VRx*/
         if( DSL_DRV_VRX_InternalInitDevice( dev, &fioVrxDrvInit) < 0 )
         {
            DSL_DEBUG( DSL_DBG_ERR, (pContext,
               SYS_DBG_ERR"DSL[%02d]: VRx init failed, error %d" DSL_DRV_CRLF,
               DSL_DEV_NUM(pContext), fioVrxDrvInit.ictl.retCode));

            return DSL_ERROR;
         }
      }
      else
      {
         DSL_DEBUG( DSL_DBG_ERR, (pContext,
            SYS_DBG_ERR"DSL[%02d]: VRx init failed, no valid data"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return DSL_ERROR;
      }
   }

   /* Get handle to the NFC opened instance*/
   dev = pContext->pDevCtx->nfc_lowHandle;

   /* Enable the autonomious messages */
   if( DSL_DRV_VRX_InternalNfcEnable( (MEI_DYN_CNTRL_T*)dev ) < 0 )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: VRx NFC enable failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERROR;
   }

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_InitDeviceDrv()"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return DSL_SUCCESS;
}

/*
   This functions stores the VRx configuration data in the device context.

   \param pContext - Pointer to DSL CPE library context structure, [I]
   \param pData    - Pointer to configuration parameters, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
static DSL_Error_t DSL_DRV_VRX_DeviceCfgStore(
   DSL_Context_t *pContext,
   DSL_Init_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   /*
      System Interface Configuration check
   */
   nErrCode = DSL_DRV_SystemInterfaceConfigCheck(pContext, &(pData->data.nDeviceCfg.sysCIF));
   if( nErrCode < DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - System Interface Configuration check failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }
   else
   {
      DSL_CTX_WRITE(pContext, nErrCode, pDevCtx->data.deviceCfg.sysCIF, pData->data.nDeviceCfg.sysCIF);
   }

   DSL_CTX_WRITE(pContext, nErrCode, pDevCtx->data.deviceCfg.ChannelConfigData[0], pData->data.nDeviceCfg.ChannelConfigData[0]);
   DSL_CTX_WRITE(pContext, nErrCode, pDevCtx->data.deviceCfg.ChannelConfigData[1], pData->data.nDeviceCfg.ChannelConfigData[1]);

   /* Call device specific implementation*/
   nErrCode = DSL_DRV_DEV_LowLevelConfigurationSet(pContext, &(pData->data.nDeviceCfg.cfg));

   if( nErrCode == DSL_ERROR )
   {
      DSL_DEBUG( DSL_DBG_MSG,
         (pContext, SYS_DBG_MSG"DSL[%02d]: ERROR - Low Level Configuration set failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }

   if (pData->data.nDeviceCfg.nMultimodeCfg.nNextMode < DSL_FW_TYPE_NA ||
       pData->data.nDeviceCfg.nMultimodeCfg.nNextMode >= DSL_FW_TYPE_LAST)
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_MSG"DSL[%02d]: ERROR - invalid nNextMode=%d specified!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), pData->data.nDeviceCfg.nMultimodeCfg.nNextMode));

      return DSL_ERR_INVALID_PARAMETER;
   }

   /* Write FSM config*/
   DSL_CTX_WRITE(pContext, nErrCode, pDevCtx->data.nCamConfig, pData->data.nDeviceCfg.nMultimodeCfg);

   if (pData->data.nDeviceCfg.nActivationCfg.nActivationSequence < DSL_ACT_SEQ_STD ||
       pData->data.nDeviceCfg.nActivationCfg.nActivationSequence >= DSL_ACT_SEQ_LAST)
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_MSG"DSL[%02d]: ERROR - invalid nActivationSequence=%d specified!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
         pData->data.nDeviceCfg.nActivationCfg.nActivationSequence));

      return DSL_ERR_INVALID_PARAMETER;
   }

   if (pData->data.nDeviceCfg.nActivationCfg.nActivationMode < DSL_ACT_MODE_NA ||
       pData->data.nDeviceCfg.nActivationCfg.nActivationMode >= DSL_ACT_MODE_LAST)
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_MSG"DSL[%02d]: ERROR - invalid nActivationMode=%d specified!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
         pData->data.nDeviceCfg.nActivationCfg.nActivationMode));

      return DSL_ERR_INVALID_PARAMETER;
   }

   /* Write FSM activation config data*/
   DSL_CTX_WRITE(pContext, nErrCode, pDevCtx->data.nActivationCfg,
      pData->data.nDeviceCfg.nActivationCfg);

   return (nErrCode);
}

/*
   Read all the Inventory information of the Near End from the device,
   and store it in the DSL CPE context

   \param pContext   Pointer to dsl library context structure, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
static DSL_Error_t DSL_DRV_VRX_NeInventoryGet(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_NeInventoryGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   DSL_DRV_MUTEX_LOCK(pContext->dataMutex);

   /*
      XTSE capabilities
   */

   /* refresh the current NE info */
   memset(pContext->lineInventoryNe.XTSECapabilities, 0,
      DSL_G997_NUM_XTSE_OCTETS);

   if(DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_ADSLA))
      memcpy(
         pContext->lineInventoryNe.XTSECapabilities,
         DSL_XtseAdslA,
         DSL_G997_NUM_XTSE_OCTETS);
   if(DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_ADSLB))
      memcpy(
         pContext->lineInventoryNe.XTSECapabilities,
         DSL_XtseAdslB,
         DSL_G997_NUM_XTSE_OCTETS);
   if(DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2))
      memcpy(
         pContext->lineInventoryNe.XTSECapabilities,
         DSL_XtseVdsl2,
         DSL_G997_NUM_XTSE_OCTETS);

   DSL_DRV_MUTEX_UNLOCK(pContext->dataMutex);

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_NeInventoryGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_LineStatusGet(
   DSL_Context_t *pContext,
   DSL_AccessDir_t nDirection,
   DSL_G997_LineStatusData_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   union
   {
      ACK_LineStatusUS_Get_t US;
      ACK_LineStatusDS_Get_t DS;
   } sAck;

   DSL_CHECK_POINTER(pContext,pData);
   DSL_CHECK_ERR_CODE();

   /* Get line status*/
   nErrCode = DSL_DRV_VRX_SendMsgLineStatusGet(pContext, nDirection, (DSL_uint8_t*)&sAck.US);

   /* Copy data only if successful */
   if (nErrCode >= DSL_SUCCESS)
   {
      if (DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_ADSL))
      {
         pData->LATN = nDirection == DSL_DOWNSTREAM ? sAck.DS.LATNds :
                                                      sAck.US.LATNus;
         /* Check for the FW 1023 special value*/
         pData->LATN = pData->LATN == 1023 ? 1271 : pData->LATN;

         pData->SATN = nDirection == DSL_DOWNSTREAM ? sAck.DS.SATNds :
                                                      sAck.US.SATNus;
         /* Check for the FW 1023 special value*/
         pData->SATN = pData->SATN == 1023 ? 1271 : pData->SATN;

         pData->SNR  = nDirection == DSL_DOWNSTREAM ?
            ((DSL_int16_t)sAck.DS.SNRMds == -512 ? -641 : (DSL_int16_t)sAck.DS.SNRMds):
            ((DSL_int16_t)sAck.US.SNRMus == -512 ? -641 : (DSL_int16_t)sAck.US.SNRMus);

         pData->ACTPS = nDirection == DSL_DOWNSTREAM ?
            ((DSL_int16_t)sAck.DS.ACTPSDds == -512 ? -641 : (DSL_int16_t)sAck.DS.ACTPSDds):
            ((DSL_int16_t)sAck.US.ACTPSDus == -512 ? -641 : (DSL_int16_t)sAck.US.ACTPSDus);
      }
      else
      {
         pData->LATN = nDirection == DSL_DOWNSTREAM ? (DSL_int16_t)sAck.DS.LATNds :
                                                      (DSL_int16_t)sAck.US.LATNus;

         pData->SATN = nDirection == DSL_DOWNSTREAM ? (DSL_int16_t)sAck.DS.SATNds :
                                                      (DSL_int16_t)sAck.US.SATNus;

         pData->SNR  = nDirection == DSL_DOWNSTREAM ?
            (sAck.DS.SNRMds == 0 ? -641 : (DSL_int16_t)sAck.DS.SNRMds):
            (sAck.US.SNRMus == 0 ? -641 : (DSL_int16_t)sAck.US.SNRMus);

         /* Not supported yet*/
         pData->ACTPS =  -901;
      }

      pData->ATTNDR  = nDirection == DSL_DOWNSTREAM ?
                          ((DSL_uint32_t)sAck.DS.ATTNDRds_LSW) |
                          (((DSL_uint32_t)sAck.DS.ATTNDRds_MSW) << 16) :
                          ((DSL_uint32_t)sAck.US.ATTNDRus_LSW) |
                          (((DSL_uint32_t)sAck.US.ATTNDRus_MSW) << 16);

      pData->ACTATP  = nDirection == DSL_DOWNSTREAM ? sAck.DS.ACTATPds :
                                                      sAck.US.ACTATPus;
   }

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file
   'drv_dsl_cpe_device_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_LineStatusPerBandGet(
   DSL_Context_t *pContext,
   DSL_AccessDir_t nDirection,
   DSL_G997_LineStatusPerBandData_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_int_t i;
   DSL_BandList_t  sBandList;
   union
   {
      ACK_LineStatusPerBandUS_Get_t US;
      ACK_LineStatusPerBandDS_Get_t DS;
   } sAck;
   DSL_G997_LineStatusData_t adslLineStatus;
   static const DSL_uint8_t nMaxNumOfBands =
      sizeof(sAck.DS.LineStatus_pb_ds)/sizeof(sAck.DS.LineStatus_pb_ds[0]);

   /* init all data */
   for (i=0; i<DSL_G997_MAX_NUMBER_OF_BANDS; i++)
   {
      pData->SNR[i]  = -32768;
      pData->LATN[i] = -32768;
      pData->SATN[i] = -32768;
   }

   /* Get VDSL specific information*/
   if (DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2))
   {
      memset(&sBandList, 0x0, sizeof(DSL_BandList_t));
      /* get number uf currently used bands */
      nErrCode = DSL_DRV_VXX_SendMsgBandControlGet(pContext, nDirection, &sBandList);
      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: ERROR - getting current band config failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return nErrCode;
      }

      nErrCode = DSL_DRV_VRX_SendMsgLineStatusPerBandGet(
                    pContext, nDirection, (DSL_uint8_t*)&sAck);

      /* Copy data only if successful */
      if (nErrCode >= 0)
      {
         /* fill only the used ones */
         for (i=0; i < sBandList.nNumData; i++)
         {
            if(i >= nMaxNumOfBands || i >= DSL_G997_MAX_NUMBER_OF_BANDS)
               break;

            if (nDirection == DSL_UPSTREAM)
            {
               pData->SNR[i]  = (DSL_int16_t)sAck.US.LineStatus_pb_us[i].SNRMpb == -512 ? -641 :
                                (DSL_int16_t)sAck.US.LineStatus_pb_us[i].SNRMpb;
               pData->LATN[i] = (DSL_int16_t)sAck.US.LineStatus_pb_us[i].LATNpb == 1023 ? 1271 :
                                (DSL_int16_t)sAck.US.LineStatus_pb_us[i].LATNpb;
               pData->SATN[i] = (DSL_int16_t)sAck.US.LineStatus_pb_us[i].SATNpb == 1023 ? 1271 :
                                (DSL_int16_t)sAck.US.LineStatus_pb_us[i].SATNpb;
            }
            else
            {
               pData->SNR[i]  = (DSL_int16_t)sAck.DS.LineStatus_pb_ds[i].SNRMpb == -512 ? -641 :
                                (DSL_int16_t)sAck.DS.LineStatus_pb_ds[i].SNRMpb;
               pData->LATN[i] = (DSL_int16_t)sAck.DS.LineStatus_pb_ds[i].LATNpb == 1023 ? 1271 :
                                (DSL_int16_t)sAck.DS.LineStatus_pb_ds[i].LATNpb;
               pData->SATN[i] = (DSL_int16_t)sAck.DS.LineStatus_pb_ds[i].SATNpb == 1023 ? 1271 :
                                (DSL_int16_t)sAck.DS.LineStatus_pb_ds[i].SATNpb;
            }
         }
      }
   }
   else
   {
      /* Get ADSL Line Status values*/
      nErrCode = DSL_DRV_VRX_LineStatusGet(pContext, nDirection, &adslLineStatus);

      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: ERROR - ADSL Line Status get failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return nErrCode;
      }

      pData->SNR[0]  = adslLineStatus.SNR;
      pData->LATN[0] = adslLineStatus.LATN;
      pData->SATN[0] = adslLineStatus.SATN;
   }

   return (nErrCode);
}

static DSL_Error_t DSL_DRV_VRX_Vdsl2ChStatusGet(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDir)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint32_t ActINP = 0;
   DSL_uint32_t Lp, Rp, Dp, q_p;
   DSL_uint32_t Nfec = 0;
   DSL_uint32_t newDataRate = 0;
   DSL_uint32_t ActualInterleaveDelay = 0;

   union
   {
      ACK_FrameDataExt2US_Get_t US;
      ACK_FrameDataExt2DS_Get_t DS;
   } sAck;

   union
   {
      ACK_BearerChsUS_Get_t US;
      ACK_BearerChsDS_Get_t DS;
   } sAckBch;

   nErrCode = DSL_DRV_VRX_SendMsgFrameDataExt2Get(pContext, nDir, (DSL_uint8_t*)&(sAck.DS));
   if( nErrCode < 0 )
      return nErrCode;

   nErrCode = DSL_DRV_VRX_SendMsgBearerChannelStatusGet(pContext, nDir, (DSL_uint8_t*)&(sAckBch.DS));
   if(nErrCode < 0)
      return nErrCode;

   /* US and DS structures are the same*/
   newDataRate = (sAckBch.US.DRusLP1_LSW | (sAckBch.US.DRusLP1_MSW << 16));

   if (newDataRate != 0)
   {
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, nLPath[DSL_UPSTREAM][0], DSL_LATENCY_FP_LP1);
   }
   else
   {
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, nLPath[DSL_UPSTREAM][0], DSL_LATENCY_IP_LP0);
   }

   /* US and DS structures are the same*/
   newDataRate += (sAckBch.US.DRusLP0_LSW | (sAckBch.US.DRusLP0_MSW << 16));
   newDataRate *= 4000;

   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, nChannelActualDataRate[nDir][0], newDataRate);

   if (nDir == DSL_UPSTREAM)
   {
      Lp   = sAck.US.Lp_LP0us;
      Rp   = sAck.US.Rp_LP0us;
      Dp   = sAck.US.Dp_LP0us;
      q_p  = sAck.US.q_LP0us;
      Nfec = sAck.US.N_fecp_LP0us;
   }
   else
   {
      Lp   = sAck.DS.Lp_LP0ds;
      Rp   = sAck.DS.Rp_LP0ds;
      Dp   = sAck.DS.Dp_LP0ds;
      q_p  = sAck.DS.q_LP0ds;
      Nfec = sAck.DS.N_fecp_LP0ds;
   }

   DSL_DEBUG( DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: Lp = %d, Rp = %d, Dp = %d, q_p = %d" DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext), Lp, Rp, Dp, q_p));

   /* The calculation is not applicable for 0 delays */
   if ((q_p * Lp) == 0)
   {
      ActINP = 0;
      DSL_DEBUG( DSL_DBG_MSG,
         (pContext, SYS_DBG_MSG"DSL[%02d]: ActINP not applicable." DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));
   }
   else
   {
      /* Formula for VDSL2 framing:*/
      ActINP = (DSL_uint32_t)(10 * 8 * Dp * (DSL_uint32_t)(Rp / (2 * q_p)) / Lp);
      DSL_DEBUG( DSL_DBG_MSG,
         (pContext, SYS_DBG_MSG"DSL[%02d]: ActINP = %d" DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext), ActINP));

      /* Calculate Interleave Delay parameter*/
      if (Nfec && newDataRate)
      {
         /*
            delay = Sp*(Dp-1)/(q_p*f_s)*[1-(q_p/Nfec)]
            where:
            Sp=8*Nfec/Lp,
            f_s - symbol rate in ksumbols/s
         */
         /* Get ActualInterleaveDelay in multiple of 1/100 ms*/
         ActualInterleaveDelay =
            ((100 * 8 * Nfec * (Dp - 1)) / (q_p * (newDataRate/1000))) * (1 - (q_p/Nfec));
         /* ... and convert it to the G997 format*/
         ActualInterleaveDelay -= (ActualInterleaveDelay % 25);
      }
      else
      {
         /* Try to get the FW value*/
         ActualInterleaveDelay = nDir == DSL_UPSTREAM ? sAckBch.US.delay_usLP0 * 100 :
                                                        sAckBch.DS.delay_dsLP0 * 100;
      }

      /* ...convert Interleave Delay parameter to the nearest 0.25ms step value.*/
      if (ActualInterleaveDelay)
      {
         ActualInterleaveDelay = (ActualInterleaveDelay % 25) ?
                                 (ActualInterleaveDelay - (ActualInterleaveDelay % 25)) + 25 :
                                 ActualInterleaveDelay;
      }
   }

   if (nDir == DSL_UPSTREAM)
   {
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, ActualInterleaveDelayUs[0], ActualInterleaveDelay);
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, ActualImpulseNoiseProtectionUs[0], ActINP);
   }
   else
   {
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, ActualInterleaveDelayDs[0], ActualInterleaveDelay);
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, ActualImpulseNoiseProtectionDs[0], ActINP);
   }

   return (nErrCode);
}


static DSL_Error_t DSL_DRV_VRX_AdslChStatusGet(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDir)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
#if !defined(INCLUDE_DSL_CPE_API_VRX_FULL_MSG)
   DSL_Error_t nRet = DSL_SUCCESS;
#endif
   DSL_boolean_t bAdsl1 = DSL_FALSE;
   DSL_uint8_t nAdsl1Mode = 0;
   DSL_uint32_t ActINP = 0;
   ACK_ADSL_FrameDataDS_LP0Get_t sAck;
   DSL_uint32_t newDataRate = 0;
   DSL_boolean_t bTrellisEnable = DSL_FALSE;
   DSL_uint32_t ActualInterleaveDelay = 0;

   union
   {
      ACK_BearerChsUS_Get_t US;
      ACK_BearerChsDS_Get_t DS;
   } sAckBch;

   nErrCode = DSL_DRV_VRX_SendMsgAdslFrameDataGet(pContext, nDir, (DSL_uint8_t*)&sAck);
   if(nErrCode < 0)
      return nErrCode;

#if !defined(INCLUDE_DSL_CPE_API_VRX_FULL_MSG)
   if (nErrCode == DSL_WRN_NOT_SUPPORTED_IN_CURRENT_ADSL_MODE_OR_ANNEX)
   {
      nRet = nErrCode;
   }
#endif

   /* calculate the ActINP here nActInp =  Rp*Dp*4/Lp */
   if (sAck.Lp > 0)
   {
      ActINP = ( 10 * 4 * sAck.Rp * sAck.Dp) / sAck.Lp;
   }

   nErrCode = DSL_DRV_VRX_SendMsgBearerChannelStatusGet(pContext, nDir, (DSL_uint8_t*)&(sAckBch.DS));
   if(nErrCode < 0)
      return nErrCode;

   DSL_CTX_READ_SCALAR(pContext, nErrCode, xtseCurr[0], nAdsl1Mode);

   /* Check for the ADSL1 mode*/
   if ( (nAdsl1Mode & XTSE_1_03_A_1_NO) || (nAdsl1Mode & XTSE_1_05_B_1_NO) ||
        (nAdsl1Mode & XTSE_1_01_A_T1_413))
   {
      bAdsl1 = DSL_TRUE;
   }

   /* Calculate Interleave Delay parameter*/
   if (bAdsl1)
   {
      /*
      delay = Sp*Dp/4 [ms],
      and in multiple of 1/100 ms:
      delay = 100*Sp*Dp/4,
      */
      ActualInterleaveDelay = (sAck.Sp == 0x8000) ?
                              (100 * sAck.Dp)/8 :
                              (100 * sAck.Sp * sAck.Dp)/4;
   }
   else
   {
      /*
      delay = Sp*Dp/4 [ms];
      Sp = 8*Nfec/Lp;
      Nfec = Mp*Kp+Rp
      and in multiple of 1/100 ms:
      delay = 100*Sp*Dp/4,
      */

      if (sAck.Lp)
      {
         ActualInterleaveDelay = (200 * (sAck.Mp * sAck.Kp + sAck.Rp) * sAck.Dp) / sAck.Lp;
      }
      else
      {
         ActualInterleaveDelay = 0;
      }
   }

   /* ...convert Interleave Delay parameter to the nearest 0.25ms step value.*/
   if (ActualInterleaveDelay)
   {
      ActualInterleaveDelay = (ActualInterleaveDelay % 25) ?
                              (ActualInterleaveDelay - (ActualInterleaveDelay % 25)) + 25 :
                              ActualInterleaveDelay;
   }

   if (nDir == DSL_UPSTREAM)
   {
      /* check whether fast path information has to be used */
      if (bAdsl1)
      {
         newDataRate = (sAckBch.US.DRusLP1_LSW | (sAckBch.US.DRusLP1_MSW << 16));
      }

      /* check whether fast path information has to be used
         FW does not indicate directly wether fast (LP1 parameters) or
         interleaved (LP0 parameters) mode is used for ADSL1.
         Use the content of DataRate LP1 to determine
         the interleaver mode therefore. */
      if (newDataRate != 0)
      {
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, nLPath[DSL_UPSTREAM][0], DSL_LATENCY_FP_LP1);
      }
      else
      {
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, nLPath[DSL_UPSTREAM][0], DSL_LATENCY_IP_LP0);
      }

      newDataRate += (sAckBch.US.DRusLP0_LSW | (sAckBch.US.DRusLP0_MSW << 16));
      newDataRate *= 4000;
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, nChannelActualDataRate[DSL_UPSTREAM][0], newDataRate);

      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, ActualInterleaveDelayUs[0], ActualInterleaveDelay);

      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, ActualImpulseNoiseProtectionUs[0], ActINP);

      bTrellisEnable = (DSL_boolean_t)sAckBch.US.TCMstatus_usLP0;
   }
   else
   {
      /* check whether fast path information has to be used */
      if (bAdsl1)
      {
         newDataRate = (sAckBch.DS.DRdsLP1_LSW | (sAckBch.DS.DRdsLP1_MSW << 16));
      }

      /* check whether fast path information has to be used
         FW does not indicate directly wether fast (LP1 parameters) or
         interleaved (LP0 parameters) mode is used for ADSL1.
         Use the content of DataRate LP1 to determine
         the interleaver mode therefore. */
      if (newDataRate != 0)
      {
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, nLPath[DSL_DOWNSTREAM][0], DSL_LATENCY_FP_LP1);
      }
      else
      {
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, nLPath[DSL_DOWNSTREAM][0], DSL_LATENCY_IP_LP0);
      }

      newDataRate += (sAckBch.DS.DRdsLP0_LSW | (sAckBch.DS.DRdsLP0_MSW << 16));
      newDataRate *= 4000;
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, nChannelActualDataRate[DSL_DOWNSTREAM][0], newDataRate);

      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, ActualInterleaveDelayDs[0], ActualInterleaveDelay);

      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, ActualImpulseNoiseProtectionDs[0], ActINP);

      bTrellisEnable = (DSL_boolean_t)sAckBch.DS.TCMstatus_dsLP0;
   }

   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, lineFeatureDataSts[nDir].bTrellisEnable, bTrellisEnable);

#if !defined(INCLUDE_DSL_CPE_API_VRX_FULL_MSG)
   if (nRet == DSL_WRN_NOT_SUPPORTED_IN_CURRENT_ADSL_MODE_OR_ANNEX)
   {
      nErrCode = DSL_WRN_INCOMPLETE_RETURN_VALUES;
   }
#endif

   return (nErrCode);
}

static DSL_Error_t DSL_DRV_VRX_ChannelStatusUpdate(
   DSL_Context_t *pContext,
   DSL_uint8_t nChannel,
   DSL_AccessDir_t nDirection)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint32_t newDataRate = 0;
   DSL_boolean_t bTrellisEnable = DSL_FALSE, bBitswapEnable = DSL_FALSE,
                 bVirtualNoiseEnable = DSL_FALSE;

   union
   {
      ACK_LineStatusUS_Get_t US;
      ACK_LineStatusDS_Get_t DS;
   } sAckLs;

   union
   {
      ACK_OLR_US_StatsGet_t US;
      ACK_OLR_DS_StatsGet_t DS;
   } sAckOlr;

   DSL_CHECK_CHANNEL_RANGE(nChannel);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_ATU_DIRECTION(nDirection);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VXX_ChannelStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   if (DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2))
   {
      /* Get line status*/
      nErrCode = DSL_DRV_VRX_SendMsgLineStatusGet(pContext, nDirection, (DSL_uint8_t*)&sAckLs.US);
      if(nErrCode < 0)
         return nErrCode;

      bTrellisEnable = nDirection == DSL_UPSTREAM ?
                      (DSL_boolean_t)sAckLs.US.TCMstatus_us :
                      (DSL_boolean_t)sAckLs.DS.TCMstatus_ds;

      bVirtualNoiseEnable = nDirection == DSL_UPSTREAM ?
                      (DSL_boolean_t)sAckLs.US.VNstatus_us :
                      (DSL_boolean_t)sAckLs.DS.VNstatus_ds;

      /* Update Trellis Status*/
      DSL_CTX_WRITE_SCALAR(
         pContext, nErrCode, lineFeatureDataSts[nDirection].bTrellisEnable,
         bTrellisEnable);

      /* Update Virtual Noise Status*/
      DSL_CTX_WRITE_SCALAR(
         pContext, nErrCode, lineFeatureDataSts[nDirection].bVirtualNoiseSupport,
         bVirtualNoiseEnable);

      /* Update ReTx status information*/
      if (nDirection == DSL_DOWNSTREAM)
           {
         nErrCode = DSL_DRV_VRX_ReTxStatusGet(pContext);

         if (nErrCode != DSL_SUCCESS)
         {
            DSL_DEBUG( DSL_DBG_ERR, (pContext,
               SYS_DBG_ERR"DSL[%02d]: ERROR - ReTx status get failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            return nErrCode;
         }
      }
   }

   nErrCode = DSL_DRV_VRX_SendMsgOlrStatusGet(pContext, nDirection, (DSL_uint8_t*)&sAckOlr.US);
   if(nErrCode < 0)
      return nErrCode;

   if (nDirection == DSL_UPSTREAM)
   {
      if (sAckOlr.US.BitswapReqs || sAckOlr.US.ExtBitswapReqs ||
          sAckOlr.US.BitswapUTCs || sAckOlr.US.BitswapsDone)
      {
         bBitswapEnable = DSL_TRUE;
      }
   }
   else
   {
      if (sAckOlr.DS.BitswapReqs || sAckOlr.DS.ExtBitswapReqs ||
          sAckOlr.DS.BitswapUTCs || sAckOlr.DS.BitswapsDone)
      {
         bBitswapEnable = DSL_TRUE;
      }
   }

   DSL_CTX_WRITE_SCALAR(
      pContext, nErrCode, lineFeatureDataSts[nDirection].bBitswapEnable, bBitswapEnable);

   /* Get bearer channel params and calculate INP */
   if (DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2))
   {
      /* VDSL2 mode */
      nErrCode = DSL_DRV_VRX_Vdsl2ChStatusGet(pContext, nDirection);
   }
   else
   {
      /* ADSL mode */
      nErrCode = DSL_DRV_VRX_AdslChStatusGet(pContext, nDirection);
   }

   if(nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - Channel Status Data for %s get failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nDirection == DSL_DOWNSTREAM ? "DS":"US"));
   }

   DSL_CTX_READ_SCALAR(pContext, nErrCode, nChannelActualDataRate[nDirection][0], newDataRate);

   /* Update Data Rate*/
   nErrCode = DSL_DRV_VXX_DataRateUpdate(pContext, nDirection, newDataRate);
   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - Data Rate Update failed (nDirection=%d)!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nDirection));

      return nErrCode;
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VXX_ChannelStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

static DSL_Error_t DSL_DRV_VRX_ReTxStatusGet(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_boolean_t bReTxEnable = DSL_FALSE;
   DSL_DEV_VersionCheck_t nVerCheck;

   ACK_VDSL_RTX_StatusGet_t RtxDsEnSt;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_ReTxStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Get FW information*/
   nErrCode = DSL_DRV_VXX_FirmwareVersionCheck(pContext,
                  DSL_MIN_FW_VERSION_RETX, &nVerCheck);
   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW version check failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }
   else
   {
      if (DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2) && nVerCheck >= DSL_VERSION_EQUAL)
      {
         /* Get line status*/
         nErrCode = DSL_DRV_VRX_SendMsgRtxDsEnableStatusGet(pContext, (DSL_uint8_t *)&RtxDsEnSt);
         if(nErrCode < 0)
            return nErrCode;

         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: DSL_DRV_VRX_ReTxStatusGet: RtxDsStatus = %hu"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), RtxDsEnSt.RtxUsed));

         bReTxEnable = (RtxDsEnSt.RtxUsed == 0) ? DSL_TRUE : DSL_FALSE;
      }

      /* Update ReTx Status */
      DSL_CTX_WRITE_SCALAR(
         pContext, nErrCode, lineFeatureDataSts[DSL_DOWNSTREAM].bReTxEnable,
         bReTxEnable);
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_ReTxStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

/*
   This function sets the NE line failures to the DSL CPE context.

   \param pContext - Pointer to dsl library context structure, [I/O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
static DSL_Error_t DSL_DRV_VRX_LineFailuresNeStatusGet(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
#if defined(INCLUDE_DSL_G997_ALARM) || \
   (defined(INCLUDE_DSL_PM) && defined(INCLUDE_DSL_CPE_PM_LINE_EVENT_SHOWTIME_COUNTERS))
   DSL_uint32_t nActLFM = 0;
   DSL_uint32_t nCfgLFM = 0;
#if defined(INCLUDE_DSL_PM) && defined(INCLUDE_DSL_CPE_PM_LINE_EVENT_SHOWTIME_COUNTERS)
   DSL_pmLineEventShowtimeData_t pmLineEventShowtimeCounters;
#endif /* defined(INCLUDE_DSL_PM) && defined(INCLUDE_DSL_CPE_PM_LINE_EVENT_SHOWTIME_COUNTERS)*/
#endif /* defined(INCLUDE_DSL_G997_ALARM) ||
         (defined(INCLUDE_DSL_PM) && defined(INCLUDE_DSL_CPE_PM_LINE_EVENT_SHOWTIME_COUNTERS))*/
   DSL_uint32_t nActLF = 0;
   DSL_uint32_t nPreLF = 0;
   DSL_uint32_t nRelevantBitMask = 0;
   DSL_LineStateValue_t nLineState = DSL_LINESTATE_UNKNOWN;
   ACK_LineFailureNE_Get_t sAck;

   nErrCode = DSL_DRV_VRX_SendMsgLineFailureNeGet(pContext, (DSL_uint8_t *)&sAck);
   if( nErrCode < 0 )
   {
      return nErrCode;
   }

   /* Get previous state of NE line failures */
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineFailuresNe, nPreLF);

   nRelevantBitMask = DSL_G997_LINEFAILURE_LPR |
                      DSL_G997_LINEFAILURE_LOF |
                      DSL_G997_LINEFAILURE_LOS |
                      DSL_G997_LINEFAILURE_LOM |
                      DSL_G997_LINEFAILURE_LOL |
                      DSL_G997_LINEFAILURE_ESE;

#if defined(INCLUDE_DSL_G997_ALARM) || \
   (defined(INCLUDE_DSL_PM) && defined(INCLUDE_DSL_CPE_PM_LINE_EVENT_SHOWTIME_COUNTERS))
   DSL_FAILUREBIT_SET(LOS_NE, nPreLF, DSL_G997_LINEFAILURE_LOS, nActLF, nActLFM);
   DSL_FAILUREBIT_SET(LOF_NE, nPreLF, DSL_G997_LINEFAILURE_LOF, nActLF, nActLFM);
   DSL_FAILUREBIT_SET(LOM_NE, nPreLF, DSL_G997_LINEFAILURE_LOM, nActLF, nActLFM);
   DSL_FAILUREBIT_SET(ESE_NE, nPreLF, DSL_G997_LINEFAILURE_ESE, nActLF, nActLFM);
#ifdef HAS_TO_BE_CROSSCHECKED_FOR_VRX
   DSL_FAILUREBIT_SET(LOL_NE, nPreLF, DSL_G997_LINEFAILURE_LOL, nActLF, nActLFM);
#endif /* HAS_TO_BE_CROSSCHECKED_FOR_VRX*/
#else
   DSL_FAILUREBIT_SET(LOS_NE, DSL_G997_LINEFAILURE_LOS, nActLF);
   DSL_FAILUREBIT_SET(LOF_NE, DSL_G997_LINEFAILURE_LOF, nActLF);
   DSL_FAILUREBIT_SET(LOM_NE, DSL_G997_LINEFAILURE_LOM, nActLF);
   DSL_FAILUREBIT_SET(ESE_NE, DSL_G997_LINEFAILURE_ESE, nActLF);
#ifdef HAS_TO_BE_CROSSCHECKED_FOR_VRX
   DSL_FAILUREBIT_SET(LOL_NE, DSL_G997_LINEFAILURE_LOL, nActLF);
#endif /* HAS_TO_BE_CROSSCHECKED_FOR_VRX*/
#endif /* INCLUDE_DSL_G997_ALARM*/

   /* Write new settings for line failures back to the DSL CPE context.
      Take care to set only failures bits that should be handled
      by this function (relevant bit mask). */
   nActLF &= nRelevantBitMask;

   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, nLineFailuresNe, nActLF);

#if defined(INCLUDE_DSL_PM) && defined(INCLUDE_DSL_CPE_PM_LINE_EVENT_SHOWTIME_COUNTERS)
   DSL_CTX_READ(pContext, nErrCode, pmLineEventShowtimeCounters, pmLineEventShowtimeCounters);

   /* Update LOS counter*/
   if (nActLFM & DSL_G997_LINEFAILURE_LOS)
   {
      pmLineEventShowtimeCounters.data_ne.nLOS++;
   }

   /* Update LOF counter*/
   if (nActLFM & DSL_G997_LINEFAILURE_LOF)
   {
      pmLineEventShowtimeCounters.data_ne.nLOF++;
   }

   /* Update LOM counter*/
   if (nActLFM & DSL_G997_LINEFAILURE_LOM)
   {
      pmLineEventShowtimeCounters.data_ne.nLOM++;
   }

   /* Update Line Event Showtime counters in the DSL CPE Context*/
   DSL_CTX_WRITE(pContext, nErrCode, pmLineEventShowtimeCounters, pmLineEventShowtimeCounters);
#endif /* defined(INCLUDE_DSL_PM) && defined(INCLUDE_DSL_CPE_PM_LINE_EVENT_SHOWTIME_COUNTERS)*/

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: Loss-of-signal failure NE (LOS) = %d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
      (nActLF & DSL_G997_LINEFAILURE_LOS) ? 1 : 0 ));

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: Loss-of-frame failure NE (LOF) = %d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
      (nActLF & DSL_G997_LINEFAILURE_LOF) ? 1 : 0 ));

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: Loss of margin failure NE (LOM) = %d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
      (nActLF & DSL_G997_LINEFAILURE_LOM) ? 1 : 0 ));

#ifdef HAS_TO_BE_CROSSCHECKED_FOR_VRX
   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: Loss of link failure NE (LOL) = %d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
      (nActLF & DSL_G997_LINEFAILURE_LOL) ? 1 : 0 ));
#endif /* HAS_TO_BE_CROSSCHECKED_FOR_VRX*/

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: Excessive Severe Errors NE (ESE) = %d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
      (nActLF & DSL_G997_LINEFAILURE_ESE) ? 1 : 0 ));

   /* Get line state*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineState, nLineState);

#ifdef INCLUDE_DSL_G997_ALARM
   /* Get (user) mask configuration for line failure event handling */
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineFailuresNeAlarmMask, nCfgLFM);

   /* Do not signal line failure events that are disbaled (masked) by the
      user. */
   nActLFM &= nCfgLFM;
   if (nActLFM != 0)
   {
      nErrCode = DSL_DRV_EventGenerate(
                     pContext, 0, DSL_ACCESSDIR_NA, DSL_NEAR_END,
                     DSL_EVENT_I_LINE_FAILURES,
                     (DSL_EventData_Union_t*)&nActLF,
                     sizeof(DSL_G997_LineFailuresData_t));
      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Event(%d) generate failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), DSL_EVENT_I_LINE_FAILURES));

         return nErrCode;
      }
   }
#endif /* INCLUDE_DSL_G997_ALARM*/

   return nErrCode;
}

/*
   This function sets the FE line failures to the DSL CPE context.

   \param pContext - Pointer to dsl library context structure, [I/O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
static DSL_Error_t DSL_DRV_VRX_LineFailuresFeStatusGet(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
#if defined(INCLUDE_DSL_G997_ALARM) || \
   (defined(INCLUDE_DSL_PM) && defined(INCLUDE_DSL_CPE_PM_LINE_EVENT_SHOWTIME_COUNTERS))
   DSL_uint32_t nActLFM = 0;
   DSL_uint32_t nCfgLFM = 0;
#if defined(INCLUDE_DSL_PM) && defined(INCLUDE_DSL_CPE_PM_LINE_EVENT_SHOWTIME_COUNTERS)
   DSL_pmLineEventShowtimeData_t pmLineEventShowtimeCounters;
#endif /* defined(INCLUDE_DSL_PM) && defined(INCLUDE_DSL_CPE_PM_LINE_EVENT_SHOWTIME_COUNTERS)*/
#endif /* defined(INCLUDE_DSL_G997_ALARM) ||
         (defined(INCLUDE_DSL_PM) && defined(INCLUDE_DSL_CPE_PM_LINE_EVENT_SHOWTIME_COUNTERS))*/
   DSL_uint32_t nActLF = 0;
   DSL_uint32_t nPreLF = 0;
   DSL_uint32_t nRelevantBitMask = 0;
   DSL_LineStateValue_t nLineState = DSL_LINESTATE_UNKNOWN;
   ACK_LineFailureFE_Get_t sAck;

   nErrCode = DSL_DRV_VRX_SendMsgLineFailureFeGet(pContext, (DSL_uint8_t*)&sAck);

   if( nErrCode < DSL_SUCCESS )
   {
      return nErrCode;
   }

   /* Get previous state of NE line failures */
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineFailuresFe, nPreLF);

   nRelevantBitMask = DSL_G997_LINEFAILURE_LPR |
                      DSL_G997_LINEFAILURE_LOF |
                      DSL_G997_LINEFAILURE_LOS |
                      DSL_G997_LINEFAILURE_LOM |
                      DSL_G997_LINEFAILURE_ESE;

#ifdef INCLUDE_DSL_G997_ALARM
   DSL_FAILUREBIT_SET(LOS_FE, nPreLF, DSL_G997_LINEFAILURE_LOS, nActLF, nActLFM);
   DSL_FAILUREBIT_SET(LOM_FE, nPreLF, DSL_G997_LINEFAILURE_LOM, nActLF, nActLFM);
   DSL_FAILUREBIT_SET(LPR_FE, nPreLF, DSL_G997_LINEFAILURE_LPR, nActLF, nActLFM);
   DSL_FAILUREBIT_SET(ESE_FE, nPreLF, DSL_G997_LINEFAILURE_ESE, nActLF, nActLFM);
#else
   DSL_FAILUREBIT_SET(LOS_FE, DSL_G997_LINEFAILURE_LOS, nActLF);
   DSL_FAILUREBIT_SET(LOM_FE, DSL_G997_LINEFAILURE_LOM, nActLF);
   DSL_FAILUREBIT_SET(LPR_FE, DSL_G997_LINEFAILURE_LPR, nActLF);
   DSL_FAILUREBIT_SET(ESE_FE, DSL_G997_LINEFAILURE_ESE, nActLF);
#endif /* INCLUDE_DSL_G997_ALARM*/

   /* Write new settings for line failures back to the DSL CPE context.
      Take care to set only failures bits that should be handled
      by this function (relevant bit mask). */
   nActLF &= nRelevantBitMask;

#if defined(INCLUDE_DSL_PM) && defined(INCLUDE_DSL_CPE_PM_LINE_EVENT_SHOWTIME_COUNTERS)
   DSL_CTX_READ(pContext, nErrCode, pmLineEventShowtimeCounters, pmLineEventShowtimeCounters);

   /* Update LOS counter*/
   if (nActLFM & DSL_G997_LINEFAILURE_LOS)
   {
      pmLineEventShowtimeCounters.data_fe.nLOS++;
   }

   /* Update LOF counter*/
   if (nActLFM & DSL_G997_LINEFAILURE_LPR)
   {
      pmLineEventShowtimeCounters.data_fe.nLPR++;
   }

   /* Update LOM counter*/
   if (nActLFM & DSL_G997_LINEFAILURE_LOM)
   {
      pmLineEventShowtimeCounters.data_fe.nLOM++;
   }

   /* Update Line Event Showtime counters in the DSL CPE Context*/
   DSL_CTX_WRITE(pContext, nErrCode, pmLineEventShowtimeCounters, pmLineEventShowtimeCounters);
#endif /* defined(INCLUDE_DSL_PM) && defined(INCLUDE_DSL_CPE_PM_LINE_EVENT_SHOWTIME_COUNTERS)*/


   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, nLineFailuresFe, nActLF);

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: Loss-of-signal failure FE (LOS) = %d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
      (nActLF & DSL_G997_LINEFAILURE_LOS) ? 1 : 0 ));

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: Loss of margin failure FE (LOM) = %d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
      (nActLF & DSL_G997_LINEFAILURE_LOM) ? 1 : 0 ));

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: Loss-of-power failure FE (LPR) = %d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
      (nActLF & DSL_G997_LINEFAILURE_LPR) ? 1 : 0 ));

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: Excessive Severe Errors Failure FE (ESE) = %d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
      (nActLF & DSL_G997_LINEFAILURE_ESE) ? 1 : 0 ));

   /* Get line state*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineState, nLineState);

#ifdef INCLUDE_DSL_G997_ALARM
   /* Get (user) mask configuration for line failure event handling */
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineFailuresFeAlarmMask, nCfgLFM);

    /* Do not signal line failure events that are disbaled (masked) by the
      user. */
   nActLFM &= nCfgLFM;
   if (nActLFM != 0)
   {
      nErrCode = DSL_DRV_EventGenerate(
                     pContext, 0, DSL_ACCESSDIR_NA, DSL_FAR_END,
                     DSL_EVENT_I_LINE_FAILURES,
                     (DSL_EventData_Union_t*)&nActLF,
                     sizeof(DSL_G997_LineFailuresData_t));
      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Event(%d) generate failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), DSL_EVENT_I_LINE_FAILURES));

         return nErrCode;
      }
   }
#endif /* INCLUDE_DSL_G997_ALARM*/

   return nErrCode;
}

static DSL_Error_t DSL_DRV_VRX_OnEapsTimeout(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: EAPS timeout occured" DSL_DRV_CRLF,
       DSL_DEV_NUM(pContext)));

   /* Clear EAPS timeout ID in the DSL CPE Context*/
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, nEapsTimeoutId, 0);

   if (DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2))
   {
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.nCurrShowtime, FWMODE_VDSL);
   }
   else if (DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_ADSL))
   {
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.nCurrShowtime, FWMODE_ADSL);
   }
   else
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Unknown FW mode!" DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));

      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.nCurrShowtime, FWMODE_NA);
   }

   /* Set bPermanentShowtime flag*/
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.bPermanentShowtime, DSL_TRUE);
#ifdef INCLUDE_DSL_PM
   /* Handle PM device specific stuff*/
   nErrCode = DSL_DRV_PM_DEV_OnEapsTimeout(pContext);

   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - PM EAPS timeout handling failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }
#endif /* INCLUDE_DSL_PM*/

   /* Handle all other necessary stuff which depends on the EAPS timeout...*/

   return nErrCode;
}

#ifdef HAS_TO_BE_CROSSCHECKED_FOR_VRX
static DSL_Error_t DSL_DRV_VRX_PollDrvForModemResetState(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_int32_t nWaitCount = 0;
   DSL_LineStateValue_t nLineState = DSL_LINESTATE_UNKNOWN;
   DSL_boolean_t bIdleReached = DSL_FALSE;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_PollDrvForModemResetState"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* wait for Modem Reset state max 4 sec */
   for (nWaitCount = 0; nWaitCount < 40; nWaitCount++)
   {
      /* Get line state*/
      nErrCode = DSL_DRV_DEV_LineStateGet( pContext, &nLineState);
      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - line state get failed, retCode=%d!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

         break;
      }

      /* Check if the DSL_LINESTATE_IDLE state reached*/
      if( nLineState == DSL_LINESTATE_IDLE )
      {
         bIdleReached = DSL_TRUE;
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: MSG - modem RESET state reached"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         break;
      }
      DSL_DRV_MSecSleep(100);
   }

   if( !bIdleReached )
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - RESET state not reached!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      nErrCode = DSL_ERROR;
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_PollDrvForModemResetState, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}
#endif /* #ifdef HAS_TO_BE_CROSSCHECKED_FOR_VRX*/


#ifdef INCLUDE_DSL_G997_FRAMING_PARAMETERS
DSL_Error_t DSL_DRV_VRX_G997_FramingParameterStatusVdsl2Get(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN const DSL_AccessDir_t nDirection,
   DSL_IN const DSL_uint8_t nChannel,
   DSL_OUT DSL_G997_FramingParameterStatusData_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   union
   {
      ACK_FrameDataExt2US_Get_t US;
      ACK_FrameDataExt2DS_Get_t DS;
   } sAck;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_G997_FramingParameterStatusVdsl2Get,"
      " (nDirection=%d, nChannel=%d)"DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
      nDirection, nChannel));

   /* Get VDSL2 framing status parameters*/
   nErrCode = DSL_DRV_VRX_SendMsgFrameDataExt2Get(pContext, nDirection, (DSL_uint8_t*)&(sAck.DS));

   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Framing Parameters get failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }
   else
   {
      pData->nINTLVBLOCK = nDirection == DSL_DOWNSTREAM ? sAck.DS.Ip_LP0ds :
                                                          sAck.US.Ip_LP0us;

      pData->nINTLVDEPTH = nDirection == DSL_DOWNSTREAM ? sAck.DS.Dp_LP0ds :
                                                          sAck.US.Dp_LP0us;

      pData->nLPATH      = 0;

      pData->nLSYMB      = nDirection == DSL_DOWNSTREAM ? sAck.DS.Lp_LP0ds :
                                                          sAck.US.Lp_LP0us;

      pData->nNFEC       = nDirection == DSL_DOWNSTREAM ? sAck.DS.N_fecp_LP0ds :
                                                          sAck.US.N_fecp_LP0us;

      pData->nRFEC       = nDirection == DSL_DOWNSTREAM ? sAck.DS.Rp_LP0ds :
                                                          sAck.US.Rp_LP0us;
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_G997_FramingParameterStatusVdsl2Get, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}
#endif /* INCLUDE_DSL_G997_FRAMING_PARAMETERS*/

#ifdef INCLUDE_DSL_G997_FRAMING_PARAMETERS
DSL_Error_t DSL_DRV_VRX_G997_FramingParameterStatusAdslGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN const DSL_AccessDir_t nDirection,
   DSL_IN const DSL_uint8_t nChannel,
   DSL_OUT DSL_G997_FramingParameterStatusData_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint16_t nNfec = 0;
   ACK_ADSL_FrameDataDS_LP0Get_t sAck;
   DSL_uint8_t XTSE[DSL_G997_NUM_XTSE_OCTETS] = {0};
   DSL_LatencyPath_t nLPath = DSL_LATENCY_UNKNOWN;

   DSL_CHECK_CHANNEL_RANGE(nChannel);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_DIRECTION(nDirection);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_G997_FramingParameterStatusAdslGet,"
      " (nDirection=%d, nChannel=%d)"DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext), nDirection, nChannel));

   /* Get ADSL framing status parameters*/
   nErrCode = DSL_DRV_VRX_SendMsgAdslFrameDataGet(pContext, nDirection, (DSL_uint8_t*)&sAck);

#if !defined(INCLUDE_DSL_CPE_API_VRX_FULL_MSG)
   if (nErrCode == DSL_WRN_NOT_SUPPORTED_IN_CURRENT_ADSL_MODE_OR_ANNEX)
   {
      return DSL_ERR_NOT_SUPPORTED_BY_FIRMWARE;
   }
#endif

   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Framing Parameters get failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }
   else
   {
      /*Get current xTSE octets*/
      DSL_CTX_READ(pContext, nErrCode, xtseCurr, XTSE);

      /* Check for ADSL1*/
      /* Special conditions:
         - fraMode and Sp are only provided in ADSL1 mode, Sp = 0x8000 means 1/2
         - Mp, Tp, SEQ, MSG are only provided in ADSL2/2+ */
      /* Nfec = M*K + R (ADSL2/2+) and
         Nfec = (S*L)/8 (ADSL1) */
      if ((XTSE[1-1] & (XTSE_1_03_A_1_NO | XTSE_1_05_B_1_NO)))
      {
         nNfec = (DSL_uint16_t)((sAck.Sp == 0x8000) ? (1 * sAck.Lp)/(8 * 2) : (sAck.Sp * sAck.Lp)/8);
      }
      else
      {
         nNfec = (DSL_uint16_t)(sAck.Mp * sAck.Kp + sAck.Rp);
      }

      pData->nINTLVBLOCK = nNfec;
      pData->nINTLVDEPTH = sAck.Dp;
      pData->nLSYMB      = sAck.Lp;
      pData->nNFEC       = nNfec;
      pData->nRFEC       = sAck.Rp;

      DSL_CTX_READ(pContext, nErrCode, nLPath[nDirection][nChannel], nLPath);

      switch (nLPath)
      {
      case DSL_LATENCY_IP_LP0:
         pData->nLPATH = 0;
         break;
      case DSL_LATENCY_FP_LP1:
         pData->nLPATH = 1;
         break;
      default:
         pData->nLPATH = nChannel;
         break;
      }
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_G997_FramingParameterStatusAdslGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}
#endif /* INCLUDE_DSL_G997_FRAMING_PARAMETERS*/

#ifdef INCLUDE_DSL_DELT
/**
   This function handles the following firmware messages which includes the
   array DELT parameters for Hlin upstream/downstream:
   + RSP_Diag_C_MSG2_LD for upstream G.992.3/5
   + RSP_Diag_R_MSG2_LD..RSP_Diag_R_MSG5_LD for downstream G.992.3
   + RSP_Diag_R_MSG2_LD..RSP_Diag_R_MSG9_LD for downstream G.992.5

   \param pContext   DSL library context
   \param pMsg       Pointer to received firmware message, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
static DSL_Error_t DSL_DRV_VRX_DeltFeUpdate(
   DSL_Context_t *pContext,
   const EVT_PMD_TestParamsGet_t *pMsg)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint16_t i = 0;
   DSL_uint16_t nDataLen = 0;

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_DeltFeUpdate"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   if( pContext->DELT_SHOWTIME == DSL_NULL )
   {
      DSL_DEBUG( DSL_DBG_WRN,
         (pContext, SYS_DBG_WRN"DSL[%02d]: WARNING - No memory to store FE DELT parameters!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   /* Store DELT data*/
   if (DSL_DRV_MUTEX_LOCK(pContext->dataMutex))
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Couldn't lock data mutex"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERROR;
   }

   nDataLen = (pMsg->EndIndex - pMsg->StartIndex) + 1;


   /* Copy retrieved data to internal buffer*/
   for( i = 0; (i < nDataLen) &&
               (i < sizeof(pMsg->TestPar)/sizeof(pMsg->TestPar[0])) &&
               (pMsg->StartIndex + i < VNX_MAX_SCGROUPS); i++ )
   {
      pContext->DELT_SHOWTIME->hlogDataUsVdsl.deltHlog.nSCGData[pMsg->StartIndex + i] = pMsg->TestPar[i].hlog;
      pContext->DELT_SHOWTIME->qlnDataUsVdsl.deltQln.nSCGData[pMsg->StartIndex + i]   = pMsg->TestPar[i].qln;
      pContext->DELT_SHOWTIME->snrDataUsVdsl.deltSnr.nSCGData[pMsg->StartIndex + i]   = pMsg->TestPar[i].snr;
   }

   pContext->DELT_SHOWTIME->hlogDataUsVdsl.deltHlog.nNumData += nDataLen;
   pContext->DELT_SHOWTIME->qlnDataUsVdsl.deltQln.nNumData   += nDataLen;
   pContext->DELT_SHOWTIME->snrDataUsVdsl.deltSnr.nNumData   += nDataLen;

   DSL_DRV_MUTEX_UNLOCK(pContext->dataMutex);

   /* Request next DELT data */
   if( pMsg->EndIndex < (VNX_MAX_SCGROUPS-1) )
   {
      nErrCode = DSL_DRV_VRX_SendMsgTestParamsFeRequest(
                    pContext,
                    (DSL_uint16_t)(pMsg->EndIndex+1),
                    (DSL_uint16_t)(DSL_MIN((pMsg->EndIndex + 32), (VNX_MAX_SCGROUPS-1))) );
      if(nErrCode != DSL_SUCCESS)
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: failed to QueryDeltArrayParams!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return (nErrCode);
      }
   }
   else
   {
      pContext->DELT_SHOWTIME->hlogDataUsVdsl.nMeasurementTime = pMsg->hlogTime;
      pContext->DELT_SHOWTIME->qlnDataUsVdsl.nMeasurementTime  = pMsg->qlnTime;
      pContext->DELT_SHOWTIME->snrDataUsVdsl.nMeasurementTime  = pMsg->snrTime;

      /* all data retrieved */
      DSL_DEBUG( DSL_DBG_MSG,
         (pContext, SYS_DBG_MSG"DSL[%02d]: All DELT FE date retrieved"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_DeltFeUpdate"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}
#endif /* INCLUDE_DSL_DELT*/

#ifdef HAS_TO_BE_CROSSCHECKED_FOR_VRX
/**
   This function copies the received data to the structure \ref DSL_FctWaiting_t.

   \param pContext Pointer to dsl library context structure, [I]
   \param pMsg    - Pointer to message data, [I]
   \param nSize   - total byte count of the message, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
*/
static DSL_Error_t DSL_DRV_VRX_WaitingMessageCopyData(
   DSL_Context_t *pContext,
   DSL_uint8_t const *pMsg,
   const DSL_uint16_t nSize)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_FctWaiting_t *pFctWait = DSL_NULL;
   DSL_int_t nLength=0;

   /* Set pointer to function waiting structure that includes the actual
      settings for the messages that has to be processed here */
   pFctWait = &(pContext->pDevCtx->data.fctWait);

   nLength = nSize;
   if (nLength<=0)
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: DSL_VNX_WaitingMessageCopyData - Length <= 0 !!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      pFctWait->bDataReceived = DSL_FALSE;
   }
   else
   {
      /* Copies the data from the firmware message to the message buffer included
         within the function waiting structure */
      memcpy(&pFctWait->nData[0], pMsg,
         (DSL_uint32_t)nLength <= sizeof(pFctWait->nData) ?
         (DSL_uint32_t)nLength : sizeof(pFctWait->nData));
   }

   pFctWait->bDataReceived = DSL_TRUE;

   return (nErrCode);
}
#endif /* HAS_TO_BE_CROSSCHECKED_FOR_VRX*/

static DSL_Error_t DSL_DRV_VRX_HandleMessage(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint8_t buf[512];
   IOCTL_MEI_message_t sMsg;
   DSL_FctWaiting_t *pFctWait = DSL_NULL;
   pFctWait = &(pContext->pDevCtx->data.fctWait);

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_HandleMessage"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   sMsg.paylSize_byte = sizeof(buf);
   sMsg.pPayload      = buf;

   /* check for received NFCs */
   if( DSL_DRV_VRX_ChReadMessage(pContext, &sMsg) >= DSL_SUCCESS )
   {
   /* one message received, handle it...*/
      if( sMsg.msgCtrl != MEI_MSG_CTRL_MODEM_MSG )
      {
         DSL_DEBUG( DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: VRx driver message received MsgID %x"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), sMsg.msgId));
      }
      else
      {
         switch(sMsg.msgId)
         {
#ifdef INCLUDE_DSL_DELT
         case EVT_PMD_TESTPARAMSGET:
            /* Get FE DELT parameters*/
            nErrCode = DSL_DRV_VRX_DeltFeUpdate(pContext, (EVT_PMD_TestParamsGet_t*)buf);
            break;
#endif /* INCLUDE_DSL_DELT*/
         default:
            DSL_DEBUG( DSL_DBG_MSG,
               (pContext, SYS_DBG_MSG"DSL[%02d]: VRx control message received MsgID %x"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext), sMsg.msgId));

            break;
         }
      }
   }

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_HandleMessage"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

static DSL_Error_t DSL_DRV_VRX_ReTxCountersGet(
   DSL_Context_t *pContext,
   DSL_ReTxCounters_t *pCounters)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_ReTxCounters_t retxCounters;
   ACK_RTX_DS_StatsGet_t retxStats;
   ACK_RTX_PMwoThreshDS_Get_t retxPmwoThresh;
   DSL_DEV_VersionCheck_t nVerCheck;

   /* Get FW information*/
   nErrCode = DSL_DRV_VXX_FirmwareVersionCheck(pContext,
               DSL_MIN_FW_VERSION_RETX, &nVerCheck);
   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW version check failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return nErrCode;
   }

   if (DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2) && nVerCheck >= DSL_VERSION_EQUAL)
   {
      /* Get first part of ReTx counters from the FW*/
      nErrCode = DSL_DRV_VRX_SendMsgRtxDsStatsGet(pContext, (DSL_uint8_t *)&retxStats);
      if(nErrCode < 0)
         return nErrCode;

      /* Get second part of ReTx counters from the FW*/
      nErrCode = DSL_DRV_VRX_SendMsgRtxPmwoThreshDsGet(pContext, (DSL_uint8_t *)&retxPmwoThresh);
      if(nErrCode < 0)
         return nErrCode;

      /* Fill the output counters structure*/
      /* RxUnCorrected */
      pCounters->nRxUncorrectedProtected =
         ((DSL_uint32_t)(retxStats.RxDtuNoCorr_MSW << 16)) | retxStats.RxDtuNoCorr_LSW;

      /* RxCorrected */
      pCounters->nRxCorrected =
         ((DSL_uint32_t)(retxStats.RxDtuCorr_MSW << 16)) | retxStats.RxDtuCorr_LSW;

      /* RxCorrupted */
      pCounters->nRxCorruptedTotal = pCounters->nRxCorrected + pCounters->nRxUncorrectedProtected;

      /* RxRetransmitted - NOT valid for VRX200 */
      pCounters->nRxRetransmitted = 0;

      /* TxRetransmitted */
      pCounters->nTxRetransmitted =
         ((DSL_uint32_t)(retxStats.TxDtuRTX_MSW << 16)) | retxStats.TxDtuRTX_LSW;

      /* ErrorFreeBits */
      pCounters->nErrorFreeBits =
         ((DSL_uint32_t)(retxPmwoThresh.ErrorFreeBits_MSW << 16)) | retxPmwoThresh.ErrorFreeBits_LSW;
   }
   else
   {
      /* Just ignore request if FW doesn't support it */
      memset(&retxCounters,0x0,sizeof(DSL_ReTxCounters_t));
   }

   DSL_CTX_READ( pContext, nErrCode, retxCounters, retxCounters);

   pCounters->nRxCorrected += retxCounters.nRxCorrected;
   pCounters->nRxCorruptedTotal += retxCounters.nRxCorruptedTotal;
   pCounters->nRxRetransmitted += retxCounters.nRxRetransmitted;
   pCounters->nRxUncorrectedProtected += retxCounters.nRxUncorrectedProtected;
   pCounters->nTxRetransmitted += retxCounters.nTxRetransmitted;
   pCounters->nErrorFreeBits += retxCounters.nErrorFreeBits;

   return nErrCode;
}

DSL_Error_t DSL_DRV_DEV_TrainingTimeoutSet(
   DSL_IN DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint32_t nTimeout;
   DSL_DEV_CamStates_t nCamState = DSL_DRV_VXX_CamFsmStateGet(pContext);
   DSL_ActivationFsmConfigData_t nActivationCfg;
   DSL_boolean_t bT1_413;

   DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.nActivationCfg, nActivationCfg);
   DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.bT1_413, bT1_413);

   if (bT1_413)
   {
      switch (nCamState)
      {
         case DSL_CAM_VDSL_FORCED:
            nTimeout = DSL_AUTOBOOT_TRAINING_TIMEOUT_V_GHS;
            break;
      
         case DSL_CAM_MULTI:
            if (nActivationCfg.nActivationSequence == DSL_ACT_SEQ_NON_STD)
            {
               if (nActivationCfg.nActivationMode == DSL_ACT_MODE_ANSI_T1413)
               {
                  nTimeout = DSL_AUTOBOOT_TRAINING_TIMEOUT_GOTO_V_ANSI;
               }
               else
               {
                  nTimeout = DSL_AUTOBOOT_TRAINING_TIMEOUT_GOTO_V_GHS;
               }
            }
            else
            {
               nTimeout = DSL_AUTOBOOT_TRAINING_TIMEOUT_GOTO_V_STD;
            }
            break;

         default:
            nTimeout = DSL_AUTOBOOT_TRAINING_TIMEOUT_DEFAULT;
            break;
      }
   }
   else
   {
      nTimeout = DSL_AUTOBOOT_TRAINING_TIMEOUT_DEFAULT;
   }

   return DSL_DRV_AutobootTimeoutSet(pContext, nTimeout);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device.h'
*/
DSL_Error_t DSL_DRV_DEV_RetxStatisticsGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_ReTxStatistics_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_boolean_t bReTxEnable = DSL_FALSE;
   DSL_DEV_VersionCheck_t nVerCheck;

   DSL_ReTxCounters_t retxCounters;

   DSL_CTX_READ_SCALAR(
      pContext, nErrCode, lineFeatureDataSts[DSL_DOWNSTREAM].bReTxEnable,
      bReTxEnable);

   if (pData->nDirection == DSL_FAR_END)
   {
      DSL_DEBUG( DSL_DBG_WRN,
         (pContext, SYS_DBG_WRN"DSL[%02d]: Far end is not supported for VRX "
         "retransmission statistics."DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));

      return nErrCode;
   }
   else
   {
      /* Get FW information*/
      nErrCode = DSL_DRV_VXX_FirmwareVersionCheck(pContext,
                  DSL_MIN_FW_VERSION_RETX, &nVerCheck);
      if (nErrCode != DSL_SUCCESS)
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW version check failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         return nErrCode;
      }

      if (DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2) && nVerCheck >= DSL_VERSION_EQUAL)
      {
         if (bReTxEnable == DSL_FALSE)
         {
            DSL_DEBUG( DSL_DBG_ERR,
               (pContext, SYS_DBG_ERR"DSL[%02d]: Retransmission is not active."DSL_DRV_CRLF,
               DSL_DEV_NUM(pContext)));

            return DSL_ERR_RETRANSMISSION_DISABLED;
         }

         pData->data.nRxRetransmitted = 0;

         nErrCode = DSL_DRV_VRX_ReTxCountersGet(pContext, &retxCounters);

         if (nErrCode != DSL_SUCCESS)
         {
            DSL_DEBUG( DSL_DBG_ERR,
               (pContext, SYS_DBG_ERR"DSL[%02d]: Unable to get counters."DSL_DRV_CRLF,
               DSL_DEV_NUM(pContext)));

            return nErrCode;
         }
      }
      else
      {
         memset(&retxCounters,0x0,sizeof(DSL_ReTxCounters_t));

         DSL_DEBUG( DSL_DBG_WRN,
            (pContext, SYS_DBG_WRN"DSL[%02d]: ReTx is not supported by this firmware."DSL_DRV_CRLF,
            DSL_DEV_NUM(pContext)));
      }

      pData->data.nRxCorrected = retxCounters.nRxCorrected;
      pData->data.nRxCorruptedTotal = retxCounters.nRxCorruptedTotal;
      pData->data.nTxRetransmitted = retxCounters.nTxRetransmitted;
      pData->data.nRxUncorrectedProtected = retxCounters.nRxUncorrectedProtected;
      pData->data.nErrorFreeBits = retxCounters.nErrorFreeBits;
   }

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device.h'
*/
DSL_Error_t DSL_DRV_DEV_ReTxCountersSave(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_ReTxCounters_t retxCounters;

   /* Save near-end rettransmission counters*/
   nErrCode = DSL_DRV_VRX_ReTxCountersGet(pContext,&retxCounters);

   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: Can't save near-end retransmission counters."DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   DSL_CTX_WRITE( pContext, nErrCode, retxCounters, retxCounters);

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device.h'
*/
DSL_Error_t DSL_DRV_DEV_LowLevelConfigurationSet(
   DSL_Context_t *pContext,
   DSL_DeviceLowLevelConfig_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_int_t tmp = 0;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_LowLevelConfigurationSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   if (DSL_DRV_MUTEX_LOCK(pContext->dataMutex))
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - data mutex lock failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERROR;
   }

   /* Check and set nFilter*/
   if( pData->nFilter != DSL_DEV_FILTER_NA )
   {
      nErrCode = DSL_WRN_CONFIG_PARAM_IGNORED;
      pData->nFilter = DSL_DEV_FILTER_NA;

   }

   /* Check and set nHsToneGroupMode */
   if( pData->nHsToneGroupMode != DSL_DEV_HS_TONE_GROUP_MODE_NA )
   {
      tmp = (DSL_int_t)pData->nHsToneGroupMode;
      nErrCode = DSL_DRV_VXX_CheckTruncParamRange(
                    pContext,
                    (DSL_int_t)DSL_DEV_HS_TONE_GROUP_MODE_AUTO,
                    (DSL_int_t)(DSL_DEV_HS_TONE_GROUP_MODE_LAST-1),
                    &tmp,
                    "nHsToneGroupMode");
      /* Set new value in the Device context */
      pContext->pDevCtx->data.deviceCfg.cfg.nHsToneGroupMode =
         (DSL_DEV_HsToneGroupMode_t)tmp;
   }

   if (pContext->pDevCtx->data.deviceCfg.cfg.nHsToneGroupMode ==
      DSL_DEV_HS_TONE_GROUP_MODE_MANUAL)
   {
      /* Check and set nHsToneGroup_A */
      if( pData->nHsToneGroup_A != DSL_DEV_HS_TONE_GROUP_NA )
      {
         tmp = (DSL_int_t)pData->nHsToneGroup_A;
         nErrCode = DSL_DRV_VXX_CheckTruncParamRange(
                       pContext,
                       (DSL_int_t)DSL_DEV_HS_TONE_GROUP_VDSL2_B43,
                       (DSL_int_t)(DSL_DEV_HS_TONE_GROUP_LAST - 1),
                       &tmp,
                       "nHsToneGroup_A");
         /* Set new value in the Device context */
         pContext->pDevCtx->data.deviceCfg.cfg.nHsToneGroup_A =
            (DSL_DEV_HsToneGroup_t)tmp;
      }
      /* Check and set nHsToneGroup_V */
      if( pData->nHsToneGroup_V != DSL_DEV_HS_TONE_GROUP_NA )
      {
         tmp = (DSL_int_t)pData->nHsToneGroup_V;
         nErrCode = DSL_DRV_VXX_CheckTruncParamRange(
                       pContext,
                       (DSL_int_t)DSL_DEV_HS_TONE_GROUP_VDSL2_B43,
                       (DSL_int_t)(DSL_DEV_HS_TONE_GROUP_LAST - 1),
                       &tmp,
                       "nHsToneGroup_V");
         /* Set new value in the Device context */
         pContext->pDevCtx->data.deviceCfg.cfg.nHsToneGroup_V =
            (DSL_DEV_HsToneGroup_t)tmp;
      }
      /* Check and set nHsToneGroup_AV */
      if( pData->nHsToneGroup_AV != DSL_DEV_HS_TONE_GROUP_NA )
      {
         tmp = (DSL_int_t)pData->nHsToneGroup_AV;
         nErrCode = DSL_DRV_VXX_CheckTruncParamRange(
                       pContext,
                       (DSL_int_t)DSL_DEV_HS_TONE_GROUP_VDSL2_B43,
                       (DSL_int_t)(DSL_DEV_HS_TONE_GROUP_LAST - 1),
                       &tmp,
                       "nHsToneGroup_AV");
         /* Set new value in the Device context*/
         pContext->pDevCtx->data.deviceCfg.cfg.nHsToneGroup_AV =
            (DSL_DEV_HsToneGroup_t)tmp;
      }
   }

   /* Check and set nBaseAddr*/
   if (pData->nBaseAddr != (DSL_uint32_t)-1)
   {
      /* Set new value in the Device context*/
      pContext->pDevCtx->data.deviceCfg.cfg.nBaseAddr = pData->nBaseAddr;
   }

   /* Check and set nIrqNum*/
   if (pData->nIrqNum != -1)
   {
      tmp = (DSL_int_t)pData->nIrqNum;
      nErrCode = DSL_DRV_VXX_CheckTruncParamRange(
                    pContext,
                    (DSL_int_t)0,
                    (DSL_int_t)99,
                    &tmp,
                    "nIrqNum");
      /* Set new value in the Device context*/
      pContext->pDevCtx->data.deviceCfg.cfg.nIrqNum = (DSL_int8_t)tmp;
   }

   tmp = (DSL_int_t)pData->bNtrEnable;
   if( tmp != -1 )
   {
      /* Set new value in the Device context*/
      pContext->pDevCtx->data.deviceCfg.cfg.bNtrEnable = pData->bNtrEnable;
   }

   DSL_DRV_MUTEX_UNLOCK(pContext->dataMutex);

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_LowLevelConfigurationSet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

DSL_Error_t DSL_DRV_DEV_ModemWriteConfig(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_boolean_t bFwReady = DSL_FALSE, bCamFwReload = DSL_FALSE, bReTxEnable = DSL_FALSE;
   DSL_TestModeControlSet_t nTestModeControl = DSL_TESTMODE_DISABLE;
   DSL_G997_RateAdaptationConfigData_t raCfgData[DSL_ACCESSDIR_LAST] =
      {{DSL_G997_RA_MODE_DYNAMIC}, {DSL_G997_RA_MODE_DYNAMIC}};
   DSL_int32_t nNoiseMarginDelta = 0;
   DSL_BF_RebootCriteriaConfigData_t RbCrNeCfgData = DSL_REBOOT_CRITERIA_CLEANED;
   DSL_uint8_t *pXtseAdslA = (DSL_uint8_t *) &DSL_XtseAdslA[0];
   DSL_uint8_t *pXtseAdslB = (DSL_uint8_t *) &DSL_XtseAdslB[0];
   DSL_uint8_t *pXtseVdsl2 = (DSL_uint8_t *) &DSL_XtseVdsl2[0];
   DSL_VNX_FwVersion_t sFwVersion = {0};
   DSL_DEV_VersionCheck_t nVerCheck;

   DSL_CHECK_CTX_POINTER(pContext);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_POINTER(pContext, pContext->pDevCtx);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_POINTER(pContext, pContext->pDevCtx->lowHandle);
   DSL_CHECK_ERR_CODE();

   /* Get FW DL information*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->bFirmwareReady, bFwReady);

   /* Get FW info*/
   DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.fwFeatures, sFwVersion);

   if( nErrCode != DSL_SUCCESS || !bFwReady )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: DSL_DEV_ModemWriteConfig: ERROR - no FW downloaded!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERROR;
   }

   /* Get FW information*/
   nErrCode = DSL_DRV_VXX_FirmwareVersionCheck(pContext,
                  DSL_MIN_FW_VERSION_RETX, &nVerCheck);
   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW version check failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return nErrCode;
   }

   /* Get bCamFwReload flag*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->data.bCamFwReload, bCamFwReload);
   if( bCamFwReload )
   {
      /* No further configurations needed in case of CAM FW reload*/
      return DSL_SUCCESS;
   }

   /* Get Test Mode Control settings*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nTestModeControl, nTestModeControl);

   /* In case of using ADSL FW there will be currently an error because it is
      not supported within FW. However for a generic implementation this error
      will be just interpreted as a warning to be able to use an updated FW
      directly with API.
      \todo Interpret error in case of ADSL FW as error instead of warning if
            ADSL FW supports handshake tone configuration. */
   nErrCode = DSL_DRV_VRX_SendMsgHsToneGroupSet(pContext, pXtseAdslA, pXtseAdslB,
      pXtseVdsl2);
   if( nErrCode!=DSL_SUCCESS )
   {
      if (DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2))
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: DSL_DEV_ModemWriteConfig: ERROR - "
            "handshake tones config write!" DSL_DRV_CRLF,
            DSL_DEV_NUM(pContext)));

         return nErrCode;
      }
      else
      {
         DSL_DEBUG( DSL_DBG_WRN, (pContext,
            SYS_DBG_WRN"DSL[%02d]: DSL_DEV_ModemWriteConfig: WARNING - "
            "Handshake tones config currently not supported by ADSL FW!" DSL_DRV_CRLF,
            DSL_DEV_NUM(pContext)));

         nErrCode = DSL_SUCCESS;
      }
   }

   /* Set Near-End Re-Init configuration options depending on the
   Test Mode Control settings*/
   DSL_CTX_READ(pContext, nErrCode, RebootCriteriaNeConfigData, RbCrNeCfgData);
   nErrCode = DSL_DRV_VRX_SendMsgReInitNeConfigure(
                 pContext,
                 (nTestModeControl != DSL_TESTMODE_SHOWTIME_LOCK
                  && nTestModeControl != DSL_TESTMODE_TRAINING_LOCK)
                 ? DSL_TRUE : DSL_FALSE, RbCrNeCfgData);
   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: DSL_DEV_ModemWriteConfig: ERROR - "
         "re-init NE config write failed!" DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   /* VTSE and Handshake configuration */
   nErrCode = DSL_DRV_VXX_XtseConfigure(pContext);
   if( nErrCode!=DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: DSL_DEV_ModemWriteConfig: ERROR - "
         "VTSE config write!" DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   nErrCode = DSL_DRV_VRX_SendMsgAlgorithmControl(pContext, 0, 0);
   if( nErrCode!=DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: DSL_DEV_ModemWriteConfig: ERROR - "
         "algorithm configuration!" DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   nErrCode = DSL_DRV_VRX_SendOperationOptionsSet(pContext);
   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: DSL_DEV_ModemWriteConfig: ERROR - "
         "operation options configuration!" DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   /* Settings related to the line Configs */
   if (((sFwVersion.nPlatform == 5) && (sFwVersion.nFeatureSet >= 4)
      && (sFwVersion.nMajor >= 0)
      && (sFwVersion.nMinor >= 8 || sFwVersion.nMajor >= 1)
      && DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_ADSL)))
   {
      nErrCode = DSL_DRV_VRX_SendMsgBearerChTcLayerSet(pContext);
      if( nErrCode!=DSL_SUCCESS )
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: DSL_DEV_ModemWriteConfig: ERROR - "
            "Channel short config write!" DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return nErrCode;
      }
   }

   /* this is not possible for ADSL CPE */
   if (DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2))
   {
      /* Settings related to the line Configs */
      nErrCode = DSL_DRV_VRX_SendMsgBearerChSet(pContext);
      if( nErrCode!=DSL_SUCCESS )
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: DSL_DEV_ModemWriteConfig: ERROR - "
            "Channel config write!" DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return nErrCode;
      }

      nErrCode = DSL_DRV_VRX_SendMsgVdsl2ProfileControl(pContext);
      if( nErrCode!=DSL_SUCCESS )
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: DSL_DEV_ModemWriteConfig: ERROR - "
            "Profile Control write!" DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return nErrCode;
      }
   }

   /* Get Rate Adaptation Mode Settings*/
   DSL_CTX_READ(pContext, nErrCode, rateAdaptationMode[DSL_DOWNSTREAM], raCfgData[DSL_DOWNSTREAM].RA_MODE);
   DSL_CTX_READ(pContext, nErrCode, rateAdaptationMode[DSL_UPSTREAM], raCfgData[DSL_UPSTREAM].RA_MODE);

   /* Send Rate Adaptation Mode Settings
      Check/set default auto SRA enable mode if not configured yet*/
   nErrCode = DSL_DRV_VRX_SendMsgOlrControl(
                 pContext,
                 raCfgData[DSL_DOWNSTREAM].RA_MODE == DSL_G997_RA_MODE_AT_INIT ?
                 DSL_FALSE : DSL_TRUE,
                 raCfgData[DSL_UPSTREAM].RA_MODE == DSL_G997_RA_MODE_AT_INIT ?
                 DSL_FALSE : DSL_TRUE,
                 raCfgData[DSL_DOWNSTREAM].RA_MODE == DSL_G997_RA_MODE_DYNAMIC_SOS ?
                 DSL_TRUE : DSL_FALSE,
                 raCfgData[DSL_UPSTREAM].RA_MODE == DSL_G997_RA_MODE_DYNAMIC_SOS ?
                 DSL_TRUE : DSL_FALSE);

   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG(DSL_DBG_ERR,
        (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Online Reconfiguration parameters set failed!" DSL_DRV_CRLF,
        DSL_DEV_NUM(pContext)));
      return nErrCode;
   }

   /* Send retransmission settings if it is possible */
   if (DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2) && nVerCheck >= DSL_VERSION_EQUAL)
   {
      /* Get ReTx control configuration */
      DSL_CTX_READ_SCALAR(pContext, nErrCode, lineFeatureDataCfg[DSL_DOWNSTREAM].bReTxEnable,
         bReTxEnable);

      nErrCode = DSL_DRV_VRX_SendMsgRtxDsConfigure(pContext, bReTxEnable);
      if (nErrCode != DSL_SUCCESS)
      {
         DSL_DEBUG(DSL_DBG_ERR,
           (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - ReTx parameters set failed!" DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));
         return nErrCode;
      }
   }

   /* Get configuration for the Noise Margin Delta parameter*/
   DSL_CTX_READ(pContext, nErrCode,
      lineOptionsConfig[DSL_OPT_NOISE_MARGIN_DELTA_DS], nNoiseMarginDelta);

   /* Set Noise Margin Delta*/
   nErrCode = DSL_DRV_VRX_SendMsgNoiseMarginDeltaSet(pContext, (DSL_int16_t)nNoiseMarginDelta);
   if ((nErrCode != DSL_SUCCESS) && (nErrCode != DSL_ERR_NOT_SUPPORTED_BY_FIRMWARE) )
   {
      DSL_DEBUG(DSL_DBG_ERR,
        (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Noise Margin Delta set failed!"
        DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return nErrCode;
   }

   /* Set Near-End Inventory information*/
   nErrCode = DSL_DRV_VRX_SendMsgInventoryNeSet(pContext);
   if( nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Line Inventory set failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

#ifdef INCLUDE_DSL_BONDING
   /* this is not possible for ADSL  CPE */
   if (DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2))
   {
      /* Write Bonding configuration*/
      nErrCode = DSL_DRV_BND_VNX_ConfigWrite(pContext);
      if( nErrCode < DSL_SUCCESS)
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Bonding Config write failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return nErrCode;
      }
   }
#endif /* INCLUDE_DSL_BONDING*/

   /* Get FW information*/
   nErrCode = DSL_DRV_VXX_FirmwareVersionCheck(pContext,
                  DSL_MIN_FW_VERSION_MISC_CFG_ADSL, &nVerCheck);
   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW version check failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return nErrCode;
   }

   if (DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2) ||
      (DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_ADSL) && nVerCheck >= DSL_VERSION_EQUAL))
   {
      nErrCode = DSL_DRV_VRX_SendMsgMiscConfigSet(pContext);
      if( nErrCode < DSL_SUCCESS)
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Misc config set failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return nErrCode;
      }
   }

   return DSL_SUCCESS;
}

DSL_DEV_Handle_t DSL_DRV_DEV_DriverHandleGet(
   DSL_int_t nMaj,
   DSL_int_t nNum)
{
   MEI_DYN_CNTRL_T *pMeiDynCntrl = DSL_NULL;

   return DSL_DRV_VRX_InternalDevOpen(
             (IFX_uint16_t)nNum,
             &pMeiDynCntrl) ? DSL_NULL : (DSL_DEV_Handle_t)pMeiDynCntrl;
}

DSL_Error_t DSL_DRV_DEV_DriverHandleDelete(
   DSL_DEV_Handle_t handle)
{
   return DSL_DRV_VRX_InternalDevClose(
             (MEI_DYN_CNTRL_T*)handle) ? DSL_ERROR : DSL_SUCCESS;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device.h'
*/
DSL_Error_t DSL_DRV_DEV_FwDownload(
   DSL_Context_t *pContext,
   const DSL_char_t *pFirmware,
   DSL_uint32_t nFirmwareSize,
   const DSL_char_t *pFirmware2,
   DSL_uint32_t nFirmwareSize2,
   DSL_int32_t *pLoff,
   DSL_int32_t *pCurrentOff,
   DSL_boolean_t bLastChunk)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_DEV_Handle_t dev;
   IOCTL_MEI_fwDownLoad_t Vdsl2_FwDl;
   IOCTL_MEI_reset_t rstCfg;
   IOCTL_MEI_FwModeCtrlSet_t fwModeSelect;
   DSL_VNX_FwVersion_t sFwVersion = {0};
   DSL_uint32_t verNum = 0;
   DSL_FirmwareRequestType_t nFwType = DSL_FW_REQUEST_NA;
#if defined(INCLUDE_DSL_PM)
   DSL_uint32_t fwDwnldStartTime = 0, fwDwnldStopTime = 0, fwDwnldTime = 0;
#endif
   DSL_FwDownloadStatusData_t fwDwnlStatus;
   DSL_uint8_t nReloadCnt = 0;

   DSL_CHECK_POINTER(pContext, pContext->pDevCtx->lowHandle);
   DSL_CHECK_ERR_CODE();

   /* Get device handle*/
   dev = DSL_DEVICE_LOWHANDLE(pContext);

   DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->data.deviceCfg.nFwType, nFwType);

#if defined(INCLUDE_DSL_PM)
   fwDwnldStartTime = DSL_DRV_ElapsedTimeMSecGet(0);
#endif

   for(;;)
   {
      memset(&Vdsl2_FwDl,0x0,sizeof(IOCTL_MEI_fwDownLoad_t));

      fwModeSelect.bMultiLineModeLock      = IFX_TRUE;
      fwModeSelect.bXdslModeLock           = IFX_TRUE;
      fwModeSelect.eMultiLineModeCurrent   = e_MEI_MULTI_LINEMODE_SINGLE;
      fwModeSelect.eMultiLineModePreferred = e_MEI_MULTI_LINEMODE_SINGLE;

      switch(nFwType)
      {
      case DSL_FW_REQUEST_ADSL:
         fwModeSelect.eXdslModeCurrent        = e_MEI_XDSLMODE_ADSL;
         fwModeSelect.eXdslModePreferred      = e_MEI_XDSLMODE_ADSL;
         break;
      case DSL_FW_REQUEST_VDSL:
         fwModeSelect.eXdslModeCurrent        = e_MEI_XDSLMODE_VDSL;
         fwModeSelect.eXdslModePreferred      = e_MEI_XDSLMODE_VDSL;
         break;
      default:
         DSL_DEBUG( DSL_DBG_ERR, (pContext,
            SYS_DBG_ERR"DSL[%02d]: ERROR - Invalid FW mode %d specified!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nFwType));

         return DSL_ERROR;
      }

      Vdsl2_FwDl.pFwImage  = (unsigned char *)pFirmware;
      Vdsl2_FwDl.size_byte = (unsigned long)nFirmwareSize;

      /* Check FW availability*/
      if ( (Vdsl2_FwDl.size_byte == 0) || (Vdsl2_FwDl.pFwImage == DSL_NULL) )
      {
         DSL_DEBUG( DSL_DBG_ERR, (pContext,
            SYS_DBG_ERR"DSL[%02d]: ERROR - VRx FW download failed - no binary for %d FW mode!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nFwType));

         return DSL_ERROR;
      }

      /* Reset the device*/
      DSL_DEBUG( DSL_DBG_MSG,
         (pContext, SYS_DBG_MSG"DSL[%02d]: Resetting the VRx device..."
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      rstCfg.rstMode    = e_MEI_RESET;
      rstCfg.rstSelMask = MEI_IOCTL_HW_RST_MASK_ALL;
      if( DSL_DRV_VRX_InternalDevReset((MEI_DYN_CNTRL_T*)dev, &rstCfg, 0x1) != 0 )
      {
         DSL_DEBUG( DSL_DBG_ERR, (pContext,
            SYS_DBG_ERR"DSL[%02d]: ERROR - VRX device reset failed, (nReturn = %d)!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), rstCfg.ictl.retCode));

         return DSL_ERROR;
      }
      else
      {
         /* Reset bFirmwareReady flag*/
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->bFirmwareReady, DSL_FALSE);
      }

      /* Set VRX device FW mode*/
      if( DSL_DRV_VRX_InternalFwModeCtrlSet((MEI_DYN_CNTRL_T*)dev, &fwModeSelect) != 0 )
      {
         DSL_DEBUG( DSL_DBG_ERR, (pContext,
            SYS_DBG_ERR"DSL[%02d]: ERROR - VRX FW Mode set failed, (nReturn = %d)!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), fwModeSelect.ictl.retCode));

         return DSL_ERROR;
      }

      /* Do the FW Download*/
      if( DSL_DRV_VRX_InternalFirmwareDownload( (MEI_DYN_CNTRL_T*)dev, &Vdsl2_FwDl) != 0 )
      {
         DSL_DEBUG( DSL_DBG_ERR, (pContext,
            SYS_DBG_ERR"DSL[%02d]: ERROR - VRx FW downloading failed, (nReturn = %d)!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), Vdsl2_FwDl.ictl.retCode));

         if (Vdsl2_FwDl.ictl.retCode == -e_MEI_ERR_INVAL_FW_IMAGE)
         {
            /* switch xDSL mode*/
            nFwType = nFwType == DSL_FW_REQUEST_ADSL ? DSL_FW_REQUEST_VDSL : DSL_FW_REQUEST_ADSL;

            /* Update CAM state*/
            nErrCode = DSL_DRV_VXX_CamFsmStateSet(pContext,
                          nFwType == DSL_FW_REQUEST_ADSL ? DSL_CAM_MULTI : DSL_CAM_VDSL_FORCED);
            if( nErrCode != DSL_SUCCESS )
            {
               DSL_DEBUG( DSL_DBG_ERR,
                  (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - CAM FSM state set failed!"
                  DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

               return DSL_ERROR;
            }

            if (nReloadCnt >= 1)
            {
               return DSL_ERROR;
            }

            nReloadCnt++;
            continue;
         }

         return DSL_ERROR;
      }

#if defined(INCLUDE_DSL_PM)
      fwDwnldStopTime = DSL_DRV_ElapsedTimeMSecGet(0);
      fwDwnldTime     = fwDwnldStartTime > fwDwnldStopTime ?
                        fwDwnldStopTime :
                        ((fwDwnldStopTime - fwDwnldStartTime)/1000);

      /* Set FW unavailable time*/
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, nFwUnavailableTime, fwDwnldTime);
#endif

      DSL_DEBUG( DSL_DBG_MSG, (pContext,
         SYS_DBG_MSG"DSL[%02d]: %s FW downloaded successfully" DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext),
         nFwType == DSL_FW_REQUEST_VDSL ? "VDSL" : "ADSL"));

      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->bFirmwareReady, DSL_TRUE);

      /* Get the downloaded FW version information*/
      if( DSL_DRV_VRX_SendMsgFwVerInfoGet(pContext, &verNum) != DSL_SUCCESS)
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW version get failed, msg send error!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return DSL_ERROR;
      }

      DSL_DRV_VRX_FirmwareVersionDecode(pContext, verNum, &sFwVersion);

      /* Write FW application type to the DSL CPE context*/
      DSL_CTX_WRITE(pContext, nErrCode,
                    pDevCtx->data.fwFeatures, sFwVersion);

      /* Get Near-End inventory information*/
      nErrCode = DSL_DRV_VRX_NeInventoryGet(pContext);
      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - NE inventory get failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

          return nErrCode;
      }

      if (nFwType == DSL_FW_REQUEST_VDSL &&
          !DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2))
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - ADSL FW was loaded, expecting VDSL FW!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return DSL_ERROR;
      }

      if (nFwType == DSL_FW_REQUEST_ADSL &&
          !DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_ADSL))
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - VDSL FW was loaded, expecting ADSL FW!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return DSL_ERROR;
      }

      break;
   } /* for(;;)*/

   /* Fill DSL_EVENT_S_FIRMWARE_DOWNLOAD_STATUS event structure*/
   fwDwnlStatus.nError  = nErrCode == DSL_SUCCESS ? DSL_FW_LOAD_SUCCESS: DSL_FW_LOAD_ERROR;
   fwDwnlStatus.nFwModeStatus =
      nFwType == DSL_FW_REQUEST_ADSL ? DSL_FW_STATUS_ADSL : DSL_FW_STATUS_VDSL;

   if( DSL_DRV_EventGenerate( pContext, 0, DSL_ACCESSDIR_NA,
          DSL_XTUDIR_NA, DSL_EVENT_S_FIRMWARE_DOWNLOAD_STATUS,
         (DSL_EventData_Union_t*)&fwDwnlStatus, sizeof(DSL_FwDownloadStatusData_t))
       != DSL_SUCCESS )
   {
       DSL_DEBUG( DSL_DBG_ERR,
          (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Event(%d) generate failed!"
          DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
          DSL_EVENT_S_FIRMWARE_DOWNLOAD_STATUS));
   }

   return (nErrCode);
}


/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device.h'
*/
DSL_Error_t DSL_DRV_DEV_DeviceInit(
   DSL_Context_t *pContext,
   DSL_Init_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint8_t nVtse = 0, nAtse = 0, nAtseCurr = 0, i = 0, nXtse1 = 0;
   DSL_DEV_CamStates_t nCamStateNew = DSL_CAM_INIT;
   DSL_MultimodeFsmConfigData_t nCamOpt;
   DSL_ActivationFsmConfigData_t nActivationCfg;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   /* Store Device specific configuration data in the device context*/
   nErrCode = DSL_DRV_VRX_DeviceCfgStore(pContext, pData);
   if (nErrCode < DSL_SUCCESS)
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - VRx device configuration store failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   /* Check current driver state, do the driver init, */
   if ( DSL_DRV_VRX_InitDeviceDrv(pContext, &pData->data.nDeviceCfg.cfg) != DSL_SUCCESS)
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - VRx driver init failed!" DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));

      return DSL_ERROR;
   }


   /* get the ADSL bits */
   for (i = 0; i < DSL_G997_NUM_XTSE_OCTETS - 1; i++)
   {
      DSL_CTX_READ_SCALAR(pContext, nErrCode, xtseCfg[i], nAtseCurr);
      nAtse |= nAtseCurr;
   }

   /* get the VDSL bits */
   DSL_CTX_READ_SCALAR(pContext, nErrCode, xtseCfg[7], nVtse);

   /* Enable CAM FSM */
   nErrCode = DSL_DRV_VXX_CamFsmEnable(pContext, (nAtse && nVtse) ? DSL_TRUE : DSL_FALSE);
   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - CAM FSM init failed!" DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));

      return DSL_ERROR;
   }

   /* Reset multimode operation flag*/
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.bMultimode, DSL_FALSE);

   /* Read FSM configuration values*/
   DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.nCamConfig, nCamOpt);

   DSL_CTX_READ_SCALAR(pContext, nErrCode, xtseCfg[0], nXtse1);
   DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.nActivationCfg, nActivationCfg);


   if (nXtse1 & XTSE_1_01_A_T1_413)
   {
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.bT1_413, DSL_TRUE);
   }
   else
   {
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.bT1_413, DSL_FALSE);
      if (nActivationCfg.nActivationSequence == DSL_ACT_SEQ_NON_STD)
      {
         DSL_DEBUG( DSL_DBG_WRN, (pContext,
            SYS_DBG_ERR"DSL[%02d]: WARNING - Non-standard activation sequence configured "
                       "with a disabled T1.413 mode" DSL_DRV_CRLF,
            DSL_DEV_NUM(pContext)));
      }
   }

   if (nAtse && nVtse)
   {
      switch(nCamOpt.nNextMode)
      {
      case DSL_FW_TYPE_VDSL:
         /* Try again in VDSL mode*/
         nCamStateNew = DSL_CAM_VDSL_FORCED;
         /* Set VDSL FW image type*/
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.deviceCfg.nFwType, DSL_FW_REQUEST_VDSL);
         /* Set Activation mode to GHS*/
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode,
            pDevCtx->data.nActivationCfg.nActivationMode, DSL_ACT_MODE_GHS);
         break;

      case DSL_FW_TYPE_ADSL:
         nCamStateNew = DSL_CAM_MULTI;
         /* Set multimode operation flag*/
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.bMultimode, DSL_TRUE);

         /* Set ADSL FW image type*/
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.deviceCfg.nFwType, DSL_FW_REQUEST_ADSL);
         break;

      case DSL_FW_TYPE_NA:
      default:
         /*
         No dedicated information is specified. It is used to activate a API internal
         multimode default handling. */
         nCamStateNew = DSL_CAM_MULTI;
         /* Set multimode operation flag*/
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.bMultimode, DSL_TRUE);
         break;
      }
   }
   else if (nAtse)
   {
      nCamStateNew = DSL_CAM_MULTI;
      /* Set multimode operation flag*/
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.bMultimode, DSL_TRUE);

      /* Set ADSL FW image type*/
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.deviceCfg.nFwType, DSL_FW_REQUEST_ADSL);
   }
   else if (nVtse)
   {
      nCamStateNew = DSL_CAM_VDSL_FORCED;
      /* Set VDSL FW image type*/
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.deviceCfg.nFwType, DSL_FW_REQUEST_VDSL);
   }
   else
   {
      /* Set VDSL mode by default if no XTSE configuration defined*/
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.deviceCfg.nFwType, DSL_FW_REQUEST_VDSL);
   }

   nErrCode = DSL_DRV_VXX_CamFsmStateSet(pContext, nCamStateNew);
   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - CAM FSM state set failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   /* Reset FW mode information*/
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.nLastShowtime, FWMODE_NA);
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.nCurrShowtime, FWMODE_NA);
   /* Reset bPermanentShowtime information*/
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.bPermanentShowtime, DSL_FALSE);

   DSL_DRV_MUTEX_INIT(pContext->pDevCtx->data.fctWait.MsgTransferMutex);

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device.h'
*/
DSL_Error_t DSL_DRV_DEV_LinkReset(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_LinkReset"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
#ifdef HAS_TO_BE_CROSSCHECKED_FOR_VRX
   nErrCode = DSL_DRV_VXX_SendMsgModemStateSet(
             pContext, CMD_ModemFSM_StateSet_LINKRES);

   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FSM state set failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERROR;
   }

    /* Poll for the modem RESET state at least 4 sec*/
   nErrCode = DSL_DRV_VRX_PollDrvForModemResetState(pContext);
   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - RESET state poll failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }
#endif /* HAS_TO_BE_CROSSCHECKED_FOR_VRX*/

   /* Update Power Management Status*/
   nErrCode = DSL_DRV_VXX_PowerManagementStatusSet(pContext, DSL_G997_PMS_L3);
   if(nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - Power Management State set failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

     return nErrCode;
   }

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_LinkReset"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device.h'
*/
DSL_Error_t DSL_DRV_DEV_FirmwareVersionGet(
   DSL_Context_t *pContext,
   DSL_char_t *pString)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_boolean_t bFwReady = DSL_FALSE;
   DSL_uint32_t verNum = 0;
   DSL_VNX_FwVersion_t sFwVersion = {0};

   DSL_CHECK_POINTER(pContext, pString);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_POINTER(pContext, pContext->pDevCtx);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_POINTER(pContext, pContext->pDevCtx->lowHandle);
   DSL_CHECK_ERR_CODE();

   /* Get FW DL information*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->bFirmwareReady, bFwReady);

   if( nErrCode == DSL_SUCCESS && bFwReady )
   {
      nErrCode = DSL_DRV_VRX_SendMsgFwVerInfoGet(pContext, &verNum);
      if( nErrCode != DSL_SUCCESS)
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW version get failed, msg send error!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return DSL_ERROR;
      }

      DSL_DRV_VRX_FirmwareVersionDecode(pContext, verNum, &sFwVersion);

      DSL_DRV_snprintf(pString, MAX_INFO_STRING_LEN,
         "%d.%d.%d.%d.%d.%d",
         sFwVersion.nPlatform,
         sFwVersion.nFeatureSet,
         sFwVersion.nMajor,
         sFwVersion.nMinor,
         sFwVersion.nReleaseStatus,
         sFwVersion.nApplication);

      DSL_DEBUG( DSL_DBG_MSG,
         (pContext, SYS_DBG_MSG"DSL[%02d]: Firmware version: %s (plt.fts.maj.min.rel.app)"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), pString));
   }
   else
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - FW version get failed, no FW loaded!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERROR;
   }

   return DSL_SUCCESS;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device.h'
*/
DSL_Error_t DSL_DRV_DEV_ChipHardwareVersionGet(
   DSL_Context_t *pContext,
   DSL_char_t *pString)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_boolean_t bFwReady = DSL_FALSE;
   DSL_uint32_t verNum = 0;

   DSL_CHECK_POINTER(pContext, pString);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_POINTER(pContext, pContext->pDevCtx);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_POINTER(pContext, pContext->pDevCtx->lowHandle);
   DSL_CHECK_ERR_CODE();

   /* Get FW DL information*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->bFirmwareReady, bFwReady);

   if( nErrCode == DSL_SUCCESS && bFwReady )
   {
      nErrCode = DSL_DRV_VRX_SendMsgHwVerInfoGet(pContext, &verNum);
      if( nErrCode != DSL_SUCCESS)
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - HW version get failed, msg send error!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return DSL_ERROR;
      }

      DSL_DRV_VRX_HardwareVersionDecode(pContext, verNum, pString);

      DSL_DEBUG( DSL_DBG_MSG,
         (pContext, SYS_DBG_MSG"DSL[%02d]: Hardware version: %s" DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext), pString));
   }
   else
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - HW version get failed, no FW loaded!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERROR;
   }

   return DSL_SUCCESS;
}


/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device.h'
*/
DSL_Error_t DSL_DRV_DEV_DriverVersionGet(
   DSL_Context_t *pContext,
   DSL_char_t *pString)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_DEV_Handle_t dev;
   IOCTL_MEI_drvVersion_t drvVersion;
   DSL_char_t Version[80]="";

   DSL_CHECK_POINTER(pContext, pString);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_POINTER(pContext, pContext->pDevCtx);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_POINTER(pContext, pContext->pDevCtx->lowHandle);
   DSL_CHECK_ERR_CODE();

   if( nErrCode == DSL_SUCCESS )
   {

      /* Get device handle*/
      dev = DSL_DEVICE_LOWHANDLE(pContext);

      drvVersion.pVersionStr = Version;
      drvVersion.strSize = sizeof(Version);

      /* Request the driver version*/
      if ( DSL_DRV_VRX_InternalDrvVersionGet((MEI_DYN_CNTRL_T*)dev, &drvVersion) != 0 )
      {
         DSL_DEBUG( DSL_DBG_ERR, (pContext,
            SYS_DBG_ERR"DSL[%02d]: VRx driver version get failed, error %d"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), drvVersion.ictl.retCode));

         return DSL_ERROR;
      }

      memcpy((void*)pString, (void*)&Version[0], MAX_INFO_STRING_LEN);

      DSL_DEBUG( DSL_DBG_MSG,
         (pContext, SYS_DBG_MSG"DSL[%02d]: Device Driver version: %s"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), pString));
   }
   else
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - Driver version get failed, no FW loaded!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERROR;
   }

   return DSL_SUCCESS;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device.h'
*/
DSL_Error_t DSL_DRV_DEV_HybridTypeGet(
   DSL_Context_t *pContext,
   DSL_HybridType_t *pHybrid)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_boolean_t bFwReady = DSL_FALSE;

   DSL_CHECK_POINTER(pContext, pHybrid);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_POINTER(pContext, pContext->pDevCtx);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_POINTER(pContext, pContext->pDevCtx->lowHandle);
   DSL_CHECK_ERR_CODE();

   /* Get FW DL information*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->bFirmwareReady, bFwReady);

   if( nErrCode == DSL_SUCCESS && bFwReady )
   {
      nErrCode = DSL_DRV_VRX_SendMsgHybridTypeGet(pContext, pHybrid);
      if( nErrCode != DSL_SUCCESS)
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Hybrid type get failed, msg send error!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return DSL_ERROR;
      }

      DSL_DEBUG( DSL_DBG_MSG,
         (pContext, SYS_DBG_MSG"DSL[%02d]: Hybrid type is: %d" DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext), &pHybrid));
   }
   else
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - Hybrid type get failed, no FW loaded!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERROR;
   }

   return DSL_SUCCESS;
}

DSL_Error_t DSL_DRV_DEV_MeiShowtimeSignaling
(
   DSL_Context_t *pContext,
   DSL_boolean_t bShowtimeEntry)
{
#ifdef INCLUDE_DSL_ATM_PTM_INTERFACE_SUPPORT
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_DEV_Handle_t dev;
   DSL_uint32_t nRate = 0;
   DSL_LatencyPath_t nLPath = DSL_LATENCY_UNKNOWN;
   MEI_XTM_ShowtimeEnter_t meiArgXtmEnter = {0};
   MEI_XTM_ShowtimeExit_t meiArgXtmExit = {0};

   /* Get device handle*/
   dev = DSL_DEVICE_LOWHANDLE(pContext);

   if (bShowtimeEntry)
   {
      /* Update channel status information*/
      nErrCode = DSL_DRV_DEV_ChannelsStatusUpdate(pContext);

      if (nErrCode != DSL_SUCCESS)
      {
         DSL_DEBUG( DSL_DBG_ERR, (pContext,
            SYS_DBG_ERR"DSL[%02d]: ERROR - Channels status update failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return nErrCode;
      }

      DSL_CTX_READ_SCALAR(pContext, nErrCode, nChannelActualDataRate[DSL_UPSTREAM][0], nRate);
      DSL_CTX_READ_SCALAR(pContext, nErrCode, nLPath[DSL_UPSTREAM][0], nLPath);

      if (nLPath == DSL_LATENCY_FP_LP1)
      {
         meiArgXtmEnter.rate_fast = nRate;
      }
      else
      {
         meiArgXtmEnter.rate_intl = nRate;
      }

      DSL_DEBUG( DSL_DBG_WRN,
         (pContext, SYS_DBG_WRN"DSL[%02d]: MEI_InternalXtmSwhowtimeEntrySignal"
         "(nFast=%u, nIntl=%u)"DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext), meiArgXtmEnter.rate_fast, meiArgXtmEnter.rate_intl));

      if (MEI_InternalXtmSwhowtimeEntrySignal((MEI_DYN_CNTRL_T*)dev, &meiArgXtmEnter))
      {
         DSL_DEBUG( DSL_DBG_ERR, (pContext,
            SYS_DBG_ERR"DSL[%02d]: ERROR - ATM/PTM Showtime Entry Signal failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         return DSL_ERROR;
      }
   }
   else
   {
      DSL_DEBUG( DSL_DBG_WRN,
         (pContext, SYS_DBG_WRN"DSL[%02d]: MEI_InternalXtmSwhowtimeExitSignal()"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      if (MEI_InternalXtmSwhowtimeExitSignal((MEI_DYN_CNTRL_T*)dev, &meiArgXtmExit))
      {
         DSL_DEBUG( DSL_DBG_ERR, (pContext,
            SYS_DBG_ERR"DSL[%02d]: ERROR - ATM/PTM Showtime Exit Signal failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         return DSL_ERROR;
      }
   }
#endif /* INCLUDE_DSL_ATM_PTM_INTERFACE_SUPPORT*/

   return DSL_SUCCESS;
}

DSL_Error_t DSL_DRV_DEV_ChannelsStatusUpdate(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_int32_t i;

   DSL_DEBUG( DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_ChannelsStatusUpdate"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Update Channel status information*/
   for(i = 0; i < DSL_ACCESSDIR_LAST; i++)
   {
      nErrCode = DSL_DRV_VRX_ChannelStatusUpdate(pContext, 0, i);

      if(nErrCode != DSL_SUCCESS)
      {
         DSL_DEBUG( DSL_DBG_ERR, (pContext,
            SYS_DBG_ERR"DSL[%02d]: ERROR - Channel %d status update failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), i));

         break;
      }
   }

   DSL_DEBUG( DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_ChannelsStatusUpdate"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device.h'
*/
DSL_Error_t DSL_DRV_DEV_ShowtimeStatusUpdate(
   DSL_Context_t *pContext,
   DSL_boolean_t bInit)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
#ifdef INCLUDE_DSL_DELT
   ACK_TestParamsAuxDS_Get_t  sAck;
#endif /* INCLUDE_DSL_DELT*/
#ifdef INCLUDE_DSL_ATM_PTM_INTERFACE_SUPPORT
   DSL_DEV_Handle_t dev;
   DSL_uint32_t nRate = 0;
   DSL_LatencyPath_t nLPath = DSL_LATENCY_UNKNOWN;
   MEI_XTM_ShowtimeEnter_t meiArgXtm = {0};
#endif /* INCLUDE_DSL_ATM_PTM_INTERFACE_SUPPORT*/
   DSL_uint32_t nEapsTimeout = 0, nEapsTimeoutId;
   DSL_boolean_t bMultimode = DSL_FALSE;
   DSL_uint8_t nXTSE1 = 0;

   DSL_CHECK_CTX_POINTER(pContext);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_ShowtimeStatusUpdate"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Update Channel status information*/
   nErrCode = DSL_DRV_DEV_ChannelsStatusUpdate(pContext);

   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - Channels status update failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   if( bInit )
   {
#ifdef INCLUDE_DSL_ATM_PTM_INTERFACE_SUPPORT
      DSL_CTX_READ_SCALAR(pContext, nErrCode, nChannelActualDataRate[DSL_UPSTREAM][0], nRate);
      DSL_CTX_READ_SCALAR(pContext, nErrCode, nLPath[DSL_UPSTREAM][0], nLPath);

      if (nLPath == DSL_LATENCY_FP_LP1)
      {
         meiArgXtm.rate_fast = nRate;
      }
      else
      {
         meiArgXtm.rate_intl = nRate;
      }


      /* Get device handle*/
      dev = DSL_DEVICE_LOWHANDLE(pContext);

      if (MEI_InternalXtmSwhowtimeEntrySignal((MEI_DYN_CNTRL_T*)dev, &meiArgXtm))
      {
         DSL_DEBUG( DSL_DBG_ERR, (pContext,
            SYS_DBG_ERR"DSL[%02d]: ERROR - ATM/PTM Showtime Entry Signal failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         return DSL_ERROR;
      }
#endif /* INCLUDE_DSL_ATM_PTM_INTERFACE_SUPPORT*/

      /*
      Workaround since FW reports XTSE status after ~1 sec after signalling
      FULL_INIT state
      */
      /* Update XTSE status information*/
      nErrCode = DSL_DRV_VXX_SendMsgXtseStatusGet( pContext );
      if(nErrCode != DSL_SUCCESS)
      {
         DSL_DEBUG( DSL_DBG_ERR, (pContext,
            SYS_DBG_ERR"DSL[%02d]: ERROR - XTSE status get failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         return nErrCode;
      }

      /* Set the showtime indication flag*/
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, bShowtimeReached, DSL_TRUE);

      /* Get multimode operation flag*/
      DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.bMultimode, bMultimode);

      /* Add EAPS timeout event.*/
      nEapsTimeoutId = DSL_DRV_Timeout_AddEvent(pContext, (DSL_int_t)DSL_TIMEOUTEVENT_EAPS, nEapsTimeout);
      /* Set EAPS timeout ID in the DSL CPE Context*/
      DSL_CTX_WRITE(pContext, nErrCode, nEapsTimeoutId, nEapsTimeoutId);

      /* Reset CAM state*/
      nErrCode = DSL_DRV_VXX_CamFsmStateSet(pContext, DSL_CAM_INIT);
      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - CAM FSM state set failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return nErrCode;
      }

      /* Read XTSE1 bits */
      DSL_CTX_READ(pContext, nErrCode, xtseCurr[0], nXTSE1);

      /* Update the current activation mode value */
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode,
         pDevCtx->data.nActivationCfg.nActivationMode,
         (nXTSE1 & XTSE_1_01_A_T1_413) ? DSL_ACT_MODE_ANSI_T1413 : DSL_ACT_MODE_GHS);

      /* Update XTSE VDSL2 status information*/
      if( DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2) )
      {
         nErrCode = DSL_DRV_VXX_SendMsgSelectedProfileVdsl2Get(pContext);
         if( nErrCode != DSL_SUCCESS )
         {
            DSL_DEBUG( DSL_DBG_ERR, (pContext,
               SYS_DBG_ERR"DSL[%02d]: ERROR - Selected Profile Vdsl2 Get failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            return nErrCode;
         }
      }

      /* Update Power Management Status*/
      nErrCode = DSL_DRV_VXX_PowerManagementStatusSet(pContext, DSL_G997_PMS_L0);
      if(nErrCode != DSL_SUCCESS)
      {
         DSL_DEBUG( DSL_DBG_ERR, (pContext,
            SYS_DBG_ERR"DSL[%02d]: ERROR - Power Management State set failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

        return nErrCode;
      }

#ifdef INCLUDE_DSL_DELT
      if (DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2))
      {
#ifndef DSL_CPE_STATIC_DELT_DATA
         if (pContext->DELT_SHOWTIME == DSL_NULL)
         {
            pContext->DELT_SHOWTIME = DSL_DRV_Malloc(sizeof(DSL_G997_DeltShowtimeData_t));
            if( pContext->DELT_SHOWTIME == DSL_NULL )
            {
               DSL_DEBUG( DSL_DBG_ERR,
                  (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - memory for DELT SHOWTIME data "
                  "could not be allocated!"DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

               return nErrCode;
            }
         }
#endif /* #ifndef DSL_CPE_STATIC_DELT_DATA*/

         memset(pContext->DELT_SHOWTIME, 0x0, sizeof(DSL_G997_DeltShowtimeData_t));

         /* Get FE Aux Test parameters here only for the VDSL US*/
         nErrCode = DSL_DRV_VRX_SendMsgTestParamsAuxGet(pContext, DSL_UPSTREAM, (DSL_uint8_t*)&sAck);
         if( nErrCode != DSL_SUCCESS )
         {
            DSL_DEBUG(DSL_DBG_ERR,
               (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - DELT Aux US data get failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            return DSL_ERROR;
         }

         /* Update VDSL FE group size parameter*/
         pContext->DELT_SHOWTIME->hlogDataUsVdsl.nGroupSize = (DSL_uint8_t)(sAck.HLOGG);
         pContext->DELT_SHOWTIME->qlnDataUsVdsl.nGroupSize  = (DSL_uint8_t)(sAck.QLNG);
         pContext->DELT_SHOWTIME->snrDataUsVdsl.nGroupSize  = (DSL_uint8_t)(sAck.SNRG);

         /* Request firts portion of DELT data from the FE*/
         nErrCode = DSL_DRV_VRX_SendMsgTestParamsFeRequest(pContext, 0, 31);
      }
#endif /* INCLUDE_DSL_DELT*/
   }
   else
   {
      nErrCode = DSL_DRV_VXX_DataPathFailuresStatusGet(pContext);
      if(nErrCode != DSL_SUCCESS)
      {
         DSL_DEBUG( DSL_DBG_ERR, (pContext,
            SYS_DBG_ERR"DSL[%02d]: ERROR - Data Path Failures update failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

        return nErrCode;
      }

      /* Update Line failures information*/
      nErrCode = DSL_DRV_VXX_LineFailuresStatusUpdate(pContext);
      if(nErrCode != DSL_SUCCESS)
      {
         DSL_DEBUG( DSL_DBG_ERR, (pContext,
            SYS_DBG_ERR"DSL[%02d]: ERROR - Line Failures update failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

        return nErrCode;
      }

      /* Handle VRX driver autonomous messages*/
      nErrCode = DSL_DRV_VRX_HandleMessage(pContext);
   }


   DSL_DEBUG( DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_ShowtimeStatusUpdate"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return (nErrCode);
}

/*
   VRx Device specific implementation of the DSL_DRV_OnTimeoutEvent() function
*/
DSL_Error_t DSL_DRV_DEV_OnTimeoutEvent(
   DSL_Context_t *pContext,
   DSL_int_t nEventType,
   DSL_uint32_t nTimeoutID)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CHECK_CTX_POINTER(pContext);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: DSL_DRV_VNX_OnTimeoutEvent: "
      "nEventType=%d, nTimeoutID=0x%08X" DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext), nEventType, nTimeoutID));

   switch (nEventType)
   {
#ifdef INCLUDE_DSL_G997_LINE_INVENTORY
   case DSL_TIMEOUTEVENT_FE_STATUS:
      nErrCode = DSL_DRV_VXX_OnLineInventoryFe(pContext);
      if (nErrCode == DSL_SUCCESS)
      {
         /* Set FE line inventory flag*/
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, bFeLineInventoryValid, DSL_TRUE);
      }
      break;
#endif /* INCLUDE_DSL_G997_LINE_INVENTORY*/
   case DSL_TIMEOUTEVENT_EAPS:
      nErrCode = DSL_DRV_VRX_OnEapsTimeout(pContext);
      break;
   default:
      DSL_DEBUG(DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: Unknown nEventType (%d)" DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext), nEventType));
      break;
   }

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'lib_dsl_cpe_api.h'
*/
DSL_Error_t DSL_DRV_DEV_DBG_DeviceMessageSend(
   DSL_Context_t *pContext,
   DSL_DeviceMessageData_t *pMsg )
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   IOCTL_MEI_messageSend_t sRw;

   if (pMsg->nSizeTx < 6)
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - The device message has to consist of ID, "
          "Index and Length minimum!" DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      nErrCode = DSL_ERROR;

      return (nErrCode);
   }
   if (pMsg->nSizeRx < 6)
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - The device Acknowledge has to keep ID, "
          "Index and Length minimum!" DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      nErrCode = DSL_ERROR;

      return (nErrCode);
   }

   sRw.write_msg.msgId          = *(DSL_uint16_t*)pMsg->pMsg;
   sRw.write_msg.pPayload       = pMsg->pMsg+2;
   sRw.write_msg.paylSize_byte  = pMsg->nSizeTx-2;
   /* to receive the answer hand over the payload buffer */
   sRw.ack_msg.pPayload         = pMsg->pMsg+2;
   sRw.ack_msg.paylSize_byte    = pMsg->nSizeRx-2;

   nErrCode = DSL_DRV_VRX_ChRequestMessage(pContext, &sRw);
   if (nErrCode < 0)
   {
      return (DSL_ERROR);
   }
   else
   {
      *(DSL_uint16_t*)pMsg->pMsg = sRw.ack_msg.msgId;
      pMsg->nSizeRx = (DSL_uint16_t)(sRw.ack_msg.paylSize_byte + 2);
   }

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device.h'
*/
DSL_Error_t DSL_DRV_DEV_LED_FirmwareInit(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_ERR_NOT_IMPLEMENTED;

   return nErrCode;
}

#ifdef INCLUDE_DSL_SYSTEM_INTERFACE
/*
   This function gets the System Interface Status to the DSL CPE context.

   \param pContext - Pointer to dsl library context structure, [I/O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
static DSL_Error_t DSL_DRV_VRX_SystemInterfaceStatusGet(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   ACK_TC_StatusGet_t sAck;

   nErrCode = DSL_DRV_VRX_SendMsgTcStatusGet(pContext, (DSL_uint8_t*)&sAck);
   if( nErrCode < DSL_SUCCESS )
   {
      return nErrCode;
   }

   switch (sAck.TC)
   {
   case ACK_TC_StatusGet_EFM_TC:
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.sysCIFSts.nTcLayer, DSL_TC_EFM);
      break;
   case ACK_TC_StatusGet_ATM_TC:
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.sysCIFSts.nTcLayer, DSL_TC_ATM);
      break;
   default:
      nErrCode = DSL_ERROR;
      DSL_DEBUG( DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - System interface status unknown!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   return nErrCode;
}

DSL_Error_t DSL_DRV_VRX_SystemInterfaceStatusUpdate(
   DSL_IN DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_VNX_FwVersion_t  sFwVersion = {0};
   DSL_SystemInterfaceConfigData_t stsData =
      {DSL_TC_UNKNOWN, DSL_EMF_TC_CLEANED, DSL_EMF_TC_CLEANED, DSL_SYSTEMIF_UNKNOWN};

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_SystemInterfaceStatusUpdate"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Get FW info*/
   DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.fwFeatures, sFwVersion);

   /* Update system interface status */
   if (DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2))
   {
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.sysCIFSts.nTcLayer, DSL_TC_EFM);
      DSL_CHECK_ERR_CODE();
   }
   else
   {
      /* System interface configuration support */
      if ((sFwVersion.nPlatform == 5) && (sFwVersion.nFeatureSet >= 4)
         && (sFwVersion.nMajor >= 0)
         && (sFwVersion.nMinor >= 8 || sFwVersion.nMajor >= 1))
      {
         nErrCode = DSL_DRV_VRX_SystemInterfaceStatusGet(pContext);
         if(nErrCode != DSL_SUCCESS)
         {
            DSL_DEBUG( DSL_DBG_ERR, (pContext,
               SYS_DBG_ERR"DSL[%02d]: ERROR - System interface status update failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            return nErrCode;
         }
      }
      else
      {
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.sysCIFSts.nTcLayer, DSL_TC_ATM);
         DSL_CHECK_ERR_CODE();
      }
   }

   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.bSystemIfStatusValid, DSL_TRUE);

   /* NDv: Temporarily it is assumed that this configuration cannot be changed */
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.sysCIFSts.nEfmTcConfigUs, DSL_EMF_TC_CLEANED);
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.sysCIFSts.nEfmTcConfigDs, DSL_EMF_TC_CLEANED);
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.sysCIFSts.nSystemIf, DSL_SYSTEMIF_MII);

   DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.sysCIFSts, stsData);

   nErrCode = DSL_DRV_EventGenerate(
                 pContext, 0, DSL_ACCESSDIR_NA, DSL_XTUDIR_NA,
                 DSL_EVENT_S_SYSTEM_INTERFACE_STATUS,
                 (DSL_EventData_Union_t*)&stsData,
                 sizeof(DSL_SystemInterfaceConfigData_t));

   if( nErrCode < DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - Event(%d) generate failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
         DSL_EVENT_S_SYSTEM_INTERFACE_STATUS));
      return nErrCode;
   }

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_SystemInterfaceStatusUpdate, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return (nErrCode);
}
#endif /* INCLUDE_DSL_SYSTEM_INTERFACE */

DSL_Error_t DSL_DRV_DEV_LoopLengthStatusGet(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_LoopLengthStatusData_t loopLengthStatusData;
   DSL_boolean_t bLoopLengthValid = DSL_FALSE;
   DSL_Error_t nLoopLengthErrCode = DSL_SUCCESS;
   DSL_DEV_VersionCheck_t nVerCheck = 0;
   ACK_MFD_LooplengthGet_t sAck;

   /* Only for ADSL mode */
   if (!DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_ADSL))
   {
      DSL_DEBUG(DSL_DBG_WRN, (pContext, SYS_DBG_WRN"DSL[%02d]: WARNING - "
         "Hybrid/filter/looplength statistics supported only in ADSL mode"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   /* Check firmware version */
   nErrCode = DSL_DRV_VXX_FirmwareVersionCheck(pContext,
               DSL_MIN_FW_VERSION_MFD, &nVerCheck);

   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW version check failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return nErrCode;
   }

   do
   {
      if (nVerCheck < DSL_VERSION_EQUAL)
      {
         DSL_DEBUG(DSL_DBG_ERR, (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - "
            "Firmware version mismatch."DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         nLoopLengthErrCode = DSL_ERR_NOT_SUPPORTED_BY_FIRMWARE;
         break;
      }

      /* Read looplength from fw*/
      nErrCode = DSL_DRV_VRX_SendMsgLoopLengthGet(pContext, 
                                                          (DSL_uint8_t*)&sAck);
      if (nErrCode != DSL_SUCCESS)
      {
         nLoopLengthErrCode = DSL_ERR_DEVICE_NO_DATA;
         break;
      }

      loopLengthStatusData.nLength_Awg26 = (DSL_uint32_t)(sAck.Awg26Length);
      loopLengthStatusData.nLength_Awg24 = (DSL_uint32_t)(sAck.Awg24Length);

      DSL_CTX_WRITE(pContext, nErrCode, 
               showtimeMeasurement.loopLengthStatusData, loopLengthStatusData);

      bLoopLengthValid = DSL_TRUE;

   } while (0);

   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, 
                       showtimeMeasurement.bLoopLengthValid, bLoopLengthValid);

   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, 
                   showtimeMeasurement.nLoopLengthErrCode, nLoopLengthErrCode);

   return nErrCode;
}

DSL_Error_t DSL_DRV_VRX_ShowtimeMeasurementUpdate(
   DSL_Context_t *pContext,
   DSL_boolean_t bForceUpdate)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_HybridSelectionData_t hybridSelectionData;
   DSL_boolean_t bHybridReady = DSL_FALSE;
   DSL_boolean_t bHybridNeeded = DSL_FALSE;
   DSL_boolean_t bLoopLengthValid = DSL_FALSE;
   DSL_boolean_t bLoopLengthReady = DSL_FALSE;
   DSL_boolean_t bLoopLengthNeeded = DSL_FALSE;
#ifdef INCLUDE_DSL_FILTER_DETECTION
   DSL_FilterDetectionData_t filterDetectionData;
   DSL_boolean_t bMetric2Needed = DSL_FALSE;
   DSL_boolean_t bMetric2Ready = DSL_FALSE;
#endif /* #ifdef INCLUDE_DSL_FILTER_DETECTION*/
   DSL_DEV_VersionCheck_t nVerCheck = 0;
   ACK_MFD_ResultsGet_t sResultsAck;
   ACK_MFD_HybridInfoGet_t sHybridInfoAck;

   /* Only for ADSL mode */
   if (!DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_ADSL))
   {
      DSL_DEBUG(DSL_DBG_WRN, (pContext, SYS_DBG_WRN"DSL[%02d]: WARNING - "
         "Hybrid/filter/looplength statistics supported only in ADSL mode"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   /* Check firmware version */
   nErrCode = DSL_DRV_VXX_FirmwareVersionCheck(pContext,
               DSL_MIN_FW_VERSION_MFD, &nVerCheck);

   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW version check failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return nErrCode;
   }

   if (nVerCheck < DSL_VERSION_EQUAL)
   {
      DSL_DEBUG(DSL_DBG_WRN, (pContext, SYS_DBG_WRN"DSL[%02d]: WARNING - "
         "Firmware version mismatch." DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_SUCCESS;
   }

   DSL_CTX_READ(pContext, nErrCode, 
                 showtimeMeasurement.hybridSelectionData, hybridSelectionData);

   if ((bForceUpdate == DSL_TRUE) ||
       (hybridSelectionData.actualSelection.nHybridIndex == -1) ||
       (hybridSelectionData.secondBestSelection.nHybridIndex == -1))
   {
      bHybridNeeded = DSL_TRUE;
   }

   DSL_CTX_READ_SCALAR(pContext, nErrCode, 
                       showtimeMeasurement.bLoopLengthValid, bLoopLengthValid);
   bLoopLengthNeeded = !bLoopLengthValid;

#ifdef INCLUDE_DSL_FILTER_DETECTION
   DSL_CTX_READ(pContext, nErrCode, 
                 showtimeMeasurement.filterDetectionData, filterDetectionData);

   if ((bForceUpdate == DSL_TRUE) ||
       (filterDetectionData.nNonLinearEchoMetric2 == 128))
   {
      bMetric2Needed = DSL_TRUE;
   }
#endif /* #ifdef INCLUDE_DSL_FILTER_DETECTION*/

   if ((bHybridNeeded == DSL_FALSE) &&
#ifdef INCLUDE_DSL_FILTER_DETECTION
       (bMetric2Needed == DSL_FALSE) &&
#endif /* #ifdef INCLUDE_DSL_FILTER_DETECTION*/
       (bLoopLengthNeeded == DSL_FALSE))
   {
      return nErrCode;
   }

   /* Read mfd, hybrid, looplength ready bits from firmware */
   nErrCode = DSL_DRV_VRX_SendMsgMfdResultsGet(pContext, 
                                                   (DSL_uint8_t*)&sResultsAck);
   if (nErrCode != DSL_SUCCESS)
   {
      return nErrCode;
   }

   bHybridReady = (DSL_boolean_t)(sResultsAck.TrainingHybridInfoReady);
   bLoopLengthReady = (DSL_boolean_t)(sResultsAck.TrainingLoopLengthReady);
#ifdef INCLUDE_DSL_FILTER_DETECTION
   bMetric2Ready = (DSL_boolean_t)(sResultsAck.TrainingMetric2Ready);
#endif /* #ifdef INCLUDE_DSL_FILTER_DETECTION*/

   if ((bHybridNeeded == DSL_TRUE) && (bHybridReady == DSL_TRUE))
   {
      /* Read hybrid info from fw */
      nErrCode = DSL_DRV_VRX_SendMsgHybridInfoGet(pContext, 
                                                (DSL_uint8_t*)&sHybridInfoAck);

      if (nErrCode != DSL_SUCCESS)
      {
         return nErrCode;
      }

      /* Only modify the training/showtime related hybrid results (rd/mod/wr)*/
      hybridSelectionData.actualSelection.nHybridIndex = 
                                 (DSL_int16_t)(sHybridInfoAck.HybridMaxIndex);
      hybridSelectionData.actualSelection.nHybridMetric = 
                               (DSL_uint16_t)(sHybridInfoAck.HybridMaxMetric);
      hybridSelectionData.secondBestSelection.nHybridIndex = 
                             (DSL_int16_t)(sHybridInfoAck.Hybrid2ndBestIndex);
      hybridSelectionData.secondBestSelection.nHybridMetric = 
                           (DSL_uint16_t)(sHybridInfoAck.Hybrid2ndBestMetric);

      DSL_CTX_WRITE(pContext, nErrCode, 
                showtimeMeasurement.hybridSelectionData, hybridSelectionData);
   }

   if ((bLoopLengthNeeded == DSL_TRUE) && (bLoopLengthReady == DSL_TRUE))
   {
      nErrCode = DSL_DRV_DEV_LoopLengthStatusGet(pContext);
   }

#ifdef INCLUDE_DSL_FILTER_DETECTION
   if ((bMetric2Needed == DSL_TRUE) &&( bMetric2Ready == DSL_TRUE))
   {
      /* Read mfd metric2 from fw */
      nErrCode = DSL_DRV_VRX_SendMsgMfdResultsGet(pContext, 
                                                   (DSL_uint8_t*)&sResultsAck);
      if (nErrCode != DSL_SUCCESS)
      {
         return nErrCode;
      }

      filterDetectionData.nNonLinearEchoMetric2 = 
                                           (DSL_int16_t)(sResultsAck.M2Metric);
      DSL_CTX_WRITE(pContext, nErrCode, 
                 showtimeMeasurement.filterDetectionData, filterDetectionData);

   }
#endif /* #ifdef INCLUDE_DSL_FILTER_DETECTION*/

   return nErrCode;
}

#ifdef INCLUDE_DSL_FILTER_DETECTION
DSL_Error_t DSL_DRV_VRX_ShowtimeMeasurementCompleted(DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_LineStateValue_t nLineState = 
                                   DSL_LINESTATE_TEST_FILTERDETECTION_COMPLETE;
   DSL_FilterDetectionData_t filterDetectionData;
   DSL_HybridSelectionData_t hybridSelectionData;
   DSL_DEV_VersionCheck_t nVerCheck = 0;
   ACK_MFD_ResultsGet_t sResultsAck;
   ACK_MFD_HybridInfoGet_t sHybridInfoAck;

   /* Only for ADSL mode */
   if (!DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_ADSL))
   {
      DSL_DEBUG(DSL_DBG_WRN, (pContext, SYS_DBG_WRN"DSL[%02d]: WARNING - "
         "Hybrid/filter/looplength statistics supported only in ADSL mode"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   /* Check firmware version */
   nErrCode = DSL_DRV_VXX_FirmwareVersionCheck(pContext,
               DSL_MIN_FW_VERSION_MFD, &nVerCheck);

   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW version check failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return nErrCode;
   }

   do
   {
      if (nVerCheck < DSL_VERSION_EQUAL)
      {
         DSL_DEBUG(DSL_DBG_WRN, (pContext, SYS_DBG_WRN"DSL[%02d]: WARNING - "
            "Firmware version mismatch."DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return DSL_SUCCESS;
      }

      /* --------------------- Filter Detection handling --------------------*/
      /* Read mfd statistic, hybrid ready bit from fw */
      nErrCode = DSL_DRV_VRX_SendMsgMfdResultsGet(pContext, 
                                                   (DSL_uint8_t*)&sResultsAck);

      if (nErrCode != DSL_SUCCESS)
      {
         break;
      }

      filterDetectionData.nNoiseLevel = (DSL_uint8_t)(sResultsAck.M1QlnLevel);
      filterDetectionData.nBridgeTapFlag = 
                            (DSL_BridgeTapFlag_t)(sResultsAck.BTRange);
      filterDetectionData.bNoConfidence = 
                                   (DSL_boolean_t)(sResultsAck.M1NoConfidence);
      filterDetectionData.nNonLinearEchoMetric1 = 
                                           (DSL_int16_t)(sResultsAck.M1metric);

      if (sResultsAck.M1metric & (1 << 11))
      {
         filterDetectionData.nNonLinearEchoMetric1 |= 0xF000;
      }
      /* This value will be updated later on reaching showtime. Initialize with
         special value here to avoid inconsistent metric 1/2 relations. */
      filterDetectionData.nNonLinearEchoMetric2 = 128;

      DSL_CTX_WRITE(pContext, nErrCode, 
                 showtimeMeasurement.filterDetectionData, filterDetectionData);

      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, 
                      showtimeMeasurement.bFilterDetectionCompleted, DSL_TRUE);
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, 
                        showtimeMeasurement.bFilterDetectionActive, DSL_FALSE);

      /* --------------------- Hybrid Selection handling --------------------*/
      if (sResultsAck.TrainingHybridInfoReady)
      { 
         /* Read hybrid info from fw */
         nErrCode = DSL_DRV_VRX_SendMsgHybridInfoGet(pContext, 
                                                (DSL_uint8_t*)&sHybridInfoAck);
         if (nErrCode != DSL_SUCCESS)
         {
            break;
         }
   
         /* Only modify the filter detection related
                                                hybrid results (rd/mod/wr) */
         DSL_CTX_READ(pContext, nErrCode, 
                 showtimeMeasurement.hybridSelectionData, hybridSelectionData);
   
         hybridSelectionData.fdActualSelection.nHybridIndex = 
                                  (DSL_int16_t)(sHybridInfoAck.HybridMaxIndex);
         hybridSelectionData.fdActualSelection.nHybridMetric = 
                                (DSL_uint16_t)(sHybridInfoAck.HybridMaxMetric);
         hybridSelectionData.fdSecondBestSelection.nHybridIndex = 
                              (DSL_int16_t)(sHybridInfoAck.Hybrid2ndBestIndex);
         hybridSelectionData.fdSecondBestSelection.nHybridMetric = 
                            (DSL_uint16_t)(sHybridInfoAck.Hybrid2ndBestMetric);
   
         DSL_CTX_WRITE(pContext, nErrCode, 
                 showtimeMeasurement.hybridSelectionData, hybridSelectionData);
      }
   } while (0);

   /* Filter detection relevant data updated within API context (only if CMV
      processing was ok). Generate line state event in any case. */
   nErrCode =  DSL_DRV_EventGenerate(pContext, 0,
      DSL_ACCESSDIR_NA, DSL_XTUDIR_NA, DSL_EVENT_S_LINE_STATE,
      (DSL_EventData_Union_t*)&nLineState, sizeof(DSL_LineStateData_t));

   return nErrCode;
}
#endif /* INCLUDE_DSL_FILTER_DETECTION */

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device.h'
*/
#ifdef INCLUDE_DSL_SYSTEM_INTERFACE
DSL_Error_t DSL_DRV_DEV_SystemInterfaceConfigSet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_SystemInterfaceConfig_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_VNX_FwVersion_t  sFwVersion = {0};

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_SystemInterfaceConfigSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Get FW info*/
   DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.fwFeatures, sFwVersion);

   if ((sFwVersion.nPlatform == 5) && (sFwVersion.nFeatureSet >= 4)
      && (sFwVersion.nMajor >= 0)
      && (sFwVersion.nMinor >= 8 || sFwVersion.nMajor >= 1)
      && DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_ADSL))
   {
      /* Copy System Configuration data to internal VRx device context*/
      DSL_CTX_WRITE(pContext, nErrCode, pDevCtx->data.deviceCfg.sysCIF, pData->data);
   }
   else
   {
      nErrCode = DSL_WRN_CONFIG_PARAM_IGNORED;
   }

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_SystemInterfaceConfigSet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return (nErrCode);
}
#endif /* INCLUDE_DSL_SYSTEM_INTERFACE*/

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device.h'
*/
#ifdef INCLUDE_DSL_SYSTEM_INTERFACE
DSL_Error_t DSL_DRV_DEV_SystemInterfaceConfigGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_SystemInterfaceConfig_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_SystemInterfaceConfigGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Get System Configuration from the internal VRx device context*/
   DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.deviceCfg.sysCIF, pData->data);

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_SystemInterfaceConfigGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return (nErrCode);
}
#endif /* INCLUDE_DSL_SYSTEM_INTERFACE*/

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device.h'
*/
DSL_Error_t DSL_DRV_DEV_G994VendorIdFirmwareUpdate(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_VNX_FwVersion_t  sFwVersion = {0};
   DSL_uint16_t nVendor = 0;

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_G994VendorIdFirmwareUpdate"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Get FW info*/
   DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.fwFeatures, sFwVersion);

   nVendor |= ((DSL_uint16_t)sFwVersion.nPlatform   & 0x000F) << 12;
   nVendor |= ((DSL_uint16_t)sFwVersion.nFeatureSet & 0x000F) << 8;
   nVendor |= ((DSL_uint16_t)sFwVersion.nMajor      & 0x000F) << 4;
   nVendor |= ((DSL_uint16_t)sFwVersion.nMinor      & 0x000F);

   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, lineInventoryNe.G994VendorID[6],
      (DSL_uint8_t) 0xFF & (nVendor >> 8));
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, lineInventoryNe.G994VendorID[7],
      (DSL_uint8_t) 0xFF & nVendor);

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_G994VendorIdFirmwareUpdate, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device.h'
*/
#ifdef INCLUDE_DSL_SYSTEM_INTERFACE
DSL_Error_t DSL_DRV_DEV_SystemInterfaceStatusGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_SystemInterfaceStatus_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_SystemInterfaceStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   if (pContext->pDevCtx->data.bSystemIfStatusValid == DSL_TRUE)
   {
      /* Get System Configuration from the internal VRx device context. */
      DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.sysCIFSts, pData->data);
   }
   else
   {
      nErrCode = DSL_ERR_DEVICE_NO_DATA;
   }

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_SystemInterfaceStatusGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return (nErrCode);
}
#endif /* INCLUDE_DSL_SYSTEM_INTERFACE*/

#ifdef INCLUDE_DSL_RESOURCE_STATISTICS
DSL_Error_t DSL_DRV_DEV_ResourceUsageStatisticsGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_OUT DSL_ResourceUsageStatisticsData_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   /* Static memory*/
   pData->staticMemUsage += sizeof(ChipString);
   pData->staticMemUsage += sizeof(DSL_XtseAdslA);
   pData->staticMemUsage += sizeof(DSL_XtseAdslB);
   pData->staticMemUsage += sizeof(DSL_XtseVdsl2);

   /* Dynamic memory*/
   pData->dynamicMemUsage = 0;

   return nErrCode;
}
#endif /* INCLUDE_DSL_RESOURCE_STATISTICS*/

#ifdef INCLUDE_PILOT_TONES_STATUS
DSL_Error_t DSL_DRV_DEV_PilotTonesStatusGet(
   DSL_Context_t *pContext,
   DSL_PilotTonesStatus_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint16_t i;
   ACK_PilotTonesGet_t sAck;

   nErrCode = DSL_DRV_VRX_SendMsgPilotTonesGet(pContext, (DSL_uint8_t*)(&sAck));
   if (nErrCode != DSL_SUCCESS)
   {
      return nErrCode;
   }

   memset(pData->data.nPilotTone, 0x0, sizeof(pData->data.nPilotTone));

   pData->data.nNumData = (DSL_uint8_t)sAck.numPilot;

   for (i = 0; (i < sAck.numPilot) && (i < MAX_NUMBER_OF_PILOT_TONES); i++)
   {
      pData->data.nPilotTone[i] = sAck.pilot[i];
   }

   return nErrCode;
}
#endif /* #ifdef INCLUDE_PILOT_TONES_STATUS*/

#endif /* #ifdef INCLUDE_DSL_CPE_API_VRX*/

