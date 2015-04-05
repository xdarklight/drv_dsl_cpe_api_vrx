/******************************************************************************

                              Copyright (c) 2013
                            Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/
#define DSL_INTERN

#include "drv_dsl_cpe_api.h"

#ifdef INCLUDE_DSL_CPE_API_VRX

#undef DSL_DBG_BLOCK
#define DSL_DBG_BLOCK DSL_DBG_DEVICE

#ifndef DSL_DEBUG_DISABLE
/** Blacklist for VRX message dump
   all message IDs contained here will occur in the
   message dump only from level MSG downwards */
VRX_MsgId_t DSL_DRV_VRX_g_MsgDumpBlacklist[]=
{
   {(DSL_uint16_t)(CMD_MODEMFSM_STATEGET)    },
   {(DSL_uint16_t)(CMD_LINEPERFCOUNTNE_GET)  },
   {(DSL_uint16_t)(CMD_LINEPERFCOUNTFE_GET)  },
   {(DSL_uint16_t)(CMD_PTM_BC0_STATSNE_GET)  },
   {(DSL_uint16_t)(CMD_ATM_BC0_STATSNE_GET)  },
   {(DSL_uint16_t)(CMD_CRC_STATSNE_GET)      },
   {(DSL_uint16_t)(CMD_FEC_STATSNE_GET)      },
   {(DSL_uint16_t)(CMD_CRC_STATSFE_GET)      },
   {(DSL_uint16_t)(CMD_FEC_STATSFE_GET)      },
   {(DSL_uint16_t)(CMD_OLR_US_STATSGET)      },
   {(DSL_uint16_t)(CMD_ADSL_FRAMEDATAUS_LP0GET)},
   {(DSL_uint16_t)(CMD_BEARERCHSUS_GET)      },
   {(DSL_uint16_t)(CMD_OLR_DS_STATSGET)      },
   {(DSL_uint16_t)(CMD_ADSL_FRAMEDATADS_LP0GET)},
   {(DSL_uint16_t)(CMD_BEARERCHSDS_GET)      },
   {(DSL_uint16_t)(CMD_DATAPATHFAILURESGET)  },
   {(DSL_uint16_t)(CMD_LINEFAILURENE_GET)    },
   { 0xFFFF }
};
#endif /* #ifdef DSL_DEBUG_DISABLE*/

#define CMD_FREEZE_TX   0x0443 /* missed in MCAT */
#define CMD_FREEZE_RX   0x0543 /* missed in MCAT */
static DSL_uint16_t g_VRxMsgWhitelist[] =
{
                                    /*           :             : FW           */
                                    /* CPE API   : CPE API     : supported    */
                                    /* supported : implemented : ADSL/VDSL    */
   CMD_MODEMFSM_STATEGET,           /*     +     :      +      :      +       */
   CMD_MODEMFSM_STATESET,           /*     +     :      +      :      +/-     */
   CMD_MODEMFSM_OPTIONSSET,         /*     +     :      +      :      -       */
   CMD_FRAMEDATAEXT2DS_GET,         /*     +     :      +      :      +       */
   CMD_FRAMEDATAEXT2US_GET,         /*     +     :      +      :      +       */
   CMD_BEARERCHSDS_GET,             /*     +     :      +      :      +       */
   CMD_BEARERCHSUS_GET,             /*     +     :      +      :      +       */
   CMD_VERSIONINFOGET,              /*     +     :      +      :      +       */
   CMD_LINESTATUSDS_GET,            /*     +     :      +      :      +/-     */
   CMD_LINESTATUSUS_GET,            /*     +     :      +      :      +/-     */
   CMD_XTSE_STATUSGET,              /*     +     :      +      :      +       */
   CMD_BAT_TABLEENTRIESGET,         /*     +     :      +      :      +       */
   CMD_BANDCONTROL_DS_GET,          /*     +     :      +      :      +       */
   CMD_BANDCONTROL_US_GET,          /*     +     :      +      :      +       */
   CMD_SNR_NE_TABLEENTRIESGET,      /*     +     :      +      :      +       */
   CMD_NOISEMARGINDELTASET,         /*     +     :      +      :      +       */
   CMD_OLR_CONTROL,                 /*     +     :      +      :      +/-     */
   CMD_HS_SELECTEDPROFILEVDSL2GET,  /*     +     :      +      :      +       */
   CMD_REINITNE_CONFIGURE,          /*     +     :      +      :      +/-     */
   CMD_VENDORID_O_GET,              /*     +     :      +      :      +       */
   CMD_SYSVENDORID_O_GET,           /*     +     :      +      :      +       */
   CMD_SYSVENDORVERSIONNUMO_GET,    /*     +     :      +      :      +       */
   CMD_SYSVENDORSERIALNUM_O_GET,    /*     +     :      +      :      +       */
   CMD_AUXINVENTORYINFO_O_GET,      /*     +     :      +      :      +       */
   CMD_OH_OPTIONSSET,               /*     -     :      +      :      -       */
   CMD_TESTPARAMSAUXDS_GET,         /*     +     :      +      :      +       */
   CMD_TESTPARAMSAUXUS_GET,         /*     +     :      +      :      +       */
   CMD_CRC_STATSNE_GET,             /*     +     :      +      :      +       */
   CMD_CRC_STATSFE_GET,             /*     +     :      +      :      +       */
   CMD_FEC_STATSNE_GET,             /*     +     :      +      :      +       */
   CMD_FEC_STATSFE_GET,             /*     +     :      +      :      +       */
   CMD_CRC_STATSNE_SET,             /*     +     :      +      :      +       */
   CMD_FEC_STATSNE_SET,             /*     +     :      +      :      +       */
   CMD_LINEPERFCOUNTNE_GET,         /*     +     :      +      :      +       */
   CMD_LINEPERFCOUNTFE_GET,         /*     +     :      +      :      +       */
   CMD_LINEPERFCOUNTNE_SET,         /*     +     :      +      :      +       */
   CMD_PTM_BC0_STATSNE_GET,         /*     +     :      +      :      +/-     */
   CMD_ATM_BC0_STATSNE_GET,         /*     +     :      +      :      +       */
   CMD_ATM_BC0_STATSNE_SET,         /*     +     :      +      :      +       */
   CMD_PTM_BC0_STATSNE_SET,         /*     +     :      +      :      +       */
   CMD_DATAPATHFAILURESGET,         /*     +     :      +      :      +       */
   CMD_SYSVENDORID_R_SET,           /*     +     :      +      :      +       */
   CMD_SYSVENDORVERSIONNUM_R_SET,   /*     +     :      +      :      +       */
   CMD_AUXINVENTORYINFO_R_SET,      /*     +     :      +      :      +       */
   CMD_SYSVENDORSERIALNUM_R_SET,    /*     +     :      +      :      +       */
   CMD_MODEMFSM_FAILREASONGET,      /*     +     :      +      :      +/-     */
   CMD_ADSL_BAT_US_GET,             /*     +     :      +      :      +       */
   CMD_ADSL_BAT_DS_GET,             /*     +     :      +      :      +       */
   CMD_GAINTABLEENTRIESGET,         /*     +     :      +      :      +       */
   CMD_ADSL_GAINTABLEUS_GET,        /*     +     :      +      :      +       */
   CMD_ADSL_GAINTABLEDS_GET,        /*     +     :      +      :      +       */
   CMD_SNR_DS_GET,                  /*     +     :      +      :      +       */
   CMD_SNR_US_GET,                  /*     +     :      +      :      +       */
   CMD_HLOGDS_GET,                  /*     +     :      +      :      +       */
   CMD_HLOGUS_GET,                  /*     +     :      +      :      +       */
   CMD_HLINDS_GET,                  /*     +     :      +      :      +       */
   CMD_HLINUS_GET,                  /*     +     :      +      :      +       */
   CMD_QLN_DS_GET,                  /*     +     :      +      :      +       */
   CMD_QLN_US_GET,                  /*     +     :      +      :      +       */
   CMD_HS_TONEGROUPSET,             /*     +     :      +      :      +       */
   CMD_HS_VDSL2PROFILECONTROL,      /*     +     :      +      :      +/-     */
   CMD_HS_STANDARDINFOFE_SPAR1GET,  /*     +     :      +      :      +       */
   CMD_LINEFAILUREFE_GET,           /*     +     :      +      :      +/-     */
   CMD_LINEFAILURENE_GET,           /*     +     :      +      :      +       */
   CMD_OLR_DS_STATSGET,             /*     +     :      +      :      +       */
   CMD_OLR_US_STATSGET,             /*     +     :      +      :      +       */
   CMD_ADSL_FRAMEDATAUS_LP0GET,     /*     +     :      +      :      +       */
   CMD_ADSL_FRAMEDATADS_LP0GET,     /*     +     :      +      :      +       */
   CMD_ADSL_FRAMEDATAUS_LP1GET,     /*     +     :      +      :      ?       */
   CMD_ADSL_FRAMEDATADS_LP1GET,     /*     +     :      +      :      ?       */
   CMD_LINESTATUSPERBANDUS_GET,     /*     +     :      +      :      +       */
   CMD_LINESTATUSPERBANDDS_GET,     /*     +     :      +      :      +       */
   CMD_PILOTTONESGET,               /*     +     :      +      :      +       */
   CMD_XTSE_CONFIGURE,              /*     +     :      +      :      +/-     */
   CMD_HYBRIDGET,                   /*     +     :      +      :      +       */
   CMD_BEARERCH0_DS_SET,            /*     +     :      +      :      +       */
   CMD_BEARERCH0_US_SET,            /*     +     :      +      :      +       */
   CMD_TC_STATUSGET,                /*     +     :      +      :      +       */
   CMD_BEARERCHSDS_RTX_GET,         /*     +     :      +      :      +       */
   CMD_RTX_DS_CONFIGURE,            /*     +     :      +      :      +       */
   CMD_RTX_STATUSGET,               /*     +     :      +      :      +       */
   CMD_RTX_DS_STATSGET,             /*     +     :      +      :      +       */
   CMD_RTX_PM_DS_GET,               /*     +     :      +      :      +       */
   CMD_PAF_HS_CONTROL,              /*     +     :      +      :      -/+     */
   CMD_PAF_HS_STATUSGET,            /*     +     :      +      :      -/+     */
   CMD_PAF_HS_CONTINUE,             /*     +     :      +      :      -/+     */
   CMD_OPERATIONOPTIONSSET,         /*     +     :      +      :      +       */
   CMD_MFD_RESULTSGET,              /*     +     :      +      :      +/-     */
   CMD_MFD_LOOPLENGTHGET,           /*     +     :      +      :      +/-     */
   CMD_MFD_HYBRIDINFOGET,           /*     +     :      +      :      +/-     */
   CMD_MISC_CONFIGSET,              /*     +     :      +      :      +       */
   CMD_MODEMFSM_OPTIONS2SET,        /*     +     :      +      :      +       */
   CMD_FREEZE_TX,/*missed in MCAT*/ /*     +     :      +      :      +/+     */
   CMD_FREEZE_RX,/*missed in MCAT*/ /*     +     :      +      :      +/+     */
   CMD_SHUTDOWNREQUEST,             /*     +     :      +      :      -/+     */
   CMD_ATM_BC0_TXSTATSNE_GET,       /*     +     :      +      :      +/+     */
   CMD_PBO_AELEM_STATUS_GET,        /*     +     :      +      :      -/+     */
   CMD_TESTPARAMSFE_REQUEST,        /*     +     :      +      :      -/+     */
      /* Delimeter only*/
   0xFFFF
};

#ifdef INCLUDE_DSL_G997_LINE_INVENTORY
static DSL_void_t DSL_DRV_VRX_Array16_to_8(
   DSL_uint16_t *pIn,
   DSL_uint8_t inSz,
   DSL_uint8_t *pOut,
   DSL_uint8_t outSz);

static DSL_Error_t DSL_DRV_VRX_FeXtseCapabilitiesAdslGet(
   DSL_Context_t *pContext);

static DSL_Error_t DSL_DRV_VRX_SendMsgAuxInventoryInfoOGet(
   DSL_Context_t *pContext,
   DSL_AuxLineInventoryData_t *pData);
#endif /* #ifdef INCLUDE_DSL_G997_LINE_INVENTORY*/

