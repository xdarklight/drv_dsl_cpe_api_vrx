/******************************************************************************

                               Copyright (c) 2012
                            Lantiq Deutschland GmbH
                     Am Campeon 3; 85579 Neubiberg, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#define DSL_INTERN

#include "drv_dsl_cpe_api.h"

#if defined(INCLUDE_DSL_CPE_API_VRX) && defined(INCLUDE_DSL_PM)

#include "drv_dsl_cpe_pm_core.h"

#undef DSL_DBG_BLOCK
#define DSL_DBG_BLOCK DSL_DBG_PM

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device_pm.h'
*/
DSL_Error_t DSL_DRV_PM_DEV_Start(DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
#ifdef INCLUDE_DSL_CPE_PM_CONFIG
   DSL_Error_t nRet = DSL_SUCCESS;
   DSL_PM_ConfigData_t pmCfg;
#endif /* INCLUDE_DSL_CPE_PM_CONFIG*/

   /* Reset bPmDataValid flag*/
   DSL_DRV_PM_CONTEXT(pContext)->bPmDataValid = DSL_FALSE;

   /* Reset FW mode information*/
   DSL_DRV_PM_CONTEXT(pContext)->nLastShowtime = DSL_PM_FWMODE_NA;
   DSL_DRV_PM_CONTEXT(pContext)->nCurrShowtime = DSL_PM_FWMODE_NA;

#ifdef INCLUDE_DSL_CPE_PM_CONFIG
   /* Get PM module configuration data*/
   memcpy(&pmCfg, &(DSL_DRV_PM_CONTEXT(pContext)->nPmConfig),
      sizeof(DSL_PM_ConfigData_t));

   /* Send EOC configuration*/
   nRet = DSL_DRV_VRX_SendMsgOhOptionsSet(
             pContext,
             pmCfg.nBasicUpdateCycle * pmCfg.nFeUpdateCycleFactor,
             pmCfg.bFePollingOff ? DSL_FALSE : DSL_TRUE, DSL_TRUE);

   if ( (nRet < DSL_SUCCESS) &&
        (nRet != DSL_ERR_NOT_SUPPORTED_BY_FIRMWARE) )
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - EOC configuration set failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return nRet;
   }
#endif /* INCLUDE_DSL_CPE_PM_CONFIG*/

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device_pm.h'
*/
DSL_Error_t DSL_DRV_PM_DEV_Restart(DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
#ifdef INCLUDE_DSL_CPE_PM_CONFIG
   DSL_Error_t nRet = DSL_SUCCESS;
   DSL_PM_ConfigData_t pmCfg;
#endif /* INCLUDE_DSL_CPE_PM_CONFIG*/

#ifdef INCLUDE_DSL_CPE_PM_CONFIG
   /* Get PM module configuration data*/
   memcpy(&pmCfg, &(DSL_DRV_PM_CONTEXT(pContext)->nPmConfig),
      sizeof(DSL_PM_ConfigData_t));

   /* Send EOC configuration*/
   nRet = DSL_DRV_VRX_SendMsgOhOptionsSet(
             pContext,
             (pmCfg.nBasicUpdateCycle * pmCfg.nFeUpdateCycleFactor),
             pmCfg.bFePollingOff ? DSL_FALSE : DSL_TRUE, DSL_TRUE);

   if ( (nRet < DSL_SUCCESS) &&
        (nRet != DSL_ERR_NOT_SUPPORTED_BY_FIRMWARE) )
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - EOC configuration set failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return nRet;
   }
#endif /* INCLUDE_DSL_CPE_PM_CONFIG*/

   /* Allow counters polling if the current FW binary mode equals to the
      Last Showtime mode*/
   if( ((DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2)) &&
        (DSL_DRV_PM_CONTEXT(pContext)->nLastShowtime == DSL_PM_FWMODE_VDSL)) ||
       ((DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_ADSL)) &&
        (DSL_DRV_PM_CONTEXT(pContext)->nLastShowtime == DSL_PM_FWMODE_ADSL)) )
   {
      /* Set bPmDataValid flag*/
      DSL_DRV_PM_CONTEXT(pContext)->bPmDataValid = DSL_TRUE;
   }

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device_pm.h'
*/
DSL_Error_t DSL_DRV_PM_DEV_Suspend(DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   /* Reset bPmDataValid flag*/
   DSL_DRV_PM_CONTEXT(pContext)->bPmDataValid = DSL_FALSE;

   /* Update nLastShowtime value*/
   DSL_DRV_PM_CONTEXT(pContext)->nLastShowtime = DSL_DRV_PM_CONTEXT(pContext)->nCurrShowtime;

   return nErrCode;
}

DSL_Error_t DSL_DRV_PM_DEV_ShowtimeReachedHandle(DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
#ifdef INCLUDE_DSL_CPE_PM_CHANNEL_COUNTERS
   CMD_InteropConfigGet_t sCmd;
   ACK_InteropConfigGet_t sAck;

   if ( DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2))
   {
      /* Check for the Channel counters reset not need*/
      return DSL_SUCCESS;
   }

   memset(&sCmd, 0x0, sizeof(sCmd));

   sCmd.Index  = 0x1;
   sCmd.Length = 0x1;

   /* Get Interop status*/
   nErrCode = DSL_DRV_VRX_SendMessage(
                 pContext,
                 CMD_INTEROPCONFIGGET,
                 sizeof(sCmd), (DSL_uint8_t*)&(sCmd),
                 sizeof(sAck), (DSL_uint8_t*)&(sAck));

   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Interop Status get failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return nErrCode;
   }

   /* Check for the Channel counters FW autonomous reset*/
   if ( sAck.cntRestore == VRX_DISABLE )
   {
      /* Set bNeChannelCntReset flag. Flag will be cleared on the 1st
         Channel Counters FW access*/
      DSL_DRV_PM_CONTEXT(pContext)->bNeChannelCntReset = DSL_TRUE;

      DSL_DEBUG( DSL_DBG_MSG,
         (pContext, SYS_DBG_MSG"DSL[%02d]: FW channel counters autonomous reset is enabled"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }
#endif /* INCLUDE_DSL_CPE_PM_CHANNEL_COUNTERS*/

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device_pm.h'
*/
DSL_Error_t DSL_DRV_PM_DEV_OnEapsTimeout(DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   if (DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2))
   {
      DSL_DRV_PM_CONTEXT(pContext)->nCurrShowtime = DSL_PM_FWMODE_VDSL;
   }
   else if (DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_ADSL))
   {
      DSL_DRV_PM_CONTEXT(pContext)->nCurrShowtime = DSL_PM_FWMODE_ADSL;
   }
   else
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Unknown FW mode!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      DSL_DRV_PM_CONTEXT(pContext)->nCurrShowtime = DSL_PM_FWMODE_NA;
   }

   if (DSL_DRV_PM_CONTEXT(pContext)->nCurrShowtime != DSL_DRV_PM_CONTEXT(pContext)->nLastShowtime)
   {
      /* Lock PM module processing*/
      nErrCode = DSL_DRV_PM_Lock(pContext);
      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: ERROR - PM module lock failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         return nErrCode;
      }

      /*
         Reset all PM module counters, except Line Init
      */
#ifdef INCLUDE_DSL_CPE_PM_CHANNEL_COUNTERS
      /* Reset Channel Counters*/
      nErrCode = DSL_DRV_PM_CountersReset(
                    pContext, DSL_PM_COUNTER_CHANNEL, DSL_PM_RESET_ALL);
      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Channel counters reset failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      }
#endif /* #ifdef INCLUDE_DSL_CPE_PM_CHANNEL_COUNTERS*/

#ifdef INCLUDE_DSL_CPE_PM_LINE_COUNTERS
      /* Reset Line Sec Counters*/
      nErrCode = DSL_DRV_PM_CountersReset(
                    pContext, DSL_PM_COUNTER_LINE_SEC, DSL_PM_RESET_ALL);
      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: ERROR - Line Sec counters reset failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      }
