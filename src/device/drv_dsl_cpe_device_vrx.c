/******************************************************************************

                              Copyright (c) 2014
                            Lantiq Deutschland GmbH

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
static VRX_CodeString_t ChipString[]=
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
   DSL_AccessDir_t nDirection,
   DSL_ReTxCounters_t *pCounters);

#ifdef INCLUDE_DSL_SYSTEM_INTERFACE
static DSL_Error_t DSL_DRV_VRX_SystemInterfaceStatusGet(
   DSL_Context_t *pContext);
#endif /* INCLUDE_DSL_SYSTEM_INTERFACE */

#undef DSL_DBG_BLOCK
#define DSL_DBG_BLOCK DSL_DBG_MULTIMODE
static DSL_Error_t DSL_DRV_VRX_CamRetryReset(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.nCamReinits, 0);
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.nCamFwReinits, 0);
#ifdef INCLUDE_DEVICE_EXCEPTION_CODES
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.nCamGhsReinits, 0);
#endif /* INCLUDE_DEVICE_EXCEPTION_CODES*/

   DSL_DEBUG(DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: CAM - Retry counters reset: "
                 "nCamReinits = 0, nCamFwReinits = 0"
#ifdef INCLUDE_DEVICE_EXCEPTION_CODES
                 ", nCamGhsReinits = 0"
#endif /* INCLUDE_DEVICE_EXCEPTION_CODES*/
                 DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

/*CAM*/
/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_CamFsmStateSet(
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

   nErrCode = DSL_DRV_VRX_CamRetryReset(pContext);

   return(nErrCode);
}

DSL_DEV_CamStates_t DSL_DRV_VRX_CamFsmStateGet(
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
   please refer to the description in the header file 'drv_dsl_cpe_device_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_CamExceptionHandle(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_DEV_CamStates_t nCurrentState = DSL_DRV_VRX_CamFsmStateGet(pContext);
   DSL_uint8_t nCamReinits = 0;
   DSL_uint8_t nCamFwReinits = 0;
   DSL_boolean_t bCamDualPortModeSteady = DSL_FALSE;
   DSL_G997_LineInitStatusData_t lineInitStatus = {LINIT_UNKNOWN, LINIT_SUB_NONE};
#ifdef INCLUDE_DEVICE_EXCEPTION_CODES
   DSL_LastExceptionCodesData_t LastExceptionCodes;
   DSL_uint8_t nCamGhsReinits = 0;
#endif /* INCLUDE_DEVICE_EXCEPTION_CODES*/
   DSL_boolean_t bFsmStatusChange = DSL_FALSE;
   DSL_uint8_t i, nAtse = 0, nAtseCurr = 0, nVtse = 0, nXtse1 = 0;
   DSL_MultimodeFsmConfigData_t nCamOpt;
   DSL_ActivationFsmConfigData_t nActivationCfg;
   DSL_ActivationMode_t nActivationModeCurr;
   DSL_boolean_t bT1_413 = DSL_FALSE, bNextModeChange = DSL_FALSE;
   DSL_uint8_t nSpar1 = 0, nSpar2 = 0, nSpar3 = 0, nSpar4 = 0, nSpar5 = 0;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_CamExceptionHandle"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - current state %d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nCurrentState));

   DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.nCamStatus, nCamOpt);
   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - current Next Mode %d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nCamOpt.nNextMode));

   DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.bCamDualPortModeSteady, bCamDualPortModeSteady);

   if (nCurrentState != DSL_CAM_INIT && !bCamDualPortModeSteady)
   {
      if (nCurrentState == DSL_CAM_ADSL_SINGLE)
      {
         /* Update XTSE status information */
         nErrCode = DSL_DRV_VRX_SendMsgXtseStatusGet(pContext);
         if (nErrCode != DSL_SUCCESS)
         {
            DSL_DEBUG(DSL_DBG_ERR, (pContext,
               SYS_DBG_ERR"DSL[%02d]: ERROR - XTSE status get failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
            return nErrCode;
         }
         DSL_DEBUG(DSL_DBG_MSG, (pContext, SYS_DBG_MSG
            "DSL[%02d]: CAM - Update current Activation Start Mode from"
            " XTSE status"DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         /* Read XTSE1 bits (status) */
         DSL_CTX_READ(pContext, nErrCode, xtseCurr[0], nXtse1);

         /* Update the current activation mode value */
         nActivationModeCurr = (nXtse1 & XTSE_1_01_A_T1_413) ?
                           DSL_ACT_MODE_ANSI_T1413 : DSL_ACT_MODE_GHS;

         /* Save Start mode*/
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode,
            pDevCtx->data.nActivationCfg.nActivationMode, nActivationModeCurr);
         DSL_DEBUG(DSL_DBG_MSG, (pContext, SYS_DBG_MSG
            "DSL[%02d]: CAM - Set Activation Start Mode %u"DSL_DRV_CRLF,
            DSL_DEV_NUM(pContext), nActivationModeCurr));
      }

      /* Get reinit counters*/
      DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.nCamReinits, nCamReinits);
      DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.nCamFwReinits, nCamFwReinits);
#ifdef INCLUDE_DEVICE_EXCEPTION_CODES
      DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.nCamGhsReinits, nCamGhsReinits);
#endif /* INCLUDE_DEVICE_EXCEPTION_CODES*/

      /* Get Line Init info*/
      DSL_CTX_READ(pContext, nErrCode, lineInitStatus, lineInitStatus);

#ifdef INCLUDE_DEVICE_EXCEPTION_CODES
      memset(&LastExceptionCodes, 0x0, sizeof(DSL_LastExceptionCodesData_t));
      /* Get fw failure code */
      DSL_CTX_READ(pContext, nErrCode, LastExceptionCodes, LastExceptionCodes);
#endif /* INCLUDE_DEVICE_EXCEPTION_CODES*/

      if (lineInitStatus.nLineInitSubStatus == LINIT_SUB_OPP_MODE &&
          nCurrentState != DSL_CAM_VDSL_DUAL)
      {
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - LINIT_SUB_OPP_MODE"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         bFsmStatusChange = DSL_TRUE;
      }
#if defined(INCLUDE_DSL_BONDING) && (DSL_DRV_LINES_PER_DEVICE == 2)
      else if (lineInitStatus.nLineInitSubStatus == LINIT_SUB_PORT_MODE &&
               nCurrentState == DSL_CAM_VDSL_DUAL)
      {
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - LINIT_SUB_PORT_MODE"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         bFsmStatusChange = DSL_TRUE;
      }
      else if (lineInitStatus.nLineInitSubStatus == LINIT_SUB_TIMEOUT &&
               nCurrentState == DSL_CAM_VDSL_DUAL)
      {
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - LINIT_SUB_TIMEOUT"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         bFsmStatusChange = DSL_TRUE;
      }
#endif
      else
      {
         /* Update FW reinit counter*/
         if ((lineInitStatus.nLineInitSubStatus == LINIT_SUB_FW_RETRY) ||
             (lineInitStatus.nLineInitSubStatus == LINIT_SUB_S_PP_DRIVER))
         {
            nCamFwReinits++;
            DSL_DEBUG(DSL_DBG_MSG,
               (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - nCamFwReinits=%u"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nCamFwReinits));
         }
#ifdef INCLUDE_DEVICE_EXCEPTION_CODES
         else if ((LastExceptionCodes.nFwErrorCode1 >= 300) &&
                  (LastExceptionCodes.nFwErrorCode1 < 400))
         {
            /* Update GHS reinit counter*/
            nCamGhsReinits++;
            DSL_DEBUG(DSL_DBG_MSG,
               (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - nCamGhsReinits=%u"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nCamGhsReinits));
         }
#endif /* INCLUDE_DEVICE_EXCEPTION_CODES*/
         else {
            /* Update general reinit counter*/
            nCamReinits++;
            DSL_DEBUG(DSL_DBG_MSG,
               (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - nCamReinits=%u"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nCamReinits));
         }

         if ((nCamReinits >= DSL_CAM_MAX_REINITS) || (nCamFwReinits >= DSL_CAM_MAX_FW_REINITS)
#ifdef INCLUDE_DEVICE_EXCEPTION_CODES
            || (nCamGhsReinits >= DSL_CAM_MAX_GHS_REINITS)
#endif /* INCLUDE_DEVICE_EXCEPTION_CODES*/
            )
         {
            DSL_DEBUG(DSL_DBG_MSG,
               (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - Re-init limit reached, "
               "nCamReinits=%u nCamFwReinits=%u"
#ifdef INCLUDE_DEVICE_EXCEPTION_CODES
               " nCamGhsReinits=%u"
#endif /* INCLUDE_DEVICE_EXCEPTION_CODES*/
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nCamReinits, nCamFwReinits
#ifdef INCLUDE_DEVICE_EXCEPTION_CODES
               , nCamGhsReinits
#endif /* INCLUDE_DEVICE_EXCEPTION_CODES*/
                ));

            bFsmStatusChange = DSL_TRUE;

            /* Toggle Activation Start Mode */
            if (lineInitStatus.nLineInitSubStatus == LINIT_SUB_NO_COMMON_MODE &&
                nCurrentState == DSL_CAM_ADSL_SINGLE)
            {
               DSL_DEBUG(DSL_DBG_MSG,
                  (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - LINIT_SUB_NO_COMMON_MODE"
                  DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

               DSL_CTX_READ(pContext, nErrCode,
                                 pDevCtx->data.nActivationCfg, nActivationCfg);
               DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.bT1_413, bT1_413);

               if ( (nActivationCfg.nActivationSequence == DSL_ACT_SEQ_AUTO ||
                     nActivationCfg.nActivationSequence == DSL_ACT_SEQ_NON_STD)
                     && bT1_413)
               {
                  switch (nActivationCfg.nActivationMode)
                  {
                     case DSL_ACT_MODE_GHS:
                        nActivationCfg.nActivationMode = DSL_ACT_MODE_ANSI_T1413;
                        break;

                     case DSL_ACT_MODE_ANSI_T1413:
                     default:
                        nActivationCfg.nActivationMode = DSL_ACT_MODE_GHS;
                        break;
                  }

                  /* Save Start mode*/
                  DSL_CTX_WRITE(pContext, nErrCode,
                                pDevCtx->data.nActivationCfg, nActivationCfg);
                  DSL_DEBUG(DSL_DBG_MSG, (pContext, SYS_DBG_MSG
                     "DSL[%02d]: CAM - Toggle Activation Start Mode (%u)"DSL_DRV_CRLF,
                     DSL_DEV_NUM(pContext), nActivationCfg.nActivationMode));
               }
            }
         }
      }

      /* Update reinit counters*/
      DSL_CTX_WRITE(pContext, nErrCode, pDevCtx->data.nCamReinits, nCamReinits);
      DSL_CTX_WRITE(pContext, nErrCode, pDevCtx->data.nCamFwReinits, nCamFwReinits);
#ifdef INCLUDE_DEVICE_EXCEPTION_CODES
      DSL_CTX_WRITE(pContext, nErrCode, pDevCtx->data.nCamGhsReinits, nCamGhsReinits);
#endif /* INCLUDE_DEVICE_EXCEPTION_CODES*/

      if (bFsmStatusChange)
      {
         /* get the ADSL bits (config) */
         for (i = 0; i < DSL_G997_NUM_XTSE_OCTETS - 1; i++)
         {
            DSL_CTX_READ_SCALAR(pContext, nErrCode, xtseCfg[i], nAtseCurr);
            nAtse |= nAtseCurr;
         }

         /* get the VDSL bits (config) */
         DSL_CTX_READ_SCALAR(pContext, nErrCode, xtseCfg[7], nVtse);

         switch (nCurrentState)
         {
            case DSL_CAM_VDSL_DUAL:
               nCurrentState = DSL_CAM_VDSL_SINGLE;
               break;

            case DSL_CAM_ADSL_SINGLE:
               if (nAtse && nVtse)
               {
                  nCurrentState = DSL_CAM_INIT;
                  nCamOpt.nNextMode = DSL_FW_TYPE_VDSL;
                  bNextModeChange = DSL_TRUE;
               }
               else
               {
                  nCurrentState = DSL_CAM_ADSL_SINGLE;
               }
               break;

            case DSL_CAM_VDSL_SINGLE:
               if (nAtse && nVtse)
               {
                  nCurrentState = DSL_CAM_INIT;
                  nCamOpt.nNextMode = DSL_FW_TYPE_ADSL;
                  bNextModeChange = DSL_TRUE;
               }
               else
               {
                  nCurrentState = DSL_CAM_INIT;
               }
               break;

            default:
               break;
         }

         if (bNextModeChange)
         {
            DSL_CTX_WRITE(pContext, nErrCode, pDevCtx->data.nCamStatus, nCamOpt);
            DSL_DEBUG(DSL_DBG_MSG,
               (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - Set Next Mode %i"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nCamOpt.nNextMode));
         }
         else
         {
            DSL_DEBUG(DSL_DBG_MSG,
               (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - Use current Next Mode %i"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nCamOpt.nNextMode));
         }

         nErrCode = DSL_DRV_VRX_CamFsmStateSet(pContext, nCurrentState);
         if (nErrCode != DSL_SUCCESS)
         {
            DSL_DEBUG(DSL_DBG_MSG, (pContext,
               SYS_DBG_MSG"DSL[%02d]: CAM - FSM state set failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            return nErrCode;
         }
      }

      /* Get SPAR info */
      DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->data.nSpar1, nSpar1);
      DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->data.nSpar2, nSpar2);
      DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->data.nSpar3, nSpar3);
      DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->data.nSpar4, nSpar4);
      DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->data.nSpar5, nSpar5);
      DSL_DEBUG(DSL_DBG_WRN, (pContext,
         SYS_DBG_WRN"DSL[%02d]: CAM - Spar1 0x%X, Spar2 0x%X, Spar3 0x%X, Spar4 0x%X, Spar5 0x%x"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nSpar1, nSpar2, nSpar3, nSpar4, nSpar5));
   }

   /* Do a hard-reboot or soft-reboot depending on the current Port mode*/
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, bSoftRestart,
                        nCurrentState == DSL_CAM_VDSL_DUAL ? DSL_TRUE : DSL_FALSE);

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_CamExceptionHandle"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return(nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_CamFwLoadNeededCheck(
   DSL_Context_t *pContext,
   DSL_boolean_t *bDownloadRequired,
   DSL_VRX_FwMode_t *nFwMode)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_DEV_CamStates_t nCamState = DSL_DRV_VRX_CamFsmStateGet(pContext);

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_CamFwLoadNeededCheck"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   *nFwMode = FWMODE_VDSL;
   *bDownloadRequired = DSL_FALSE;

   switch (nCamState)
   {
   case DSL_CAM_ADSL_SINGLE:
      *nFwMode = FWMODE_ADSL;
      break;
   case DSL_CAM_VDSL_SINGLE:
   case DSL_CAM_VDSL_DUAL:
      *nFwMode = FWMODE_VDSL;
      break;
   default:
      DSL_DEBUG(DSL_DBG_WRN, (pContext,
         SYS_DBG_WRN"DSL[%02d]: DSL_VRX_CamFwLoad unhandled state %d"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nCamState));
      break;
   }

   /* now check if we need a new download */
   if (*nFwMode == FWMODE_ADSL)
   {
      if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
      {
         *bDownloadRequired = DSL_TRUE;
      }
   }
   else
   {
      if (!DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
      {
         *bDownloadRequired = DSL_TRUE;
      }
   }

   DSL_DEBUG(DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: CAM - FW reload needed=%d, type=%s"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
      *bDownloadRequired, ((*nFwMode == FWMODE_VDSL)?"VDSL":"ADSL")));

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_CamFwLoadNeededCheck"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return(nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_CamFsmUpdate(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_VRX_FwMode_t nFwType;
   DSL_boolean_t bDownload = DSL_FALSE;
   DSL_DEV_CamStates_t nCamStateNew = DSL_CAM_VDSL_DUAL,
      nCamStateCurrent = DSL_DRV_VRX_CamFsmStateGet(pContext);
   DSL_uint8_t i, nAtse = 0, nAtseCurr = 0, nVtse = 0, nXtse1 = 0;
   DSL_boolean_t bNextModeChange = DSL_FALSE;
#if defined(INCLUDE_DSL_BONDING) && (DSL_DRV_LINES_PER_DEVICE == 2)
   DSL_boolean_t bPafEnable = DSL_FALSE;
   DSL_PortMode_t BndPortModeSync = DSL_PORT_MODE_NA;
#endif
   DSL_MultimodeFsmConfigData_t nCamOpt;
   DSL_boolean_t bCamSinglePortModeSteady = DSL_FALSE,
      bRemember = DSL_FALSE, bT1_413 = DSL_FALSE;
   DSL_ActivationMode_t nActivationMode;

   DSL_CHECK_CTX_POINTER(pContext);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_CamFsmUpdate."
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   if (DSL_DRV_VRX_CamFsmStateGet(pContext) == DSL_CAM_DISABLED)
   {
      DSL_DRV_VRX_CamFsmEnable(pContext, DSL_TRUE);
   }

   DSL_CTX_WRITE_SCALAR(pContext, nErrCode,
             pDevCtx->data.bCamDualPortModeSteady, DSL_FALSE);

   DSL_CTX_READ_SCALAR(pContext, nErrCode,
             pDevCtx->data.bCamSinglePortModeSteady, bCamSinglePortModeSteady);
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode,
             pDevCtx->data.bCamSinglePortModeSteady, DSL_FALSE);

   /* Check Fail after showtime case */
   if (bCamSinglePortModeSteady)
   {
      /* Remebmer last showtime settings enable/disable ? */
      DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->data.bRemember, bRemember);

      DSL_DEBUG(DSL_DBG_MSG,
         (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - Activation after showtime,"
          " bRemember %u" DSL_DRV_CRLF, DSL_DEV_NUM(pContext), bRemember));

      if (bRemember)
      {
         /* Update XTSE status information*/
         nErrCode = DSL_DRV_VRX_SendMsgXtseStatusGet(pContext);
         if (nErrCode != DSL_SUCCESS)
         {
            DSL_DEBUG(DSL_DBG_ERR, (pContext,
               SYS_DBG_ERR"DSL[%02d]: ERROR - XTSE status get failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
            return nErrCode;
         }
         DSL_DEBUG(DSL_DBG_MSG, (pContext, SYS_DBG_MSG
            "DSL[%02d]: CAM - Update current Activation Start Mode from"
            " XTSE status"DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         /* Read XTSE1 bits (status) */
         DSL_CTX_READ(pContext, nErrCode, xtseCurr[0], nXtse1);

         /* Update the current activation mode value */
         nActivationMode = (nXtse1 & XTSE_1_01_A_T1_413) ?
            DSL_ACT_MODE_ANSI_T1413 : DSL_ACT_MODE_GHS;

         DSL_CTX_WRITE_SCALAR(pContext, nErrCode,
            pDevCtx->data.nActivationCfg.nActivationMode, nActivationMode);

         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - Set Activation Start Mode %u"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nActivationMode));

         DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.nCamStatus, nCamOpt);

         DSL_DEBUG(DSL_DBG_MSG,
         (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - Use current Next Mode %u"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nCamOpt.nNextMode));
      }
      else
      {
         DSL_DEBUG(DSL_DBG_MSG,
         (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - Use initial settings"
          DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         /* Get initial NextMode */
         DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.nCamConfig, nCamOpt);
         DSL_CTX_WRITE(pContext, nErrCode, pDevCtx->data.nCamStatus, nCamOpt);
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - Set Next Mode %u"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nCamOpt.nNextMode));
      }
   }

   DSL_DEBUG(DSL_DBG_MSG, (pContext, SYS_DBG_MSG
      "DSL[%02d]: CAM - Update T1.413 from XTSE configuration"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* get the XTSE1 bits (config) */
   DSL_CTX_READ_SCALAR(pContext, nErrCode, xtseCfg[0], nXtse1);

   /* update T1.413 within reset state */
   bT1_413 = (nXtse1 & XTSE_1_01_A_T1_413) ? DSL_TRUE : DSL_FALSE;
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.bT1_413, bT1_413);

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - Set T1.413 %i"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), bT1_413));

   /* get the ADSL bits (config) */
   for (i = 0; i < DSL_G997_NUM_XTSE_OCTETS - 1; i++)
   {
      DSL_CTX_READ_SCALAR(pContext, nErrCode, xtseCfg[i], nAtseCurr);
      nAtse |= nAtseCurr;
   }

   /* get the VDSL bits (config) */
   DSL_CTX_READ_SCALAR(pContext, nErrCode, xtseCfg[7], nVtse);

   /* xts config could be changed manually */
   /* check corresponding to the current state */
   switch (nCamStateCurrent)
   {
      case DSL_CAM_VDSL_DUAL:
      case DSL_CAM_VDSL_SINGLE:
         /* nAtse only & VDSL */
         if (nAtse && !nVtse)
         {
            nCamStateCurrent = DSL_CAM_INIT;
            nCamOpt.nNextMode = DSL_FW_TYPE_ADSL;
            bNextModeChange = DSL_TRUE;
         }
         break;

      case DSL_CAM_ADSL_SINGLE:
         /* nVtse only & ADSL */
         if (!nAtse && nVtse)
         {
            nCamStateCurrent = DSL_CAM_INIT;
            nCamOpt.nNextMode = DSL_FW_TYPE_VDSL;
            bNextModeChange = DSL_TRUE;
         }
         break;

      default:
         break;
   }

   if (bNextModeChange)
   {
      nErrCode = DSL_DRV_VRX_CamFsmStateSet(pContext, nCamStateCurrent);
      if (nErrCode != DSL_SUCCESS)
      {
         DSL_DEBUG(DSL_DBG_MSG, (pContext,
            SYS_DBG_MSG"DSL[%02d]: CAM - FSM state set failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return nErrCode;
      }

      DSL_CTX_WRITE(pContext, nErrCode, pDevCtx->data.nCamStatus, nCamOpt);

      DSL_DEBUG(DSL_DBG_MSG,
         (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - Set Next Mode %i"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nCamOpt.nNextMode));
   }

   /* In INIT state modify the CPE Auto mode FSM
      according to the NextMode */
   if (nCamStateCurrent == DSL_CAM_INIT)
   {
      DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.nCamStatus, nCamOpt);
      if (nCamOpt.nNextMode == DSL_FW_TYPE_ADSL)
      {
         nCamStateNew = DSL_CAM_ADSL_SINGLE;
         /* change Port Mode to Single*/
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.nPortMode,
                                                    DSL_PORT_MODE_SINGLE);
      }
      else
      {
#if defined(INCLUDE_DSL_BONDING) && (DSL_DRV_LINES_PER_DEVICE == 2)
      /* Read Port Mode sync data*/
      DSL_CTX_READ(pContext, nErrCode, BndPortModeSync, BndPortModeSync);
      /* ...and check if it was specified */
      if (BndPortModeSync != DSL_PORT_MODE_NA)
      {
         /* reset sync data*/
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, BndPortModeSync, DSL_PORT_MODE_NA);

         nCamStateNew = BndPortModeSync == DSL_PORT_MODE_DUAL ?
                              DSL_CAM_VDSL_DUAL : DSL_CAM_VDSL_SINGLE;
         DSL_CTX_WRITE(pContext, nErrCode, pDevCtx->data.nPortMode, BndPortModeSync);

          DSL_DEBUG(DSL_DBG_MSG,
               (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - Port Mode Sync, next mode %d"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext), BndPortModeSync));
      }
      else
      {
         DSL_CTX_READ(pContext, nErrCode, BndConfig.bPafEnable, bPafEnable);
         if (bPafEnable)
         {
            nCamStateNew = DSL_CAM_VDSL_DUAL;
            /* change Port Mode to Double*/
            DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.nPortMode,
                                                         DSL_PORT_MODE_DUAL);
         }
         else
         {
            nCamStateNew = DSL_CAM_VDSL_SINGLE;
            /* change Port Mode to Single*/
            DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.nPortMode,
                                                         DSL_PORT_MODE_SINGLE);
         }
      }
#else
         nCamStateNew = DSL_CAM_VDSL_SINGLE;
         /* change Port Mode to Single*/
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.nPortMode,
                                                         DSL_PORT_MODE_SINGLE);
#endif
         /* Reset hybrid,looplength,mfd context */
         /* hybrid,looplength,mfd is not supported by VDSL firmware */
         DSL_DRV_HybridContextReset(pContext);
      }

      /* Set New CAM state*/
      nErrCode = DSL_DRV_VRX_CamFsmStateSet(pContext, nCamStateNew);
      if (nErrCode != DSL_SUCCESS)
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: CAM - FSM state (%u) set failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nCamStateNew));

         return DSL_ERROR;
      }
   }
   else
   {
      DSL_DEBUG(DSL_DBG_MSG,
         (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - Use current state %u"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nCamStateCurrent));

      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.nPortMode,
                              nCamStateCurrent == DSL_CAM_VDSL_DUAL ?
                                 DSL_PORT_MODE_DUAL : DSL_PORT_MODE_SINGLE);
   }

   /* check the loaded FW and load a different one if needed */
   nErrCode = DSL_DRV_VRX_CamFwLoadNeededCheck(pContext, &bDownload, &nFwType);
   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: CAM - FW check failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERROR;
   }

   /* Check if it is neccessary to reload the FW binary*/
   if (bDownload)
   {
      if (nFwType == FWMODE_VDSL)
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
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_CamFsmUpdate"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return(nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_CamFsmEnable(
   DSL_Context_t *pContext,
   const DSL_boolean_t bEnable)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   if (bEnable)
   {
      /* if already enabled, this causes a reset of the state machine */
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.nCamState, DSL_CAM_INIT);
      nErrCode = DSL_DRV_VRX_CamRetryReset(pContext);
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

DSL_Error_t DSL_DRV_VRX_CamTrainingTimeoutHandle(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_DEV_CamStates_t nCamState = DSL_DRV_VRX_CamFsmStateGet(pContext);
   DSL_MultimodeFsmConfigData_t nCamOpt;
   DSL_uint8_t i, nAtse = 0, nAtseCurr = 0, nVtse = 0;
   DSL_ActivationFsmConfigData_t nActivationCfg;
   DSL_boolean_t bT1_413 = DSL_FALSE;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_CamTrainingTimeoutHandle."
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - current state %d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nCamState));

   DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.nCamStatus, nCamOpt);
   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - current Next Mode %d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nCamOpt.nNextMode));

   /* get the ADSL bits */
   for (i = 0; i < DSL_G997_NUM_XTSE_OCTETS - 1; i++)
   {
      DSL_CTX_READ_SCALAR(pContext, nErrCode, xtseCfg[i], nAtseCurr);
      nAtse |= nAtseCurr;
   }

   /* get the VDSL bits */
   DSL_CTX_READ_SCALAR(pContext, nErrCode, xtseCfg[7], nVtse);

   switch (nCamState)
   {
      case DSL_CAM_VDSL_SINGLE:
      case DSL_CAM_VDSL_DUAL:
         DSL_CTX_READ(pContext, nErrCode,
                                 pDevCtx->data.nActivationCfg, nActivationCfg);
         DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.bT1_413, bT1_413);

         if ( (nActivationCfg.nActivationSequence == DSL_ACT_SEQ_AUTO ||
               nActivationCfg.nActivationSequence == DSL_ACT_SEQ_NON_STD)
               && bT1_413)
         {
            /* Set Activation mode to ANSI-T1.413 */
            DSL_CTX_WRITE_SCALAR(pContext, nErrCode,
               pDevCtx->data.nActivationCfg.nActivationMode, DSL_ACT_MODE_ANSI_T1413);
            DSL_DEBUG(DSL_DBG_MSG,
               (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - Set Activation Start Mode %i"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext), DSL_ACT_MODE_ANSI_T1413));
         }
         nCamOpt.nNextMode = (nAtse && nVtse) ? DSL_FW_TYPE_ADSL:
                                                DSL_FW_TYPE_VDSL;
         break;

      case DSL_CAM_ADSL_SINGLE:
         nCamOpt.nNextMode = (nAtse && nVtse) ? DSL_FW_TYPE_VDSL:
                                                DSL_FW_TYPE_ADSL;
         break;

      default:
         break;
   }

   DSL_CTX_WRITE(pContext, nErrCode, pDevCtx->data.nCamStatus, nCamOpt);
   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - Set Next Mode %i"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nCamOpt.nNextMode));

   nErrCode = DSL_DRV_VRX_CamFsmStateSet(pContext, DSL_CAM_INIT);
   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG(DSL_DBG_MSG, (pContext,
         SYS_DBG_MSG"DSL[%02d]: CAM - FSM state set failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_CamTrainingTimeoutHandle"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

#undef DSL_DBG_BLOCK
#define DSL_DBG_BLOCK DSL_DBG_DEVICE

/**
   This function returns the DSL mode index based on the enum type
   \ref DSL_DslModeSelection_t and using the currently operated DSL firmware
   binary on the line of the current context instance.

   \param pContext - Pointer to dsl library context structure.

   \return DSL mode index.
   Return value is within range of 0 to \ref DSL_MODE_LAST - 1.
   Special value of -1 will be returned in case of an error occured and no index
   can be determined.
*/
DSL_int_t DSL_DRV_VRX_DslModeIndexGet(
   DSL_Context_t *pContext)
{
   DSL_int_t nIndex = -1;

   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSL))
   {
      nIndex = 0;
   }
   else if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
   {
      nIndex = 1;
   }
   else
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - DSL_DRV_VRX_DslModeIndexGet!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }

   return nIndex;
}

DSL_boolean_t DSL_DRV_VRX_FirmwareXdslModeCheck(
   DSL_Context_t           *pContext,
   const DSL_VRX_XdslMode_t nXdslMode)
{
   DSL_VRX_ApplicationCode_t nAppType = DSL_APP_UNKNOWN;
   DSL_boolean_t bReturn = DSL_FALSE;

#if defined(INCLUDE_DSL_BONDING) && (DSL_DRV_LINES_PER_DEVICE == 2)
   /* Get XDSL modes from the base line */
   DSL_uint8_t nLineBase;

   nLineBase = (pContext->pDevCtx->nNum / DSL_DRV_LINES_PER_DEVICE) * DSL_DRV_MAX_DEVICE_NUMBER;
   pContext = pContext->pXDev[nLineBase].pContext;
#endif

   nAppType = (DSL_VRX_ApplicationCode_t)pContext->pDevCtx->data.fwFeatures.nApplication;

   switch (nXdslMode)
   {
   case DSL_VRX_FW_VDSL2:
      if ((nAppType == DSL_APP_VDSL_POTS ) ||
          (nAppType == DSL_APP_VDSL_ISDN ) ||
          (nAppType == DSL_APP_VDSL_ISDN_VEC))
         bReturn = DSL_TRUE;
      break;
   case DSL_VRX_FW_ADSLA:
      if (nAppType == DSL_APP_ADSLA)
         bReturn = DSL_TRUE;
      break;
   case DSL_VRX_FW_ADSLB:
      if ((nAppType == DSL_APP_ADSLB_J) ||
          (nAppType == DSL_APP_ADSLB  ))
         bReturn = DSL_TRUE;
      break;
   case DSL_VRX_FW_ADSL:
      if ((nAppType == DSL_APP_ADSLA) ||
          (nAppType == DSL_APP_ADSLB_J) ||
          (nAppType == DSL_APP_ADSLB  ))
         bReturn = DSL_TRUE;
      break;
   }

   DSL_DEBUG( DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: DSL_DRV_VRX_FirmwareXdslModeCheck %d vs FW %d, result %d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nXdslMode, nAppType, bReturn));

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
DSL_Error_t DSL_DRV_VRX_DataPathFailuresStatusGet(
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

   memset(&sAck, 0, sizeof(sAck));

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
DSL_Error_t DSL_DRV_VRX_FramingParametersVdsl2StatusGet(
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
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_VRX_FramingParametersVdsl2StatusGet,"
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
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_FramingParametersVdsl2StatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return (nErrCode);
}
#endif /* INCLUDE_DSL_FRAMING_PARAMETERS*/

#ifdef INCLUDE_DSL_FRAMING_PARAMETERS
DSL_Error_t DSL_DRV_VRX_FramingParametersAdslStatusGet(
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
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_FramingParametersAdslStatusGet,"
      " (nDirection=%d, nChannel=%d)"DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
      nDir, nChannel));

   memset(&sAck, 0, sizeof(sAck));

   /* Get ADSL framing parameters*/
   nErrCode = DSL_DRV_VRX_SendMsgAdslFrameDataGet(pContext, nDir, (DSL_uint8_t*)&sAck);
   if(nErrCode < 0)
      return nErrCode;

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
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_FramingParametersAdslStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return (nErrCode);

}
#endif /* INCLUDE_DSL_FRAMING_PARAMETERS*/

#ifdef INCLUDE_DSL_DELT
DSL_Error_t DSL_DRV_VRX_DiagnosticDeltDataGet(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   ACK_TestParamsAuxDS_Get_t  sAck;
   DSL_uint16_t i = 0;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_AdslDeltAllDataGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   if( pContext->DELT == DSL_NULL )
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - No memory allocated to store DELT data!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERROR;
   }

   memset(&sAck, 0, sizeof(sAck));

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

      if(DSL_DRV_MUTEX_LOCK(pContext->dataMutex))
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: Couldn't lock data mutex"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return DSL_ERR_SEMAPHORE_GET;
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

      DSL_DRV_MUTEX_UNLOCK(pContext->dataMutex);

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

      /* Get SNR values (without Virtual Noise) */
      nErrCode = DSL_DRV_VRX_SendMsgSnrGet(pContext, (DSL_AccessDir_t)i,
                     ((DSL_AccessDir_t)i) == DSL_DOWNSTREAM ?
                     (DSL_G997_NSCData8_t*)&(pContext->DELT->snrDataDs.deltSnr) :
                     (DSL_G997_NSCData8_t*)&(pContext->DELT->snrDataUs.deltSnr));

      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG(DSL_DBG_ERR,(pContext,SYS_DBG_ERR
            "DSL[%02d]: ERROR - DELT SNR data get failed, nDirection=%d!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), i));

         break;
      }

      /* Get SNR values (with Virtual Noise), DS only */
      if ((DSL_AccessDir_t)i == DSL_DOWNSTREAM)
      {
         nErrCode = DSL_DRV_VRX_SnrAllocationTableNeGet(pContext,
            (DSL_G997_NSCData8_t*)&(pContext->DELT->snrDataDs.deltSnrVn));

         if( nErrCode != DSL_SUCCESS )
         {
            DSL_DEBUG(DSL_DBG_ERR,(pContext, SYS_DBG_ERR
               "DSL[%02d]: ERROR - DELT SNR data get failed, nDirection=%d!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext), i));

            break;
         }
         else
         {
            /* Calculate SNR values per subcarrier group*/
            nErrCode = DSL_DRV_VRX_SnrPerGroupCalculate(pContext,
               (DSL_uint8_t)sAck.SNRG, &(pContext->DELT->snrDataDs.deltSnrVn));
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
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_AdslDeltAllDataGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return(nErrCode);
}
#endif /* INCLUDE_DSL_DELT*/

DSL_Error_t DSL_DRV_VRX_LineFailuresStatusUpdate(
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
DSL_boolean_t DSL_DRV_VRX_BandPlanSupportedCheck(
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
         SYS_DBG_ERR"DSL[%02d]: ERROR - DSL_VRX_BandPlanSupportedCheck: (nBandplan=%d, "
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

DSL_Error_t DSL_DRV_VRX_FailReasonGet(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_G997_LineInit_t lineInitStatus = LINIT_UNKNOWN;
   DSL_G997_LineInitSubStatus_t lineInitSubStatus = LINIT_SUB_NONE;
   ACK_ModemFSM_FailReasonGet_t sAck;
#ifdef INCLUDE_DEVICE_EXCEPTION_CODES
   DSL_LastExceptionCodesData_t LastExceptionCodes;
#endif /* INCLUDE_DEVICE_EXCEPTION_CODES*/

   memset(&sAck, 0, sizeof(sAck));

   nErrCode = DSL_DRV_VRX_SendMsgModemFSMFailReasonGet(
                  pContext,(DSL_uint8_t*)&sAck);

   if( nErrCode == DSL_SUCCESS )
   {
#ifdef INCLUDE_DEVICE_EXCEPTION_CODES
      /* Read previous Firmware failure/error codes from the CPE API Context */
      DSL_CTX_READ(pContext, nErrCode, LastExceptionCodes, LastExceptionCodes);

      /* Update previous Firmware failure/error code values */
      LastExceptionCodes.nErrorCode2 = LastExceptionCodes.nErrorCode1;
      LastExceptionCodes.nSubErrorCode2 = LastExceptionCodes.nSubErrorCode1;
      LastExceptionCodes.nFwErrorCode2 = LastExceptionCodes.nFwErrorCode1;

      /* Update current Firmware failure/error code values */
      LastExceptionCodes.nErrorCode1 = (DSL_uint32_t)sAck.ErrorCode;
      LastExceptionCodes.nSubErrorCode1 = (DSL_uint32_t)sAck.SubErrorCode;
      LastExceptionCodes.nFwErrorCode1 = (DSL_uint32_t)sAck.FW_FailCode;

      DSL_CTX_WRITE(pContext, nErrCode, LastExceptionCodes, LastExceptionCodes);
#endif /* INCLUDE_DEVICE_EXCEPTION_CODES */
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
   case ALM_ModemFSM_FailReasonGet_S_TIMEOUT:
      lineInitSubStatus = LINIT_SUB_TIMEOUT;
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
   case ALM_ModemFSM_FailReasonGet_S_PORT_MODE:
      lineInitSubStatus = LINIT_SUB_PORT_MODE;
      break;
   case ALM_ModemFSM_FailReasonGet_S_FAST_LOS:
      lineInitSubStatus = LINIT_SUB_FAST_LOS;
      break;
   case ALM_ModemFSM_FailReasonGet_S_PP_DRIVER:
      lineInitSubStatus = LINIT_SUB_S_PP_DRIVER;
#ifdef INCLUDE_DSL_SYSTEM_INTERFACE
         nErrCode = DSL_DRV_VRX_SystemInterfaceStatusUpdate(pContext);
         if(nErrCode != DSL_SUCCESS)
         {
            DSL_DEBUG( DSL_DBG_ERR, (pContext,SYS_DBG_ERR
               "DSL[%02d]: ERROR - System Interface status update failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         }
#endif /* INCLUDE_DSL_SYSTEM_INTERFACE*/
      break;
   case ALM_ModemFSM_FailReasonGet_S_OK:
      lineInitSubStatus = LINIT_SUB_NONE;
      break;
   case ALM_ModemFSM_FailReasonGet_S_INTENDED_LOCAL_SHUTDOWN:
      lineInitSubStatus = LINIT_SUB_S_INTENDED_LOCAL_SHUTDOWN;
      break;
   default:
      lineInitSubStatus = LINIT_SUB_UNKNOWN;
      break;
   }

   DSL_DRV_HandleLinitValue(pContext, lineInitStatus, lineInitSubStatus);

   return (nErrCode);
}

DSL_Error_t DSL_DRV_VRX_XtseConfigure(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint32_t i = 0;
   DSL_G997_XTUSystemEnablingData_t XTSE;

   for (i = 0; i < DSL_G997_NUM_XTSE_OCTETS; i++)
   {
      DSL_CTX_READ_SCALAR(pContext, nErrCode, xtseCfg[i], XTSE.XTSE[i]);
   }

   /* Check for the VDSL only mode*/
   if (DSL_DRV_VRX_CamFsmStateGet(pContext) == DSL_CAM_VDSL_DUAL)
   {
      /* ADSL bits will be masked*/
      DSL_DEBUG( DSL_DBG_MSG, (pContext,
         SYS_DBG_MSG"DSL[%02d]: ADSL bits masked out" DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));
      XTSE.XTSE[1-1] = XTSE.XTSE[2-1] = XTSE.XTSE[3-1] = XTSE.XTSE[4-1] = XTSE.XTSE[5-1] =
         XTSE.XTSE[6-1] = XTSE.XTSE[7-1] = 0x0;
   }

   nErrCode = DSL_DRV_VRX_SendMsgXtseConfigure(pContext, &XTSE);

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
   DSL_VRX_FwVersion_t *pData)
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
   VRX_CodeString_t *pCS;

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
         else if (fioVrxDrvInit.ictl.retCode == e_MEI_ERR_ALREADY_DONE)
         {
            DSL_DEBUG(DSL_DBG_WRN, (pContext,
               SYS_DBG_WRN"DSL[%02d]: WARNING - MEI device already initialized"
               "(device tree data used)"DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            nErrCode = DSL_WRN_ALREADY_INITIALIZED;
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

   return nErrCode;
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
   nErrCode = DSL_DRV_SystemInterfaceConfigCheck(pContext, DSL_MODE_ADSL,
      &(pData->data.nDeviceCfg.sysCIF[DSL_MODE_ADSL]));
   if( nErrCode < DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext, SYS_DBG_ERR
         "DSL[%02d]: ERROR - ADSL System Interface Configuration check failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   nErrCode = DSL_DRV_SystemInterfaceConfigCheck(pContext, DSL_MODE_VDSL,
      &(pData->data.nDeviceCfg.sysCIF[DSL_MODE_VDSL]));
   if( nErrCode < DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext, SYS_DBG_ERR
         "DSL[%02d]: ERROR - VDSL System Interface Configuration check failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   DSL_CTX_WRITE(pContext, nErrCode,
      pDevCtx->data.deviceCfg.sysCIF[DSL_MODE_ADSL],
      pData->data.nDeviceCfg.sysCIF[DSL_MODE_ADSL]);

   DSL_CTX_WRITE(pContext, nErrCode,
      pDevCtx->data.deviceCfg.sysCIF[DSL_MODE_VDSL],
      pData->data.nDeviceCfg.sysCIF[DSL_MODE_VDSL]);

   DSL_CTX_WRITE(pContext, nErrCode,
      pDevCtx->data.deviceCfg.ChannelConfigData[DSL_UPSTREAM],
      pData->data.nDeviceCfg.ChannelConfigData[DSL_UPSTREAM]);

   DSL_CTX_WRITE(pContext, nErrCode,
      pDevCtx->data.deviceCfg.ChannelConfigData[DSL_DOWNSTREAM],
      pData->data.nDeviceCfg.ChannelConfigData[DSL_DOWNSTREAM]);

   /* Call device specific implementation*/
   nErrCode = DSL_DRV_DEV_LowLevelConfigurationSet(pContext,
      &(pData->data.nDeviceCfg.cfg));

   if( nErrCode == DSL_ERROR )
   {
      DSL_DEBUG( DSL_DBG_MSG, (pContext, SYS_DBG_MSG
         "DSL[%02d]: ERROR - Low Level Configuration set failed!"
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

   if (pData->data.nDeviceCfg.nActivationCfg.nActivationSequence < DSL_ACT_SEQ_AUTO ||
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

   /* Write FSM remember configuration*/
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.bRemember,
      pData->data.nDeviceCfg.bRememberCfg);

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

   if(DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSLA))
      memcpy(
         pContext->lineInventoryNe.XTSECapabilities,
         DSL_XtseAdslA,
         DSL_G997_NUM_XTSE_OCTETS);
   if(DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSLB))
      memcpy(
         pContext->lineInventoryNe.XTSECapabilities,
         DSL_XtseAdslB,
         DSL_G997_NUM_XTSE_OCTETS);
   if(DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
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
      if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSL))
      {
         /* FW special value equals API special value */
         pData->ACTPS = nDirection == DSL_DOWNSTREAM ? ((DSL_int16_t)sAck.DS.ACTPSDds):
                                                       ((DSL_int16_t)sAck.US.ACTPSDus);
      }
      else
      {
         /* Not supported */
         pData->ACTPS =  -901;
      }

      pData->LATN = nDirection == DSL_DOWNSTREAM ? sAck.DS.LATNds :
                                                   sAck.US.LATNus;
      /* Check for special FW value (1023) and map it to special API value (1271) */
      pData->LATN = pData->LATN == 1023 ? 1271 : pData->LATN;

      pData->SATN = nDirection == DSL_DOWNSTREAM ? sAck.DS.SATNds :
                                                   sAck.US.SATNus;
      /* Check for special FW value (1023) and map it to special API value (1271) */
      pData->SATN = pData->SATN == 1023 ? 1271 : pData->SATN;

      pData->SNR = nDirection == DSL_DOWNSTREAM ? (DSL_int16_t)sAck.DS.SNRMds:
                                                  (DSL_int16_t)sAck.US.SNRMus;
      /* Check for special FW value (-512) and map it to special API value (-641) */
      pData->SNR = pData->SNR == -512 ? -641 : pData->SNR;

      /* If value equals zero than return warning code (incomplete values) */
      pData->ATTNDR  = nDirection == DSL_DOWNSTREAM ?
                    ((DSL_uint32_t)sAck.DS.ATTNDRds_LSW) |
                    (((DSL_uint32_t)sAck.DS.ATTNDRds_MSW) << 16) :
                    ((DSL_uint32_t)sAck.US.ATTNDRus_LSW) |
                    (((DSL_uint32_t)sAck.US.ATTNDRus_MSW) << 16);

      /* value is not ready yet (get by EOC), try later*/
      if (pData->ATTNDR == 0)
      {
         nErrCode = DSL_WRN_INCOMPLETE_RETURN_VALUES;
      }

      /* FW special value equals API special value */
      pData->ACTATP  = nDirection == DSL_DOWNSTREAM ? sAck.DS.ACTATPds:
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
   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
   {
      memset(&sBandList, 0x0, sizeof(DSL_BandList_t));
      /* get number uf currently used bands */
      nErrCode = DSL_DRV_VRX_SendMsgBandControlGet(pContext, nDirection, &sBandList);
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

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file
   'drv_dsl_cpe_device_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_UsPowerBackOffStatusGet(
   DSL_Context_t *pContext,
   DSL_G997_UsPowerBackOffStatusData_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_int_t i;
   ACK_PBO_AELEM_Status_Get_t sAck;
   DSL_BandList_t  sBandList;
   static const DSL_uint8_t nMaxNumOfBands =
      sizeof(sAck.Kl0EstimOPb)/sizeof(sAck.Kl0EstimOPb[0]);

   memset(&sAck, 0, sizeof(sAck));

   /* init all data */
   for (i = 0; i < DSL_G997_MAX_NUMBER_OF_BANDS; i++)
   {
      pData->nKl0EstimOPb[i] = 0x800;
   }

   /* Get VDSL specific information*/
   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
   {
      memset(&sBandList, 0x0, sizeof(DSL_BandList_t));
      /* get number uf currently used bands */
      nErrCode = DSL_DRV_VRX_SendMsgBandControlGet(pContext, DSL_UPSTREAM, &sBandList);
      if (nErrCode != DSL_SUCCESS)
      {
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: ERROR - getting current band config failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return nErrCode;
      }

      nErrCode = DSL_DRV_VRX_SendMsgUsPowerBackOffStatusGet(
                    pContext, (DSL_uint8_t*)&sAck);

      /* Copy data only if successful */
      if (nErrCode >= 0)
      {
         /* fill only the used ones */
         for (i = 0; i < sBandList.nNumData; i++)
         {
            if(i >= nMaxNumOfBands || i >= DSL_G997_MAX_NUMBER_OF_BANDS)
            {
               break;
            }
            pData->nKl0EstimOPb[i] = (DSL_int16_t)sAck.Kl0EstimOPb[i];
         }

         /* set Actual AELE-MODE */
         pData->nAeleMode = (DSL_G997_AeleMode_t)sAck.AeleMode;
      }
   }

   return (nErrCode);
}

static DSL_Error_t DSL_DRV_VRX_BearerChStatusGet(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDir)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint32_t ActInp = 0;
   DSL_uint32_t ActInpRein = 0;
   DSL_uint32_t ActInpNoErasure = 0;
   DSL_uint32_t newDataRate = 0, newNetDataRate = 0;
   DSL_boolean_t bTrellisEnable = DSL_FALSE;
   DSL_uint32_t ActualInterleaveDelay = 0;
   DSL_DEV_VersionCheck_t nVerCheck = 0;
   DSL_boolean_t bReTxEnable = DSL_FALSE;
   ACK_RTX_BearerChsDS_Get_t sAckBchRtx;
   union
   {
      ACK_BearerChsUS_Get_t US;
      ACK_BearerChsDS_Get_t DS;
   } sAckBch;

   memset(&sAckBch, 0, sizeof(sAckBch));
   memset(&sAckBchRtx, 0, sizeof(sAckBchRtx));

   /* Check firmware version */
   nErrCode = DSL_DRV_VRX_FirmwareVersionCheck(pContext,
               DSL_MIN_FW_VERSION_R6_RELEASE, &nVerCheck);
   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW version check failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return nErrCode;
   }

   nErrCode = DSL_DRV_VRX_SendMsgBearerChannelStatusGet(pContext, nDir,
      (DSL_uint8_t*)&(sAckBch.DS));
   if (nErrCode < 0)
   {
      return nErrCode;
   }

   DSL_CTX_READ_SCALAR(
      pContext, nErrCode, lineFeatureDataSts[nDir].bReTxEnable,
      bReTxEnable);

   /* In case of retransmission (ReTx) is currently used an additional ReTx
      specific message includes required values. */
   if (bReTxEnable)
   {
      nErrCode = DSL_DRV_VRX_SendMsgBearerChsRtxGet(pContext,nDir,
         (DSL_uint8_t *)&sAckBchRtx);

      if (nErrCode < DSL_SUCCESS)
      {
         return nErrCode;
      }
   }

   /* *********************************************************************** */
   /*                        ActualInterleaveDelay                            */
   /* *********************************************************************** */
   if (bReTxEnable)
   {
      ActualInterleaveDelay = sAckBchRtx.ActDelay;
   }
   else
   {
      ActualInterleaveDelay = (nDir == DSL_UPSTREAM) ?
         /* Use fw interleaver delay values*/
         (sAckBch.US.delay_usLP0 + sAckBch.US.delay_usLP1) :
         (sAckBch.DS.delay_dsLP0 + sAckBch.DS.delay_dsLP1);
   }

   if (nVerCheck < DSL_VERSION_EQUAL)
   {
      /* Convert to the API interface relevant scaling (1/100ms) */
      /* FW value is in multiple of 1 ms in case of using FW older than R6 */
      ActualInterleaveDelay *= 100;
   }

   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, ActualInterleaveDelay[nDir][0],
                        ActualInterleaveDelay);

   /* *********************************************************************** */
   /*                           ActualDataRate                                */
   /*                         ActualNetDataRate                               */
   /* *********************************************************************** */
   if (bReTxEnable)
   {
      newNetDataRate = (sAckBch.DS.DRdsLP1_LSW | (sAckBch.DS.DRdsLP1_MSW << 16));
      newNetDataRate += (sAckBch.DS.DRdsLP0_LSW | (sAckBch.DS.DRdsLP0_MSW << 16));

      if (nVerCheck < DSL_VERSION_EQUAL)
      {
         /* Rate given in units of 4 kBit/s */
         newNetDataRate *= 4000;
      }
      else
      {
         /* Rate given in units of 1 kBit/s */
         newNetDataRate *= 1000;
      }

      newDataRate = (DSL_uint32_t)sAckBchRtx.ETR_LSW |
                    (((DSL_uint32_t)sAckBchRtx.ETR_MSW) << 16);
      /* This FW value is always in units of 1 kBit/s */
      newDataRate *= 1000;

      /* In case of ReTx is enabled LP1 is always used. */
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, nLPath[nDir][0],
                           DSL_LATENCY_FP_LP1);
   }
   else
   {
      newNetDataRate = (nDir == DSL_UPSTREAM) ?
         (sAckBch.US.DRusLP1_LSW | (sAckBch.US.DRusLP1_MSW << 16)) :
         (sAckBch.DS.DRdsLP1_LSW | (sAckBch.DS.DRdsLP1_MSW << 16));

      /* check whether fast path information has to be used
         FW does not indicate directly wether fast (LP1 parameters) or
         interleaved (LP0 parameters) mode is used for ADSL1.
         Use the content of DataRate LP1 to determine
         the interleaver mode therefore. */
      if (newNetDataRate != 0)
      {
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, nLPath[nDir][0],
                              DSL_LATENCY_FP_LP1);
      }
      else
      {
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, nLPath[nDir][0],
                              DSL_LATENCY_IP_LP0);
      }

      newNetDataRate += (nDir == DSL_UPSTREAM) ?
         (sAckBch.US.DRusLP0_LSW | (sAckBch.US.DRusLP0_MSW << 16)) :
         (sAckBch.DS.DRdsLP0_LSW | (sAckBch.DS.DRdsLP0_MSW << 16));

      if (nVerCheck < DSL_VERSION_EQUAL)
      {
         /* Rate given in units of 4 kBit/s */
         newNetDataRate *= 4000;
      }
      else
      {
         /* Rate given in units of 1 kBit/s */
         newNetDataRate *= 1000;
      }
      newDataRate = newNetDataRate;
   }

   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, nChannelActualDataRate[nDir][0],
                        newDataRate);
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, nChannelActualNetDataRate[nDir][0],
                        newNetDataRate);

   /* *********************************************************************** */
   /*                           Trellis update                                */
   /* *********************************************************************** */
   bTrellisEnable = (nDir == DSL_UPSTREAM) ?
      ((DSL_boolean_t)sAckBch.US.TCMstatus_usLP0) :
      ((DSL_boolean_t)sAckBch.DS.TCMstatus_dsLP0);

   DSL_CTX_WRITE_SCALAR(pContext, nErrCode,
                        lineFeatureDataSts[nDir].bTrellisEnable, bTrellisEnable);

   /* *********************************************************************** */
   /*                    ActualImpulseNoiseProtection                         */
   /*                  ActualImpulseNoiseProtectionRein                       */
   /*                ActualImpulseNoiseProtectionNoErasure                    */
   /* *********************************************************************** */
   if (nVerCheck >= DSL_VERSION_EQUAL)
   {
      ActInpNoErasure = (nDir == DSL_UPSTREAM) ?
         (sAckBch.US.actInpNoErasure_LP0 + sAckBch.US.actInpNoErasure_LP1) :
         (sAckBch.DS.actInpNoErasure_LP0 + sAckBch.DS.actInpNoErasure_LP1);
   }

   if (nDir == DSL_UPSTREAM)
   {
      ActInp =
         DSL_MAX(sAckBch.US.actInpNoErasure_LP0 + sAckBch.US.actInpNoErasure_LP1,
                 sAckBch.US.actInpErasure_LP0 + sAckBch.US.actInpErasure_LP1);
   }
   else
   {
      ActInp =
         DSL_MAX(sAckBch.DS.actInpNoErasure_LP0 + sAckBch.DS.actInpNoErasure_LP1,
                 sAckBch.DS.actInpErasure_LP0 + sAckBch.DS.actInpErasure_LP1);
   }

   if (bReTxEnable)
   {
      ActInpRein = (sAckBchRtx.ActInpREIN >= 255) ? 255 : sAckBchRtx.ActInpREIN;

      ActInp = (sAckBchRtx.ActInpSHINE >= 1023) ? 1023 : sAckBchRtx.ActInpSHINE;
   }

   DSL_CTX_WRITE_SCALAR(pContext, nErrCode,
                        ActualImpulseNoiseProtection[nDir][0], ActInp);

   DSL_CTX_WRITE_SCALAR(pContext, nErrCode,
                        ActualImpulseNoiseProtectionRein[nDir][0], ActInpRein);

   DSL_CTX_WRITE_SCALAR(pContext, nErrCode,
                        ActualImpulseNoiseProtectionNoErasure[nDir][0],
                        ActInpNoErasure);

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
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_ChannelStatusUpdate"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
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

   nErrCode = DSL_DRV_VRX_BearerChStatusGet(pContext, nDirection);
   if(nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - Channel Status Data for %s get failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nDirection == DSL_DOWNSTREAM ? "DS":"US"));
   }

   DSL_CTX_READ_SCALAR(pContext, nErrCode, nChannelActualDataRate[nDirection][0], newDataRate);

   /* Update Data Rate*/
   nErrCode = DSL_DRV_VRX_DataRateUpdate(pContext, nDirection, newDataRate);
   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - Data Rate Update failed (nDirection=%d)!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nDirection));

      return nErrCode;
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_ChannelStatusUpdate"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

static DSL_Error_t DSL_DRV_VRX_ReTxStatusGet(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_boolean_t bReTxEnable[DSL_ACCESSDIR_LAST] = { DSL_FALSE };
   DSL_DEV_VersionCheck_t nVerCheck = DSL_VERSION_ERROR;
   ACK_RTX_StatusGet_t RtxEnSt;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_ReTxStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
   {
      /* Get FW information */
      nErrCode = DSL_DRV_VRX_FirmwareVersionCheck(pContext,
         DSL_MIN_FW_VERSION_RETX_VDSL_DS, &nVerCheck);
      if (nErrCode != DSL_SUCCESS)
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW version check failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         return nErrCode;
      }
   }
   else if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSL))
   {
      /* Get FW information */
      nErrCode = DSL_DRV_VRX_FirmwareVersionCheck(pContext,
                  DSL_MIN_FW_VERSION_RETX_ADSL_DS, &nVerCheck);
      if (nErrCode != DSL_SUCCESS)
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW version check failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         return nErrCode;
      }
   }

   memset(&RtxEnSt, 0, sizeof(RtxEnSt));

   if (nVerCheck >= DSL_VERSION_EQUAL)
   {
      /* Get line status*/
      nErrCode = DSL_DRV_VRX_SendMsgRtxEnableStatusGet(pContext,
         (DSL_uint8_t *)&RtxEnSt);
      if(nErrCode < 0)
         return nErrCode;

      DSL_DEBUG(DSL_DBG_MSG, (pContext, SYS_DBG_MSG
         "DSL[%02d]: DSL_DRV_VRX_ReTxStatusGet: RtxDsStat = %hu, RtxUsStat = %hu"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), RtxEnSt.RtxUsedDs, RtxEnSt.RtxUsedUs));

         bReTxEnable[DSL_DOWNSTREAM] = (DSL_boolean_t) RtxEnSt.RtxUsedDs;
         bReTxEnable[DSL_UPSTREAM] = (DSL_boolean_t) RtxEnSt.RtxUsedUs;
   }

   /* Update ReTx Status */
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode,
      lineFeatureDataSts[DSL_DOWNSTREAM].bReTxEnable,
      bReTxEnable[DSL_DOWNSTREAM]);

   DSL_CTX_WRITE_SCALAR(pContext, nErrCode,
      lineFeatureDataSts[DSL_UPSTREAM].bReTxEnable,
      bReTxEnable[DSL_UPSTREAM]);

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

   memset(&sAck, 0, sizeof(sAck));

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

   memset(&sAck, 0, sizeof(sAck));

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

   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
   {
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.nCurrShowtime, FWMODE_VDSL);
   }
   else if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSL))
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

   memset(&sAck, 0, sizeof(sAck));

   /* Get ADSL framing status parameters*/
   nErrCode = DSL_DRV_VRX_SendMsgAdslFrameDataGet(pContext, nDirection, (DSL_uint8_t*)&sAck);

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
static DSL_Error_t DSL_DRV_VRX_TestParametersFeUpdate(
   DSL_Context_t *pContext,
   const EVT_PMD_TestParamsGet_t *pMsg)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint16_t i = 0;
   DSL_uint16_t nDataLen = 0;

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_TestParametersFeUpdate"
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
               (pMsg->StartIndex + i < VRX_MAX_SCGROUPS); i++ )
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
   if( pMsg->EndIndex < (VRX_MAX_SCGROUPS-1) )
   {
      nErrCode = DSL_DRV_VRX_SendMsgTestParamsFeRequest(
                    pContext,
                    (DSL_uint16_t)(pMsg->EndIndex+1),
                    (DSL_uint16_t)(DSL_MIN((pMsg->EndIndex + VRX_TESTPARAMS_FE_BLOCK_SIZE),
                                                                  (VRX_MAX_SCGROUPS-1))) );
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
      if(DSL_DRV_MUTEX_LOCK(pContext->dataMutex))
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: Couldn't lock data mutex"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return DSL_ERR_SEMAPHORE_GET;
      }

      pContext->DELT_SHOWTIME->hlogDataUsVdsl.nMeasurementTime = pMsg->hlogTime;
      pContext->DELT_SHOWTIME->qlnDataUsVdsl.nMeasurementTime  = pMsg->qlnTime;
      pContext->DELT_SHOWTIME->snrDataUsVdsl.nMeasurementTime  = pMsg->snrTime;

      DSL_DRV_MUTEX_UNLOCK(pContext->dataMutex);

      /* generate event only for the first time */
      if (pContext->eTestParametersFeReady == VRX_FE_TESTPARAMS_FIRST_UPDATING)
      {
         nErrCode = DSL_DRV_EventGenerate(
                    pContext, 0, DSL_ACCESSDIR_NA, DSL_FAR_END,
                    DSL_EVENT_S_FE_TESTPARAMS_AVAILABLE,
                    DSL_NULL,
                    0);
      }

      pContext->eTestParametersFeReady = VRX_FE_TESTPARAMS_READY;
      pContext->nTestParametersFeRefreshTimeout = 0;

      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Event(%d) generate failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
            DSL_EVENT_S_FE_INVENTORY_AVAILABLE));

         return nErrCode;
      }

      /* all data retrieved */
      DSL_DEBUG( DSL_DBG_MSG,
         (pContext, SYS_DBG_MSG"DSL[%02d]: All DELT FE date retrieved"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_TestParametersFeUpdate"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

static DSL_Error_t DSL_DRV_VRX_TestParametersFePeriodicallyUpdate(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   pContext->nTestParametersFeRefreshTimeout += DSL_AUTOBOOT_SHOWTIME_POLL_TIME;

   if (pContext->nTestParametersFeRefreshTimeout >= VRX_TESTPARAMS_FE_UPDATE_PERIOD)
   {
      pContext->eTestParametersFeReady = VRX_FE_TESTPARAMS_UPDATING;
      pContext->nTestParametersFeRefreshTimeout = 0;
      /* clear current amount of data (will be set after updating finished) */
      pContext->DELT_SHOWTIME->snrDataUsVdsl.deltSnr.nNumData = 0;
      /* Request firts portion of DELT data from the FE*/
      nErrCode = DSL_DRV_VRX_SendMsgTestParamsFeRequest(pContext, 0,
                                      VRX_TESTPARAMS_FE_BLOCK_SIZE - 1);
   }

   return nErrCode;
}

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
         (pContext, SYS_DBG_ERR"DSL[%02d]: DSL_VRX_WaitingMessageCopyData - Length <= 0 !!"
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
            nErrCode = DSL_DRV_VRX_TestParametersFeUpdate(pContext, (EVT_PMD_TestParamsGet_t*)buf);
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
   DSL_AccessDir_t nDirection,
   DSL_ReTxCounters_t *pCounters)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_ReTxCounters_t retxCounters;
   ACK_RTX_DS_StatsGet_t retxStats;
   DSL_DEV_VersionCheck_t nVerCheck = DSL_VERSION_ERROR;

   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
   {
      /* Get FW information */
      if (nDirection == DSL_DOWNSTREAM)
      {
         nErrCode = DSL_DRV_VRX_FirmwareVersionCheck(pContext,
            DSL_MIN_FW_VERSION_RETX_VDSL_DS, &nVerCheck);
      }
      else if (nDirection == DSL_UPSTREAM)
      {
         nErrCode = DSL_DRV_VRX_FirmwareVersionCheck(pContext,
            DSL_MIN_FW_VERSION_RETX_VDSL_US, &nVerCheck);
      }

      if (nErrCode != DSL_SUCCESS)
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW version check failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         return nErrCode;
      }
   }
   else if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSL))
   {
      /* Retransmission is not supported for ADSL/US */
      if (nDirection == DSL_DOWNSTREAM)
      {
         /* Get FW information */
         nErrCode = DSL_DRV_VRX_FirmwareVersionCheck(pContext,
                     DSL_MIN_FW_VERSION_RETX_ADSL_DS, &nVerCheck);
         if (nErrCode != DSL_SUCCESS)
         {
            DSL_DEBUG(DSL_DBG_ERR,
               (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW version check failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
            return nErrCode;
         }
      }
   }

   memset(&retxStats, 0, sizeof(retxStats));

   if (nVerCheck >= DSL_VERSION_EQUAL)
   {
      /* Get first part of ReTx counters from the FW*/
      nErrCode = DSL_DRV_VRX_SendMsgRtxStatsGet(pContext, nDirection,
         (DSL_uint8_t *)&retxStats);
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

      /* RxRetransmitted - NOT valid for VRX200/VRX300 */
      pCounters->nRxRetransmitted = 0;

      /* TxRetransmitted */
      pCounters->nTxRetransmitted =
         ((DSL_uint32_t)(retxStats.TxDtuRTX_MSW << 16)) | retxStats.TxDtuRTX_LSW;
   }
   else
   {
      /* Just ignore request if FW doesn't support it */
      memset(&retxCounters,0x0,sizeof(DSL_ReTxCounters_t));
   }

   /* To be extended for US handling within context of DSLCPE_SW-749 */
   if (nDirection == DSL_DOWNSTREAM)
   {
      DSL_CTX_READ( pContext, nErrCode, retxCounters, retxCounters);

      pCounters->nRxCorrected += retxCounters.nRxCorrected;
      pCounters->nRxCorruptedTotal += retxCounters.nRxCorruptedTotal;
      pCounters->nRxRetransmitted += retxCounters.nRxRetransmitted;
      pCounters->nRxUncorrectedProtected += retxCounters.nRxUncorrectedProtected;
      pCounters->nTxRetransmitted += retxCounters.nTxRetransmitted;
   }

   return nErrCode;
}

DSL_Error_t DSL_DRV_DEV_TrainingTimeoutSet(
   DSL_IN DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint32_t nTimeout;
   DSL_DEV_CamStates_t nCamState = DSL_DRV_VRX_CamFsmStateGet(pContext);
   DSL_ActivationFsmConfigData_t nActivationCfg;
   DSL_boolean_t bT1_413 = DSL_FALSE;
   DSL_uint8_t nVtse = 0;

   /* get the VDSL bits */
   DSL_CTX_READ_SCALAR(pContext, nErrCode, xtseCfg[7], nVtse);

   if (nVtse)
   {
      switch (nCamState)
      {
         case DSL_CAM_VDSL_SINGLE:
         case DSL_CAM_VDSL_DUAL:
            nTimeout = DSL_AUTOBOOT_HANDSHAKE_TIMEOUT_V_GHS;
            break;

         case DSL_CAM_ADSL_SINGLE:
            DSL_CTX_READ(pContext, nErrCode,
                                    pDevCtx->data.nActivationCfg, nActivationCfg);
            DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.bT1_413, bT1_413);

            if ( (nActivationCfg.nActivationSequence == DSL_ACT_SEQ_AUTO ||
                  nActivationCfg.nActivationSequence == DSL_ACT_SEQ_NON_STD)
                  && bT1_413)
            {
               if (nActivationCfg.nActivationMode == DSL_ACT_MODE_ANSI_T1413)
               {
                  nTimeout = DSL_AUTOBOOT_HANDSHAKE_TIMEOUT_GOTO_V_ANSI;
               }
               else
               {
                  nTimeout = DSL_AUTOBOOT_HANDSHAKE_TIMEOUT_GOTO_V_GHS;
               }
            }
            else
            {
               nTimeout = DSL_AUTOBOOT_HANDSHAKE_TIMEOUT_GOTO_V_STD;
            }
            break;

         default:
            /* automode disabled case: no need to handshake wait/check, */
            /* set default training timeout                             */
            nTimeout = DSL_AUTOBOOT_PRE_TRAINING_TIMEOUT_DEFAULT;
            break;
      }
   }
   else
   {
      /* XTSE does not contain VDSL2 case: according to APS3 handshake  */
      /* timeout is infinite (no need to handshake wait and check),     */
      /* set default training timeout                                   */
      nTimeout = DSL_AUTOBOOT_PRE_TRAINING_TIMEOUT_DEFAULT;
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
   DSL_AccessDir_t nDirection;
   DSL_DEV_VersionCheck_t nVerCheck = DSL_VERSION_ERROR;
   DSL_ReTxCounters_t retxCounters;

   /* Important: API direction mapping to the FW (NE->DS, FE->US) */
   if (pData->nDirection == DSL_NEAR_END)
   {
      nDirection = DSL_DOWNSTREAM;
   }
   else
   {
      nDirection = DSL_UPSTREAM;
   }

   DSL_CTX_READ_SCALAR(
      pContext, nErrCode, lineFeatureDataSts[nDirection].bReTxEnable,
      bReTxEnable);

   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
   {
      /* Get FW information */
      if (nDirection == DSL_DOWNSTREAM)
      {
         nErrCode = DSL_DRV_VRX_FirmwareVersionCheck(pContext,
            DSL_MIN_FW_VERSION_RETX_VDSL_DS, &nVerCheck);
      }
      else if (nDirection == DSL_UPSTREAM)
      {
         nErrCode = DSL_DRV_VRX_FirmwareVersionCheck(pContext,
            DSL_MIN_FW_VERSION_RETX_VDSL_US, &nVerCheck);
      }

      if (nErrCode != DSL_SUCCESS)
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW version check failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         return nErrCode;
      }
   }
   else if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSL))
   {
      /* Retransmission is not supported for ADSL/US */
      if (nDirection == DSL_DOWNSTREAM)
      {
         /* Get FW information */
         nErrCode = DSL_DRV_VRX_FirmwareVersionCheck(pContext,
                     DSL_MIN_FW_VERSION_RETX_ADSL_DS, &nVerCheck);
         if (nErrCode != DSL_SUCCESS)
         {
            DSL_DEBUG(DSL_DBG_ERR,
               (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW version check failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
            return nErrCode;
         }
      }
   }

   if (nVerCheck >= DSL_VERSION_EQUAL)
   {
      if (bReTxEnable == DSL_FALSE)
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: Retransmission is not active."DSL_DRV_CRLF,
            DSL_DEV_NUM(pContext)));

         return DSL_ERR_RETRANSMISSION_DISABLED;
      }

      pData->data.nRxRetransmitted = 0;

      nErrCode = DSL_DRV_VRX_ReTxCountersGet(pContext, nDirection,
         &retxCounters);

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
   nErrCode = DSL_DRV_VRX_ReTxCountersGet(pContext, DSL_DOWNSTREAM, &retxCounters);

   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext, SYS_DBG_ERR
         "DSL[%02d]: Can't save near-end retransmission counters."DSL_DRV_CRLF,
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
      nErrCode = DSL_DRV_VRX_CheckTruncParamRange(
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
         nErrCode = DSL_DRV_VRX_CheckTruncParamRange(
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
         nErrCode = DSL_DRV_VRX_CheckTruncParamRange(
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
         nErrCode = DSL_DRV_VRX_CheckTruncParamRange(
                       pContext,
                       (DSL_int_t)DSL_DEV_HS_TONE_GROUP_CLEANED,
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
      nErrCode = DSL_DRV_VRX_CheckTruncParamRange(
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
   DSL_boolean_t bFwReady = DSL_FALSE, bCamFwReload = DSL_FALSE;
   DSL_TestModeControlSet_t nTestModeControl = DSL_TESTMODE_DISABLE;
   DSL_int32_t nNoiseMarginDelta = 0;
   DSL_BF_RebootCriteriaConfigData_t RbCrNeCfgData = DSL_REBOOT_CRITERIA_CLEANED;
   DSL_uint8_t *pXtseAdslA = (DSL_uint8_t *) &DSL_XtseAdslA[0];
   DSL_uint8_t *pXtseAdslB = (DSL_uint8_t *) &DSL_XtseAdslB[0];
   DSL_uint8_t *pXtseVdsl2 = (DSL_uint8_t *) &DSL_XtseVdsl2[0];
   DSL_DEV_VersionCheck_t nVerCheck = DSL_VERSION_ERROR;
   DSL_boolean_t bEnableVirtualNoise = DSL_FALSE;

   DSL_CHECK_CTX_POINTER(pContext);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_POINTER(pContext, pContext->pDevCtx);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_POINTER(pContext, pContext->pDevCtx->lowHandle);
   DSL_CHECK_ERR_CODE();

   /* Get FW DL information*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->bFirmwareReady, bFwReady);

   if( nErrCode != DSL_SUCCESS || !bFwReady )
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: DSL_DEV_ModemWriteConfig: ERROR - no FW downloaded!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERROR;
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

   /* Check firmware version */
   nErrCode = DSL_DRV_VRX_FirmwareVersionCheck(pContext,
               DSL_MIN_FW_VERSION_R6_RELEASE, &nVerCheck);
   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW version check failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return nErrCode;
   }

   /* In case of using pre R6 release of ADSL FW there could be an error because
      message is not supported within FW. In this case interprete the return
      code always as a warning. */
   nErrCode = DSL_DRV_VRX_SendMsgHsToneGroupSet(pContext, pXtseAdslA, pXtseAdslB,
      pXtseVdsl2);
   if( nErrCode!=DSL_SUCCESS )
   {
      if ((DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSL) &&
          (nVerCheck < DSL_VERSION_EQUAL)))
      {
         DSL_DEBUG( DSL_DBG_WRN, (pContext,
            SYS_DBG_WRN"DSL[%02d]: DSL_DEV_ModemWriteConfig: WARNING - "
            "Handshake tones config currently not supported by ADSL FW!" DSL_DRV_CRLF,
            DSL_DEV_NUM(pContext)));

         nErrCode = DSL_SUCCESS;
      }
      else
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: DSL_DEV_ModemWriteConfig: ERROR - "
            "handshake tones config write!" DSL_DRV_CRLF,
            DSL_DEV_NUM(pContext)));

         return nErrCode;
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
   nErrCode = DSL_DRV_VRX_XtseConfigure(pContext);
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

   /* Check firmware version */
   nErrCode = DSL_DRV_VRX_FirmwareVersionCheck(pContext,
               DSL_MIN_FW_VERSION_TC_STAT_ADSL, &nVerCheck);
   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG(DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - FW version check failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return nErrCode;
   }

   /* In case of ADSL with given minimum FW version send shortened bearer channel
      configuration message (only TC control bits in parameter 2 used). */
   if ((nVerCheck >= DSL_VERSION_EQUAL) &&
       (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSL)))
   {
      nErrCode = DSL_DRV_VRX_SendMsgBearerChTcLayerSet(pContext);
      if( nErrCode!=DSL_SUCCESS )
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: DSL_DEV_ModemWriteConfig: ERROR - "
            "Channel shortend config write!" DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return nErrCode;
      }
   }

   /* In case of VDSL send complete bearer channel configuration message
      (this and VDSL specific profile control are not possible for ADSL). */
   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
   {
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

   /* Send Rate Adaptation Mode Settings */
   nErrCode = DSL_DRV_VRX_SendMsgOlrControl(pContext);

   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG(DSL_DBG_ERR, (pContext, SYS_DBG_ERR
        "DSL[%02d]: ERROR - Online Reconfiguration parameters set failed!"
        DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return nErrCode;
   }

   /* Send retransmission settings */
   nErrCode = DSL_DRV_VRX_SendMsgRtxControl(pContext);
   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - ReTx parameters set failed!" DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext)));
      return nErrCode;
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
   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
   {
      /* Write Bonding configuration*/
      nErrCode = DSL_DRV_BND_VRX_ConfigWrite(pContext);
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
   nErrCode = DSL_DRV_VRX_FirmwareVersionCheck(pContext,
                  DSL_MIN_FW_VERSION_FGAIN, &nVerCheck);
   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW version check failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return nErrCode;
   }

   if (nVerCheck >= DSL_VERSION_EQUAL)
   {
      nErrCode = DSL_DRV_VRX_SendMsgMiscConfigSet(pContext);
      if( nErrCode < DSL_SUCCESS)
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Misc config set failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return nErrCode;
      }

      nErrCode = DSL_DRV_VRX_SendMsgTestOptionsSet(pContext);
      if( nErrCode < DSL_SUCCESS)
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Test options set failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return nErrCode;
      }
   }


   /* check for virtual noise firmware support */
   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
   {
      /* Get FW information*/
      nErrCode = DSL_DRV_VRX_FirmwareVersionCheck(pContext,
                  DSL_MIN_FW_VERSION_VIRT_NOISE_VDSL, &nVerCheck);
      if (nErrCode != DSL_SUCCESS)
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW version check failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         return nErrCode;
      }
   }
   else if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSL))
   {
      /* Get FW information*/
      nErrCode = DSL_DRV_VRX_FirmwareVersionCheck(pContext,
                  DSL_MIN_FW_VERSION_VIRT_NOISE_ADSL, &nVerCheck);
      if (nErrCode != DSL_SUCCESS)
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW version check failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         return nErrCode;
      }
   }

   /* if virtual noise supported - set default configuration for virtual noise */
   bEnableVirtualNoise = (nVerCheck >= DSL_VERSION_EQUAL) ? DSL_TRUE : DSL_FALSE;

   /* set Virtual Noise and AELEM */
   nErrCode = DSL_DRV_VRX_SendMsgModemFsmOptions2Set(pContext, bEnableVirtualNoise);
   if (nErrCode < DSL_SUCCESS)
   {
      DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Fsm options2 set failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
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
   DSL_VRX_FwVersion_t sFwVersion = {0};
   DSL_uint32_t verNum = 0;
   DSL_FirmwareRequestType_t nFwType = DSL_FW_REQUEST_NA;
   DSL_boolean_t bDefaultRaModeSet = DSL_TRUE;
#if defined(INCLUDE_DSL_PM)
   DSL_uint32_t fwDwnldStartTime = 0, fwDwnldStopTime = 0, fwDwnldTime = 0;
#endif
   DSL_FwDownloadStatusData_t fwDwnlStatus;
   DSL_uint8_t nReloadCnt = 0;
   DSL_int_t nNum;
   DSL_boolean_t bSlavePort;
   DSL_PortMode_t nPortMode;
#if defined(INCLUDE_DSL_BONDING) && (DSL_DRV_LINES_PER_DEVICE == 2)
   DSL_FirmwareXdslFeature_t nFeatureSet = DSL_FW_XDSLFEATURE_CLEANED;
#endif
   DSL_FirmwareFeatures_t nFwFeatures;
   DSL_FirmwareType_t nNextMode;

   DSL_CHECK_POINTER(pContext, pContext->pDevCtx->lowHandle);
   DSL_CHECK_ERR_CODE();

   /* Get device handle*/
   dev = DSL_DEVICE_LOWHANDLE(pContext);

   DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->data.deviceCfg.nFwType, nFwType);

   /* get device number*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->nNum, nNum);
   /* get port mode type*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->data.nPortMode, nPortMode);

   bSlavePort = nNum % DSL_DRV_LINES_PER_DEVICE ? DSL_TRUE : DSL_FALSE;

#if defined(INCLUDE_DSL_PM)
   fwDwnldStartTime = DSL_DRV_ElapsedTimeMSecGet(0);
#endif

#if defined(INCLUDE_DSL_BONDING) && (DSL_DRV_LINES_PER_DEVICE == 2)
   if ((pFirmware != DSL_NULL) && nFirmwareSize)
   {
      /* Get VDSL firmware features for 1st (vectoring) fw */
      DSL_CTX_READ(pContext, nErrCode, nFwFeatures.nFirmwareVdslFeatures, nFeatureSet);

      /* 1st fw could not support vectoring & bonding */
      if (nFeatureSet & DSL_FW_XDSLFEATURE_DUALPORT)
      {
         DSL_DEBUG(DSL_DBG_WRN, (pContext, SYS_DBG_WRN
            "DSL[%02d]: WARNING - Vectoring support firmware could not support bonding "
            "due to the resources limitations!"DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      }
   }

   if ((pFirmware2 != DSL_NULL) && nFirmwareSize2)
   {
      /* Get VDSL firmware features for 2n (bonding) fw */
      DSL_CTX_READ(pContext, nErrCode, nFwFeatures2.nFirmwareVdslFeatures, nFeatureSet);

      /* 2nd fw should support bonding */
      if (!(nFeatureSet & DSL_FW_XDSLFEATURE_DUALPORT))
      {
         DSL_DEBUG(DSL_DBG_ERR, (pContext, SYS_DBG_ERR
            "DSL[%02d]: ERROR - Trying to load firmware that doesn't support "
            "bonding within bonding API"DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return DSL_ERR_BND_NOT_SUPPORTED_BY_FIRMWARE;
      }
   }
#endif

   for(;;)
   {
      memset(&Vdsl2_FwDl,0x0,sizeof(IOCTL_MEI_fwDownLoad_t));

#if defined(INCLUDE_DSL_BONDING) && (DSL_DRV_LINES_PER_DEVICE == 2)
      /* Choose R4/R6 firmware */
      if (nPortMode == DSL_PORT_MODE_DUAL)
      {
         /* if 2nd fw is not empty... */
         if ((pFirmware2 != DSL_NULL) && nFirmwareSize2)
         {
            /* use 2nd fw */
            Vdsl2_FwDl.pFwImage  = (unsigned char *)pFirmware2;
            Vdsl2_FwDl.size_byte = (unsigned long)nFirmwareSize2;
            DSL_CTX_READ(pContext, nErrCode, nFwFeatures2, nFwFeatures);
            /* update context fw features */
            DSL_CTX_WRITE(pContext, nErrCode, nFwFeatures, nFwFeatures);
         }
         /* 2nd fw is empty */
         else
         {
            DSL_DEBUG( DSL_DBG_ERR, (pContext, SYS_DBG_ERR
               "DSL[%02d]: ERROR - missing VRx bonding support firmware!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            return DSL_ERROR;
         }
      }
      else
      {
         /* if 1st fw is not empty... */
         if ((pFirmware != DSL_NULL) && nFirmwareSize)
         {
            /* use 1st fw */
            Vdsl2_FwDl.pFwImage  = (unsigned char *)pFirmware;
            Vdsl2_FwDl.size_byte = (unsigned long)nFirmwareSize;
            DSL_CTX_READ(pContext, nErrCode, nFwFeatures, nFwFeatures);
         }
         /* 1st fw is empty */
         else
         {
            DSL_DEBUG( DSL_DBG_ERR, (pContext, SYS_DBG_ERR
               "DSL[%02d]: ERROR - missing VRx vectoring support firmware!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            return DSL_ERROR;
         }
      }
#else
      Vdsl2_FwDl.pFwImage  = (unsigned char *)pFirmware;
      Vdsl2_FwDl.size_byte = (unsigned long)nFirmwareSize;
      DSL_CTX_READ(pContext, nErrCode, nFwFeatures, nFwFeatures);
#endif

      /* Check FW availability*/
      if ( (Vdsl2_FwDl.size_byte == 0) || (Vdsl2_FwDl.pFwImage == DSL_NULL) )
      {
         DSL_DEBUG( DSL_DBG_ERR, (pContext,
            SYS_DBG_ERR"DSL[%02d]: ERROR - VRx FW download failed - no binary for %d FW mode!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nFwType));

         return DSL_ERROR;
      }

      if (!bSlavePort)
      {
         fwModeSelect.bMultiLineModeLock      = IFX_TRUE;
         fwModeSelect.bXdslModeLock           = IFX_TRUE;
         if (nPortMode == DSL_PORT_MODE_DUAL)
         {
            fwModeSelect.eMultiLineModeCurrent   = e_MEI_MULTI_LINEMODE_DUAL;
            fwModeSelect.eMultiLineModePreferred = e_MEI_MULTI_LINEMODE_DUAL;
         }
         else
         {
            fwModeSelect.eMultiLineModeCurrent   = e_MEI_MULTI_LINEMODE_SINGLE;
            fwModeSelect.eMultiLineModePreferred = e_MEI_MULTI_LINEMODE_SINGLE;
         }

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
         fwModeSelect.firmwareFeatures.nPlatformId = nFwFeatures.nPlatformId;
         fwModeSelect.firmwareFeatures.eFirmwareXdslModes = nFwFeatures.nFirmwareXdslModes;

         if( DSL_DRV_VRX_InternalFwModeCtrlSet((MEI_DYN_CNTRL_T*)dev, &fwModeSelect) != 0 )
         {
            DSL_DEBUG( DSL_DBG_ERR, (pContext,
               SYS_DBG_ERR"DSL[%02d]: ERROR - VRX FW Mode set failed, (nReturn = %d)!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext), fwModeSelect.ictl.retCode));

            return DSL_ERROR;
         }
      }
      else
      {
         /* Reset bFirmwareReady flag*/
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->bFirmwareReady, DSL_FALSE);
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
            nErrCode = DSL_DRV_VRX_CamFsmStateSet(pContext,
                          nFwType == DSL_FW_REQUEST_ADSL ? DSL_CAM_ADSL_SINGLE : DSL_CAM_VDSL_SINGLE);

            if( nErrCode != DSL_SUCCESS )
            {
               DSL_DEBUG( DSL_DBG_ERR,
                  (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - CAM FSM state set failed!"
                  DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

               return DSL_ERROR;
            }

            /* Todo: investigate this case */
            /* Update Next Mode */
            nNextMode = (nFwType == DSL_FW_REQUEST_ADSL) ?
                                    DSL_FW_TYPE_ADSL : DSL_FW_TYPE_VDSL;
            DSL_CTX_WRITE_SCALAR(pContext, nErrCode,
                                pDevCtx->data.nCamStatus.nNextMode, nNextMode);
            DSL_DEBUG(DSL_DBG_MSG,
               (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - Set Next Mode %u"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nNextMode));

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

      /* Used for special handling of default SRA value for DSLCPE_SW-768
         In case of VDSL FW is non-vectoring capable disable RA-mode for VDSL
         only once after first FW download and only if not already manually
         re-configured by the user (set default value)! */
      DSL_CTX_READ(pContext, nErrCode, bDefaultRaModeSet, bDefaultRaModeSet);

      if ( bDefaultRaModeSet == DSL_FALSE )
      {
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, bDefaultRaModeSet, DSL_TRUE);

         DSL_DEBUG( DSL_DBG_MSG, (pContext, SYS_DBG_MSG
            "DSL[%02d]: Update API default values for SRA dependend on used FW."
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         if (nFwFeatures.nFirmwareXdslModes & DSL_FW_XDSLMODE_VDSL2_VECTOR)
         {
            DSL_CTX_WRITE_SCALAR(pContext, nErrCode,
               rateAdaptationMode[DSL_MODE_VDSL][DSL_UPSTREAM],
               DSL_G997_RA_MODE_DYNAMIC);

            DSL_CTX_WRITE_SCALAR(pContext, nErrCode,
               rateAdaptationMode[DSL_MODE_VDSL][DSL_DOWNSTREAM],
               DSL_G997_RA_MODE_DYNAMIC);

            DSL_DEBUG( DSL_DBG_MSG, (pContext, SYS_DBG_MSG
               "DSL[%02d]: - Enable SRA for VDSL (vectoring capable FW)"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         }
         else if(nFwFeatures.nFirmwareXdslModes & DSL_FW_XDSLMODE_VDSL2)
         {
            DSL_CTX_WRITE_SCALAR(pContext, nErrCode,
               rateAdaptationMode[DSL_MODE_VDSL][DSL_UPSTREAM],
               DSL_G997_RA_MODE_AT_INIT);

            DSL_CTX_WRITE_SCALAR(pContext, nErrCode,
               rateAdaptationMode[DSL_MODE_VDSL][DSL_DOWNSTREAM],
               DSL_G997_RA_MODE_AT_INIT);

            DSL_DEBUG( DSL_DBG_MSG, (pContext, SYS_DBG_MSG
               "DSL[%02d]: - Disable SRA for VDSL (non-vectoring capable FW)"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         }
         else
         {
            DSL_DEBUG(DSL_DBG_ERR, (pContext, SYS_DBG_ERR
               "DSL[%02d]: Unexpected FW application type, cannot set default SRA config"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         }
      }

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
          !DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
      {
         if (!bSlavePort)
         {
            DSL_DEBUG( DSL_DBG_ERR,
               (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - ADSL FW was loaded, expecting VDSL FW!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            return DSL_ERROR;
         }
         else
         {
            DSL_DRV_VRX_CamFsmStateSet(pContext, DSL_CAM_ADSL_SINGLE);
            DSL_CTX_WRITE_SCALAR(pContext, nErrCode,
                         pDevCtx->data.nCamStatus.nNextMode, DSL_FW_TYPE_ADSL);
            DSL_DEBUG(DSL_DBG_MSG,
               (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - Set Next Mode %u"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext), DSL_FW_TYPE_ADSL));
         }
      }

      if (nFwType == DSL_FW_REQUEST_ADSL &&
          !DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSL))
      {
         if (!bSlavePort)
         {
            DSL_DEBUG( DSL_DBG_ERR,
               (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - VDSL FW was loaded, expecting ADSL FW!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            return DSL_ERROR;
         }
         else
         {
            DSL_DRV_VRX_CamFsmStateSet(pContext, DSL_CAM_VDSL_DUAL);
            DSL_CTX_WRITE_SCALAR(pContext, nErrCode,
                         pDevCtx->data.nCamStatus.nNextMode, DSL_FW_TYPE_VDSL);
            DSL_DEBUG(DSL_DBG_MSG,
               (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - Set Next Mode %u"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext), DSL_FW_TYPE_VDSL));
         }
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

   return nErrCode;
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
   DSL_DEV_CamStates_t nCamStateNew = DSL_CAM_INIT;
   DSL_uint8_t i, nAtse = 0, nAtseCurr = 0, nVtse = 0, nXtse1 = 0;
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
   if ( DSL_DRV_VRX_InitDeviceDrv(pContext, &pData->data.nDeviceCfg.cfg) < DSL_SUCCESS)
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

   /* Check if both ADSL and VDSL setings are not configured*/
   if ((nAtse == 0) && (nVtse == 0))
   {
      DSL_DEBUG(DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: CAM - No valid XTSE configuration!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return nErrCode;
   }

   /* Enable CAM FSM */
   nErrCode = DSL_DRV_VRX_CamFsmEnable(pContext, (nAtse || nVtse) ? DSL_TRUE : DSL_FALSE);
   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - CAM FSM init failed!" DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));

      return DSL_ERROR;
   }

   DSL_CTX_READ_SCALAR(pContext, nErrCode, xtseCfg[0], nXtse1);
   DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.nActivationCfg, nActivationCfg);

   if (nXtse1 & XTSE_1_01_A_T1_413)
   {
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.bT1_413, DSL_TRUE);
      if (nActivationCfg.nActivationSequence == DSL_ACT_SEQ_STD)
      {
         DSL_DEBUG( DSL_DBG_WRN, (pContext,
            SYS_DBG_ERR"DSL[%02d]: WARNING - Standard activation sequence configured "
                       "with a enabled T1.413 mode" DSL_DRV_CRLF,
            DSL_DEV_NUM(pContext)));
      }
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

   DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.nCamConfig, nCamOpt);
   /* Set Next Mode */
   if (nAtse && nVtse)
   {
      /* VDSL or ADSL default */
      if (nCamOpt.nNextMode != DSL_FW_TYPE_ADSL)
      {
         nCamOpt.nNextMode = DSL_FW_TYPE_VDSL;
      }
   }
   else if (nAtse)
   {
      nCamOpt.nNextMode = DSL_FW_TYPE_ADSL;
   }
   else
   {
      nCamOpt.nNextMode = DSL_FW_TYPE_VDSL;
   }

   /* Set initial NextMode (will not changed later) */
   DSL_CTX_WRITE(pContext, nErrCode, pDevCtx->data.nCamConfig, nCamOpt);
   /* Set dynamic NextMode */
   DSL_CTX_WRITE(pContext, nErrCode, pDevCtx->data.nCamStatus, nCamOpt);

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: CAM - Set initial Next Mode %u"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nCamOpt.nNextMode));

   if (nCamOpt.nNextMode == DSL_FW_TYPE_ADSL)
   {
      /* Set ADSL FW image type*/
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.deviceCfg.nFwType,
                                                         DSL_FW_REQUEST_ADSL);
      /* Set Port mode */
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.nPortMode,
                                                         DSL_PORT_MODE_SINGLE);
      nCamStateNew = DSL_CAM_ADSL_SINGLE;
   }
   else
   {
      /* Set VDSL FW image type*/
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.deviceCfg.nFwType,
                                                         DSL_FW_REQUEST_VDSL);
#if defined(INCLUDE_DSL_BONDING) && (DSL_DRV_LINES_PER_DEVICE == 2)
      /* Set Port mode */
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.nPortMode, DSL_PORT_MODE_DUAL);
      nCamStateNew = DSL_CAM_VDSL_DUAL;
#else
      /* Set Port mode */
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.nPortMode, DSL_PORT_MODE_SINGLE);
      nCamStateNew = DSL_CAM_VDSL_SINGLE;
#endif
   }

#if defined(INCLUDE_DSL_BONDING)
   /* Enable Bonding by default*/
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, BndConfig.bPafEnable, DSL_TRUE);
#endif

   nErrCode = DSL_DRV_VRX_CamFsmStateSet(pContext, nCamStateNew);
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

   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
   {
      nErrCode = DSL_DRV_VRX_SendMsgModemStateSet(
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
   }

   /* \todo To be checked if VDSL also supports Power Management State update via
            message. Manual set of L3 state required in this case? */
   /* Update Power Management Status */
   nErrCode = DSL_DRV_VRX_PowerManagementStatusSet(pContext, DSL_G997_PMS_L3);
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
DSL_Error_t DSL_DRV_DEV_LinkFreeze(
   DSL_Context_t *pContext,
   DSL_uint8_t *pXTSE)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CHECK_POINTER(pContext, pXTSE);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_LinkFreeze"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* ADSL 1 */
   if (pXTSE[0] || pXTSE[1])
   {
      /* stop at R_MEDLEY_TX */
      nErrCode = DSL_DRV_VRX_SendMsgLinkFreezeTX(pContext, 0x1B);
      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - stop at "
                                  "VDSL2_R_P_MEDLEY_TX failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return DSL_ERROR;
      }

      /* stop at R_C_MEDLEY_RX */
      nErrCode = DSL_DRV_VRX_SendMsgLinkFreezeRX(pContext, 0x4A);
      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - stop at "
                                "VDSL2_R_O_P_MEDLEY_RX failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return DSL_ERROR;
      }
   }
   /* ADSL 2/2+ */
   else if (pXTSE[2] || pXTSE[3] || pXTSE[4] || pXTSE[5] || pXTSE[6])
   {
      /* stop at R_MEDLEY_TX_BIS */
      nErrCode = DSL_DRV_VRX_SendMsgLinkFreezeTX(pContext, 0x142);
      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - stop at "
                                  "VDSL2_R_P_MEDLEY_TX failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return DSL_ERROR;
      }

      /* stop at R_C_MEDLEY_RX_BIS */
      nErrCode = DSL_DRV_VRX_SendMsgLinkFreezeRX(pContext, 0x162);
      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - stop at "
                                "VDSL2_R_O_P_MEDLEY_RX failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return DSL_ERROR;
      }
   }
   /* VDSL */
   else if (pXTSE[7])
   {
      /* stop at VDSL2_R_P_MEDLEY_TX */
      nErrCode = DSL_DRV_VRX_SendMsgLinkFreezeTX(pContext, 0x3B);
      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - stop at "
                                  "VDSL2_R_P_MEDLEY_TX failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return DSL_ERROR;
      }

      /* stop at VDSL2_R_O_P_MEDLEY_RX */
      nErrCode = DSL_DRV_VRX_SendMsgLinkFreezeRX(pContext, 0x58);
      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - stop at "
                                  "VDSL2_R_O_P_MEDLEY_RX failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return DSL_ERROR;
      }
   }
   /* all octets are zero */
   else
   {
      DSL_DEBUG( DSL_DBG_WRN,
         (pContext, SYS_DBG_WRN"DSL[%02d]: WARNING  - no link freeze "
                                                     "messages needed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_LinkFreeze"
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
   DSL_VRX_FwVersion_t sFwVersion = {0};

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

#undef DSL_DBG_BLOCK
#define DSL_DBG_BLOCK DSL_DBG_NOTIFICATIONS

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
/* Do not return error for VR10 because this interface is not clean and
   returning an error will restart the autoboot handler! */
#if defined (DSL_VRX_DEVICE_VR9)
         return DSL_ERROR;
#endif
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
/* Do not return error for VR10 because this interface is not clean and
   returning an error will restart the autoboot handler! */
#if defined (DSL_VRX_DEVICE_VR9)
         return DSL_ERROR;
#endif
      }
   }
#endif /* INCLUDE_DSL_ATM_PTM_INTERFACE_SUPPORT*/

   return DSL_SUCCESS;
}

#undef DSL_DBG_BLOCK
#define DSL_DBG_BLOCK DSL_DBG_DEVICE

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
   DSL_DEV_CamStates_t nCamCurrentState;

   DSL_CHECK_CTX_POINTER(pContext);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_ShowtimeStatusUpdate"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Update ReTx status information*/
   nErrCode = DSL_DRV_VRX_ReTxStatusGet(pContext);

   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - ReTx status get failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   /* Update Channel status information*/
   nErrCode = DSL_DRV_DEV_ChannelsStatusUpdate(pContext);

   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - Channels status update failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   memset(&sAck, 0, sizeof(sAck));

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
/* Do not return error for VR10 because this interface is not clean and
   returning an error will restart the autoboot handler! */
#if defined (DSL_VRX_DEVICE_VR9)
         return DSL_ERROR;
#endif
      }
#endif /* INCLUDE_DSL_ATM_PTM_INTERFACE_SUPPORT*/

      /*
      Workaround since FW reports XTSE status after ~1 sec after signalling
      FULL_INIT state
      */
      /* Update XTSE status information*/
      nErrCode = DSL_DRV_VRX_SendMsgXtseStatusGet( pContext );
      if(nErrCode != DSL_SUCCESS)
      {
         DSL_DEBUG( DSL_DBG_ERR, (pContext,
            SYS_DBG_ERR"DSL[%02d]: ERROR - XTSE status get failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         return nErrCode;
      }

      /* Set the showtime indication flag*/
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, bShowtimeReached, DSL_TRUE);

      /* Add EAPS timeout event.*/
      nEapsTimeoutId = DSL_DRV_Timeout_AddEvent(pContext, (DSL_int_t)DSL_TIMEOUTEVENT_EAPS, nEapsTimeout);
      /* Set EAPS timeout ID in the DSL CPE Context*/
      DSL_CTX_WRITE(pContext, nErrCode, nEapsTimeoutId, nEapsTimeoutId);

      nCamCurrentState = DSL_DRV_VRX_CamFsmStateGet(pContext);

      if(nCamCurrentState != DSL_CAM_VDSL_DUAL)
      {
         /* Indicate Single Port Mode showtime */
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.bCamSinglePortModeSteady, DSL_TRUE);

         /* Reset CAM state*/
         nErrCode = DSL_DRV_VRX_CamFsmStateSet(pContext, DSL_CAM_INIT);
         if( nErrCode != DSL_SUCCESS )
         {
            DSL_DEBUG( DSL_DBG_ERR,
               (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - CAM FSM state set failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            return nErrCode;
         }
      }
      else
      {
         /* Indicate Dual Port Mode showtime */
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.bCamDualPortModeSteady, DSL_TRUE);

         /* Reset retry counters only*/
         nErrCode = DSL_DRV_VRX_CamRetryReset(pContext);
         if( nErrCode != DSL_SUCCESS )
         {
            DSL_DEBUG( DSL_DBG_ERR,
               (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - CAM retry reset failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            return nErrCode;
         }
      }

      /* Update XTSE VDSL2 status information*/
      if( DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2) )
      {
         nErrCode = DSL_DRV_VRX_SendMsgSelectedProfileVdsl2Get(pContext);
         if( nErrCode != DSL_SUCCESS )
         {
            DSL_DEBUG( DSL_DBG_ERR, (pContext,
               SYS_DBG_ERR"DSL[%02d]: ERROR - Selected Profile Vdsl2 Get failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            return nErrCode;
         }
      }

      /* VDSL FW does not support Power Management State via message therefore set
         it manually here (should be removed completely if supported by FW where
         it is updated within context of periodical line state update). */
      if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
      {
         /* Update Power Management Status*/
         nErrCode = DSL_DRV_VRX_PowerManagementStatusSet(pContext, DSL_G997_PMS_L0);
         if(nErrCode != DSL_SUCCESS)
         {
            DSL_DEBUG( DSL_DBG_ERR, (pContext,
               SYS_DBG_ERR"DSL[%02d]: ERROR - Power Management State set failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

           return nErrCode;
         }
      }

#ifdef INCLUDE_DSL_DELT
      if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
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

         if(DSL_DRV_MUTEX_LOCK(pContext->dataMutex))
         {
            DSL_DEBUG( DSL_DBG_ERR,
               (pContext, SYS_DBG_ERR"DSL[%02d]: Couldn't lock data mutex"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            return DSL_ERR_SEMAPHORE_GET;
         }

         /* Update VDSL FE group size parameter*/
         pContext->DELT_SHOWTIME->hlogDataUsVdsl.nGroupSize = (DSL_uint8_t)(sAck.HLOGG);
         pContext->DELT_SHOWTIME->qlnDataUsVdsl.nGroupSize  = (DSL_uint8_t)(sAck.QLNG);
         pContext->DELT_SHOWTIME->snrDataUsVdsl.nGroupSize  = (DSL_uint8_t)(sAck.SNRG);

         /* Request firts portion of DELT data from the FE*/
         pContext->eTestParametersFeReady = VRX_FE_TESTPARAMS_FIRST_UPDATING;
         pContext->nTestParametersFeRefreshTimeout = 0;

         DSL_DRV_MUTEX_UNLOCK(pContext->dataMutex);

         nErrCode = DSL_DRV_VRX_SendMsgTestParamsFeRequest(pContext, 0,
                                      VRX_TESTPARAMS_FE_BLOCK_SIZE - 1);
      }
#endif /* INCLUDE_DSL_DELT*/
   }
   else
   {
      nErrCode = DSL_DRV_VRX_DataPathFailuresStatusGet(pContext);
      if(nErrCode != DSL_SUCCESS)
      {
         DSL_DEBUG( DSL_DBG_ERR, (pContext,
            SYS_DBG_ERR"DSL[%02d]: ERROR - Data Path Failures update failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

        return nErrCode;
      }

      /* Update Line failures information*/
      nErrCode = DSL_DRV_VRX_LineFailuresStatusUpdate(pContext);
      if(nErrCode != DSL_SUCCESS)
      {
         DSL_DEBUG( DSL_DBG_ERR, (pContext,
            SYS_DBG_ERR"DSL[%02d]: ERROR - Line Failures update failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

        return nErrCode;
      }

      /* Handle VRX driver autonomous messages*/
      nErrCode = DSL_DRV_VRX_HandleMessage(pContext);

      if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
      {
         /* Handle periodical update */
         nErrCode = DSL_DRV_VRX_TestParametersFePeriodicallyUpdate(pContext);
      }
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
      (pContext, SYS_DBG_MSG"DSL[%02d]: DSL_DRV_VRX_OnTimeoutEvent: "
      "nEventType=%d, nTimeoutID=0x%08X" DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext), nEventType, nTimeoutID));

   switch (nEventType)
   {
#ifdef INCLUDE_DSL_G997_LINE_INVENTORY
   case DSL_TIMEOUTEVENT_FE_STATUS:
      nErrCode = DSL_DRV_VRX_OnLineInventoryFe(pContext);
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

   memset(&sAck, 0, sizeof(sAck));

   /* Temporarily it is assumed that this configuration cannot be changed */
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.sysCIFSts.nEfmTcConfigUs, DSL_EMF_TC_CLEANED);
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.sysCIFSts.nEfmTcConfigDs, DSL_EMF_TC_CLEANED);
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.sysCIFSts.nSystemIf, DSL_SYSTEMIF_MII);

   nErrCode = DSL_DRV_VRX_SendMsgTcStatusGet(pContext, (DSL_uint8_t*)&sAck);
   if( nErrCode < DSL_SUCCESS )
   {
      return nErrCode;
   }

   switch (sAck.TC)
   {
      case ACK_TC_StatusGet_UNKNOWN_TC:
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.sysCIFSts.nTcLayer, DSL_TC_UNKNOWN);
         break;
      case ACK_TC_StatusGet_EFM_TC:
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.sysCIFSts.nTcLayer, DSL_TC_EFM);
         break;
      case ACK_TC_StatusGet_ATM_TC:
         DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.sysCIFSts.nTcLayer, DSL_TC_ATM);
         break;
      default:
         nErrCode = DSL_ERROR;
         DSL_DEBUG( DSL_DBG_ERR, (pContext,
            SYS_DBG_ERR"DSL[%02d]: ERROR - System interface status undefined!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         return nErrCode;
   }

   return nErrCode;
}

DSL_Error_t DSL_DRV_VRX_SystemInterfaceStatusUpdate(
   DSL_IN DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_boolean_t bFwRequest = DSL_FALSE;
   DSL_boolean_t bIfStsValid = DSL_FALSE;
   DSL_DEV_VersionCheck_t nVerCheck = DSL_VERSION_ERROR;
   DSL_TcLayerSelection_t nTcModeFwCfg = DSL_TC_UNKNOWN;
   DSL_SystemInterfaceConfigData_t stsData =
      {DSL_TC_UNKNOWN, DSL_EMF_TC_CLEANED, DSL_EMF_TC_CLEANED,
       DSL_SYSTEMIF_UNKNOWN};

   DSL_DEBUG( DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_SystemInterfaceStatusUpdate"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->data.bSystemIfStatusValid,
                       bIfStsValid);
   if (bIfStsValid == DSL_TRUE)
   {
      /* Handling to update SystemInterface Status is not required if already
         valid */
      return nErrCode;
   }

   /* Get TC-Layer configuration value that is used for current link
      activation.*/
   DSL_CTX_READ_SCALAR( pContext, nErrCode,
      pDevCtx->data.nTcModeFwCfg, nTcModeFwCfg);

   /* Update of TC-Layer status via FW message handling is only necessary in
      case of auto TC (\ref DSL_TC_AUTO) is configured. Otherwise just use the
      configuration value to update API status and generate event. */
   if ((nTcModeFwCfg == DSL_TC_ATM) || (nTcModeFwCfg == DSL_TC_EFM))
   {
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode,
         pDevCtx->data.sysCIFSts.nTcLayer, nTcModeFwCfg);

      /* Temporarily it is assumed that this configuration cannot be changed */
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode,
         pDevCtx->data.sysCIFSts.nEfmTcConfigUs, DSL_EMF_TC_CLEANED);
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode,
         pDevCtx->data.sysCIFSts.nEfmTcConfigDs, DSL_EMF_TC_CLEANED);
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode,
         pDevCtx->data.sysCIFSts.nSystemIf, DSL_SYSTEMIF_MII);
   }
   else
   {
      if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
      {
         bFwRequest = DSL_TRUE;
      }
      else
      {
         /* Check firmware version */
         nErrCode = DSL_DRV_VRX_FirmwareVersionCheck(pContext,
                     DSL_MIN_FW_VERSION_TC_STAT_ADSL, &nVerCheck);
         if (nErrCode != DSL_SUCCESS)
         {
            DSL_DEBUG(DSL_DBG_ERR, (pContext,
               SYS_DBG_ERR"DSL[%02d]: ERROR - FW version check failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
            return nErrCode;
         }

         if (nVerCheck >= DSL_VERSION_EQUAL)
         {
            bFwRequest = DSL_TRUE;
         }
         else
         {
            DSL_CTX_WRITE_SCALAR(pContext, nErrCode,
                                 pDevCtx->data.sysCIFSts.nTcLayer, DSL_TC_ATM);
            DSL_CHECK_ERR_CODE();
         }
      }

      if (bFwRequest == DSL_TRUE)
      {
         nErrCode = DSL_DRV_VRX_SystemInterfaceStatusGet(pContext);
         if(nErrCode != DSL_SUCCESS)
         {
            DSL_DEBUG( DSL_DBG_ERR, (pContext, SYS_DBG_ERR
               "DSL[%02d]: ERROR - System interface status update failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            return nErrCode;
         }
      }
   }

   DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.sysCIFSts, stsData);

   if ((stsData.nTcLayer == DSL_TC_ATM) || (stsData.nTcLayer == DSL_TC_EFM))
   {
      /* Update vilidity flag for TC-Layer status in context */
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode,
                           pDevCtx->data.bSystemIfStatusValid, DSL_TRUE);

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
   }

   DSL_DEBUG( DSL_DBG_MSG, (pContext, SYS_DBG_MSG
      "DSL[%02d]: OUT - DSL_DRV_VRX_SystemInterfaceStatusUpdate, retCode=%d"
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
   if (!DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSL))
   {
      DSL_DEBUG(DSL_DBG_WRN, (pContext, SYS_DBG_WRN"DSL[%02d]: WARNING - "
         "Hybrid/filter/looplength statistics supported only in ADSL mode"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   /* Check firmware version */
   nErrCode = DSL_DRV_VRX_FirmwareVersionCheck(pContext,
               DSL_MIN_FW_VERSION_MFD, &nVerCheck);

   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW version check failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return nErrCode;
   }

   memset(&sAck, 0, sizeof(sAck));

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
   if (!DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSL))
   {
      DSL_DEBUG(DSL_DBG_WRN, (pContext, SYS_DBG_WRN"DSL[%02d]: WARNING - "
         "Hybrid/filter/looplength statistics supported only in ADSL mode"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   /* Check firmware version */
   nErrCode = DSL_DRV_VRX_FirmwareVersionCheck(pContext,
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

   memset(&sResultsAck, 0, sizeof(sResultsAck));
   memset(&sHybridInfoAck, 0, sizeof(sHybridInfoAck));

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
   if (!DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSL))
   {
      DSL_DEBUG(DSL_DBG_WRN, (pContext, SYS_DBG_WRN"DSL[%02d]: WARNING - "
         "Hybrid/filter/looplength statistics supported only in ADSL mode"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   /* Check firmware version */
   nErrCode = DSL_DRV_VRX_FirmwareVersionCheck(pContext,
               DSL_MIN_FW_VERSION_MFD, &nVerCheck);

   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW version check failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return nErrCode;
   }

   memset(&sResultsAck, 0, sizeof(sResultsAck));
   memset(&sHybridInfoAck, 0, sizeof(sHybridInfoAck));

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
   DSL_int_t nDlsModeIdx = pData->nDslMode;
   DSL_DEV_VersionCheck_t nVerCheck = DSL_VERSION_ERROR;

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_SystemInterfaceConfigSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Check firmware version */
   nErrCode = DSL_DRV_VRX_FirmwareVersionCheck(pContext,
               DSL_MIN_FW_VERSION_TC_STAT_ADSL, &nVerCheck);
   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG(DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - FW version check failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return nErrCode;
   }

   /* In case of VDSL2 or ADSL with given minimum FW version */
   if ((nVerCheck >= DSL_VERSION_EQUAL) ||
       (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2)))
   {
      if ((nDlsModeIdx >= 0) && (nDlsModeIdx < DSL_MODE_LAST))
      {
         {
            /* Copy System Configuration data to internal VRx device context*/
            DSL_CTX_WRITE(pContext, nErrCode,
               pDevCtx->data.deviceCfg.sysCIF[nDlsModeIdx],
               pData->data);
         }
      }
      else
      {
         nErrCode = DSL_ERR_INTERNAL;
      }
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
   DSL_int_t nDlsModeIdx = pData->nDslMode;
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_SystemInterfaceConfigGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   if ((nDlsModeIdx >= 0) && (nDlsModeIdx < DSL_MODE_LAST))
   {
      {
         /* Get System Configuration from the internal VRx device context*/
         DSL_CTX_READ(pContext, nErrCode,
            pDevCtx->data.deviceCfg.sysCIF[nDlsModeIdx],
            pData->data);
      }
   }
   else
   {
      nErrCode = DSL_ERR_INTERNAL;
   }

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
   DSL_VRX_FwVersion_t  sFwVersion = {0};
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
   DSL_LineStateValue_t nCurrentState = DSL_LINESTATE_UNKNOWN;

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_SystemInterfaceStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* For VDSL status available only in showtime */
   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
   {
      /* Get current line state*/
      DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineState, nCurrentState);

      /* Only proceed if the line is in SHOWTIME state.*/
      if ( (nCurrentState != DSL_LINESTATE_SHOWTIME_TC_SYNC) &&
           (nCurrentState != DSL_LINESTATE_SHOWTIME_NO_SYNC) )
      {
         DSL_DEBUG(DSL_DBG_ERR, (pContext, SYS_DBG_ERR
            "DSL[%02d]: ERROR - Function is only available in the SHOWTIME!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         nErrCode = DSL_ERR_ONLY_AVAILABLE_IN_SHOWTIME;
      }
   }

   if (nErrCode == DSL_SUCCESS)
   {
      if (pContext->pDevCtx->data.bSystemIfStatusValid == DSL_TRUE)
      {
         /* Get System Configuration from the internal VRx device context. */
         DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.sysCIFSts, pData->data);
      }
      else
      {
         nErrCode = DSL_ERR_DEVICE_NO_DATA;
      }
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

   memset(&sAck, 0, sizeof(sAck));

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

DSL_Error_t DSL_DRV_DEV_OlrStatisticsGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_OUT DSL_OlrStatistics_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   union
   {
      ACK_OLR_US_StatsGet_t US;
      ACK_OLR_DS_StatsGet_t DS;
   } sAckOlr;

   DSL_DEBUG( DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"ERR[%02d]: IN - DSL_DRV_DEV_OlrStatisticsGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Assume that US/DS messages have same format */
   nErrCode = DSL_DRV_VRX_SendMsgOlrStatusGet(pContext, pData->nDirection,
                                                   (DSL_uint8_t*)&sAckOlr.US);

   if (nErrCode < DSL_SUCCESS)
   {
      return nErrCode;
   }

   memset(pData, 0x0, sizeof(DSL_OlrStatistics_t));

   pData->data.nBitswapRequested = sAckOlr.US.BitswapReqs;
   pData->data.nExtBitswapRequested = sAckOlr.US.ExtBitswapReqs;
   pData->data.nBitswapExecuted = sAckOlr.US.BitswapsDone;
   pData->data.nBitswapRejected = sAckOlr.US.BitswapUTCs;
   pData->data.nSraRequested = sAckOlr.US.SRA_Reqs;
   pData->data.nSraExecuted = sAckOlr.US.SRAsDone;
   pData->data.nSraRejected = sAckOlr.US.SRA_UTCs;

   if (pData->nDirection == DSL_DOWNSTREAM)
   {
      pData->data.nBitswapTimeout = sAckOlr.DS.BitswapTimeOuts;
      pData->data.nSraTimeout =  sAckOlr.DS.SRA_TimeOuts;
   }

   DSL_DEBUG( DSL_DBG_MSG, (pContext,
     SYS_DBG_ERR"DSL[%02d]: OUT - DSL_DRV_DEV_OlrStatisticsGet"
     DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

/*
   VRX common implementation
*/

#ifdef INCLUDE_DSL_DELT
/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SnrPerGroupCalculate(
   DSL_Context_t *pContext,
   DSL_uint8_t nGroupSize,
   DSL_G997_NSCData8_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint8_t *pDest = DSL_NULL;
   DSL_uint32_t nAvrg = 0, i = 0;

   pDest = &(pData->nNSCData[0]);
   while((i < pData->nNumData) && (i < DSL_MAX_NSC))
   {
      if (pData->nNSCData[i] == 255)
      {
         /* as soon as one value is not valid,
            the whole group shall be invalid */
         nAvrg += (nGroupSize * 255);
      }
      else
      {
         nAvrg += pData->nNSCData[i];
      }

      if ((i % nGroupSize) == (DSL_uint8_t)(nGroupSize - 1))
      {
         /* one group is summarized */
         *pDest = (DSL_uint8_t)DSL_MIN(255, (nAvrg / nGroupSize));
         nAvrg = 0;
         pDest++;
      }
      i++;
   }

   pData->nNumData = pData->nNumData / nGroupSize;

   return(nErrCode);
}
#endif /* #ifdef INCLUDE_DSL_DELT*/

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_BandPlanStatusGet(
   DSL_Context_t *pContext,
   DSL_BandPlanConfigData_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_BandPlanStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Only Available for VDSL mode*/
   if( DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2) )
   {
      if(DSL_DRV_MUTEX_LOCK(pContext->dataMutex))
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - data mutex lock failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return DSL_ERROR;
      }

      /* Get Band Plan Status from the internal VRX device context*/
      memcpy(
         pData,
         &(pContext->pDevCtx->data.deviceCfg.bandPlanStatusNe),
         sizeof(DSL_BandPlanConfigData_t));

      DSL_DRV_MUTEX_UNLOCK(pContext->dataMutex);
   }
   else
   {
      nErrCode = DSL_ERR_NOT_SUPPORTED_BY_FIRMWARE;
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_BandPlanStatusGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_CheckTruncParamRange(
   DSL_Context_t *pContext,
   DSL_int_t nMin, DSL_int_t nMax, DSL_int_t *pVal, DSL_char_t *pName)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_int_t originalValue = 0;

   DSL_CHECK_POINTER(pContext, pVal);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_POINTER(pContext, pName);
   DSL_CHECK_ERR_CODE();

   originalValue = *pVal;

   if( *pVal < nMin )
   {
      *pVal = nMin;

      DSL_DEBUG(DSL_DBG_WRN,
         (pContext, SYS_DBG_WRN"DSL[%02d]: WARNING - Parameter (%s, %d) truncated "
         "to min value (%d)"DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
         pName,originalValue,nMin));

      nErrCode = DSL_WRN_CONFIG_PARAM_TRUNCATED;
   }
   else if( *pVal > nMax )
   {
      *pVal = nMax;

      DSL_DEBUG(DSL_DBG_WRN,
         (pContext, SYS_DBG_WRN"DSL[%02d]: WARNING - Parameter (%s, %d) truncated"
         " to max value (%d)"DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
         pName,originalValue,nMax));

      nErrCode = DSL_WRN_CONFIG_PARAM_TRUNCATED;
   }

   return nErrCode;
}

DSL_Error_t DSL_DRV_VRX_ChannelStatusGet(
   DSL_Context_t *pContext,
   DSL_uint8_t nChannel,
   DSL_AccessDir_t nDirection,
   DSL_G997_ChannelStatusData_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CHECK_ATU_DIRECTION(nDirection);
   DSL_CHECK_ERR_CODE();

   DSL_CTX_READ_SCALAR(pContext, nErrCode,
      nChannelActualDataRate[nDirection][nChannel],
      pData->ActualDataRate);

   DSL_CTX_READ_SCALAR(pContext, nErrCode,
      nChannelPreviousDataRate[nDirection][nChannel],
      pData->PreviousDataRate);

   DSL_CTX_READ_SCALAR(pContext, nErrCode,
      ActualInterleaveDelay[nDirection][nChannel],
      pData->ActualInterleaveDelay);

   DSL_CTX_READ_SCALAR(pContext, nErrCode,
      ActualImpulseNoiseProtection[nDirection][nChannel],
      pData->ActualImpulseNoiseProtection);

   DSL_CTX_READ_SCALAR(pContext, nErrCode,
      nChannelActualNetDataRate[nDirection][nChannel],
      pData->ActualNetDataRate);

   DSL_CTX_READ_SCALAR(pContext, nErrCode,
      ActualImpulseNoiseProtectionRein[nDirection][nChannel],
      pData->ActualImpulseNoiseProtectionRein);

   DSL_CTX_READ_SCALAR(pContext, nErrCode,
      ActualImpulseNoiseProtectionNoErasure[nDirection][nChannel],
      pData->ActualImpulseNoiseProtectionNoErasure);

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_PowerManagementStatusSet(
   DSL_Context_t *pContext,
   const DSL_G997_PowerManagement_t nPMMode)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_G997_PowerManagementStatusData_t nPrePmMode = {DSL_G997_PMS_NA};

   DSL_CTX_READ(pContext, nErrCode, powerMgmtStatus, nPrePmMode);

   if( nPrePmMode.nPowerManagementStatus != nPMMode )
   {
      nPrePmMode.nPowerManagementStatus = nPMMode;
      DSL_CTX_WRITE(pContext, nErrCode, powerMgmtStatus, nPrePmMode);

      /* If event procedure is enabled inform the upper software layer
         about the new power management state changes */
      nErrCode = DSL_DRV_EventGenerate(
                    pContext, 0, DSL_ACCESSDIR_NA, DSL_XTUDIR_NA,
                    DSL_EVENT_S_LINE_POWERMANAGEMENT_STATE,
                    (DSL_EventData_Union_t*)&nPrePmMode,
                    sizeof(DSL_G997_PowerManagementStatusData_t));

      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Event(%d) generate failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
            DSL_EVENT_S_LINE_POWERMANAGEMENT_STATE));
      }
   }

   return (nErrCode);
}

static DSL_Error_t DSL_DRV_VRX_OnEventChannelDataRateUpdate(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDir)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_G997_ChannelStatusData_t chStsData;

   nErrCode = DSL_DRV_VRX_ChannelStatusGet(pContext, 0, nDir, &chStsData);
   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Channel status %s get failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nDir == DSL_UPSTREAM ? "US" : "DS"));

      return nErrCode;
   }

   nErrCode = DSL_DRV_EventGenerate(
                  pContext, 0, nDir, DSL_XTUDIR_NA, DSL_EVENT_S_CHANNEL_DATARATE,
                  (DSL_EventData_Union_t*)&chStsData,
                  sizeof(DSL_G997_ChannelStatusData_t));

   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Event(%d) generate failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), DSL_EVENT_S_CHANNEL_DATARATE));

      return nErrCode;
   }

   return nErrCode;
}

#ifdef INCLUDE_DSL_G997_ALARM
static DSL_Error_t DSL_DRV_VRX_OnEventChannelDataRateThresholdCrossingUpdate(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDir,
   DSL_uint32_t *pOldDr,
   DSL_uint32_t *pNewDr)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_boolean_t bNotifyDataRateShift = DSL_FALSE;
   DSL_uint32_t nDataRateShiftThresh = 0;
   DSL_G997_DataRateShiftThresholdCrossingData_t DataRateShiftThresholdCrossingData;
#ifdef INCLUDE_DSL_ADSL_MIB
   DSL_MIB_ADSL_Thresholds_t nMibThresholds = 0;
#endif

   DSL_CHECK_POINTER(pContext, pOldDr);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_POINTER(pContext, pNewDr);
   DSL_CHECK_ERR_CODE();

   /* Check for the Data Rate Upshift*/
   if( *pNewDr > *pOldDr )
   {
      DSL_CTX_READ_SCALAR(pContext, nErrCode,
         channelDataRateThreshold[nDir].nDataRateThresholdUpshift, nDataRateShiftThresh);

      if( *pNewDr - *pOldDr > nDataRateShiftThresh)
      {
         /* Upshift occured*/
         bNotifyDataRateShift = DSL_TRUE;
         DataRateShiftThresholdCrossingData.nDataRateThresholdType =
            DSL_G997_DATARATE_THRESHOLD_UPSHIFT;
      }
   }

   /* Check for the Data Rate Downshift*/
   if( *pNewDr < *pOldDr )
   {
      DSL_CTX_READ_SCALAR(pContext, nErrCode,
         channelDataRateThreshold[nDir].nDataRateThresholdDownshift, nDataRateShiftThresh);

      if( *pOldDr - *pNewDr > nDataRateShiftThresh)
      {
         bNotifyDataRateShift = DSL_TRUE;
         /* Downshift occured*/
         DataRateShiftThresholdCrossingData.nDataRateThresholdType =
            DSL_G997_DATARATE_THRESHOLD_DOWNSHIFT;
      }
   }

   if( bNotifyDataRateShift && nErrCode == DSL_SUCCESS)
   {
      nErrCode = DSL_DRV_EventGenerate(
                     pContext, 0, nDir, DSL_XTUDIR_NA,
                     DSL_EVENT_I_CHANNEL_DATARATE_SHIFT_THRESHOLD_CROSSING,
                     (DSL_EventData_Union_t*)&DataRateShiftThresholdCrossingData,
                     sizeof(DSL_G997_DataRateShiftThresholdCrossingData_t));

      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Event(%d) generate failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
            DSL_EVENT_I_CHANNEL_DATARATE_SHIFT_THRESHOLD_CROSSING));

         return nErrCode;
      }
#ifdef INCLUDE_DSL_ADSL_MIB
      DSL_CTX_READ_SCALAR(pContext, nErrCode, MibAdslCtx.nThresholds,
         nMibThresholds);
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, MibAdslCtx.nThresholds,
         nMibThresholds | ((nDir == DSL_UPSTREAM) ?
         DSL_MIB_THRESHOLD_ATUR_RATE_CHANGE_FLAG :
         DSL_MIB_THRESHOLD_ATUC_RATE_CHANGE_FLAG));
#endif /* INCLUDE_DSL_ADSL_MIB*/
   }

   return nErrCode;
}
#endif /* #ifdef INCLUDE_DSL_G997_ALARM*/

DSL_Error_t DSL_DRV_VRX_DataRateUpdate(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDir,
   DSL_uint32_t dataRate)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint32_t oldDataRate = 0;

   DSL_CHECK_DIRECTION(nDir);
   DSL_CHECK_ERR_CODE();

   DSL_CTX_READ_SCALAR(pContext, nErrCode, nChannelActualDataRatePrev[nDir][0], oldDataRate);
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, nChannelActualDataRatePrev[nDir][0], dataRate);

   if (dataRate != oldDataRate)
   {
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode, nChannelPreviousDataRate[nDir][0], oldDataRate);
   }

   if( dataRate != oldDataRate )
   {
      nErrCode = DSL_DRV_VRX_OnEventChannelDataRateUpdate(pContext, nDir);
   }

#ifdef INCLUDE_DSL_G997_ALARM
   if( oldDataRate != 0 && nErrCode == DSL_SUCCESS )
   {
      nErrCode = DSL_DRV_VRX_OnEventChannelDataRateThresholdCrossingUpdate(
                    pContext, nDir,
                    &oldDataRate, &dataRate);
   }
#endif /* INCLUDE_DSL_G997_ALARM*/

   return nErrCode;
}

static DSL_Error_t DSL_DRV_VRX_LoopDiagnosticCompleteHandle(DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
#ifdef INCLUDE_DSL_DELT
   DSL_uint8_t nAutoCount = 0;
   DSL_G997_LDSF_t nLoopMode = DSL_G997_INHIBIT_LDSF;
#endif /* INCLUDE_DSL_DELT*/

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_LoopDiagnosticCompleteHandle"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

#ifdef INCLUDE_DSL_DELT
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nLoopAutoCount, nAutoCount);
   if( nAutoCount > 0 )
   {
      nAutoCount -= 1;
      DSL_CTX_WRITE(pContext, nErrCode, nLoopAutoCount, nAutoCount);
   }

   DSL_CTX_READ_SCALAR(pContext, nErrCode, lineActivateConfig.nLDSF, nLoopMode);
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, bLoopDiagnosticsCompleted, DSL_TRUE);

   #ifndef DSL_CPE_STATIC_DELT_DATA
   if (pContext->DELT == DSL_NULL)
   {
      pContext->DELT = DSL_DRV_Malloc(sizeof(DSL_G997_DeltData_t));
      if( pContext->DELT == DSL_NULL )
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - memory for DELT data "
            "could not be allocated!"DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return nErrCode;
      }
   }
   #endif /* #ifndef DSL_CPE_STATIC_DELT_DATA*/

   memset(pContext->DELT, 0x0, sizeof(DSL_G997_DeltData_t));


   /* Update XTSE status information*/
   nErrCode = DSL_DRV_VRX_SendMsgXtseStatusGet( pContext );
   if(nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - XTSE status get failed!" DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));

     return nErrCode;
   }

   if( DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2) )
   {
      nErrCode = DSL_DRV_VRX_SendMsgSelectedProfileVdsl2Get(pContext);
      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG( DSL_DBG_ERR, (pContext,
            SYS_DBG_ERR"DSL[%02d]: ERROR - Selected Profile Vdsl2 Get failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return nErrCode;
      }
   }

   /* Store all DELT diagnostic data*/
   nErrCode = DSL_DRV_VRX_DiagnosticDeltDataGet(pContext);
   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - DELT data save failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

       return nErrCode;
   }
#endif /* INCLUDE_DSL_DELT*/

   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
   {
      /* Send LINKRES message even if the CPE didn't triggered the DELT measurements.
         The configuration and state variables keep their values. FE DELT data will be requested
         later in the showtime. All NE data also will be available in the showtime*/
      nErrCode = DSL_DRV_DEV_LinkReset(pContext);

      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Link Reset failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return nErrCode;
      }

#ifdef INCLUDE_DSL_DELT
      if( nLoopMode == DSL_G997_AUTO_LDSF )
      {
         nErrCode = DSL_DRV_VRX_SendMsgModemFsmOptionsSet(pContext, DSL_FALSE, DSL_FALSE);
         if( nErrCode!=DSL_SUCCESS )
         {
            DSL_DEBUG( DSL_DBG_ERR,
               (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Modem FSM options set failed!"
               "<%d>"DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

            return nErrCode;
         }
      }
#endif /* INCLUDE_DSL_DELT*/

      nErrCode = DSL_DRV_VRX_SendMsgModemStateSet(
                pContext, CMD_ModemFSM_StateSet_LINKINI);
      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Link Initialization failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return nErrCode;
      }
   }
   else
   {
      /* Reboot modem*/
      nErrCode = DSL_DRV_AutobootStateSet(
                    pContext,
                    DSL_AUTOBOOTSTATE_RESTART,
                    DSL_AUTOBOOT_RESTART_POLL_TIME);
   }

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_LoopDiagnosticCompleteHandle"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

#ifdef INCLUDE_DSL_CPE_MISC_LINE_STATUS
static DSL_Error_t DSL_DRV_VRX_AdslBandBorderStatusGet(
   DSL_Context_t *pContext,
   DSL_BandBorderStatus_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_G997_NSCData8_t *pBitAllocationNsc = DSL_NULL;
   DSL_uint16_t nToneIdx = 0;
   DSL_AccessDir_t nDirection;
   DSL_Band_t bandLimits[DSL_ACCESSDIR_LAST];
   DSL_Band_t bandBorders[DSL_ACCESSDIR_LAST];

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_AdslBandBorderStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* For the ADSL we have only one Band*/

   /* Allocate BAT*/
   pBitAllocationNsc = DSL_DRV_Malloc(sizeof(DSL_G997_NSCData8_t));
   if (!pBitAllocationNsc)
   {
      DSL_DEBUG(DSL_DBG_ERR,(pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - BAT memory allocation failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return DSL_ERROR;
   }

   for (nDirection = DSL_UPSTREAM; nDirection < DSL_ACCESSDIR_LAST; nDirection++)
   {
      /* Get Band Limits */
      nErrCode =  DSL_DRV_AdslBandLimitsGet(pContext, nDirection, &bandLimits[nDirection]);
      if (nErrCode != DSL_SUCCESS)
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Band limits %s get failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
            nDirection == DSL_UPSTREAM ? "US" : "DS"));
         break;
      }

      /* Get Band Borders*/
      memset(pBitAllocationNsc, 0x0, sizeof(DSL_G997_NSCData8_t));

      /* Get Bit Allocation Table*/
      nErrCode = DSL_DRV_VRX_BitAllocationTableGet(pContext,
                  nDirection, pBitAllocationNsc, DSL_NULL);

      if (nErrCode != DSL_SUCCESS)
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - BAT %s get failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
            nDirection == DSL_UPSTREAM ? "US" : "DS"));
         break;
      }

      /* Find Band Border first tone index*/
      for (nToneIdx = 0; nToneIdx < pBitAllocationNsc->nNumData; nToneIdx++)
      {
         if (pBitAllocationNsc->nNSCData[nToneIdx] != 0)
         {
            bandBorders[nDirection].nFirstToneIndex = nToneIdx;
            break;
         }
      }

      /* Find Band Border last tone index. SC with index 0 is a DC component which is not
        used for the transmission*/
      for (nToneIdx = (pBitAllocationNsc->nNumData - 1);
           (nToneIdx > 0) && (nToneIdx < DSL_MAX_NSC); nToneIdx--)
      {
         if (pBitAllocationNsc->nNSCData[nToneIdx] != 0)
         {
            bandBorders[nDirection].nLastToneIndex = nToneIdx;
            break;
         }
      }
   }

   /* Free BAT*/
   DSL_DRV_MemFree(pBitAllocationNsc);

   if (nErrCode != DSL_SUCCESS)
   {
      return nErrCode;
   }

   /* Check if Borders from BAT fits to limits from standard*/
   if ((bandBorders[pData->nDirection].nFirstToneIndex < bandLimits[pData->nDirection].nFirstToneIndex) ||
       (bandBorders[pData->nDirection].nLastToneIndex  > bandLimits[pData->nDirection].nLastToneIndex))
   {
      DSL_DEBUG( DSL_DBG_WRN,
         (pContext, SYS_DBG_WRN"DSL[%02d]: WARNING - out of limits Band Border detected!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));
      nErrCode = DSL_WRN_FW_BB_STANDARD_VIOLATION;
   }

   /* Special check to cover issue issue with Split point configuration*/
   if (bandLimits[DSL_UPSTREAM].nLastToneIndex >= bandBorders[DSL_DOWNSTREAM].nFirstToneIndex)
   {
      bandLimits[DSL_UPSTREAM].nLastToneIndex = bandBorders[DSL_UPSTREAM].nLastToneIndex;
      bandLimits[DSL_DOWNSTREAM].nFirstToneIndex = bandBorders[DSL_DOWNSTREAM].nFirstToneIndex;

      DSL_DEBUG( DSL_DBG_WRN,
         (pContext, SYS_DBG_WRN"DSL[%02d]: WARNING - Band split point issue found!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

      nErrCode = DSL_WRN_FW_BB_STANDARD_VIOLATION;
   }

   /* Only one band*/
   pData->data.nNumData = 1;

   pData->data.nBandLimits[0].nFirstToneIndex = bandLimits[pData->nDirection].nFirstToneIndex;
   pData->data.nBandLimits[0].nLastToneIndex  = bandLimits[pData->nDirection].nLastToneIndex;

   pData->data.nBandBorder[0].nFirstToneIndex = bandBorders[pData->nDirection].nFirstToneIndex;
   pData->data.nBandBorder[0].nLastToneIndex  = bandBorders[pData->nDirection].nLastToneIndex;

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_AdslBandBorderStatusGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return (nErrCode);
}

static DSL_Error_t DSL_DRV_VRX_VdslBandBorderStatusGet(
   DSL_Context_t *pContext,
   DSL_BandBorderStatus_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint16_t nBand;
   DSL_BandList_t bandList;
   DSL_G997_NSCData8_t *pBitAllocationNsc = DSL_NULL;
   DSL_uint16_t nToneIdx = 0;

   /*
      Get Band Limits
   */
   nErrCode = DSL_DRV_VRX_SendMsgBandControlGet(
                 pContext, pData->nDirection, &bandList);

   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Band Limits get failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return nErrCode;
   }

   /* Fill Band Limits*/
   pData->data.nNumData = bandList.nNumData;

   memcpy(
      &(pData->data.nBandLimits),
      &(bandList.nBand), sizeof((pData->data.nBandLimits)));

   /*
      Get Band Borders
   */
   pBitAllocationNsc = DSL_DRV_Malloc(sizeof(DSL_G997_NSCData8_t));
   if (!pBitAllocationNsc)
   {
      DSL_DEBUG(DSL_DBG_ERR,(pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - BAT memory allocation failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return DSL_ERROR;
   }

   memset(pBitAllocationNsc, 0x0, sizeof(DSL_G997_NSCData8_t));

   /* Get Bit Allocation Table*/
   nErrCode = DSL_DRV_VRX_BitAllocationTableGet(pContext,
               pData->nDirection, pBitAllocationNsc, DSL_NULL);

   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - BAT %s get failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
         pData->nDirection == DSL_UPSTREAM ? "US" : "DS"));

      /* Free BAT*/
      DSL_DRV_MemFree(pBitAllocationNsc);

      return nErrCode;
   }

   for (nBand = 0; (nBand < bandList.nNumData) &&
                   (nBand < DSL_MAX_NUMBER_OF_BANDS); nBand++)
   {
      /* Find First Band Border index*/
      for (nToneIdx = bandList.nBand[nBand].nFirstToneIndex;
           (nToneIdx <= bandList.nBand[nBand].nLastToneIndex) &&
           (nToneIdx < DSL_MAX_NSC); nToneIdx++)
      {
         if (pBitAllocationNsc->nNSCData[nToneIdx])
         {
            pData->data.nBandBorder[nBand].nFirstToneIndex = nToneIdx;
            break;
         }
      }


      /* Find Band Border last tone index. SC with index 0 is a DC component which is not
        used for the transmission*/
      for (nToneIdx = bandList.nBand[nBand].nLastToneIndex;
           (nToneIdx >= bandList.nBand[nBand].nFirstToneIndex) &&
           (nToneIdx > 0) && (nToneIdx < DSL_MAX_NSC); nToneIdx--)
      {
         if (pBitAllocationNsc->nNSCData[nToneIdx])
         {
            pData->data.nBandBorder[nBand].nLastToneIndex = nToneIdx;
            break;
         }
      }
   }

   /* Free BAT*/
   DSL_DRV_MemFree(pBitAllocationNsc);

   return nErrCode;
}
#endif /* INCLUDE_DSL_CPE_MISC_LINE_STATUS*/

#ifdef INCLUDE_DSL_G997_LINE_INVENTORY
DSL_Error_t DSL_DRV_VRX_OnLineInventoryFe(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   /* Retreive all FE inventory information*/
   nErrCode = DSL_DRV_VRX_SendMsgInventoryFeGet(pContext);
   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FE inventory get failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   nErrCode = DSL_DRV_EventGenerate(
                 pContext, 0, DSL_ACCESSDIR_NA, DSL_FAR_END,
                 DSL_EVENT_S_FE_INVENTORY_AVAILABLE,
                 DSL_NULL,
                 0);

   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Event(%d) generate failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
         DSL_EVENT_S_FE_INVENTORY_AVAILABLE));

      return nErrCode;
   }

   return nErrCode;
}
#endif /* INCLUDE_DSL_G997_LINE_INVENTORY*/

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_BandPlanSupportedGet(
   DSL_Context_t *pContext,
   DSL_BandPlanSupportData_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_ProfileType_t nProfile   = (DSL_ProfileType_t)0;
   DSL_BandPlanType_t nBandPlan = (DSL_BandPlanType_t)0;
   DSL_BandPlanConfigData_t sBpConfig;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: Supported (Bandplan, Profile) combinations:"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   if( DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2) )
   {
      for( nBandPlan = (DSL_BandPlanType_t)0; nBandPlan < DSL_BANDPLAN_LAST; nBandPlan++ )
      {
         sBpConfig.nBandPlan = (DSL_BandPlanType_t)nBandPlan;
         for (nProfile = (DSL_ProfileType_t)0; nProfile < DSL_PROFILE_LAST; nProfile++)
         {
            sBpConfig.nProfile = (DSL_ProfileType_t)nProfile;
            pData->bSupported[nBandPlan][nProfile] =
               DSL_DRV_VRX_BandPlanSupportedCheck(pContext, &sBpConfig);
               DSL_DEBUG( DSL_DBG_MSG, (pContext,
               SYS_DBG_MSG"%d ", pData->bSupported[nBandPlan][nProfile]) );
         }
         DSL_DEBUG( DSL_DBG_MSG, (pContext, SYS_DBG_MSG""DSL_DRV_CRLF ) );
      }
   }
   else
   {
      nErrCode = DSL_ERR_NOT_SUPPORTED_BY_FIRMWARE;
   }

   return nErrCode;
}

DSL_Error_t DSL_DRV_VRX_FirmwareVersionCheck(
   DSL_Context_t *pContext,
   DSL_int_t nFs,
   DSL_int_t nMajVer,
   DSL_int_t nMinVer,
   DSL_int_t nRs,
   DSL_int_t nApp,
   DSL_DEV_VersionCheck_t *pVerCheck)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint32_t nActUsedVer = 0, nVer = 0;
   DSL_VRX_FwVersion_t firmwareVersion = {0};

   DSL_CHECK_POINTER(pContext, pVerCheck);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_FirmwareVersionCheck" DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext)));

   *pVerCheck = DSL_VERSION_ERROR;

   /* Get FW version*/
   DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.fwFeatures, firmwareVersion);

   if (nApp != -1 && firmwareVersion.nApplication != nApp)
   {
      *pVerCheck = DSL_VERSION_MISMATCH;
      return nErrCode;
   }

   if (nRs != -1 && firmwareVersion.nReleaseStatus != nRs)
   {
      /* Do nothing here because release status shall not be taken into account
         for firmware version check (DSLCPE_SW-702) */
   }

   if (nFs != -1)
   {
      nVer = ((DSL_uint32_t)nFs) << 8;
      nActUsedVer = ((DSL_uint32_t)firmwareVersion.nFeatureSet) << 8;
   }

   if (nMajVer != -1)
   {
      nVer |= (((DSL_uint32_t)nMajVer) & 0xF) << 4;
      nActUsedVer |= ((DSL_uint32_t)firmwareVersion.nMajor) << 4;
   }

   if (nMinVer != -1)
   {
      nVer |= (((DSL_uint32_t)nMinVer) & 0xF);
      nActUsedVer |= ((DSL_uint32_t)firmwareVersion.nMinor);
   }

   /* Check both generated 32-bit values */
   if (nActUsedVer < nVer)
   {
      *pVerCheck = DSL_VERSION_SMALLER;
   }
   else if (nActUsedVer > nVer)
   {
      *pVerCheck = DSL_VERSION_BIGGER;
   }
   else
   {
      *pVerCheck = DSL_VERSION_EQUAL;
   }

   DSL_DEBUG(DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_FirmwareVersionCheck" DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext)));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device.h'
*/
DSL_boolean_t DSL_DRV_DEV_FirmwareFeatureCheck(
   DSL_Context_t *pContext,
   DSL_FirmwareXdslFeature_t nFeature)
{
   DSL_FirmwareXdslFeature_t nFeatureSet = DSL_FW_XDSLFEATURE_CLEANED;
   DSL_boolean_t bReturn = DSL_FALSE;

   if (nFeature == DSL_FW_XDSLFEATURE_CLEANED)
   {
      DSL_DEBUG( DSL_DBG_WRN,
         (pContext, SYS_DBG_WRN"DSL[%02d]: WARNING - Incorrect xDSL feature passed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }
   else
   {
      if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
      {
         nFeatureSet = pContext->nFwFeatures.nFirmwareVdslFeatures;
      }
      else if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSL))
      {
         nFeatureSet = pContext->nFwFeatures.nFirmwareAdslFeatures;
      }
      else
      {
         nFeatureSet = DSL_FW_XDSLFEATURE_CLEANED;

         DSL_DEBUG( DSL_DBG_WRN,
            (pContext, SYS_DBG_WRN"DSL[%02d]: WARNING - Firmware xDSL mode cannot be checked correctly!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      }

      if ((nFeatureSet & nFeature) != 0)
      {
         bReturn = DSL_TRUE;
      }
   }

   DSL_DEBUG( DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: DSL_DRV_DEV_FirmwareFeatureCheck %d vs FW %d, result %d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nFeature, nFeatureSet, bReturn));

   return (bReturn);
}

#ifdef INCLUDE_DSL_CPE_MISC_LINE_STATUS
DSL_Error_t DSL_DRV_DEV_Annex_M_J_UsBandBordersStatusGet(
   DSL_Context_t *pContext,
   DSL_Band_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint8_t XTSE[DSL_G997_NUM_XTSE_OCTETS] = {0};

   /*Get current xTSE octets*/
   DSL_CTX_READ(pContext, nErrCode, xtseCurr, XTSE);

   if ((XTSE[5-1] & XTSE_5_07_M_3_NO) || (XTSE[7-1] & XTSE_7_03_M_5_NO))
   {
      /* Mask:
         M(n+1)=6..(32+n*4); with n=0..7;
         M(n+1)=6..(32+n*4-1); with n=8;
         In the future FW versions it is neccessary to
         get the Mask information and assign band limits accordingly.
         Currently take the highest US tone*/
      pData->nFirstToneIndex = 6;
      pData->nLastToneIndex  = 63;
   }
   else if ((XTSE[4-1] & XTSE_4_07_J_3_NO) || (XTSE[7-1] & XTSE_7_01_J_5_NO))
   {
      /* Mask:
         M(n+1)=1..(32+n*4); with n=0..7;
         M(n+1)=1..(32+n*4-1); with n=8;
         In the future FW versions it is neccessary to
         get the Mask information and assign band limits accordingly.
         Currently take the highest US tone*/
      pData->nFirstToneIndex = 1;
      pData->nLastToneIndex  = 63;
   }
   else
   {
      nErrCode = DSL_ERR_NOT_SUPPORTED_IN_CURRENT_ADSL_MODE_OR_ANNEX;
   }

   return nErrCode;
}

/*
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_BAND_BORDER_STATUS_GET
*/
DSL_Error_t DSL_DRV_DEV_BandBorderStatusGet(
   DSL_Context_t *pContext,
   DSL_BandBorderStatus_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_LineStateValue_t nLineState = DSL_LINESTATE_UNKNOWN;

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_BandBorderStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Get Line State*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineState, nLineState);

   /* Only proceed if the specified line is in SHOWTIME state.*/
   if (nLineState != DSL_LINESTATE_SHOWTIME_TC_SYNC)
   {
      DSL_DEBUG(DSL_DBG_ERR,(pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - Function is only available if line is in showtime state!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERR_ONLY_AVAILABLE_IN_SHOWTIME;
   }

   /* Get Band Border status according to the current xDSL mode*/
   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
   {
      /* Call VDSL specific implementation*/
      nErrCode = DSL_DRV_VRX_VdslBandBorderStatusGet(pContext, pData);
   }
   else if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSL))
   {
      /* Call ADSL specific implementation*/
      nErrCode = DSL_DRV_VRX_AdslBandBorderStatusGet(pContext, pData);
   }
   else
   {
      nErrCode = DSL_ERR_NOT_SUPPORTED;
   }

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_BandBorderStatusGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return (nErrCode);
}
#endif /* #ifdef INCLUDE_DSL_CPE_MISC_LINE_STATUS*/

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device.h'
*/
#ifdef INCLUDE_DSL_FRAMING_PARAMETERS
DSL_Error_t DSL_DRV_DEV_FramingParameterStatusGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_FramingParameterStatus_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_LineStateValue_t nCurrentState = DSL_LINESTATE_UNKNOWN;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_DIRECTION(pData->nDirection);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_CHANNEL_RANGE(pData->nChannel);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_FramingParameterStatusGet,"
      " (nDirection=%d, nChannel=%d)"DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
      pData->nDirection, pData->nChannel));

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
      nErrCode = DSL_DRV_VRX_FramingParametersVdsl2StatusGet(
                    pContext, pData->nChannel,
                    pData->nDirection,&(pData->data));
   }
   else if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSL))
   {
      /* ADSL mode */
      nErrCode = DSL_DRV_VRX_FramingParametersAdslStatusGet(
                    pContext, pData->nChannel,
                    pData->nDirection,&(pData->data));
   }
   else
   {
      nErrCode = DSL_ERR_NOT_SUPPORTED_BY_FIRMWARE;
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_FramingParameterStatusGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}
#endif /* INCLUDE_DSL_FRAMING_PARAMETERS*/

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device.h'
*/
DSL_Error_t DSL_DRV_DEV_LineStateGet(
   DSL_Context_t *pContext,
   DSL_LineStateValue_t *pnLineState)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_boolean_t bFwReady = DSL_FALSE, bLxStateValid;
   DSL_G997_PowerManagement_t nPMMode = DSL_G997_PMS_NA;
   ACK_ModemFSM_StateGet_t sAck;

   DSL_CHECK_POINTER(pContext, pnLineState);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_POINTER(pContext, pContext->pDevCtx);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_POINTER(pContext, pContext->pDevCtx->lowHandle);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_LineStateGet" DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext)));

   memset(&sAck, 0, sizeof(sAck));

   /* Get FW DL information*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->bFirmwareReady, bFwReady);

   if( bFwReady )
   {
      nErrCode = DSL_DRV_VRX_SendMsgModemFsmStateGet(pContext,(DSL_uint8_t*)&sAck);
      if(nErrCode != DSL_SUCCESS)
      {
         DSL_DEBUG( DSL_DBG_MSG, (pContext,
            SYS_DBG_MSG"DSL[%02d]: ERROR - Modem FSM state get failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         return DSL_ERROR;
      }

      bLxStateValid = sAck.Length == 0x2 ? DSL_TRUE : DSL_FALSE;

      DSL_DEBUG( DSL_DBG_MSG, (pContext,
         SYS_DBG_MSG"DSL[%02d]: DSL_DEV_LineStateGet: Got %hu state"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), sAck.ModemState));

      /* Map states to internal*/
      switch(sAck.ModemState)
      {
      case ACK_ModemFSM_StateGet_RESET_STATE:
         *pnLineState = DSL_LINESTATE_IDLE;
         break;
      case ACK_ModemFSM_StateGet_READY_STATE:
         *pnLineState = DSL_LINESTATE_SILENT;
         break;
      case ACK_ModemFSM_StateGet_FAIL_STATE:
         *pnLineState = DSL_LINESTATE_EXCEPTION;
         break;
      case ACK_ModemFSM_StateGet_DIAG_COMPLETE_STATE:
         *pnLineState = DSL_LINESTATE_LOOPDIAGNOSTIC_COMPLETE;
         break;
      case ACK_ModemFSM_StateGet_T1413_STATE:
         *pnLineState = DSL_LINESTATE_T1413;
         break;
      case ACK_ModemFSM_StateGet_PRE_FAIL_STATE:
         *pnLineState = DSL_LINESTATE_ORDERLY_SHUTDOWN;
         break;
      case ACK_ModemFSM_StateGet_GHS_STATE:
         *pnLineState = DSL_LINESTATE_HANDSHAKE;
         break;
#ifdef INCLUDE_DSL_BONDING
      case ACK_ModemFSM_StateGet_GHS_BONDING_CLR_STATE:
         *pnLineState = DSL_LINESTATE_BONDING_CLR;
         break;
#endif /* INCLUDE_DSL_BONDING*/
      case ACK_ModemFSM_StateGet_FULLINIT_STATE:
         *pnLineState = DSL_LINESTATE_FULL_INIT;
         break;
      case ACK_ModemFSM_StateGet_STEADY_STATE_TC_NOSYNC:
         *pnLineState = DSL_LINESTATE_SHOWTIME_NO_SYNC;
         break;
      case ACK_ModemFSM_StateGet_STEADY_STATE_TC_SYNC:
         *pnLineState = DSL_LINESTATE_SHOWTIME_TC_SYNC;
         break;
      case ACK_ModemFSM_StateGet_DIAGMODE_STATE:
         *pnLineState = DSL_LINESTATE_LOOPDIAGNOSTIC_ACTIVE;
         break;
      case ACK_ModemFSM_StateGet_TEST_STATE:
         *pnLineState = DSL_LINESTATE_TEST;
         break;
#ifdef INCLUDE_DSL_FILTER_DETECTION
      case ACK_ModemFSM_StateGet_MFD_STATE:
         *pnLineState = DSL_LINESTATE_TEST_FILTERDETECTION_ACTIVE;
         break;
      case ACK_ModemFSM_StateGet_MFD_COMPLETE_STATE:
         *pnLineState = DSL_LINESTATE_TEST_FILTERDETECTION_COMPLETE;
         break;
#endif /* INCLUDE_DSL_FILTER_DETECTION */
      default:
         DSL_DEBUG( DSL_DBG_MSG,
         (pContext, SYS_DBG_MSG"DSL[%02d]: DSL_DEV_LineStateGet: "
         "DSL_LINESTATE_UNKNOWN will be returned" DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         *pnLineState = DSL_LINESTATE_UNKNOWN;

         break;
      }

      /* Check if the LxState is available from the FW*/
      if (bLxStateValid)
      {
         switch(sAck.LxState)
         {
         case ACK_ModemFSM_StateGet_L0:
            nPMMode = DSL_G997_PMS_L0;
            break;
         case ACK_ModemFSM_StateGet_L2:
            nPMMode = DSL_G997_PMS_L2;
            break;
         case ACK_ModemFSM_StateGet_L3:
            nPMMode = DSL_G997_PMS_L3;
            break;
         default:
            DSL_DEBUG( DSL_DBG_ERR,
               (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Unknown LxState=%d received!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext), sAck.LxState));
            nErrCode = DSL_ERROR;
            break;
         }

         if (nErrCode == DSL_SUCCESS)
         {
            /* Update Power Management Status*/
            nErrCode = DSL_DRV_VRX_PowerManagementStatusSet(pContext, nPMMode);
            if(nErrCode != DSL_SUCCESS)
            {
               DSL_DEBUG( DSL_DBG_ERR, (pContext,
                  SYS_DBG_ERR"DSL[%02d]: ERROR - Power Management State set failed!"
                  DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
            }
         }
      }
   }
   else
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - Line state get failed, no FW loaded!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      nErrCode = DSL_ERROR;
   }

   DSL_DEBUG( DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_LineStateGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return (nErrCode);
}

DSL_Error_t DSL_DRV_DEV_LinkActivate(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_LineStateValue_t nLineState;
   DSL_TestModeControlSet_t nTestModeControl = DSL_TESTMODE_DISABLE;
   DSL_uint16_t linkStartMode = CMD_ModemFSM_StateSet_LINKINI;
#ifdef INCLUDE_DSL_FILTER_DETECTION
   DSL_DEV_VersionCheck_t nVerCheck = 0;
   DSL_boolean_t bFilterDetectionActive = DSL_FALSE;
#endif /* INCLUDE_DSL_FILTER_DETECTION */

   /* Get device line state*/
   nErrCode = DSL_DRV_DEV_LineStateGet(pContext, &nLineState);
   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Line state get failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   if (nLineState != DSL_LINESTATE_IDLE)
   {
      DSL_DEBUG( DSL_DBG_ERR,
        (pContext, SYS_DBG_ERR
        "DSL[%02d]: ERROR - Expecting IDLE line state while Link Activation, nLineState=0x%08X!"
        DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nLineState));

      return DSL_ERROR;
   }

#ifdef INCLUDE_DSL_FILTER_DETECTION

   /* Get FW information*/
   nErrCode = DSL_DRV_VRX_FirmwareVersionCheck(pContext,
               DSL_MIN_FW_VERSION_MFD, &nVerCheck);
   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW version check failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return nErrCode;
   }

   /* Get Microfilter detection state */
   DSL_CTX_READ(pContext, nErrCode,
      showtimeMeasurement.bFilterDetectionActive, bFilterDetectionActive);

   if (bFilterDetectionActive)
   {
      do {
         /* MFD only for ADSL mode */
         if (!DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSL))
         {
            DSL_DEBUG(DSL_DBG_WRN, (pContext, SYS_DBG_WRN"DSL[%02d]: WARNING"
               " - hybrid/filter/looplength statistics are not supported in "
               "current VDSL mode"DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            break;
         }

         /* MFD only for firmware version since DSL_MIN_FW_VERSION_MFD */
         if (nVerCheck < DSL_VERSION_EQUAL)
         {
            DSL_DEBUG(DSL_DBG_WRN, (pContext, SYS_DBG_WRN"DSL[%02d]: WARNING"
               " - hybrid/filter/looplength statistics are not supported "
               "by firmware"DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            break;
         }

         /* acitvate for force/auto filter detection */
         linkStartMode = CMD_ModemFSM_StateSet_MFD;

      } while(0);
   }

#endif /* INCLUDE_DSL_FILTER_DETECTION */

   /* Get Test Mode Control settings*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nTestModeControl, nTestModeControl);

   /* Activate LINK in TEST mode only if required.
      LINKINI is the default setting */
   if((nTestModeControl == DSL_TESTMODE_QUIET) ||
      (nTestModeControl == DSL_TESTMODE_SLEEP))
   {
      linkStartMode = CMD_ModemFSM_StateSet_TESTSTA;
   }

   /* Send LINKINI message*/
   nErrCode = DSL_DRV_VRX_SendMsgModemStateSet(pContext, linkStartMode);
   if ( nErrCode < DSL_SUCCESS )
   {
      /* Set LINIT value to 'LINIT_UNKNOWN' in case of an error */
      DSL_DRV_HandleLinitValue(pContext, LINIT_UNKNOWN, LINIT_SUB_NONE);

      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - modem LINKINI failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   /* SILENT line state reached*/
   DSL_DRV_LineStateSet(pContext, DSL_LINESTATE_SILENT);

   return nErrCode;
}

#ifdef INCLUDE_DSL_CPE_MISC_LINE_STATUS
/*
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_MISC_LINE_STATUS_GET
*/
DSL_Error_t DSL_DRV_DEV_MiscLineStatusGet(
   DSL_Context_t *pContext,
   DSL_MiscLineStatus_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_MiscLineStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   nErrCode = DSL_ERR_NOT_SUPPORTED_BY_FIRMWARE;

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_MiscLineStatusGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return (nErrCode);
}
#endif /* INCLUDE_DSL_CPE_MISC_LINE_STATUS*/

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device.h'
*/
DSL_Error_t DSL_DRV_DEV_AutobootHandleStart(
   DSL_Context_t *pContext,
   DSL_boolean_t bLoopTest,
   DSL_boolean_t bShortInit)
{
   DSL_Error_t nErrCode = DSL_SUCCESS, nRet = DSL_SUCCESS;
   DSL_Autoboot_State_t nState = DSL_AUTOBOOTSTATE_UNKNOWN;

   /* print unused parameters to suppress warnings*/
   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_AutobootHandleStart(%d, %d)"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), bLoopTest, bShortInit));

   /* Reset bCamFwReload flag*/
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.bCamFwReload, DSL_FALSE);
   /* Reset bCamDualPortModeSteady flag*/
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.bCamDualPortModeSteady, DSL_FALSE);
   /* Reset bCamSinglePortModeSteady flag*/
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.bCamSinglePortModeSteady, DSL_FALSE);

   /* Write Modem configuration data*/
   nErrCode = DSL_DRV_DEV_ModemWriteConfig(pContext);
   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - modem conf. write failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   /* Get actual autoboot state*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nAutobootState, nState);
   if( nState != DSL_AUTOBOOTSTATE_INIT )
   {
      /* No further actions needed in case of the Autoboot state change*/
      return DSL_SUCCESS;
   }
   /* Diagnostic mode switch, auto-restart enable/disable */
   nRet = DSL_DRV_VRX_SendMsgModemFsmOptionsSet(pContext, DSL_FALSE, bLoopTest);

   if( (nRet != DSL_SUCCESS) && (nRet != DSL_ERR_NOT_SUPPORTED_BY_FIRMWARE) )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Modem FSM options set failed!"
         "[%d]"DSL_DRV_CRLF, DSL_DEV_NUM(pContext),nRet));

      return nRet;
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_AutobootHandleStart"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device.h'
*/
DSL_Error_t DSL_DRV_DEV_AutobootHandleTraining(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_LineStateValue_t nLineState = DSL_LINESTATE_UNKNOWN,
                        nPrevLineState = DSL_LINESTATE_UNKNOWN;
   DSL_TestModeControlSet_t nTestMode = DSL_TESTMODE_DISABLE;
#ifdef INCLUDE_DSL_PM
#ifdef INCLUDE_DSL_CPE_PM_LINE_COUNTERS
   DSL_uint32_t nFullInits = 0, nFailedFullInits = 0;
#endif /* INCLUDE_DSL_CPE_PM_LINE_COUNTERS*/
#endif /* INCLUDE_DSL_PM*/
   DSL_boolean_t bPreFail = DSL_FALSE;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_AutobootHandleTraining"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Get Line State from the DSL CPE internal memory*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineState, nPrevLineState);

   /* Update Line State in the DSL CPE internal memory*/
   nErrCode = DSL_DRV_LineStateUpdate(pContext);
   if( nErrCode != DSL_SUCCESS )
   {
      return nErrCode;
   }

   /* Get Line State from the DSL CPE internal memory*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineState, nLineState);

   if (nLineState != nPrevLineState)
   {
      /* Get Test mode from the DSL CPE internal memory*/
      DSL_CTX_READ_SCALAR(pContext, nErrCode, nTestModeControl, nTestMode);

      switch (nLineState)
      {
      case DSL_LINESTATE_SILENT:
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: SILENT state reached"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         break;
      case DSL_LINESTATE_SHOWTIME_TC_SYNC:
      case DSL_LINESTATE_SHOWTIME_NO_SYNC:
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: SHOWTIME state reached"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         /* Clear bGotFullInit flag*/
         pContext->bGotFullInit = DSL_FALSE;
         pContext->bGotShowtime = DSL_TRUE;

#ifdef INCLUDE_DSL_SYSTEM_INTERFACE
         nErrCode = DSL_DRV_VRX_SystemInterfaceStatusUpdate(pContext);
         if(nErrCode != DSL_SUCCESS)
         {
            DSL_DEBUG( DSL_DBG_ERR, (pContext,
               SYS_DBG_ERR"DSL[%02d]: ERROR - System Interface status update failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         }
#endif /* INCLUDE_DSL_SYSTEM_INTERFACE*/

         /* Get XTSE status info and update it in the DSL CPE internal memory*/
         nErrCode = DSL_DRV_DEV_ShowtimeStatusUpdate(pContext, DSL_TRUE);
         if( nErrCode != DSL_SUCCESS )
         {
            DSL_DEBUG(DSL_DBG_MSG,
               (pContext, SYS_DBG_MSG"DSL[%02d]: ERROR - Showtime status update failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         }

         /* Move to the next Autoboot state*/
         nErrCode = DSL_DRV_AutobootStateSet(
                       pContext,
                       DSL_AUTOBOOTSTATE_SHOWTIME,
                       DSL_AUTOBOOT_SHOWTIME_POLL_TIME);

         /* Set LINIT value to 'LINIT_SUCCESSFUL' here because activation procedure
            was finished without errors */
         DSL_DRV_HandleLinitValue(pContext, LINIT_SUCCESSFUL, LINIT_SUB_NONE);

#if defined(INCLUDE_DSL_CPE_API_VRX)
         nErrCode = DSL_DRV_VRX_ShowtimeMeasurementUpdate(pContext, DSL_TRUE);
         if (nErrCode != DSL_SUCCESS)
         {
            DSL_DEBUG( DSL_DBG_WRN, (pContext, "DSL[%02d]: WARNING - Could not"
                  " update hybrid/filter/looplength!"DSL_DRV_CRLF,
                  DSL_DEV_NUM(pContext)));
         }
#endif /* INCLUDE_DSL_CPE_API_VRX */

#ifdef INCLUDE_DSL_G997_LINE_INVENTORY
         /* Add FE Inventory Status timeout event. FE inventory information is available
             from the FW after 5 sec of the showtime state*/
         DSL_DRV_Timeout_AddEvent( pContext,
                              (DSL_int_t)DSL_TIMEOUTEVENT_FE_STATUS,
                              (DSL_uint32_t)DSL_DEV_TIMEOUT_FE_STATUS );
#endif /* INCLUDE_DSL_G997_LINE_INVENTORY*/
         break;
#if defined(INCLUDE_DSL_CPE_API_VRX)
      case DSL_LINESTATE_ORDERLY_SHUTDOWN:
         bPreFail = DSL_TRUE;
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: ORDERLY_SHUTDOWN state reached"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         /* do not use break here, continue handling */
#endif /* INCLUDE_DSL_CPE_API_VRX */
      case DSL_LINESTATE_EXCEPTION:
         if (!bPreFail)
         {
            DSL_DEBUG(DSL_DBG_MSG,
               (pContext, SYS_DBG_MSG"DSL[%02d]: EXCEPTION state reached"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         }
#ifdef INCLUDE_DSL_PM
#ifdef INCLUDE_DSL_CPE_PM_LINE_COUNTERS
         /* Check for the Failed Full init condition*/
         if (pContext->bGotFullInit)
         {
            /* Increment the number of Failed Full Inits*/
            DSL_CTX_READ(pContext, nErrCode, nFailedFullInits, nFailedFullInits);
            nFailedFullInits++;
            DSL_CTX_WRITE(pContext, nErrCode, nFailedFullInits, nFailedFullInits);
         }
#endif /* INCLUDE_DSL_CPE_PM_LINE_COUNTERS*/
#endif /* INCLUDE_DSL_PM*/
         if (nTestMode == DSL_TESTMODE_TRAINING_LOCK)
         {
            DSL_DEBUG(DSL_DBG_MSG,
               (pContext, SYS_DBG_MSG"DSL[%02d]: EXCEPTION - training lock is set, "
               "exception state will not be set"DSL_DRV_CRLF,
               DSL_DEV_NUM(pContext)));
         }
         else
         {
#if defined(INCLUDE_DSL_CPE_API_VRX)
            if (bPreFail)
            {
               nErrCode = DSL_DRV_AutobootStateSet(pContext,
                                       DSL_AUTOBOOTSTATE_ORDERLY_SHUTDOWN_WAIT,
                                       DSL_AUTOBOOT_RESTART_POLL_TIME);
               /* Activate Autoboot timeout */
               DSL_DRV_AutobootTimeoutSet(pContext, DSL_AUTOBOOT_ORDERLY_SHUTDOWN_TIMEOUT);
            }
            else
#endif /* INCLUDE_DSL_CPE_API_VRX */
            {
               DSL_DEBUG(DSL_DBG_MSG,
                  (pContext, SYS_DBG_MSG"DSL[%02d]: EXCEPTION - set exception state"
                  DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

               nErrCode = DSL_DRV_AutobootStateSet(pContext,
                                       DSL_AUTOBOOTSTATE_EXCEPTION,
                                       DSL_AUTOBOOT_EXCEPTION_POLL_TIME);
            }
         }
         break;

      case DSL_LINESTATE_T1413:
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: T1413 state reached"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         break;

      case DSL_LINESTATE_HANDSHAKE:
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: HANDSHAKE state reached"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         DSL_DRV_AutobootTimeoutSet(pContext, DSL_AUTOBOOT_TRAINING_TIMEOUT_VRX);

         break;
      case DSL_LINESTATE_FULL_INIT:
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: FULL_INIT state reached"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         if (pContext->bGotFullInit == DSL_FALSE)
         {
            pContext->bGotFullInit = DSL_TRUE;
#ifdef INCLUDE_DSL_PM
#ifdef INCLUDE_DSL_CPE_PM_LINE_COUNTERS
            /* Increment the number of Full Inits*/
            DSL_CTX_READ(pContext, nErrCode, nFullInits, nFullInits);
            nFullInits++;
            DSL_CTX_WRITE(pContext, nErrCode, nFullInits, nFullInits);
#endif /* INCLUDE_DSL_CPE_PM_LINE_COUNTERS*/
#endif /* INCLUDE_DSL_PM*/
         }

         /* Update XTSE status information*/
         nErrCode = DSL_DRV_VRX_SendMsgXtseStatusGet(pContext);
         if(nErrCode != DSL_SUCCESS)
         {
            DSL_DEBUG( DSL_DBG_ERR, (pContext,
               SYS_DBG_ERR"DSL[%02d]: ERROR - XTSE status get failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
            break;
         }
#ifdef INCLUDE_DSL_SYSTEM_INTERFACE
#if defined(INCLUDE_DSL_CPE_API_VRX)
         nErrCode = DSL_DRV_VRX_SystemInterfaceStatusUpdate(pContext);
#endif
         if(nErrCode != DSL_SUCCESS)
         {
            DSL_DEBUG( DSL_DBG_ERR, (pContext,
               SYS_DBG_ERR"DSL[%02d]: ERROR - System Interface status update failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         }
#endif /* INCLUDE_DSL_SYSTEM_INTERFACE*/

#if defined(INCLUDE_DSL_BONDING)
         nErrCode = DSL_DRV_AutobootStateSet(
                       pContext,
                       DSL_AUTOBOOTSTATE_TRAIN,
                       DSL_AUTOBOOT_TRAINING_POLL_TIME);
#endif /* defined(INCLUDE_DSL_BONDING)*/
         break;

      case DSL_LINESTATE_LOOPDIAGNOSTIC_ACTIVE:
         nErrCode = DSL_DRV_AutobootStateSet(
                       pContext,
                       DSL_AUTOBOOTSTATE_DIAGNOSTIC,
                       DSL_AUTOBOOT_DIAGNOSTIC_POLL_TIME);

         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: LOOPDIAGNOSTIC_ACTIVE state reached"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         /* In case of DIAGNOSTIC mode use this state to identify FULL_INIT*/
         if (pContext->bGotFullInit == DSL_FALSE)
         {
            pContext->bGotFullInit = DSL_TRUE;
#ifdef INCLUDE_DSL_PM
#ifdef INCLUDE_DSL_CPE_PM_LINE_COUNTERS
            /* Increment the number of Full Inits*/
            DSL_CTX_READ(pContext, nErrCode, nFullInits, nFullInits);
            nFullInits++;
            DSL_CTX_WRITE(pContext, nErrCode, nFullInits, nFullInits);
#endif /* INCLUDE_DSL_CPE_PM_LINE_COUNTERS*/
#endif /* INCLUDE_DSL_PM*/
         }
         DSL_DRV_AutobootTimeoutSet(pContext, DSL_AUTOBOOT_DELT_TIMEOUT_VRX);
         break;
      case DSL_LINESTATE_LOOPDIAGNOSTIC_COMPLETE:
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: LOOPDIAGNOSTIC_COMPLETE state reached"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         /* Handle DSL_LINESTATE_LOOPDIAGNOSTIC_COMPLETE state */
         nErrCode = DSL_DRV_VRX_LoopDiagnosticCompleteHandle(pContext);
         break;

#if defined(INCLUDE_DSL_FILTER_DETECTION) && defined(INCLUDE_DSL_CPE_API_VRX)
      case DSL_LINESTATE_TEST_FILTERDETECTION_ACTIVE:
         DSL_DEBUG(DSL_DBG_MSG, (pContext, SYS_DBG_MSG"DSL[%02d]: "
            "TEST_FILTERDETECTION_ACTIVE state reached" DSL_DRV_CRLF,
            DSL_DEV_NUM(pContext)));
         break;

      case DSL_LINESTATE_TEST_FILTERDETECTION_COMPLETE:
         {
            DSL_boolean_t bFilterDetectionActive = DSL_FALSE;

            DSL_DEBUG(DSL_DBG_MSG, (pContext, SYS_DBG_MSG"DSL[%02d]: "
               "TEST_FILTERDETECTION_COMPLETE state reached" DSL_DRV_CRLF,
               DSL_DEV_NUM(pContext)));

            DSL_CTX_READ(pContext, nErrCode,
                         showtimeMeasurement.bFilterDetectionActive,
                         bFilterDetectionActive);

            if (bFilterDetectionActive)
            {
               nErrCode = DSL_DRV_VRX_ShowtimeMeasurementCompleted(pContext);
               if (nErrCode != DSL_SUCCESS)
               {
                  DSL_DEBUG(DSL_DBG_WRN, (pContext, "DSL[%02d]: WARNING - "
                     "Could not complete filter detection mode!" DSL_DRV_CRLF,
                     DSL_DEV_NUM(pContext)));
               }

               /* Restart after MFD complete*/
               nErrCode = DSL_DRV_AutobootStateSet(
                             pContext,
                             DSL_AUTOBOOTSTATE_RESTART,
                             DSL_AUTOBOOT_TRAINING_POLL_TIME);
            }
         break;
         }
#endif /* INCLUDE_DSL_FILTER_DETECTION && INCLUDE_DSL_CPE_API_VRX */

      case DSL_LINESTATE_IDLE:
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: IDLE state reached" DSL_DRV_CRLF,
            DSL_DEV_NUM(pContext)));
         break;
      case DSL_LINESTATE_TEST:
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: TEST state reached"DSL_DRV_CRLF,
            DSL_DEV_NUM(pContext)));

         if( nTestMode == DSL_TESTMODE_QUIET)
         {
            /* Switch Off Tx Signals*/
            nErrCode = DSL_DRV_VRX_SendMsgTxControlSet(pContext, CMD_TEST_TxControlSet_SEND_QUIET);
            if( nErrCode!=DSL_SUCCESS )
            {
               DSL_DEBUG( DSL_DBG_ERR,
                  (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Tx Control config write failed!"
                  DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
               return nErrCode;
            }
         }
         break;
#ifdef INCLUDE_DSL_BONDING
      case DSL_LINESTATE_BONDING_CLR:
         /*
         This state is handled outside CPE API driver.
         */
         break;
#endif /* INCLUDE_DSL_BONDING*/
      default:
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: Unexpected line state (%X) while training"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nLineState));
         nErrCode = DSL_ERROR;
         break;
      }
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_AutobootHandleTraining, errCode(%d)"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device.h'
*/
DSL_Error_t DSL_DRV_DEV_AutobootHandleException(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_VRX_FwVersion_t  sFwVersion = {0};

   DSL_CHECK_CTX_POINTER(pContext);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_AutobootHandleException"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* VDSL FW does not support Power Management State via message and ADSL FW
      currently indicates L0 state in exception. Therefore set it manually here
      for both xDSL flavors (should be removed if supported by FW where
      it is updated within context of periodical line state update and set to
      L3 in case of reaching exception state). */
   /* Update Power Management Status*/
   nErrCode = DSL_DRV_VRX_PowerManagementStatusSet(pContext, DSL_G997_PMS_L3);
   if(nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - Power Management State set failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

     return nErrCode;
   }

#if defined(INCLUDE_DSL_CPE_API_VRX)
   /* Update filter detection and hybrid selection data if needed*/
   nErrCode = DSL_DRV_VRX_ShowtimeMeasurementUpdate(pContext, DSL_FALSE);
   if(nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG( DSL_DBG_MSG, (pContext,
         "DSL[%02d]: Filter detection and hybrid selection data not available!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }
#endif /* INCLUDE_DSL_CPE_API_VRX */

   /* Get Modem Fail Reason*/
   nErrCode = DSL_DRV_VRX_FailReasonGet( pContext );
   if( nErrCode!= DSL_SUCCESS )
   {
      DSL_DEBUG(DSL_DBG_WRN,
         (pContext, SYS_DBG_WRN"DSL[%02d]: Can't get modem Fail reason"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }

   /* Request the handshake information about the far-end operating mode*/
   nErrCode = DSL_DRV_VRX_SendMsgSpar1Get(pContext);
   if( nErrCode!= DSL_SUCCESS )
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - SPAR1 get failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   /* Handle exception for CAM*/
   nErrCode = DSL_DRV_VRX_CamExceptionHandle(pContext);
   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - CAM exception handle failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   /* Get FW info*/
   DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.fwFeatures, sFwVersion);

   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
   {
      nErrCode = DSL_DRV_VRX_DataPathFailuresStatusGet(pContext);
      if(nErrCode != DSL_SUCCESS)
      {
         DSL_DEBUG( DSL_DBG_ERR, (pContext,
            SYS_DBG_ERR"DSL[%02d]: ERROR - Data Path Failures update failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

        return nErrCode;
      }
   }

   /* Update Line failures information*/
   nErrCode = DSL_DRV_VRX_LineFailuresStatusUpdate(pContext);

   DSL_DEBUG(DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_AutobootHandleException, retCode(%d)"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

DSL_Error_t DSL_DRV_DEV_AutobootHandleFwRequest(DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_AutobootHandleFwRequest"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Update CAM FSM*/
   nErrCode = DSL_DRV_VRX_CamFsmUpdate(pContext);
   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - CAM FSM Update failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_AutobootHandleFwRequest"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

DSL_Error_t DSL_DRV_DEV_XtseSettingsCheck(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN DSL_uint8_t *pXTSE)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_XtseSettingsCheck"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Check if G992.3 Annex A mode specified in case of Annex L*/
   if (((pXTSE[5-1] & XTSE_5_04_L_3_NO) || (pXTSE[5-1] & XTSE_5_03_L_3_NO)) &&
       (!(pXTSE[3-1] & XTSE_3_03_A_3_NO)))
   {
      DSL_DEBUG(DSL_DBG_WRN,
         (pContext, SYS_DBG_WRN"DSL[%02d]: WARNING - Annex L mode is only supported in conjunction with G992.3 Annex A"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      /* Set G992.3 Annex A mode*/
      pXTSE[3-1] |= XTSE_3_03_A_3_NO;

      nErrCode = DSL_WRN_INCONSISTENT_XTSE_CONFIGURATION;
   }

   /* Check if both narrow and wide Annex L modes are set*/
   if ((pXTSE[5-1] & (XTSE_5_04_L_3_NO | XTSE_5_03_L_3_NO)) &&
       ((pXTSE[5-1] & (XTSE_5_04_L_3_NO | XTSE_5_03_L_3_NO)) != (XTSE_5_04_L_3_NO | XTSE_5_03_L_3_NO)))
   {
      DSL_DEBUG(DSL_DBG_WRN,
         (pContext, SYS_DBG_WRN"DSL[%02d]: WARNING - Setting only both (narrow, wide) Annex L allowed"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      /* Set both Annex L modes*/
      pXTSE[5-1] |= (XTSE_5_04_L_3_NO | XTSE_5_03_L_3_NO);

      nErrCode = DSL_WRN_INCONSISTENT_XTSE_CONFIGURATION;
   }

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_XtseSettingsCheck"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device.h'
*/
DSL_Error_t DSL_DRV_DEV_AutobootHandleL3(
   DSL_Context_t *pContext,
   DSL_boolean_t bL3Forced)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_AutobootHandleL3"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   if( bL3Forced == DSL_FALSE )
   {
      nErrCode = DSL_DRV_AutobootStateSet(pContext, DSL_AUTOBOOTSTATE_INIT,
         DSL_AUTOBOOT_INIT_POLL_TIME);
   }

   DSL_DEBUG(DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DEV_AutobootHandleL3, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device.h'
*/
DSL_Error_t DSL_DRV_DEV_InitDataPrepare(
   DSL_Context_t *pContext,
   DSL_boolean_t bIsInKernel,
   DSL_Init_t *pInit)
{
   /* Nothing to be done for VRX */
   return DSL_SUCCESS;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device.h'
*/
DSL_void_t DSL_DRV_DEV_InitDataFree(DSL_Init_t *pInit)
{
   /* Nothing to be done for VRX */
   return;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device.h'
*/
DSL_boolean_t DSL_DRV_DEV_ModemIsReady(DSL_Context_t *pContext)
{
   DSL_LineStateValue_t nLineState;

   if( DSL_DRV_DEV_LineStateGet(pContext, &nLineState) != DSL_ERROR )
   {
      return nLineState == DSL_LINESTATE_IDLE ? DSL_TRUE : DSL_FALSE;
   }

   return DSL_FALSE;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device.h'
*/
DSL_Error_t DSL_DRV_DEV_ChipSetTypeGet(
   DSL_Context_t *pContext,
   DSL_char_t    *pString)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CHECK_POINTER(pContext, pString);
   DSL_CHECK_ERR_CODE();

   strncpy(pString, DSL_CHIPSET_TYPE_STRING, MAX_INFO_STRING_LEN);

   return nErrCode;
}

DSL_Error_t DSL_DRV_DEV_TestModeControlSet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_TestModeControl_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_TestModeControlSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   DSL_CTX_WRITE(pContext, nErrCode, nTestModeControl, pData->data.nTestMode);

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_TestModeControlSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return (nErrCode);
}

#ifdef INCLUDE_DSL_G997_LINE_INVENTORY
DSL_Error_t DSL_DRV_DEV_AuxLineInventoryGet(
   DSL_Context_t *pContext,
   DSL_AuxLineInventory_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();
   DSL_CHECK_ATU_DIRECTION(pData->nDirection);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_AuxLineInventoryGet(nDirection=%s)"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), (pData->nDirection==DSL_NEAR_END?"NE":"FE")));

   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
   {
      if (pData->nDirection == DSL_NEAR_END)
      {
         DSL_CTX_READ(pContext, nErrCode, auxInventoryNe, pData->data);
      }
      else
      {
         DSL_CTX_READ(pContext, nErrCode, auxInventoryFe, pData->data);
      }
   }
   else
   {
      nErrCode = DSL_ERR_NOT_SUPPORTED_BY_FIRMWARE;
   }

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_AuxLineInventoryGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return (nErrCode);
}
#endif /* INCLUDE_DSL_G997_LINE_INVENTORY*/

DSL_Error_t  DSL_DRV_VRX_TestParamsFeCheck(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   /* Data updated at showtime entry, see DSL_DRV_DEV_ShowtimeStatusUpdate() */
   if (pContext->eTestParametersFeReady == VRX_FE_TESTPARAMS_FIRST_UPDATING)
   {
      DSL_DEBUG(DSL_DBG_ERR, (pContext, SYS_DBG_ERR
         "DSL[%02d]: SNR Fe parameters updating in progress!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      nErrCode = DSL_ERR_DATA_UPDATE_IN_PROGRESS;
   }
   else if (pContext->eTestParametersFeReady == VRX_FE_TESTPARAMS_UPDATING)
   {
      DSL_DEBUG(DSL_DBG_WRN, (pContext, SYS_DBG_WRN
         "DSL[%02d]: SNR Fe parameters updating in progress!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      nErrCode = DSL_WRN_INCOMPLETE_RETURN_VALUES;
   }

   return (nErrCode);
}

#ifdef INCLUDE_DSL_DELT
/*
   For a detailed description please refer to the drv_dsl_cpe_intern.h
*/
DSL_Error_t DSL_DRV_DEV_DeltSNRGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN const DSL_AccessDir_t nDirection,
   DSL_IN DSL_DeltDataType_t nDeltDataType,
   DSL_OUT DSL_G997_DeltSnrData_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_LineStateValue_t nCurrentState = DSL_LINESTATE_UNKNOWN;
   ACK_TestParamsAuxDS_Get_t  sAck;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_DeltSNRGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   if (nDirection == DSL_UPSTREAM)
   {
      DSL_DEBUG(DSL_DBG_ERR, (pContext, SYS_DBG_ERR
         "DSL[%02d]: SNR with Virtual Noise are not supported for Upstream!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERR_NOT_SUPPORTED;
   }

   if (nDeltDataType == DSL_DELT_DATA_SHOWTIME)
   {
      /* Get current line state*/
      DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineState, nCurrentState);

      /* Only proceed if the specified line is in SHOWTIME state.*/
      if ( (nCurrentState == DSL_LINESTATE_SHOWTIME_TC_SYNC  ||
            nCurrentState == DSL_LINESTATE_SHOWTIME_NO_SYNC))
      {
         memset(&sAck, 0, sizeof(sAck));
         /* Get Aux Parameters directly from the FW*/
         nErrCode = DSL_DRV_VRX_SendMsgTestParamsAuxGet(
                       pContext, nDirection, (DSL_uint8_t*)&sAck);

         if (nErrCode != DSL_SUCCESS)
         {
            DSL_DEBUG(DSL_DBG_ERR,(pContext, SYS_DBG_ERR
               "DSL[%02d]: ERROR - Aux Test Parameters get failed (nDirection=%d)"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nDirection));
         }

         /* Get SNR values per subcarrier (with Virtual Noise) */
         nErrCode = DSL_DRV_VRX_SnrAllocationTableNeGet(pContext, &(pData->deltSnr));
         if( nErrCode != DSL_SUCCESS )
         {
            DSL_DEBUG(DSL_DBG_ERR,
               (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - SNR Table get failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         }
         else
         {
            /* Calculate SNR values per subcarrier group*/
            nErrCode = DSL_DRV_VRX_SnrPerGroupCalculate(
                          pContext, (DSL_uint8_t)sAck.SNRG, &(pData->deltSnr));

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
         if(DSL_DRV_MUTEX_LOCK(pContext->dataMutex))
         {
            DSL_DEBUG( DSL_DBG_ERR,
               (pContext, SYS_DBG_ERR"DSL[%02d]: Couldn't lock data mutex"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            return DSL_ERR_SEMAPHORE_GET;
         }

         memcpy(&pData->deltSnr,
                (DSL_void_t*)&(pContext->DELT->snrDataDs.deltSnrVn),
                sizeof(DSL_G997_NSCData8_t));

         pData->nGroupSize       = pContext->DELT->snrDataDs.nGroupSize;
         pData->nMeasurementTime = pContext->DELT->snrDataDs.nMeasurementTime;

         DSL_DRV_MUTEX_UNLOCK(pContext->dataMutex);
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
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_DEV_DeltSNRGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}
#endif /* INCLUDE_DSL_DELT*/

#endif /* #ifdef INCLUDE_DSL_CPE_API_VRX*/