#if defined(INCLUDE_DSL_G997_PER_TONE) || defined(INCLUDE_DSL_CPE_MISC_LINE_STATUS)
/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_BitAllocationTableGet(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDirection,
   DSL_G997_NSCData8_t *pData,
   DSL_uint32_t *pBitsPerSymbol )
{
   DSL_Error_t    nErrCode = DSL_SUCCESS;
   DSL_uint32_t   nMsgId = 0;
   DSL_uint16_t   i = 0, nTonesLeft = 0, nTonesCurrent = 0, nTonesMax = 0,
                  nAckSize = 0, nAckIndex = 0, nAckLength = 0;
   DSL_LineStateValue_t   nCurrentState = DSL_LINESTATE_UNKNOWN;
   /* assume US/DS ADSL/VDSL to have the same CMD Structures */
   CMD_BAT_TableEntriesGet_t  sCmd;
   union
   {
      ACK_BAT_TableEntriesGet_t  Vdsl;
      ACK_ADSL_BAT_US_Get_t      AdslUs;
      ACK_ADSL_BAT_DS_Get_t      AdslDs;
   }sAck;
   DSL_BandList_t  sBandList;
   DSL_Band_t     *pBand = DSL_NULL;
   DSL_uint16_t   nBand = 0;
   DSL_uint8_t    nMaxNumOfEntries = 0;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_BitAllocationTableGet(nDirection=%d)"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nDirection));

   if(DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSL))
   {
      if (nDirection == DSL_UPSTREAM)
      {
         nMsgId = CMD_ADSL_BAT_US_GET;
         nMaxNumOfEntries = DSL_ARRAY_LENGTH(sAck.AdslUs.BAT);
         nAckSize = sizeof(sAck.AdslUs);
      }
      else
      {
         nMsgId = CMD_ADSL_BAT_DS_GET;
         nMaxNumOfEntries = DSL_ARRAY_LENGTH(sAck.AdslDs.BAT);
         nAckSize = sizeof(sAck.AdslDs);
      }
   }
   else
   {
      /* assume VDSL */
      nMsgId = CMD_BAT_TABLEENTRIESGET;
      nMaxNumOfEntries = DSL_ARRAY_LENGTH(sAck.Vdsl.BAT);
      nAckSize = sizeof(sAck.Vdsl);
   }

   /* the maximum number of tone data retreivable with one message */
   nTonesMax = 2 * nMaxNumOfEntries;

   /* Only proceed if the specified line is in SHOWTIME state.
      In other cases set number of used data elements to zero. */
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineState, nCurrentState);

   if ((nCurrentState == DSL_LINESTATE_SHOWTIME_TC_SYNC) ||
       (nCurrentState == DSL_LINESTATE_SHOWTIME_NO_SYNC))
   {
      /* init the return structure */
      if (pData != DSL_NULL)
      {
         memset (pData->nNSCData, 0, DSL_MAX_NSC*sizeof(pData->nNSCData[0]));
      }
      else
      {
         return DSL_ERROR;
      }

      memset(&sBandList, 0x0, sizeof(DSL_BandList_t));

      /* get number uf currently used bands */
      nErrCode = DSL_DRV_VRX_SendMsgBandControlGet(pContext, nDirection, &sBandList);
      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: ERROR -  getting current band config failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return nErrCode;
      }

      /* get number uf currently used bands */
      if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
      {
         pData->nNumData = 4096;
      }
      else
      {
         pData->nNumData = sBandList.nBand[0].nLastToneIndex + 1;
      }

      if (pBitsPerSymbol != DSL_NULL)
         *pBitsPerSymbol = 0;

      /* retrieve data only for the appropriate bands
         in sBandList */
      for (nBand = 0; (nBand < sBandList.nNumData) &&
                      (nBand < DSL_MAX_NUMBER_OF_BANDS); nBand++)
      {
         pBand = &sBandList.nBand[nBand];
         /*  several messages for max 128*2 values have to be sent */
         nTonesLeft = (pBand->nLastToneIndex - pBand->nFirstToneIndex) + 1;

         while (nTonesLeft)
         {
            nTonesCurrent = nTonesLeft > nTonesMax ? nTonesMax : nTonesLeft;

            /* the Msg structure allows retreival in pairs only */
            sCmd.Index = (pBand->nLastToneIndex - (nTonesLeft - 1)) / 2;
            /* round up, to avoid missing last tone */
            sCmd.Length = (nTonesCurrent + 1) / 2;
            nErrCode = DSL_DRV_VRX_SendMessage(pContext, nMsgId,
                           sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                           nAckSize, (DSL_uint8_t*)&sAck);

            nAckIndex  = sAck.Vdsl.Index;
            nAckLength = sAck.Vdsl.Length;

            /* Copy data only if successful and valid */
            if( (nErrCode == DSL_SUCCESS) &&
                (nAckIndex <= DSL_MAX_NSC/2 - nAckLength) )
            {
               /* Copy received data to structure */
               DSL_DEBUG(DSL_DBG_MSG,(pContext,
                  SYS_DBG_MSG"DSL[%02d]: Bit Allocation data received (idx %d - %d):"
                  DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nAckIndex, nAckIndex + nAckLength));

               if (nMaxNumOfEntries < nAckLength)
               {
                  DSL_DEBUG(DSL_DBG_WRN, (pContext,
                     SYS_DBG_WRN"DSL[%02d]: received BAT Elements truncated from %d to %d"
                     DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nAckLength, nMaxNumOfEntries));
               }

               /* The indexed words contain 2 values each */
               for(i = 0;
                  (i < nAckLength) && (i < nMaxNumOfEntries) &&
                  (((i + nAckIndex) * 2 + 1) < DSL_MAX_NSC); i++)
               {
                  /* VDSL and ADSL Ack structures differ
                     only in length of entries */
                  if (pData != DSL_NULL)
                  {
                     pData->nNSCData[(i + nAckIndex) * 2]     = sAck.Vdsl.BAT[i].data_00;

                     if ( ((i + nAckIndex) * 2 + 1) <= pBand->nLastToneIndex)
                     {
                        pData->nNSCData[(i + nAckIndex) * 2 + 1] = sAck.Vdsl.BAT[i].data_01;
                     }
                     else
                     {
                        pData->nNSCData[(i + nAckIndex) * 2 + 1] = 0x0;
                     }

                     DSL_DEBUG(DSL_DBG_MSG,(pContext,
                        SYS_DBG_MSG"%04X, ", pData->nNSCData[(i+nAckIndex)*2]));
                     DSL_DEBUG(DSL_DBG_MSG,(pContext,
                        SYS_DBG_MSG"%04X, ", pData->nNSCData[(i+nAckIndex)*2 + 1]));
                  }

                  if (pBitsPerSymbol != DSL_NULL)
                  {
                     *pBitsPerSymbol += sAck.Vdsl.BAT[i].data_00;
                     *pBitsPerSymbol += sAck.Vdsl.BAT[i].data_01;
                  }
               }
            }
            else
            {
               DSL_DEBUG(DSL_DBG_ERR, (pContext,
                  SYS_DBG_ERR""DSL_DRV_CRLF "DSL[%02d]: ERROR - Failed to retrieve BAT %d to %d!"
                  DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
                  sCmd.Index * 2, (sCmd.Index + sCmd.Length) * 2));

               nErrCode = DSL_ERR_FUNCTION_WAITING_TIMEOUT;
               return nErrCode;
            }

            nTonesLeft -= nTonesCurrent;
         }
      }
   }
   else
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - Function is only available if line is in showtime!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      nErrCode = DSL_ERR_ONLY_AVAILABLE_IN_SHOWTIME;
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_BitAllocationTableGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return (nErrCode);
}
#endif /* defined(INCLUDE_DSL_G997_PER_TONE) || defined(INCLUDE_DSL_CPE_MISC_LINE_STATUS)*/

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgXtseConfigure(
   DSL_Context_t *pContext,
   DSL_G997_XTUSystemEnablingData_t *pXTSE)
{
#define XTSE_CMD_SET(CmdBit, XtseByte, XtseBit) (sCmd.CmdBit = \
        (((pXTSE->XTSE[(XtseByte)-1] & (XtseBit)) != 0) ? \
         VRX_ENABLE : VRX_DISABLE))

   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_PortMode_t nPortMode;
   CMD_XTSE_Configure_t   sCmd;
   ACK_XTSE_Configure_t   sAck;

   memset(&sCmd, 0, sizeof(sCmd));
   memset(&sAck, 0, sizeof(sAck));

   sCmd.Length = 2;

   /* VDSL bits */
   if (((pXTSE->XTSE[8-1] & XTSE_8_01_A) != 0) ||
       ((pXTSE->XTSE[8-1] & XTSE_8_02_B) != 0) ||
       ((pXTSE->XTSE[8-1] & XTSE_8_03_C) != 0))
   {
      sCmd.V2 = VRX_ENABLE;
   }

   /* get port mode type*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->data.nPortMode, nPortMode);

   if (nPortMode != DSL_PORT_MODE_DUAL)
   {
      /*
         ADSL bits
      */
      /* ANSI T1.413 1998 */
      XTSE_CMD_SET(A0, 1, XTSE_1_01_A_T1_413);
      /* G.992.5, Annex M */
      XTSE_CMD_SET(A1, 7, XTSE_7_03_M_5_NO);
      /* G992.1 Annex A */
      XTSE_CMD_SET(A2, 1, XTSE_1_03_A_1_NO);
      /* G992.1 Annex B */
      XTSE_CMD_SET(A3, 1, XTSE_1_05_B_1_NO);
      /* G992.3 Annex I */
      XTSE_CMD_SET(A4, 4, XTSE_4_05_I_3_NO);
      /* G992.3 Annex J */
      XTSE_CMD_SET(A5, 4, XTSE_4_07_J_3_NO);
      /* G992.5 Annex I */
      XTSE_CMD_SET(A6, 6, XTSE_6_07_I_5_NO);
      /* G992.5 Annex J */
      XTSE_CMD_SET(A7, 7, XTSE_7_01_J_5_NO);
      /* G.992.3, Annex A */
      XTSE_CMD_SET(A8, 3, XTSE_3_03_A_3_NO);
      /* G.992.3, Annex B */
      XTSE_CMD_SET(A9, 3, XTSE_3_05_B_3_NO);
      /* G.992.3, Annex L US mask 1 */
      XTSE_CMD_SET(A11, 5, XTSE_5_03_L_3_NO);
      /* G.992.3, Annex L US mask 2 */
      XTSE_CMD_SET(A12, 5, XTSE_5_04_L_3_NO);
      /* G.992.3, Annex M */
      XTSE_CMD_SET(A13, 5, XTSE_5_07_M_3_NO);
      /* G.992.5, Annex B */
      XTSE_CMD_SET(A14, 6, XTSE_6_03_B_5_NO);
      /* G.992.5, Annex A */
      XTSE_CMD_SET(A15, 6, XTSE_6_01_A_5_NO);
   }

   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
   {
      if (sCmd.V2 == 0)
      {
         DSL_DEBUG( DSL_DBG_ERR, (pContext,
            SYS_DBG_ERR"DSL[%02d]: ERROR - No VDSL mode selected in the XTSE configuration!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         return DSL_ERROR;
      }
   }
   else if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSLA))
   {
      if ((sCmd.A0 == 0) && (sCmd.A1 == 0) && (sCmd.A2 == 0) && (sCmd.A8 == 0) &&
          (sCmd.A11 == 0) && (sCmd.A12 == 0) && (sCmd.A13 == 0) && (sCmd.A15 == 0) &&
          (sCmd.A4 == 0) && (sCmd.A6 == 0))
      {
         DSL_DEBUG( DSL_DBG_ERR, (pContext,
            SYS_DBG_ERR"DSL[%02d]: ERROR - No valid mode selected in the XTSE configuration!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         return DSL_ERROR;
      }
   }
   else if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSLB))
   {
      if ((sCmd.A0 == 0) && (sCmd.A3 == 0) && (sCmd.A9 == 0) && (sCmd.A14 == 0) &&
          (sCmd.A13 == 0) && (sCmd.A1 == 0) && (sCmd.A5 == 0) && (sCmd.A7 == 0))
      {
         DSL_DEBUG( DSL_DBG_ERR, (pContext,
            SYS_DBG_ERR"DSL[%02d]: ERROR - No valid mode selected in the XTSE configuration!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         return DSL_ERROR;
      }
   }
   else
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - Unknown FW type!" DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));

      return DSL_ERROR;
   }

   nErrCode = DSL_DRV_VRX_SendMessage( pContext, CMD_XTSE_CONFIGURE,
                 sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                 sizeof(sAck), (DSL_uint8_t*)&sAck);

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgBandControlGet(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDirection,
   DSL_BandList_t *pBandList)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   CMD_BandControl_DS_Get_t  sCmd;
   ACK_BandControl_DS_Get_t  sAck;
   DSL_uint32_t nMsgId = 0;
   DSL_uint16_t i;
   static const DSL_uint8_t nMaxNumOfBands =
                               sizeof(sAck.band)/sizeof(sAck.band[0]);
   DSL_uint8_t XTSE[DSL_G997_NUM_XTSE_OCTETS] = {0};

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_SendMsgBandControlGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   DSL_CHECK_CTX_POINTER(pContext);
   DSL_CHECK_ERR_CODE();

   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
   {
      nMsgId = nDirection == DSL_DOWNSTREAM ? CMD_BANDCONTROL_DS_GET :
                                              CMD_BANDCONTROL_US_GET;

      memset(&sCmd, 0, sizeof(sCmd));
      sCmd.Length = DSL_VRX_16BIT_RD_MSG_LEN_GET(sAck);

      nErrCode = DSL_DRV_VRX_SendMessage( pContext, nMsgId,
                     sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                     sizeof(sAck), (DSL_uint8_t*)&sAck );

      /* Copy data only if successful */
      if (nErrCode >= 0)
      {
         if (sAck.NumBandsDS <= DSL_MIN(DSL_MAX_NUMBER_OF_BANDS, nMaxNumOfBands))
         {
            pBandList->nNumData = sAck.NumBandsDS;
         }
         else
         {
            pBandList->nNumData = DSL_MIN(DSL_MAX_NUMBER_OF_BANDS,nMaxNumOfBands);

            DSL_DEBUG(DSL_DBG_WRN,(pContext,
               SYS_DBG_WRN"DSL[%02d]: Band List truncated from %d to %d!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
               sAck.NumBandsDS, pBandList->nNumData ));

            nErrCode = DSL_ERROR;
         }

         for (i = 0; i < pBandList->nNumData; i++)
         {
            pBandList->nBand[i].nFirstToneIndex = sAck.band[i].first;
            pBandList->nBand[i].nLastToneIndex  = sAck.band[i].last;
         }
      }
      else
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR""DSL_DRV_CRLF "DSL[%02d]: ERROR - Failed to retrieve Band Config!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      }
   }
   else
   {
      /*Get current xTSE octets*/
      DSL_CTX_READ(pContext, nErrCode, xtseCurr, XTSE);
      /* Only one band in ADSL mode*/
      pBandList->nNumData = 1;
      pBandList->nBand[0].nFirstToneIndex = 0;

      /*
      ADSL   Annex   US/DS
      1      A       0...32/0...256
      1      B       0...64/0...256
      2      A,I,L   0...32/0...256
      2      B,J,M   0...64/0...256
      2+     A,I,L   0...32/0...512
      2+     B,J,M   0...64/0...512
      */
      if (nDirection == DSL_DOWNSTREAM)
      {
         if ( (XTSE[6-1] & (XTSE_6_01_A_5_NO |  XTSE_6_03_B_5_NO | XTSE_6_07_I_5_NO)) ||
              (XTSE[7-1] & (XTSE_7_01_J_5_NO |  XTSE_7_03_M_5_NO)))
         {
            pBandList->nBand[0].nLastToneIndex = 511;
         }
         else if( (XTSE[1-1] & (XTSE_1_03_A_1_NO | XTSE_1_05_B_1_NO | XTSE_1_01_A_T1_413)) ||
                  (XTSE[2-1] & (XTSE_2_01_A_2_NO)) ||
                  (XTSE[3-1] & (XTSE_3_03_A_3_NO |  XTSE_3_05_B_3_NO)) ||
                  (XTSE[4-1] & (XTSE_4_05_I_3_NO |  XTSE_4_07_J_3_NO)) ||
                  (XTSE[5-1] & (XTSE_5_03_L_3_NO |  XTSE_5_04_L_3_NO | XTSE_5_07_M_3_NO)) )
         {
            pBandList->nBand[0].nLastToneIndex = 255;
         }
         else
         {
            pBandList->nBand[0].nLastToneIndex = 0;

            nErrCode = DSL_ERROR;

            DSL_DEBUG(DSL_DBG_ERR,
              (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - XTSE current settings not detected!"
              DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         }
      }
      else
      {
         if ( (XTSE[1-1] & (XTSE_1_05_B_1_NO)) ||
              (XTSE[2-1] & (XTSE_2_01_A_2_NO)) ||
              (XTSE[3-1] & (XTSE_3_05_B_3_NO)) ||
              (XTSE[4-1] & (XTSE_4_07_J_3_NO)) ||
              (XTSE[5-1] & (XTSE_5_07_M_3_NO)) ||
              (XTSE[6-1] & (XTSE_6_03_B_5_NO)) ||
              (XTSE[7-1] & (XTSE_7_01_J_5_NO | XTSE_7_03_M_5_NO)) )
         {
            pBandList->nBand[0].nLastToneIndex = 63;
         }
         else if( (XTSE[1-1] & (XTSE_1_03_A_1_NO | XTSE_1_01_A_T1_413)) ||
                  (XTSE[3-1] & (XTSE_3_03_A_3_NO)) ||
                  (XTSE[4-1] & (XTSE_4_05_I_3_NO )) ||
                  (XTSE[5-1] & (XTSE_5_03_L_3_NO |  XTSE_5_04_L_3_NO)) ||
                  (XTSE[6-1] & (XTSE_6_01_A_5_NO |  XTSE_6_07_I_5_NO)) )
         {
            pBandList->nBand[0].nLastToneIndex = 31;
         }
         else
         {
            pBandList->nBand[0].nLastToneIndex = 0;

            nErrCode = DSL_ERROR;

            DSL_DEBUG(DSL_DBG_ERR,
              (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - XTSE current settings not detected!"
              DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         }
      }
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_SendMsgBandControlGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgTxControlSet(
   DSL_Context_t *pContext,
   const DSL_uint16_t TxControl)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   CMD_TEST_TxControlSet_t sCmd;
   ACK_TEST_TxControlSet_t sAck;

   switch(TxControl)
   {
   case CMD_TEST_TxControlSet_SEND_QUIET:
   case CMD_TEST_TxControlSet_SEND_REVERB:
   case CMD_TEST_TxControlSet_SEND_SEGUE:
      sCmd.TxControl = TxControl;

      memset(&sCmd, 0, sizeof(sCmd));
      sCmd.Length = 1;

      nErrCode = DSL_DRV_VRX_SendMessage( pContext, CMD_TEST_TXCONTROLSET,
                    sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                    sizeof(sAck), (DSL_uint8_t*)&sAck );
      break;

   default:
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Unsupported TxControl option %d!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), TxControl));

      nErrCode = DSL_ERROR;
      break;
   }

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgSelectedProfileVdsl2Get(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   /* The message structures are identical for CO and CPE */
   ACK_HS_SelectedProfileVDSL2Get_t sAck;
   CMD_HS_SelectedProfileVDSL2Get_t sCmd;
   DSL_uint8_t i = 0, j = 0, nBitCount = 0;
   DSL_uint8_t actualProfile = 0;
   DSL_ProfileType_t nProfile = DSL_PROFILE_LAST;
   DSL_G997_XTUSystemEnablingData_t data = {{0,0,0,0,0,0,0,0}};

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_SendMsgSelectedProfileVdsl2Get"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   memset(&sCmd, 0, sizeof(sCmd));
   memset(&sAck, 0, sizeof(sAck));

   /* Read data from data base */
   for (i = 0; i < DSL_G997_NUM_XTSE_OCTETS; i++)
   {
      data.XTSE[i] = pContext->xtseCurr[i];
   }

   sCmd.Length = DSL_VRX_16BIT_RD_MSG_LEN_GET(sAck);
   nErrCode = DSL_DRV_VRX_SendMessage(pContext, CMD_HS_SELECTEDPROFILEVDSL2GET,
      sizeof(sCmd), (DSL_uint8_t*)&sCmd,
      sizeof(sAck), (DSL_uint8_t*)&sAck);

   if( nErrCode != DSL_SUCCESS)
   {
      return DSL_ERROR;
   }

   /* get the data */
   data.XTSE[8-1] &= ~(DSL_uint8_t)(XTSE_8_01_A|XTSE_8_02_B|XTSE_8_03_C);
   if (sAck.A_US0PsdSel21 ||
      sAck.A_US0PsdSel20 ||
      sAck.A_US0PsdSel17 ||
      sAck.A_US0PsdSel16 ||
      sAck.A_US0PsdSel15 ||
      sAck.A_US0PsdSel14 ||
      sAck.A_US0PsdSel13 ||
      sAck.A_US0PsdSel12 ||
      sAck.A_US0PsdSel11 ||
      sAck.A_US0PsdSel10 ||
      sAck.A_US0PsdSel9 ||
      sAck.A_US0PsdSel8 ||
      sAck.A_US0PsdSel7 ||
      sAck.A_US0PsdSel6 ||
      sAck.A_US0PsdSel5 ||
      sAck.A_US0PsdSel4 ||
      sAck.A_US0PsdSel3 ||
      sAck.A_US0PsdSel2 ||
      sAck.A_US0PsdSel1 ||
      sAck.A_US0PsdSel0)
      data.XTSE[8-1] |= (DSL_uint8_t)XTSE_8_01_A;
   if (sAck.B_US0PsdSel2 ||
      sAck.B_US0PsdSel1 ||
      sAck.B_US0PsdSel0)
      data.XTSE[8-1] |= (DSL_uint8_t)XTSE_8_02_B;
   if (sAck.C_US0PsdSel5 ||
      sAck.C_US0PsdSel4 ||
      sAck.C_US0PsdSel1 ||
      sAck.C_US0PsdSel0)
      data.XTSE[8-1] |= (DSL_uint8_t)XTSE_8_03_C;

   /* Check consistency of ATSE status setting - Only one bit should be set at
      a time */
   for (i = 0, nBitCount = 0; i < DSL_G997_NUM_XTSE_OCTETS; i++)
   {
      for (j = 0; j < 8; j++)
      {
         if ( ((data.XTSE[i] >> j) & 0x01) != 0)
         {
            nBitCount++;
         }
      }
   }

   if (nBitCount > 1)
   {
      DSL_DEBUG( DSL_DBG_WRN,
         (pContext,
         SYS_DBG_WRN"DSL[%02d]: Invalid XTSE status setting - more than one "
          "(%d) mode set within FW msg"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nBitCount));
   }

   if (nBitCount == 0)
   {
      /* According to DSLCPE_SW-720 Region A/B/C bits shall be set in this case */
      data.XTSE[8-1] |= (DSL_uint8_t)(XTSE_8_01_A | XTSE_8_02_B | XTSE_8_03_C);

      DSL_DEBUG( DSL_DBG_MSG, (pContext, SYS_DBG_WRN
          "DSL[%02d]: US0 not used, region information not available on CPE side. "
          "Setting all defined region bits A/B/C."
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }

   /* Get Actual Profile Information*/
   actualProfile = (sAck.profileSel7 << 7) | (sAck.profileSel6 << 6) |
                   (sAck.profileSel5 << 5) | (sAck.profileSel4 << 4) |
                   (sAck.profileSel3 << 3) | (sAck.profileSel2 << 2) |
                   (sAck.profileSel1 << 1) | (sAck.profileSel0);

   /* Decode Actual Profile*/
   switch(actualProfile)
   {
   case DSL_VRX_PROFILES_8A:
      nProfile = DSL_PROFILE_8A;
      break;
   case DSL_VRX_PROFILES_8B:
      nProfile = DSL_PROFILE_8B;
      break;
   case DSL_VRX_PROFILES_8C:
      nProfile = DSL_PROFILE_8C;
      break;
   case DSL_VRX_PROFILES_8D:
      nProfile = DSL_PROFILE_8D;
      break;
   case DSL_VRX_PROFILES_12A:
      nProfile = DSL_PROFILE_12A;
      break;
   case DSL_VRX_PROFILES_12B:
      nProfile = DSL_PROFILE_12B;
      break;
   case DSL_VRX_PROFILES_17A:
      nProfile = DSL_PROFILE_17A;
      break;
   case DSL_VRX_PROFILES_30A:
      nProfile = DSL_PROFILE_30A;
      break;
   default:
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Unknown profile (%d)"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), actualProfile));

      nErrCode = DSL_ERROR;
      break;
   }

   /* TODO: Get Actual Band Plan Information*/

   if (nErrCode == DSL_SUCCESS)
   {
      /* Set Profile information in the DSL CPE VRX device context*/
      DSL_CTX_WRITE_SCALAR(pContext, nErrCode,
         pDevCtx->data.deviceCfg.bandPlanStatusNe.nProfile, nProfile);

      DSL_CTX_WRITE(pContext, nErrCode, xtseCurr, data.XTSE);
   }

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_SendMsgSelectedProfileVdsl2Get"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgSpar1Get(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   CMD_HS_StandardInfoFE_SPAR1Get_t sCmd;
   ACK_HS_StandardInfoFE_SPAR1Get_t sAck;
   DSL_uint16_t *pAck = (DSL_uint16_t*)&sAck;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_SendMsgSpar1Get"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* fill up the message to be sent */
   memset(&sCmd, 0, sizeof(sCmd));
   memset(&sAck, 0, sizeof(sAck));
   sCmd.Length = DSL_VRX_16BIT_RD_MSG_LEN_GET(sAck);

   /* KAv: This delay is needed to get the correct Spar1 values after the exception state
      was detected*/
   DSL_DRV_MSecSleep(1000);

   nErrCode = DSL_DRV_VRX_SendMessage(
                 pContext, CMD_HS_STANDARDINFOFE_SPAR1GET,
                 sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                 sizeof(sAck), (DSL_uint8_t*)&sAck );

   if (nErrCode != 0)
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Retrieving SPAR1!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return(DSL_ERROR);
   }

   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.nSpar1, (*(pAck+2)) & 0xFF);
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.nSpar2, (*(pAck+2)) >> 8);
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.nSpar3, (*(pAck+3)) & 0xFF);
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.nSpar4, (*(pAck+3)) >> 8);
   DSL_CTX_WRITE_SCALAR(pContext, nErrCode, pDevCtx->data.nSpar5, (*(pAck+4)) & 0xFF);

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_SendMsgSpar1Get"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgModemFSMFailReasonGet(
   DSL_Context_t *pContext,
   DSL_uint8_t *pAck)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   CMD_ModemFSM_FailReasonGet_t sCmd;
   ACK_ModemFSM_FailReasonGet_t sAck;

   memset(&sCmd, 0, sizeof(sCmd));
   sCmd.Length = 10;

   nErrCode = DSL_DRV_VRX_SendMessage( pContext, CMD_MODEMFSM_FAILREASONGET,
                  sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                  sizeof(sAck), (DSL_uint8_t*)&sAck );

   if( nErrCode == DSL_SUCCESS )
   {
      memcpy(pAck, &sAck, sizeof(ACK_ModemFSM_FailReasonGet_t));
   }

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgXtseStatusGet(
   DSL_Context_t *pContext)
{
/* map the operation status bits to the ATSE octets */
#define VRX_OP_TO_XTSE(BIT, idx, mask) if(pAck->BIT) \
         data.XTSE[idx] |= mask;

   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_G997_XTUSystemEnablingData_t data;
   DSL_uint8_t i = 0, j = 0, nBitCount = 0;
   CMD_XTSE_StatusGet_t sCmd;
   ACK_XTSE_StatusGet_t sAck, *pAck = &sAck;

   memset(&data, 0, sizeof(DSL_G997_XTUSystemEnablingData_t));

   /* fill up the message to be sent */
   memset(&sCmd, 0, sizeof(sCmd));
   sCmd.Length = DSL_VRX_16BIT_RD_MSG_LEN_GET(sAck);

   nErrCode = DSL_DRV_VRX_SendMessage( pContext, CMD_XTSE_STATUSGET,
                  sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                  sizeof(sAck), (DSL_uint8_t*)&sAck );

   /* Copy data only if successful */
   if (nErrCode != 0)
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - Retrieving XTSE Status failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERROR;
   }

   /* read the Ack */
   data.XTSE[7] &= ~XTSE_8_08;

   /* ANSI T1.413 1998 */
   VRX_OP_TO_XTSE(A0, 0, XTSE_1_01_A_T1_413)
   /* G.992.5, Annex M */
   VRX_OP_TO_XTSE(A1, 6, XTSE_7_03_M_5_NO)
   /* G992.1 Annex A */
   VRX_OP_TO_XTSE(A2, 0, XTSE_1_03_A_1_NO)
   /* G992.1 Annex B */
   VRX_OP_TO_XTSE(A3, 0, XTSE_1_05_B_1_NO)
   /* G992.3 Annex I */
   VRX_OP_TO_XTSE(A4, 3, XTSE_4_05_I_3_NO);
   /* G992.3 Annex J */
   VRX_OP_TO_XTSE(A5, 3, XTSE_4_07_J_3_NO);
   /* G992.5 Annex I */
   VRX_OP_TO_XTSE(A6, 5, XTSE_6_07_I_5_NO);
   /* G992.5 Annex J */
   VRX_OP_TO_XTSE(A7, 6, XTSE_7_01_J_5_NO);
   /* G.992.3, Annex A */
   VRX_OP_TO_XTSE(A8, 2, XTSE_3_03_A_3_NO)
   /* G.992.3, Annex B */
   VRX_OP_TO_XTSE(A9, 2, XTSE_3_05_B_3_NO)

   /* G.992.3, Annex L, US Mask 1
      wide upstream */
   VRX_OP_TO_XTSE(A11, 4, XTSE_5_03_L_3_NO);
   /* G.992.3, Annex L, US Mask 2
      narrow upstream */
   VRX_OP_TO_XTSE(A12, 4, XTSE_5_04_L_3_NO);
   /* G.992.3, Annex M */
   VRX_OP_TO_XTSE(A13, 4, XTSE_5_07_M_3_NO);
   /* G.992.5, Annex B */
   VRX_OP_TO_XTSE(A14, 5, XTSE_6_03_B_5_NO);
   /* G.992.5, Annex A */
   VRX_OP_TO_XTSE(A15, 5, XTSE_6_01_A_5_NO);


   /* Check consistency of XTSE status setting - Only one bit should be set at
      a time */
   for (i = 0, nBitCount = 0; i < DSL_G997_NUM_XTSE_OCTETS; i++)
   {
      for (j = 0; j < 8; j++)
      {
         if ( ((data.XTSE[i] >> j) & 0x01) != 0)
         {
            nBitCount++;
         }
      }
   }

   if (nBitCount > 1)
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext,SYS_DBG_ERR"DSL[%02d]: Invalid XTSE status setting "
         "(0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X) - more than one (%d) mode set."
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), data.XTSE[0], data.XTSE[1], data.XTSE[2], data.XTSE[3],
         data.XTSE[4], data.XTSE[5], data.XTSE[6], data.XTSE[7], nBitCount));
   }

   /* Copy data to the DSL CPE internal memory*/
   memcpy(pContext->xtseCurr, data.XTSE, DSL_G997_NUM_XTSE_OCTETS);

   return (nErrCode);
}

#ifdef INCLUDE_DSL_G997_LINE_INVENTORY
/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgInventoryFeGet(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   union
   {
      ACK_VendorID_O_Get_t             G994VendorId;
      ACK_SysVendorID_O_Get_t          VendorId;
      ACK_SysVendorVersionNum_O_Get_t  VendorVersion;
      ACK_SysVendorSerialNum_O_Get_t   VendorSerial;
   } sAck;
   CMD_SysVendorID_O_Get_t sCmd;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_SendMsgInventoryFeGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   DSL_DRV_MUTEX_LOCK(pContext->dataMutex);

   /* do the G994 Vendor ID */
   memset(&sCmd, 0, sizeof(sCmd));
   memset(&sAck, 0, sizeof(sAck));

   sCmd.Length = DSL_VRX_16BIT_RD_MSG_LEN_GET(sAck.G994VendorId);
   nErrCode = DSL_DRV_VRX_SendMessage(pContext, CMD_VENDORID_O_GET,
      sizeof(sCmd), (DSL_uint8_t*)&sCmd,
      sizeof(sAck.G994VendorId), (DSL_uint8_t*)&sAck);

   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - G994 Vendor ID get failed, retCoe=%d!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));
   }
   else
   {
      /* get the data */
      DSL_DRV_VRX_Array16_to_8(
         sAck.G994VendorId.vendorID,
         DSL_ARRAY_LENGTH(sAck.G994VendorId.vendorID),
         pContext->lineInventoryFe.G994VendorID,
         DSL_ARRAY_LENGTH(pContext->lineInventoryFe.G994VendorID));
   }

   /* do the Vendor ID */
   memset(&sCmd, 0, sizeof(sCmd));
   memset(&sAck, 0, sizeof(sAck));

   sCmd.Length = DSL_VRX_16BIT_RD_MSG_LEN_GET(sAck.VendorId);
   nErrCode = DSL_DRV_VRX_SendMessage(pContext, CMD_SYSVENDORID_O_GET,
      sizeof(sCmd), (DSL_uint8_t*)&sCmd,
      sizeof(sAck.VendorId), (DSL_uint8_t*)&sAck);

   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Vendor ID get failed, retCode=%d!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));
   }
   else
   {
      /* get the data */
      DSL_DRV_VRX_Array16_to_8(
         sAck.VendorId.sysVendorID,
         DSL_ARRAY_LENGTH(sAck.VendorId.sysVendorID),
         pContext->lineInventoryFe.SystemVendorID,
         DSL_ARRAY_LENGTH(pContext->lineInventoryFe.SystemVendorID));
   }

   /* do the Vendor Version */
   memset(&sCmd, 0, sizeof(sCmd));
   memset(&sAck, 0, sizeof(sAck));

   sCmd.Length = DSL_VRX_16BIT_RD_MSG_LEN_GET(sAck.VendorVersion);

   nErrCode = DSL_DRV_VRX_SendMessage(
                 pContext, CMD_SYSVENDORVERSIONNUMO_GET,
                 sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                 sizeof(sAck.VendorVersion), (DSL_uint8_t*)&sAck);

   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Vendor Version get failed, retCode=%d!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));
   }
   else
   {
      /* get the data */
      DSL_DRV_VRX_Array16_to_8(
         sAck.VendorVersion.versionNum,
         DSL_ARRAY_LENGTH(sAck.VendorVersion.versionNum),
         pContext->lineInventoryFe.VersionNumber,
         DSL_ARRAY_LENGTH(pContext->lineInventoryFe.VersionNumber));
   }

   /* do the Vendor Serial Num */
   memset(&sCmd, 0, sizeof(sCmd));
   memset(&sAck, 0, sizeof(sAck));

   sCmd.Length = DSL_VRX_16BIT_RD_MSG_LEN_GET(sAck.VendorSerial);

   nErrCode = DSL_DRV_VRX_SendMessage(
                 pContext, CMD_SYSVENDORSERIALNUM_O_GET,
                 sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                 sizeof(sAck.VendorSerial), (DSL_uint8_t*)&sAck);

   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Vendor Serial get failed, retCode=%d!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));
   }
   else
   {
      /* get the data */
      DSL_DRV_VRX_Array16_to_8(
         sAck.VendorSerial.serialNum,
         DSL_ARRAY_LENGTH(sAck.VendorSerial.serialNum),
         pContext->lineInventoryFe.SerialNumber,
         DSL_ARRAY_LENGTH(pContext->lineInventoryFe.SerialNumber));
   }

   /* not for ADSL mode*/
   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
   {
      nErrCode = DSL_DRV_VRX_SendMsgStandardInfoFeVdsl2Get(
                    pContext,
                    pContext->lineInventoryFe.XTSECapabilities);
      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FE XTSE get failed, retCode=%d!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));
      }

      /* Get FE Aux Line information*/
      nErrCode = DSL_DRV_VRX_SendMsgAuxInventoryInfoOGet(
                    pContext,
                    &(pContext->auxInventoryFe));
      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FE Aux line inventory get failed, retCode=%d!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));
      }
   }

   DSL_DRV_MUTEX_UNLOCK(pContext->dataMutex);

   /* Update FE ADSL XTSE Capabilities*/
   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSL))
   {
      nErrCode = DSL_DRV_VRX_FeXtseCapabilitiesAdslGet(pContext);

      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FE XTSE get failed, retCode=%d!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));
      }
   }

   if( nErrCode != DSL_SUCCESS )
      nErrCode = DSL_ERROR;

   DSL_DEBUG(DSL_DBG_MSG,(pContext,
      SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_SendMsgInventoryFeGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return (nErrCode);
}
#endif /* INCLUDE_DSL_G997_LINE_INVENTORY*/

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgModemFsmStateGet(
   DSL_Context_t *pContext,
   DSL_uint8_t *pAck)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   CMD_ModemFSM_StateGet_t sCmd;
   ACK_ModemFSM_StateGet_t sAck;

   memset(&sCmd, 0, sizeof(sCmd));

   /* DSLCPE_SW-751: Temporary exclude L3 shutdown request from CO */
   /* Always set message length to one to avoid using inconsistent L3 mode */
   sCmd.Length = 0x1;

   nErrCode = DSL_DRV_VRX_SendMessage( pContext, CMD_MODEMFSM_STATEGET,
                  sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                  sizeof(sAck), (DSL_uint8_t*)&sAck );

   if( nErrCode == DSL_SUCCESS )
   {
      memcpy(pAck, &sAck, sizeof(ACK_ModemFSM_StateGet_t));
   }

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgModemFsmOptionsSet(
   DSL_Context_t *pContext,
   const DSL_boolean_t bAutoRestart,
   const DSL_boolean_t bDiagMode)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_G997_PMMode_t PMMode;
   CMD_ModemFSM_OptionsSet_t sCmd;
   ACK_ModemFSM_OptionsSet_t sAck;

   memset(&sCmd, 0, sizeof(sCmd));
   sCmd.Length = DSL_VRX_16BIT_RD_MSG_LEN_GET(sCmd);
   sCmd.E0 = (bAutoRestart == DSL_FALSE) ? VRX_DISABLE : VRX_ENABLE;
   sCmd.E2 = (bDiagMode    == DSL_FALSE) ? VRX_DISABLE : VRX_ENABLE;

   DSL_CTX_READ(pContext, nErrCode, PMMode, PMMode);
   sCmd.E8  = (PMMode == DSL_G997_PMMODE_BIT_L2_STATE)? VRX_ENABLE : VRX_DISABLE;
   sCmd.E9  = (PMMode == DSL_G997_PMMODE_BIT_L2_STATE)? VRX_ENABLE : VRX_DISABLE;
   sCmd.E10 = (PMMode == DSL_G997_PMMODE_BIT_L2_STATE)? VRX_ENABLE : VRX_DISABLE;

   nErrCode = DSL_DRV_VRX_SendMessage( pContext, CMD_MODEMFSM_OPTIONSSET,
                  sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                  sizeof(sAck), (DSL_uint8_t*)&sAck );

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgModemFsmOptions2Set(
   DSL_Context_t *pContext,
   DSL_boolean_t bEnableVirtualNoise)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   CMD_ModemFSM_Options2Set_t sCmd;
   ACK_ModemFSM_Options2Set_t sAck;
   DSL_LineFeatureData_t lineFeatureDataCfg;

   memset(&sCmd, 0, sizeof(sCmd));
   sCmd.Length = DSL_VRX_16BIT_RD_MSG_LEN_GET(sCmd);

   if (bEnableVirtualNoise)
   {
      /* Get Virtual Noise US Line Feature settings */
      DSL_CTX_READ(pContext, nErrCode, lineFeatureDataCfg[DSL_UPSTREAM],
            lineFeatureDataCfg);

      /* Set US Virtual Noise support */
      sCmd.enableVN_US = (lineFeatureDataCfg.bVirtualNoiseSupport) ?
                         VRX_ENABLE  : VRX_DISABLE;

      /* Get Virtual Noise US Line Feature settings */
      DSL_CTX_READ(pContext, nErrCode, lineFeatureDataCfg[DSL_DOWNSTREAM],
            lineFeatureDataCfg);

      /* Set US Virtual Noise support */
      sCmd.enableVN_DS = (lineFeatureDataCfg.bVirtualNoiseSupport) ?
                         VRX_ENABLE  : VRX_DISABLE;
   }

   /* Set AELEM */
   sCmd.enableAelem = (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2)) ?
                         VRX_ENABLE  : VRX_DISABLE;

   nErrCode = DSL_DRV_VRX_SendMessage( pContext, CMD_MODEMFSM_OPTIONS2SET,
                  sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                  sizeof(sAck), (DSL_uint8_t*)&sAck );

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgModemStateSet(
   DSL_Context_t *pContext,
   const DSL_uint16_t nMode)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   CMD_ModemFSM_StateSet_t sCmd;
   ACK_ModemFSM_StateSet_t sAck;

   DSL_CHECK_CTX_POINTER(pContext);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_SendMsgModemStateSet()"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   memset(&sCmd, 0, sizeof(sCmd));
   /* fill up the message to be sent */
   sCmd.Length  = 1;
   sCmd.LinkControl = nMode;

   nErrCode = DSL_DRV_VRX_SendMessage( pContext, CMD_MODEMFSM_STATESET,
                  sizeof(CMD_ModemFSM_StateSet_t), (DSL_uint8_t *)&sCmd,
                  sizeof(ACK_ModemFSM_StateSet_t), (DSL_uint8_t *)&sAck);

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_SendMsgModemStateSet()"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return (nErrCode);
}