#endif /* #ifdef INCLUDE_DSL_CPE_PM_LINE_COUNTERS*/

#ifdef INCLUDE_DSL_CPE_PM_DATA_PATH_COUNTERS
      /* Reset Data Path Counters*/
      nErrCode = DSL_DRV_PM_CountersReset(
                    pContext, DSL_PM_COUNTER_DATA_PATH, DSL_PM_RESET_ALL);
      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: ERROR - Data Path counters reset failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      }
#endif /* #ifdef INCLUDE_DSL_CPE_PM_DATA_PATH_COUNTERS*/

#ifdef INCLUDE_DSL_CPE_PM_DATA_PATH_FAILURE_COUNTERS
      /* Reset Data Path Counters*/
      nErrCode = DSL_DRV_PM_CountersReset(
                    pContext, DSL_PM_COUNTER_DATA_PATH_FAILURE, DSL_PM_RESET_ALL);
      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: ERROR - Data Path failure counters reset failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      }
#endif /* INCLUDE_DSL_CPE_PM_DATA_PATH_COUNTERS*/

#ifdef INCLUDE_DSL_CPE_PM_LINE_EVENT_SHOWTIME_COUNTERS
      /* Reset Line Event Showtime Counters*/
      nErrCode = DSL_DRV_PM_CountersReset(
                    pContext, DSL_PM_COUNTER_LINE_EVENT_SHOWTIME, DSL_PM_RESET_ALL);

      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: ERROR - Line failure counters reset failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      }
#endif /* INCLUDE_DSL_CPE_PM_DATA_PATH_COUNTERS*/

      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - PM counters reset failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      }
      else
      {
         /* Reset Internal PM counters state*/
         DSL_DRV_PM_InternalStateReset(pContext);

         DSL_DEBUG( DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: Permanent xDSL FW mode switch detected, all PM counters are reset..."
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      }

      DSL_DRV_WAKEUP_EVENT(DSL_DRV_PM_CONTEXT(pContext)->pmThreadNe.pmEvent);
      DSL_DRV_WAKEUP_EVENT(DSL_DRV_PM_CONTEXT(pContext)->pmThreadFe.pmEvent);

      /* Unlock PM module processing*/
      DSL_DRV_PM_UnLock(pContext);
   }

   if( nErrCode == DSL_SUCCESS )
   {
      /* Set bPmDataValid flag*/
      DSL_DRV_PM_CONTEXT(pContext)->bPmDataValid = DSL_TRUE;
   }

   return nErrCode;
}

