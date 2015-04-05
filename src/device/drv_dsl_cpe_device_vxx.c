/******************************************************************************

                               Copyright (c) 2011
                            Lantiq Deutschland GmbH
                     Am Campeon 3; 85579 Neubiberg, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/
#define DSL_INTERN

#include "drv_dsl_cpe_api.h"

#if defined(INCLUDE_DSL_CPE_API_VINAX) || defined(INCLUDE_DSL_CPE_API_VRX)

#include "drv_dsl_cpe_device_g997.h"

#undef DSL_DBG_BLOCK
#define DSL_DBG_BLOCK DSL_DBG_DEVICE

/*
   Vinax and VRx common implementation
*/

#ifdef INCLUDE_DSL_DELT
/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device_vxx.h'
*/
DSL_Error_t DSL_DRV_VXX_SnrPerGroupCalculate(
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
   please refer to the description in the header file 'drv_dsl_cpe_device_vxx.h'
*/
DSL_Error_t DSL_DRV_VXX_BandPlanStatusGet(
   DSL_Context_t *pContext,
   DSL_BandPlanConfigData_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VXX_BandPlanStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Only Available for VDSL mode*/
   if( DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2) )
   {
      if(DSL_DRV_MUTEX_LOCK(pContext->dataMutex))
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - data mutex lock failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return DSL_ERROR;
      }

      /* Get Band Plan Status from the internal VINAX device context*/
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
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VXX_BandPlanStatusGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device_vxx.h'
*/
DSL_Error_t DSL_DRV_VXX_CheckTruncParamRange(
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

DSL_Error_t DSL_DRV_VXX_ChannelStatusGet(
   DSL_Context_t *pContext,
   DSL_uint8_t nChannel,
   DSL_AccessDir_t nDirection,
   DSL_G997_ChannelStatusData_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
#if defined(INCLUDE_DSL_CPE_API_VRX)
   DSL_boolean_t bReTxEnable = DSL_FALSE;
   ACK_BearerChsDS_RTX_Get_t sAck;
#endif /* defined(INCLUDE_DSL_CPE_API_VRX) */

   DSL_CHECK_ATU_DIRECTION(nDirection);
   DSL_CHECK_ERR_CODE();

   if (nDirection == DSL_UPSTREAM)
   {
      DSL_CTX_READ_SCALAR(pContext, nErrCode,
         ActualInterleaveDelayUs[nChannel], pData->ActualInterleaveDelay);

      DSL_CTX_READ_SCALAR(pContext, nErrCode,
         ActualImpulseNoiseProtectionUs[nChannel], pData->ActualImpulseNoiseProtection);
   }
   else
   {
      DSL_CTX_READ_SCALAR(pContext, nErrCode,
         ActualInterleaveDelayDs[nChannel], pData->ActualInterleaveDelay);

      DSL_CTX_READ_SCALAR(pContext, nErrCode,
         ActualImpulseNoiseProtectionDs[nChannel], pData->ActualImpulseNoiseProtection);
   }

   DSL_CTX_READ_SCALAR(pContext, nErrCode,
      nChannelActualDataRate[nDirection][nChannel], pData->ActualDataRate);

   DSL_CTX_READ_SCALAR(pContext, nErrCode,
      nChannelPreviousDataRate[nDirection][nChannel], pData->PreviousDataRate);

#if defined(INCLUDE_DSL_CPE_API_VRX)
   pData->ActualNetDataRate = pData->ActualDataRate;

   DSL_CTX_READ_SCALAR(
      pContext, nErrCode, lineFeatureDataSts[DSL_DOWNSTREAM].bReTxEnable,
      bReTxEnable);

   /* retransmission is not supported for up stream */
   if ((bReTxEnable) && (nDirection == DSL_DOWNSTREAM))
   {
      nErrCode = DSL_DRV_VRX_SendMsgBearerChsDsRtxGet(pContext, &sAck);

      if (nErrCode < DSL_SUCCESS)
      {
         return nErrCode;
      }

      /* Update parameters for Retransmission enable */
      pData->ActualDataRate = (DSL_uint32_t)sAck.ETR_LSW |
                                          (((DSL_uint32_t)sAck.ETR_MSW) << 16);
      pData->ActualInterleaveDelay = sAck.ActDelay;
   }
#endif /* defined(INCLUDE_DSL_CPE_API_VRX) */

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device_vxx.h'
*/
DSL_Error_t DSL_DRV_VXX_PowerManagementStatusSet(
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

static DSL_Error_t DSL_DRV_VXX_OnEventChannelDataRateUpdate(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDir)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_G997_ChannelStatusData_t chStsData;

   nErrCode = DSL_DRV_VXX_ChannelStatusGet(pContext, 0, nDir, &chStsData);
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
static DSL_Error_t DSL_DRV_VXX_OnEventChannelDataRateThresholdCrossingUpdate(
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

DSL_Error_t DSL_DRV_VXX_DataRateUpdate(
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
      nErrCode = DSL_DRV_VXX_OnEventChannelDataRateUpdate(pContext, nDir);
   }

#ifdef INCLUDE_DSL_G997_ALARM
   if( oldDataRate != 0 && nErrCode == DSL_SUCCESS )
   {
      nErrCode = DSL_DRV_VXX_OnEventChannelDataRateThresholdCrossingUpdate(
                    pContext, nDir,
                    &oldDataRate, &dataRate);
   }
#endif /* INCLUDE_DSL_G997_ALARM*/

   return nErrCode;
}

DSL_Error_t DSL_DRV_VXX_XtseConfigure(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint32_t i = 0;
   DSL_G997_XTUSystemEnablingData_t XTSE;

   for (i = 0; i < DSL_G997_NUM_XTSE_OCTETS; i++)
   {
      DSL_CTX_READ_SCALAR(pContext, nErrCode, xtseCfg[i], XTSE.XTSE[i]);
   }

   nErrCode = DSL_DRV_VXX_SendMsgXtseConfigure(pContext, &XTSE);

   return (nErrCode);
}

static DSL_Error_t DSL_DRV_VXX_LoopDiagnosticCompleteHandle(DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
#ifdef INCLUDE_DSL_DELT
   DSL_uint8_t nAutoCount = 0;
   DSL_G997_LDSF_t nLoopMode = DSL_G997_INHIBIT_LDSF;
#endif /* INCLUDE_DSL_DELT*/

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VXX_LoopDiagnosticCompleteHandle"
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
   nErrCode = DSL_DRV_VXX_SendMsgXtseStatusGet( pContext );
   if(nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - XTSE status get failed!" DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));

     return nErrCode;
   }

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

   /* Store all DELT diagnostic data*/
   nErrCode = DSL_DRV_VXX_DiagnosticDeltDataGet(pContext);
   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - DELT data save failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

       return nErrCode;
   }
#endif /* INCLUDE_DSL_DELT*/

   if (DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2))
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
         nErrCode = DSL_DRV_VXX_SendMsgModemFsmOptionsSet(pContext, DSL_FALSE, DSL_FALSE);
         if( nErrCode!=DSL_SUCCESS )
         {
            DSL_DEBUG( DSL_DBG_ERR,
               (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Modem FSM options set failed!"
               "<%d>"DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

            return nErrCode;
         }
      }
#endif /* INCLUDE_DSL_DELT*/

      nErrCode = DSL_DRV_VXX_SendMsgModemStateSet(
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
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VXX_LoopDiagnosticCompleteHandle"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

#ifdef INCLUDE_DSL_CPE_MISC_LINE_STATUS
static DSL_Error_t DSL_DRV_VXX_AdslBandBorderStatusGet(
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
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VXX_AdslBandBorderStatusGet"
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
      nErrCode = DSL_DRV_VXX_BitAllocationTableGet(pContext,
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
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VXX_AdslBandBorderStatusGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return (nErrCode);
}

static DSL_Error_t DSL_DRV_VXX_VdslBandBorderStatusGet(
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
   nErrCode = DSL_DRV_VXX_SendMsgBandControlGet(
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
   nErrCode = DSL_DRV_VXX_BitAllocationTableGet(pContext,
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
DSL_Error_t DSL_DRV_VXX_OnLineInventoryFe(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   /* Retreive all FE inventory information*/
   nErrCode = DSL_DRV_VXX_SendMsgInventoryFeGet(pContext);
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
   please refer to the description in the header file 'drv_dsl_cpe_device_vxx.h'
*/
DSL_Error_t DSL_DRV_VXX_BandPlanSupportedGet(
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

   if( DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2) )
   {
      for( nBandPlan = (DSL_BandPlanType_t)0; nBandPlan < DSL_BANDPLAN_LAST; nBandPlan++ )
      {
         sBpConfig.nBandPlan = (DSL_BandPlanType_t)nBandPlan;
         for (nProfile = (DSL_ProfileType_t)0; nProfile < DSL_PROFILE_LAST; nProfile++)
         {
            sBpConfig.nProfile = (DSL_ProfileType_t)nProfile;
            pData->bSupported[nBandPlan][nProfile] =
               DSL_DRV_VXX_BandPlanSupportedCheck(pContext, &sBpConfig);
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

DSL_Error_t DSL_DRV_VXX_FirmwareVersionCheck(
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
   DSL_VNX_FwVersion_t firmwareVersion = {0};

   DSL_CHECK_POINTER(pContext, pVerCheck);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VXX_FirmwareVersionCheck" DSL_DRV_CRLF,
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
      *pVerCheck = DSL_VERSION_MISMATCH;

      return nErrCode;
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
      SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VXX_FirmwareVersionCheck" DSL_DRV_CRLF,
      DSL_DEV_NUM(pContext)));

   return nErrCode;
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
   if (DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2))
   {
      /* Call VDSL specific implementation*/
      nErrCode = DSL_DRV_VXX_VdslBandBorderStatusGet(pContext, pData);
   }
   else if (DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_ADSL))
   {
      /* Call ADSL specific implementation*/
      nErrCode = DSL_DRV_VXX_AdslBandBorderStatusGet(pContext, pData);
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

   if (DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2))
   {
      /* VDSL2 mode */
      nErrCode = DSL_DRV_VXX_FramingParametersVdsl2StatusGet(
                    pContext, pData->nChannel,
                    pData->nDirection,&(pData->data));
   }
   else if (DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_ADSL))
   {
      /* ADSL mode */
      nErrCode = DSL_DRV_VXX_FramingParametersAdslStatusGet(
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

   /* Get FW DL information*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->bFirmwareReady, bFwReady);

   if( bFwReady )
   {
      nErrCode = DSL_DRV_VXX_SendMsgModemFsmStateGet(pContext,(DSL_uint8_t*)&sAck);
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
      case ACK_ModemFSM_StateGet_GHS_STATE:
         *pnLineState = DSL_LINESTATE_HANDSHAKE;
         break;
      case 0x000B:
         *pnLineState = DSL_LINESTATE_HANDSHAKE;
         DSL_DEBUG( DSL_DBG_ERR, (pContext,
            SYS_DBG_ERR"DSL[%02d]: T.1413 handled as handshake!" DSL_DRV_CRLF,
            DSL_DEV_NUM(pContext)));
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
            nErrCode = DSL_DRV_VXX_PowerManagementStatusSet(pContext, nPMMode);
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
        (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Expecting IDLE line state while Link Activation!"
        DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERROR;
   }

#ifdef INCLUDE_DSL_FILTER_DETECTION
   /* Get FW information*/
   nErrCode = DSL_DRV_VXX_FirmwareVersionCheck(pContext,
               DSL_MIN_FW_VERSION_MFD, &nVerCheck);
   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW version check failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return nErrCode;
   }

   /* Get Microfilter detection state*/
   DSL_CTX_READ(pContext, nErrCode,
      showtimeMeasurement.bFilterDetectionActive, bFilterDetectionActive);

   if (bFilterDetectionActive)
   {
      do {
         /* MFD only for ADSL mode */
         if (!DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_ADSL))
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

   /* Activate LINK in TEST mode only if required. LINKINI is the default setting*/
   if((nTestModeControl == DSL_TESTMODE_QUIET) ||
      (nTestModeControl == DSL_TESTMODE_SLEEP))
   {
      linkStartMode = CMD_ModemFSM_StateSet_TESTSTA;
   }

   /* Send LINKINI message*/
   nErrCode = DSL_DRV_VXX_SendMsgModemStateSet(pContext, linkStartMode);
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
   nRet = DSL_DRV_VXX_SendMsgModemFsmOptionsSet(pContext, DSL_FALSE, bLoopTest);

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
#endif /* INCLUDE_DSL_FILTER_DETECTION && INCLUDE_DSL_CPE_API_VRX */

#ifdef INCLUDE_DSL_G997_LINE_INVENTORY
         /* Add FE Inventory Status timeout event. FE inventory information is available
             from the FW after 5 sec of the showtime state*/
         DSL_DRV_Timeout_AddEvent( pContext,
                              (DSL_int_t)DSL_TIMEOUTEVENT_FE_STATUS,
                              (DSL_uint32_t)DSL_DEV_TIMEOUT_FE_STATUS );
#endif /* INCLUDE_DSL_G997_LINE_INVENTORY*/
         break;
      case DSL_LINESTATE_EXCEPTION:
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: EXCEPTION state reached"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
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
            DSL_DEBUG(DSL_DBG_MSG,
               (pContext, SYS_DBG_MSG"DSL[%02d]: EXCEPTION - set exception state"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            nErrCode = DSL_DRV_AutobootStateSet(
               pContext,
               DSL_AUTOBOOTSTATE_EXCEPTION,
               DSL_AUTOBOOT_EXCEPTION_POLL_TIME);
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

         DSL_DRV_AutobootTimeoutSet(pContext, 120);

         break;
      case DSL_LINESTATE_FULL_INIT:
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: FULL_INIT state reached"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         DSL_DRV_AutobootTimeoutSet(pContext, 120);

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
         nErrCode = DSL_DRV_VXX_SendMsgXtseStatusGet(pContext);
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
#if defined(INCLUDE_DSL_CPE_API_VINAX)
         nErrCode = DSL_DRV_VINAX_SystemInterfaceStatusUpdate(pContext);
#endif
         if(nErrCode != DSL_SUCCESS)
         {
            DSL_DEBUG( DSL_DBG_ERR, (pContext,
               SYS_DBG_ERR"DSL[%02d]: ERROR - System Interface status update failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         }
#endif /* INCLUDE_DSL_SYSTEM_INTERFACE*/
         break;
      case DSL_LINESTATE_LOOPDIAGNOSTIC_ACTIVE:
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
         DSL_DRV_AutobootTimeoutSet(pContext, 360);
         break;
      case DSL_LINESTATE_LOOPDIAGNOSTIC_COMPLETE:
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: LOOPDIAGNOSTIC_COMPLETE state reached"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         /* Handle DSL_LINESTATE_LOOPDIAGNOSTIC_COMPLETE state */
         nErrCode = DSL_DRV_VXX_LoopDiagnosticCompleteHandle(pContext);
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
            nErrCode = DSL_DRV_VXX_SendMsgTxControlSet(pContext, CMD_TEST_TxControlSet_SEND_QUIET);
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
   DSL_VNX_FwVersion_t  sFwVersion = {0};

   DSL_CHECK_CTX_POINTER(pContext);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_DEV_AutobootHandleException"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Update Power Management Status*/
   nErrCode = DSL_DRV_VXX_PowerManagementStatusSet(pContext, DSL_G997_PMS_L3);
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
   nErrCode = DSL_DRV_VXX_FailReasonGet( pContext );
   if( nErrCode!= DSL_SUCCESS )
   {
      DSL_DEBUG(DSL_DBG_WRN,
         (pContext, SYS_DBG_WRN"DSL[%02d]: Can't get modem Fail reason"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }

   /* Request the handshake information about the far-end operating mode*/
   nErrCode = DSL_DRV_VXX_SendMsgSpar1Get(pContext);
   if( nErrCode!= DSL_SUCCESS )
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - SPAR1 get failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   /* Handle exception for CAM*/
   nErrCode = DSL_DRV_VXX_CamExceptionHandle(pContext);
   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - CAM exception handle failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   /* Get FW info*/
   DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.fwFeatures, sFwVersion);

   if (DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2))
   {
      nErrCode = DSL_DRV_VXX_DataPathFailuresStatusGet(pContext);
      if(nErrCode != DSL_SUCCESS)
      {
         DSL_DEBUG( DSL_DBG_ERR, (pContext,
            SYS_DBG_ERR"DSL[%02d]: ERROR - Data Path Failures update failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

        return nErrCode;
      }
   }

   /* Update Line failures information*/
   nErrCode = DSL_DRV_VXX_LineFailuresStatusUpdate(pContext);

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
   nErrCode = DSL_DRV_VXX_CamFsmUpdate(pContext);
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
#if defined(INCLUDE_DSL_CPE_API_VRX)
   DSL_ActivationFsmConfigData_t nActivationCfg;
#endif

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

#if defined(INCLUDE_DSL_CPE_API_VRX)
   DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.nActivationCfg, nActivationCfg);

   if (nActivationCfg.nActivationSequence == DSL_ACT_SEQ_NON_STD &&
       !(pXTSE[1-1] & XTSE_1_01_A_T1_413))
   {
      DSL_DEBUG( DSL_DBG_WRN, (pContext,
         SYS_DBG_ERR"DSL[%02d]: WARNING - Non-standard activation sequence configured "
                    "with a disabled T1.413 mode" DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));

      nErrCode = DSL_WRN_INCONSISTENT_XTSE_CONFIGURATION;
   }
#endif

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
   /* Nothing to be done for VINAX/VRx */
   return DSL_SUCCESS;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device.h'
*/
DSL_void_t DSL_DRV_DEV_InitDataFree(DSL_Init_t *pInit)
{
   /* Nothing to be done for VINAX/VRx */
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

   if (DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2))
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

#endif /* #if defined(INCLUDE_DSL_CPE_API_VINAX) || defined(INCLUDE_DSL_CPE_API_VRX)*/