#if defined(INCLUDE_DSL_CPE_API_VRX)
/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgOrderlyShutDownRequest(
   DSL_Context_t *pContext
)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   CMD_ShutdownRequest_t sCmd;
   ACK_ShutdownRequest_t sAck;

   DSL_CHECK_CTX_POINTER(pContext);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_SendMsgOrderlyShutDownRequest()"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   memset(&sCmd, 0, sizeof(sCmd));
   /* fill up the message to be sent */
   sCmd.Length  = 1;
   sCmd.ForcedShutdown = VRX_ENABLE;

   nErrCode = DSL_DRV_VRX_SendMessage( pContext, CMD_SHUTDOWNREQUEST,
                  sizeof(CMD_ShutdownRequest_t), (DSL_uint8_t *)&sCmd,
                  sizeof(ACK_ShutdownRequest_t), (DSL_uint8_t *)&sAck);

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_SendMsgOrderlyShutDownRequest()"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return (nErrCode);
}
#endif /* INCLUDE_DSL_CPE_API_VRX */

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgLinkFreezeTX(
   DSL_Context_t *pContext,
   const DSL_uint16_t nVal)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   /* Using unnamed stuctures because of using undefined at MCAT messages*/
   struct
   {
      /** Index */
      DSL_uint16_t Index;
      /** Length */
      DSL_uint16_t Length;
      /** Internal value */
      DSL_uint16_t Value;
   } __PACKED__ sCmd;

   struct
   {
      /** Index */
      DSL_uint16_t Index;
      /** Length */
      DSL_uint16_t Length;
   } __PACKED__ sAck;

   DSL_CHECK_CTX_POINTER(pContext);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_SendMsgLinkFreezeTX()"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   memset(&sCmd, 0, sizeof(sCmd));
   /* fill up the message to be sent */
   sCmd.Length  = 1;
   sCmd.Value = nVal;

   nErrCode = DSL_DRV_VRX_SendMessage( pContext, CMD_FREEZE_TX,
                  sizeof(sCmd), (DSL_uint8_t *)&sCmd,
                  sizeof(sAck), (DSL_uint8_t *)&sAck);

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_SendMsgLinkFreezeTX()"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgLinkFreezeRX(
   DSL_Context_t *pContext,
   const DSL_uint16_t nVal)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;

   /* Using unnamed stuctures because of using undefined at MCAT messages*/
   struct
   {
      /** Index */
      DSL_uint16_t Index;
      /** Length */
      DSL_uint16_t Length;
      /** Internal value */
      DSL_uint16_t Value;
   } __PACKED__ sCmd;

   struct
   {
      /** Index */
      DSL_uint16_t Index;
      /** Length */
      DSL_uint16_t Length;
   } __PACKED__ sAck;

   DSL_CHECK_CTX_POINTER(pContext);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_SendMsgLinkFreezeRX()"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   memset(&sCmd, 0, sizeof(sCmd));
   /* fill up the message to be sent */
   sCmd.Length  = 1;
   sCmd.Value = nVal;

   nErrCode = DSL_DRV_VRX_SendMessage( pContext, CMD_FREEZE_RX,
                  sizeof(sCmd), (DSL_uint8_t *)&sCmd,
                  sizeof(sAck), (DSL_uint8_t *)&sAck);

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_SendMsgLinkFreezeRX()"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
static DSL_Error_t DSL_DRV_VRX_GetMsgDataRateValues(
   const DSL_uint32_t nDataRate,
   const DSL_uint32_t nMode,
   DSL_uint16_t *nRate32,
   DSL_uint8_t *nRate4_8 )
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint32_t fVal = 0;

   *nRate32 = (DSL_uint16_t)(nDataRate / 32000);

   fVal = ((DSL_uint32_t)nDataRate) - *nRate32 * 32000;

   /* Special rate adder are not available for G.992.1/2 */
   if ( nMode <=1 )
   {
      if (nMode == 0)
      {
         /* For G.992.3/4 multiple of 4kbit/s are possible */
         fVal /= 4000;
      }
      else
      {
         /* For G.992.5 multiple of 8kbit/s are possible */
         fVal /= 8000;
      }
      /* Round mathematically to next integer value */
      *nRate4_8 = (DSL_uint8_t)(fVal);
   }

   return (nErrCode);
}

#ifdef INCLUDE_DSL_G997_LINE_INVENTORY
static DSL_void_t DSL_DRV_VRX_Array16_to_8(
   DSL_uint16_t *pIn,
   DSL_uint8_t inSz,
   DSL_uint8_t *pOut,
   DSL_uint8_t outSz)
{
   DSL_uint8_t i = 0;

   for( i = 0; i < inSz; i++ )
   {
      if( (DSL_uint32_t)(i*2 + 1) < outSz )
      {
         pOut[i*2 + 1] = (DSL_uint8_t)(pIn[i] >> 8);
         pOut[i*2]     = (DSL_uint8_t)(pIn[i]);
      }
   }
}

/*
   Reads the Far End xTSE capabilities from the device and stores it
   into the internal structure.

   \param pContext   Pointer to dsl library context structure, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
static DSL_Error_t DSL_DRV_VRX_FeXtseCapabilitiesAdslGet(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint8_t xtse[DSL_G997_NUM_XTSE_OCTETS] = {0};
   DSL_uint8_t nSpar1 = 0, nSpar3 = 0,
               nSpar4 = 0, nSpar5 = 0;

   /* Request the handshake information about the far-end operating mode*/
   nErrCode = DSL_DRV_VRX_SendMsgSpar1Get(pContext);
   if( nErrCode!= DSL_SUCCESS )
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - SPAR1 get failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   /* Get SPAR info from the Device Context*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->data.nSpar1, nSpar1);
   DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->data.nSpar3, nSpar3);
   DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->data.nSpar4, nSpar4);
   DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->data.nSpar5, nSpar5);

   /*
      Map SPAR1 octet to the XTSE capabilities\
   */
   /* G992.1 Annex A*/
   xtse[1-1] |= (nSpar1 & 0x1) ? XTSE_1_03_A_1_NO : 0x0;
   /* G992.1 Annex B*/
   xtse[1-1] |= (nSpar1 & 0x2) ? XTSE_1_05_B_1_NO : 0x0;
   /* G992.2 Annex A*/
   xtse[2-1] |= (nSpar1 & 0x8) ? XTSE_2_01_A_2_NO : 0x0;

   /*
      Map SPAR2 octet to the XTSE capabilities
   */
   /* No ADSL info*/

   /*
      Map SPAR3 octet to the XTSE capabilities
   */
   /* G992.3 Annex A*/
   xtse[3-1] |= (nSpar3 & 0x1) ? XTSE_3_03_A_3_NO : 0x0;
   /* G992.3 Annex L*/
   xtse[5-1] |= (nSpar3 & 0x1) ? (XTSE_5_03_L_3_NO | XTSE_5_04_L_3_NO) : 0x0;
   /* G992.3 Annex B*/
   xtse[3-1] |= (nSpar3 & 0x2) ? XTSE_3_05_B_3_NO : 0x0;
   /* G992.3 Annex I*/
   xtse[4-1] |= (nSpar3 & 0x4) ? XTSE_4_05_I_3_NO : 0x0;
   /* G992.3 Annex J*/
   xtse[4-1] |= (nSpar3 & 0x8) ? XTSE_4_07_J_3_NO : 0x0;
   /* G992.4 Annex A*/
   xtse[4-1] |= (nSpar3 & 0x10) ? XTSE_4_01_A_4_NO : 0x0;

   /*
      Map SPAR4 octet to the XTSE capabilities
   */
   /* G992.5 Annex A*/
   xtse[6-1] |= (nSpar4 & 0x1) ? XTSE_6_01_A_5_NO : 0x0;
   /* G992.5 Annex B*/
   xtse[6-1] |= (nSpar4 & 0x2) ? XTSE_6_03_B_5_NO : 0x0;
   /* G992.5 Annex I*/
   xtse[6-1] |= (nSpar4 & 0x4) ? XTSE_6_07_I_5_NO : 0x0;
   /* G992.3 Annex M*/
   xtse[5-1] |= (nSpar4 & 0x8) ? XTSE_5_07_M_3_NO : 0x0;
   /* G992.5 Annex J*/
   xtse[7-1] |= (nSpar4 & 0x10) ? XTSE_7_01_J_5_NO : 0x0;

   /*
      Map SPAR5 octet to the XTSE capabilities
   */
   /* G992.5 Annex M*/
   xtse[7-1] |= (nSpar5 & 0x1) ? XTSE_7_03_M_5_NO : 0x0;

   /* Set FE XTSE capabilities in the DSL CPE Context*/
   DSL_CTX_WRITE(pContext, nErrCode, lineInventoryFe.XTSECapabilities, xtse);

   return nErrCode;
}
#endif /* INCLUDE_DSL_G997_LINE_INVENTORY*/

#ifndef DSL_DEBUG_DISABLE
/**
   Checks if a message is on a defined blacklist.

   \param pContext Pointer to dsl library context structure, [I]
   \param nMsgId   Message Id of the VRX messages to check, [I]
*/
static DSL_boolean_t DSL_DRV_VRX_CheckMessageDumpBlacklist(
   DSL_Context_t *pContext,
   const DSL_uint16_t nMsgId)
{
   DSL_boolean_t nRet = DSL_FALSE;
   VRX_MsgId_t *pTable = &DSL_DRV_VRX_g_MsgDumpBlacklist[0];

   for ( ; pTable->nMsgId != 0xFFFF; pTable++)
   {
      if ( nMsgId == pTable->nMsgId )
      {
         nRet = DSL_TRUE;
         break;
      }
   }

   return nRet;
}

static DSL_void_t DSL_DRV_VRX_DumpMessage(
   DSL_Context_t *pContext,
   const DSL_uint16_t nMsgId,
   const DSL_uint16_t *pData,
   const DSL_uint16_t nSize,
   DSL_boolean_t bReceive)
{
   static const DSL_uint16_t *pMsg16;
   static const DSL_uint32_t *pMsg32;
   DSL_DBG_ModuleLevel_t dbgmlData;
   DSL_boolean_t bPrint = DSL_FALSE, bDirSet = DSL_FALSE;
   DSL_uint8_t i;

   dbgmlData.data.nDbgLevel  = DSL_DBG_NONE;
   dbgmlData.data.nDbgModule = DSL_DBG_MESSAGE_DUMP;

   if (DSL_DRV_DBG_ModuleLevelGet(pContext,&dbgmlData) != DSL_SUCCESS)
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: Debug module level get failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return;
   }

   if((pData == DSL_NULL) || (nSize < 4))
   {
      return ;
   }

   pMsg16 = (DSL_uint16_t*)(pData+2);
   pMsg32 = (DSL_uint32_t*)(pData+2);

   if (dbgmlData.data.nDbgLevel < DSL_DBG_ERR)
   {
      return ;
   }

   bDirSet = (nMsgId & 0x40) ? DSL_TRUE : DSL_FALSE;

   switch (dbgmlData.data.nDbgLevel)
   {
   case DSL_DBG_ERR:
      if ((DSL_DRV_VRX_CheckMessageDumpBlacklist(pContext, nMsgId) == DSL_FALSE) &&
          (bDirSet != bReceive))
      {
         bPrint = DSL_TRUE;
      }
      break;

   case DSL_DBG_WRN:
      if (DSL_DRV_VRX_CheckMessageDumpBlacklist(pContext, nMsgId) == DSL_FALSE)
      {
         bPrint = DSL_TRUE;
      }
      break;

   case DSL_DBG_MSG:
      if (bDirSet != bReceive)
      {
         bPrint = DSL_TRUE;
      }
      break;

   case DSL_DBG_LOCAL:
      bPrint = DSL_TRUE;
      break;

   default:
      bPrint = DSL_FALSE;
   }

   if (bPrint == DSL_TRUE)
   {
      DSL_DRV_debug_printf(pContext, "DSL[%02d/%s]: 0x%04x 0x%04x 0x%04x",
                           DSL_DEV_NUM(pContext),
                           bReceive == DSL_TRUE ? "rx" : "tx", nMsgId,
                           pData[0], pData[1]);

      /* decide wether to interpret the rest as 16 or 32 bit */
      if (nMsgId & VDSL2_MBX_MSG_ID_IFX_MASK)
      {
         /* IFX message: 32 bit */
         for (i=0; i<((nSize-4)/4); i++)
         {
            DSL_DRV_debug_printf(pContext, " %08X", pMsg32[i]);
         }
      }
      else
      {
         /* no IFX message: 16 bit */
         for (i=0; i<((nSize-4)/2); i++)
         {
            DSL_DRV_debug_printf(pContext, " %04X", pMsg16[i]);
         }
      }

      DSL_DRV_debug_printf(pContext, DSL_DRV_CRLF);
   }
}
#endif /* #ifndef DSL_DEBUG_DISABLE*/

DSL_Error_t DSL_DRV_VRX_ChReadMessage(
   DSL_Context_t *pContext,
   IOCTL_MEI_message_t *pMsg)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_DEV_Handle_t dev;
   DSL_uint32_t buf_size;

   DSL_CHECK_POINTER(pContext, pContext->pDevCtx->nfc_lowHandle);
   DSL_CHECK_ERR_CODE();

   dev = pContext->pDevCtx->nfc_lowHandle;

   buf_size = pMsg->paylSize_byte;

   if( DSL_DRV_VRX_InternalNfcMsgRead( (MEI_DYN_CNTRL_T*)dev, pMsg) >= 0 )
   {
      if ((pMsg->paylSize_byte > 0) && (buf_size != pMsg->paylSize_byte))
         nErrCode = DSL_SUCCESS;
      else
         nErrCode = DSL_ERROR;
   }
   else
      nErrCode = DSL_ERROR;

   return (nErrCode);
}

DSL_Error_t DSL_DRV_VRX_ChRequestMessage(
   DSL_Context_t *pContext,
   IOCTL_MEI_messageSend_t *pRw)
{
   DSL_DEV_Handle_t dev;
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_int_t ret;

   DSL_CHECK_CTX_POINTER(pContext);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_POINTER(pContext, pContext->pDevCtx);
   DSL_CHECK_ERR_CODE();

   DSL_CHECK_POINTER(pContext, pContext->pDevCtx->lowHandle);
   DSL_CHECK_ERR_CODE();

#ifndef DSL_DEBUG_DISABLE
   DSL_DRV_VRX_DumpMessage(pContext, pRw->write_msg.msgId,
      (DSL_uint16_t *)pRw->write_msg.pPayload,
      (DSL_uint16_t)pRw->write_msg.paylSize_byte, DSL_FALSE);
#endif /* #ifndef DSL_DEBUG_DISABLE*/

   /* reset for later interpretation */
   pRw->ack_msg.msgClassifier = 0xFFFF;

   /* Get device handle*/
   dev = DSL_DEVICE_LOWHANDLE(pContext);

   ret =  DSL_DRV_VRX_InternalMsgSend((MEI_DYN_CNTRL_T*)dev, pRw);

   if (ret < 0)
   {
      DSL_DEBUG(DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: Error %d during %s (MsgID=0x%02X), FctCode 0x%02X!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), pRw->ictl.retCode,
         "FIO_MEI_MBOX_MSG_SEND",
         pRw->write_msg.msgId, pRw->ack_msg.msgClassifier));
      nErrCode = DSL_ERROR;
   }

#ifndef DSL_DEBUG_DISABLE
   /* if ret was ok, pRw contains the proper answer already */
   DSL_DRV_VRX_DumpMessage(pContext, pRw->ack_msg.msgId,
      (DSL_uint16_t *)pRw->ack_msg.pPayload,
      (DSL_uint16_t)pRw->ack_msg.paylSize_byte, DSL_TRUE);
#endif /* #ifndef DSL_DEBUG_DISABLE*/

   return (nErrCode);
}


/*
   This function exchanges a command/acknowledge couple with the driver.
   The return code is checked and several retries might be done, or the
   line might be locked.

   \param pContext Pointer to DSL CPE library context structure, [I]
   \param pMsg     pointer to message exchange data, [I/O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed or message ID of acknowledge wrong
*/
static DSL_Error_t DSL_DRV_VRX_MsgTransmit(
   DSL_Context_t *pContext,
   IOCTL_MEI_messageSend_t *pMsg)
{
   DSL_Error_t nRet = DSL_SUCCESS;
   DSL_boolean_t bBlockLine = DSL_FALSE;
   DSL_uint16_t i;
   DSL_uint32_t nLength, nLenAck;
   DSL_uint8_t nMsgFctOpCode = 0;

   nLength = pMsg->write_msg.paylSize_byte;
   nLenAck = pMsg->ack_msg.paylSize_byte;

   for (i=0; i<3; i++)
   {
      /* for any retry set the CMD and ACK Buffer length again,
         because the driver is changing these fields to indicate
         the bytes processed */
      pMsg->write_msg.paylSize_byte  = nLength;
      pMsg->ack_msg.paylSize_byte    = nLenAck;
      nRet = DSL_DRV_VRX_ChRequestMessage(pContext, pMsg);
      /* Send and wait for recieved data */
      if(nRet == DSL_SUCCESS)
      {
         break;
      }
      else
      {
         nMsgFctOpCode = (pMsg->ack_msg.msgClassifier & 0xFF00) >> 8;
         /* interprete return code */
         switch(pMsg->ictl.retCode)
         {
            case -e_MEI_ERR_INVAL_STATE:
            case -e_MEI_ERR_DEV_NO_RESP:
               /* line not available - stop sending, block this line */
               DSL_DEBUG(DSL_DBG_ERR,(pContext,
                  SYS_DBG_ERR"DSL[%02d]: Config Error or line not available - block!"
                  DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

               bBlockLine = DSL_TRUE;
               nRet = DSL_ERR_MSG_EXCHANGE;
               i = 3;
               break;

            case -e_MEI_ERR_DEV_NEG_RESP:
            case -e_MEI_ERR_DEV_INVAL_RESP:
            case -e_MEI_ERR_DEV_BUSY:
               /* negative response - retry sending, keep the line alive */
               DSL_DEBUG(DSL_DBG_WRN,
                  (pContext, SYS_DBG_WRN"DSL[%02d]: negative response for "
                     "MsgID=0x%04X (Class=0x%08X) - on try %d!" DSL_DRV_CRLF,
                     DSL_DEV_NUM(pContext),
                     pMsg->write_msg.msgId, pMsg->ack_msg.msgClassifier, i));
               if ( (nMsgFctOpCode == D2H_ERROR_CMD_NOT_ALLOWED)
#if (PRELIMINARY_ADSL_SUPPORT == 1)
                    || (nMsgFctOpCode == 0x31) ||
                       (nMsgFctOpCode == 0x61) ||
                       (nMsgFctOpCode == 0x71)
#endif
                    )
               {
                  nRet = DSL_WRN_FIRMWARE_MSG_DENIED;
                  i = 3;
               }
               else if(nMsgFctOpCode == D2H_CMV_CURRENTLY_UNAVAILABLE)
               {
                  /* currently unavailable, try again */
               }
               else
               {
                  nRet = DSL_ERR_MSG_EXCHANGE;
                  i = 3;
               }
               break;

            case e_MEI_ERR_OP_FAILED:
               /* MBX busy, try again */
               DSL_DEBUG(DSL_DBG_WRN,
                     (pContext, SYS_DBG_WRN"DSL[%02d]: MBX busy, "
                     "MsgID=0x%04X (Class=0x%08X) - on try %d!" DSL_DRV_CRLF,
                     DSL_DEV_NUM(pContext),
                     pMsg->write_msg.msgId, pMsg->ack_msg.msgClassifier, i));
               break;

            default:
#if (PRELIMINARY_ADSL_SUPPORT == 1)
               /* special ADSL FW return code for unsupported messages */
               if ((nMsgFctOpCode == 0x31) ||
                   (nMsgFctOpCode == 0x61) ||
                   (nMsgFctOpCode == 0x71))
               {
                  DSL_DEBUG(DSL_DBG_WRN,
                     (pContext, SYS_DBG_WRN"DSL[%02d]: message not yet supported, "
                     "MsgID=0x%04X (Class=0x%08X) - on try %d!" DSL_DRV_CRLF,
                     DSL_DEV_NUM(pContext), pMsg->write_msg.msgId,
                     pMsg->ack_msg.msgClassifier, i));
                  nRet = DSL_WRN_FIRMWARE_MSG_DENIED;
                  i = 3;
                  break;
               }
               else
#endif /* (PRELIMINARY_ADSL_SUPPORT == 1) */
               /* try it again */
               if ( (pMsg->write_msg.paylSize_byte != nLength) &&
                    (pMsg->ack_msg.msgClassifier != 0xFFFF) )
               {
                  /* Write Error! */
                  DSL_DEBUG(DSL_DBG_ERR,
                     (pContext, SYS_DBG_ERR"DSL[%02d]: error[%d] WRITE VRX Cmd"
                     "(MsgID=0x%04X, Class=0x%08X) on try %d!" DSL_DRV_CRLF,
                     DSL_DEV_NUM(pContext),
                     nRet, pMsg->write_msg.msgId,
                     pMsg->write_msg.msgClassifier, i));
               }
               else
#if (PRELIMINARY_ADSL_SUPPORT == 1)
               {
                  DSL_DEBUG(DSL_DBG_WRN,
                     (pContext, SYS_DBG_WRN"DSL[%02d]: message not yet supported, "
                     "MsgID=0x%04X (Class=0x%08X) - on try %d!" DSL_DRV_CRLF,
                     DSL_DEV_NUM(pContext),
                     pMsg->write_msg.msgId, pMsg->ack_msg.msgClassifier, i));
                  nRet = DSL_WRN_FIRMWARE_MSG_DENIED;
                  i = 3;
                  break;
               }
#else
               {
                  /* Ack Error/Timeout! */
                  DSL_DEBUG(DSL_DBG_WRN,
                     (pContext, SYS_DBG_WRN"DSL[%02d]: error[%d] during waiting for VRX Ack "
                     "(MsgID=0x%04X, Class=0x%08X) on try %d!" DSL_DRV_CRLF,
                     DSL_DEV_NUM(pContext),
                     nRet, pMsg->write_msg.msgId,
                     pMsg->ack_msg.msgClassifier, i));

                  if (pMsg->ack_msg.msgClassifier != 0xFFFF)
                  {
                     /* an ACK has been received - stop sending, keep line */
                     i = 3;
                     break;
                  }
               }
#endif /* (PRELIMINARY_ADSL_SUPPORT == 1) */

               if (i <= 2)
                  bBlockLine = DSL_TRUE;

               nRet = DSL_ERR_FUNCTION_WAITING_TIMEOUT;
               break;
         }     /* switch(nRet) {...} */

         DSL_DRV_MSecSleep(700);
      }
   }

   if(nRet != DSL_SUCCESS)
   {
      /* Error - Line not available */
      DSL_DEBUG((bBlockLine ? DSL_DBG_ERR : DSL_DBG_WRN),
         (pContext, SYS_DBG_ERR"DSL[%02d]: Error for send CMD MsgID=0x%02X - %s line!" DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext),
         pMsg->write_msg.msgId, bBlockLine ? "BLOCK" : "KEEP"));
   }

   return nRet;
}

/*
   This function triggers to send a specified message which requests data from
   the local device and waits for appropriate answer.

   \param pContext Pointer to dsl cpe drive context structure, [I]
   \param nMsgID   Specifies the message ID as defined in the VRX firmware
                   message specification. It includes the message type and
                   subtype, [I]
   \param nLength  number of bytes of the message payload, [I]
   \param nData    pointer to the message payload data, [I]
   \param nLenAck  available buffer size for received ack, [I]
   \param pDataAck pointer to buffer for receiving ack message, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed or message ID of acknowledge wrong
*/
DSL_Error_t DSL_DRV_VRX_SendMessage(
   DSL_Context_t *pContext,
   const DSL_uint32_t nMsgID,
   const DSL_uint16_t nLength,
   const DSL_uint8_t *pData,
   const DSL_uint16_t nLenAck,
   DSL_uint8_t *pDataAck)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   IOCTL_MEI_messageSend_t sMsg;
   DSL_uint32_t i = 0;

   /* Handle VRX message white list*/
   while (g_VRxMsgWhitelist[i] != 0xFFFF)
   {
      if (nMsgID == g_VRxMsgWhitelist[i])
         break;
      i++;
   }
   if (g_VRxMsgWhitelist[i] == 0xFFFF)
   {
      DSL_DEBUG(DSL_DBG_WRN,
         (pContext, SYS_DBG_WRN"DSL[%02d]: Unsupported message with MsgID=0x%04X!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nMsgID));
      return nErrCode;
   }
   /* VRx message white list handling END*/

   if ((nLenAck == 0) || (pDataAck == DSL_NULL))
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: invalid buffer for VRX Ack!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      nErrCode = DSL_ERR_POINTER;

      pContext->nErrNo = nErrCode;

      return nErrCode;
   }

   if (DSL_DRV_MUTEX_LOCK(pContext->bspMutex))
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Getting mei driver semaphore failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERROR;
   }

   sMsg.write_msg.msgId          = (DSL_uint16_t)(nMsgID & 0xFFFF);
   sMsg.write_msg.pPayload       = (unsigned char *)pData;
   sMsg.write_msg.paylSize_byte  = nLength;
   /* to receive the answer hand over the payload buffer */
   sMsg.ack_msg.pPayload         = (unsigned char *)pDataAck;

   sMsg.ack_msg.msgId = 0;
   sMsg.write_msg.paylSize_byte  = nLength;
   sMsg.ack_msg.paylSize_byte    = nLenAck;

   nErrCode = DSL_DRV_VRX_MsgTransmit(pContext, &sMsg);

   /* check if received ID was the same as the sent one */
   if ( ((DSL_uint16_t)(nMsgID & 0xFFFF)) != sMsg.ack_msg.msgId && nErrCode > DSL_ERROR)
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: Ack mismatch in MsgID (0x%04X vs 0x%04X expected)!"
         DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext), sMsg.ack_msg.msgId, nMsgID));

      nErrCode = DSL_ERR_FUNCTION_WAITING;

      pContext->nErrNo = nErrCode;
   }

   DSL_DRV_MUTEX_UNLOCK(pContext->bspMutex);

   return nErrCode;
}