#ifdef INCLUDE_DSL_CPE_PM_CHANNEL_COUNTERS
/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device_pm.h'
*/
DSL_Error_t DSL_DRV_PM_DEV_ChannelCountersGet(
   DSL_Context_t *pContext,
   DSL_uint8_t nChannel,
   DSL_XTUDir_t nDirection,
   DSL_PM_ChannelData_t *pCounters)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   union
   {
      /* CMDs for the NE and FE have the same structure*/
      CMD_CRC_StatsNE_Get_t CrcStatsNE;
      CMD_CRC_StatsFE_Get_t CrcStatsFE;
      /* CMDs for the NE and FE have the same structure*/
      CMD_FEC_StatsNE_Get_t FecStatsNE;
      CMD_FEC_StatsFE_Get_t FecStatsFE;
   } sCmd;

   union
   {
      /* ACKs for the NE and FE have the same structure*/
      ACK_CRC_StatsNE_Get_t CrcStatsNE;
      ACK_CRC_StatsFE_Get_t CrcStatsFE;
      /* ACKs for the NE and FE have the same structure*/
      ACK_FEC_StatsNE_Get_t FecStatsNE;
      ACK_FEC_StatsFE_Get_t FecStatsFE;
   } sAck;

   DSL_PM_ChannelData_t *pCurrCounters = DSL_NULL;

   DSL_CHECK_POINTER(pContext, pCounters);
   DSL_CHECK_ATU_DIRECTION(nDirection);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_PM_DEV_ChannelCountersCurrentGet, (nDirection=%s, nChannel=%d)"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nDirection==DSL_NEAR_END ? "NE":"FE", nChannel));

   /* Reset the output structure*/
   memset(pCounters, 0x0, sizeof(DSL_PM_ChannelData_t));

   if( DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2) )
   {
   /*... VDSL only mode*/
      memset(&(sAck.CrcStatsNE), 0x0, sizeof(sAck.CrcStatsNE));

      sCmd.CrcStatsFE.Index  = sCmd.CrcStatsNE.Index  = 0x0;
      sCmd.CrcStatsFE.Length = sCmd.CrcStatsNE.Length = 4;

      /* Get Code Violations*/
      nErrCode = DSL_DRV_VRX_SendMessage(
         pContext,
         nDirection == DSL_NEAR_END ? CMD_CRC_STATSNE_GET: CMD_CRC_STATSFE_GET,
         sizeof(sCmd.CrcStatsNE), (DSL_uint8_t*)&(sCmd.CrcStatsNE),
         sizeof(sAck.CrcStatsNE), (DSL_uint8_t*)&(sAck.CrcStatsNE));

      /* Check nErrCode and return on error*/
      if( nErrCode < 0 )
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Code Violations statistics"
             " Get failed!"DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return nErrCode;
      }

      /* ACKs for the CMD_CRC_STATSNE_GET and CMD_CRC_STATSFE_GET have the
          same strucrure, so we can use any*/
      pCounters->nCodeViolations = (DSL_uint32_t)sAck.CrcStatsNE.cntCVI_LSW |
                            (((DSL_uint32_t)sAck.CrcStatsNE.cntCVI_MSW) << 16);

      memset(&(sAck.FecStatsNE), 0x0, sizeof(sAck.FecStatsNE));

      sCmd.FecStatsFE.Index  = sCmd.FecStatsNE.Index  = 0x0;
      sCmd.FecStatsFE.Length = sCmd.FecStatsNE.Length = 12;

      /* Get Forward Error Corrections*/
      nErrCode = DSL_DRV_VRX_SendMessage(
         pContext,
         nDirection == DSL_NEAR_END ? CMD_FEC_STATSNE_GET: CMD_FEC_STATSFE_GET,
         sizeof(sCmd.FecStatsNE), (DSL_uint8_t*)&(sCmd.FecStatsNE),
         sizeof(sAck.FecStatsNE), (DSL_uint8_t*)&(sAck.FecStatsNE));

      /* Check nErrCode and return on error*/
      if( nErrCode < 0 )
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext,SYS_DBG_ERR"DSL[%02d]: ERROR - Forward Error Corrections"
            " statistics Get failed!" DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return nErrCode;
      }

      /* ACKs for the CMD_FEC_STATSNE_GET and CMD_FEC_STATSFE_GET have the
          same strucrure, so we can use any*/
      pCounters->nFEC = (DSL_uint32_t)sAck.FecStatsNE.cntECI_LSW |
                      (((DSL_uint32_t)sAck.FecStatsNE.cntECI_MSW) << 16);
   }
   else if(DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_ADSL))
   {
   /*... ADSL only mode*/
      memset(&(sAck.CrcStatsNE), 0x0, sizeof(sAck.CrcStatsNE));

      sCmd.CrcStatsNE.Index  = 0x0;
      sCmd.CrcStatsNE.Length = 4;

      /* Get Code Violations*/
      nErrCode = DSL_DRV_VRX_SendMessage(
         pContext,
         nDirection == DSL_NEAR_END ? CMD_CRC_STATSNE_GET : CMD_CRC_STATSFE_GET,
         sizeof(sCmd.CrcStatsNE), (DSL_uint8_t*)&(sCmd.CrcStatsNE),
         sizeof(sAck.CrcStatsNE), (DSL_uint8_t*)&(sAck.CrcStatsNE));

      /* Check nErrCode and return on error*/
      if( nErrCode < 0 )
      {
         DSL_DEBUG( DSL_DBG_ERR,
           (pContext,SYS_DBG_ERR"DSL[%02d]: ERROR - Code Violations statistics"
               " get failed!" DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return nErrCode;
      }

      /* ACKs for the CMD_CRC_STATSNE_GET and CMD_CRC_STATSFE_GET have the
          same strucrure, so we can use any*/
      pCounters->nCodeViolations = (DSL_uint32_t)sAck.CrcStatsNE.cntCVI_LSW |
                            (((DSL_uint32_t)sAck.CrcStatsNE.cntCVI_MSW) << 16);

      pCounters->nCodeViolations += (DSL_uint32_t)sAck.CrcStatsNE.cntCVF_LSW |
                            (((DSL_uint32_t)sAck.CrcStatsNE.cntCVF_MSW) << 16);

      memset(&(sAck.FecStatsNE), 0x0, sizeof(sAck.FecStatsNE));

      sCmd.FecStatsNE.Index  = 0x0;
      sCmd.FecStatsNE.Length = 12;

      /* Get Forward Error Corrections*/
      nErrCode = DSL_DRV_VRX_SendMessage(
         pContext,
         nDirection == DSL_NEAR_END ? CMD_FEC_STATSNE_GET : CMD_FEC_STATSFE_GET,
         sizeof(sCmd.FecStatsNE), (DSL_uint8_t*)&(sCmd.FecStatsNE),
         sizeof(sAck.FecStatsNE), (DSL_uint8_t*)&(sAck.FecStatsNE));

      /* Check nErrCode and return on error*/
      if( nErrCode < 0 )
      {
         DSL_DEBUG( DSL_DBG_ERR,
            (pContext,SYS_DBG_ERR"DSL[%02d]: ERROR - Forward Error Corrections"
               " statistics get failed!" DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return nErrCode;
      }

      /* ACKs for the CMD_FEC_STATSNE_GET and CMD_FEC_STATSFE_GET have the
          same strucrure, so we can use any*/
      pCounters->nFEC = (DSL_uint32_t)sAck.FecStatsNE.cntECI_LSW |
                      (((DSL_uint32_t)sAck.FecStatsNE.cntECI_MSW) << 16);

      pCounters->nFEC += (DSL_uint32_t)sAck.FecStatsNE.cntECF_CW_LSW |
                       (((DSL_uint32_t)sAck.FecStatsNE.cntECF_CW_MSW) << 16);

      /* Check for the NE counters FW autonomous reset*/
      if ((DSL_DRV_PM_CONTEXT(pContext)->bNeChannelCntReset == DSL_TRUE) &&
          (nDirection == DSL_NEAR_END))
      {
         /* Reset bNeChannelCntReset flag*/
         DSL_DRV_PM_CONTEXT(pContext)->bNeChannelCntReset = DSL_FALSE;

         /* Get pointer to the current channel counters*/
         pCurrCounters = DSL_DRV_PM_PTR_CHANNEL_COUNTERS_CURR(nChannel,nDirection);
         if (pCurrCounters == DSL_NULL)
         {
            nErrCode = DSL_ERR_INTERNAL;
         }
         else
         {
            DSL_DEBUG( DSL_DBG_MSG,
               (pContext, SYS_DBG_MSG"DSL[%02d]: Channel reference counters reset due to the enabled"
               " FW autonomous reset" DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            /* Reset current (reference) values*/
            pCurrCounters->nFEC = 0;
            pCurrCounters->nCodeViolations = 0;
         }
      }
   }
   else
   {
     DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Unknown operation mode!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_PM_DEV_ChannelCountersCurrentGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device_pm.h'
*/
DSL_Error_t DSL_DRV_PM_DEV_ChannelCountersSet(
   DSL_Context_t *pContext,
   DSL_uint8_t nChannel,
   DSL_XTUDir_t nDirection,
   DSL_PM_ChannelData_t *pCounters)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   union
   {
      CMD_CRC_StatsNE_Set_t   CrcStatsNE;
      CMD_FEC_StatsNE_Set_t   FecStatsNE;
   } sCmd;

   union
   {
      ACK_CRC_StatsNE_Set_t   CrcStatsNE;
      ACK_FEC_StatsNE_Set_t   FecStatsNE;
   } sAck;

   DSL_CHECK_POINTER(pContext, pCounters);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_PM_DEV_ChannelCountersSet,"
       " (nDirection=%s, nChannel=%d)"DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
       nDirection==DSL_NEAR_END ? "NE":"FE",nChannel));

   if (nDirection == DSL_FAR_END)
   {
      DSL_DEBUG(DSL_DBG_WRN,
          (pContext, SYS_DBG_WRN"DSL[%02d]: WARNING - Saving PM Channel Counters"
          " for FE are not supported!" DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_SUCCESS;
   }

   /* Restore counters only if the last showtime FW mode equals to the
      current loaded FW binary*/
   if( ((DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2)) &&
        (DSL_DRV_PM_CONTEXT(pContext)->nLastShowtime == DSL_PM_FWMODE_VDSL)) ||
       ((DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_ADSL)) &&
        (DSL_DRV_PM_CONTEXT(pContext)->nLastShowtime == DSL_PM_FWMODE_ADSL)) )
   {
      if( DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2) )
      {
         memset(&(sCmd.CrcStatsNE), 0x0, sizeof(sCmd.CrcStatsNE));

         /* Fill the message length parameter*/
         sCmd.CrcStatsNE.Length = 4;

         /* Fill the LSW of the nCodeViolations counter*/
         sCmd.CrcStatsNE.cntCVI_LSW = 
                           (DSL_uint16_t)(pCounters->nCodeViolations & 0xFFFF);
         /* Fill the MSW of the nCodeViolations counter*/
         sCmd.CrcStatsNE.cntCVI_MSW = 
                   (DSL_uint16_t)((pCounters->nCodeViolations >> 16) & 0xFFFF);

         /* Set Code Violations*/
         nErrCode = DSL_DRV_VRX_SendMessage(
            pContext, CMD_CRC_STATSNE_SET,
            sizeof(sCmd.CrcStatsNE), (DSL_uint8_t*)&(sCmd.CrcStatsNE),
            sizeof(sAck.CrcStatsNE), (DSL_uint8_t*)&(sAck.CrcStatsNE));

         if( nErrCode < 0 )
         {
            DSL_DEBUG( DSL_DBG_ERR,
               (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - CRC statistics "
                  "Set failed!" DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            return nErrCode;
         }

         memset(&(sCmd.FecStatsNE), 0x0, sizeof(sCmd.FecStatsNE));

         /* Fill the message length parameter*/
         sCmd.FecStatsNE.Length = 12;

         /* Fill the LSW of the nFEC counter*/
         sCmd.FecStatsNE.cntECI_LSW = 
                                      (DSL_uint16_t)(pCounters->nFEC & 0xFFFF);
         /* Fill the MSW of the nFEC counter*/
         sCmd.FecStatsNE.cntECI_MSW = 
                              (DSL_uint16_t)((pCounters->nFEC >> 16) & 0xFFFF);

         /* Set FEC*/
         nErrCode = DSL_DRV_VRX_SendMessage(
            pContext, CMD_FEC_STATSNE_SET,
            sizeof(sCmd.FecStatsNE), (DSL_uint8_t*)&(sCmd.FecStatsNE),
            sizeof(sAck.FecStatsNE), (DSL_uint8_t*)&(sAck.FecStatsNE));

         if( nErrCode < 0 )
         {
            DSL_DEBUG( DSL_DBG_ERR,
            (pContext,SYS_DBG_ERR"DSL[%02d]: ERROR - Forward Error Corrections"
               " statistics Set failed!" DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            return nErrCode;
         }
      }
      else
      {
         memset(&(sCmd.CrcStatsNE), 0x0, sizeof(sCmd.CrcStatsNE));

         /* Fill the message length parameter*/
         sCmd.CrcStatsNE.Length = 4;

         /* Fill the LSW of the nCodeViolations counter*/
         sCmd.CrcStatsNE.cntCVI_LSW = 
                           (DSL_uint16_t)(pCounters->nCodeViolations & 0xFFFF);
         /* Fill the MSW of the nCodeViolations counter*/
         sCmd.CrcStatsNE.cntCVI_MSW = 
                   (DSL_uint16_t)((pCounters->nCodeViolations >> 16) & 0xFFFF);

         /* Set Code Violations*/
         nErrCode = DSL_DRV_VRX_SendMessage(
            pContext, CMD_CRC_STATSNE_SET,
            sizeof(sCmd.CrcStatsNE), (DSL_uint8_t*)&(sCmd.CrcStatsNE),
            sizeof(sAck.CrcStatsNE), (DSL_uint8_t*)&(sAck.CrcStatsNE));

         if( nErrCode != DSL_SUCCESS )
         {
            DSL_DEBUG( DSL_DBG_ERR,
               (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Code Violations "
                "statistics Set failed!" DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            return nErrCode;
         }

         memset(&(sCmd.FecStatsNE), 0x0, sizeof(sCmd.FecStatsNE));

         /* Fill the message length parameter*/
         sCmd.FecStatsNE.Length = 12;

         /* Fill the LSW of the nFEC counter*/
         sCmd.FecStatsNE.cntECI_LSW = 
                                      (DSL_uint16_t)(pCounters->nFEC & 0xFFFF);
         /* Fill the MSW of the nFEC counter*/
         sCmd.FecStatsNE.cntECI_MSW = 
                              (DSL_uint16_t)((pCounters->nFEC >> 16) & 0xFFFF);

         /* Set FEC*/
         nErrCode = DSL_DRV_VRX_SendMessage(
            pContext, CMD_FEC_STATSNE_SET,
            sizeof(sCmd.FecStatsNE), (DSL_uint8_t*)&(sCmd.FecStatsNE),
            sizeof(sAck.FecStatsNE), (DSL_uint8_t*)&(sAck.FecStatsNE));

         if( nErrCode < 0 )
         {
            DSL_DEBUG( DSL_DBG_ERR,
            (pContext,SYS_DBG_ERR"DSL[%02d]: ERROR - Forward Error Corrections"
             " statistics Set failed!"DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            return nErrCode;
         }
      }
   }
   else
   {
      DSL_DEBUG( DSL_DBG_MSG,
         (pContext, SYS_DBG_MSG"DSL[%02d]: FW mode switch detected, PM channel"
           " counters restore skipped..."DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_PM_DEV_ChannelCountersSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}
#endif /* #ifdef INCLUDE_DSL_CPE_PM_CHANNEL_COUNTERS*/

#ifdef INCLUDE_DSL_CPE_PM_DATA_PATH_COUNTERS
/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device_pm.h'
*/
DSL_Error_t DSL_DRV_PM_DEV_DataPathCountersGet(
   DSL_Context_t *pContext,
   DSL_uint8_t nChannel,
   DSL_XTUDir_t nDirection,
   DSL_PM_DataPathData_t *pCounters)
{
   DSL_Error_t    nErrCode = DSL_SUCCESS;

   union
   {
      CMD_PTM_BC0_StatsNE_Get_t   cmdPtmStatsGet;
      CMD_ATM_BC0_StatsNE_Get_t   cmdAtmStatsGet;
      CMD_ATM_BC0_TxStatsNE_Get_t cmdAtmTxStatsGet;
   } sCmd;

   union
   {
      ACK_PTM_BC0_StatsNE_Get_t   ackPtmStatsGet;
      ACK_ATM_BC0_StatsNE_Get_t   ackAtmStatsGet;
      ACK_ATM_BC0_TxStatsNE_Get_t ackAtmTxStatsGet;
   } sAck;


   DSL_CHECK_POINTER(pContext, pCounters);
   DSL_CHECK_ERR_CODE();

   if( nChannel > 1 )
      return DSL_SUCCESS;

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_PM_DEV_DataPathCountersCurrentGet, (nDirection=%s, nChannel=%d)"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nDirection==DSL_NEAR_END ? "NE":"FE",nChannel));

   /* Clear the output structure*/
   memset(pCounters, 0x0, sizeof(DSL_PM_DataPathData_t));

   /* not for ADSL-only mode */
   if (DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2))
   {
      if( nDirection == DSL_NEAR_END )
      {
         memset(&(sAck.ackPtmStatsGet), 0x0, sizeof(sAck.ackPtmStatsGet));

         sCmd.cmdPtmStatsGet.Index  = 0x0;
         sCmd.cmdPtmStatsGet.Length = 8;

         /* Get PTM statistics*/
         nErrCode = DSL_DRV_VRX_SendMessage(
            pContext, CMD_PTM_BC0_STATSNE_GET,
            sizeof(sCmd.cmdPtmStatsGet), (DSL_uint8_t*)&(sCmd.cmdPtmStatsGet),
            sizeof(sAck.ackPtmStatsGet), (DSL_uint8_t*)&(sAck.ackPtmStatsGet));

         /* Check nErrCode and return on error*/
         if( nErrCode < 0 )
         {
            DSL_DEBUG( DSL_DBG_ERR,
               (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - PTM statistics get failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            return nErrCode;
         }

         /* Fill the output data structure*/
         pCounters->nCRCP_P = (sAck.ackPtmStatsGet.cntCRCP_MSW << 16) | sAck.ackPtmStatsGet.cntCRCP_LSW;
         pCounters->nCVP_P  = (sAck.ackPtmStatsGet.cntCVP_MSW << 16) | sAck.ackPtmStatsGet.cntCVP_LSW;

         pCounters->nCRC_P  = (sAck.ackPtmStatsGet.cntCRC_MSW << 16) | sAck.ackPtmStatsGet.cntCRC_LSW;
         pCounters->nCV_P   = (sAck.ackPtmStatsGet.cntCV_MSW << 16) | sAck.ackPtmStatsGet.cntCV_LSW;
      }
      else
      {
         DSL_DEBUG( DSL_DBG_WRN,
            (pContext, SYS_DBG_WRN"DSL[%02d]: WARNING - Data Path counters are not supported for the VDSL FE!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      }
   }
   else if (DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_ADSL))
   {
   /*... not for VDSL-only mode */
      if( nDirection == DSL_NEAR_END )
      {
         memset(&(sAck.ackAtmStatsGet), 0x0, sizeof(sAck.ackAtmStatsGet));

         sCmd.cmdAtmStatsGet.Index  = 0x0;
         sCmd.cmdAtmStatsGet.Length = 8;

         /* Get ATM statistics*/
         nErrCode = DSL_DRV_VRX_SendMessage(
            pContext, CMD_ATM_BC0_STATSNE_GET,
            sizeof(sCmd.cmdAtmStatsGet), (DSL_uint8_t*)&(sCmd.cmdAtmStatsGet),
            sizeof(sAck.ackAtmStatsGet), (DSL_uint8_t*)&(sAck.ackAtmStatsGet));

         /* Check nErrCode and return on error*/
         if( nErrCode < 0 )
         {
            DSL_DEBUG( DSL_DBG_ERR,
               (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - ATM statistics get failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            return nErrCode;
         }

         /* Fill the output data structure*/
         pCounters->nHEC            = (sAck.ackAtmStatsGet.hecp_MSW << 16) | sAck.ackAtmStatsGet.hecp_LSW;
         pCounters->nTotalCells     = (sAck.ackAtmStatsGet.cdp_MSW << 16) | sAck.ackAtmStatsGet.cdp_LSW;
         pCounters->nUserTotalCells = (sAck.ackAtmStatsGet.cup_MSW << 16) | sAck.ackAtmStatsGet.cup_LSW;
         pCounters->nIBE            = (sAck.ackAtmStatsGet.ibep_MSW << 16) | sAck.ackAtmStatsGet.ibep_LSW;

         memset(&(sCmd.cmdAtmTxStatsGet), 0x0, sizeof(sCmd.cmdAtmTxStatsGet));
         memset(&(sAck.ackAtmTxStatsGet), 0x0, sizeof(sAck.ackAtmTxStatsGet));

         sCmd.cmdAtmTxStatsGet.Index  = 0;
         sCmd.cmdAtmTxStatsGet.Length = 0x4;

         /* Get ATM Tx statistics*/
         nErrCode = DSL_DRV_VRX_SendMessage(
            pContext, CMD_ATM_BC0_TXSTATSNE_GET,
            sizeof(sCmd.cmdAtmTxStatsGet), (DSL_uint8_t*)&(sCmd.cmdAtmTxStatsGet),
            sizeof(sAck.ackAtmTxStatsGet), (DSL_uint8_t*)&(sAck.ackAtmTxStatsGet));

         /* Check nErrCode and return on error*/
         if( nErrCode < 0 )
         {
            DSL_DEBUG( DSL_DBG_ERR,
               (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - ATM Tx statistics get failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            return nErrCode;
         }

         /* Fill the output data structure*/
         pCounters->nTxUserTotalCells = (sAck.ackAtmTxStatsGet.TxCU_MSW << 16) |
                                         sAck.ackAtmTxStatsGet.TxCU_LSW;
         pCounters->nTxIBE            = (sAck.ackAtmTxStatsGet.TxIdle_MSW << 16) |
                                         sAck.ackAtmTxStatsGet.TxIdle_LSW;
      }
      else
      {
         DSL_DEBUG( DSL_DBG_WRN,
            (pContext, SYS_DBG_WRN"DSL[%02d]: WARNING - Data Path Counters are not supported "
            "for the ADSL FE!" DSL_DRV_CRLF,
            DSL_DEV_NUM(pContext)));
      }
   }
   else
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Unknown operation mode!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_PM_DEV_DataPathCountersCurrentGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device_pm.h'
*/
DSL_Error_t DSL_DRV_PM_DEV_DataPathCountersSet(
   DSL_Context_t *pContext,
   DSL_uint8_t nChannel,
   DSL_XTUDir_t nDirection,
   DSL_PM_DataPathData_t *pCounters)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   union
   {
      CMD_PTM_BC0_StatsNE_Set_t   PTM;
      CMD_ATM_BC0_StatsNE_Set_t   ATM;
      CMD_ATM_BC0_TxStatsNE_Set_t TX_ATM;
   } sCmd;

   union
   {
      ACK_PTM_BC0_StatsNE_Set_t   PTM;
      ACK_ATM_BC0_StatsNE_Set_t   ATM;
      ACK_ATM_BC0_TxStatsNE_Set_t TX_ATM;
   } sAck;


   DSL_CHECK_POINTER(pContext, pCounters);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_PM_DEV_DataPathCountersSet, (nDirection=%s, nChannel=%d)"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nDirection==DSL_NEAR_END ? "NE":"FE",nChannel));

   /* Restore counters only if the last showtime FW mode equals to the
      current loaded FW binary*/
   if( ((DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2)) &&
        (DSL_DRV_PM_CONTEXT(pContext)->nLastShowtime == DSL_PM_FWMODE_VDSL)) ||
       ((DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_ADSL)) &&
        (DSL_DRV_PM_CONTEXT(pContext)->nLastShowtime == DSL_PM_FWMODE_ADSL)) )
   {
      if( DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2) )
      {
         memset(&(sCmd.PTM), 0x0, sizeof(sCmd.PTM));
         sCmd.PTM.Length = 8;

         sCmd.PTM.cntCRC_LSW  = pCounters->nCRC_P & 0xFFFF;
         sCmd.PTM.cntCRC_MSW  = (pCounters->nCRC_P >> 16) & 0xFFFF;
         sCmd.PTM.cntCRCP_LSW = pCounters->nCRCP_P & 0xFFFF;
         sCmd.PTM.cntCRCP_MSW = (pCounters->nCRCP_P >> 16) & 0xFFFF;
         sCmd.PTM.cntCV_LSW   = pCounters->nCV_P & 0xFFFF;
         sCmd.PTM.cntCV_MSW   = (pCounters->nCV_P >> 16) & 0xFFFF;
         sCmd.PTM.cntCVP_LSW  = pCounters->nCVP_P & 0xFFFF;
         sCmd.PTM.cntCVP_MSW  = (pCounters->nCVP_P >> 16) & 0xFFFF;


         /* Set PTM statistics*/
         nErrCode = DSL_DRV_VRX_SendMessage(
            pContext, CMD_PTM_BC0_STATSNE_SET,
            sizeof(sCmd.PTM), (DSL_uint8_t*)&(sCmd.PTM),
            sizeof(sAck.PTM), (DSL_uint8_t*)&(sAck.PTM));

         if( nErrCode != DSL_SUCCESS )
         {
            DSL_DEBUG( DSL_DBG_ERR,
               (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - ATM statistics Set failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            return nErrCode;
         }
      }
      else
      {
         memset(&(sCmd.ATM), 0x0, sizeof(sCmd.ATM));
         sCmd.ATM.Length = 8;

         /* Fill Near-end Hec Violation Count (HEC-P), Current Period*/
         sCmd.ATM.hecp_LSW = pCounters->nHEC & 0xFFFF;
         sCmd.ATM.hecp_MSW = (pCounters->nHEC >> 16) & 0xFFFF;
         /* Fill Near-end Delineated Total Cell Count (CD-P), Current Period*/
         sCmd.ATM.cdp_LSW = pCounters->nTotalCells & 0xFFFF;
         sCmd.ATM.cdp_MSW = (pCounters->nTotalCells >> 16) & 0xFFFF;
         /* Fill Near-end User Total Cell Count (CU-P), Current Period*/
         sCmd.ATM.cup_LSW = pCounters->nUserTotalCells & 0xFFFF;
         sCmd.ATM.cup_MSW = (pCounters->nUserTotalCells >> 16) & 0xFFFF;
         /* Fill Near-end Idle Cell Bit Error Count (IBE-P), Current Period*/
         sCmd.ATM.ibep_LSW = pCounters->nIBE & 0xFFFF;
         sCmd.ATM.ibep_MSW = (pCounters->nIBE >> 16) & 0xFFFF;

         /* Ignore last 8 parameters (sizeof(sCmd.ATM) - 8 * sizeof(DSL_uint16_t))*/

         /* Set ATM statistics*/
         nErrCode = DSL_DRV_VRX_SendMessage(
            pContext, CMD_ATM_BC0_STATSNE_SET,
            sizeof(sCmd.ATM) - 8 * sizeof(DSL_uint16_t), (DSL_uint8_t*)&(sCmd.ATM),
            sizeof(sAck.ATM), (DSL_uint8_t*)&(sAck.ATM));

         if( nErrCode != DSL_SUCCESS )
         {
            DSL_DEBUG( DSL_DBG_ERR,
               (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - ATM statistics Set failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            return nErrCode;
         }

         memset(&(sCmd.TX_ATM), 0x0, sizeof(sCmd.TX_ATM));
         memset(&(sAck.TX_ATM), 0x0, sizeof(sAck.TX_ATM));

         sCmd.TX_ATM.Index  = 0;
         sCmd.TX_ATM.Length = 4;

         /* Fill Near-end Transmit ATM user cell count (TxCU), Current Period*/
         sCmd.TX_ATM.TxCU_LSW = pCounters->nTxUserTotalCells & 0xFFFF;
         sCmd.TX_ATM.TxCU_MSW = (pCounters->nTxUserTotalCells >> 16) & 0xFFFF;
         /* Fill Near-end Transmit ATM idle cell count (nTxIBE), Current Period*/
         sCmd.TX_ATM.TxIdle_LSW = pCounters->nTxIBE & 0xFFFF;
         sCmd.TX_ATM.TxIdle_MSW = (pCounters->nTxIBE >> 16) & 0xFFFF;

         /* Set ATM Tx statistics*/
         nErrCode = DSL_DRV_VRX_SendMessage(
            pContext, CMD_ATM_BC0_TXSTATSNE_SET,
            sizeof(sCmd.TX_ATM), (DSL_uint8_t*)&(sCmd.TX_ATM),
            sizeof(sAck.TX_ATM), (DSL_uint8_t*)&(sAck.TX_ATM));

         if( nErrCode != DSL_SUCCESS )
         {
            DSL_DEBUG( DSL_DBG_ERR,
               (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - ATM Tx statistics Set failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            return nErrCode;
         }
      }
   }
   else
   {
      DSL_DEBUG( DSL_DBG_MSG,
         (pContext, SYS_DBG_MSG"DSL[%02d]: FW mode switch detected, PM data path counters restore skipped..."
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_PM_DEV_DataPathCountersSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}
#endif /* #ifdef INCLUDE_DSL_CPE_PM_DATA_PATH_COUNTERS*/

#ifdef INCLUDE_DSL_CPE_PM_DATA_PATH_FAILURE_COUNTERS
/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device_pm.h'
*/
DSL_Error_t DSL_DRV_PM_DEV_DataPathFailureCountersGet(
   DSL_Context_t *pContext,
   DSL_uint8_t nChannel,
   DSL_XTUDir_t nDirection,
   DSL_PM_DataPathFailureData_t *pCounters)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_CHECK_POINTER(pContext, pCounters);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_CHANNEL_RANGE(nChannel);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_ATU_DIRECTION(nDirection);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_PM_DEV_DataPathFailureCountersGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   if(DSL_DRV_MUTEX_LOCK(pContext->dataMutex))
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: Couldn't lock data mutex"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERR_SEMAPHORE_GET;
   }

   /* Copy Data Path failure counters from the CPE API Context*/
   memcpy(pCounters,
      nDirection == DSL_NEAR_END ?
      &(pContext->pmDataPathFailureCounters.data_ne[nChannel]) :
      &(pContext->pmDataPathFailureCounters.data_fe[nChannel]),
      sizeof(DSL_PM_DataPathFailureData_t));

   DSL_DRV_MUTEX_UNLOCK(pContext->dataMutex);

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_PM_DEV_DataPathFailureCountersGet (retCode=%d)"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}
#endif /* INCLUDE_DSL_CPE_PM_DATA_PATH_FAILURE_COUNTERS*/

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device_pm.h'
*/
DSL_Error_t DSL_DRV_PM_DEV_LineSecCountersGet(
   DSL_Context_t *pContext,
   DSL_XTUDir_t nDirection,
   DSL_PM_LineSecData_t *pCounters)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   union
   {
      /* The same CMD structure for both NE and FE*/
      CMD_LinePerfCountNE_Get_t  NE;
      CMD_LinePerfCountFE_Get_t  FE;
   } sCmd;

   union
   {
      /* The same ACK structure for both NE and FE*/
      ACK_LinePerfCountNE_Get_t  NE;
      ACK_LinePerfCountFE_Get_t  FE;
   } sAck;


   DSL_CHECK_POINTER(pContext, pCounters);
   DSL_CHECK_ATU_DIRECTION(nDirection);
   DSL_CHECK_ERR_CODE();

   /* Reset the output structure*/
   memset(pCounters, 0x0, sizeof(DSL_PM_LineSecData_t));

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_PM_DEV_LineSecCountersCurrentGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   memset(&(sCmd), 0, sizeof(sCmd));
   memset(&(sAck), 0, sizeof(sAck));

   /* For VDSL only mode*/
   if( DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2) )
   {
      sCmd.NE.Length = 10;

      nErrCode = DSL_DRV_VRX_SendMessage(
         pContext,
         nDirection == DSL_NEAR_END ? CMD_LINEPERFCOUNTNE_GET : CMD_LINEPERFCOUNTFE_GET,
         (DSL_uint16_t)sizeof(sCmd.NE), (DSL_uint8_t*)&(sCmd.NE),
         (DSL_uint16_t)sizeof(sAck.NE), (DSL_uint8_t*)&(sAck.NE));

      if( nErrCode == DSL_SUCCESS )
      {
         /* Get current ES value*/
         pCounters->nES   = (sAck.NE.cntES_MSW << 16) | sAck.NE.cntES_LSW;
         /* Get current SES value*/
         pCounters->nSES  = (sAck.NE.cntSES_MSW << 16) | sAck.NE.cntSES_LSW;
         /* Get current LOSS value*/
         pCounters->nLOSS  = (sAck.NE.cntLOSS_MSW << 16) | sAck.NE.cntLOSS_LSW;
         /* Get current UAS value*/
         pCounters->nUAS  = (sAck.NE.cntUAS_MSW << 16) | sAck.NE.cntUAS_LSW;
         /* Not supported by the VRX VDSL FW*/
         pCounters->nLOFS = 0;
      }
   }
   else if(DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_ADSL))
   {
      sCmd.NE.Length = 10;

      nErrCode = DSL_DRV_VRX_SendMessage(
         pContext,
         nDirection == DSL_NEAR_END ? CMD_LINEPERFCOUNTNE_GET : CMD_LINEPERFCOUNTFE_GET,
         sizeof(sCmd.NE), (DSL_uint8_t*)&(sCmd.NE),
         sizeof(sAck.NE), (DSL_uint8_t*)&(sAck.NE));

      if( nErrCode == DSL_SUCCESS )
      {
         /* Get current ES value*/
         pCounters->nES   = (sAck.NE.cntES_MSW << 16) | sAck.NE.cntES_LSW;
         /* Get current SES value*/
         pCounters->nSES  = (sAck.NE.cntSES_MSW << 16) | sAck.NE.cntSES_LSW;
         /* Get current LOSS value*/
         pCounters->nLOSS = (sAck.NE.cntLOSS_MSW << 16) | sAck.NE.cntLOSS_LSW;
         /* UAS counter not supported by the FW in the ADSL mode*/
         pCounters->nUAS  = 0;
         /* LOFS counter not supported by the FW in the ADSL mode*/
         pCounters->nLOFS = 0;
      }
   }
   else
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Unknown operation mode!"
          DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_PM_DEV_LineSecCountersCurrentGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device_pm.h'
*/
DSL_Error_t DSL_DRV_PM_DEV_LineSecCountersSet(
   DSL_Context_t *pContext,
   DSL_XTUDir_t nDirection,
   DSL_PM_LineSecData_t *pCounters)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   union
   {
      CMD_LinePerfCountNE_Set_t  LinePerfNE;
   } sCmd;

   union
   {
      ACK_LinePerfCountNE_Set_t  LinePerfNE;
   } sAck;


   DSL_CHECK_POINTER(pContext, pCounters);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext,SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_PM_DEV_LineSecCountersSet,"
      " (nDirection=%s)"DSL_DRV_CRLF, DSL_DEV_NUM(pContext), 
       nDirection==DSL_NEAR_END ? "NE":"FE"));

   if (nDirection == DSL_FAR_END)
   {
      DSL_DEBUG( DSL_DBG_WRN,
         (pContext, SYS_DBG_WRN"DSL[%02d]: WARNING - Saving PM Line Performance"
          " Counters for FE are not supported!" DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_SUCCESS;
   }

   /* Restore counters only if the last showtime FW mode equals to the
      current loaded FW binary*/
   if( ((DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2)) &&
        (DSL_DRV_PM_CONTEXT(pContext)->nLastShowtime == DSL_PM_FWMODE_VDSL)) ||
       ((DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_ADSL)) &&
        (DSL_DRV_PM_CONTEXT(pContext)->nLastShowtime == DSL_PM_FWMODE_ADSL)) )
   {
      /* Only applicable for ADSL mode*/
      if( DSL_DRV_VXX_FwFeatureCheck(pContext, DSL_VXX_FW_VDSL2) )
      {
         memset(&(sCmd.LinePerfNE), 0x0, sizeof(sCmd.LinePerfNE));

         /* Fill the message*/
         sCmd.LinePerfNE.Length      = 10;
         sCmd.LinePerfNE.cntES_MSW   = (DSL_uint16_t)(pCounters->nES>>16);
         sCmd.LinePerfNE.cntES_LSW   = (DSL_uint16_t)pCounters->nES;
         sCmd.LinePerfNE.cntSES_MSW  = (DSL_uint16_t)(pCounters->nSES>>16);
         sCmd.LinePerfNE.cntSES_LSW  = (DSL_uint16_t)(pCounters->nSES);
         sCmd.LinePerfNE.cntES_MSW   = (DSL_uint16_t)(pCounters->nLOSS>>16);
         sCmd.LinePerfNE.cntLOSS_LSW = (DSL_uint16_t)(pCounters->nLOSS);
         sCmd.LinePerfNE.cntUAS_MSW  = (DSL_uint16_t)(pCounters->nUAS>>16);
         sCmd.LinePerfNE.cntUAS_LSW  = (DSL_uint16_t)(pCounters->nUAS);

         /* Set Line sec counters*/
         nErrCode =  DSL_DRV_VRX_SendMessage(
            pContext, CMD_LINEPERFCOUNTNE_SET,
            sizeof(sCmd.LinePerfNE), (DSL_uint8_t *)&(sCmd.LinePerfNE),
            sizeof(sAck.LinePerfNE),(DSL_uint8_t *)&(sAck.LinePerfNE));
      }
      else
      {
         memset(&(sCmd.LinePerfNE), 0x0, sizeof(sCmd.LinePerfNE));

         /* Fill the message*/
         sCmd.LinePerfNE.Length      = 10;
         sCmd.LinePerfNE.cntES_MSW   = (DSL_uint16_t)(pCounters->nES>>16);
         sCmd.LinePerfNE.cntES_LSW   = (DSL_uint16_t)pCounters->nES;
         sCmd.LinePerfNE.cntSES_MSW  = (DSL_uint16_t)(pCounters->nSES>>16);
         sCmd.LinePerfNE.cntSES_LSW  = (DSL_uint16_t)(pCounters->nSES);
         sCmd.LinePerfNE.cntLOSS_MSW = (DSL_uint16_t)(pCounters->nLOSS>>16);
         sCmd.LinePerfNE.cntLOSS_LSW = (DSL_uint16_t)(pCounters->nLOSS);
         sCmd.LinePerfNE.cntUAS_MSW  = (DSL_uint16_t)(pCounters->nUAS>>16);
         sCmd.LinePerfNE.cntUAS_LSW  = (DSL_uint16_t)(pCounters->nUAS);

         /* Set Line sec counters*/
         nErrCode =  DSL_DRV_VRX_SendMessage(
            pContext, CMD_LINEPERFCOUNTNE_SET,
            sizeof(sCmd.LinePerfNE), (DSL_uint8_t *)&(sCmd.LinePerfNE),
            sizeof(sAck.LinePerfNE),(DSL_uint8_t *)&(sAck.LinePerfNE));
      }
   }
   else
   {
      DSL_DEBUG( DSL_DBG_MSG,
         (pContext,SYS_DBG_MSG"DSL[%02d]: FW mode switch detected, PM line sec"
         " counters restore skipped..." DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }

   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR,
        (pContext,SYS_DBG_ERR"DSL[%02d]: ERROR - Line sec Counters Set failed!"
        DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext,SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_PM_DEV_LineSecCountersSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

#ifdef INCLUDE_DSL_CPE_PM_LINE_COUNTERS
/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device_pm.h'
*/
DSL_Error_t DSL_DRV_PM_DEV_LineInitCountersGet(
   DSL_Context_t *pContext,
   DSL_PM_LineInitData_t *pCounters)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint32_t nFullInits = 0, nFailedFullInits = 0;

   DSL_CHECK_POINTER(pContext, pCounters);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_PM_DEV_LineInitCountersCurrentGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Reset the output structure*/
   memset(pCounters, 0x0, sizeof(DSL_PM_LineInitData_t));

   /* Get Line initialization performance counters*/
   DSL_CTX_READ(pContext, nErrCode, nFullInits, nFullInits);
   DSL_CTX_READ(pContext, nErrCode, nFailedFullInits, nFailedFullInits);

   pCounters->nFullInits        = nFullInits;
   pCounters->nFailedFullInits  = nFailedFullInits;
   /* Short Init stuff is not supported*/
   pCounters->nShortInits       = 0;
   pCounters->nFailedShortInits = 0;

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_PM_DEV_LineInitCountersCurrentGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

DSL_Error_t DSL_DRV_PM_DEV_LineInitCountersSet(
   DSL_Context_t *pContext,
   DSL_PM_LineInitData_t *pCounters)
{
   DSL_Error_t    nErrCode = DSL_SUCCESS;

   DSL_CHECK_POINTER(pContext, pCounters);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_PM_DEV_LineInitCountersSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Today, Line Init counters are completely handled on the API level*/

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_PM_DEV_LineInitCountersSet, retCode"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}
#endif /* #ifdef INCLUDE_DSL_CPE_PM_LINE_COUNTERS*/

#ifdef INCLUDE_DSL_CPE_PM_LINE_EVENT_SHOWTIME_COUNTERS
/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device_pm.h'
*/
DSL_Error_t DSL_DRV_PM_DEV_LineEventShowtimeCountersGet(
   DSL_Context_t *pContext,
   DSL_XTUDir_t nDirection,
   DSL_PM_LineEventShowtimeData_t *pCounters)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
#if 0
   /* VRx SOS counter not supported by the FW yet, set to 0*/
   CMD_SOS_StatsNE_Get_t sCmd;
   ACK_SOS_StatsNE_Get_t sAck;
#endif

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_PM_DEV_LineEventShowtimeCountersGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   DSL_CHECK_POINTER(pContext, pCounters);
   DSL_CHECK_ATU_DIRECTION(nDirection);
   DSL_CHECK_ERR_CODE();

   if(DSL_DRV_MUTEX_LOCK(pContext->dataMutex))
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: Couldn't lock data mutex"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERR_SEMAPHORE_GET;
   }

   /* Copy Line Event Showtime counters from the CPE API Context*/
   memcpy(pCounters,
      nDirection == DSL_NEAR_END ?
      &(pContext->pmLineEventShowtimeCounters.data_ne) :
      &(pContext->pmLineEventShowtimeCounters.data_fe),
      sizeof(DSL_PM_LineEventShowtimeData_t));

   DSL_DRV_MUTEX_UNLOCK(pContext->dataMutex);

#if 0
   memset(&sCmd, 0x0, sizeof(CMD_SOS_StatsNE_Get_t));
   memset(&sAck, 0x0, sizeof(ACK_SOS_StatsNE_Get_t));

   sCmd.Index  = 0;
   sCmd.Length = 0x2;

   /* SOS Statistics*/
   nErrCode = DSL_DRV_VRX_SendMessage(
      pContext,
      nDirection == DSL_NEAR_END ? CMD_SOS_STATSNE_GET: CMD_SOS_STATSFE_GET,
      sizeof(sCmd), (DSL_uint8_t*)&(sCmd),
      sizeof(sAck), (DSL_uint8_t*)&(sAck));

   /* Check nErrCode and return on error*/
   if( nErrCode < 0 )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - SOS statistics get failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   pCounters->nSosSuccess = (((DSL_uint32_t)sAck.cntSOS_SUC_MSW) << 16) |
                             ((DSL_uint32_t)sAck.cntSOS_SUC_LSW);
#else
   /* VRx SOS counter not supported by the FW yet, set to 0*/
   pCounters->nSosSuccess = 0;
#endif

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_PM_DEV_LineEventShowtimeCountersGet (retCode=%d)"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device_pm.h'
*/
DSL_Error_t DSL_DRV_PM_DEV_LineEventShowtimeCountersSet(
   DSL_Context_t *pContext,
   DSL_XTUDir_t nDirection,
   DSL_PM_LineEventShowtimeData_t *pCounters)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
#if 0
   /* Both NE and FE structures are the same*/
   CMD_SOS_StatsNE_Set_t sCmd;
   ACK_SOS_StatsNE_Set_t sAck;
#endif

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_PM_DEV_LineEventShowtimeCountersSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   DSL_CHECK_POINTER(pContext, pCounters);
   DSL_CHECK_ATU_DIRECTION(nDirection);
   DSL_CHECK_ERR_CODE();

   /* VRx SOS counter not supported by the FW yet, do nothing*/
#if 0
   memset(&sCmd, 0x0, sizeof(CMD_SOS_StatsNE_Set_t));
   memset(&sAck, 0x0, sizeof(ACK_SOS_StatsNE_Set_t));

   sCmd.Index  = 0;
   sCmd.Length = 0x2;

   sCmd.cntSOS_SUC_LSW = (DSL_uint16_t)(pCounters->nSosSuccess & 0x0000FFFF);
   sCmd.cntSOS_SUC_MSW = (DSL_uint16_t)((pCounters->nSosSuccess >> 16) & 0x0000FFFF);

   /* SOS Statistics*/
   nErrCode = DSL_DRV_VRX_SendMessage(
      pContext,
      nDirection == DSL_NEAR_END ? CMD_SOS_STATSNE_SET: CMD_SOS_STATSFE_SET,
      sizeof(sCmd), (DSL_uint8_t*)&(sCmd),
      sizeof(sAck), (DSL_uint8_t*)&(sAck));

   /* Check nErrCode and return on error*/
   if( nErrCode < 0 )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - SOS statistics get failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }
#endif

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_PM_DEV_LineEventShowtimeCountersSet (retCode=%d)"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}
#endif /* INCLUDE_DSL_CPE_PM_LINE_EVENT_SHOWTIME_COUNTERS*/

#ifdef INCLUDE_DSL_CPE_PM_RETX_COUNTERS
/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_device_pm.h'
*/
DSL_Error_t DSL_DRV_PM_DEV_ReTxCountersGet(
   DSL_Context_t *pContext,
   DSL_XTUDir_t nDirection,
   DSL_PM_ReTxData_t *pCounters)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_PM_DEV_ReTxCountersGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   DSL_CHECK_POINTER(pContext, pCounters);
   DSL_CHECK_ATU_DIRECTION(nDirection);
   DSL_CHECK_ERR_CODE();

   memset(pCounters, 0x0, sizeof(DSL_PM_ReTxData_t));

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_PM_DEV_ReTxCountersGet (retCode=%d)"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return nErrCode;
}
#endif /* INCLUDE_DSL_CPE_PM_RETX_COUNTERS*/


#ifdef INCLUDE_DSL_CPE_PM_HISTORY
DSL_Error_t DSL_DRV_PM_DEV_HistoryIntervalAlign(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint8_t i = 0, nChannel = 0;
#ifdef INCLUDE_DSL_CPE_PM_CHANNEL_COUNTERS
   DSL_PM_ChannelData_t ChannelData;
#endif /* INCLUDE_DSL_CPE_PM_CHANNEL_COUNTERS*/
#ifdef INCLUDE_DSL_CPE_PM_DATA_PATH_COUNTERS
   DSL_PM_DataPathData_t DataPathData;
#endif /* INCLUDE_DSL_CPE_PM_DATA_PATH_COUNTERS*/
   DSL_PM_LineSecData_t LineSecData;

   DSL_CHECK_CTX_POINTER(pContext);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_PM_DEV_HistoryIntervalAlign"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Lock PM module FE direction mutex*/
   nErrCode = DSL_DRV_PM_DirectionMutexControl(pContext, DSL_FAR_END, DSL_TRUE);
   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - PM Far-End mutex lock failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   /* Check if there are pending triggers (15min/1day) within the current
      EAPS timeframe. Reset all PM FW counters in this case.
      This condition is met only in case of FW switching from one xDSL mode to another.*/
   if ((DSL_DRV_PM_CONTEXT(pContext)->bPmDataValid == DSL_FALSE) &&
       (DSL_DRV_PM_CONTEXT(pContext)->b15minElapsed || DSL_DRV_PM_CONTEXT(pContext)->b1dayElapsed))
   {
      /* Step over directions*/
      for (i = 0; i < 2; i++)
      {
         /* Step over all available channels*/
         for( nChannel = 0; nChannel < DSL_CHANNELS_PER_LINE; nChannel++ )
         {
#ifdef INCLUDE_DSL_CPE_PM_CHANNEL_COUNTERS
            /* Reset FW Channel counters*/
            memset(&ChannelData, 0x0, sizeof(DSL_PM_ChannelData_t));
            nErrCode = DSL_DRV_PM_DEV_ChannelCountersSet(
                          pContext, nChannel, i == 0 ? DSL_NEAR_END : DSL_FAR_END,
                          &ChannelData);
            if (nErrCode != DSL_SUCCESS)
            {
               DSL_DEBUG( DSL_DBG_ERR,
                  (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW Channel Counters reset failed!"
                  DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

               break;
            }
#endif /* INCLUDE_DSL_CPE_PM_CHANNEL_COUNTERS*/

#ifdef INCLUDE_DSL_CPE_PM_DATA_PATH_COUNTERS
            /* Reset FW Data Path counters*/
            memset(&DataPathData, 0x0, sizeof(DSL_PM_DataPathData_t));
            nErrCode = DSL_DRV_PM_DEV_DataPathCountersSet(
                          pContext, nChannel, i == 0 ? DSL_NEAR_END : DSL_FAR_END,
                          &DataPathData);
            if (nErrCode != DSL_SUCCESS)
            {
               DSL_DEBUG( DSL_DBG_ERR,
                  (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW Data Path Counters reset failed!"
                  DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

               break;
            }
#endif /* INCLUDE_DSL_CPE_PM_DATA_PATH_COUNTERS*/
         }

         if (nErrCode != DSL_SUCCESS)
         {
            break;
         }

         /* Reset FW Line Sec counters*/
         memset(&LineSecData, 0x0, sizeof(DSL_PM_LineSecData_t));
         nErrCode = DSL_DRV_PM_DEV_LineSecCountersSet(
                       pContext, i == 0 ? DSL_NEAR_END : DSL_FAR_END,
                       &LineSecData);
         if (nErrCode != DSL_SUCCESS)
         {
            DSL_DEBUG( DSL_DBG_ERR,
               (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW Line Sec Counters reset failed!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

            break;
         }
      }

      /* Reset last UAS counter*/
      DSL_DRV_PM_CONTEXT(pContext)->pCounters->lineSecCounters.auxData.sec_data_ne.nUASLast = 0;
      DSL_DRV_PM_CONTEXT(pContext)->pCounters->lineSecCounters.auxData.sec_data_fe.nUASLast = 0;
   }

   /* Unlock PM module FE direction mutex*/
   DSL_DRV_PM_DirectionMutexControl(pContext, DSL_FAR_END, DSL_FALSE);

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_PM_DEV_HistoryIntervalAlign"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}
#endif /* INCLUDE_DSL_CPE_PM_HISTORY*/

#endif /* #ifdef defined(INCLUDE_DSL_CPE_API_VRX) && defined(INCLUDE_DSL_PM) */