#if defined(INCLUDE_DSL_G997_PER_TONE) || defined(INCLUDE_DSL_DELT)
/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgSnrGet(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDirection,
   DSL_G997_NSCData8_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint16_t i = 0;
   DSL_uint16_t nTonesLeft, nTonesCurrent, nTonesDone, nDataNum = 512;
   /* assume US/DS to have the same Msg Structures */
   CMD_SNR_DS_Get_t  sCmd;
   ACK_SNR_DS_Get_t  sAck;
   static const DSL_uint8_t nMaxNumOfEntries = DSL_ARRAY_LENGTH(sAck.SNRpsds);
   static const DSL_uint16_t nTonesMax = 256;
   DSL_BandList_t  sBandList;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_DeltSnrGet(nDirection=%d)"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nDirection));

   /* init the return structure */
   memset(pData->nNSCData, 0, nDataNum * sizeof(pData->nNSCData[0]));
   pData->nNumData = 0;

   /* Check current xDSL mode*/
   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
   {
      nDataNum = 512;
   }
   else
   {
      memset(&sBandList, 0x0, sizeof(DSL_BandList_t));

      /* get number uf currently used bands */
      nErrCode = DSL_DRV_VRX_SendMsgBandControlGet(pContext, nDirection, &sBandList);
      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: ERROR -  getting current band config failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return nErrCode;
      }

      nDataNum = sBandList.nBand[0].nLastToneIndex + 1;
   }

   /*  several messages for max 128*2 values have to be sent */
   nTonesLeft = nDataNum;
   nTonesDone = 0;

   while (nTonesLeft)
   {
      nTonesCurrent = nTonesLeft > nTonesMax ? nTonesMax : nTonesLeft;

      sCmd.Index  = nTonesDone/2;
      sCmd.Length = nTonesCurrent/2;
      nErrCode = DSL_DRV_VRX_SendMessage(
                    pContext,
                    nDirection == DSL_DOWNSTREAM ? CMD_SNR_DS_GET : CMD_SNR_US_GET,
                    sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                    sizeof(sAck), (DSL_uint8_t*)&sAck);

      /* Copy data only if successful and valid */
      if ((nErrCode >= 0) && (sAck.Index <= (nDataNum/2) - sAck.Length))
      {
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: SNR data received (idx %d - %d):"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
            2*sAck.Index, 2*(sAck.Index + sAck.Length - 1)));

         for (i = 0;
               (i < sAck.Length) && (i < nTonesMax/2) && (i < nMaxNumOfEntries) &&
               ((2*(i + sAck.Index) + 1) < DSL_MAX_NSC); i++)
         {
            /* Copy received data to structure */
            pData->nNSCData[2*(i + sAck.Index)] =
               sAck.SNRpsds[i].snr_00;
            pData->nNSCData[2*(i + sAck.Index) + 1] =
               sAck.SNRpsds[i].snr_01;
            DSL_DEBUG(DSL_DBG_MSG,(pContext, SYS_DBG_MSG"%04X, %04X, ",
               pData->nNSCData[2*(i + sAck.Index)],
               pData->nNSCData[2*(i + sAck.Index) + 1]));
         }
      }
      else
      {
         DSL_DEBUG (DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR""DSL_DRV_CRLF "DSL[%02d]: Failed to "
            "retrieve SNR %d to %d" DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
            sCmd.Index*2, (sCmd.Index + sCmd.Length)*2));

         nErrCode = DSL_ERROR;
         break;
      }

      nTonesLeft -= ((DSL_uint16_t)(sAck.Length * 2));
      nTonesDone += ((DSL_uint16_t)(sAck.Length * 2));
   }

   if( nErrCode == DSL_SUCCESS )
   {
      pData->nNumData = nTonesDone;
      DSL_DEBUG(DSL_DBG_MSG,
         (pContext, SYS_DBG_MSG", nNumData=%d" DSL_DRV_CRLF, pData->nNumData));
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_DeltSnrGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return (nErrCode);
}
#endif /* defined(INCLUDE_DSL_G997_PER_TONE) || defined(INCLUDE_DSL_DELT)*/

#ifdef INCLUDE_DSL_DELT
static DSL_Error_t DSL_DRV_VRX_SpreadArray(
   DSL_void_t* pArray,
   DSL_uint16_t valueSize,
   DSL_uint16_t valueCount,
   DSL_uint16_t multiplier)
{
   DSL_void_t* pSrc = pArray + valueSize * (valueCount - 1);
   DSL_void_t* pDst = pArray + valueSize * (valueCount*multiplier - 1);

   if (pArray == DSL_NULL)
   {
      return DSL_ERROR;
   }

   for (; valueCount; --valueCount, pSrc -= valueSize)
   {
      DSL_uint16_t i;

      for ( i = multiplier; i; --i, pDst -= valueSize)
      {
         memcpy(pDst,pSrc,valueSize);
      }
   }

   return DSL_SUCCESS;
}

/**
   This function requests a set of up to 60 entries of the DELT data.

   \param pContext   DSL CPE API context
   \param nStartIndex Specifies the first tone group index to retrieve. [I]
   \param nStopIndex Specifies the last tone group index to retrieve. [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_DRV_VRX_SendMsgTestParamsFeRequest(
   DSL_Context_t *pContext,
   const DSL_uint16_t nStartIndex,
   const DSL_uint16_t nStopIndex)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   CMD_TestParamsFE_Request_t sCmd;
   ACK_TestParamsFE_Request_t sAck;

   DSL_CHECK_CTX_POINTER(pContext);
   DSL_CHECK_ERR_CODE();

   memset(&sCmd, 0, sizeof(sCmd));
   memset(&sAck, 0, sizeof(sAck));

   if ((nStopIndex < nStartIndex) ||
      (nStopIndex - nStartIndex > 60) ||
      (nStopIndex > 511))
   {
      DSL_DEBUG(DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: DSL_DRV_VRX_SendMsgTestParamsFeRequest illegal params start %d stop %d"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nStartIndex, nStopIndex));

      return (DSL_ERROR);
   }

   sCmd.Length     = DSL_VRX_16BIT_RD_MSG_LEN_GET(sCmd);
   sCmd.Control    = CMD_TestParamsFE_Request_TRIGGER;
   sCmd.StartIndex = nStartIndex;
   sCmd.EndIndex   = nStopIndex;

   nErrCode = DSL_DRV_VRX_SendMessage( pContext, CMD_TESTPARAMSFE_REQUEST,
                  sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                  sizeof(sAck), (DSL_uint8_t*)&sAck );

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgHlinGet(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDirection,
   DSL_G997_NSCComplexData_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint16_t i;
   DSL_uint16_t nTonesLeft, nTonesCurrent, nTonesDone, nDataNum = 512;
   CMD_HlinDS_Get_t  sCmd;
   ACK_HlinDS_Get_t  sAck;
   static const DSL_uint16_t nTonesMax = 64;
   static const DSL_uint8_t nMaxNumOfEntries = DSL_ARRAY_LENGTH(sAck.HLINpsds);
   DSL_BandList_t  sBandList;

   DSL_DEBUG( DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_SendMsgHlinGet(nDirection=%d)"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nDirection));

   /* init the return structure */
   memset(pData->nNSCComplexData, 0, nDataNum * sizeof(pData->nNSCComplexData[0]));
   pData->nNumData = 0;

   /* Check current xDSL mode*/
   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
   {
      nDataNum = 512;
   }
   else
   {
      memset(&sBandList, 0x0, sizeof(DSL_BandList_t));

      /* get number uf currently used bands */
      nErrCode = DSL_DRV_VRX_SendMsgBandControlGet(pContext, nDirection, &sBandList);
      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: ERROR -  getting current band config failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return nErrCode;
      }

      nDataNum = sBandList.nBand[0].nLastToneIndex + 1;
   }

   /*  several messages for max 64*2 values have to be sent */
   nTonesLeft = nDataNum;
   nTonesDone = 0;
   while (nTonesLeft)
   {
      nTonesCurrent = nTonesLeft > nTonesMax ? nTonesMax : nTonesLeft;

      memset(&sAck, 0, sizeof(sAck));
      sCmd.Index  = (DSL_uint16_t)(nTonesDone * 2);
      sCmd.Length = (DSL_uint16_t)(nTonesCurrent * 2);

      nErrCode = DSL_DRV_VRX_SendMessage(
                    pContext,
                    nDirection == DSL_DOWNSTREAM ? CMD_HLINDS_GET : CMD_HLINUS_GET,
                    sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                    sizeof(sAck), (DSL_uint8_t*)&sAck);

      /* Copy data only if successful and valid */
      if ((nErrCode == DSL_SUCCESS) && (sAck.Index <= (nDataNum * 2) - sAck.Length))
      {
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: Hlin data received (idx %d - %d):"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
            sAck.Index / 2, (sAck.Index + sAck.Length) / 2 - 1));

         for (i = 0;
               (i < sAck.Length / 2) && (i < nTonesMax) && (i < nMaxNumOfEntries) &&
               (i + sAck.Index/2 < DSL_MAX_NSC); i++)
         {
            /* Copy received data to structure */
            pData->nNSCComplexData[i + sAck.Index/2].nImag =
               (DSL_uint16_t)(sAck.HLINpsds[i].hlin_imag);
            pData->nNSCComplexData[i + sAck.Index/2].nReal =
               (DSL_uint16_t)(sAck.HLINpsds[i].hlin_real);
            DSL_DEBUG(DSL_DBG_MSG,(pContext, SYS_DBG_MSG"(%d, %d) ",
               pData->nNSCComplexData[i + sAck.Index/2].nReal,
               pData->nNSCComplexData[i + sAck.Index/2].nImag));
         }
      }
      else
      {
         DSL_DEBUG (DSL_DBG_WRN, (pContext, SYS_DBG_WRN""DSL_DRV_CRLF
            "DSL[%02d]: Failed to retrieve Hlin %d to %d" DSL_DRV_CRLF,
            DSL_DEV_NUM(pContext), sCmd.Index*2, (sCmd.Index + sCmd.Length)*2));

         nErrCode = DSL_ERROR;
         break;
      }

      nTonesLeft -= (sAck.Length / 2);
      nTonesDone += (sAck.Length / 2);
   }

   /* ADSL DS values are subsampled in FW by 2*/
   if( nErrCode == DSL_SUCCESS )
   {
      if( DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSL) &&
          (nDirection == DSL_DOWNSTREAM) && (nTonesDone > 256))
      {
         DSL_DRV_VRX_SpreadArray(
            (DSL_void_t*)pData->nNSCComplexData,sizeof(DSL_G997_ComplexNumber_t),nTonesDone/2,2);
      }

      pData->nNumData = nTonesDone;

      DSL_DEBUG(DSL_DBG_MSG,
         (pContext, SYS_DBG_MSG", nNumData=%d" DSL_DRV_CRLF, pData->nNumData));
   }

   DSL_DEBUG( DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_SendMsgHlinGet(nDirection=%d)"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nDirection));

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgHlogGet(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDirection,
   DSL_G997_NSCData16_t *pData)
{
   DSL_uint16_t nTonesMax = 128, nDataNum = 512;
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint16_t i = 0;
   DSL_uint16_t nTonesLeft, nTonesCurrent, nTonesDone;
   /* assume US/DS to have the same Msg Structures */
   CMD_HlogDS_Get_t  sCmd;
   ACK_HlogDS_Get_t  sAck;
   static const DSL_uint8_t nMaxNumOfEntries = DSL_ARRAY_LENGTH(sAck.HLOGpsds);
   DSL_BandList_t  sBandList;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_DeltHlogNeGet(nDirection=%d)"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nDirection));

   /* init the return structure */
   memset(pData->nNSCData, 0, nDataNum * sizeof(pData->nNSCData[0]));
   pData->nNumData = 0;

   /* Check current xDSL mode*/
   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
   {
      nDataNum = 512;
   }
   else
   {
      memset(&sBandList, 0x0, sizeof(DSL_BandList_t));

      /* get number uf currently used bands */
      nErrCode = DSL_DRV_VRX_SendMsgBandControlGet(pContext, nDirection, &sBandList);
      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: ERROR -  getting current band config failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return nErrCode;
      }

      nDataNum = sBandList.nBand[0].nLastToneIndex + 1;
   }

   nTonesLeft = nDataNum;

   nTonesDone = 0;
   while( nTonesLeft )
   {
      nTonesCurrent = nTonesLeft > nTonesMax ? nTonesMax : nTonesLeft;

      sCmd.Index  = nTonesDone;
      sCmd.Length = nTonesCurrent;
      nErrCode = DSL_DRV_VRX_SendMessage(
         pContext, nDirection == DSL_DOWNSTREAM ? CMD_HLOGDS_GET : CMD_HLOGUS_GET,
         sizeof(sCmd), (DSL_uint8_t*)&sCmd,
         sizeof(sAck), (DSL_uint8_t*)&sAck);

      /* Copy data only if successful and valid */
      if( (nErrCode >= 0) && (sAck.Index <= nDataNum - sAck.Length) )
      {
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: HLOG data received (idx %d - %d):"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
            sAck.Index, sAck.Index + sAck.Length - 1));

         for( i = 0;
             (i < sAck.Length) && (i < nTonesMax) && (i < nMaxNumOfEntries) &&
             (i + sAck.Index < DSL_MAX_NSC); i++)
         {
            /* Copy received data to structure */
            pData->nNSCData[i + sAck.Index] = sAck.HLOGpsds[i];
            DSL_DEBUG(DSL_DBG_MSG,
               (pContext, SYS_DBG_MSG"%04X, ", pData->nNSCData[i + sAck.Index]));
         }
      }
      else
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR""DSL_DRV_CRLF "DSL[%02d]: ERROR - Failed to retrieve HLOG %d to %d!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
            sCmd.Index, (sCmd.Index + sCmd.Length)));

         nErrCode = DSL_ERROR;
         break;
      }

      nTonesLeft -= sAck.Length;
      nTonesDone += sAck.Length;
   }

   if( nErrCode == DSL_SUCCESS )
   {
      if( DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSL) &&
          (nDirection == DSL_DOWNSTREAM) && (nTonesDone > 256))
      {
         DSL_DRV_VRX_SpreadArray((DSL_void_t*)pData->nNSCData,sizeof(DSL_uint16_t),nTonesDone/2,2);
      }

      pData->nNumData = nTonesDone;

      DSL_DEBUG(DSL_DBG_MSG,
         (pContext, SYS_DBG_MSG", nNumData=%d" DSL_DRV_CRLF, pData->nNumData));
   }

   if( nErrCode == DSL_SUCCESS )
   {
      pData->nNumData = nTonesDone;

      DSL_DEBUG(DSL_DBG_MSG,
         (pContext, SYS_DBG_MSG", nNumData=%d" DSL_DRV_CRLF, pData->nNumData));
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_DeltHlogNeGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgQlnGet(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDirection,
   DSL_G997_NSCData8_t * pData)
{
   static const DSL_uint16_t nTonesMax = 128;
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint16_t i = 0, nDataNum = 512;
   DSL_uint16_t nTonesLeft, nTonesCurrent, nTonesDone;
   /* assume US/DS to have the same Msg Structures */
   CMD_QLN_DS_Get_t  sCmd;
   ACK_QLN_DS_Get_t  sAck;
   static const DSL_uint8_t nMaxNumOfEntries = DSL_ARRAY_LENGTH(sAck.QLNds);
   DSL_BandList_t  sBandList;

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_DeltQlnNeGet(nDirection=%d)"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nDirection));

   /* init the return structure */
   memset(pData->nNSCData, 0, nDataNum * sizeof(pData->nNSCData[0]));
   pData->nNumData = 0;

   /* Check current xDSL mode*/
   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
   {
      nDataNum = 512;
   }
   else
   {
      memset(&sBandList, 0x0, sizeof(DSL_BandList_t));

      /* get number uf currently used bands */
      nErrCode = DSL_DRV_VRX_SendMsgBandControlGet(pContext, nDirection, &sBandList);
      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR -  getting current band config failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return nErrCode;
      }

      nDataNum = sBandList.nBand[0].nLastToneIndex + 1;
   }

   /* Request maximum allowable data independently of the current ADSL or VDSL mode*/
   nTonesLeft = nDataNum;

   nTonesDone = 0;
   while (nTonesLeft)
   {
      nTonesCurrent = nTonesLeft > nTonesMax ? nTonesMax : nTonesLeft;

      sCmd.Index  = nTonesDone/2;
      sCmd.Length = nTonesCurrent/2;
      nErrCode = DSL_DRV_VRX_SendMessage(pContext,
         nDirection == DSL_DOWNSTREAM ? CMD_QLN_DS_GET : CMD_QLN_US_GET,
         sizeof(sCmd), (DSL_uint8_t*)&sCmd,
         sizeof(sAck), (DSL_uint8_t*)&sAck);

      /* Copy data only if successful and valid */
      if ((nErrCode >= 0) && (sAck.Index <= (nDataNum/2)-sAck.Length))
      {
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: QLN data received (idx %d - %d):"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
            2*sAck.Index, 2*(sAck.Index + sAck.Length)));

         for (i = 0;
               (i < sAck.Length) && (i < nTonesMax/2) && (i < nMaxNumOfEntries) &&
               ((2 * (i + sAck.Index) + 1) < DSL_MAX_NSC); i++)
         {
            /* Copy received data to structure */
            pData->nNSCData[2 * (i + sAck.Index)] =
               (DSL_uint8_t)(sAck.QLNds[i].qln_00);
            pData->nNSCData[2 * (i + sAck.Index) + 1] =
               (DSL_uint8_t)(sAck.QLNds[i].qln_01);
            DSL_DEBUG(DSL_DBG_MSG,(pContext, SYS_DBG_MSG"%02X, %02X, ",
               pData->nNSCData[2 * (i + sAck.Index)],
               pData->nNSCData[2 * (i + sAck.Index) + 1]));
         }
      }
      else
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR""DSL_DRV_CRLF "DSL[%02d]: ERROR - Failed to retrieve QLN %d to %d!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext), sCmd.Index*2, (sCmd.Index + sCmd.Length)*2));
         nErrCode = DSL_ERROR;
         break;
      }

      nTonesLeft -= ((DSL_uint16_t)(sAck.Length * 2));
      nTonesDone += ((DSL_uint16_t)(sAck.Length * 2));
   }

   if( nErrCode == DSL_SUCCESS )
   {
      if( DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSL) &&
          (nDirection == DSL_DOWNSTREAM) && (nTonesDone > 256))
      {
         DSL_DRV_VRX_SpreadArray((DSL_void_t*)pData->nNSCData,sizeof(DSL_uint8_t),nTonesDone,2);
      }

      pData->nNumData = nTonesDone;

      DSL_DEBUG(DSL_DBG_MSG,
         (pContext, SYS_DBG_MSG", nNumData=%d" DSL_DRV_CRLF, pData->nNumData));
   }

   if( nErrCode == DSL_SUCCESS )
   {
      pData->nNumData = nTonesDone;

      DSL_DEBUG(DSL_DBG_MSG,
         (pContext, SYS_DBG_MSG", nNumData=%d" DSL_DRV_CRLF, pData->nNumData));
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_DeltQlnGet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return (nErrCode);
}
#endif /* INCLUDE_DSL_DELT*/

#ifdef INCLUDE_DSL_G997_PER_TONE
/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_GainAllocationTableGet(
   DSL_IN DSL_Context_t *pContext,
   const DSL_AccessDir_t nDirection,
   DSL_IN_OUT DSL_G997_NSCData16_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_VRX_FwVersion_t  sFwVersion = {0};
   DSL_uint32_t   nMsgId = 0;
   DSL_uint16_t   i = 0, nTonesLeft = 0, nTonesCurrent = 0, nTonesMax = 0,
                  nAckSize = 0, nAckIndex = 0, nAckLength = 0;
   DSL_LineStateValue_t nCurrentState = DSL_LINESTATE_UNKNOWN;
   /* assume US/DS ADSL/VDSL to have the same CMD Structures */
   CMD_GainTableEntriesGet_t  sCmd;
   union
   {
      ACK_GainTableEntriesGet_t  Vdsl;
      ACK_ADSL_GainTableUS_Get_t AdslUs;
      ACK_ADSL_GainTableDS_Get_t AdslDs;
   } sAck;
   DSL_BandList_t  sBandList;
   DSL_Band_t     *pBand = DSL_NULL;
   DSL_uint16_t   nBand = 0;
   DSL_uint8_t    nMaxNumOfEntries = 0;
   DSL_boolean_t  bAdslCpe = DSL_FALSE, bATUCformat = DSL_FALSE;
   /* Excess Margin Reduction*/
   DSL_uint16_t eExMrgRed = 0;
   CMD_ADSL_ExMarginReductionGet_t sCmdEMR;
   ACK_ADSL_ExMarginReductionGet_t sAckEMR;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_GainAllocationTableGet(nDirection=%d)"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nDirection));

   /* Only proceed if the specified line is in SHOWTIME state.
      In other cases set number of used data elements to zero. */
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineState, nCurrentState);

   if ((nCurrentState == DSL_LINESTATE_SHOWTIME_TC_SYNC) ||
       (nCurrentState == DSL_LINESTATE_SHOWTIME_NO_SYNC))
   {
      /* init the return structure */
      if (pData != DSL_NULL)
      {
         memset (pData->nNSCData, 0, DSL_MAX_NSC * sizeof(pData->nNSCData[0]));
      }
      else
      {
         return DSL_ERROR;
      }

      memset(&sBandList, 0x0, sizeof(DSL_BandList_t));

      /* assume VDSL */
      nErrCode = DSL_DRV_VRX_SendMsgBandControlGet(pContext, nDirection, &sBandList);
      if( nErrCode != DSL_SUCCESS )
      {
         DSL_DEBUG(DSL_DBG_MSG,
            (pContext, SYS_DBG_MSG"DSL[%02d]: ERROR - getting current band config failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return nErrCode;
      }

      /* get number uf currently used bands */
      if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
      {
         pData->nNumData = 4096;

         nMsgId = CMD_GAINTABLEENTRIESGET;
         nMaxNumOfEntries = DSL_ARRAY_LENGTH(sAck.Vdsl.Gains);
         nAckSize = sizeof(sAck.Vdsl);
      }
      else
      {
      /*...ADSL mode*/
         pData->nNumData = sBandList.nBand[0].nLastToneIndex + 1;

         /* Get FW info*/
         DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.fwFeatures, sFwVersion);

         if ( ((sFwVersion.nApplication == 5) && (sFwVersion.nFeatureSet >= 4)) ||
              ((sFwVersion.nApplication == 6) && (sFwVersion.nFeatureSet >= 5)) )
         {
            /* Assume that the GAIN values format was aligned with the ATU-C format*/
            bATUCformat = DSL_TRUE;
         }

         bAdslCpe = DSL_TRUE;

         if (nDirection == DSL_UPSTREAM)
         {
            nMsgId = CMD_ADSL_GAINTABLEUS_GET;
            nMaxNumOfEntries = DSL_ARRAY_LENGTH(sAck.AdslUs.Gains);
            nAckSize = sizeof(sAck.AdslUs);
         }
         else
         {
            nMsgId = CMD_ADSL_GAINTABLEDS_GET;
            nMaxNumOfEntries = DSL_ARRAY_LENGTH(sAck.AdslDs.Gains);
            nAckSize = sizeof(sAck.AdslDs);

            /* Excess Margin Reduction needed in case we use an old Q8.8 dB format*/
            if (!bATUCformat)
            {
               sCmdEMR.Index  = 0;
               sCmdEMR.Length = 1;

               /* Get Excess Margin Reduction*/
               nErrCode = DSL_DRV_VRX_SendMessage (pContext, CMD_ADSL_EXMARGINREDUCTIONGET,
                             sizeof(sCmdEMR), (DSL_uint8_t*)&sCmdEMR,
                             sizeof(sAckEMR), (DSL_uint8_t*)&sAckEMR);
               if (nErrCode != DSL_SUCCESS)
               {
                  DSL_DEBUG(DSL_DBG_ERR,
                     (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Excess Margin Reduction get failed!"
                     DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

                  return nErrCode;
               }
               else
               {
                  eExMrgRed = sAckEMR.eSnrmReduction;
               }
            }
         }
      }

      /* the maximum number of tone data retreivable with one message */
      nTonesMax = nMaxNumOfEntries;

      /* retrieve data only for the appropriate bands in sBandList */
      for (nBand = 0; (nBand < sBandList.nNumData) &&
                      (nBand < DSL_MAX_NUMBER_OF_BANDS); nBand++)
      {
         pBand = &sBandList.nBand[nBand];
         /*  several messages for max 128*2 values
         have to be sent */
         nTonesLeft = (pBand->nLastToneIndex - pBand->nFirstToneIndex) + 1;
         while (nTonesLeft)
         {
            nTonesCurrent = nTonesLeft > nTonesMax ? nTonesMax : nTonesLeft;

            sCmd.Index = (pBand->nLastToneIndex - (nTonesLeft - 1));
            sCmd.Length = nTonesCurrent;
            nErrCode = DSL_DRV_VRX_SendMessage(pContext, nMsgId,
                          sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                          nAckSize, (DSL_uint8_t*)&sAck);

            nAckIndex = sAck.Vdsl.Index;
            nAckLength = sAck.Vdsl.Length;

            /* Copy data only if successful and valid */
            if( (nErrCode == DSL_SUCCESS) &&
                (nAckIndex <= DSL_MAX_NSC - nAckLength) )
            {
               DSL_DEBUG(DSL_DBG_MSG,(pContext,
                  SYS_DBG_MSG"DSL[%02d]: Gain per Tone data received:"
                  DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

               if (nMaxNumOfEntries < nAckLength)
               {
                  DSL_DEBUG(DSL_DBG_WRN,(pContext,
                     SYS_DBG_WRN"DSL[%02d]: received Gain Elements truncated from %d to %d"
                     DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nAckLength, nMaxNumOfEntries));
               }

               for(i = 0;
                  (i < nAckLength) && (i < nMaxNumOfEntries) &&
                  (i + nAckIndex < DSL_MAX_NSC); i++)
               {
                  /* Copy received data to structure */
                  if (nDirection == DSL_DOWNSTREAM)
                  {
                     /* Rx Data is handled */
                     if (bAdslCpe == DSL_TRUE)
                     {
                        if (sAck.AdslDs.Gains[i] == Q88_NOVALUE)
                        {
                           pData->nNSCData[i + nAckIndex] = 0;
                        }
                        else
                        {
                           if (bATUCformat)
                           {
                           /* exactly the fine gains are reported here,
                              no additional calculation needed */
                           /* FW Format is Q3.13 (unsigned),
                              API codes units of 1/512 = 2^(-9)
                              (see chapter 7.5.1.20.2 of G.997.1)
                              APIV = (FWV / 2^13) * 2^9 = FWV / 2^4 */
                              pData->nNSCData[i + nAckIndex] = sAck.AdslDs.Gains[i] >> 4;
                           }
                           else
                           {
                              /* Values in Q8.8 dB */
                              pData->nNSCData[i + nAckIndex] = sAck.AdslDs.Gains[i] - eExMrgRed;
                           }
                        }
                     }
                     else
                     {
                        /* The receive gain provides the reciprocal
                           of the fine gain */
                        if (sAck.Vdsl.Gains[i] == 0)
                        {
                           pData->nNSCData[i + nAckIndex] = 0;
                        }
                        else
                        {
                           /* FW Format is Q3.13 (unsigned),
                              API codes units of 1/512 = 2^(-9)
                              (see chapter 7.5.1.20.2 of G.997.1)
                              APIV = (1 / (FWV / 2^13)) * 2^9 = 2^22 / FWV */
                           pData->nNSCData[i + nAckIndex] =
                                 (DSL_uint16_t)(0x400000 / sAck.Vdsl.Gains[i]);
                        }
                     }
                  }
                  else
                  {
                     /* Tx Data is handled */
                     if (bAdslCpe == DSL_TRUE)
                     {
                        /* exactly the fine gains are reported here,
                           no additional calculation needed */
                        /* FW Format is Q3.13 (unsigned),
                           API codes units of 1/512 = 2^(-9)
                           (see chapter 7.5.1.20.2 of G.997.1)
                           APIV = (FWV / 2^13) * 2^9 = FWV / 2^4 */
                        pData->nNSCData[i + nAckIndex] = sAck.AdslUs.Gains[i] >> 4;
                     }
                     else
                     {
                        /* The transmit gain includes both
                           the fine gain g and PSD shaping gain TSSI:
                           Gains(FW) =  g * 10^( TSSI / 20 )*/
                        /* FW Format is Q3.13 (unsigned),
                           API codes units of 1/512 = 2^(-9)
                           (see chapter 7.5.1.20.2 of G.997.1)
                           APIV = (FWV / 2^13) * 2^9 = FWV / 2^4 */
                        pData->nNSCData[i + nAckIndex] = sAck.Vdsl.Gains[i] >> 4;
                     }
                  }
                  DSL_DEBUG(DSL_DBG_MSG,(pContext,
                     SYS_DBG_MSG"%04X, ", pData->nNSCData[i + nAckIndex]));
               }
            }
            else
            {
               DSL_DEBUG (DSL_DBG_ERR, (pContext,
                  SYS_DBG_ERR""DSL_DRV_CRLF "DSL[%02d]: ERROR - Failed to retrieve Gains %d to %d"
                  DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
                  sCmd.Index * 2, (sCmd.Index + sCmd.Length) * 2));

               nErrCode = DSL_ERR_FUNCTION_WAITING_TIMEOUT;
               return nErrCode;
            }

            nTonesLeft -= nTonesCurrent;
         }
      }
   }
   else
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - Function is only available if line is in showtime!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      nErrCode = DSL_ERR_ONLY_AVAILABLE_IN_SHOWTIME;
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_GainAllocationTableGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SnrAllocationTableNeGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_OUT DSL_G997_NSCData8_t *pData)
{
   static const DSL_uint16_t nTonesMax = 100;
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint16_t nTonesLeft = 0, snrQ88 = 0, nTonesCurrent;
   DSL_uint16_t i = 0;
   /* assume US/DS to have the same Msg Structures */
   CMD_SNR_NE_TableEntriesGet_t  sCmd;
   ACK_SNR_NE_TableEntriesGet_t  sAck;
   DSL_BandList_t  sBandList;
   DSL_Band_t    *pBand = DSL_NULL;
   DSL_uint16_t  nBand = 0, intPart = 0;
   DSL_LineStateValue_t nCurrentState = DSL_LINESTATE_UNKNOWN;
   static const DSL_uint8_t nMaxNumOfEntries =
                               sizeof(sAck.SNRps)/sizeof(sAck.SNRps[0]);

   DSL_CHECK_POINTER(pContext, pData);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG (DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_SnrAllocationTableNeGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Only proceed if the specified line is in SHOWTIME state.
      In other cases set number of used data elements to zero. */
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nLineState, nCurrentState);

   if ((nCurrentState == DSL_LINESTATE_SHOWTIME_TC_SYNC) ||
       (nCurrentState == DSL_LINESTATE_SHOWTIME_NO_SYNC) ||
       (nCurrentState == DSL_LINESTATE_LOOPDIAGNOSTIC_COMPLETE))
   {
      if (pData != DSL_NULL)
      {
         /* init the return structure */
         memset (pData->nNSCData, 0xFF, DSL_MAX_NSC*sizeof(pData->nNSCData[0]));
      }
      else
      {
         return DSL_ERROR;
      }

      memset(&sBandList, 0x0, sizeof(DSL_BandList_t));

      /* get number uf currently used bands */
      nErrCode = DSL_DRV_VRX_SendMsgBandControlGet(
                    pContext, DSL_DOWNSTREAM, &sBandList);
      if( nErrCode != DSL_SUCCESS)
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - getting current band config failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

         return nErrCode;
      }

      if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
      {
         pData->nNumData = 4096;
      }
      else
      {
      /*...ADSL mode*/
         pData->nNumData = sBandList.nBand[0].nLastToneIndex + 1;
      }

      /* retrieve data only for the appropriate bands
         in sBandList */
      for (nBand = 0; (nBand < sBandList.nNumData) &&
                      (nBand < DSL_MAX_NUMBER_OF_BANDS); nBand++)
      {
         pBand = &sBandList.nBand[nBand];
         /*  several messages for max 128*2 values
         have to be sent */
         nTonesLeft = (pBand->nLastToneIndex - pBand->nFirstToneIndex) + 1;
         while (nTonesLeft)
         {
            nTonesCurrent = nTonesLeft > nTonesMax ? nTonesMax : nTonesLeft;

            sCmd.Index = (pBand->nLastToneIndex - (nTonesLeft - 1));
            sCmd.Length = nTonesCurrent;
            nErrCode = DSL_DRV_VRX_SendMessage(
                          pContext, CMD_SNR_NE_TABLEENTRIESGET,
                          sizeof(CMD_SNR_NE_TableEntriesGet_t),
                          (DSL_uint8_t*)&sCmd,
                          sizeof(ACK_SNR_NE_TableEntriesGet_t),
                          (DSL_uint8_t*)&sAck);

            /* Copy data only if successful and valid */
            if( (nErrCode == DSL_SUCCESS) &&
                (sAck.Index <= DSL_MAX_NSC-sAck.Length) )
            {
               DSL_DEBUG(DSL_DBG_MSG,
                  (pContext, SYS_DBG_MSG"DSL[%02d]: Signal to Noise Ratio data received (idx %d - %d):"
                  DSL_DRV_CRLF, DSL_DEV_NUM(pContext),sAck.Index, sAck.Index + sAck.Length));

               if( nMaxNumOfEntries < sAck.Length )
               {
                  DSL_DEBUG(DSL_DBG_WRN, (pContext,
                     SYS_DBG_WRN"DSL[%02d]: received SNR Elements truncated from %d to %d"
                     DSL_DRV_CRLF, DSL_DEV_NUM(pContext), sAck.Length, nMaxNumOfEntries));
               }

               for( i = 0;
                   (i < sAck.Length) && (i<nTonesMax) &&
                   (i < nMaxNumOfEntries) && (i + sAck.Index < DSL_MAX_NSC); i++)
               {
                  /* Copy received data to structure */
                  if (sAck.SNRps[i]  == Q88_NOVALUE)
                  {
                     /* a 0 value indicates that no measurement took place
                        in FW. DSL CPE API indicates 0xFF in this case. */
                     pData->nNSCData[i + sAck.Index] = 255;
                  }
                  else
                  {
                     /* SNR Data from the message is Q8.8 coded,
                        API value is (SNR[dB]+32)*2
                       (see chapter 7.5.1.20.4 of G.997.1) */
                     /*pData->nNSCData[i + sAck.Index] =
                       (DSL_uint8_t)
                       ((DSL_DRV_VRX_GetDbFromQ88(sAck.SNRps[i],1)+32) * 2 + 0.5);
                     */

                     snrQ88  = (DSL_uint16_t)(((sAck.SNRps[i]) + (32 << 8)) * 2);
                     intPart = (snrQ88 >> 8) & 0xFF;

                     pData->nNSCData[i + sAck.Index] =
                        (DSL_uint8_t)((snrQ88 & 0xFF) < 128 ? intPart : intPart + 1);
                  }

                  DSL_DEBUG(DSL_DBG_MSG,
                     (pContext, SYS_DBG_MSG"%02X, ", pData->nNSCData[i + sAck.Index]));
               }
            }
            else
            {
               DSL_DEBUG(DSL_DBG_ERR,
                  (pContext, SYS_DBG_ERR""DSL_DRV_CRLF "DSL[%02d]: ERROR - Failed to retrieve SNR %d to %d!"
                  DSL_DRV_CRLF, DSL_DEV_NUM(pContext),
                  sCmd.Index*2, (sCmd.Index + sCmd.Length)*2));

               nErrCode = DSL_ERR_FUNCTION_WAITING_TIMEOUT;
               return nErrCode;
            }

            nTonesLeft -= nTonesCurrent;
         }
      }
   }
   else
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - SNR allocation table retreiving only"
         " available in the SHOWTIME or DIAG_COMPLETE states!" DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));

      nErrCode = DSL_ERROR;
   }


   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_SnrAllocationTableNeGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return (nErrCode);
}
#endif /* INCLUDE_DSL_G997_PER_TONE*/

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgNoiseMarginDeltaSet(
   DSL_Context_t *pContext,
   DSL_int16_t nNoiseMarginDelta)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   CMD_NoiseMarginDeltaSet_t sCmd;
   ACK_NoiseMarginDeltaSet_t sAck;

   memset(&sCmd, 0, sizeof(sCmd));
   memset(&sAck, 0, sizeof(sAck));
   sCmd.Length = 0x1;
   sCmd.deltaTARSNRMds = nNoiseMarginDelta;

   nErrCode = DSL_DRV_VRX_SendMessage( pContext, CMD_NOISEMARGINDELTASET,
                 sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                 sizeof(sAck), (DSL_uint8_t*)&sAck);

   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - NoiseMarginDelta set failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return DSL_ERROR;
   }

   return nErrCode;
}

#if defined(INCLUDE_DSL_PM) && defined(INCLUDE_DSL_CPE_PM_CONFIG)
/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgOhOptionsSet(
   DSL_Context_t *pContext,
   DSL_uint32_t nEocPollPeriod,
   DSL_boolean_t bEocPoll1,
   DSL_boolean_t bEocPoll2)
{
   return DSL_ERR_NOT_SUPPORTED_BY_FIRMWARE;

   /* Attention: This message is currently not supported but will be supported
                 in the near future. Therefore implementation is excluded at
                 compile time here only temporarily. */
#if 0
   DSL_Error_t nErrCode = DSL_SUCCESS, nRet = DSL_SUCCESS;
   DSL_VRX_FwVersion_t fwVersion = {0};
   DSL_uint32_t nEocPollFactor = 1;
   CMD_OH_OptionsSet_t sCmd;
   ACK_OH_OptionsSet_t sAck;

   /* Get FW version information*/
   DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.fwFeatures, fwVersion);

   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
   {
      DSL_DEBUG(DSL_DBG_WRN,
         (pContext, SYS_DBG_WRN"DSL[%02d]: WARNING - EOC configuration not supported!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return DSL_WRN_FIRMWARE_MSG_DENIED;
   }

   /* Check EOC configuration*/
   if ((bEocPoll1 || bEocPoll2) && !nEocPollPeriod)
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Invalid EOC configuration!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return DSL_ERR_INVALID_PARAMETER;
   }

   memset(&sCmd, 0, sizeof(sCmd));

   sCmd.Length = 0x1;
   sCmd.eocPoll1 = bEocPoll1 ? VRX_ENABLE : VRX_DISABLE;
   sCmd.eocPoll2 = bEocPoll2 ? VRX_ENABLE : VRX_DISABLE;

   nEocPollFactor     = (nEocPollPeriod/5) + ((nEocPollPeriod%5) ? 1 : 0);
   sCmd.eocPollFactor = nEocPollFactor > 10 ? 10 : nEocPollFactor;

   /* Check for truncation*/
   if ((nEocPollPeriod%5) || (nEocPollFactor > 10))
   {
      DSL_DEBUG(DSL_DBG_WRN,
         (pContext, SYS_DBG_WRN"DSL[%02d]: WARNING - EOC poll period truncated(%d->%d)!"\
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nEocPollPeriod, sCmd.eocPollFactor*5));
      nRet = DSL_WRN_CONFIG_PARAM_TRUNCATED;
   }

   nErrCode =  DSL_DRV_VRX_SendMessage( pContext, CMD_OH_OPTIONSSET,
                   sizeof(CMD_OH_OptionsSet_t), (DSL_uint8_t *)&sCmd,
                   sizeof(ACK_OH_OptionsSet_t), (DSL_uint8_t *)&sAck );

   if (nErrCode != DSL_SUCCESS)
   {
      return nErrCode;
   }

   return nRet;
#endif
}
#endif /* defined(INCLUDE_DSL_PM) && defined(INCLUDE_DSL_CPE_PM_CONFIG)*/

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgLineFailureFeGet(
   DSL_Context_t *pContext,
   DSL_uint8_t *pAck)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   CMD_LineFailureFE_Get_t sCmd;
   ACK_LineFailureFE_Get_t sAck;

   sCmd.Index  = 0;
   sCmd.Length = 1;

   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSL))
   {
      DSL_DEBUG( DSL_DBG_WRN,
         (pContext, SYS_DBG_WRN"DSL[%02d]: WARNING - FE line failures retrieving not supported "
         "by the FW in ADSL mode!"DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   /* Get Line failures status information from the FW*/
   nErrCode =  DSL_DRV_VRX_SendMessage(
                   pContext, CMD_LINEFAILUREFE_GET,
                   sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                   sizeof(sAck), (DSL_uint8_t*)&sAck);

   if( nErrCode < 0 )
   {
      return nErrCode;
   }

   memcpy(pAck, &sAck, sizeof(ACK_LineFailureFE_Get_t));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgLineFailureNeGet(
   DSL_Context_t *pContext,
   DSL_uint8_t *pAck)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   CMD_LineFailureNE_Get_t sCmd;
   ACK_LineFailureNE_Get_t sAck;

   sCmd.Index  = 0;
   sCmd.Length = 1;

   /* Get Line failures status information from the FW*/
   nErrCode =  DSL_DRV_VRX_SendMessage(
                   pContext, CMD_LINEFAILURENE_GET,
                   sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                   sizeof(sAck), (DSL_uint8_t*)&sAck);

   if( nErrCode < 0 )
   {
      return nErrCode;
   }

   memcpy(pAck, &sAck, sizeof(ACK_LineFailureNE_Get_t));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgDataPathFailuresGet(
   DSL_Context_t *pContext,
   DSL_uint8_t *pAck)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   CMD_DataPathFailuresGet_t sCmd;
   ACK_DataPathFailuresGet_t sAck;

   sCmd.Index  = 0;
   sCmd.Length = 2;

   nErrCode =  DSL_DRV_VRX_SendMessage( pContext, CMD_DATAPATHFAILURESGET,
                    sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                    sizeof(sAck), (DSL_uint8_t*)&sAck );
   if( nErrCode < 0 )
   {
      return nErrCode;
   }

   memcpy(pAck, &sAck, sizeof(ACK_DataPathFailuresGet_t));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgLineStatusPerBandGet(
   DSL_Context_t *pContext,
   DSL_AccessDir_t nDirection,
   DSL_uint8_t *pAck)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint32_t nMsgId = 0;
   CMD_LineStatusPerBandDS_Get_t sCmd;
   union
   {
      ACK_LineStatusPerBandUS_Get_t US;
      ACK_LineStatusPerBandDS_Get_t DS;
   } sAck;

   memset (&sCmd, 0 , sizeof(sCmd));
   sCmd.Length = DSL_VRX_16BIT_RD_MSG_LEN_GET(sAck.US);

   nMsgId = nDirection == DSL_UPSTREAM ? CMD_LINESTATUSPERBANDUS_GET :
                                         CMD_LINESTATUSPERBANDDS_GET;

   nErrCode = DSL_DRV_VRX_SendMessage(pContext, nMsgId,
      sizeof(sCmd), (DSL_uint8_t*)&sCmd,
      sizeof(ACK_LineStatusPerBandDS_Get_t), (DSL_uint8_t*)&sAck);

   if( nErrCode >= DSL_SUCCESS )
   {
      memcpy(pAck, &sAck, sizeof(ACK_LineStatusPerBandDS_Get_t));
   }

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgUsPowerBackOffStatusGet(
   DSL_Context_t *pContext,
   DSL_uint8_t *pAck)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   CMD_PBO_AELEM_Status_Get_t sCmd;
   ACK_PBO_AELEM_Status_Get_t sAck;

   memset (&sCmd, 0 , sizeof(sCmd));
   memset (&sAck, 0 , sizeof(sAck));
   sCmd.Length = 11;

   nErrCode =  DSL_DRV_VRX_SendMessage( pContext, CMD_PBO_AELEM_STATUS_GET,
                    sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                    sizeof(sAck), (DSL_uint8_t*)&sAck );
   if( nErrCode < 0 )
   {
      return nErrCode;
   }

   memcpy(pAck, &sAck, sizeof(ACK_PBO_AELEM_Status_Get_t));

   return (nErrCode);
}

#ifdef INCLUDE_DSL_G997_LINE_INVENTORY
static DSL_Error_t DSL_DRV_VRX_SendMsgAuxInventoryInfoOGet(
   DSL_Context_t *pContext,
   DSL_AuxLineInventoryData_t *pData)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   CMD_AuxInventoryInfo_O_Get_t sCmd;
   ACK_AuxInventoryInfo_O_Get_t sAck;
   DSL_uint32_t nBytes = sizeof(pData->pData);
   DSL_uint8_t i = 0;

   /* do the Aux Info, not part of G997.1 */
   memset(&sCmd, 0, sizeof(sCmd));
   memset(&sAck, 0, sizeof(sAck));

   /* there was nothing copied yet */
   pData->nLength = 0;
   sCmd.Length = DSL_VRX_16BIT_RD_MSG_LEN_GET(sAck);

   nErrCode = DSL_DRV_VRX_SendMessage(
                 pContext, CMD_AUXINVENTORYINFO_O_GET,
                 sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                 sizeof(sAck), (DSL_uint8_t*)&sAck);

   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Aux Line Inventory Get failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   pData->nLength = (sCmd.Length * 2);

   /* get the data */
   for( i = 0; i < DSL_ARRAY_LENGTH(sAck.auxInfo); i++ )
   {
      if( (DSL_uint32_t)(i*2 + 1) < nBytes )
      {
         pData->pData[i*2 + 1] = (DSL_uint8_t)(sAck.auxInfo[i] >> 8);
         pData->pData[i*2]     = (DSL_uint8_t)(sAck.auxInfo[i]);
      }
   }

   return nErrCode;
}
#endif /* INCLUDE_DSL_G997_LINE_INVENTORY*/

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgInventoryNeSet(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint16_t nBytesLeft, nBytesSent, nBytesMsg, nBytes, nIdx = 0;

   union
   {
      CMD_SysVendorID_R_Set_t          VendorId;
      CMD_SysVendorVersionNum_R_Set_t  VendorVersion;
      CMD_SysVendorSerialNum_R_Set_t   VendorSerial;
      CMD_AuxInventoryInfo_R_Set_t     AuxInfo;
   } sCmd;
   ACK_SysVendorID_R_Set_t sAck;
   DSL_uint8_t i = 0;

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_SendMsgInventoryNeSet()"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   DSL_DRV_MUTEX_LOCK(pContext->dataMutex);

   /* do the Vendor ID */
   memset(&sCmd, 0, sizeof(sCmd));

   sCmd.VendorId.Length = DSL_VRX_16BIT_RD_MSG_LEN_GET(sCmd.VendorId);
   /* fill the message */
   for (i = 0; i < DSL_ARRAY_LENGTH(sCmd.VendorId.sysVendorID); i++)
   {
      if( (DSL_uint32_t)(i*2 + 1) <
                   DSL_ARRAY_LENGTH(pContext->lineInventoryNe.SystemVendorID) )
         sCmd.VendorId.sysVendorID[i] =
            ((DSL_uint16_t)(pContext->lineInventoryNe.SystemVendorID[i*2 + 1]) << 8) |
             (DSL_uint16_t)(pContext->lineInventoryNe.SystemVendorID[i*2]);
   }
   nErrCode = DSL_DRV_VRX_SendMessage(
                 pContext, CMD_SYSVENDORID_R_SET,
                 sizeof(sCmd.VendorId), (DSL_uint8_t*)&sCmd,
                 sizeof(sAck), (DSL_uint8_t*)&sAck);

   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG(DSL_DBG_MSG,(pContext,
         SYS_DBG_MSG"DSL[%02d]: ERROR - Vendor ID set, retCode=%d!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));
   }

   /* do the Vendor Version */
   memset(&sCmd, 0, sizeof(sCmd));

   sCmd.VendorVersion.Length = DSL_VRX_16BIT_RD_MSG_LEN_GET(sCmd.VendorVersion);
   for (i = 0; i < DSL_ARRAY_LENGTH(sCmd.VendorVersion.versionNum); i++)
   {
      if( (DSL_uint32_t)(i*2 + 1) <
                    DSL_ARRAY_LENGTH(pContext->lineInventoryNe.VersionNumber) )
         sCmd.VendorVersion.versionNum[i] =
            ((DSL_uint16_t)(pContext->lineInventoryNe.VersionNumber[i*2 + 1]) << 8) |
             (DSL_uint16_t)(pContext->lineInventoryNe.VersionNumber[i*2]);
   }
   nErrCode = DSL_DRV_VRX_SendMessage(
                 pContext, CMD_SYSVENDORVERSIONNUM_R_SET,
                 sizeof(sCmd.VendorVersion), (DSL_uint8_t*)&sCmd,
                 sizeof(sAck), (DSL_uint8_t*)&sAck);

   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG(DSL_DBG_ERR,(pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - Vendor Version set, retCode=%d!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));
   }

   /* do the Vendor Serial Num */
   memset(&sCmd, 0, sizeof(sCmd));

   sCmd.VendorSerial.Length = DSL_VRX_16BIT_RD_MSG_LEN_GET(sCmd.VendorSerial);
   for (i = 0; i < DSL_ARRAY_LENGTH(sCmd.VendorSerial.serialNum); i++)
   {
      if( (DSL_uint32_t)(i*2 + 1) <
                     DSL_ARRAY_LENGTH(pContext->lineInventoryNe.SerialNumber) )
         sCmd.VendorSerial.serialNum[i] =
            ((DSL_uint16_t)(pContext->lineInventoryNe.SerialNumber[i*2 + 1]) << 8) |
             (DSL_uint16_t)(pContext->lineInventoryNe.SerialNumber[i*2]);
   }
   nErrCode = DSL_DRV_VRX_SendMessage(
                 pContext, CMD_SYSVENDORSERIALNUM_R_SET,
                 sizeof(sCmd.VendorSerial), (DSL_uint8_t*)&sCmd,
                 sizeof(sAck), (DSL_uint8_t*)&sAck);

   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG(DSL_DBG_MSG,(pContext,
         SYS_DBG_MSG"DSL[%02d]: ERROR - Vendor Serial set, retCode=%d!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));
   }

   /* not for ADSL-only mode */
   if( DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2) )
   {
      /* do the Aux Info, not part of G997.1 */
      memset(&sCmd, 0, sizeof(sCmd));

      nBytesLeft = (DSL_uint16_t)(pContext->auxInventoryNe.nLength);
      nBytesSent = 0;
      nBytesMsg = DSL_VRX_16BIT_RD_MSG_LEN_GET(sCmd.AuxInfo) * 2;
      while( nBytesLeft )
      {
         nBytes = nBytesLeft > nBytesMsg ? nBytesMsg : nBytesLeft;

         sCmd.AuxInfo.Length  = (nBytes + 1) / 2;
         sCmd.AuxInfo.Index   = nBytesSent / 2;

         for( i = 0; (i < sCmd.AuxInfo.Length) && (i < DSL_ARRAY_LENGTH(sCmd.AuxInfo.auxInfo)) &&
                     (nIdx+1 < DSL_G993_LI_MAXLEN_AUX);
              i++, nIdx = i*2 + nBytesSent)
         {
            sCmd.AuxInfo.auxInfo[i] =
               ((DSL_uint16_t)(pContext->auxInventoryNe.pData[nIdx + 1]) << 8) |
                (DSL_uint16_t)(pContext->auxInventoryNe.pData[nIdx]);
         }

         nErrCode = DSL_DRV_VRX_SendMessage(
                       pContext, CMD_AUXINVENTORYINFO_R_SET,
                       (DSL_uint16_t)(4 + (sCmd.AuxInfo.Length * 2)), (DSL_uint8_t*)&sCmd,
                       sizeof(sAck), (DSL_uint8_t*)&sAck);

         if( nErrCode != DSL_SUCCESS )
         {
            DSL_DEBUG(DSL_DBG_ERR,(pContext,
               SYS_DBG_ERR"DSL[%02d]: ERROR - Aux Info set failed, retCode=%d!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));
         }
         nBytesLeft -= nBytes;
         nBytesSent += nBytes;
      }
   }

   DSL_DRV_MUTEX_UNLOCK(pContext->dataMutex);

   if( nErrCode != DSL_SUCCESS )
      nErrCode = DSL_ERROR;

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_SendMsgInventoryNeSet, retCode=%d"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return DSL_SUCCESS;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgOlrControl(
   DSL_Context_t *pContext,
   DSL_boolean_t bAutoSRA_DS,
   DSL_boolean_t bAutoSRA_US,
   DSL_boolean_t bSos_DS,
   DSL_boolean_t bSos_US)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_VRX_FwVersion_t sFwVersion = {0};
   CMD_OLR_Control_t sCmd;
   ACK_OLR_Control_t sAck;
   DSL_boolean_t bTmpVal = DSL_FALSE;
   DSL_DEV_VersionCheck_t nVerCheck = 0;

   /* Get FW information*/
   DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.fwFeatures, sFwVersion);

   memset(&sCmd, 0, sizeof(sCmd));
   memset(&sAck, 0, sizeof(sAck));

   sCmd.Length = 0x1;

   /* Check firmware version */
   nErrCode = DSL_DRV_VRX_FirmwareVersionCheck(pContext,
               DSL_MIN_FW_VERSION_SRA_VDSL, &nVerCheck);

   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG(DSL_DBG_ERR, (pContext, SYS_DBG_ERR
         "DSL[%02d]: ERROR - FW version check failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return nErrCode;
   }

   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSL) ||
                 (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2)
                                          && (nVerCheck >= DSL_VERSION_EQUAL)))
   {
      sCmd.autoSRA_DS = bAutoSRA_DS ? VRX_ENABLE : VRX_DISABLE;
      sCmd.autoSRA_US = bAutoSRA_US ? VRX_ENABLE : VRX_DISABLE;
   }

   /* Rx Bitswap */
   DSL_CTX_READ_SCALAR(pContext, nErrCode,
      lineFeatureDataCfg[DSL_DOWNSTREAM].bBitswapEnable,
      bTmpVal);
   sCmd.RxBitswap = bTmpVal ? VRX_ENABLE : VRX_DISABLE;

   /* Tx Bitswap */
   DSL_CTX_READ_SCALAR(pContext, nErrCode,
      lineFeatureDataCfg[DSL_UPSTREAM].bBitswapEnable,
      bTmpVal);
   sCmd.TxBitswap = bTmpVal ? VRX_ENABLE : VRX_DISABLE;

   nErrCode = DSL_DRV_VRX_SendMessage(
                 pContext, CMD_OLR_CONTROL,
                 sizeof(CMD_OLR_Control_t), (DSL_uint8_t*)&sCmd,
                 sizeof(ACK_OLR_Control_t), (DSL_uint8_t*)&sAck);

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgRtxDsConfigure(
   DSL_Context_t *pContext,
   DSL_boolean_t bReTxEnable)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   CMD_RTX_DS_Configure_t sCmd;
   ACK_RTX_DS_Configure_t sAck;
   DSL_DEV_VersionCheck_t nVerCheck = DSL_VERSION_ERROR;

   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
   {
      /* Get FW information */
      nErrCode = DSL_DRV_VRX_FirmwareVersionCheck(pContext,
                  DSL_MIN_FW_VERSION_RETX_VDSL, &nVerCheck);
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
                  DSL_MIN_FW_VERSION_RETX_ADSL, &nVerCheck);
      if (nErrCode != DSL_SUCCESS)
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW version check failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         return nErrCode;
      }
   }

   if (nVerCheck >= DSL_VERSION_EQUAL)
   {
      memset(&sCmd, 0, sizeof(sCmd));
      memset(&sAck, 0, sizeof(sAck));

      sCmd.Length = 0x1;
      sCmd.RtxMode = bReTxEnable ? VRX_ENABLE : VRX_DISABLE;

      nErrCode = DSL_DRV_VRX_SendMessage(
                 pContext, CMD_RTX_DS_CONFIGURE,
                 sizeof(CMD_RTX_DS_Configure_t), (DSL_uint8_t*)&sCmd,
                 sizeof(ACK_RTX_DS_Configure_t), (DSL_uint8_t*)&sAck);
   }
   else
   {
      DSL_DEBUG(DSL_DBG_WRN, (pContext,
         SYS_DBG_WRN"DSL[%02d]: WARNING - ReTx not supported by the FW!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgTr1Control(
   DSL_Context_t *pContext,
   DSL_boolean_t bTR1Enable,
   DSL_boolean_t bTR1EventEnable,
   DSL_uint16_t nTR1Period,
   DSL_uint16_t nTR1Control,
   DSL_uint16_t nTR1Adjust )
{
   return DSL_SUCCESS;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgTestParamsAuxGet(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDirection,
   DSL_uint8_t *pAck)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   /* assume US/DS to have the same Msg Structures */
   CMD_TestParamsAuxDS_Get_t  sCmd;
   ACK_TestParamsAuxDS_Get_t  sAck;

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_SendMsgTestParamsAuxGet(nDirection=%d)"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nDirection));

   memset(&sCmd, 0, sizeof(sCmd));
   memset(&sAck, 0, sizeof(sAck));

   sCmd.Length = DSL_VRX_16BIT_RD_MSG_LEN_GET(sAck);
   nErrCode = DSL_DRV_VRX_SendMessage(
      pContext,
      nDirection == DSL_DOWNSTREAM ? CMD_TESTPARAMSAUXDS_GET : CMD_TESTPARAMSAUXUS_GET,
      sizeof(sCmd), (DSL_uint8_t*)&sCmd,
      sizeof(sAck), (DSL_uint8_t*)&sAck);

   if( nErrCode != DSL_ERROR )
   {
      memcpy(pAck, &sAck, sizeof(ACK_TestParamsAuxDS_Get_t));
   }

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_SendMsgTestParamsAuxGet, retCode"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext), nErrCode));

   return (nErrCode);
}

#ifdef INCLUDE_DSL_G997_LINE_INVENTORY
/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgStandardInfoFeVdsl2Get(
   DSL_Context_t *pContext,
   DSL_uint8_t *pXTSE)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   /* The message structures are identical for CO and CPE */
   ACK_HS_StandardInfoFE_VDSL2Get_t sAck;
   CMD_HS_StandardInfoFE_VDSL2Get_t sCmd;

   DSL_CHECK_POINTER(pContext, pXTSE);
   DSL_CHECK_ERR_CODE();

   memset(&sCmd, 0, sizeof(sCmd));
   memset(&sAck, 0, sizeof(sAck));

   DSL_DEBUG(DSL_DBG_MSG,(pContext,
      SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_SendMsgStandardInfoFeVdsl2Get"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   sCmd.Length = DSL_VRX_16BIT_RD_MSG_LEN_GET(sAck);
   nErrCode = DSL_DRV_VRX_SendMessage(
                 pContext, CMD_HS_STANDARDINFOFE_VDSL2GET,
                 sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                 sizeof(sAck), (DSL_uint8_t*)&sAck);

   if( nErrCode != DSL_SUCCESS )
   {
      DSL_DEBUG(DSL_DBG_MSG, (pContext,
         SYS_DBG_MSG"DSL[%02d]: ERROR - Far-End XTSE capabilities get failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }

   /* get the data */
   pXTSE[8-1] &= ~(DSL_uint8_t)(XTSE_8_01_A|XTSE_8_02_B|XTSE_8_03_C);
   if (sAck.A_US0PsdSup21 ||
       sAck.A_US0PsdSup20 ||
       sAck.A_US0PsdSup19 ||
       sAck.A_US0PsdSup18 ||
       sAck.A_US0PsdSup17 ||
       sAck.A_US0PsdSup16 ||
       sAck.A_US0PsdSup15 ||
       sAck.A_US0PsdSup14 ||
       sAck.A_US0PsdSup13 ||
       sAck.A_US0PsdSup12 ||
       sAck.A_US0PsdSup11 ||
       sAck.A_US0PsdSup10 ||
       sAck.A_US0PsdSup9 ||
       sAck.A_US0PsdSup8 ||
       sAck.A_US0PsdSup7 ||
       sAck.A_US0PsdSup6 ||
       sAck.A_US0PsdSup5 ||
       sAck.A_US0PsdSup4 ||
       sAck.A_US0PsdSup3 ||
       sAck.A_US0PsdSup2 ||
       sAck.A_US0PsdSup1 ||
       sAck.A_US0PsdSup0)
       pXTSE[8-1] |= (DSL_uint8_t)XTSE_8_01_A;
   if (sAck.B_US0PsdSup4 ||
       sAck.B_US0PsdSup3 ||
       sAck.B_US0PsdSup2 ||
       sAck.B_US0PsdSup1 ||
       sAck.B_US0PsdSup0)
       pXTSE[8-1] |= (DSL_uint8_t)XTSE_8_02_B;
   if (sAck.C_US0PsdSup0 ||
       sAck.C_US0PsdSup1 ||
       sAck.C_US0PsdSup4 ||
       sAck.C_US0PsdSup5 ||
       sAck.C_US0PsdSup12 ||
       sAck.C_US0PsdSup13)
       pXTSE[8-1] |= (DSL_uint8_t)XTSE_8_03_C;

   DSL_DEBUG(DSL_DBG_MSG,(pContext,
      SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_SendMsgStandardInfoFeVdsl2Get"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return (nErrCode);
}
#endif /* INCLUDE_DSL_G997_LINE_INVENTORY*/

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgVdsl2ProfileControl(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint32_t i = 0;
   DSL_G997_XTUSystemEnablingData_t XTSE;
   DSL_PortMode_t nPortMode;
   CMD_HS_VDSL2ProfileControl_t   sCmd;
   ACK_HS_VDSL2ProfileControl_t   sAck;

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_SendMsgVdsl2ProfileControl"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   memset(&sCmd, 0, sizeof(sCmd));
   memset(&sAck, 0, sizeof(sAck));

   for (i = 0; i < DSL_G997_NUM_XTSE_OCTETS; i++)
   {
      DSL_CTX_READ_SCALAR(pContext, nErrCode, xtseCfg[i], XTSE.XTSE[i]);
   }

   if (((XTSE.XTSE[8-1] & XTSE_8_01_A) == 0) &&
       ((XTSE.XTSE[8-1] & XTSE_8_02_B) == 0) &&
       ((XTSE.XTSE[8-1] & XTSE_8_03_C) == 0))
   {
      /* no VDSL2 bit set, message is not needed */
      return (nErrCode);
   }
   /* get port mode type*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->data.nPortMode, nPortMode);

   sCmd.Length = DSL_VRX_16BIT_RD_MSG_LEN_GET(sCmd);

   /* enable only profiles 8 a/b/c/d and its related US0 support bits. */
   if (nPortMode == DSL_PORT_MODE_DUAL)
   {
      sCmd.profileSup3 = sCmd.profileSup2 =
      sCmd.profileSup1 = sCmd.profileSup0 = 1;
   }
   /* enable all profiles and its related US0 support bits. */
   else
   {
      sCmd.profileSup7 = sCmd.profileSup6 = sCmd.profileSup5 =
      sCmd.profileSup4 = sCmd.profileSup3 = sCmd.profileSup2 =
      sCmd.profileSup1 = sCmd.profileSup0 = 1;

      /* ------- US0 support bits for profiles != 8 ------- */
      /* Message parameter #5 - Annex A US0 PSDs Supported Bits */
      sCmd.A_US0PsdSup18 = 1;  /* 12b Profile */
      sCmd.A_US0PsdSup19 = 1;  /* 17a Profile */
      /* Message parameter #8 - Annex B US0 PSDs Supported Bits */
      sCmd.B_US0PsdSup3 = 1;  /* 12b Profile */
      sCmd.B_US0PsdSup4 = 1;  /* 17a Profile */
      /* Message parameter #10 -  Annex B US0 PSDs Supported Bits */
      sCmd.C_US0PsdSup12 = 1; /* 12b Profile, Annex C */
      sCmd.C_US0PsdSup13 = 1; /* 17a Profile, Annex C */
   }

   /* ------- US0 support bits for all profiles ------- */
   /* Message parameter #4 - Annex A US0 PSDs Supported Bits */
   sCmd.A_US0PsdSup0 = 1;   /* EU-32 */
   sCmd.A_US0PsdSup1 = 1;   /* EU-36 */
   sCmd.A_US0PsdSup2 = 1;   /* EU-40 */
   sCmd.A_US0PsdSup3 = 1;   /* EU-44 */
   sCmd.A_US0PsdSup4 = 1;   /* EU-48 */
   sCmd.A_US0PsdSup5 = 1;   /* EU-52 */
   sCmd.A_US0PsdSup6 = 1;   /* EU-56 */
   sCmd.A_US0PsdSup7 = 1;   /* EU-60 */
   sCmd.A_US0PsdSup8 = 1;   /* EU-64 */
   sCmd.A_US0PsdSup9 = 1;   /* ADLU-32 */
   sCmd.A_US0PsdSup10 = 1;  /* ADLU-36 */
   sCmd.A_US0PsdSup11 = 1;  /* ADLU-40 */
   sCmd.A_US0PsdSup12 = 1;  /* ADLU-44 */
   sCmd.A_US0PsdSup13 = 1;  /* ADLU-48 */
   sCmd.A_US0PsdSup14 = 1;  /* ADLU-52 */
   sCmd.A_US0PsdSup15 = 1;  /* ADLU-56 */

   /* Message parameter #5 - Annex A US0 PSDs Supported Bits */
   sCmd.A_US0PsdSup16 = 1;  /* ADLU-60 */
   sCmd.A_US0PsdSup17 = 1;  /* ADLU-64 */
   sCmd.A_US0PsdSup20 = 1;  /* EU-128 */
   sCmd.A_US0PsdSup21 = 1;  /* ADLU-128 */

   /* Message parameter #8 -  Annex B US0 PSDs Supported Bits */
   sCmd.B_US0PsdSup0 = 1;  /* US0 In 25 to 138 kHz */
   sCmd.B_US0PsdSup1 = 1;  /* US0 In 25 to 276 kHz */
   sCmd.B_US0PsdSup2 = 1;  /* US0 In 120 to 276 kHz */

   /* Message parameter #10 -  Annex B US0 PSDs Supported Bits */
   sCmd.C_US0PsdSup0 = 1; /* US0 In 25 to 138 kHz, Annex C*/
   sCmd.C_US0PsdSup1 = 1; /* US0 In 25 to 276kHz, Annex C */
   sCmd.C_US0PsdSup4 = 1; /* US0 In 25 to 138 kHz, Annex C */
   sCmd.C_US0PsdSup5 = 1; /* US0 In 25 to 276 kHz, Annex C */

   nErrCode = DSL_DRV_VRX_SendMessage( pContext, CMD_HS_VDSL2PROFILECONTROL,
                       sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                       sizeof(sAck), (DSL_uint8_t*)&sAck);

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_SendMsgVdsl2ProfileControl"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgBearerChSet(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint16_t nRate32 = 0, tmpVal_16 = 0, nRateMin = 0, nRateMax = 0;
   DSL_int_t nRateEst = 0;
   DSL_uint8_t nRate4_8 = 0, tmpVal_8 = 0, xtse6 = 0, xtse7 = 0;
   DSL_uint32_t tmpVal_32 = 0;
   DSL_int_t nDlsModeIdx = -1;
   DSL_boolean_t bPafEnable = DSL_FALSE;
   DSL_TcLayerSelection_t nTcMode = DSL_TC_UNKNOWN;

   union
   {
      CMD_BearerCh0_US_Set_t UsCfg;
      CMD_BearerCh0_DS_Set_t DsCfg;
   } sCmd;
   union
   {
      ACK_BearerCh0_US_Set_t UsCfg;
      ACK_BearerCh0_DS_Set_t DsCfg;
   } sAck;
   DSL_uint32_t nMode = 0;

   DSL_DEBUG( DSL_DBG_MSG,
     (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_SendMsgBearerChSet()"
     DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

#ifdef INCLUDE_DSL_BONDING
   /* Bonding is always enabled for both lines/devices together so using the
      configuration from the current line/device is ok. */
   DSL_CTX_READ_SCALAR( pContext, nErrCode, BndConfig.bPafEnable, bPafEnable);
#endif

   /* Programm downstream configuration */

   nDlsModeIdx = DSL_DRV_VRX_DslModeIndexGet(pContext);
   if ((nDlsModeIdx >= 0) && (nDlsModeIdx < DSL_MODE_LAST))
   {
      /* Get System Interface Configuration*/
      DSL_CTX_READ_SCALAR( pContext, nErrCode,
         pDevCtx->data.deviceCfg.sysCIF[nDlsModeIdx].nTcLayer,
         nTcMode);
   }
   else
   {
      return DSL_ERROR;
   }

   /* Backup FW configuration value for TC-Layer that is used for current link
      activation */
   DSL_CTX_WRITE_SCALAR( pContext, nErrCode,
      pDevCtx->data.nTcModeFwCfg, nTcMode);

   /* Common Bearer Channel configuration parameter
      for upstream/downstream. */
   memset(&sCmd, 0, sizeof(sCmd));
   sCmd.DsCfg.Length = DSL_VRX_16BIT_RD_MSG_LEN_GET(sCmd.DsCfg);

   switch (nTcMode)
   {
      case DSL_TC_EFM:
         sCmd.DsCfg.ATMControl = 0;
         sCmd.DsCfg.PTMControl = 1;
         break;
      case DSL_TC_ATM:
         sCmd.DsCfg.ATMControl = 1;
         sCmd.DsCfg.PTMControl = 0;
         break;
      case DSL_TC_AUTO:
         /* In case of bonding is enabled mask out ATM because PAF bonding is
            only supported in PTM (this is also indicated to the user during API
            configuration).
            Reverse logic: Only set ATM bit if bonding is not enabled. */
         if (bPafEnable == DSL_FALSE)
         {
            sCmd.DsCfg.ATMControl = 1;
         }
         sCmd.DsCfg.PTMControl = 1;
         break;
      default:
         return DSL_ERROR;
   }

   DSL_CTX_READ_SCALAR(pContext, nErrCode,
      pDevCtx->data.deviceCfg.ChannelConfigData[DSL_DOWNSTREAM].MaxIntDelay,
      tmpVal_16);

   sCmd.DsCfg.maxDelay0_PTMds = (DSL_uint8_t)tmpVal_16;
   sCmd.DsCfg.maxDelay0_ATMds = (DSL_uint8_t)tmpVal_16;

   DSL_CTX_READ_SCALAR(pContext, nErrCode,
      pDevCtx->data.deviceCfg.ChannelConfigData[DSL_DOWNSTREAM].MinINP,
      tmpVal_8);

   /* special value for 0.5 */
   sCmd.DsCfg.minINP_ATMds = (tmpVal_8 == 1) ? 0x800 : tmpVal_8/2;
   sCmd.DsCfg.minINP_PTMds = (tmpVal_8 == 1) ? 0x800 : tmpVal_8/2;

   if( !DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2) )
   {
      /* ADSL specific limits
         data rates in units of 4kbit */
      nRateMin = DSL_DEV_MIN_DATARATE / 4000;

      DSL_CTX_READ_SCALAR(pContext, nErrCode, xtseCfg[6], xtse7);
      DSL_CTX_READ_SCALAR(pContext, nErrCode, xtseCfg[5], xtse6);

      if( xtse6 || xtse7 )
      {
         nRateMax = DSL_DEV_MAX_DATARATE_DS_ADSL2P / 4000; /* ADSL2+ */
      }
      else
      {
         nRateMax = DSL_DEV_MAX_DATARATE_DS_ADSL2 / 4000; /* ADSL1/2 */
      }
   }
   else
   {
      /* VDSL specific limits
         data rates in units of 4kbit */
      nRateMin = 64 / 4;
      nRateMax = 0xF000 ; /* 246Mbit */
   }

   DSL_CTX_READ_SCALAR(pContext, nErrCode,
      pDevCtx->data.deviceCfg.ChannelConfigData[DSL_DOWNSTREAM].MinDataRate,
      tmpVal_32);

   /* The datarates have to be entered in multiples of 4 kbps */
   DSL_DRV_VRX_GetMsgDataRateValues(tmpVal_32, nMode, &nRate32, &nRate4_8);
   nRateEst = nRate32 * 8 + nRate4_8;

   nErrCode = DSL_DRV_VRX_CheckTruncParamRange(
                    pContext,
                    (DSL_int_t)nRateMin,
                    (DSL_int_t)nRateMax,
                    &nRateEst,
                    "SendMsgSetPsd: MinRate");
   if( nErrCode < DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - DSL_VRX_CheckTruncParamRange!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   sCmd.DsCfg.minRate0_PTMds = (DSL_uint16_t)nRateEst;
   sCmd.DsCfg.minRate0_ATMds = sCmd.DsCfg.minRate0_PTMds;

   DSL_CTX_READ_SCALAR(pContext, nErrCode,
      pDevCtx->data.deviceCfg.ChannelConfigData[DSL_DOWNSTREAM].MaxDataRate,
      tmpVal_32);

   DSL_DRV_VRX_GetMsgDataRateValues(tmpVal_32, nMode, &nRate32, &nRate4_8);
   nRateEst = nRate32 * 8 + nRate4_8;

   nErrCode = DSL_DRV_VRX_CheckTruncParamRange(
                    pContext,
                    (DSL_int_t)nRateMin,
                    (DSL_int_t)nRateMax,
                    &nRateEst,
                    "SendMsgSetPsd: MaxRate");
   if( nErrCode < DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - DSL_VRX_CheckTruncParamRange!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   sCmd.DsCfg.maxRate0_PTMds = (DSL_uint16_t)nRateEst;
   sCmd.DsCfg.maxRate0_ATMds = sCmd.DsCfg.maxRate0_PTMds;

   if (!DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
   {
      /* Select to set ATM or packet parameters */
      sCmd.DsCfg.minResRate0_PTMds = 8;
      sCmd.DsCfg.minResRate0_ATMds = 8;
      sCmd.DsCfg.BER_PTMds = 2;
      sCmd.DsCfg.BER_ATMds = 2;
   }

   nErrCode = DSL_DRV_VRX_SendMessage( pContext, CMD_BEARERCH0_DS_SET,
      sizeof(CMD_BearerCh0_DS_Set_t), (DSL_uint8_t *)&sCmd,
      sizeof(ACK_BearerCh0_DS_Set_t), (DSL_uint8_t *)&sAck);

   /* Programm upstream configuration */

    /* Common Bearer Channel configuration parameter
      for upstream/downstream. */
   memset(&sCmd, 0, sizeof(sCmd));
   sCmd.UsCfg.Length = DSL_VRX_16BIT_RD_MSG_LEN_GET(sCmd.UsCfg);

   /* Select to set ATM or Packet parameters */
   switch (nTcMode)
   {
      case DSL_TC_EFM:
         sCmd.UsCfg.ATMControl = 0;
         sCmd.UsCfg.PTMControl = 1;
         break;
      case DSL_TC_ATM:
         sCmd.UsCfg.ATMControl = 1;
         sCmd.UsCfg.PTMControl = 0;
         break;
      case DSL_TC_AUTO:
         /* In case of bonding is enabled mask out ATM because PAF bonding is
            only supported in PTM (this is also indicated to the user during API
            configuration).
            Reverse logic: Only set ATM bit if bonding is not enabled. */
         if (bPafEnable == DSL_FALSE)
         {
            sCmd.DsCfg.ATMControl = 1;
         }
         sCmd.UsCfg.PTMControl = 1;
         break;
      default:
         return DSL_ERROR;
   }

   DSL_CTX_READ_SCALAR(pContext, nErrCode,
      pDevCtx->data.deviceCfg.ChannelConfigData[DSL_UPSTREAM].MaxIntDelay,
      tmpVal_16);

   sCmd.UsCfg.maxDelay0_PTMus = (DSL_uint8_t)tmpVal_16;
   sCmd.UsCfg.maxDelay0_ATMus = (DSL_uint8_t)tmpVal_16;

   DSL_CTX_READ_SCALAR(pContext, nErrCode,
      pDevCtx->data.deviceCfg.ChannelConfigData[DSL_UPSTREAM].MinINP,tmpVal_8);

   /* special value for 0.5 */
   sCmd.UsCfg.minINP_ATMus = (tmpVal_8 == 1) ? 0x800 : tmpVal_8/2;
   sCmd.UsCfg.minINP_PTMus = (tmpVal_8 == 1) ? 0x800 : tmpVal_8/2;

   DSL_CTX_READ_SCALAR(pContext, nErrCode,
      pDevCtx->data.deviceCfg.ChannelConfigData[DSL_UPSTREAM].MinDataRate,
      tmpVal_32);

   if (!DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
   {
      /* ADSL specific limits
         data rates in units of 4kbit */
      nRateMin = DSL_DEV_MIN_DATARATE / 4000;
      nRateMax = 0x60 * 32 / 4;
   }
   else
   {
      /* VDSL specific limits
         data rates in units of 4kbit */
      nRateMin = 64 / 4;
      nRateMax = 0xF000 ; /* 246Mbit */
   }

   /* The datarates have to be entered in multiples of 4 kbps */
   DSL_DRV_VRX_GetMsgDataRateValues(tmpVal_32, nMode, &nRate32, &nRate4_8);
   nRateEst = nRate32 * 8 + nRate4_8;

   nErrCode = DSL_DRV_VRX_CheckTruncParamRange(
                    pContext,
                    (DSL_int_t)nRateMin,
                    (DSL_int_t)nRateMax,
                    &nRateEst,
                    "SendMsgSetPsd: MinRate");
   if( nErrCode < DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - DSL_VRX_CheckTruncParamRange!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }
   sCmd.UsCfg.minRate0_PTMus = (DSL_uint16_t)nRateEst;
   sCmd.UsCfg.minRate0_ATMus = sCmd.UsCfg.minRate0_PTMus;

   DSL_CTX_READ_SCALAR(pContext, nErrCode,
      pDevCtx->data.deviceCfg.ChannelConfigData[DSL_UPSTREAM].MaxDataRate,
      tmpVal_32);

   DSL_DRV_VRX_GetMsgDataRateValues(tmpVal_32, nMode, &nRate32, &nRate4_8);
   nRateEst = nRate32 * 8 + nRate4_8;

   nErrCode = DSL_DRV_VRX_CheckTruncParamRange(
                    pContext,
                    (DSL_int_t)nRateMin,
                    (DSL_int_t)nRateMax,
                    &nRateEst,
                    "SendMsgSetPsd: MaxRate");
   if( nErrCode < DSL_SUCCESS )
   {
      DSL_DEBUG( DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - DSL_VRX_CheckTruncParamRange!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

      return nErrCode;
   }

   sCmd.UsCfg.maxRate0_PTMus = (DSL_uint16_t)nRateEst;
   sCmd.UsCfg.maxRate0_ATMus = sCmd.UsCfg.maxRate0_PTMus;

   if (!DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
   {
      /* Select to set ATM or packet parameters */
      sCmd.UsCfg.minResRate0_PTMus = 8;
      sCmd.UsCfg.minResRate0_ATMus = 8;
      sCmd.UsCfg.BER_PTMus = 2;
      sCmd.UsCfg.BER_ATMus = 2;
   }


   nErrCode = DSL_DRV_VRX_SendMessage( pContext, CMD_BEARERCH0_US_SET,
      sizeof(CMD_BearerCh0_US_Set_t), (DSL_uint8_t *)&sCmd,
      sizeof(ACK_BearerCh0_US_Set_t), (DSL_uint8_t *)&sAck);

   DSL_DEBUG( DSL_DBG_MSG,
     (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_SendMsgBearerChSet()"
     DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgBearerChTcLayerSet(
   DSL_Context_t *pContext)
{
   DSL_int_t nDlsModeIdx = -1;
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_TcLayerSelection_t nTcMode = DSL_TC_UNKNOWN;

   union
   {
      CMD_BearerCh0_US_Set_t UsCfg;
      CMD_BearerCh0_DS_Set_t DsCfg;
   } sCmd;
   union
   {
      ACK_BearerCh0_US_Set_t UsCfg;
      ACK_BearerCh0_DS_Set_t DsCfg;
   } sAck;

   DSL_DEBUG( DSL_DBG_MSG,
     (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_SendMsgBearerChTcLayerSet()"
     DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Programm downstream configuration */

   /* Common Bearer Channel configuration parameter
      for upstream/downstream. */
   memset(&sCmd, 0, sizeof(sCmd));
   sCmd.DsCfg.Length = 1;

   nDlsModeIdx = DSL_DRV_VRX_DslModeIndexGet(pContext);
   if ((nDlsModeIdx >= 0) && (nDlsModeIdx < DSL_MODE_LAST))
   {
      /* Get System Interface Configuration*/
      DSL_CTX_READ_SCALAR( pContext, nErrCode,
         pDevCtx->data.deviceCfg.sysCIF[nDlsModeIdx].nTcLayer,
         nTcMode);
   }
   else
   {
      return DSL_ERROR;
   }

   /* Backup FW configuration value for TC-Layer that is used for current link
      activation */
   DSL_CTX_WRITE_SCALAR( pContext, nErrCode,
      pDevCtx->data.nTcModeFwCfg, nTcMode);

   switch (nTcMode)
   {
      case DSL_TC_EFM:
         sCmd.DsCfg.ATMControl = 0;
         sCmd.DsCfg.PTMControl = 1;
         break;
      case DSL_TC_ATM:
         sCmd.DsCfg.ATMControl = 1;
         sCmd.DsCfg.PTMControl = 0;
         break;
      case DSL_TC_AUTO:
         sCmd.DsCfg.ATMControl = 1;
         sCmd.DsCfg.PTMControl = 1;
         break;
      default:
         return DSL_ERROR;
   }

   nErrCode = DSL_DRV_VRX_SendMessage( pContext, CMD_BEARERCH0_DS_SET,
      6, (DSL_uint8_t *)&sCmd,
      sizeof(ACK_BearerCh0_DS_Set_t), (DSL_uint8_t *)&sAck);

   /* Programm upstream configuration */

    /* Common Bearer Channel configuration parameter
      for upstream/downstream. */
   memset(&sCmd, 0, sizeof(sCmd));
   sCmd.UsCfg.Length = 1;
   /* Select to set ATM or Packet parameters */
   switch (nTcMode)
   {
      case DSL_TC_EFM:
         sCmd.UsCfg.ATMControl = 0;
         sCmd.UsCfg.PTMControl = 1;
         break;
      case DSL_TC_ATM:
         sCmd.UsCfg.ATMControl = 1;
         sCmd.UsCfg.PTMControl = 0;
         break;
      case DSL_TC_AUTO:
         sCmd.DsCfg.ATMControl = 1;
         sCmd.UsCfg.PTMControl = 1;
         break;
      default:
         return DSL_ERROR;
   }

   nErrCode = DSL_DRV_VRX_SendMessage( pContext, CMD_BEARERCH0_US_SET,
      6, (DSL_uint8_t *)&sCmd,
      sizeof(ACK_BearerCh0_US_Set_t), (DSL_uint8_t *)&sAck);

   DSL_DEBUG( DSL_DBG_MSG,
     (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_SendMsgBearerChSet()"
     DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgOlrStatusGet(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDirection,
   DSL_uint8_t *pAck)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   /* Assume that US/DS messages have same format */
   CMD_OLR_US_StatsGet_t sCmd;
   union
   {
      ACK_OLR_US_StatsGet_t US;
      ACK_OLR_DS_StatsGet_t DS;
   } sAck;

   /* fill up the message to be sent */
   memset(&sCmd, 0, sizeof(sCmd));

   sCmd.Length = 15;

   nErrCode =  DSL_DRV_VRX_SendMessage( pContext,
                    nDirection == DSL_DOWNSTREAM ? CMD_OLR_DS_STATSGET :
                                                   CMD_OLR_US_STATSGET,
                    sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                    sizeof(sAck), (DSL_uint8_t*)&sAck );

   /* Copy data only if successful */
   if (nErrCode >= 0)
   {
      memcpy(pAck, &(sAck.US), sizeof(ACK_OLR_US_StatsGet_t));
   }

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgBearerChannelStatusGet(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDirection,
   DSL_uint8_t *pAck)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   /* Assume that US/DS messages have same format */
   CMD_BearerChsUS_Get_t sCmd;
   union
   {
      ACK_BearerChsUS_Get_t US;
      ACK_BearerChsDS_Get_t DS;
   } sAck;

   DSL_CHECK_CTX_POINTER(pContext);
   DSL_CHECK_ERR_CODE();

   /* fill up the message to be sent */
   memset(&sCmd, 0, sizeof(sCmd));

   sCmd.Length = DSL_VRX_16BIT_RD_MSG_LEN_GET(sAck);

   nErrCode =  DSL_DRV_VRX_SendMessage( pContext,
                    nDirection == DSL_DOWNSTREAM ? CMD_BEARERCHSDS_GET :
                                                   CMD_BEARERCHSUS_GET,
                    sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                    sizeof(sAck), (DSL_uint8_t*)&sAck );

   /* Copy data only if successful */
   if (nErrCode >= 0)
   {
      memcpy(pAck, &(sAck.US), sizeof(ACK_BearerChsUS_Get_t));
   }

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgTxL3RequestFailReasonGet(
   DSL_Context_t *pContext,
   DSL_VRX_L3RequestFailReason_t *pFailReason)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   CMD_TxL3ReqFailReasonGet_t sCmd;
   ACK_TxL3ReqFailReasonGet_t sAck;

   DSL_CHECK_POINTER(pContext, pFailReason);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG,(pContext,
      SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_SendMsgTxL3RequestFailReasonGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSL))
   {
      memset(&sCmd, 0, sizeof(sCmd));
      sCmd.Length = 0x1;

      nErrCode = DSL_DRV_VRX_SendMessage(
                    pContext, CMD_TXL3REQFAILREASONGET,
                    sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                    sizeof(sAck), (DSL_uint8_t*)&sAck);

      if (nErrCode == DSL_SUCCESS)
      {
         switch (sAck.L3FailReason)
         {
         case ACK_TxL3ReqFailReasonGet_NOT_L0:
            *pFailReason = DSL_VRX_L3_FAIL_REASON_NOT_L0;
            break;
         case ACK_TxL3ReqFailReasonGet_TIMEOUT:
            *pFailReason = DSL_VRX_L3_FAIL_REASON_TIMEOUT;
            break;
         default:
            *pFailReason = DSL_VRX_L3_FAIL_REASON_NA;
            DSL_DEBUG( DSL_DBG_ERR,
               (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Unknown L3 "
               "fail reason status (%d) received!"DSL_DRV_CRLF,
               DSL_DEV_NUM(pContext), sAck.L3FailReason));
            break;
         }
      }
      else
      {
         *pFailReason = DSL_VRX_L3_FAIL_REASON_NA;
      }
   }
   else
   {
      DSL_DEBUG( DSL_DBG_ERR,(pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - L3 mode not supported by the FW"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      nErrCode = DSL_ERR_NOT_SUPPORTED_BY_FIRMWARE;
   }


   DSL_DEBUG( DSL_DBG_MSG,(pContext,
      SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_SendMsgTxL3RequestFailReasonGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgTxL3RequestStatusGet(
   DSL_Context_t *pContext,
   DSL_VRX_L3RequestStatus_t *pStatus)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   CMD_TxL3RequestStatusGet_t sCmd;
   ACK_TxL3RequestStatusGet_t sAck;

   DSL_CHECK_POINTER(pContext, pStatus);
   DSL_CHECK_ERR_CODE();

   DSL_DEBUG( DSL_DBG_MSG,(pContext,
      SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_SendMsgTxL3RequestStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSL))
   {
      memset(&sCmd, 0, sizeof(sCmd));
      sCmd.Length = 0x1;

      nErrCode = DSL_DRV_VRX_SendMessage(
                    pContext, CMD_TXL3REQUESTSTATUSGET,
                    sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                    sizeof(sAck), (DSL_uint8_t*)&sAck);

      if (nErrCode == DSL_SUCCESS)
      {
         switch (sAck.L3ReqStatus)
         {
         case ACK_TxL3RequestStatusGet_L3PENDING:
            *pStatus = DSL_VRX_L3_STATUS_PENDING;
            break;
         case ACK_TxL3RequestStatusGet_L3REJECTED:
            *pStatus = DSL_VRX_L3_STATUS_REJECTED;
            break;
         case ACK_TxL3RequestStatusGet_L3ACCEPTED:
            *pStatus = DSL_VRX_L3_STATUS_ACCEPTED;
            break;
         case ACK_TxL3RequestStatusGet_L3FAIL:
            *pStatus = DSL_VRX_L3_STATUS_FAIL;
            break;
         default:
            *pStatus = DSL_VRX_L3_STATUS_NA;
            DSL_DEBUG( DSL_DBG_ERR,(pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - Unknown L3 request status (%d) received!"
               DSL_DRV_CRLF, DSL_DEV_NUM(pContext), sAck.L3ReqStatus));
            break;
         }
      }
      else
      {
         *pStatus = DSL_VRX_L3_STATUS_NA;
      }
   }
   else
   {
      DSL_DEBUG( DSL_DBG_ERR,(pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - L3 mode not supported by the FW"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      nErrCode = DSL_ERR_NOT_SUPPORTED_BY_FIRMWARE;
   }


   DSL_DEBUG( DSL_DBG_MSG,(pContext,
      SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_SendMsgTxL3RequestStatusGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgShutdownRequest(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   CMD_ShutdownRequest_t sCmd;
   ACK_ShutdownRequest_t sAck;

   DSL_DEBUG( DSL_DBG_MSG,(pContext,
      SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_SendMsgShutdownRequest"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSLA))
   {
      memset(&sCmd, 0, sizeof(sCmd));
      sCmd.Length = 0x1;
      sCmd.L3shutdown = VRX_ENABLE;

      nErrCode = DSL_DRV_VRX_SendMessage(
                    pContext, CMD_SHUTDOWNREQUEST,
                    sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                    sizeof(sAck), (DSL_uint8_t*)&sAck);
   }
   else
   {
      DSL_DEBUG( DSL_DBG_ERR,(pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - L3 mode not supported by the FW"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      nErrCode = DSL_ERR_NOT_SUPPORTED_BY_FIRMWARE;
   }

   DSL_DEBUG( DSL_DBG_MSG,(pContext,
      SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_SendMsgShutdownRequest"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgBearerChsDsRtxGet(
   DSL_Context_t *pContext,
   DSL_uint8_t *pAck)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint32_t   nMsgId;
   CMD_BearerChsDS_RTX_Get_t sCmd;
   ACK_BearerChsDS_RTX_Get_t sAck;
   DSL_DEV_VersionCheck_t nVerCheck = DSL_VERSION_ERROR;

   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
   {
      /* Get FW information */
      nErrCode = DSL_DRV_VRX_FirmwareVersionCheck(pContext,
                  DSL_MIN_FW_VERSION_RETX_VDSL, &nVerCheck);
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
                  DSL_MIN_FW_VERSION_RETX_ADSL, &nVerCheck);
      if (nErrCode != DSL_SUCCESS)
      {
         DSL_DEBUG(DSL_DBG_ERR,
            (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW version check failed!"
            DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
         return nErrCode;
      }
   }

   if (nVerCheck >= DSL_VERSION_EQUAL)
   {
      /* fill up the message to be sent */
      memset(&sCmd, 0, sizeof(sCmd));

      nMsgId = CMD_BEARERCHSDS_RTX_GET;
      sCmd.Length = 5;

      nErrCode = DSL_DRV_VRX_SendMessage( pContext, nMsgId,
         sizeof(CMD_BearerChsDS_RTX_Get_t), (DSL_uint8_t*)&sCmd,
         sizeof(ACK_BearerChsDS_RTX_Get_t), (DSL_uint8_t*)&sAck );

      /* Copy data only if successful */
      if (nErrCode >= DSL_SUCCESS)
      {
         memcpy(pAck,&sAck, sizeof(ACK_BearerChsDS_RTX_Get_t));
      }
   }
   else
   {
      DSL_DEBUG(DSL_DBG_WRN, (pContext,
         SYS_DBG_WRN"DSL[%02d]: WARNING - ReTx not supported by the FW!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgRtxDsEnableStatusGet(
   DSL_Context_t *pContext,
   DSL_uint8_t *pAck)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint32_t   nMsgId;
   CMD_RTX_StatusGet_t sCmd;
   ACK_RTX_StatusGet_t sAck;

   /* fill up the message to be sent */
   memset(&sCmd, 0, sizeof(sCmd));

   nMsgId = CMD_RTX_STATUSGET;
   sCmd.Index = 0x0;
   sCmd.Length = 1;

   nErrCode = DSL_DRV_VRX_SendMessage( pContext, nMsgId,
      sizeof(CMD_RTX_StatusGet_t), (DSL_uint8_t*)&sCmd,
      sizeof(ACK_RTX_StatusGet_t), (DSL_uint8_t*)&sAck );

   /* Copy data only if successful */
   if (nErrCode >= DSL_SUCCESS)
   {
      memcpy(pAck,&sAck, sizeof(ACK_RTX_StatusGet_t));
   }

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgRtxDsStatsGet(
   DSL_Context_t *pContext,
   DSL_uint8_t *pAck)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint32_t   nMsgId;
   CMD_RTX_DS_StatsGet_t sCmd;
   ACK_RTX_DS_StatsGet_t sAck;

   /* fill up the message to be sent */
   memset(&sCmd, 0, sizeof(sCmd));

   nMsgId = CMD_RTX_DS_STATSGET;
   sCmd.Length = 6;

   nErrCode = DSL_DRV_VRX_SendMessage( pContext, nMsgId,
      sizeof(CMD_RTX_DS_StatsGet_t), (DSL_uint8_t*)&sCmd,
      sizeof(ACK_RTX_DS_StatsGet_t), (DSL_uint8_t*)&sAck );

   /* Copy data only if successful */
   if (nErrCode >= DSL_SUCCESS)
   {
      memcpy(pAck,&sAck, sizeof(ACK_RTX_DS_StatsGet_t));
   }

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgRtxPmDsGet(
   DSL_Context_t *pContext,
   DSL_uint8_t *pAck)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint32_t   nMsgId;
   CMD_RTX_PM_DS_Get_t sCmd;
   ACK_RTX_PM_DS_Get_t sAck;

   /* fill up the message to be sent */
   memset(&sCmd, 0, sizeof(sCmd));

   nMsgId = CMD_RTX_PM_DS_GET;
   sCmd.Length = 12;

   nErrCode = DSL_DRV_VRX_SendMessage( pContext, nMsgId,
      sizeof(CMD_RTX_PM_DS_Get_t), (DSL_uint8_t*)&sCmd,
      sizeof(ACK_RTX_PM_DS_Get_t), (DSL_uint8_t*)&sAck );

   /* Copy data only if successful */
   if (nErrCode >= DSL_SUCCESS)
   {
      memcpy(pAck,&sAck, sizeof(ACK_RTX_PM_DS_Get_t));
   }

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgLineStatusGet(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDirection,
   DSL_uint8_t *pAck)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   DSL_uint32_t   nMsgId;
   CMD_LineStatusUS_Get_t sCmd;
   ACK_LineStatusUS_Get_t sAck;

   /* fill up the message to be sent */
   memset(&sCmd, 0, sizeof(sCmd));

   nMsgId = nDirection == DSL_DOWNSTREAM ? CMD_LINESTATUSDS_GET :
                                           CMD_LINESTATUSUS_GET;

   /*
      Starting with VDSL-FW version w.8.x.y.z.a another16-Bit word was appended
      (--> Length=9). The same will be done in future ADSL releases (check release
      notes). Older VDSL FW-versions and all current ADSL FW-versions can
      request only 8 parameters (Length=8).
   */
   sCmd.Length = 9;

   nErrCode = DSL_DRV_VRX_SendMessage( pContext, nMsgId,
      sizeof(CMD_LineStatusUS_Get_t), (DSL_uint8_t*)&sCmd,
      sizeof(ACK_LineStatusUS_Get_t), (DSL_uint8_t*)&sAck );

   /* Copy data only if successful */
   if (nErrCode >= DSL_SUCCESS)
   {
      memcpy(pAck,&sAck, sizeof(ACK_LineStatusUS_Get_t));
   }

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgAdslFrameDataGet(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDir,
   DSL_uint8_t *pAck)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   /* Structures for the DS and US directions are the same.
       So we can use any for CMD and ACK*/
   CMD_ADSL_FrameDataDS_LP0Get_t sCmd;
   ACK_ADSL_FrameDataDS_LP0Get_t sAck;
   DSL_boolean_t bAdsl1 = DSL_FALSE;
   DSL_uint32_t nAdsl1Mode = 0;
   DSL_uint32_t nMsgId = 0;
   DSL_uint8_t lp = 0;

   DSL_CTX_READ_SCALAR(pContext, nErrCode, xtseCurr[0], nAdsl1Mode);

   /* Check for the ADSL1 mode*/
   if ( (nAdsl1Mode & XTSE_1_03_A_1_NO) || (nAdsl1Mode & XTSE_1_05_B_1_NO))
   {
      bAdsl1 = DSL_TRUE;
   }

   /* loop over the latency paths */
   for (lp = 0 ; lp < 2; lp++)
   {
      /* each latency path needs to be handled differently */
      if (lp == 0)
      {
         nMsgId = (nDir == DSL_UPSTREAM) ? CMD_ADSL_FRAMEDATAUS_LP0GET : CMD_ADSL_FRAMEDATADS_LP0GET;
      }
      else
      {
         /* LP1 may be used for ADSL1 only */
         if (!bAdsl1)
            break;

         nMsgId = (nDir == DSL_UPSTREAM) ? CMD_ADSL_FRAMEDATAUS_LP1GET : CMD_ADSL_FRAMEDATADS_LP1GET;
      }

      memset(&sCmd, 0, sizeof(sCmd));
      memset(&sAck, 0, sizeof(sAck));

      sCmd.Length = DSL_VRX_16BIT_RD_MSG_LEN_GET(sAck);

      nErrCode = DSL_DRV_VRX_SendMessage(pContext, nMsgId,
                    sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                    sizeof(sAck), (DSL_uint8_t*)&sAck);

      /* if the current latency path seems not to carry proper data
         try the next */
      if (sAck.Lp == 0)
      {
         continue;
      }
      else
      {
         break;
      }
   }

   /* Copy data only if successful */
   if (nErrCode >= DSL_SUCCESS)
   {
      memcpy(pAck, &sAck, sizeof(ACK_ADSL_FrameDataDS_LP0Get_t));
   }

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgFrameDataExt2Get(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDirection,
   DSL_uint8_t *pAck)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   /* Structures for the DS and US directions are the same.
       So we can use any for CMD and ACK*/
   CMD_FrameDataExt2US_Get_t sCmd;
   ACK_FrameDataExt2US_Get_t sAck;

   DSL_CHECK_CTX_POINTER(pContext);
   DSL_CHECK_ERR_CODE();

   /* fill up the message to be sent */
   memset(&sCmd, 0, sizeof(sCmd));

   sCmd.Length = DSL_VRX_16BIT_RD_MSG_LEN_GET(sAck);

   nErrCode = DSL_DRV_VRX_SendMessage( pContext,
                  nDirection == DSL_DOWNSTREAM ? CMD_FRAMEDATAEXT2DS_GET :
                                                 CMD_FRAMEDATAEXT2US_GET,
                  sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                  sizeof(sAck), (DSL_uint8_t*)&sAck );

   /* Copy data only if successful */
   if (nErrCode >= DSL_SUCCESS)
   {
      memcpy(pAck, &sAck, sizeof(ACK_FrameDataExt2US_Get_t));
   }

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgReInitNeConfigure(
   DSL_Context_t *pContext,
   const DSL_boolean_t bEnableRestart,
   DSL_BF_RebootCriteriaConfigData_t nReInitNeConfigure)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   CMD_ReInitNE_Configure_t sCmd;
   ACK_ReInitNE_Configure_t sAck;
   DSL_uint16_t cmdByteLength = 0;

   memset(&sCmd, 0, sizeof(sCmd));

   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSL))
   {
      sCmd.Length = 2;
      cmdByteLength = sizeof(sCmd);
   }
   else {
      /* Ignore last parameter (ADSL only)*/
      sCmd.Length = 1;
      cmdByteLength = sizeof(sCmd) - sizeof(DSL_uint16_t);
   }

   if (bEnableRestart)
   {
      sCmd.E0  = (nReInitNeConfigure & DSL_REBOOT_CRITERIA_LOS) ? VRX_ENABLE : VRX_DISABLE;
      sCmd.E1  = (nReInitNeConfigure & DSL_REBOOT_CRITERIA_LOF) ? VRX_ENABLE : VRX_DISABLE;
      sCmd.E3  = (nReInitNeConfigure & DSL_REBOOT_CRITERIA_LOM) ? VRX_ENABLE : VRX_DISABLE;
      sCmd.E10 = (nReInitNeConfigure & DSL_REBOOT_CRITERIA_OOS_BC0) ? VRX_ENABLE : VRX_DISABLE;
      sCmd.E13 = (nReInitNeConfigure & DSL_REBOOT_CRITERIA_ESE) ? VRX_ENABLE : VRX_DISABLE;
      sCmd.E8  = (nReInitNeConfigure & DSL_REBOOT_CRITERIA_NCD_BC0) ? VRX_ENABLE : VRX_DISABLE;
      sCmd.E6  = (nReInitNeConfigure & DSL_REBOOT_CRITERIA_LCD_BC0) ? VRX_ENABLE : VRX_DISABLE;

      /* ADSL only*/
      if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSL))
      {
         sCmd.E14  = (nReInitNeConfigure & DSL_REBOOT_CRITERIA_NEGATIVE_MARGIN) ? VRX_ENABLE : VRX_DISABLE;
         sCmd.F1  = (nReInitNeConfigure & DSL_REBOOT_CRITERIA_ES90) ? VRX_ENABLE : VRX_DISABLE;
         sCmd.F0  = (nReInitNeConfigure & DSL_REBOOT_CRITERIA_SES30) ? VRX_ENABLE : VRX_DISABLE;
      }
   }

   nErrCode = DSL_DRV_VRX_SendMessage( pContext, CMD_REINITNE_CONFIGURE,
                  cmdByteLength, (DSL_uint8_t*)&sCmd,
                  sizeof(sAck), (DSL_uint8_t*)&sAck );

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendOperationOptionsSet(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   CMD_OperationOptionsSet_t sCmd;
   ACK_OperationOptionsSet_t sAck;
   DSL_boolean_t bTmpVal = DSL_FALSE;
   DSL_int32_t nUsEnhFraming = 0;
   DSL_DEV_VersionCheck_t nVerCheck;

   memset(&sCmd, 0, sizeof(sCmd));

   sCmd.Length = 0x1;
   sCmd.Index  = 0x0;

   /* ------- Process FW version related MFD configuration ------- */
   nErrCode = DSL_DRV_VRX_FirmwareVersionCheck(pContext,
               DSL_MIN_FW_VERSION_MFD, &nVerCheck);
   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW version check failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return nErrCode;
   }

   /* Currently this message is only supported for ADSL AnnexA */
   if ((DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_ADSLA)) &&
       (nVerCheck >= DSL_VERSION_EQUAL))
   {
      sCmd.Mfd = VRX_ENABLE;
   }

   /* ------- Process FW version related NTR configuration ------- */
   nErrCode = DSL_DRV_VRX_FirmwareVersionCheck(pContext,
               DSL_MIN_FW_VERSION_NTR, &nVerCheck);
   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG(DSL_DBG_ERR,
         (pContext, SYS_DBG_ERR"DSL[%02d]: ERROR - FW version check failed!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return nErrCode;
   }

   /* Currently this message is only supported for VDSL */
   if ((DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2)) &&
       (nVerCheck >= DSL_VERSION_EQUAL))
   {
      /* Get NTR setting from device (low level config) context */
      DSL_CTX_READ(pContext, nErrCode,
         pDevCtx->data.deviceCfg.cfg.bNtrEnable, bTmpVal);

      /* NTR (VDSL only)*/
      sCmd.Ntr = bTmpVal ? VRX_ENABLE : VRX_DISABLE;
   }
   else
   {
      sCmd.Ntr = VRX_DISABLE;
   }

   /* Get configuration for the Upstream enhanced framing */
   DSL_CTX_READ_SCALAR(pContext, nErrCode,
      lineOptionsConfig[DSL_ENHANCED_FRAMING_US], nUsEnhFraming);

   sCmd.UsFramingExt = (nUsEnhFraming == 1) ? VRX_ENABLE : VRX_DISABLE;

   nErrCode = DSL_DRV_VRX_SendMessage( pContext, CMD_OPERATIONOPTIONSSET,
                  sizeof(CMD_OperationOptionsSet_t), (DSL_uint8_t *)&sCmd,
                  sizeof(ACK_OperationOptionsSet_t), (DSL_uint8_t *)&sAck);

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgAlgorithmControl(
   DSL_Context_t *pContext,
   const DSL_uint16_t nInitWord,
   const DSL_uint16_t nSteadyWord)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   CMD_AlgorithmControlSet_t sCmd;
   ACK_AlgorithmControlSet_t sAck;
   DSL_boolean_t bTmpVal = DSL_FALSE;

   /* in this message the bits are negated: enable = 0
      set by default anything to 1 therefore */
   memset(&sCmd, 0xFF, sizeof(sCmd));

   sCmd.Length = 0x1;
   sCmd.Index  = 0x0;

   /* Set xDSL mode specific values*/
   if (DSL_DRV_VRX_FirmwareXdslModeCheck(pContext, DSL_VRX_FW_VDSL2))
   {
      sCmd.SAC10 = VRX_ENABLE_N;  /* Tone Reordering Table: default ON */
   }

   sCmd.SAC8 = VRX_ENABLE_N;   /* Steady-State Tx PSD Scaling: default ON */
   sCmd.SAC6 = VRX_ENABLE_N;   /* Steady-State SNR:            default ON */
   sCmd.SAC4 = VRX_ENABLE_N;   /* SFDQ Training:               default ON */

   /* Trellis Coding Tx  */
   DSL_CTX_READ_SCALAR(pContext, nErrCode,
      lineFeatureDataCfg[DSL_UPSTREAM].bTrellisEnable,
      bTmpVal);
   sCmd.SAC1 = bTmpVal ? VRX_ENABLE_N : VRX_DISABLE_N;

   nErrCode = DSL_DRV_VRX_SendMessage( pContext, CMD_ALGORITHMCONTROLSET,
                  sizeof(CMD_AlgorithmControlSet_t), (DSL_uint8_t *)&sCmd,
                  sizeof(ACK_AlgorithmControlSet_t), (DSL_uint8_t *)&sAck);

   return (nErrCode);
}

/**
   Handling for automatic selection of G.HS tones excludes V43 by default as
   described within DSLCPE_SW-736. Hence this preprocessor macro needs to be
   undefined.
   Previous implementation can be activated if this definition is set.
*/
#undef VRX_GHS_AUTO_INCLUDE_V43

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgHsToneGroupSet(
   DSL_Context_t *pContext,
   const DSL_uint8_t *pXtseAdslA,
   const DSL_uint8_t *pXtseAdslB,
   const DSL_uint8_t *pXtseVdsl2)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   CMD_HS_ToneGroupSet_t sCmd;
   ACK_HS_ToneGroupSet_t sAck;
   DSL_uint16_t nToneSet = 0;
   DSL_FirmwareXdslMode_t nFwXdslModes = DSL_FW_XDSLMODE_CLEANED;
   DSL_DEV_HsToneGroupMode_t nHsToneGroupMode = DSL_DEV_HS_TONE_GROUP_MODE_NA;
   DSL_DEV_HsToneGroup_t nHsToneGroup = DSL_DEV_HS_TONE_GROUP_CLEANED;
   DSL_DEV_HsToneGroup_t nHsToneGroup_A = DSL_DEV_HS_TONE_GROUP_CLEANED;
   DSL_DEV_HsToneGroup_t nHsToneGroup_V = DSL_DEV_HS_TONE_GROUP_CLEANED;
   DSL_uint8_t i, nXtseA = 0, nXtseB = 0, nXtseV = 0, nXtseCurr = 0;
   /*DSL_uint8_t *pXtseA = DSL_NULL, *pXtseB = DSL_NULL, *pXtseV = DSL_NULL;*/
   DSL_DEV_CamStates_t nCamState = DSL_CAM_DISABLED;

   DSL_DEBUG( DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_SendMsgHsToneGroupSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   /* Get mode for handshake tone configuration */
   DSL_CTX_READ(pContext, nErrCode,
      pDevCtx->data.deviceCfg.cfg.nHsToneGroupMode, nHsToneGroupMode);

   /* Get user configuration for ADSL only HS tones */
   DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.deviceCfg.cfg.nHsToneGroup_A,
      nHsToneGroup_A);

   /* Get user configuration for VDSL only HS tones */
   DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.deviceCfg.cfg.nHsToneGroup_V,
      nHsToneGroup_V);

   /* Get FW capabilities */
   DSL_CTX_READ(pContext, nErrCode, nFwFeatures.nFirmwareXdslModes, nFwXdslModes);

   /* Get CAM state information*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, pDevCtx->data.nCamState, nCamState);

   /* Get ADSL AnnexA, ADSL AnnexB and VDSL2 bits */
   for (i = 0; i < DSL_G997_NUM_XTSE_OCTETS; i++)
   {
      DSL_CTX_READ_SCALAR(pContext, nErrCode, xtseCfg[i], nXtseCurr);
      nXtseA |= (nXtseCurr & *(pXtseAdslA+i));
      nXtseB |= (nXtseCurr & *(pXtseAdslB+i));
      nXtseV |= (nXtseCurr & *(pXtseVdsl2+i));
   }

   if (nHsToneGroupMode == DSL_DEV_HS_TONE_GROUP_MODE_AUTO)
   {
      if (nFwXdslModes & DSL_FW_XDSLMODE_ADSL_A)
      {
         /* AnnexA (Hybrid type A) FW used */

         nHsToneGroup |= DSL_DEV_HS_TONE_GROUP_VDSL2_A43;
         nHsToneGroup |= DSL_DEV_HS_TONE_GROUP_ADSL2_A43C;

#ifdef VRX_GHS_AUTO_INCLUDE_V43
         /* VDSL only or ADSL multimode */
         switch (nCamState)
         {
         case DSL_CAM_DISABLED:
            if ((nXtseA == 0) && (nXtseV != 0))
            {
               nHsToneGroup |= DSL_DEV_HS_TONE_GROUP_VDSL2_V43;
            }
            break;
         case DSL_CAM_VDSL_SINGLE:
         case DSL_CAM_VDSL_DUAL:
            nHsToneGroup |= DSL_DEV_HS_TONE_GROUP_VDSL2_V43;
            break;
         default:
            /* Nothing to do here */
            break;
         }
#endif /* VRX_GHS_AUTO_INCLUDE_V43 */
      }
      else
      {
         /* AnnexB (Hybrid type B/BJ) FW used OR
            No Hybrid type detected (probably a VDSL only FW) */

         nHsToneGroup |= DSL_DEV_HS_TONE_GROUP_VDSL2_B43;
         nHsToneGroup |= DSL_DEV_HS_TONE_GROUP_ADSL2_B43C;

#ifdef VRX_GHS_AUTO_INCLUDE_V43
         switch (nCamState)
         {
         case DSL_CAM_DISABLED:
            if ((nXtseB == 0) && (nXtseV != 0))
            {
               /* VDSL only or ADSL multimode */
               nHsToneGroup |= DSL_DEV_HS_TONE_GROUP_VDSL2_V43;
            }
            break;
         case DSL_CAM_VDSL_SINGLE:
         case DSL_CAM_VDSL_DUAL:
            nHsToneGroup |= DSL_DEV_HS_TONE_GROUP_VDSL2_V43;
            break;
         default:
            /* Nothing to do here */
            break;
         }
#endif /* VRX_GHS_AUTO_INCLUDE_V43 */
      }
   }
   else
   {
      switch (nCamState)
      {
      case DSL_CAM_INIT:
      case DSL_CAM_DISABLED:
         if ((nXtseA == 0) && (nXtseB == 0) && (nXtseV != 0))
         {
            nHsToneGroup = nHsToneGroup_V;
         }
         else
         {
            nHsToneGroup = nHsToneGroup_A;
         }
         break;
      case DSL_CAM_VDSL_SINGLE:
      case DSL_CAM_VDSL_DUAL:
         nHsToneGroup = nHsToneGroup_V;
         break;
      case DSL_CAM_ADSL_SINGLE:
      default:
         nHsToneGroup = nHsToneGroup_A;
      }
   }

   memset(&sCmd, 0, sizeof(sCmd));
   sCmd.Length = DSL_VRX_16BIT_RD_MSG_LEN_GET(sCmd);

   if(nHsToneGroup & DSL_DEV_HS_TONE_GROUP_VDSL2_B43)  nToneSet |= 0x0004;
   if(nHsToneGroup & DSL_DEV_HS_TONE_GROUP_VDSL2_A43)  nToneSet |= 0x0001;
   if(nHsToneGroup & DSL_DEV_HS_TONE_GROUP_VDSL2_V43)  nToneSet |= 0x0040;
   if(nHsToneGroup & DSL_DEV_HS_TONE_GROUP_VDSL1_V43P) nToneSet |= 0x0080;
   if(nHsToneGroup & DSL_DEV_HS_TONE_GROUP_VDSL1_V43I) nToneSet |= 0x0100;
   if(nHsToneGroup & DSL_DEV_HS_TONE_GROUP_ADSL1_C43)  nToneSet |= 0x0010;
   if(nHsToneGroup & DSL_DEV_HS_TONE_GROUP_ADSL2_J43)  nToneSet |= 0x0020;
   if(nHsToneGroup & DSL_DEV_HS_TONE_GROUP_ADSL2_B43C) nToneSet |= 0x0008;
   if(nHsToneGroup & DSL_DEV_HS_TONE_GROUP_ADSL2_A43C) nToneSet |= 0x0002;

   memcpy(((DSL_uint16_t*)&sCmd)+2, &nToneSet, 2);

   nErrCode = DSL_DRV_VRX_SendMessage( pContext, CMD_HS_TONEGROUPSET,
                  sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                  sizeof(ACK_HS_ToneGroupSet_t), (DSL_uint8_t*)&sAck );

   DSL_DEBUG(DSL_DBG_MSG,
      (pContext, SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_SendMsgHsToneGroupSet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgFwVerInfoGet(
    DSL_Context_t *pContext,
    DSL_uint32_t *pVerNum)
{
   CMD_VersionInfoGet_t  sCmd = {0, 3};
   ACK_VersionInfoGet_t  sAck;
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_DEBUG( DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_SendMsgFwVerInfoGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   nErrCode = DSL_DRV_VRX_SendMessage(pContext,
      (DSL_uint16_t)CMD_VERSIONINFOGET,
      sizeof(sCmd), (DSL_uint8_t*)&sCmd,
      sizeof(ACK_VersionInfoGet_t), (DSL_uint8_t*)&sAck);

   if(nErrCode == DSL_SUCCESS)
   {
      *pVerNum = (DSL_uint32_t)sAck.FW_Version;
   }
   else
   {
      DSL_DEBUG(DSL_DBG_ERR,(pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - Failed to retrieve FW Version!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }

   DSL_DEBUG(DSL_DBG_MSG,(pContext,
      SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_SendMsgFwVerInfoGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgHwVerInfoGet(
   DSL_Context_t *pContext,
   DSL_uint32_t *pVerNum)
{
   CMD_VersionInfoGet_t  sCmd = {0, 3};
   ACK_VersionInfoGet_t  sAck;
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_DEBUG( DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_SendMsgHwVerInfoGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   nErrCode = DSL_DRV_VRX_SendMessage(pContext,
      (DSL_uint16_t)CMD_VERSIONINFOGET,
      sizeof(sCmd), (DSL_uint8_t*)&sCmd,
      sizeof(ACK_VersionInfoGet_t), (DSL_uint8_t*)&sAck);

   if(nErrCode == DSL_SUCCESS)
   {
      *pVerNum = (DSL_uint32_t)sAck.HW_Version;
   }
   else
   {
      DSL_DEBUG(DSL_DBG_ERR,(pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - Failed to retrieve HW Version!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }


   DSL_DEBUG(DSL_DBG_MSG,(pContext,
      SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_SendMsgHwVerInfoGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgHybridTypeGet(
   DSL_Context_t *pContext,
   DSL_HybridType_t *pHybridType)
{
   CMD_HybridGet_t  sCmd = {0,1};
   ACK_HybridGet_t  sAck = {0,0,0};
   DSL_Error_t nErrCode = DSL_SUCCESS;

   DSL_DEBUG( DSL_DBG_MSG, (pContext,
      SYS_DBG_MSG"DSL[%02d]: IN - DSL_DRV_VRX_SendMsgHybridTypeGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   nErrCode = DSL_DRV_VRX_SendMessage(pContext,
      (DSL_uint16_t)CMD_HYBRIDGET,
      sizeof(sCmd), (DSL_uint8_t*)&sCmd,
      sizeof(ACK_HybridGet_t), (DSL_uint8_t*)&sAck);

   if(nErrCode == DSL_SUCCESS)
   {
      *pHybridType = (DSL_uint32_t)sAck.HybridType;
   }
   else
   {
      DSL_DEBUG(DSL_DBG_ERR,(pContext,
         SYS_DBG_ERR"DSL[%02d]: ERROR - Failed to retrieve HW Version!"
         DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
   }


   DSL_DEBUG(DSL_DBG_MSG,(pContext,
      SYS_DBG_MSG"DSL[%02d]: OUT - DSL_DRV_VRX_SendMsgHybridTypeGet"
      DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));

   return (nErrCode);
}

#ifdef INCLUDE_PILOT_TONES_STATUS
/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgPilotTonesGet(
   DSL_Context_t *pContext,
   DSL_uint8_t *pAck)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   CMD_PilotTonesGet_t sCmd;
   ACK_PilotTonesGet_t sAck;

   /* fill up the message to be sent */
   memset(&sCmd, 0, sizeof(sCmd));

   sCmd.Length = 17;

   nErrCode = DSL_DRV_VRX_SendMessage( pContext,
                  CMD_PILOTTONESGET,
                  sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                  sizeof(sAck), (DSL_uint8_t*)&sAck );

   /* Copy data only if successful */
   if (nErrCode >= 0)
   {
      memcpy(pAck, &sAck, sizeof(ACK_PilotTonesGet_t));
   }

   return (nErrCode);
}
#endif /* #ifdef INCLUDE_PILOT_TONES_STATUS*/

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgTcStatusGet(
   DSL_Context_t *pContext,
   DSL_uint8_t *pAck)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   CMD_TC_StatusGet_t sCmd;
   ACK_TC_StatusGet_t sAck;

   memset(&sCmd, 0, sizeof(sCmd));
   sCmd.Length = 1;

   nErrCode = DSL_DRV_VRX_SendMessage( pContext, CMD_TC_STATUSGET,
                  sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                  sizeof(sAck), (DSL_uint8_t*)&sAck );

   if( nErrCode == DSL_SUCCESS )
   {
      memcpy(pAck, &sAck, sizeof(ACK_TC_StatusGet_t));
   }

   return (nErrCode);
}

DSL_Error_t DSL_DRV_VRX_SendMsgMfdResultsGet(
   DSL_Context_t *pContext,
   DSL_uint8_t *pAck)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   CMD_MFD_ResultsGet_t sCmd;
   ACK_MFD_ResultsGet_t sAck;

   /* fill up the message to be sent */
   memset(&sCmd, 0, sizeof(sCmd));
   sCmd.Length = 3;

   /* clear buf for ack */
   memset(&sAck, 0, sizeof(sAck));

   /* Read necessary msg that includes filter detection results */
   nErrCode =  DSL_DRV_VRX_SendMessage(pContext, CMD_MFD_RESULTSGET,
                                       sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                                       sizeof(sAck), (DSL_uint8_t*)&sAck);
   /* Copy data only if successful */
   if (nErrCode >= 0)
   {
      memcpy(pAck, &(sAck), sizeof(sAck));
   }
   else
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext,
         "DSL[%02d]: ERROR - ACK_MFD_ResultsGet read failed!" DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));
      return nErrCode;
   }

   return (nErrCode);
}

DSL_Error_t DSL_DRV_VRX_SendMsgLoopLengthGet(
   DSL_Context_t *pContext,
   DSL_uint8_t *pAck)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   CMD_MFD_LooplengthGet_t sCmd;
   ACK_MFD_LooplengthGet_t sAck;

   /* fill up the message to be sent */
   memset(&sCmd, 0, sizeof(sCmd));
   sCmd.Length = 2;

   /* clear buf for ack */
   memset(&sAck, 0, sizeof(sAck));

   /* Read necessary msg that includes filter detection results */
   nErrCode =  DSL_DRV_VRX_SendMessage(pContext, CMD_MFD_LOOPLENGTHGET,
                                       sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                                       sizeof(sAck), (DSL_uint8_t*)&sAck);
   /* Copy data only if successful */
   if (nErrCode >= 0)
   {
      memcpy(pAck, &(sAck), sizeof(sAck));
   }
   else
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext,
         "DSL[%02d]: ERROR - ACK_MFD_LooplengthGet read failed!" DSL_DRV_CRLF,
         DSL_DEV_NUM(pContext)));
   }

   return (nErrCode);
}

DSL_Error_t DSL_DRV_VRX_SendMsgHybridInfoGet(
   DSL_Context_t *pContext,
   DSL_uint8_t *pAck)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   CMD_MFD_HybridInfoGet_t sCmd;
   ACK_MFD_HybridInfoGet_t sAck;

   /* fill up the message to be sent */
   memset(&sCmd, 0, sizeof(sCmd));
   sCmd.Length = 6;

   /* clear buf for ack */
   memset(&sAck, 0, sizeof(sAck));

   /* Read necessary msg that includes filter detection results */
   nErrCode =  DSL_DRV_VRX_SendMessage(pContext, CMD_MFD_HYBRIDINFOGET,
                                       sizeof(sCmd), (DSL_uint8_t*)&sCmd,
                                       sizeof(sAck), (DSL_uint8_t*)&sAck);
   /* Copy data only if successful */
   if (nErrCode >= 0)
   {
      memcpy(pAck, &(sAck), sizeof(sAck));
   }
   if (nErrCode != DSL_SUCCESS)
   {
      DSL_DEBUG( DSL_DBG_ERR, (pContext,
                     "DSL[%02d]: ERROR - ACK_MFD_HybridInfoGet read failed!"
                     DSL_DRV_CRLF, DSL_DEV_NUM(pContext)));
      return nErrCode;
   }

   return (nErrCode);
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgMiscConfigSet(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   CMD_Misc_ConfigSet_t sCmd;
   ACK_Misc_ConfigSet_t sAck;
   DSL_ActivationFsmConfigData_t nActivationCfg;
   DSL_boolean_t bT1_413;

   memset(&sCmd, 0, sizeof(sCmd));
   memset(&sAck, 0, sizeof(sAck));

   sCmd.Length = 0x1;

   DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.nActivationCfg, nActivationCfg);
   DSL_CTX_READ(pContext, nErrCode, pDevCtx->data.bT1_413, bT1_413);

   if (DSL_DRV_VRX_CamFsmStateGet(pContext) == DSL_CAM_ADSL_SINGLE && bT1_413)
   {
      if (nActivationCfg.nActivationSequence == DSL_ACT_SEQ_NON_STD)
      {
         sCmd.GhsAnsiSeq = VRX_ENABLE;
         sCmd.StartMode =
            nActivationCfg.nActivationMode == DSL_ACT_MODE_ANSI_T1413 ? 1 : 0;
      }
   }

   /* Set default values */
   sCmd.ShortClrA2p = VRX_ENABLE;
   sCmd.ShortClrA2 = VRX_ENABLE;
   sCmd.ShortClrA1 = VRX_ENABLE;
   sCmd.FrameParams = VRX_DISABLE;

   nErrCode = DSL_DRV_VRX_SendMessage(
                 pContext, CMD_MISC_CONFIGSET,
                 sizeof(CMD_Misc_ConfigSet_t), (DSL_uint8_t*)&sCmd,
                 sizeof(ACK_Misc_ConfigSet_t), (DSL_uint8_t*)&sAck);

   return nErrCode;
}

/*
   For a detailed description of the function, its arguments and return value
   please refer to the description in the header file 'drv_dsl_cpe_msg_vrx.h'
*/
DSL_Error_t DSL_DRV_VRX_SendMsgTestOptionsSet(
   DSL_Context_t *pContext)
{
   DSL_Error_t nErrCode = DSL_SUCCESS;
   CMD_TestOptionsSet_t sCmd;
   ACK_TestOptionsSet_t sAck;
   DSL_TestModeControlSet_t nTestModeControl = DSL_TESTMODE_DISABLE;

   memset(&sCmd, 0, sizeof(sCmd));
   memset(&sAck, 0, sizeof(sAck));

   sCmd.Length = 0x1;

   /* Test mode control*/
   DSL_CTX_READ_SCALAR(pContext, nErrCode, nTestModeControl, nTestModeControl);

   sCmd.FgainControlDs = (nTestModeControl == DSL_TESTMODE_SHOWTIME_LOCK
                       || nTestModeControl == DSL_TESTMODE_TRAINING_LOCK) ?
                       VRX_ENABLE : VRX_DISABLE;

   sCmd.FgainControlUs = (nTestModeControl == DSL_TESTMODE_SHOWTIME_LOCK
                       || nTestModeControl == DSL_TESTMODE_TRAINING_LOCK) ?
                       VRX_ENABLE : VRX_DISABLE;

   nErrCode = DSL_DRV_VRX_SendMessage(
                 pContext, CMD_TESTOPTIONSSET,
                 sizeof(CMD_TestOptionsSet_t), (DSL_uint8_t*)&sCmd,
                 sizeof(ACK_TestOptionsSet_t), (DSL_uint8_t*)&sAck);

   return nErrCode;
}
#endif /* #ifdef INCLUDE_DSL_CPE_API_VRX*/
