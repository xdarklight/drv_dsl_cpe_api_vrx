/******************************************************************************

                              Copyright (c) 2013
                            Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/


#ifndef _DRV_DSL_CPE_VRX_MSG_GHS_STATUS_H_
#define _DRV_DSL_CPE_VRX_MSG_GHS_STATUS_H_

/** \file

*/

#ifndef __PACKED__
   #if defined (__GNUC__) || defined (__GNUG__)
      /* GNU C or C++ compiler */
      #define __PACKED__ __attribute__ ((packed))
   #else
      /* Byte alignment adjustment */
      #pragma pack(1)
      #define __PACKED__      /* nothing */
   #endif
   #define __PACKED_DEFINED__ 1
#endif


/** @defgroup _GHS_STATUS_
 *  @{
 */

#ifdef __cplusplus
   extern "C" {
#endif

/* ----- Message Specific Constants Definition section ----- */
#define ACK_T1413_RevNo_O_Get_T1413REV1 0x0
#define ACK_T1413_RevNo_O_Get_T1413REV2 0x1
#define ACK_T1413_RevNo_R_Get_T1413REV1 0x0
#define ACK_T1413_RevNo_R_Get_T1413REV2 0x1
#define CMD_RFI_BandControlGet_FV 33
#define ACK_RFI_BandControlGet_FV 33
#define CMD_LineStatusPerBandDS_Get_FV 25
#define ACK_LineStatusPerBandDS_Get_FV 25
#define CMD_LineStatusPerBandUS_Get_FV 25
#define ACK_LineStatusPerBandUS_Get_FV 25
#define CMD_PSD_BreakpointsUS_Get_FV 97
#define ACK_PSD_BreakpointsUS_Get_FV 97
#define CMD_BearerChsDS_Get_FV 18
#define ACK_BearerChsDS_Get_FV 18
#define CMD_BearerChsUS_Get_FV 18
#define ACK_BearerChsUS_Get_FV 18
#define CMD_FrameDataExt2DS_Get_FV 40
#define ACK_FrameDataExt2DS_Get_FV 40
#define CMD_FrameDataExt2US_Get_FV 40
#define ACK_FrameDataExt2US_Get_FV 40
#define ACK_HybridGet_POTS 0x1
#define ACK_HybridGet_ISDN 0x2
#define ACK_MFD_ResultsGet_NONLNF 0x0
#define ACK_MFD_ResultsGet_NLNFTRIGGERED 0x1
#define ACK_MFD_ResultsGet_NLNFSTARTED 0x2
#define ACK_MFD_ResultsGet_NLNFDONE 0x3
#define ACK_MFD_ResultsGet_NORANGE 0x0
#define ACK_MFD_ResultsGet_SHORTRANGE 0x1
#define ACK_MFD_ResultsGet_MIDRANGE 0x2
#define ACK_MFD_ResultsGet_LONGRANGE 0x3
#define ACK_MFD_ResultsGet_READY 0x1
#define ACK_MFD_ResultsGet_NOT_READY 0x0
/* ----- Message Specific Constants Definition section (End) ----- */

/** Message ID for CMD_HS_ToneGroupSet */
#define CMD_HS_TONEGROUPSET 0x2D48

/**
   Selects one or more groups of tones to be used during the handshake phase.
   (See G.994.1 Amendment 2 [8], Table 1/G.994.1).
*/
typedef struct CMD_HS_ToneGroupSet CMD_HS_ToneGroupSet_t;

/** Message ID for ACK_HS_ToneGroupSet */
#define ACK_HS_TONEGROUPSET 0x2D48

/**
   Acknowledgement to CMD_HS_ToneGroupSet.
*/
typedef struct ACK_HS_ToneGroupSet ACK_HS_ToneGroupSet_t;

/** Message ID for CMD_HS_VDSL2ProfileControl */
#define CMD_HS_VDSL2PROFILECONTROL 0x2C48

/**
   Configures the profile settings to be used for the handshake phase. (See
   G.994.1 Amendment 4 [10], NPAR(3) coding).All "support" bits should be set to
   "Enable".
*/
typedef struct CMD_HS_VDSL2ProfileControl CMD_HS_VDSL2ProfileControl_t;

/** Message ID for ACK_HS_VDSL2ProfileControl */
#define ACK_HS_VDSL2PROFILECONTROL 0x2C48

/**
   Acknowledgement to CMD_HS_VDSL2ProfileControl.
*/
typedef struct ACK_HS_VDSL2ProfileControl ACK_HS_VDSL2ProfileControl_t;

/** Message ID for CMD_HS_StandardInfoFE_VDSL2Get */
#define CMD_HS_STANDARDINFOFE_VDSL2GET 0xCC03

/**
   Requests the VDSL2 far-end Standard Info (See G.994.1 Amendment 4 [10],
   NPAR(3) coding).
*/
typedef struct CMD_HS_StandardInfoFE_VDSL2Get CMD_HS_StandardInfoFE_VDSL2Get_t;

/** Message ID for ACK_HS_StandardInfoFE_VDSL2Get */
#define ACK_HS_STANDARDINFOFE_VDSL2GET 0xCC03

/**
   Delivers the VDSL2 far-end info as requested by
   CMD_HS_StandardInfoFE_VDSL2Get (see G.994.1 Amendment 4 [10], NPAR(3)
   coding).
*/
typedef struct ACK_HS_StandardInfoFE_VDSL2Get ACK_HS_StandardInfoFE_VDSL2Get_t;

/** Message ID for CMD_HS_SelectedProfileVDSL2Get */
#define CMD_HS_SELECTEDPROFILEVDSL2GET 0xCD03

/**
   Requests the actual selected VDSL2 Profile (See G.994.1 Amendment 4 [10],
   NPAR(3) coding).
*/
typedef struct CMD_HS_SelectedProfileVDSL2Get CMD_HS_SelectedProfileVDSL2Get_t;

/** Message ID for ACK_HS_SelectedProfileVDSL2Get */
#define ACK_HS_SELECTEDPROFILEVDSL2GET 0xCD03

/**
   Provides the actual selected VDSL2 Profile as requested by
   CMD_HS_SelectedProfileVDSL2Get (see G.994.1 Amendment 4 [10], NPAR(3)
   coding).
*/
typedef struct ACK_HS_SelectedProfileVDSL2Get ACK_HS_SelectedProfileVDSL2Get_t;

/** Message ID for CMD_HS_StandardInfoFE_SPAR1Get */
#define CMD_HS_STANDARDINFOFE_SPAR1GET 0xCB03

/**
   Requests the handshake information about the far-end operating mode (See
   G.994.1 [6] and Amendments 1-4 [7]-[10], standard information fields- SPAR(1)
   coding).
*/
typedef struct CMD_HS_StandardInfoFE_SPAR1Get CMD_HS_StandardInfoFE_SPAR1Get_t;

/** Message ID for ACK_HS_StandardInfoFE_SPAR1Get */
#define ACK_HS_STANDARDINFOFE_SPAR1GET 0xCB03

/**
   Provides the handshake information about the far-end operating mode (See
   G.994.1 [6] and Amendments 1-4 [7]-[10], Standard information field - SPAR(1)
   coding). The bit numbering per octet below follows the standard (starting
   with "1").
*/
typedef struct ACK_HS_StandardInfoFE_SPAR1Get ACK_HS_StandardInfoFE_SPAR1Get_t;

/** Message ID for CMD_SysVendorID_R_Set */
#define CMD_SYSVENDORID_R_SET 0x3C43

/**
   Sets the in System Vendor ID at the xTU-R. (Section 7.4.4 of G997.1 [11])
*/
typedef struct CMD_SysVendorID_R_Set CMD_SysVendorID_R_Set_t;

/** Message ID for ACK_SysVendorID_R_Set */
#define ACK_SYSVENDORID_R_SET 0x3C43

/**
   Acknowledgement to CMD_SysVendorID_R_Set.
*/
typedef struct ACK_SysVendorID_R_Set ACK_SysVendorID_R_Set_t;

/** Message ID for CMD_SysVendorVersionNum_R_Set */
#define CMD_SYSVENDORVERSIONNUM_R_SET 0x3D43

/**
   Sets the in system vendor version number at the xTU-R. (Section 7.4.6 of
   G997.1 [11])
*/
typedef struct CMD_SysVendorVersionNum_R_Set CMD_SysVendorVersionNum_R_Set_t;

/** Message ID for ACK_SysVendorVersionNum_R_Set */
#define ACK_SYSVENDORVERSIONNUM_R_SET 0x3D43

/**
   Acknowledgement to CMD_SysVendorVersionNum_R_Set.
*/
typedef struct ACK_SysVendorVersionNum_R_Set ACK_SysVendorVersionNum_R_Set_t;

/** Message ID for CMD_SysVendorSerialNum_R_Set */
#define CMD_SYSVENDORSERIALNUM_R_SET 0x3E43

/**
   Sets the system vendor serial number at the xTU-R. (Section 7.4.8 of G997.1
   [11])
*/
typedef struct CMD_SysVendorSerialNum_R_Set CMD_SysVendorSerialNum_R_Set_t;

/** Message ID for ACK_SysVendorSerialNum_R_Set */
#define ACK_SYSVENDORSERIALNUM_R_SET 0x3E43

/**
   Acknowledgement to CMD_SysVendorSerialNum_R_Set.
*/
typedef struct ACK_SysVendorSerialNum_R_Set ACK_SysVendorSerialNum_R_Set_t;

/** Message ID for CMD_AuxInventoryInfo_R_Set */
#define CMD_AUXINVENTORYINFO_R_SET 0x4343

/**
   Sets auxiliary inventory information at the VTU-R (¤11.2.3.2.4 of G993.2
   [5]).
*/
typedef struct CMD_AuxInventoryInfo_R_Set CMD_AuxInventoryInfo_R_Set_t;

/** Message ID for ACK_AuxInventoryInfo_R_Set */
#define ACK_AUXINVENTORYINFO_R_SET 0x4343

/**
   Acknowledgement to CMD_AuxInventoryInfo_R_Set.
*/
typedef struct ACK_AuxInventoryInfo_R_Set ACK_AuxInventoryInfo_R_Set_t;

/** Message ID for CMD_VendorID_O_Get */
#define CMD_VENDORID_O_GET 0x4003

/**
   Requests the xTU-C Vendor ID. (Section 7.4.1 of G997.1 [11])
*/
typedef struct CMD_VendorID_O_Get CMD_VendorID_O_Get_t;

/** Message ID for ACK_VendorID_O_Get */
#define ACK_VENDORID_O_GET 0x4003

/**
   Provides the xTU-C Vendor ID as requested by CMD_VendorID_O_Get.
*/
typedef struct ACK_VendorID_O_Get ACK_VendorID_O_Get_t;

/** Message ID for CMD_VendorID_R_Get */
#define CMD_VENDORID_R_GET 0x4103

/**
   Requests the xTU-R Vendor ID. (Section 7.4.2 of G997.1 [11])
*/
typedef struct CMD_VendorID_R_Get CMD_VendorID_R_Get_t;

/** Message ID for ACK_VendorID_R_Get */
#define ACK_VENDORID_R_GET 0x4103

/**
   Provides the xTU-R Vendor ID as requested by CMD_VendorID_R_Get.
*/
typedef struct ACK_VendorID_R_Get ACK_VendorID_R_Get_t;

/** Message ID for CMD_SysVendorID_O_Get */
#define CMD_SYSVENDORID_O_GET 0x3803

/**
   Requests the xTU-C System Vendor ID. (Section 7.4.3 of G997.1 [11])
*/
typedef struct CMD_SysVendorID_O_Get CMD_SysVendorID_O_Get_t;

/** Message ID for ACK_SysVendorID_O_Get */
#define ACK_SYSVENDORID_O_GET 0x3803

/**
   Delivers the xTU-C System Vendor ID as requested by CMD_SysVendorID_O_Get.
*/
typedef struct ACK_SysVendorID_O_Get ACK_SysVendorID_O_Get_t;

/** Message ID for CMD_SysVendorID_R_Get */
#define CMD_SYSVENDORID_R_GET 0x3C03

/**
   Requests the xTU-R System Vendor ID. (Section 7.4.4 of G997.1 [11])
*/
typedef struct CMD_SysVendorID_R_Get CMD_SysVendorID_R_Get_t;

/** Message ID for ACK_SysVendorID_R_Get */
#define ACK_SYSVENDORID_R_GET 0x3C03

/**
   Delivers the xTU-R System Vendor ID as requested by CMD_SysVendorID_R_Get.
*/
typedef struct ACK_SysVendorID_R_Get ACK_SysVendorID_R_Get_t;

/** Message ID for CMD_SysVendorVersionNumO_Get */
#define CMD_SYSVENDORVERSIONNUMO_GET 0x3A03

/**
   Requests the xTU-C system vendor version number. (Section 7.4.5 of G997.1
   [11])
*/
typedef struct CMD_SysVendorVersionNumO_Get CMD_SysVendorVersionNumO_Get_t;

/** Message ID for ACK_SysVendorVersionNum_O_Get */
#define ACK_SYSVENDORVERSIONNUM_O_GET 0x3A03

/**
   Delivers the xTU-C system vendor version number as requested by
   CMD_SysVendorVersionNumO_Get.
*/
typedef struct ACK_SysVendorVersionNum_O_Get ACK_SysVendorVersionNum_O_Get_t;

/** Message ID for CMD_SysVendorVersionNum_R_Get */
#define CMD_SYSVENDORVERSIONNUM_R_GET 0x3D03

/**
   Requests the xTU-R system vendor version number. (Section 7.4.6 of G997.1
   [11])
*/
typedef struct CMD_SysVendorVersionNum_R_Get CMD_SysVendorVersionNum_R_Get_t;

/** Message ID for ACK_SysVendorVersionNum_R_Get */
#define ACK_SYSVENDORVERSIONNUM_R_GET 0x3D03

/**
   Delivers the xTU-R system vendor version number as requested by
   CMD_SysVendorVersionNum_R_Get.
*/
typedef struct ACK_SysVendorVersionNum_R_Get ACK_SysVendorVersionNum_R_Get_t;

/** Message ID for CMD_SysVendorSerialNum_O_Get */
#define CMD_SYSVENDORSERIALNUM_O_GET 0x3903

/**
   Requests the xTU-C system vendor serial number. (Section 7.4.7 of G997.1
   [11])
*/
typedef struct CMD_SysVendorSerialNum_O_Get CMD_SysVendorSerialNum_O_Get_t;

/** Message ID for ACK_SysVendorSerialNum_O_Get */
#define ACK_SYSVENDORSERIALNUM_O_GET 0x3903

/**
   Delivers the xTU-C system vendor serial number as requested by
   CMD_SysVendorSerialNum_O_Get.
*/
typedef struct ACK_SysVendorSerialNum_O_Get ACK_SysVendorSerialNum_O_Get_t;

/** Message ID for CMD_SysVendorSerialNum_R_Get */
#define CMD_SYSVENDORSERIALNUM_R_GET 0x3E03

/**
   Requests the xTU-R system vendor serial number. (Section 7.4.8 of G997.1
   [11])
*/
typedef struct CMD_SysVendorSerialNum_R_Get CMD_SysVendorSerialNum_R_Get_t;

/** Message ID for ACK_SysVendorSerialNum_R_Get */
#define ACK_SYSVENDORSERIALNUM_R_GET 0x3E03

/**
   Delivers the xTU-R system vendor version number as requested by
   CMD_SysVendorSerialNum_R_Get.
*/
typedef struct ACK_SysVendorSerialNum_R_Get ACK_SysVendorSerialNum_R_Get_t;

/** Message ID for CMD_AuxInventoryInfo_O_Get */
#define CMD_AUXINVENTORYINFO_O_GET 0x4203

/**
   Requests VTU-O auxiliary inventory information (¤11.2.3.2.4 of G993.2 [5]).
*/
typedef struct CMD_AuxInventoryInfo_O_Get CMD_AuxInventoryInfo_O_Get_t;

/** Message ID for ACK_AuxInventoryInfo_O_Get */
#define ACK_AUXINVENTORYINFO_O_GET 0x4203

/**
   Delivers the VTU-O auxiliary inventory information as requested by
   CMD_AuxInventoryInfo_O_Get.
*/
typedef struct ACK_AuxInventoryInfo_O_Get ACK_AuxInventoryInfo_O_Get_t;

/** Message ID for CMD_AuxInventoryInfo_R_Get */
#define CMD_AUXINVENTORYINFO_R_GET 0x4303

/**
   Requests VTU-R auxiliary inventory information (¤11.2.3.2.4 of G993.2 [5]).
*/
typedef struct CMD_AuxInventoryInfo_R_Get CMD_AuxInventoryInfo_R_Get_t;

/** Message ID for ACK_AuxInventoryInfo_R_Get */
#define ACK_AUXINVENTORYINFO_R_GET 0x4303

/**
   Delivers the VTU-R auxiliary inventory information as requested by
   CMD_AuxInventoryInfo_R_Get.
*/
typedef struct ACK_AuxInventoryInfo_R_Get ACK_AuxInventoryInfo_R_Get_t;

/** Message ID for CMD_T1413_VendorID_O_Get */
#define CMD_T1413_VENDORID_O_GET 0x7C03

/**
   Requests the ATU-CÕs T1.413 Vendor identification. (Section 9.6.4.2 of ANSI
   T1.413 [16])
*/
typedef struct CMD_T1413_VendorID_O_Get CMD_T1413_VendorID_O_Get_t;

/** Message ID for ACK_T1413_VendorID_O_Get */
#define ACK_T1413_VENDORID_O_GET 0x7C03

/**
   Delivers the ATU-CÕs T1.413 vendor identification number.
*/
typedef struct ACK_T1413_VendorID_O_Get ACK_T1413_VendorID_O_Get_t;

/** Message ID for CMD_T1413_VendorID_R_Get */
#define CMD_T1413_VENDORID_R_GET 0x7D03

/**
   Requests the ATU-RÕs T1.413 Vendor identification. (Section 9.6.4.2 of ANSI
   T1.413 [16])
*/
typedef struct CMD_T1413_VendorID_R_Get CMD_T1413_VendorID_R_Get_t;

/** Message ID for ACK_T1413_VendorID_R_Get */
#define ACK_T1413_VENDORID_R_GET 0x7D03

/**
   Reports the ATU-RÕs T1.413 vendor identification number.
*/
typedef struct ACK_T1413_VendorID_R_Get ACK_T1413_VendorID_R_Get_t;

/** Message ID for CMD_T1413_RevNo_O_Get */
#define CMD_T1413_REVNO_O_GET 0x7E03

/**
   Requests the xTU-OÕs T1.413 revision number. (Section 9.6.4.3 of ANSI T1.413
   [16])
*/
typedef struct CMD_T1413_RevNo_O_Get CMD_T1413_RevNo_O_Get_t;

/** Message ID for ACK_T1413_RevNo_O_Get */
#define ACK_T1413_REVNO_O_GET 0x7E03

/**
   Delivers the xTU-OÕs T1.413 revision number.
*/
typedef struct ACK_T1413_RevNo_O_Get ACK_T1413_RevNo_O_Get_t;

/** Message ID for CMD_T1413_RevNo_R_Get */
#define CMD_T1413_REVNO_R_GET 0x7F03

/**
   Requests the xTU-RÕs T1.413 revision number. (Section 9.7.6.2 of ANSI T1.413
   [16])
*/
typedef struct CMD_T1413_RevNo_R_Get CMD_T1413_RevNo_R_Get_t;

/** Message ID for ACK_T1413_RevNo_R_Get */
#define ACK_T1413_REVNO_R_GET 0x7F03

/**
   Delivers the xTU-RÕs T1.413 revision number.
*/
typedef struct ACK_T1413_RevNo_R_Get ACK_T1413_RevNo_R_Get_t;

/** Message ID for CMD_T1413_VendorRevNo_O_Get */
#define CMD_T1413_VENDORREVNO_O_GET 0x8003

/**
   Requests the xTU-OÕs T1.413 vendor revision number. (Section 9.6.4.4 of ANSI
   T1.413 [16])
*/
typedef struct CMD_T1413_VendorRevNo_O_Get CMD_T1413_VendorRevNo_O_Get_t;

/** Message ID for ACK_T1413_VendorRevNo_O_Get */
#define ACK_T1413_VENDORREVNO_O_GET 0x8003

/**
   Delivers the xTU-OÕs T1.413 vendor revision number.
*/
typedef struct ACK_T1413_VendorRevNo_O_Get ACK_T1413_VendorRevNo_O_Get_t;

/** Message ID for CMD_T1413_VendorRevNo_R_Get */
#define CMD_T1413_VENDORREVNO_R_GET 0x8103

/**
   Requests the xTU-RÕs T1.413 vendor revision number. (Section 9.7.6.3 of ANSI
   T1.413 [16])
*/
typedef struct CMD_T1413_VendorRevNo_R_Get CMD_T1413_VendorRevNo_R_Get_t;

/** Message ID for ACK_T1413_VendorRevNo_R_Get */
#define ACK_T1413_VENDORREVNO_R_GET 0x8103

/**
   Delivers the xTU-RÕs T1.413 vendor revision number.
*/
typedef struct ACK_T1413_VendorRevNo_R_Get ACK_T1413_VendorRevNo_R_Get_t;

/** Message ID for CMD_XTSE_StatusGet */
#define CMD_XTSE_STATUSGET 0x0102

/**
   Requests the transmission system in use on the line (DSL flavour). (Section
   7.5.1.1. of G.997.1 [11]) See note at ACK_XTSE_StatusGet about the expected
   behaviour.
*/
typedef struct CMD_XTSE_StatusGet CMD_XTSE_StatusGet_t;

/** Message ID for ACK_XTSE_StatusGet */
#define ACK_XTSE_STATUSGET 0x0102

/**
   Reports the transmission system mode in use as requested by
   CMD_XTSE_StatusGet.The information is set by the FW after mode select (MS)
   during the handshake phase or during T1.413 state. Before mode selection the
   value "0" is reported (=none), after selection of a common mode exactly this
   single mode must be indicated. If there is no common mode, "0" is to be
   reported (here modem enters FAIL state).
*/
typedef struct ACK_XTSE_StatusGet ACK_XTSE_StatusGet_t;

/** Message ID for CMD_BandControl_DS_Get */
#define CMD_BANDCONTROL_DS_GET 0xC803

/**
   Reads the used downstream band configuration.The reported used DS band
   configuration reflects the MEDLEYds set as defined in the O-PRM message of
   the channel discovery phase according to G.993.2.
*/
typedef struct CMD_BandControl_DS_Get CMD_BandControl_DS_Get_t;

/** Message ID for ACK_BandControl_DS_Get */
#define ACK_BANDCONTROL_DS_GET 0xC803

/**
   Acknowledgement for message CMD_BandControl_DS_Get.
*/
typedef struct ACK_BandControl_DS_Get ACK_BandControl_DS_Get_t;

/** Message ID for CMD_BandControl_US_Get */
#define CMD_BANDCONTROL_US_GET 0xC903

/**
   Reads the used upstream band configuration.The reported used US band
   configuration reflects the MEDLEYus set as defined in the R-PRM message of
   the channel discovery phase in G.993.2.
*/
typedef struct CMD_BandControl_US_Get CMD_BandControl_US_Get_t;

/** Message ID for ACK_BandControl_US_Get */
#define ACK_BANDCONTROL_US_GET 0xC903

/**
   Acknowledgement for message CMD_BandControl_US_Get.
*/
typedef struct ACK_BandControl_US_Get ACK_BandControl_US_Get_t;

/** Message ID for CMD_RFI_BandControlGet */
#define CMD_RFI_BANDCONTROLGET 0xCA03

/**
   Reads the RFI band configuration.
*/
typedef struct CMD_RFI_BandControlGet CMD_RFI_BandControlGet_t;

/** Message ID for ACK_RFI_BandControlGet */
#define ACK_RFI_BANDCONTROLGET 0xCA03

/**
   Acknowledgement for message CMD_RFI_BandControlGet.
*/
typedef struct ACK_RFI_BandControlGet ACK_RFI_BandControlGet_t;

/** Message ID for CMD_LineStatusDS_Get */
#define CMD_LINESTATUSDS_GET 0x4403

/**
   This message requests data about the line status (like noise margin) for
   downstream direction.
*/
typedef struct CMD_LineStatusDS_Get CMD_LineStatusDS_Get_t;

/** Message ID for ACK_LineStatusDS_Get */
#define ACK_LINESTATUSDS_GET 0x4403

/**
   The message delivers the line status data for downstream direction requested
   by CMD_LineStatusDS_Get.
*/
typedef struct ACK_LineStatusDS_Get ACK_LineStatusDS_Get_t;

/** Message ID for CMD_LineStatusUS_Get */
#define CMD_LINESTATUSUS_GET 0x4503

/**
   This message requests data about the upstream line status (like noise
   margin).
*/
typedef struct CMD_LineStatusUS_Get CMD_LineStatusUS_Get_t;

/** Message ID for ACK_LineStatusUS_Get */
#define ACK_LINESTATUSUS_GET 0x4503

/**
   The message delivers the line status data for upstream direction requested by
   CMD_LineStatusUS_Get.
*/
typedef struct ACK_LineStatusUS_Get ACK_LineStatusUS_Get_t;

/** Message ID for CMD_LineStatusPerBandDS_Get */
#define CMD_LINESTATUSPERBANDDS_GET 0xD203

/**
   This message requests data about the line status (noise margin, line
   attenuation and signal attenuation) for downstream direction in a "per band"
   granularity.
*/
typedef struct CMD_LineStatusPerBandDS_Get CMD_LineStatusPerBandDS_Get_t;

/** Message ID for ACK_LineStatusPerBandDS_Get */
#define ACK_LINESTATUSPERBANDDS_GET 0xD203

/**
   The message delivers the line status data (line attenuation, signal
   attenuation and noise margin) for downstream direction in a "per band"
   granularity as requested by CMD_LineStatusPerBandDS_Get.
*/
typedef struct ACK_LineStatusPerBandDS_Get ACK_LineStatusPerBandDS_Get_t;

/** Message ID for CMD_LineStatusPerBandUS_Get */
#define CMD_LINESTATUSPERBANDUS_GET 0xD303

/**
   This message requests data about the line status (noise margin, line - and
   signal attenuation) for upstream direction in a "per band" granularity.
*/
typedef struct CMD_LineStatusPerBandUS_Get CMD_LineStatusPerBandUS_Get_t;

/** Message ID for ACK_LineStatusPerBandUS_Get */
#define ACK_LINESTATUSPERBANDUS_GET 0xD303

/**
   The message delivers the line status data (line attenuation, signal
   attenuation and noise margin) for upstream direction in a "per band"
   granularity as requested by CMD_LineStatusPerBandUS_Get.
*/
typedef struct ACK_LineStatusPerBandUS_Get ACK_LineStatusPerBandUS_Get_t;

/** Message ID for CMD_PSD_BreakpointsDS_Get */
#define CMD_PSD_BREAKPOINTSDS_GET 0xD403

/**
   Requests the breakpoints of the actual PSD for the downstream direction. PSDs
   consisting of more than 42 PSD breakpoints must be read with 2 messages due
   to the the mailbox size. (The first parameter in the retrieved data structure
   contains the "Number of Breakpoints" parameter.)
*/
typedef struct CMD_PSD_BreakpointsDS_Get CMD_PSD_BreakpointsDS_Get_t;

/** Message ID for ACK_PSD_BreakpointsDS_Get */
#define ACK_PSD_BREAKPOINTSDS_GET 0xD403

/**
   Delivers the breakpoints of the actual PSD for the downstream direction as
   requested by CMD_PSD_BreakpointsDS_Get.
*/
typedef struct ACK_PSD_BreakpointsDS_Get ACK_PSD_BreakpointsDS_Get_t;

/** Message ID for CMD_PSD_BreakpointsUS_Get */
#define CMD_PSD_BREAKPOINTSUS_GET 0xD503

/**
   Requests the breakpoints of the actual PSD for the upstream direction.
*/
typedef struct CMD_PSD_BreakpointsUS_Get CMD_PSD_BreakpointsUS_Get_t;

/** Message ID for ACK_PSD_BreakpointsUS_Get */
#define ACK_PSD_BREAKPOINTSUS_GET 0xD503

/**
   Delivers the breakpoints of the actual PSD for the upstream direction as
   requested by CMD_PSD_BreakpointsUS_Get.
*/
typedef struct ACK_PSD_BreakpointsUS_Get ACK_PSD_BreakpointsUS_Get_t;

/** Message ID for CMD_TssiDS_VDSL2_Get */
#define CMD_TSSIDS_VDSL2_GET 0x2803

/**
   The Downstream TSSI Breakpoints are specified by the transmitter (VTU-O) and
   communicated to the receiver (VTU-R) via the O-PRM message.   The TSSI
   Breakpoints are used to shape the O-P-TEQ signal. This message provides read
   only access to the DS TSSI Breakpoints. The DS TSSI Breakpoints are not
   configurable via messages.
*/
typedef struct CMD_TssiDS_VDSL2_Get CMD_TssiDS_VDSL2_Get_t;

/** Message ID for ACK_TssiDS_VDSL2_Get */
#define ACK_TSSIDS_VDSL2_GET 0x2803

/**
   This is the acknowledgement of CMD_TssiDS_VDSL2_Get.
*/
typedef struct ACK_TssiDS_VDSL2_Get ACK_TssiDS_VDSL2_Get_t;

/** Message ID for CMD_TssiUS_VDSL2_Get */
#define CMD_TSSIUS_VDSL2_GET 0x2903

/**
   The Upstream TSSI Breakpoints are specified by the transmitter (VTU-R) and
   communicated to the receiver (VTU-O) via the R-PRM message.   The TSSI
   Breakpoints are used to shape the R-P-TEQ signal.  The TSSI Breakpoints can
   be read only but are not configurable via messages.
*/
typedef struct CMD_TssiUS_VDSL2_Get CMD_TssiUS_VDSL2_Get_t;

/** Message ID for ACK_TssiUS_VDSL2_Get */
#define ACK_TSSIUS_VDSL2_GET 0x2903

/**
   This is the acknowledgement of CMD_TssiUS_VDSL2_Get.
*/
typedef struct ACK_TssiUS_VDSL2_Get ACK_TssiUS_VDSL2_Get_t;

/** Message ID for CMD_ADSL_PCB_Get */
#define CMD_ADSL_PCB_GET 0x000E

/**
   Reads the actual power cutback value.
*/
typedef struct CMD_ADSL_PCB_Get CMD_ADSL_PCB_Get_t;

/** Message ID for ACK_ADSL_PCB_Get */
#define ACK_ADSL_PCB_GET 0x000E

/**
   Provides the actual power cutback values requested with CMD_ADSL_PCB_Get.
*/
typedef struct ACK_ADSL_PCB_Get ACK_ADSL_PCB_Get_t;

/** Message ID for CMD_ActualPSD_Get */
#define CMD_ACTUALPSD_GET 0xD803

/**
   Requests the PSD and power parameter values actually used by the firmware.
*/
typedef struct CMD_ActualPSD_Get CMD_ActualPSD_Get_t;

/** Message ID for ACK_ActualPSD_Get */
#define ACK_ACTUALPSD_GET 0xD803

/**
   Provieds actually used PSD and power parameters as requested by
   CMD_ActualPSD_Get.
*/
typedef struct ACK_ActualPSD_Get ACK_ActualPSD_Get_t;

/** Message ID for CMD_ADSL_MJ_UsMaskGet */
#define CMD_ADSL_MJ_USMASKGET 0x1802

/**
   Requests the actually used ADSL upstream mask for ADSL2/2+ Annexes M and J.
   To be applied only in combination with G.992.3/5 Annex M support in
   ACK_XTSE_StatusGet.
*/
typedef struct CMD_ADSL_MJ_UsMaskGet CMD_ADSL_MJ_UsMaskGet_t;

/** Message ID for ACK_ADSL_MJ_UsMaskGet */
#define ACK_ADSL_MJ_USMASKGET 0x1802

/**
   Provides the actually used ADSL upstream mask for ADSL2/2+ Annexes M and J.
*/
typedef struct ACK_ADSL_MJ_UsMaskGet ACK_ADSL_MJ_UsMaskGet_t;

/** Message ID for CMD_BearerChsDS_Get */
#define CMD_BEARERCHSDS_GET 0x0106

/**
   Requests status information for the downstream bearer channels.
*/
typedef struct CMD_BearerChsDS_Get CMD_BearerChsDS_Get_t;

/** Message ID for ACK_BearerChsDS_Get */
#define ACK_BEARERCHSDS_GET 0x0106

/**
   Delivers status information for the downstream bearer channels. **: The terms
   "LP0/Interleaved Path" and "LP1/Fast Path" below refer to the terminology
   used in ITU DSL standards [1],[2], [3], [5] which means:- LP0 or LP1 in case
   of VDSL2, ADSL2/2+- "Interleaved Path" or "Fast Path" in case of
   ADSL1.Attention: In VRX V1.x the "LP1/Fast Path" parameters are populated
   only in ADSL1 mode with selected fast path.
*/
typedef struct ACK_BearerChsDS_Get ACK_BearerChsDS_Get_t;

/** Message ID for CMD_BearerChsUS_Get */
#define CMD_BEARERCHSUS_GET 0x0006

/**
   Requests status information for the upstream bearer channels.
*/
typedef struct CMD_BearerChsUS_Get CMD_BearerChsUS_Get_t;

/** Message ID for ACK_BearerChsUS_Get */
#define ACK_BEARERCHSUS_GET 0x0006

/**
   Delivers status information for the upstream bearer channels. **: The terms
   "LP0/Interleaved Path" and "LP1/Fast Path" below refer to the terminology
   used in ITU DSL standards [1],[2], [3], [5] which means:- LP0 or LP1 in case
   of VDSL2, ADSL2/2+- "Interleaved Path" or "Fast Path" in case of
   ADSL1.Attention: In VRX V1.x the "LP1/Fast Path" parameters are populated
   only in ADSL1 mode with selected fast path.
*/
typedef struct ACK_BearerChsUS_Get ACK_BearerChsUS_Get_t;

/** Message ID for CMD_FrameDataExt2DS_Get */
#define CMD_FRAMEDATAEXT2DS_GET 0x2203

/**
   Requests an extended set of VDSL2 framing parameters for the downstream
   direction.
*/
typedef struct CMD_FrameDataExt2DS_Get CMD_FrameDataExt2DS_Get_t;

/** Message ID for ACK_FrameDataExt2DS_Get */
#define ACK_FRAMEDATAEXT2DS_GET 0x2203

/**
   Delivers an extended set of VDSL2 framing parameters for the downstream
   direction requested by CMD_FrameDataExt2DS_Get. (Sections 7.5.2.6 of G.997.1
   and 9.5 & 9.6 of G.993.2 [5])
*/
typedef struct ACK_FrameDataExt2DS_Get ACK_FrameDataExt2DS_Get_t;

/** Message ID for CMD_FrameDataExt2US_Get */
#define CMD_FRAMEDATAEXT2US_GET 0x2303

/**
   Requests an extended set of VDSL2 framing parameters for the upstream
   direction.
*/
typedef struct CMD_FrameDataExt2US_Get CMD_FrameDataExt2US_Get_t;

/** Message ID for ACK_FrameDataExt2US_Get */
#define ACK_FRAMEDATAEXT2US_GET 0x2303

/**
   Delivers an extended set of VDSL2 framing parameters for the upstream
   direction requested by CMD_FrameDataExt2US_Get. (Sections 7.5.2.6 of G.997.1
   and 9.5 & 9.6 of G.993.2 [5])
*/
typedef struct ACK_FrameDataExt2US_Get ACK_FrameDataExt2US_Get_t;

/** Message ID for CMD_ADSL_FrameDataDS_LP0Get */
#define CMD_ADSL_FRAMEDATADS_LP0GET 0x010E

/**
   Requests ADSL framing parameters for the downstream direction, latency path
   0/Interleaved Path.
*/
typedef struct CMD_ADSL_FrameDataDS_LP0Get CMD_ADSL_FrameDataDS_LP0Get_t;

/** Message ID for ACK_ADSL_FrameDataDS_LP0Get */
#define ACK_ADSL_FRAMEDATADS_LP0GET 0x010E

/**
   Delivers the ADSL framing parameters for the downstream direction,
   LP0/Interleaved path, as requested by CMD_ADSL_FrameDataDS_LP0Get. (Sections
   7.5 & 7.6 of G.992.3 [2] and 7.4 of G.992.1 [1])
*/
typedef struct ACK_ADSL_FrameDataDS_LP0Get ACK_ADSL_FrameDataDS_LP0Get_t;

/** Message ID for CMD_ADSL_FrameDataUS_LP0Get */
#define CMD_ADSL_FRAMEDATAUS_LP0GET 0x020E

/**
   Requests ADSL framing parameters for the upstream direction, latency path
   0/Interleaved path.
*/
typedef struct CMD_ADSL_FrameDataUS_LP0Get CMD_ADSL_FrameDataUS_LP0Get_t;

/** Message ID for ACK_ADSL_FrameDataUS_LP0Get */
#define ACK_ADSL_FRAMEDATAUS_LP0GET 0x020E

/**
   Delivers the ADSL framing parameters for the upstream direction, latency path
   0/Interleaved path, as requested by CMD_ADSL_FrameDataUS_LP0Get. (Sections
   7.5 & 7.6 of G.992.3 [2] and 7.4 of G.992.1 [1])
*/
typedef struct ACK_ADSL_FrameDataUS_LP0Get ACK_ADSL_FrameDataUS_LP0Get_t;

/** Message ID for CMD_ADSL_FrameDataDS_LP1Get */
#define CMD_ADSL_FRAMEDATADS_LP1GET 0x030E

/**
   Requests ADSL framing parameters for the downstream direction, latency path 1
   (LP1)/ Fast path.In VRX Rev. 1.x to be used for ADSL1, "fast path" only.
*/
typedef struct CMD_ADSL_FrameDataDS_LP1Get CMD_ADSL_FrameDataDS_LP1Get_t;

/** Message ID for ACK_ADSL_FrameDataDS_LP1Get */
#define ACK_ADSL_FRAMEDATADS_LP1GET 0x030E

/**
   Delivers the ADSL framing parameters for the downstream direction, LP1/Fast
   path, as requested by CMD_ADSL_FrameDataDS_LP1Get. (Sections 7.5 & 7.6 of
   G.992.3 [2] and 7.4 of G.992.1 [1])
*/
typedef struct ACK_ADSL_FrameDataDS_LP1Get ACK_ADSL_FrameDataDS_LP1Get_t;

/** Message ID for CMD_ADSL_FrameDataUS_LP1Get */
#define CMD_ADSL_FRAMEDATAUS_LP1GET 0x040E

/**
   Requests ADSL framing parameters for the upstream direction, latency path 1
   (LP1)/Fast path.In VRX Rev. 1.x to be used for ADSL1, "fast path" only.
*/
typedef struct CMD_ADSL_FrameDataUS_LP1Get CMD_ADSL_FrameDataUS_LP1Get_t;

/** Message ID for ACK_ADSL_FrameDataUS_LP1Get */
#define ACK_ADSL_FRAMEDATAUS_LP1GET 0x040E

/**
   Delivers the ADSL framing parameters for the upstream direction, LP1/Fast
   path, as requested by CMD_ADSL_FrameDataUS_LP1Get. (Sections 7.5 & 7.6 of
   G.992.3 [2] and 7.4 of G.992.1 [1])
*/
typedef struct ACK_ADSL_FrameDataUS_LP1Get ACK_ADSL_FrameDataUS_LP1Get_t;

/** Message ID for CMD_AlgorithmControlGet */
#define CMD_ALGORITHMCONTROLGET 0x0205

/**
   Reads back the parameter values configured with CMD_AlgorithmControlSet.
*/
typedef struct CMD_AlgorithmControlGet CMD_AlgorithmControlGet_t;

/** Message ID for ACK_AlgorithmControlGet */
#define ACK_ALGORITHMCONTROLGET 0x0205

/**
   Provides the information requested by CMD_AlgorithmControlGet.
*/
typedef struct ACK_AlgorithmControlGet ACK_AlgorithmControlGet_t;

/** Message ID for CMD_HybridGet */
#define CMD_HYBRIDGET 0x0922

/**
   Reads the connected hybrid type.
*/
typedef struct CMD_HybridGet CMD_HybridGet_t;

/** Message ID for ACK_HybridGet */
#define ACK_HYBRIDGET 0x0922

/**
   Contains the connected hybrid type.
*/
typedef struct ACK_HybridGet ACK_HybridGet_t;

/** Message ID for CMD_PilotTonesGet */
#define CMD_PILOTTONESGET 0x0322

/**
   Requests the pilot tones used in Showtime.
*/
typedef struct CMD_PilotTonesGet CMD_PilotTonesGet_t;

/** Message ID for ACK_PilotTonesGet */
#define ACK_PILOTTONESGET 0x0322

/**
   Provides the Showtime pilot tones as requested by CMD_PilotTonesGet.
*/
typedef struct ACK_PilotTonesGet ACK_PilotTonesGet_t;

/** Message ID for CMD_MFD_ResultsGet */
#define CMD_MFD_RESULTSGET 0x1022

/**
   Requests the results of the micro filter detection (MFD) function. Valid
   information in Showtime and FAIL will be available only if the microfilter
   detection mechanism has been run before.
*/
typedef struct CMD_MFD_ResultsGet CMD_MFD_ResultsGet_t;

/** Message ID for ACK_MFD_ResultsGet */
#define ACK_MFD_RESULTSGET 0x1022

/**
   Reports the results of the micro filter detection (MFD) function.
*/
typedef struct ACK_MFD_ResultsGet ACK_MFD_ResultsGet_t;

/** Message ID for CMD_MFD_LooplengthGet */
#define CMD_MFD_LOOPLENGTHGET 0x1122

/**
   Requests the loop length estimates done by the microfilter detection
   function.In Showtime the data are always available.The availability of data
   is indicated by the flag TrainingLoopLengthReady in ACK_MFD_ResultsGet.In
   Showtime the data are always available.
*/
typedef struct CMD_MFD_LooplengthGet CMD_MFD_LooplengthGet_t;

/** Message ID for ACK_MFD_LooplengthGet */
#define ACK_MFD_LOOPLENGTHGET 0x1122

/**
   Reports the loop length estimates done by the microfilter detection (MFD)
   function.
*/
typedef struct ACK_MFD_LooplengthGet ACK_MFD_LooplengthGet_t;

/** Message ID for CMD_MFD_HybridInfoGet */
#define CMD_MFD_HYBRIDINFOGET 0x1222

/**
   Requests hybrid information used by the microfilter detection function.In
   Showtime the data are always available.The availability of data is indicated
   by the flag TrainingHybridInfoReady in ACK_MFD_ResultsGet. In Showtime the
   data are always available.
*/
typedef struct CMD_MFD_HybridInfoGet CMD_MFD_HybridInfoGet_t;

/** Message ID for ACK_MFD_HybridInfoGet */
#define ACK_MFD_HYBRIDINFOGET 0x1222

/**
   Reports the hybrid information used by the microfilter detection function.
*/
typedef struct ACK_MFD_HybridInfoGet ACK_MFD_HybridInfoGet_t;

/**
   Selects one or more groups of tones to be used during the handshake phase.
   (See G.994.1 Amendment 2 [8], Table 1/G.994.1).
*/
struct CMD_HS_ToneGroupSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 7;
   /** Tone Group 8: CAR_SET_V43I */
   DSL_uint16_t Group8 : 1;
   /** Tone Group 7: CAR_SET_V43P */
   DSL_uint16_t Group7 : 1;
   /** Tone Group 6: CAR_SET_V43 */
   DSL_uint16_t Group6 : 1;
   /** Tone Group 5: CAR_SET_J43 */
   DSL_uint16_t Group5 : 1;
   /** Tone Group 4: CAR_SET_C43 */
   DSL_uint16_t Group4 : 1;
   /** Tone Group 3: CAR_SET_B43C */
   DSL_uint16_t Group3 : 1;
   /** Tone Group 2: CAR_SET_B43 */
   DSL_uint16_t Group2 : 1;
   /** Tone Group 1: CAR_SET_A43C */
   DSL_uint16_t Group1 : 1;
   /** Tone Group 0: CAR_SET_A43 */
   DSL_uint16_t Group0 : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Tone Group 0: CAR_SET_A43 */
   DSL_uint16_t Group0 : 1;
   /** Tone Group 1: CAR_SET_A43C */
   DSL_uint16_t Group1 : 1;
   /** Tone Group 2: CAR_SET_B43 */
   DSL_uint16_t Group2 : 1;
   /** Tone Group 3: CAR_SET_B43C */
   DSL_uint16_t Group3 : 1;
   /** Tone Group 4: CAR_SET_C43 */
   DSL_uint16_t Group4 : 1;
   /** Tone Group 5: CAR_SET_J43 */
   DSL_uint16_t Group5 : 1;
   /** Tone Group 6: CAR_SET_V43 */
   DSL_uint16_t Group6 : 1;
   /** Tone Group 7: CAR_SET_V43P */
   DSL_uint16_t Group7 : 1;
   /** Tone Group 8: CAR_SET_V43I */
   DSL_uint16_t Group8 : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 7;
#endif
} __PACKED__ ;


/**
   Acknowledgement to CMD_HS_ToneGroupSet.
*/
struct ACK_HS_ToneGroupSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Configures the profile settings to be used for the handshake phase. (See
   G.994.1 Amendment 4 [10], NPAR(3) coding).All "support" bits should be set to
   "Enable".
*/
struct CMD_HS_VDSL2ProfileControl
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t  Length;
   /** Reserved */
   DSL_uint8_t Res0;
   /** 30a, Profile Support Bit 7 */
   DSL_uint16_t profileSup7 : 1;
   /** 17a, Profile Support Bit 6 */
   DSL_uint16_t profileSup6 : 1;
   /** 12b, Profile Support Bit 5 */
   DSL_uint16_t profileSup5 : 1;
   /** 12a, Profile Support Bit 4 */
   DSL_uint16_t profileSup4 : 1;
   /** 8d, Profile Support Bit 3 */
   DSL_uint16_t profileSup3 : 1;
   /** 8c, Profile Support Bit 2 */
   DSL_uint16_t profileSup2 : 1;
   /** 8b, Profile Support Bit 1 */
   DSL_uint16_t profileSup1 : 1;
   /** 8a, Profile Support Bit 0 */
   DSL_uint16_t profileSup0 : 1;
   /** Reserved */
   DSL_uint8_t Res1;
   /** 30a, Profile Preferred Bit 7 */
   DSL_uint16_t profilePref7 : 1;
   /** 17a, Profile Preferred Bit 6 */
   DSL_uint16_t profilePref6 : 1;
   /** 12b, Profile Preferred Bit 5 */
   DSL_uint16_t profilePref5 : 1;
   /** 12a, Profile Preferred Bit 4 */
   DSL_uint16_t profilePref4 : 1;
   /** 8d, Profile Preferred Bit 3 */
   DSL_uint16_t profilePref3 : 1;
   /** 8c, Profile Preferred Bit 2 */
   DSL_uint16_t profilePref2 : 1;
   /** 8b, Profile Preferred Bit 1 */
   DSL_uint16_t profilePref1 : 1;
   /** 8a, Profile Preferred Bit 0 */
   DSL_uint16_t profilePref0 : 1;
   /** ADLU-56, Annex A US0 PSDs Supported Bit 15 */
   DSL_uint16_t A_US0PsdSup15 : 1;
   /** ADLU-52, Annex A US0 PSDs Supported Bit 14 */
   DSL_uint16_t A_US0PsdSup14 : 1;
   /** ADLU-48, Annex A US0 PSDs Supported Bit 13 */
   DSL_uint16_t A_US0PsdSup13 : 1;
   /** ADLU-44, Annex A US0 PSDs Supported Bit 12 */
   DSL_uint16_t A_US0PsdSup12 : 1;
   /** ADLU-40, Annex A US0 PSDs Supported Bit 11 */
   DSL_uint16_t A_US0PsdSup11 : 1;
   /** ADLU-36, Annex A US0 PSDs Supported Bit 10 */
   DSL_uint16_t A_US0PsdSup10 : 1;
   /** ADLU-32, Annex A US0 PSDs Supported Bit 9 */
   DSL_uint16_t A_US0PsdSup9 : 1;
   /** EU-64, Annex A US0 PSDs Supported Bit 8 */
   DSL_uint16_t A_US0PsdSup8 : 1;
   /** EU-60, Annex A US0 PSDs Supported Bit 7 */
   DSL_uint16_t A_US0PsdSup7 : 1;
   /** EU-56, Annex A US0 PSDs Supported Bit 6 */
   DSL_uint16_t A_US0PsdSup6 : 1;
   /** EU-52, Annex A US0 PSDs Supported Bit 5 */
   DSL_uint16_t A_US0PsdSup5 : 1;
   /** EU-48, Annex A US0 PSDs Supported Bit 4 */
   DSL_uint16_t A_US0PsdSup4 : 1;
   /** EU-44, Annex A US0 PSDs Supported Bit 3 */
   DSL_uint16_t A_US0PsdSup3 : 1;
   /** EU-40, Annex A US0 PSDs Supported Bit 2 */
   DSL_uint16_t A_US0PsdSup2 : 1;
   /** EU-36, Annex A US0 PSDs Supported Bit 1 */
   DSL_uint16_t A_US0PsdSup1 : 1;
   /** EU-32, Annex A US0 PSDs Supported Bit 0 */
   DSL_uint16_t A_US0PsdSup0 : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 10;
   /** ADLU-128, Annex A US0 PSDs Supported Bit 21 */
   DSL_uint16_t A_US0PsdSup21 : 1;
   /** EU-128, Annex A US0 PSDs Supported Bit 20 */
   DSL_uint16_t A_US0PsdSup20 : 1;
   /** 17a Profile, Annex A US0 PSDs Supported Bit 19 */
   DSL_uint16_t A_US0PsdSup19 : 1;
   /** 12b Profile, Annex A US0 PSDs Supported Bit 18 */
   DSL_uint16_t A_US0PsdSup18 : 1;
   /** ADLU-64, Annex A US0 PSDs Supported Bit 17 */
   DSL_uint16_t A_US0PsdSup17 : 1;
   /** ADLU-60, Annex A US0 PSDs Supported Bit 16 */
   DSL_uint16_t A_US0PsdSup16 : 1;
   /** Reserved */
   DSL_uint16_t Res3[2];
   /** Reserved */
   DSL_uint16_t Res4 : 11;
   /** 17a Profile, Annex B US0 PSDs Supported Bit 4 */
   DSL_uint16_t B_US0PsdSup4 : 1;
   /** 12b Profile, Annex B US0 PSDs Supported Bit 3 */
   DSL_uint16_t B_US0PsdSup3 : 1;
   /** US0 In 120 to 276 kHz, Annex B US0 PSDs Supported Bit 2 */
   DSL_uint16_t B_US0PsdSup2 : 1;
   /** US0 In 25 to 276 kHz, Annex B US0 PSDs Supported Bit 1 */
   DSL_uint16_t B_US0PsdSup1 : 1;
   /** US0 In 25 to 138 kHz, Annex B US0 PSDs Supported Bit 0 */
   DSL_uint16_t B_US0PsdSup0 : 1;
   /** Reserved */
   DSL_uint16_t Res5;
   /** Reserved */
   DSL_uint16_t Res6 : 2;
   /** 17a Profile, Annex C US0 PSDs Supported Bit 13 */
   DSL_uint16_t C_US0PsdSup13 : 1;
   /** 12b Profile, Annex C US0 PSDs Supported Bit 12 */
   DSL_uint16_t C_US0PsdSup12 : 1;
   /** Reserved */
   DSL_uint16_t Res7 : 6;
   /** US0 In 25 to 276 kHz, Annex C US0 PSDs Supported Bit 5 */
   DSL_uint16_t C_US0PsdSup5 : 1;
   /** US0 In 25 to 138 kHz, Annex C US0 PSDs Supported Bit 4 */
   DSL_uint16_t C_US0PsdSup4 : 1;
   /** Reserved */
   DSL_uint16_t Res8 : 2;
   /** US0 In 25 to 276kHz, Annex C US0 PSDs Supported Bit 1 */
   DSL_uint16_t C_US0PsdSup1 : 1;
   /** US0 In 25 to 138 kHz, Annex C US0 PSDs Supported Bit 0 */
   DSL_uint16_t C_US0PsdSup0 : 1;
   /** Reserved */
   DSL_uint16_t Res9;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t  Length;
   /** 8a, Profile Support Bit 0 */
   DSL_uint16_t profileSup0 : 1;
   /** 8b, Profile Support Bit 1 */
   DSL_uint16_t profileSup1 : 1;
   /** 8c, Profile Support Bit 2 */
   DSL_uint16_t profileSup2 : 1;
   /** 8d, Profile Support Bit 3 */
   DSL_uint16_t profileSup3 : 1;
   /** 12a, Profile Support Bit 4 */
   DSL_uint16_t profileSup4 : 1;
   /** 12b, Profile Support Bit 5 */
   DSL_uint16_t profileSup5 : 1;
   /** 17a, Profile Support Bit 6 */
   DSL_uint16_t profileSup6 : 1;
   /** 30a, Profile Support Bit 7 */
   DSL_uint16_t profileSup7 : 1;
   /** Reserved */
   DSL_uint8_t Res0;
   /** 8a, Profile Preferred Bit 0 */
   DSL_uint16_t profilePref0 : 1;
   /** 8b, Profile Preferred Bit 1 */
   DSL_uint16_t profilePref1 : 1;
   /** 8c, Profile Preferred Bit 2 */
   DSL_uint16_t profilePref2 : 1;
   /** 8d, Profile Preferred Bit 3 */
   DSL_uint16_t profilePref3 : 1;
   /** 12a, Profile Preferred Bit 4 */
   DSL_uint16_t profilePref4 : 1;
   /** 12b, Profile Preferred Bit 5 */
   DSL_uint16_t profilePref5 : 1;
   /** 17a, Profile Preferred Bit 6 */
   DSL_uint16_t profilePref6 : 1;
   /** 30a, Profile Preferred Bit 7 */
   DSL_uint16_t profilePref7 : 1;
   /** Reserved */
   DSL_uint8_t Res1;
   /** EU-32, Annex A US0 PSDs Supported Bit 0 */
   DSL_uint16_t A_US0PsdSup0 : 1;
   /** EU-36, Annex A US0 PSDs Supported Bit 1 */
   DSL_uint16_t A_US0PsdSup1 : 1;
   /** EU-40, Annex A US0 PSDs Supported Bit 2 */
   DSL_uint16_t A_US0PsdSup2 : 1;
   /** EU-44, Annex A US0 PSDs Supported Bit 3 */
   DSL_uint16_t A_US0PsdSup3 : 1;
   /** EU-48, Annex A US0 PSDs Supported Bit 4 */
   DSL_uint16_t A_US0PsdSup4 : 1;
   /** EU-52, Annex A US0 PSDs Supported Bit 5 */
   DSL_uint16_t A_US0PsdSup5 : 1;
   /** EU-56, Annex A US0 PSDs Supported Bit 6 */
   DSL_uint16_t A_US0PsdSup6 : 1;
   /** EU-60, Annex A US0 PSDs Supported Bit 7 */
   DSL_uint16_t A_US0PsdSup7 : 1;
   /** EU-64, Annex A US0 PSDs Supported Bit 8 */
   DSL_uint16_t A_US0PsdSup8 : 1;
   /** ADLU-32, Annex A US0 PSDs Supported Bit 9 */
   DSL_uint16_t A_US0PsdSup9 : 1;
   /** ADLU-36, Annex A US0 PSDs Supported Bit 10 */
   DSL_uint16_t A_US0PsdSup10 : 1;
   /** ADLU-40, Annex A US0 PSDs Supported Bit 11 */
   DSL_uint16_t A_US0PsdSup11 : 1;
   /** ADLU-44, Annex A US0 PSDs Supported Bit 12 */
   DSL_uint16_t A_US0PsdSup12 : 1;
   /** ADLU-48, Annex A US0 PSDs Supported Bit 13 */
   DSL_uint16_t A_US0PsdSup13 : 1;
   /** ADLU-52, Annex A US0 PSDs Supported Bit 14 */
   DSL_uint16_t A_US0PsdSup14 : 1;
   /** ADLU-56, Annex A US0 PSDs Supported Bit 15 */
   DSL_uint16_t A_US0PsdSup15 : 1;
   /** ADLU-60, Annex A US0 PSDs Supported Bit 16 */
   DSL_uint16_t A_US0PsdSup16 : 1;
   /** ADLU-64, Annex A US0 PSDs Supported Bit 17 */
   DSL_uint16_t A_US0PsdSup17 : 1;
   /** 12b Profile, Annex A US0 PSDs Supported Bit 18 */
   DSL_uint16_t A_US0PsdSup18 : 1;
   /** 17a Profile, Annex A US0 PSDs Supported Bit 19 */
   DSL_uint16_t A_US0PsdSup19 : 1;
   /** EU-128, Annex A US0 PSDs Supported Bit 20 */
   DSL_uint16_t A_US0PsdSup20 : 1;
   /** ADLU-128, Annex A US0 PSDs Supported Bit 21 */
   DSL_uint16_t A_US0PsdSup21 : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 10;
   /** Reserved */
   DSL_uint16_t Res3[2];
   /** US0 In 25 to 138 kHz, Annex B US0 PSDs Supported Bit 0 */
   DSL_uint16_t B_US0PsdSup0 : 1;
   /** US0 In 25 to 276 kHz, Annex B US0 PSDs Supported Bit 1 */
   DSL_uint16_t B_US0PsdSup1 : 1;
   /** US0 In 120 to 276 kHz, Annex B US0 PSDs Supported Bit 2 */
   DSL_uint16_t B_US0PsdSup2 : 1;
   /** 12b Profile, Annex B US0 PSDs Supported Bit 3 */
   DSL_uint16_t B_US0PsdSup3 : 1;
   /** 17a Profile, Annex B US0 PSDs Supported Bit 4 */
   DSL_uint16_t B_US0PsdSup4 : 1;
   /** Reserved */
   DSL_uint16_t Res4 : 11;
   /** Reserved */
   DSL_uint16_t Res5;
   /** US0 In 25 to 138 kHz, Annex C US0 PSDs Supported Bit 0 */
   DSL_uint16_t C_US0PsdSup0 : 1;
   /** US0 In 25 to 276kHz, Annex C US0 PSDs Supported Bit 1 */
   DSL_uint16_t C_US0PsdSup1 : 1;
   /** Reserved */
   DSL_uint16_t Res8 : 2;
   /** US0 In 25 to 138 kHz, Annex C US0 PSDs Supported Bit 4 */
   DSL_uint16_t C_US0PsdSup4 : 1;
   /** US0 In 25 to 276 kHz, Annex C US0 PSDs Supported Bit 5 */
   DSL_uint16_t C_US0PsdSup5 : 1;
   /** Reserved */
   DSL_uint16_t Res7 : 6;
   /** 12b Profile, Annex C US0 PSDs Supported Bit 12 */
   DSL_uint16_t C_US0PsdSup12 : 1;
   /** 17a Profile, Annex C US0 PSDs Supported Bit 13 */
   DSL_uint16_t C_US0PsdSup13 : 1;
   /** Reserved */
   DSL_uint16_t Res6 : 2;
   /** Reserved */
   DSL_uint16_t Res9;
#endif
} __PACKED__ ;


/**
   Acknowledgement to CMD_HS_VDSL2ProfileControl.
*/
struct ACK_HS_VDSL2ProfileControl
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Requests the VDSL2 far-end Standard Info (See G.994.1 Amendment 4 [10],
   NPAR(3) coding).
*/
struct CMD_HS_StandardInfoFE_VDSL2Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t  Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t  Length;
#endif
} __PACKED__ ;


/**
   Delivers the VDSL2 far-end info as requested by
   CMD_HS_StandardInfoFE_VDSL2Get (see G.994.1 Amendment 4 [10], NPAR(3)
   coding).
*/
struct ACK_HS_StandardInfoFE_VDSL2Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t  Length;
   /** Reserved */
   DSL_uint16_t Res0;
   /** Reserved */
   DSL_uint8_t Res1;
   /** 30a, Profile Support Bit 7 */
   DSL_uint16_t profileSup7 : 1;
   /** 17a, Profile Support Bit 6 */
   DSL_uint16_t profileSup6 : 1;
   /** 12b, Profile Support Bit 5 */
   DSL_uint16_t profileSup5 : 1;
   /** 12a, Profile Support Bit 4 */
   DSL_uint16_t profileSup4 : 1;
   /** 8d, Profile Support Bit 3 */
   DSL_uint16_t profileSup3 : 1;
   /** 8c, Profile Support Bit 2 */
   DSL_uint16_t profileSup2 : 1;
   /** 8b, Profile Support Bit 1 */
   DSL_uint16_t profileSup1 : 1;
   /** 8a, Profile Support Bit 0 */
   DSL_uint16_t profileSup0 : 1;
   /** ADLU-56, Annex A US0 PSDs Supported Bit 15 */
   DSL_uint16_t A_US0PsdSup15 : 1;
   /** ADLU-52, Annex A US0 PSDs Supported Bit 14 */
   DSL_uint16_t A_US0PsdSup14 : 1;
   /** ADLU-48, Annex A US0 PSDs Supported Bit 13 */
   DSL_uint16_t A_US0PsdSup13 : 1;
   /** ADLU-44, Annex A US0 PSDs Supported Bit 12 */
   DSL_uint16_t A_US0PsdSup12 : 1;
   /** ADLU-40, Annex A US0 PSDs Supported Bit 11 */
   DSL_uint16_t A_US0PsdSup11 : 1;
   /** ADLU-36, Annex A US0 PSDs Supported Bit 10 */
   DSL_uint16_t A_US0PsdSup10 : 1;
   /** ADLU-32, Annex A US0 PSDs Supported Bit 9 */
   DSL_uint16_t A_US0PsdSup9 : 1;
   /** EU-64, Annex A US0 PSDs Supported Bit 8 */
   DSL_uint16_t A_US0PsdSup8 : 1;
   /** EU-60, Annex A US0 PSDs Supported Bit 7 */
   DSL_uint16_t A_US0PsdSup7 : 1;
   /** EU-56, Annex A US0 PSDs Supported Bit 6 */
   DSL_uint16_t A_US0PsdSup6 : 1;
   /** EU-52, Annex A US0 PSDs Supported Bit 5 */
   DSL_uint16_t A_US0PsdSup5 : 1;
   /** EU-48, Annex A US0 PSDs Supported Bit 4 */
   DSL_uint16_t A_US0PsdSup4 : 1;
   /** EU-44, Annex A US0 PSDs Supported Bit 3 */
   DSL_uint16_t A_US0PsdSup3 : 1;
   /** EU-40, Annex A US0 PSDs Supported Bit 2 */
   DSL_uint16_t A_US0PsdSup2 : 1;
   /** EU-36, Annex A US0 PSDs Supported Bit 1 */
   DSL_uint16_t A_US0PsdSup1 : 1;
   /** EU-32, Annex A US0 PSDs Supported Bit 0 */
   DSL_uint16_t A_US0PsdSup0 : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 10;
   /** ADLU-128 Annex A US0 PSDs Supported Bit 21 */
   DSL_uint16_t A_US0PsdSup21 : 1;
   /** EU-128, Annex A US0 PSDs Supported Bit 20 */
   DSL_uint16_t A_US0PsdSup20 : 1;
   /** 17a Profile, Annex A US0 PSDs Supported Bit 19 */
   DSL_uint16_t A_US0PsdSup19 : 1;
   /** 12b Profile, Annex A US0 PSDs Supported Bit 18 */
   DSL_uint16_t A_US0PsdSup18 : 1;
   /** ADLU-64, Annex A US0 PSDs Supported Bit 17 */
   DSL_uint16_t A_US0PsdSup17 : 1;
   /** ADLU-60, Annex A US0 PSDs Supported Bit 16 */
   DSL_uint16_t A_US0PsdSup16 : 1;
   /** Reserved */
   DSL_uint16_t Res3 : 11;
   /** 17a Profile, Annex B US0 PSDs Supported Bit 4 */
   DSL_uint16_t B_US0PsdSup4 : 1;
   /** 12b Profile, Annex B US0 PSDs Supported Bit 3 */
   DSL_uint16_t B_US0PsdSup3 : 1;
   /** US0 In 138 to 276 kHz, Annex B US0 PSDs Supported Bit 2 */
   DSL_uint16_t B_US0PsdSup2 : 1;
   /** US0 In 25 to 276 kHz, Annex B US0 PSDs Supported Bit 1 */
   DSL_uint16_t B_US0PsdSup1 : 1;
   /** US0 In 25 to 138 kHz, Annex B US0 PSDs Supported Bit 0 */
   DSL_uint16_t B_US0PsdSup0 : 1;
   /** Reserved */
   DSL_uint16_t Res4 : 2;
   /** 17a Profile, Annex C US0 PSDs Supported Bit 13 */
   DSL_uint16_t C_US0PsdSup13 : 1;
   /** 12b Profile, Annex C US0 PSDs Supported Bit 12 */
   DSL_uint16_t C_US0PsdSup12 : 1;
   /** Reserved */
   DSL_uint16_t Res5 : 6;
   /** US0 In 25 to 276 kHz, Annex C US0 PSDs Supported Bit 5 */
   DSL_uint16_t C_US0PsdSup5 : 1;
   /** US0 In 25 to 138 kHz, Annex C US0 PSDs Supported Bit 4 */
   DSL_uint16_t C_US0PsdSup4 : 1;
   /** Reserved */
   DSL_uint16_t Res6 : 2;
   /** US0 In 25 to 276kHz, Annex C US0 PSDs Supported Bit 1 */
   DSL_uint16_t C_US0PsdSup1 : 1;
   /** US0 In 25 to 138 kHz, Annex C US0 PSDs Supported Bit 0 */
   DSL_uint16_t C_US0PsdSup0 : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t  Length;
   /** Reserved */
   DSL_uint16_t Res0;
   /** 8a, Profile Support Bit 0 */
   DSL_uint16_t profileSup0 : 1;
   /** 8b, Profile Support Bit 1 */
   DSL_uint16_t profileSup1 : 1;
   /** 8c, Profile Support Bit 2 */
   DSL_uint16_t profileSup2 : 1;
   /** 8d, Profile Support Bit 3 */
   DSL_uint16_t profileSup3 : 1;
   /** 12a, Profile Support Bit 4 */
   DSL_uint16_t profileSup4 : 1;
   /** 12b, Profile Support Bit 5 */
   DSL_uint16_t profileSup5 : 1;
   /** 17a, Profile Support Bit 6 */
   DSL_uint16_t profileSup6 : 1;
   /** 30a, Profile Support Bit 7 */
   DSL_uint16_t profileSup7 : 1;
   /** Reserved */
   DSL_uint8_t Res1;
   /** EU-32, Annex A US0 PSDs Supported Bit 0 */
   DSL_uint16_t A_US0PsdSup0 : 1;
   /** EU-36, Annex A US0 PSDs Supported Bit 1 */
   DSL_uint16_t A_US0PsdSup1 : 1;
   /** EU-40, Annex A US0 PSDs Supported Bit 2 */
   DSL_uint16_t A_US0PsdSup2 : 1;
   /** EU-44, Annex A US0 PSDs Supported Bit 3 */
   DSL_uint16_t A_US0PsdSup3 : 1;
   /** EU-48, Annex A US0 PSDs Supported Bit 4 */
   DSL_uint16_t A_US0PsdSup4 : 1;
   /** EU-52, Annex A US0 PSDs Supported Bit 5 */
   DSL_uint16_t A_US0PsdSup5 : 1;
   /** EU-56, Annex A US0 PSDs Supported Bit 6 */
   DSL_uint16_t A_US0PsdSup6 : 1;
   /** EU-60, Annex A US0 PSDs Supported Bit 7 */
   DSL_uint16_t A_US0PsdSup7 : 1;
   /** EU-64, Annex A US0 PSDs Supported Bit 8 */
   DSL_uint16_t A_US0PsdSup8 : 1;
   /** ADLU-32, Annex A US0 PSDs Supported Bit 9 */
   DSL_uint16_t A_US0PsdSup9 : 1;
   /** ADLU-36, Annex A US0 PSDs Supported Bit 10 */
   DSL_uint16_t A_US0PsdSup10 : 1;
   /** ADLU-40, Annex A US0 PSDs Supported Bit 11 */
   DSL_uint16_t A_US0PsdSup11 : 1;
   /** ADLU-44, Annex A US0 PSDs Supported Bit 12 */
   DSL_uint16_t A_US0PsdSup12 : 1;
   /** ADLU-48, Annex A US0 PSDs Supported Bit 13 */
   DSL_uint16_t A_US0PsdSup13 : 1;
   /** ADLU-52, Annex A US0 PSDs Supported Bit 14 */
   DSL_uint16_t A_US0PsdSup14 : 1;
   /** ADLU-56, Annex A US0 PSDs Supported Bit 15 */
   DSL_uint16_t A_US0PsdSup15 : 1;
   /** ADLU-60, Annex A US0 PSDs Supported Bit 16 */
   DSL_uint16_t A_US0PsdSup16 : 1;
   /** ADLU-64, Annex A US0 PSDs Supported Bit 17 */
   DSL_uint16_t A_US0PsdSup17 : 1;
   /** 12b Profile, Annex A US0 PSDs Supported Bit 18 */
   DSL_uint16_t A_US0PsdSup18 : 1;
   /** 17a Profile, Annex A US0 PSDs Supported Bit 19 */
   DSL_uint16_t A_US0PsdSup19 : 1;
   /** EU-128, Annex A US0 PSDs Supported Bit 20 */
   DSL_uint16_t A_US0PsdSup20 : 1;
   /** ADLU-128 Annex A US0 PSDs Supported Bit 21 */
   DSL_uint16_t A_US0PsdSup21 : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 10;
   /** US0 In 25 to 138 kHz, Annex B US0 PSDs Supported Bit 0 */
   DSL_uint16_t B_US0PsdSup0 : 1;
   /** US0 In 25 to 276 kHz, Annex B US0 PSDs Supported Bit 1 */
   DSL_uint16_t B_US0PsdSup1 : 1;
   /** US0 In 138 to 276 kHz, Annex B US0 PSDs Supported Bit 2 */
   DSL_uint16_t B_US0PsdSup2 : 1;
   /** 12b Profile, Annex B US0 PSDs Supported Bit 3 */
   DSL_uint16_t B_US0PsdSup3 : 1;
   /** 17a Profile, Annex B US0 PSDs Supported Bit 4 */
   DSL_uint16_t B_US0PsdSup4 : 1;
   /** Reserved */
   DSL_uint16_t Res3 : 11;
   /** US0 In 25 to 138 kHz, Annex C US0 PSDs Supported Bit 0 */
   DSL_uint16_t C_US0PsdSup0 : 1;
   /** US0 In 25 to 276kHz, Annex C US0 PSDs Supported Bit 1 */
   DSL_uint16_t C_US0PsdSup1 : 1;
   /** Reserved */
   DSL_uint16_t Res6 : 2;
   /** US0 In 25 to 138 kHz, Annex C US0 PSDs Supported Bit 4 */
   DSL_uint16_t C_US0PsdSup4 : 1;
   /** US0 In 25 to 276 kHz, Annex C US0 PSDs Supported Bit 5 */
   DSL_uint16_t C_US0PsdSup5 : 1;
   /** Reserved */
   DSL_uint16_t Res5 : 6;
   /** 12b Profile, Annex C US0 PSDs Supported Bit 12 */
   DSL_uint16_t C_US0PsdSup12 : 1;
   /** 17a Profile, Annex C US0 PSDs Supported Bit 13 */
   DSL_uint16_t C_US0PsdSup13 : 1;
   /** Reserved */
   DSL_uint16_t Res4 : 2;
#endif
} __PACKED__ ;


/**
   Requests the actual selected VDSL2 Profile (See G.994.1 Amendment 4 [10],
   NPAR(3) coding).
*/
struct CMD_HS_SelectedProfileVDSL2Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t  Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t  Length;
#endif
} __PACKED__ ;


/**
   Provides the actual selected VDSL2 Profile as requested by
   CMD_HS_SelectedProfileVDSL2Get (see G.994.1 Amendment 4 [10], NPAR(3)
   coding).
*/
struct ACK_HS_SelectedProfileVDSL2Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t  Length;
   /** Reserved */
   DSL_uint16_t Res0 : 12;
   /** G.993.2 Annex N, Selected Bit 3 */
   DSL_uint16_t dsmSel3 : 1;
   /** G.993.2 Annex O, Selected Bit 2 */
   DSL_uint16_t dsmSel2 : 1;
   /** G.993.5 DS Vectoring, Selected Bit 1 */
   DSL_uint16_t dsmSel1 : 1;
   /** G.993.5 DS+US Vectoring, Selected Bit 0 */
   DSL_uint16_t dsmSel0 : 1;
   /** Reserved */
   DSL_uint8_t Res1;
   /** 30a, Profile Selected Bit 7 */
   DSL_uint16_t profileSel7 : 1;
   /** 17a, Profile Selected Bit 6 */
   DSL_uint16_t profileSel6 : 1;
   /** 12b, Profile Selected Bit 5 */
   DSL_uint16_t profileSel5 : 1;
   /** 12a, Profile Selected Bit 4 */
   DSL_uint16_t profileSel4 : 1;
   /** 8d, Profile Selected Bit 3 */
   DSL_uint16_t profileSel3 : 1;
   /** 8c, Profile Selected Bit 2 */
   DSL_uint16_t profileSel2 : 1;
   /** 8b, Profile Selected Bit 1 */
   DSL_uint16_t profileSel1 : 1;
   /** 8a, Profile Selected Bit 0 */
   DSL_uint16_t profileSel0 : 1;
   /** ADLU-56, Annex A US0 PSDs Selected Bit  15 */
   DSL_uint16_t A_US0PsdSel15 : 1;
   /** ADLU-52, Annex A US0 PSDs Selected Bit  14 */
   DSL_uint16_t A_US0PsdSel14 : 1;
   /** ADLU-48, Annex A US0 PSDs Selected Bit  13 */
   DSL_uint16_t A_US0PsdSel13 : 1;
   /** ADLU-44, Annex A US0 PSDs Selected Bit  12 */
   DSL_uint16_t A_US0PsdSel12 : 1;
   /** ADLU-40, Annex A US0 PSDs Selected Bit  11 */
   DSL_uint16_t A_US0PsdSel11 : 1;
   /** ADLU-36, Annex A US0 PSDs Selected Bit  10 */
   DSL_uint16_t A_US0PsdSel10 : 1;
   /** ADLU-32, Annex A US0 PSDs Selected Bit  9 */
   DSL_uint16_t A_US0PsdSel9 : 1;
   /** EU-64, Annex A US0 PSDs Selected Bit  8 */
   DSL_uint16_t A_US0PsdSel8 : 1;
   /** EU-60, Annex A US0 PSDs Selected Bit  7 */
   DSL_uint16_t A_US0PsdSel7 : 1;
   /** EU-56, Annex A US0 PSDs Selected Bit  6 */
   DSL_uint16_t A_US0PsdSel6 : 1;
   /** EU-52, Annex A US0 PSDs Selected Bit  5 */
   DSL_uint16_t A_US0PsdSel5 : 1;
   /** EU-48, Annex A US0 PSDs Selected Bit  4 */
   DSL_uint16_t A_US0PsdSel4 : 1;
   /** EU-44, Annex A US0 PSDs Selected Bit  3 */
   DSL_uint16_t A_US0PsdSel3 : 1;
   /** EU-40, Annex A US0 PSDs Selected Bit  2 */
   DSL_uint16_t A_US0PsdSel2 : 1;
   /** EU-36, Annex A US0 PSDs Selected Bit  1 */
   DSL_uint16_t A_US0PsdSel1 : 1;
   /** EU-32, Annex A US0 PSDs Selected Bit  0 */
   DSL_uint16_t A_US0PsdSel0 : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 10;
   /** ADLU-128 Annex A US0 PSDs Selected Bit 21 */
   DSL_uint16_t A_US0PsdSel21 : 1;
   /** EU-128, Annex A US0 PSDs Selected Bit 20 */
   DSL_uint16_t A_US0PsdSel20 : 1;
   /** Reserved */
   DSL_uint16_t Res3 : 2;
   /** ADLU-64, Annex A US0 PSDs Selected Bit 17 */
   DSL_uint16_t A_US0PsdSel17 : 1;
   /** ADLU-60, Annex A US0 PSDs Selected Bit 16 */
   DSL_uint16_t A_US0PsdSel16 : 1;
   /** Reserved */
   DSL_uint16_t Res4 : 13;
   /** US0 In 120 to 276 kHz, Annex B US0 PSDs Selected Bit 2 */
   DSL_uint16_t B_US0PsdSel2 : 1;
   /** US0 In 25 to 276 kHz, Annex B US0 PSDs Selected Bit 1 */
   DSL_uint16_t B_US0PsdSel1 : 1;
   /** US0 In 25 to 138 kHz, Annex B US0 PSDs Selected Bit 0 */
   DSL_uint16_t B_US0PsdSel0 : 1;
   /** Reserved */
   DSL_uint16_t Res5 : 10;
   /** US0 In 25 to 276 kHz, Annex C US0 PSDs Selected Bit 5 */
   DSL_uint16_t C_US0PsdSel5 : 1;
   /** US0 In 25 to 138 kHz, Annex C US0 PSDs Selected Bit 4 */
   DSL_uint16_t C_US0PsdSel4 : 1;
   /** Reserved */
   DSL_uint16_t Res6 : 2;
   /** US0 In 25 to 276kHz, Annex C US0 PSDs Selected Bit 1 */
   DSL_uint16_t C_US0PsdSel1 : 1;
   /** US0 In 25 to 138 kHz, Annex C US0 PSDs Selected Bit 0 */
   DSL_uint16_t C_US0PsdSel0 : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t  Length;
   /** G.993.5 DS+US Vectoring, Selected Bit 0 */
   DSL_uint16_t dsmSel0 : 1;
   /** G.993.5 DS Vectoring, Selected Bit 1 */
   DSL_uint16_t dsmSel1 : 1;
   /** G.993.2 Annex O, Selected Bit 2 */
   DSL_uint16_t dsmSel2 : 1;
   /** G.993.2 Annex N, Selected Bit 3 */
   DSL_uint16_t dsmSel3 : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 12;
   /** 8a, Profile Selected Bit 0 */
   DSL_uint16_t profileSel0 : 1;
   /** 8b, Profile Selected Bit 1 */
   DSL_uint16_t profileSel1 : 1;
   /** 8c, Profile Selected Bit 2 */
   DSL_uint16_t profileSel2 : 1;
   /** 8d, Profile Selected Bit 3 */
   DSL_uint16_t profileSel3 : 1;
   /** 12a, Profile Selected Bit 4 */
   DSL_uint16_t profileSel4 : 1;
   /** 12b, Profile Selected Bit 5 */
   DSL_uint16_t profileSel5 : 1;
   /** 17a, Profile Selected Bit 6 */
   DSL_uint16_t profileSel6 : 1;
   /** 30a, Profile Selected Bit 7 */
   DSL_uint16_t profileSel7 : 1;
   /** Reserved */
   DSL_uint8_t Res1;
   /** EU-32, Annex A US0 PSDs Selected Bit  0 */
   DSL_uint16_t A_US0PsdSel0 : 1;
   /** EU-36, Annex A US0 PSDs Selected Bit  1 */
   DSL_uint16_t A_US0PsdSel1 : 1;
   /** EU-40, Annex A US0 PSDs Selected Bit  2 */
   DSL_uint16_t A_US0PsdSel2 : 1;
   /** EU-44, Annex A US0 PSDs Selected Bit  3 */
   DSL_uint16_t A_US0PsdSel3 : 1;
   /** EU-48, Annex A US0 PSDs Selected Bit  4 */
   DSL_uint16_t A_US0PsdSel4 : 1;
   /** EU-52, Annex A US0 PSDs Selected Bit  5 */
   DSL_uint16_t A_US0PsdSel5 : 1;
   /** EU-56, Annex A US0 PSDs Selected Bit  6 */
   DSL_uint16_t A_US0PsdSel6 : 1;
   /** EU-60, Annex A US0 PSDs Selected Bit  7 */
   DSL_uint16_t A_US0PsdSel7 : 1;
   /** EU-64, Annex A US0 PSDs Selected Bit  8 */
   DSL_uint16_t A_US0PsdSel8 : 1;
   /** ADLU-32, Annex A US0 PSDs Selected Bit  9 */
   DSL_uint16_t A_US0PsdSel9 : 1;
   /** ADLU-36, Annex A US0 PSDs Selected Bit  10 */
   DSL_uint16_t A_US0PsdSel10 : 1;
   /** ADLU-40, Annex A US0 PSDs Selected Bit  11 */
   DSL_uint16_t A_US0PsdSel11 : 1;
   /** ADLU-44, Annex A US0 PSDs Selected Bit  12 */
   DSL_uint16_t A_US0PsdSel12 : 1;
   /** ADLU-48, Annex A US0 PSDs Selected Bit  13 */
   DSL_uint16_t A_US0PsdSel13 : 1;
   /** ADLU-52, Annex A US0 PSDs Selected Bit  14 */
   DSL_uint16_t A_US0PsdSel14 : 1;
   /** ADLU-56, Annex A US0 PSDs Selected Bit  15 */
   DSL_uint16_t A_US0PsdSel15 : 1;
   /** ADLU-60, Annex A US0 PSDs Selected Bit 16 */
   DSL_uint16_t A_US0PsdSel16 : 1;
   /** ADLU-64, Annex A US0 PSDs Selected Bit 17 */
   DSL_uint16_t A_US0PsdSel17 : 1;
   /** Reserved */
   DSL_uint16_t Res3 : 2;
   /** EU-128, Annex A US0 PSDs Selected Bit 20 */
   DSL_uint16_t A_US0PsdSel20 : 1;
   /** ADLU-128 Annex A US0 PSDs Selected Bit 21 */
   DSL_uint16_t A_US0PsdSel21 : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 10;
   /** US0 In 25 to 138 kHz, Annex B US0 PSDs Selected Bit 0 */
   DSL_uint16_t B_US0PsdSel0 : 1;
   /** US0 In 25 to 276 kHz, Annex B US0 PSDs Selected Bit 1 */
   DSL_uint16_t B_US0PsdSel1 : 1;
   /** US0 In 120 to 276 kHz, Annex B US0 PSDs Selected Bit 2 */
   DSL_uint16_t B_US0PsdSel2 : 1;
   /** Reserved */
   DSL_uint16_t Res4 : 13;
   /** US0 In 25 to 138 kHz, Annex C US0 PSDs Selected Bit 0 */
   DSL_uint16_t C_US0PsdSel0 : 1;
   /** US0 In 25 to 276kHz, Annex C US0 PSDs Selected Bit 1 */
   DSL_uint16_t C_US0PsdSel1 : 1;
   /** Reserved */
   DSL_uint16_t Res6 : 2;
   /** US0 In 25 to 138 kHz, Annex C US0 PSDs Selected Bit 4 */
   DSL_uint16_t C_US0PsdSel4 : 1;
   /** US0 In 25 to 276 kHz, Annex C US0 PSDs Selected Bit 5 */
   DSL_uint16_t C_US0PsdSel5 : 1;
   /** Reserved */
   DSL_uint16_t Res5 : 10;
#endif
} __PACKED__ ;


/**
   Requests the handshake information about the far-end operating mode (See
   G.994.1 [6] and Amendments 1-4 [7]-[10], standard information fields- SPAR(1)
   coding).
*/
struct CMD_HS_StandardInfoFE_SPAR1Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t  Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t  Length;
#endif
} __PACKED__ ;


/**
   Provides the handshake information about the far-end operating mode (See
   G.994.1 [6] and Amendments 1-4 [7]-[10], Standard information field - SPAR(1)
   coding). The bit numbering per octet below follows the standard (starting
   with "1").
*/
struct ACK_HS_StandardInfoFE_SPAR1Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t  Length;
   /** Reserved */
   DSL_uint16_t Res0 : 1;
   /** Octet 2, Bit 7: Committee T1 enhanced SHDSL */
   DSL_uint16_t Oct2B7 : 1;
   /** Octet 2, Bit 6: ETSI SCM VDSL */
   DSL_uint16_t Oct2B6 : 1;
   /** Octet 2, Bit 5: ETSI MCM VDSL */
   DSL_uint16_t Oct2B5 : 1;
   /** Octet 2, Bit 4: T1E1 Trial-use SCM VDSL */
   DSL_uint16_t Oct2B4 : 1;
   /** Octet 2, Bit 3: T1E1 Trial-use MCM VDSL */
   DSL_uint16_t Oct2B3 : 1;
   /** Octet 2, Bit 2: G991.2 Annex B/G */
   DSL_uint16_t Oct2B2 : 1;
   /** Octet 2, Bit 1: G991.2 Annex A/F */
   DSL_uint16_t Oct2B1 : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 1;
   /** Octet 1, Bit 7: G992.1 Annex I */
   DSL_uint16_t Oct1B7 : 1;
   /** Octet 1, Bit 6: G992.1 Annex H */
   DSL_uint16_t Oct1B6 : 1;
   /** Octet 1, Bit 5: G992.2 Annex C */
   DSL_uint16_t Oct1B5 : 1;
   /** Octet 1, Bit 4: G992.2 Annex A/B */
   DSL_uint16_t Oct1B4 : 1;
   /** Octet 1, Bit 3: G992.1 Annex C */
   DSL_uint16_t Oct1B3 : 1;
   /** Octet 1, Bit 2: G992.1 Annex B */
   DSL_uint16_t Oct1B2 : 1;
   /** Octet 1, Bit 1: G992.1 Annex A */
   DSL_uint16_t Oct1B1 : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 1;
   /** Octet 4, Bit 7: IEEE 802.3ah 2BASE-TS */
   DSL_uint16_t Oct4B7 : 1;
   /** Octet 4, Bit 6: IEEE 802.3ah 2BASE-TL */
   DSL_uint16_t Oct4B6 : 1;
   /** Octet 4, Bit 5: G992.5 Annex J */
   DSL_uint16_t Oct4B5 : 1;
   /** Octet 4, Bit 4: G992.3 Annex M */
   DSL_uint16_t Oct4B4 : 1;
   /** Octet 4, Bit 3: G992.5 Annex I */
   DSL_uint16_t Oct4B3 : 1;
   /** Octet 4, Bit 2: G992.5 Annex B */
   DSL_uint16_t Oct4B2 : 1;
   /** Octet 4, Bit 1: G992.5 Annex A */
   DSL_uint16_t Oct4B1 : 1;
   /** Reserved */
   DSL_uint16_t Res3 : 1;
   /** Octet 3, Bit 7: G992.3 Annex C */
   DSL_uint16_t Oct3B7 : 1;
   /** Octet 3, Bit 6: G992.4 Annex I */
   DSL_uint16_t Oct3B6 : 1;
   /** Octet 3, Bit 5: G992.4 Annex A */
   DSL_uint16_t Oct3B5 : 1;
   /** Octet 3, Bit 4: G992.3 Annex J */
   DSL_uint16_t Oct3B4 : 1;
   /** Octet 3, Bit 3: G992.3 Annex I */
   DSL_uint16_t Oct3B3 : 1;
   /** Octet 3, Bit 2: G992.3 Annex B */
   DSL_uint16_t Oct3B2 : 1;
   /** Octet 3, Bit 1: G992.3 Annex A/L */
   DSL_uint16_t Oct3B1 : 1;
   /** Reserved */
   DSL_uint8_t Res4;
   /** Reserved */
   DSL_uint16_t Res5 : 2;
   /** Octet 5, Bit 5: G993.2 */
   DSL_uint16_t Oct5B6 : 1;
   /** Octet 5, Bit 4: G992.5 Annex C */
   DSL_uint16_t Oct5B5 : 1;
   /** Octet 5, Bit 3: Variable Silence Period */
   DSL_uint16_t Oct5B4 : 1;
   /** Octet 5, Bit 2: G992.1 Annex I / T1E1 TRQ 12 */
   DSL_uint16_t Oct5B3 : 1;
   /** Octet 5, Bit 1: G993.1 / ANSI T1.424 */
   DSL_uint16_t Oct5B2 : 1;
   /** Octet 5, Bit 0: G992.5 Annex M */
   DSL_uint16_t Oct5B1 : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t  Length;
   /** Octet 1, Bit 1: G992.1 Annex A */
   DSL_uint16_t Oct1B1 : 1;
   /** Octet 1, Bit 2: G992.1 Annex B */
   DSL_uint16_t Oct1B2 : 1;
   /** Octet 1, Bit 3: G992.1 Annex C */
   DSL_uint16_t Oct1B3 : 1;
   /** Octet 1, Bit 4: G992.2 Annex A/B */
   DSL_uint16_t Oct1B4 : 1;
   /** Octet 1, Bit 5: G992.2 Annex C */
   DSL_uint16_t Oct1B5 : 1;
   /** Octet 1, Bit 6: G992.1 Annex H */
   DSL_uint16_t Oct1B6 : 1;
   /** Octet 1, Bit 7: G992.1 Annex I */
   DSL_uint16_t Oct1B7 : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 1;
   /** Octet 2, Bit 1: G991.2 Annex A/F */
   DSL_uint16_t Oct2B1 : 1;
   /** Octet 2, Bit 2: G991.2 Annex B/G */
   DSL_uint16_t Oct2B2 : 1;
   /** Octet 2, Bit 3: T1E1 Trial-use MCM VDSL */
   DSL_uint16_t Oct2B3 : 1;
   /** Octet 2, Bit 4: T1E1 Trial-use SCM VDSL */
   DSL_uint16_t Oct2B4 : 1;
   /** Octet 2, Bit 5: ETSI MCM VDSL */
   DSL_uint16_t Oct2B5 : 1;
   /** Octet 2, Bit 6: ETSI SCM VDSL */
   DSL_uint16_t Oct2B6 : 1;
   /** Octet 2, Bit 7: Committee T1 enhanced SHDSL */
   DSL_uint16_t Oct2B7 : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 1;
   /** Octet 3, Bit 1: G992.3 Annex A/L */
   DSL_uint16_t Oct3B1 : 1;
   /** Octet 3, Bit 2: G992.3 Annex B */
   DSL_uint16_t Oct3B2 : 1;
   /** Octet 3, Bit 3: G992.3 Annex I */
   DSL_uint16_t Oct3B3 : 1;
   /** Octet 3, Bit 4: G992.3 Annex J */
   DSL_uint16_t Oct3B4 : 1;
   /** Octet 3, Bit 5: G992.4 Annex A */
   DSL_uint16_t Oct3B5 : 1;
   /** Octet 3, Bit 6: G992.4 Annex I */
   DSL_uint16_t Oct3B6 : 1;
   /** Octet 3, Bit 7: G992.3 Annex C */
   DSL_uint16_t Oct3B7 : 1;
   /** Reserved */
   DSL_uint16_t Res3 : 1;
   /** Octet 4, Bit 1: G992.5 Annex A */
   DSL_uint16_t Oct4B1 : 1;
   /** Octet 4, Bit 2: G992.5 Annex B */
   DSL_uint16_t Oct4B2 : 1;
   /** Octet 4, Bit 3: G992.5 Annex I */
   DSL_uint16_t Oct4B3 : 1;
   /** Octet 4, Bit 4: G992.3 Annex M */
   DSL_uint16_t Oct4B4 : 1;
   /** Octet 4, Bit 5: G992.5 Annex J */
   DSL_uint16_t Oct4B5 : 1;
   /** Octet 4, Bit 6: IEEE 802.3ah 2BASE-TL */
   DSL_uint16_t Oct4B6 : 1;
   /** Octet 4, Bit 7: IEEE 802.3ah 2BASE-TS */
   DSL_uint16_t Oct4B7 : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 1;
   /** Octet 5, Bit 0: G992.5 Annex M */
   DSL_uint16_t Oct5B1 : 1;
   /** Octet 5, Bit 1: G993.1 / ANSI T1.424 */
   DSL_uint16_t Oct5B2 : 1;
   /** Octet 5, Bit 2: G992.1 Annex I / T1E1 TRQ 12 */
   DSL_uint16_t Oct5B3 : 1;
   /** Octet 5, Bit 3: Variable Silence Period */
   DSL_uint16_t Oct5B4 : 1;
   /** Octet 5, Bit 4: G992.5 Annex C */
   DSL_uint16_t Oct5B5 : 1;
   /** Octet 5, Bit 5: G993.2 */
   DSL_uint16_t Oct5B6 : 1;
   /** Reserved */
   DSL_uint16_t Res5 : 2;
   /** Reserved */
   DSL_uint8_t Res4;
#endif
} __PACKED__ ;


/**
   Sets the in System Vendor ID at the xTU-R. (Section 7.4.4 of G997.1 [11])
*/
struct CMD_SysVendorID_R_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-R System Vendor ID */
   DSL_uint16_t sysVendorID[4];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-R System Vendor ID */
   DSL_uint16_t sysVendorID[4];
#endif
} __PACKED__ ;


/**
   Acknowledgement to CMD_SysVendorID_R_Set.
*/
struct ACK_SysVendorID_R_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Sets the in system vendor version number at the xTU-R. (Section 7.4.6 of
   G997.1 [11])
*/
struct CMD_SysVendorVersionNum_R_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-R System Vendor Version Number */
   DSL_uint16_t versionNum[8];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-R System Vendor Version Number */
   DSL_uint16_t versionNum[8];
#endif
} __PACKED__ ;


/**
   Acknowledgement to CMD_SysVendorVersionNum_R_Set.
*/
struct ACK_SysVendorVersionNum_R_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Sets the system vendor serial number at the xTU-R. (Section 7.4.8 of G997.1
   [11])
*/
struct CMD_SysVendorSerialNum_R_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-R System Vendor Serial Number */
   DSL_uint16_t serialNum[16];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-R System Vendor Serial Number */
   DSL_uint16_t serialNum[16];
#endif
} __PACKED__ ;


/**
   Acknowledgement to CMD_SysVendorSerialNum_R_Set.
*/
struct ACK_SysVendorSerialNum_R_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Sets auxiliary inventory information at the VTU-R (¤11.2.3.2.4 of G993.2
   [5]).
*/
struct CMD_AuxInventoryInfo_R_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** VTU-R Auxiliary Inventory Information */
   DSL_uint16_t auxInfo[32];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** VTU-R Auxiliary Inventory Information */
   DSL_uint16_t auxInfo[32];
#endif
} __PACKED__ ;


/**
   Acknowledgement to CMD_AuxInventoryInfo_R_Set.
*/
struct ACK_AuxInventoryInfo_R_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Requests the xTU-C Vendor ID. (Section 7.4.1 of G997.1 [11])
*/
struct CMD_VendorID_O_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Provides the xTU-C Vendor ID as requested by CMD_VendorID_O_Get.
*/
struct ACK_VendorID_O_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-C G.994.1 Vendor ID */
   DSL_uint16_t vendorID[4];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-C G.994.1 Vendor ID */
   DSL_uint16_t vendorID[4];
#endif
} __PACKED__ ;


/**
   Requests the xTU-R Vendor ID. (Section 7.4.2 of G997.1 [11])
*/
struct CMD_VendorID_R_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Provides the xTU-R Vendor ID as requested by CMD_VendorID_R_Get.
*/
struct ACK_VendorID_R_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-R G.994.1 Vendor ID */
   DSL_uint16_t vendorID[4];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-R G.994.1 Vendor ID */
   DSL_uint16_t vendorID[4];
#endif
} __PACKED__ ;


/**
   Requests the xTU-C System Vendor ID. (Section 7.4.3 of G997.1 [11])
*/
struct CMD_SysVendorID_O_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers the xTU-C System Vendor ID as requested by CMD_SysVendorID_O_Get.
*/
struct ACK_SysVendorID_O_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-C System Vendor ID */
   DSL_uint16_t sysVendorID[4];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-C System Vendor ID */
   DSL_uint16_t sysVendorID[4];
#endif
} __PACKED__ ;


/**
   Requests the xTU-R System Vendor ID. (Section 7.4.4 of G997.1 [11])
*/
struct CMD_SysVendorID_R_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers the xTU-R System Vendor ID as requested by CMD_SysVendorID_R_Get.
*/
struct ACK_SysVendorID_R_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-R System Vendor ID */
   DSL_uint16_t sysVendorID[4];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-R System Vendor ID */
   DSL_uint16_t sysVendorID[4];
#endif
} __PACKED__ ;


/**
   Requests the xTU-C system vendor version number. (Section 7.4.5 of G997.1
   [11])
*/
struct CMD_SysVendorVersionNumO_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers the xTU-C system vendor version number as requested by
   CMD_SysVendorVersionNumO_Get.
*/
struct ACK_SysVendorVersionNum_O_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-C System Vendor Version Number */
   DSL_uint16_t versionNum[8];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-C System Vendor Version Number */
   DSL_uint16_t versionNum[8];
#endif
} __PACKED__ ;


/**
   Requests the xTU-R system vendor version number. (Section 7.4.6 of G997.1
   [11])
*/
struct CMD_SysVendorVersionNum_R_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers the xTU-R system vendor version number as requested by
   CMD_SysVendorVersionNum_R_Get.
*/
struct ACK_SysVendorVersionNum_R_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-R System Vendor Version Number */
   DSL_uint16_t versionNum[8];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-R System Vendor Version Number */
   DSL_uint16_t versionNum[8];
#endif
} __PACKED__ ;


/**
   Requests the xTU-C system vendor serial number. (Section 7.4.7 of G997.1
   [11])
*/
struct CMD_SysVendorSerialNum_O_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers the xTU-C system vendor serial number as requested by
   CMD_SysVendorSerialNum_O_Get.
*/
struct ACK_SysVendorSerialNum_O_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-C System Vendor Serial Number */
   DSL_uint16_t serialNum[16];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-C System Vendor Serial Number */
   DSL_uint16_t serialNum[16];
#endif
} __PACKED__ ;


/**
   Requests the xTU-R system vendor serial number. (Section 7.4.8 of G997.1
   [11])
*/
struct CMD_SysVendorSerialNum_R_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers the xTU-R system vendor version number as requested by
   CMD_SysVendorSerialNum_R_Get.
*/
struct ACK_SysVendorSerialNum_R_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-R System Vendor Serial Number */
   DSL_uint16_t serialNum[16];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-R System Vendor Serial Number */
   DSL_uint16_t serialNum[16];
#endif
} __PACKED__ ;


/**
   Requests VTU-O auxiliary inventory information (¤11.2.3.2.4 of G993.2 [5]).
*/
struct CMD_AuxInventoryInfo_O_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers the VTU-O auxiliary inventory information as requested by
   CMD_AuxInventoryInfo_O_Get.
*/
struct ACK_AuxInventoryInfo_O_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** VTU-O Auxiliary Inventory Information */
   DSL_uint16_t auxInfo[32];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** VTU-O Auxiliary Inventory Information */
   DSL_uint16_t auxInfo[32];
#endif
} __PACKED__ ;


/**
   Requests VTU-R auxiliary inventory information (¤11.2.3.2.4 of G993.2 [5]).
*/
struct CMD_AuxInventoryInfo_R_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers the VTU-R auxiliary inventory information as requested by
   CMD_AuxInventoryInfo_R_Get.
*/
struct ACK_AuxInventoryInfo_R_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** VTU-O Auxiliary Inventory Information */
   DSL_uint16_t auxInfo[32];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** VTU-O Auxiliary Inventory Information */
   DSL_uint16_t auxInfo[32];
#endif
} __PACKED__ ;


/**
   Requests the ATU-CÕs T1.413 Vendor identification. (Section 9.6.4.2 of ANSI
   T1.413 [16])
*/
struct CMD_T1413_VendorID_O_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers the ATU-CÕs T1.413 vendor identification number.
*/
struct ACK_T1413_VendorID_O_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** T1.413 Vendor ID */
   DSL_uint16_t T1413VendorID;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** T1.413 Vendor ID */
   DSL_uint16_t T1413VendorID;
#endif
} __PACKED__ ;


/**
   Requests the ATU-RÕs T1.413 Vendor identification. (Section 9.6.4.2 of ANSI
   T1.413 [16])
*/
struct CMD_T1413_VendorID_R_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Reports the ATU-RÕs T1.413 vendor identification number.
*/
struct ACK_T1413_VendorID_R_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** T1.413 Vendor ID */
   DSL_uint16_t T1413VendorID;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** T1.413 Vendor ID */
   DSL_uint16_t T1413VendorID;
#endif
} __PACKED__ ;


/**
   Requests the xTU-OÕs T1.413 revision number. (Section 9.6.4.3 of ANSI T1.413
   [16])
*/
struct CMD_T1413_RevNo_O_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers the xTU-OÕs T1.413 revision number.
*/
struct ACK_T1413_RevNo_O_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** T1.413 Revision Number */
   DSL_uint16_t T1413RevNo;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** T1.413 Revision Number */
   DSL_uint16_t T1413RevNo;
#endif
} __PACKED__ ;


/**
   Requests the xTU-RÕs T1.413 revision number. (Section 9.7.6.2 of ANSI T1.413
   [16])
*/
struct CMD_T1413_RevNo_R_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers the xTU-RÕs T1.413 revision number.
*/
struct ACK_T1413_RevNo_R_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** T1.413 Revision Number */
   DSL_uint16_t T1413RevNo;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** T1.413 Revision Number */
   DSL_uint16_t T1413RevNo;
#endif
} __PACKED__ ;


/**
   Requests the xTU-OÕs T1.413 vendor revision number. (Section 9.6.4.4 of ANSI
   T1.413 [16])
*/
struct CMD_T1413_VendorRevNo_O_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers the xTU-OÕs T1.413 vendor revision number.
*/
struct ACK_T1413_VendorRevNo_O_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** T1.413 Vendor Revision Number */
   DSL_uint16_t T1413VendorRevNo;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** T1.413 Vendor Revision Number */
   DSL_uint16_t T1413VendorRevNo;
#endif
} __PACKED__ ;


/**
   Requests the xTU-RÕs T1.413 vendor revision number. (Section 9.7.6.3 of ANSI
   T1.413 [16])
*/
struct CMD_T1413_VendorRevNo_R_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers the xTU-RÕs T1.413 vendor revision number.
*/
struct ACK_T1413_VendorRevNo_R_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** T1.413 Vendor Revision Number */
   DSL_uint16_t T1413VendorRevNo;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** T1.413 Vendor Revision Number */
   DSL_uint16_t T1413VendorRevNo;
#endif
} __PACKED__ ;


/**
   Requests the transmission system in use on the line (DSL flavour). (Section
   7.5.1.1. of G.997.1 [11]) See note at ACK_XTSE_StatusGet about the expected
   behaviour.
*/
struct CMD_XTSE_StatusGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Reports the transmission system mode in use as requested by
   CMD_XTSE_StatusGet.The information is set by the FW after mode select (MS)
   during the handshake phase or during T1.413 state. Before mode selection the
   value "0" is reported (=none), after selection of a common mode exactly this
   single mode must be indicated. If there is no common mode, "0" is to be
   reported (here modem enters FAIL state).
*/
struct ACK_XTSE_StatusGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** ADSL Mode-Bit15: G.992.5, Annex A */
   DSL_uint16_t A15 : 1;
   /** ADSL Mode-Bit14: G.992.5, Annex B */
   DSL_uint16_t A14 : 1;
   /** ADSL Mode-Bit13: G.992.3, Annex M */
   DSL_uint16_t A13 : 1;
   /** ADSL Mode-Bit12: G.992.3, Annex L, US Mask2 */
   DSL_uint16_t A12 : 1;
   /** ADSL Mode-Bit11: G.992.3, Annex L, US Mask1 */
   DSL_uint16_t A11 : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 1;
   /** ADSL Mode-Bit9: G.992.3, Annex B */
   DSL_uint16_t A9 : 1;
   /** ADSL Mode-Bit8: G.992.3, Annex A */
   DSL_uint16_t A8 : 1;
   /** ADSL Mode-Bit7: G.992.5, Annex J */
   DSL_uint16_t A7 : 1;
   /** ADSL Mode-Bit6: G.992.5, Annex I */
   DSL_uint16_t A6 : 1;
   /** ADSL Mode-Bit5: G.992.3, Annex J */
   DSL_uint16_t A5 : 1;
   /** ADSL Mode-Bit4: G.992.3, Annex I */
   DSL_uint16_t A4 : 1;
   /** ADSL Mode-Bit3: G.992.1, Annex B */
   DSL_uint16_t A3 : 1;
   /** ADSL Mode-Bit2: G.992.1, Annex A */
   DSL_uint16_t A2 : 1;
   /** ADSL Mode-Bit1: G.992.5, Annex M */
   DSL_uint16_t A1 : 1;
   /** ADSL Mode-Bit0: T1.413 */
   DSL_uint16_t A0 : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 13;
   /** VDSL Mode-Bit2: VDSL2 */
   DSL_uint16_t V2 : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 2;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** ADSL Mode-Bit0: T1.413 */
   DSL_uint16_t A0 : 1;
   /** ADSL Mode-Bit1: G.992.5, Annex M */
   DSL_uint16_t A1 : 1;
   /** ADSL Mode-Bit2: G.992.1, Annex A */
   DSL_uint16_t A2 : 1;
   /** ADSL Mode-Bit3: G.992.1, Annex B */
   DSL_uint16_t A3 : 1;
   /** ADSL Mode-Bit4: G.992.3, Annex I */
   DSL_uint16_t A4 : 1;
   /** ADSL Mode-Bit5: G.992.3, Annex J */
   DSL_uint16_t A5 : 1;
   /** ADSL Mode-Bit6: G.992.5, Annex I */
   DSL_uint16_t A6 : 1;
   /** ADSL Mode-Bit7: G.992.5, Annex J */
   DSL_uint16_t A7 : 1;
   /** ADSL Mode-Bit8: G.992.3, Annex A */
   DSL_uint16_t A8 : 1;
   /** ADSL Mode-Bit9: G.992.3, Annex B */
   DSL_uint16_t A9 : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 1;
   /** ADSL Mode-Bit11: G.992.3, Annex L, US Mask1 */
   DSL_uint16_t A11 : 1;
   /** ADSL Mode-Bit12: G.992.3, Annex L, US Mask2 */
   DSL_uint16_t A12 : 1;
   /** ADSL Mode-Bit13: G.992.3, Annex M */
   DSL_uint16_t A13 : 1;
   /** ADSL Mode-Bit14: G.992.5, Annex B */
   DSL_uint16_t A14 : 1;
   /** ADSL Mode-Bit15: G.992.5, Annex A */
   DSL_uint16_t A15 : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 2;
   /** VDSL Mode-Bit2: VDSL2 */
   DSL_uint16_t V2 : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 13;
#endif
} __PACKED__ ;


/**
   Reads the used downstream band configuration.The reported used DS band
   configuration reflects the MEDLEYds set as defined in the O-PRM message of
   the channel discovery phase according to G.993.2.
*/
struct CMD_BandControl_DS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Acknowledgement for message CMD_BandControl_DS_Get.
*/
struct ACK_BandControl_DS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint8_t Res0;
   /** Number of Downstream Bands */
   DSL_uint8_t NumBandsDS;
   /** Band Descriptor DS */
   VRX_ToneIndex_t band[8];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Number of Downstream Bands */
   DSL_uint8_t NumBandsDS;
   /** Reserved */
   DSL_uint8_t Res0;
   /** Band Descriptor DS */
   VRX_ToneIndex_t band[8];
#endif
} __PACKED__ ;


/**
   Reads the used upstream band configuration.The reported used US band
   configuration reflects the MEDLEYus set as defined in the R-PRM message of
   the channel discovery phase in G.993.2.
*/
struct CMD_BandControl_US_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Acknowledgement for message CMD_BandControl_US_Get.
*/
struct ACK_BandControl_US_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint8_t Res0;
   /** Number of Upstream Bands */
   DSL_uint8_t NumBandsUS;
   /** Band Descriptor US */
   VRX_ToneIndex_t band[8];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Number of Upstream Bands */
   DSL_uint8_t NumBandsUS;
   /** Reserved */
   DSL_uint8_t Res0;
   /** Band Descriptor US */
   VRX_ToneIndex_t band[8];
#endif
} __PACKED__ ;


/**
   Reads the RFI band configuration.
*/
struct CMD_RFI_BandControlGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Acknowledgement for message CMD_RFI_BandControlGet.
*/
struct ACK_RFI_BandControlGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint8_t Res0;
   /** Number of RFI Bands */
   DSL_uint8_t NumRfiBands;
   /** RFI Band Descriptor */
   VRX_ToneIndex_t RfiBand[16];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Number of RFI Bands */
   DSL_uint8_t NumRfiBands;
   /** Reserved */
   DSL_uint8_t Res0;
   /** RFI Band Descriptor */
   VRX_ToneIndex_t RfiBand[16];
#endif
} __PACKED__ ;


/**
   This message requests data about the line status (like noise margin) for
   downstream direction.
*/
struct CMD_LineStatusDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   The message delivers the line status data for downstream direction requested
   by CMD_LineStatusDS_Get.
*/
struct ACK_LineStatusDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0;
   /** Downstream LATN */
   DSL_uint16_t LATNds;
   /** Downstream SATN */
   DSL_uint16_t SATNds;
   /** Downstream SNRM */
   DSL_uint16_t SNRMds;
   /** Downstream ATTNDR, LSW */
   DSL_uint16_t ATTNDRds_LSW;
   /** Downstream ATTNDR, MSW */
   DSL_uint16_t ATTNDRds_MSW;
   /** Downstream ACTATP */
   DSL_int16_t ACTATPds;
   /** Downstream ACTPSD (ADSL only) */
   DSL_uint16_t ACTPSDds;
   /** Reserved */
   DSL_uint16_t Res1 : 7;
   /** Virtual Noise Status DS */
   DSL_uint16_t VNstatus_ds : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 6;
   /** NTR Status DS */
   DSL_uint16_t NTRstatus_ds : 1;
   /** TCM Status DS */
   DSL_uint16_t TCMstatus_ds : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0;
   /** Downstream LATN */
   DSL_uint16_t LATNds;
   /** Downstream SATN */
   DSL_uint16_t SATNds;
   /** Downstream SNRM */
   DSL_uint16_t SNRMds;
   /** Downstream ATTNDR, LSW */
   DSL_uint16_t ATTNDRds_LSW;
   /** Downstream ATTNDR, MSW */
   DSL_uint16_t ATTNDRds_MSW;
   /** Downstream ACTATP */
   DSL_int16_t ACTATPds;
   /** Downstream ACTPSD (ADSL only) */
   DSL_uint16_t ACTPSDds;
   /** TCM Status DS */
   DSL_uint16_t TCMstatus_ds : 1;
   /** NTR Status DS */
   DSL_uint16_t NTRstatus_ds : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 6;
   /** Virtual Noise Status DS */
   DSL_uint16_t VNstatus_ds : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 7;
#endif
} __PACKED__ ;


/**
   This message requests data about the upstream line status (like noise
   margin).
*/
struct CMD_LineStatusUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   The message delivers the line status data for upstream direction requested by
   CMD_LineStatusUS_Get.
*/
struct ACK_LineStatusUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0;
   /** Upstream LATN */
   DSL_uint16_t LATNus;
   /** Upstream SATN */
   DSL_uint16_t SATNus;
   /** Upstream SNRM */
   DSL_uint16_t SNRMus;
   /** Upstream ATTNDR, LSW */
   DSL_uint16_t ATTNDRus_LSW;
   /** Upstream ATTNDR, MSW */
   DSL_uint16_t ATTNDRus_MSW;
   /** Upstream ACTATP */
   DSL_int16_t ACTATPus;
   /** Upstream ACTPSD (ADSL only) */
   DSL_uint16_t ACTPSDus;
   /** Reserved */
   DSL_uint16_t Res1 : 7;
   /** Virtual Noise Status US */
   DSL_uint16_t VNstatus_us : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 7;
   /** TCM Status US */
   DSL_uint16_t TCMstatus_us : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0;
   /** Upstream LATN */
   DSL_uint16_t LATNus;
   /** Upstream SATN */
   DSL_uint16_t SATNus;
   /** Upstream SNRM */
   DSL_uint16_t SNRMus;
   /** Upstream ATTNDR, LSW */
   DSL_uint16_t ATTNDRus_LSW;
   /** Upstream ATTNDR, MSW */
   DSL_uint16_t ATTNDRus_MSW;
   /** Upstream ACTATP */
   DSL_int16_t ACTATPus;
   /** Upstream ACTPSD (ADSL only) */
   DSL_uint16_t ACTPSDus;
   /** TCM Status US */
   DSL_uint16_t TCMstatus_us : 1;
   /** Reserved */
   DSL_uint16_t Res2 : 7;
   /** Virtual Noise Status US */
   DSL_uint16_t VNstatus_us : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 7;
#endif
} __PACKED__ ;


/**
   This message requests data about the line status (noise margin, line
   attenuation and signal attenuation) for downstream direction in a "per band"
   granularity.
*/
struct CMD_LineStatusPerBandDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   The message delivers the line status data (line attenuation, signal
   attenuation and noise margin) for downstream direction in a "per band"
   granularity as requested by CMD_LineStatusPerBandDS_Get.
*/
struct ACK_LineStatusPerBandDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0;
   /** Line Status Descriptor DS  */
   VRX_LineStatus_pb_t LineStatus_pb_ds[8];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0;
   /** Line Status Descriptor DS  */
   VRX_LineStatus_pb_t LineStatus_pb_ds[8];
#endif
} __PACKED__ ;


/**
   This message requests data about the line status (noise margin, line - and
   signal attenuation) for upstream direction in a "per band" granularity.
*/
struct CMD_LineStatusPerBandUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   The message delivers the line status data (line attenuation, signal
   attenuation and noise margin) for upstream direction in a "per band"
   granularity as requested by CMD_LineStatusPerBandUS_Get.
*/
struct ACK_LineStatusPerBandUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0;
   /** Line Status Descriptor US  */
   VRX_LineStatus_pb_t LineStatus_pb_us[8];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0;
   /** Line Status Descriptor US  */
   VRX_LineStatus_pb_t LineStatus_pb_us[8];
#endif
} __PACKED__ ;


/**
   Requests the breakpoints of the actual PSD for the downstream direction. PSDs
   consisting of more than 42 PSD breakpoints must be read with 2 messages due
   to the the mailbox size. (The first parameter in the retrieved data structure
   contains the "Number of Breakpoints" parameter.)
*/
struct CMD_PSD_BreakpointsDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers the breakpoints of the actual PSD for the downstream direction as
   requested by CMD_PSD_BreakpointsDS_Get.
*/
struct ACK_PSD_BreakpointsDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Data */
   DSL_uint16_t Data[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Data */
   DSL_uint16_t Data[128];
#endif
} __PACKED__ ;


/**
   Requests the breakpoints of the actual PSD for the upstream direction.
*/
struct CMD_PSD_BreakpointsUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers the breakpoints of the actual PSD for the upstream direction as
   requested by CMD_PSD_BreakpointsUS_Get.
*/
struct ACK_PSD_BreakpointsUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint8_t Res0;
   /** Number of Breakpoints */
   DSL_uint8_t NumBreakPts;
   /** Breakpoint Descriptor US PSD */
   VRX_PSDbreak_t breakpoint[32];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Number of Breakpoints */
   DSL_uint8_t NumBreakPts;
   /** Reserved */
   DSL_uint8_t Res0;
   /** Breakpoint Descriptor US PSD */
   VRX_PSDbreak_t breakpoint[32];
#endif
} __PACKED__ ;


/**
   The Downstream TSSI Breakpoints are specified by the transmitter (VTU-O) and
   communicated to the receiver (VTU-R) via the O-PRM message.   The TSSI
   Breakpoints are used to shape the O-P-TEQ signal. This message provides read
   only access to the DS TSSI Breakpoints. The DS TSSI Breakpoints are not
   configurable via messages.
*/
struct CMD_TssiDS_VDSL2_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This is the acknowledgement of CMD_TssiDS_VDSL2_Get.
*/
struct ACK_TssiDS_VDSL2_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Data from the DS TSSI breakpoint buffer */
   DSL_uint16_t Data[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Data from the DS TSSI breakpoint buffer */
   DSL_uint16_t Data[128];
#endif
} __PACKED__ ;


/**
   The Upstream TSSI Breakpoints are specified by the transmitter (VTU-R) and
   communicated to the receiver (VTU-O) via the R-PRM message.   The TSSI
   Breakpoints are used to shape the R-P-TEQ signal.  The TSSI Breakpoints can
   be read only but are not configurable via messages.
*/
struct CMD_TssiUS_VDSL2_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This is the acknowledgement of CMD_TssiUS_VDSL2_Get.
*/
struct ACK_TssiUS_VDSL2_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Data from the US TSSI breakpoint buffer */
   DSL_uint16_t Data[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Data from the US TSSI breakpoint buffer */
   DSL_uint16_t Data[128];
#endif
} __PACKED__ ;


/**
   Reads the actual power cutback value.
*/
struct CMD_ADSL_PCB_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Provides the actual power cutback values requested with CMD_ADSL_PCB_Get.
*/
struct ACK_ADSL_PCB_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Downstream Power Cutback */
   DSL_uint16_t dspcb;
   /** Upstream Power Cutback */
   DSL_uint16_t uspcb;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Downstream Power Cutback */
   DSL_uint16_t dspcb;
   /** Upstream Power Cutback */
   DSL_uint16_t uspcb;
#endif
} __PACKED__ ;


/**
   Requests the PSD and power parameter values actually used by the firmware.
*/
struct CMD_ActualPSD_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Provieds actually used PSD and power parameters as requested by
   CMD_ActualPSD_Get.
*/
struct ACK_ActualPSD_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Power/PSD Configuration Parameter: MAXNOMPSDds */
   DSL_uint16_t MAXNOMPSDds;
   /** Power/PSD Configuration Parameter: MAXNOMPSDus */
   DSL_uint16_t MAXNOMPSDus;
   /** Power/PSD Configuration Parameter: MAXNOMATPds */
   DSL_uint16_t MAXNOMATPds;
   /** Power/PSD Configuration Parameters */
   DSL_uint16_t MAXNOMATPus;
   /** Reserved */
   DSL_uint16_t Res0;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Power/PSD Configuration Parameter: MAXNOMPSDds */
   DSL_uint16_t MAXNOMPSDds;
   /** Power/PSD Configuration Parameter: MAXNOMPSDus */
   DSL_uint16_t MAXNOMPSDus;
   /** Power/PSD Configuration Parameter: MAXNOMATPds */
   DSL_uint16_t MAXNOMATPds;
   /** Power/PSD Configuration Parameters */
   DSL_uint16_t MAXNOMATPus;
   /** Reserved */
   DSL_uint16_t Res0;
#endif
} __PACKED__ ;


/**
   Requests the actually used ADSL upstream mask for ADSL2/2+ Annexes M and J.
   To be applied only in combination with G.992.3/5 Annex M support in
   ACK_XTSE_StatusGet.
*/
struct CMD_ADSL_MJ_UsMaskGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Provides the actually used ADSL upstream mask for ADSL2/2+ Annexes M and J.
*/
struct ACK_ADSL_MJ_UsMaskGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 7;
   /** G.992.3/5 Annex M/J Upstream Mask 9 */
   DSL_uint16_t mask9 : 1;
   /** G.992.3/5 Annex M/J Upstream Mask 8 */
   DSL_uint16_t mask8 : 1;
   /** G.992.3/5 Annex M/J Upstream Mask 7 */
   DSL_uint16_t mask7 : 1;
   /** G.992.3/5 Annex M/J Upstream Mask 6 */
   DSL_uint16_t mask6 : 1;
   /** G.992.3/5 Annex M/J Upstream Mask 5 */
   DSL_uint16_t mask5 : 1;
   /** G.992.3/5 Annex M/J Upstream Mask 4 */
   DSL_uint16_t mask4 : 1;
   /** G.992.3/5 Annex M/J Upstream Mask 3 */
   DSL_uint16_t mask3 : 1;
   /** G.992.3/5 Annex M/J Upstream Mask 2 */
   DSL_uint16_t mask2 : 1;
   /** G.992.3/5 Annex M/J Upstream Mask 1 */
   DSL_uint16_t mask1 : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** G.992.3/5 Annex M/J Upstream Mask 1 */
   DSL_uint16_t mask1 : 1;
   /** G.992.3/5 Annex M/J Upstream Mask 2 */
   DSL_uint16_t mask2 : 1;
   /** G.992.3/5 Annex M/J Upstream Mask 3 */
   DSL_uint16_t mask3 : 1;
   /** G.992.3/5 Annex M/J Upstream Mask 4 */
   DSL_uint16_t mask4 : 1;
   /** G.992.3/5 Annex M/J Upstream Mask 5 */
   DSL_uint16_t mask5 : 1;
   /** G.992.3/5 Annex M/J Upstream Mask 6 */
   DSL_uint16_t mask6 : 1;
   /** G.992.3/5 Annex M/J Upstream Mask 7 */
   DSL_uint16_t mask7 : 1;
   /** G.992.3/5 Annex M/J Upstream Mask 8 */
   DSL_uint16_t mask8 : 1;
   /** G.992.3/5 Annex M/J Upstream Mask 9 */
   DSL_uint16_t mask9 : 1;
   /** Reserved */
   DSL_uint16_t Res0 : 7;
#endif
} __PACKED__ ;


/**
   Requests status information for the downstream bearer channels.
*/
struct CMD_BearerChsDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers status information for the downstream bearer channels. **: The terms
   "LP0/Interleaved Path" and "LP1/Fast Path" below refer to the terminology
   used in ITU DSL standards [1],[2], [3], [5] which means:- LP0 or LP1 in case
   of VDSL2, ADSL2/2+- "Interleaved Path" or "Fast Path" in case of
   ADSL1.Attention: In VRX V1.x the "LP1/Fast Path" parameters are populated
   only in ADSL1 mode with selected fast path.
*/
struct ACK_BearerChsDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** ACTNDR DS for LP0, LSW */
   DSL_uint16_t DRdsLP0_LSW;
   /** ACTNDR DS for LP0, MSW */
   DSL_uint16_t DRdsLP0_MSW;
   /** ACTNDR DS for LP1, LSW */
   DSL_uint16_t DRdsLP1_LSW;
   /** ACTNDR DS for LP1, MSW */
   DSL_uint16_t DRdsLP1_MSW;
   /** Reserved */
   DSL_uint16_t Res0[4];
   /** Actual Interleaving Delay DS, LP0 */
   DSL_uint16_t delay_dsLP0;
   /** Actual Interleaving Delay DS, LP1 */
   DSL_uint16_t delay_dsLP1;
   /** Actual Interleaver Depth DS, LP0 */
   DSL_uint16_t intlDepth_dsLP0;
   /** Actual Interleaver Depth DS, LP1 */
   DSL_uint16_t intlDepth_dsLP1;
   /** Reserved */
   DSL_uint16_t Res1 : 7;
   /** TCM Status DS, LP0  */
   DSL_uint16_t TCMstatus_dsLP0 : 1;
   /** Parity Bytes DS, LP0 */
   DSL_uint16_t parity_dsLP0 : 8;
   /** Reserved */
   DSL_uint8_t Res2;
   /** Parity Bytes DS, LP1 */
   DSL_uint16_t parity_dsLP1 : 8;
   /** Actual INP NoErasure DS , LP0 */
   DSL_uint16_t actInpNoErasure_LP0;
   /** Actual INP NoErasure DS, LP1 */
   DSL_uint16_t actInpNoErasure_LP1;
   /** Rx Estimated Actual INP DS, LP0 */
   DSL_uint16_t actInpErasure_LP0;
   /** Rx Estimated Actual INP DS, LP1 */
   DSL_uint16_t actInpErasure_LP1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** ACTNDR DS for LP0, LSW */
   DSL_uint16_t DRdsLP0_LSW;
   /** ACTNDR DS for LP0, MSW */
   DSL_uint16_t DRdsLP0_MSW;
   /** ACTNDR DS for LP1, LSW */
   DSL_uint16_t DRdsLP1_LSW;
   /** ACTNDR DS for LP1, MSW */
   DSL_uint16_t DRdsLP1_MSW;
   /** Reserved */
   DSL_uint16_t Res0[4];
   /** Actual Interleaving Delay DS, LP0 */
   DSL_uint16_t delay_dsLP0;
   /** Actual Interleaving Delay DS, LP1 */
   DSL_uint16_t delay_dsLP1;
   /** Actual Interleaver Depth DS, LP0 */
   DSL_uint16_t intlDepth_dsLP0;
   /** Actual Interleaver Depth DS, LP1 */
   DSL_uint16_t intlDepth_dsLP1;
   /** Parity Bytes DS, LP0 */
   DSL_uint16_t parity_dsLP0 : 8;
   /** TCM Status DS, LP0  */
   DSL_uint16_t TCMstatus_dsLP0 : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 7;
   /** Parity Bytes DS, LP1 */
   DSL_uint16_t parity_dsLP1 : 8;
   /** Reserved */
   DSL_uint8_t Res2;
   /** Actual INP NoErasure DS , LP0 */
   DSL_uint16_t actInpNoErasure_LP0;
   /** Actual INP NoErasure DS, LP1 */
   DSL_uint16_t actInpNoErasure_LP1;
   /** Rx Estimated Actual INP DS, LP0 */
   DSL_uint16_t actInpErasure_LP0;
   /** Rx Estimated Actual INP DS, LP1 */
   DSL_uint16_t actInpErasure_LP1;
#endif
} __PACKED__ ;


/**
   Requests status information for the upstream bearer channels.
*/
struct CMD_BearerChsUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers status information for the upstream bearer channels. **: The terms
   "LP0/Interleaved Path" and "LP1/Fast Path" below refer to the terminology
   used in ITU DSL standards [1],[2], [3], [5] which means:- LP0 or LP1 in case
   of VDSL2, ADSL2/2+- "Interleaved Path" or "Fast Path" in case of
   ADSL1.Attention: In VRX V1.x the "LP1/Fast Path" parameters are populated
   only in ADSL1 mode with selected fast path.
*/
struct ACK_BearerChsUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** ACTNDR US for LP0, LSW */
   DSL_uint16_t DRusLP0_LSW;
   /** ACTNDR US for LP0, MSW */
   DSL_uint16_t DRusLP0_MSW;
   /** ACTNDR US for LP1, LSW */
   DSL_uint16_t DRusLP1_LSW;
   /** ACTNDR US for LP1, MSW */
   DSL_uint16_t DRusLP1_MSW;
   /** Reserved */
   DSL_uint16_t Res0[4];
   /** Actual Interleaving Delay US, LP0 */
   DSL_uint16_t delay_usLP0;
   /** Actual Interleaving Delay US, LP1 */
   DSL_uint16_t delay_usLP1;
   /** Actual Interleaver Depth US, LP0 */
   DSL_uint16_t intlDepth_usLP0;
   /** Actual Interleaver Depth US, LP1 */
   DSL_uint16_t intlDepth_usLP1;
   /** Reserved */
   DSL_uint16_t Res1 : 7;
   /** TCM Status US, LP0  */
   DSL_uint16_t TCMstatus_usLP0 : 1;
   /** Parity Bytes US, LP0 */
   DSL_uint16_t parity_usLP0 : 8;
   /** Reserved */
   DSL_uint8_t Res2;
   /** Parity Bytes US, LP1 */
   DSL_uint16_t parity_usLP1 : 8;
   /** Actual INP NoErasure US , LP0 */
   DSL_uint16_t actInpNoErasure_LP0;
   /** Actual INP NoErasure US, LP1 */
   DSL_uint16_t actInpNoErasure_LP1;
   /** Rx Estimated Actual INP US , LP0 */
   DSL_uint16_t actInpErasure_LP0;
   /** Rx Estimated Actual INP US, LP1 */
   DSL_uint16_t actInpErasure_LP1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** ACTNDR US for LP0, LSW */
   DSL_uint16_t DRusLP0_LSW;
   /** ACTNDR US for LP0, MSW */
   DSL_uint16_t DRusLP0_MSW;
   /** ACTNDR US for LP1, LSW */
   DSL_uint16_t DRusLP1_LSW;
   /** ACTNDR US for LP1, MSW */
   DSL_uint16_t DRusLP1_MSW;
   /** Reserved */
   DSL_uint16_t Res0[4];
   /** Actual Interleaving Delay US, LP0 */
   DSL_uint16_t delay_usLP0;
   /** Actual Interleaving Delay US, LP1 */
   DSL_uint16_t delay_usLP1;
   /** Actual Interleaver Depth US, LP0 */
   DSL_uint16_t intlDepth_usLP0;
   /** Actual Interleaver Depth US, LP1 */
   DSL_uint16_t intlDepth_usLP1;
   /** Parity Bytes US, LP0 */
   DSL_uint16_t parity_usLP0 : 8;
   /** TCM Status US, LP0  */
   DSL_uint16_t TCMstatus_usLP0 : 1;
   /** Reserved */
   DSL_uint16_t Res1 : 7;
   /** Parity Bytes US, LP1 */
   DSL_uint16_t parity_usLP1 : 8;
   /** Reserved */
   DSL_uint8_t Res2;
   /** Actual INP NoErasure US , LP0 */
   DSL_uint16_t actInpNoErasure_LP0;
   /** Actual INP NoErasure US, LP1 */
   DSL_uint16_t actInpNoErasure_LP1;
   /** Rx Estimated Actual INP US , LP0 */
   DSL_uint16_t actInpErasure_LP0;
   /** Rx Estimated Actual INP US, LP1 */
   DSL_uint16_t actInpErasure_LP1;
#endif
} __PACKED__ ;


/**
   Requests an extended set of VDSL2 framing parameters for the downstream
   direction.
*/
struct CMD_FrameDataExt2DS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers an extended set of VDSL2 framing parameters for the downstream
   direction requested by CMD_FrameDataExt2DS_Get. (Sections 7.5.2.6 of G.997.1
   and 9.5 & 9.6 of G.993.2 [5])
*/
struct ACK_FrameDataExt2DS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Bytes "Bp" from BC0 per MDF LP0 DS */
   DSL_uint16_t B0_LP0ds;
   /** Reserved */
   DSL_uint16_t Res0;
   /** Reserved */
   DSL_uint16_t Res1;
   /** Reserved */
   DSL_uint16_t Res2;
   /** MDFs per OH Sub-Frame "Tp" LP0 DS */
   DSL_uint16_t Tp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res3;
   /** OH Bytes per OH sub-frame "Gp" LP0 DS */
   DSL_uint16_t Gp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res4;
   /** OH Frames per OH Superframe "Fp" LP0 DS */
   DSL_uint16_t Fp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res5;
   /** MDFs per RS-Codeword "Mp" LP0 DS */
   DSL_uint16_t Mp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res6;
   /** Bits per Symbol "Lp" LP0 DS */
   DSL_uint16_t Lp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res7;
   /** RS Check Bytes "Rp" LP0 DS */
   DSL_uint16_t Rp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res8;
   /** Interleaver Block Length "Ip", LP0 DS */
   DSL_uint16_t Ip_LP0ds;
   /** Reserved */
   DSL_uint16_t Res9;
   /** Interleaving Depth "Dp" LP0 DS */
   DSL_uint16_t Dp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res10;
   /** OH Frame Period "PERp", LP0 DS */
   DSL_uint16_t PERp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res11;
   /** Line Data Rate DS for LP0, LSW */
   DSL_uint16_t DRp_LP0ds_LSW;
   /** Line Data Rate DS for LP0, MSW */
   DSL_uint16_t DRp_LP0ds_MSW;
   /** Reserved */
   DSL_uint16_t Res12;
   /** Reserved */
   DSL_uint16_t Res13;
   /** RS Codeword Size "N_fecp" LP0 DS */
   DSL_uint16_t N_fecp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res14;
   /** Framing Parameter "q", LP0 DS */
   DSL_uint16_t q_LP0ds;
   /** Reserved */
   DSL_uint16_t Res15;
   /** OH Bytes per OH FrameLP0 "SEQp" DS */
   DSL_uint16_t SEQp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res16;
   /** OH sub-frames "Up" per OH frame LP0 DS */
   DSL_uint16_t Up_LP0ds;
   /** Reserved */
   DSL_uint16_t Res17;
   /** Bytes per OH Frame LP0 DS, LSW */
   DSL_uint16_t PERBp_LP0ds_LSW;
   /** Bytes per OH Frame LP0 DS, MSW */
   DSL_uint16_t PERBp_LP0ds_MSW;
   /** Reserved */
   DSL_uint16_t Res18;
   /** Reserved */
   DSL_uint16_t Res19;
   /** Bytes in MSG OH "MSGc" LP0 DS */
   DSL_uint16_t MSGc_LP0ds;
   /** Reserved */
   DSL_uint16_t Res20;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Bytes "Bp" from BC0 per MDF LP0 DS */
   DSL_uint16_t B0_LP0ds;
   /** Reserved */
   DSL_uint16_t Res0;
   /** Reserved */
   DSL_uint16_t Res1;
   /** Reserved */
   DSL_uint16_t Res2;
   /** MDFs per OH Sub-Frame "Tp" LP0 DS */
   DSL_uint16_t Tp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res3;
   /** OH Bytes per OH sub-frame "Gp" LP0 DS */
   DSL_uint16_t Gp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res4;
   /** OH Frames per OH Superframe "Fp" LP0 DS */
   DSL_uint16_t Fp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res5;
   /** MDFs per RS-Codeword "Mp" LP0 DS */
   DSL_uint16_t Mp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res6;
   /** Bits per Symbol "Lp" LP0 DS */
   DSL_uint16_t Lp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res7;
   /** RS Check Bytes "Rp" LP0 DS */
   DSL_uint16_t Rp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res8;
   /** Interleaver Block Length "Ip", LP0 DS */
   DSL_uint16_t Ip_LP0ds;
   /** Reserved */
   DSL_uint16_t Res9;
   /** Interleaving Depth "Dp" LP0 DS */
   DSL_uint16_t Dp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res10;
   /** OH Frame Period "PERp", LP0 DS */
   DSL_uint16_t PERp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res11;
   /** Line Data Rate DS for LP0, LSW */
   DSL_uint16_t DRp_LP0ds_LSW;
   /** Line Data Rate DS for LP0, MSW */
   DSL_uint16_t DRp_LP0ds_MSW;
   /** Reserved */
   DSL_uint16_t Res12;
   /** Reserved */
   DSL_uint16_t Res13;
   /** RS Codeword Size "N_fecp" LP0 DS */
   DSL_uint16_t N_fecp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res14;
   /** Framing Parameter "q", LP0 DS */
   DSL_uint16_t q_LP0ds;
   /** Reserved */
   DSL_uint16_t Res15;
   /** OH Bytes per OH FrameLP0 "SEQp" DS */
   DSL_uint16_t SEQp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res16;
   /** OH sub-frames "Up" per OH frame LP0 DS */
   DSL_uint16_t Up_LP0ds;
   /** Reserved */
   DSL_uint16_t Res17;
   /** Bytes per OH Frame LP0 DS, LSW */
   DSL_uint16_t PERBp_LP0ds_LSW;
   /** Bytes per OH Frame LP0 DS, MSW */
   DSL_uint16_t PERBp_LP0ds_MSW;
   /** Reserved */
   DSL_uint16_t Res18;
   /** Reserved */
   DSL_uint16_t Res19;
   /** Bytes in MSG OH "MSGc" LP0 DS */
   DSL_uint16_t MSGc_LP0ds;
   /** Reserved */
   DSL_uint16_t Res20;
#endif
} __PACKED__ ;


/**
   Requests an extended set of VDSL2 framing parameters for the upstream
   direction.
*/
struct CMD_FrameDataExt2US_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers an extended set of VDSL2 framing parameters for the upstream
   direction requested by CMD_FrameDataExt2US_Get. (Sections 7.5.2.6 of G.997.1
   and 9.5 & 9.6 of G.993.2 [5])
*/
struct ACK_FrameDataExt2US_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Bytes "Bp" from BC0 per MDF LP0 US */
   DSL_uint16_t B0_LP0us;
   /** Reserved */
   DSL_uint16_t Res0;
   /** Reserved */
   DSL_uint16_t Res1;
   /** Reserved */
   DSL_uint16_t Res2;
   /** MDFs per OH Sub-Frame "Tp" LP0 US */
   DSL_uint16_t Tp_LP0us;
   /** Reserved */
   DSL_uint16_t Res3;
   /** OH Bytes per OH sub-frame "Gp" LP0 US */
   DSL_uint16_t Gp_LP0us;
   /** Reserved */
   DSL_uint16_t Res4;
   /** OH Frames per OH Superframe "Fp" LP0 US */
   DSL_uint16_t Fp_LP0us;
   /** Reserved */
   DSL_uint16_t Res5;
   /** MDFs per RS-Codeword "Mp" LP0 US */
   DSL_uint16_t Mp_LP0us;
   /** Reserved */
   DSL_uint16_t Res6;
   /** Bits per Symbol "Lp" LP0 US */
   DSL_uint16_t Lp_LP0us;
   /** Reserved */
   DSL_uint16_t Res7;
   /** RS Check Bytes "Rp" LP0 US */
   DSL_uint16_t Rp_LP0us;
   /** Reserved */
   DSL_uint16_t Res8;
   /** Interleaver Block Length "Ip", LP0 US */
   DSL_uint16_t Ip_LP0us;
   /** Reserved */
   DSL_uint16_t Res9;
   /** Interleaving Depth "Dp" LP0 US */
   DSL_uint16_t Dp_LP0us;
   /** Reserved */
   DSL_uint16_t Res10;
   /** OH Frame Period "PERp", LP0 US */
   DSL_uint16_t PERp_LP0us;
   /** Reserved */
   DSL_uint16_t Res11;
   /** Line Data Rate US for LP0, LSW */
   DSL_uint16_t DRp_LP0us_LSW;
   /** Line Data Rate US for LP0, MSW */
   DSL_uint16_t DRp_LP0us_MSW;
   /** Reserved */
   DSL_uint16_t Res12;
   /** Reserved */
   DSL_uint16_t Res13;
   /** RS Codeword Size "N_fecp" LP0 US */
   DSL_uint16_t N_fecp_LP0us;
   /** Reserved */
   DSL_uint16_t Res14;
   /** Framing Parameter "q", LP0 US */
   DSL_uint16_t q_LP0us;
   /** Reserved */
   DSL_uint16_t Res15;
   /** OH Bytes per OH FrameLP0 "SEQp" US */
   DSL_uint16_t SEQp_LP0us;
   /** Reserved */
   DSL_uint16_t Res16;
   /** OH sub-frames "Up" per OH frame LP0 US */
   DSL_uint16_t Up_LP0us;
   /** Reserved */
   DSL_uint16_t Res17;
   /** Bytes per OH Frame LP0 US, LSW */
   DSL_uint16_t PERBp_LP0us_LSW;
   /** Bytes per OH Frame LP0 US, MSW */
   DSL_uint16_t PERBp_LP0us_MSW;
   /** Reserved */
   DSL_uint16_t Res18;
   /** Reserved */
   DSL_uint16_t Res19;
   /** Bytes in MSG OH "MSGc" LP0 US */
   DSL_uint16_t MSGc_LP0us;
   /** Reserved */
   DSL_uint16_t Res20;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Bytes "Bp" from BC0 per MDF LP0 US */
   DSL_uint16_t B0_LP0us;
   /** Reserved */
   DSL_uint16_t Res0;
   /** Reserved */
   DSL_uint16_t Res1;
   /** Reserved */
   DSL_uint16_t Res2;
   /** MDFs per OH Sub-Frame "Tp" LP0 US */
   DSL_uint16_t Tp_LP0us;
   /** Reserved */
   DSL_uint16_t Res3;
   /** OH Bytes per OH sub-frame "Gp" LP0 US */
   DSL_uint16_t Gp_LP0us;
   /** Reserved */
   DSL_uint16_t Res4;
   /** OH Frames per OH Superframe "Fp" LP0 US */
   DSL_uint16_t Fp_LP0us;
   /** Reserved */
   DSL_uint16_t Res5;
   /** MDFs per RS-Codeword "Mp" LP0 US */
   DSL_uint16_t Mp_LP0us;
   /** Reserved */
   DSL_uint16_t Res6;
   /** Bits per Symbol "Lp" LP0 US */
   DSL_uint16_t Lp_LP0us;
   /** Reserved */
   DSL_uint16_t Res7;
   /** RS Check Bytes "Rp" LP0 US */
   DSL_uint16_t Rp_LP0us;
   /** Reserved */
   DSL_uint16_t Res8;
   /** Interleaver Block Length "Ip", LP0 US */
   DSL_uint16_t Ip_LP0us;
   /** Reserved */
   DSL_uint16_t Res9;
   /** Interleaving Depth "Dp" LP0 US */
   DSL_uint16_t Dp_LP0us;
   /** Reserved */
   DSL_uint16_t Res10;
   /** OH Frame Period "PERp", LP0 US */
   DSL_uint16_t PERp_LP0us;
   /** Reserved */
   DSL_uint16_t Res11;
   /** Line Data Rate US for LP0, LSW */
   DSL_uint16_t DRp_LP0us_LSW;
   /** Line Data Rate US for LP0, MSW */
   DSL_uint16_t DRp_LP0us_MSW;
   /** Reserved */
   DSL_uint16_t Res12;
   /** Reserved */
   DSL_uint16_t Res13;
   /** RS Codeword Size "N_fecp" LP0 US */
   DSL_uint16_t N_fecp_LP0us;
   /** Reserved */
   DSL_uint16_t Res14;
   /** Framing Parameter "q", LP0 US */
   DSL_uint16_t q_LP0us;
   /** Reserved */
   DSL_uint16_t Res15;
   /** OH Bytes per OH FrameLP0 "SEQp" US */
   DSL_uint16_t SEQp_LP0us;
   /** Reserved */
   DSL_uint16_t Res16;
   /** OH sub-frames "Up" per OH frame LP0 US */
   DSL_uint16_t Up_LP0us;
   /** Reserved */
   DSL_uint16_t Res17;
   /** Bytes per OH Frame LP0 US, LSW */
   DSL_uint16_t PERBp_LP0us_LSW;
   /** Bytes per OH Frame LP0 US, MSW */
   DSL_uint16_t PERBp_LP0us_MSW;
   /** Reserved */
   DSL_uint16_t Res18;
   /** Reserved */
   DSL_uint16_t Res19;
   /** Bytes in MSG OH "MSGc" LP0 US */
   DSL_uint16_t MSGc_LP0us;
   /** Reserved */
   DSL_uint16_t Res20;
#endif
} __PACKED__ ;


/**
   Requests ADSL framing parameters for the downstream direction, latency path
   0/Interleaved Path.
*/
struct CMD_ADSL_FrameDataDS_LP0Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers the ADSL framing parameters for the downstream direction,
   LP0/Interleaved path, as requested by CMD_ADSL_FrameDataDS_LP0Get. (Sections
   7.5 & 7.6 of G.992.3 [2] and 7.4 of G.992.1 [1])
*/
struct ACK_ADSL_FrameDataDS_LP0Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** RS Check Bytes "Rp" DS LP0 */
   DSL_uint16_t Rp;
   /** Symbols per RS-Codeword "Sp" DS LP0 */
   DSL_uint16_t Sp;
   /** Interleaving Depth "Dp" DS LP0 */
   DSL_uint16_t Dp;
   /** Bytes per MDF "Kp" DS LP0 */
   DSL_uint16_t Kp;
   /** MDFs per RS-Codeword "Mp" DS LP0 */
   DSL_uint16_t Mp;
   /** Bits per Symbol "Lp" DS LP0 */
   DSL_uint16_t Lp;
   /** TCM Bits DS LP0 */
   DSL_uint16_t TCM;
   /** Framing Mode DS LP0 */
   DSL_uint16_t fraMode;
   /** "Tp" DS LP0 */
   DSL_uint16_t Tp;
   /** "SEQp" DS LP0 */
   DSL_uint16_t SEQp;
   /** Bytes in MSG-OH "MSGc" DS */
   DSL_uint16_t MSGc;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** RS Check Bytes "Rp" DS LP0 */
   DSL_uint16_t Rp;
   /** Symbols per RS-Codeword "Sp" DS LP0 */
   DSL_uint16_t Sp;
   /** Interleaving Depth "Dp" DS LP0 */
   DSL_uint16_t Dp;
   /** Bytes per MDF "Kp" DS LP0 */
   DSL_uint16_t Kp;
   /** MDFs per RS-Codeword "Mp" DS LP0 */
   DSL_uint16_t Mp;
   /** Bits per Symbol "Lp" DS LP0 */
   DSL_uint16_t Lp;
   /** TCM Bits DS LP0 */
   DSL_uint16_t TCM;
   /** Framing Mode DS LP0 */
   DSL_uint16_t fraMode;
   /** "Tp" DS LP0 */
   DSL_uint16_t Tp;
   /** "SEQp" DS LP0 */
   DSL_uint16_t SEQp;
   /** Bytes in MSG-OH "MSGc" DS */
   DSL_uint16_t MSGc;
#endif
} __PACKED__ ;


/**
   Requests ADSL framing parameters for the upstream direction, latency path
   0/Interleaved path.
*/
struct CMD_ADSL_FrameDataUS_LP0Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers the ADSL framing parameters for the upstream direction, latency path
   0/Interleaved path, as requested by CMD_ADSL_FrameDataUS_LP0Get. (Sections
   7.5 & 7.6 of G.992.3 [2] and 7.4 of G.992.1 [1])
*/
struct ACK_ADSL_FrameDataUS_LP0Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** RS Check Bytes "Rp" US LP0 */
   DSL_uint16_t Rp_LP0us;
   /** Symbols per RS-Codeword "Sp" US LP0 */
   DSL_uint16_t Sp;
   /** Interleaving Depth "Dp" US LP0 */
   DSL_uint16_t Dp;
   /** Bytes per MDF "Kp" US LP0 */
   DSL_uint16_t Kp;
   /** MDFs per RS-Codeword "Mp" US LP0 */
   DSL_uint16_t Mp;
   /** Bits per Symbol "Lp" US LP0 */
   DSL_uint16_t Lp;
   /** TCM Bits US LP0 */
   DSL_uint16_t TCM;
   /** Framing Mode US LP0 */
   DSL_uint16_t fraMode;
   /** "Tp" US LP0 */
   DSL_uint16_t Tp;
   /** "SEQp" US LP0 */
   DSL_uint16_t SEQp;
   /** Bytes in MSG-OH "MSGc" US */
   DSL_uint16_t MSGc;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** RS Check Bytes "Rp" US LP0 */
   DSL_uint16_t Rp_LP0us;
   /** Symbols per RS-Codeword "Sp" US LP0 */
   DSL_uint16_t Sp;
   /** Interleaving Depth "Dp" US LP0 */
   DSL_uint16_t Dp;
   /** Bytes per MDF "Kp" US LP0 */
   DSL_uint16_t Kp;
   /** MDFs per RS-Codeword "Mp" US LP0 */
   DSL_uint16_t Mp;
   /** Bits per Symbol "Lp" US LP0 */
   DSL_uint16_t Lp;
   /** TCM Bits US LP0 */
   DSL_uint16_t TCM;
   /** Framing Mode US LP0 */
   DSL_uint16_t fraMode;
   /** "Tp" US LP0 */
   DSL_uint16_t Tp;
   /** "SEQp" US LP0 */
   DSL_uint16_t SEQp;
   /** Bytes in MSG-OH "MSGc" US */
   DSL_uint16_t MSGc;
#endif
} __PACKED__ ;


/**
   Requests ADSL framing parameters for the downstream direction, latency path 1
   (LP1)/ Fast path.In VRX Rev. 1.x to be used for ADSL1, "fast path" only.
*/
struct CMD_ADSL_FrameDataDS_LP1Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers the ADSL framing parameters for the downstream direction, LP1/Fast
   path, as requested by CMD_ADSL_FrameDataDS_LP1Get. (Sections 7.5 & 7.6 of
   G.992.3 [2] and 7.4 of G.992.1 [1])
*/
struct ACK_ADSL_FrameDataDS_LP1Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** RS Check Bytes "Rp" DS LP1 */
   DSL_uint16_t Rp;
   /** Symbols per RS-Codeword "Sp" DS LP1 */
   DSL_uint16_t Sp;
   /** Interleaving Depth "Dp" DS LP1 */
   DSL_uint16_t Dp;
   /** Bytes per MDF "Kp" DS LP1 */
   DSL_uint16_t Kp;
   /** MDFs per RS-Codeword "Mp" DS LP1 */
   DSL_uint16_t Mp;
   /** Bits per Symbol "Lp" DS LP1 */
   DSL_uint16_t Lp;
   /** TCM Bits DS LP1 */
   DSL_uint16_t TCM;
   /** Framing Mode DS LP1 */
   DSL_uint16_t fraMode;
   /** "Tp" DS LP1 */
   DSL_uint16_t Tp;
   /** "SEQp" DS LP1 */
   DSL_uint16_t SEQp;
   /** Bytes in MSG-OH "MSGc" DS */
   DSL_uint16_t MSGc;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** RS Check Bytes "Rp" DS LP1 */
   DSL_uint16_t Rp;
   /** Symbols per RS-Codeword "Sp" DS LP1 */
   DSL_uint16_t Sp;
   /** Interleaving Depth "Dp" DS LP1 */
   DSL_uint16_t Dp;
   /** Bytes per MDF "Kp" DS LP1 */
   DSL_uint16_t Kp;
   /** MDFs per RS-Codeword "Mp" DS LP1 */
   DSL_uint16_t Mp;
   /** Bits per Symbol "Lp" DS LP1 */
   DSL_uint16_t Lp;
   /** TCM Bits DS LP1 */
   DSL_uint16_t TCM;
   /** Framing Mode DS LP1 */
   DSL_uint16_t fraMode;
   /** "Tp" DS LP1 */
   DSL_uint16_t Tp;
   /** "SEQp" DS LP1 */
   DSL_uint16_t SEQp;
   /** Bytes in MSG-OH "MSGc" DS */
   DSL_uint16_t MSGc;
#endif
} __PACKED__ ;


/**
   Requests ADSL framing parameters for the upstream direction, latency path 1
   (LP1)/Fast path.In VRX Rev. 1.x to be used for ADSL1, "fast path" only.
*/
struct CMD_ADSL_FrameDataUS_LP1Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers the ADSL framing parameters for the upstream direction, LP1/Fast
   path, as requested by CMD_ADSL_FrameDataUS_LP1Get. (Sections 7.5 & 7.6 of
   G.992.3 [2] and 7.4 of G.992.1 [1])
*/
struct ACK_ADSL_FrameDataUS_LP1Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** RS Check Bytes "Rp" US LP1 */
   DSL_uint16_t Rp_LP0us;
   /** Symbols per RS-Codeword "Sp" US LP1 */
   DSL_uint16_t Sp;
   /** Interleaving Depth "Dp" US LP1 */
   DSL_uint16_t Dp;
   /** Bytes per MDF "Kp" US LP1 */
   DSL_uint16_t Kp;
   /** MDFs per RS-Codeword "Mp" US LP1 */
   DSL_uint16_t Mp;
   /** Bits per Symbol "Lp" US LP1 */
   DSL_uint16_t Lp;
   /** TCM Bits US LP1 */
   DSL_uint16_t TCM;
   /** Framing Mode US LP1 */
   DSL_uint16_t fraMode;
   /** "Tp" US LP1 */
   DSL_uint16_t Tp;
   /** "SEQp" US LP1 */
   DSL_uint16_t SEQp;
   /** Bytes in MSG-OH "MSGc" US */
   DSL_uint16_t MSGc;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** RS Check Bytes "Rp" US LP1 */
   DSL_uint16_t Rp_LP0us;
   /** Symbols per RS-Codeword "Sp" US LP1 */
   DSL_uint16_t Sp;
   /** Interleaving Depth "Dp" US LP1 */
   DSL_uint16_t Dp;
   /** Bytes per MDF "Kp" US LP1 */
   DSL_uint16_t Kp;
   /** MDFs per RS-Codeword "Mp" US LP1 */
   DSL_uint16_t Mp;
   /** Bits per Symbol "Lp" US LP1 */
   DSL_uint16_t Lp;
   /** TCM Bits US LP1 */
   DSL_uint16_t TCM;
   /** Framing Mode US LP1 */
   DSL_uint16_t fraMode;
   /** "Tp" US LP1 */
   DSL_uint16_t Tp;
   /** "SEQp" US LP1 */
   DSL_uint16_t SEQp;
   /** Bytes in MSG-OH "MSGc" US */
   DSL_uint16_t MSGc;
#endif
} __PACKED__ ;


/**
   Reads back the parameter values configured with CMD_AlgorithmControlSet.
*/
struct CMD_AlgorithmControlGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Provides the information requested by CMD_AlgorithmControlGet.
*/
struct ACK_AlgorithmControlGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Algorithm Control Parameter Status */
   DSL_uint16_t Readback;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Algorithm Control Parameter Status */
   DSL_uint16_t Readback;
#endif
} __PACKED__ ;


/**
   Reads the connected hybrid type.
*/
struct CMD_HybridGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Contains the connected hybrid type.
*/
struct ACK_HybridGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Hybrid Type */
   DSL_uint16_t HybridType;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Hybrid Type */
   DSL_uint16_t HybridType;
#endif
} __PACKED__ ;


/**
   Requests the pilot tones used in Showtime.
*/
struct CMD_PilotTonesGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Provides the Showtime pilot tones as requested by CMD_PilotTonesGet.
*/
struct ACK_PilotTonesGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Number of Pilot Tones */
   DSL_uint16_t numPilot;
   /** Pilot Tones */
   DSL_uint16_t pilot[16];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Number of Pilot Tones */
   DSL_uint16_t numPilot;
   /** Pilot Tones */
   DSL_uint16_t pilot[16];
#endif
} __PACKED__ ;


/**
   Requests the results of the micro filter detection (MFD) function. Valid
   information in Showtime and FAIL will be available only if the microfilter
   detection mechanism has been run before.
*/
struct CMD_MFD_ResultsGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Reports the results of the micro filter detection (MFD) function.
*/
struct ACK_MFD_ResultsGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res0 : 1;
   /** Filter Detection Process Indication, Bit 14:13 */
   DSL_uint16_t FilterDetectStatus : 2;
   /** Reserved */
   DSL_uint16_t Res1 : 3;
   /** Training Bridged Tap Range, Bit 9:8 */
   DSL_uint16_t TrainingBTRange : 2;
   /** Training Hybrid Info Ready Indication, Bit 7 */
   DSL_uint16_t TrainingHybridInfoReady : 1;
   /** Training Loop Length Ready Indication, Bit 6 */
   DSL_uint16_t TrainingLoopLengthReady : 1;
   /** Training Metric2 Ready Indication, Bit 5 */
   DSL_uint16_t TrainingMetric2Ready : 1;
   /** Reserved Bit 4:2 */
   DSL_uint16_t Res2 : 3;
   /** Bridge Tap Range, Bit 1:0 */
   DSL_uint16_t BTRange : 2;
   /** Metric1 for NLNF, Bit 15:4 */
   DSL_uint16_t M1metric : 12;
   /** "No Confidence" Bit, Bit 3 */
   DSL_uint16_t M1NoConfidence : 1;
   /** Metric1 QLN level, Bit 2:0 */
   DSL_uint16_t M1QlnLevel : 3;
   /** Metric2 */
   DSL_int16_t M2Metric;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Bridge Tap Range, Bit 1:0 */
   DSL_uint16_t BTRange : 2;
   /** Reserved Bit 4:2 */
   DSL_uint16_t Res2 : 3;
   /** Training Metric2 Ready Indication, Bit 5 */
   DSL_uint16_t TrainingMetric2Ready : 1;
   /** Training Loop Length Ready Indication, Bit 6 */
   DSL_uint16_t TrainingLoopLengthReady : 1;
   /** Training Hybrid Info Ready Indication, Bit 7 */
   DSL_uint16_t TrainingHybridInfoReady : 1;
   /** Training Bridged Tap Range, Bit 9:8 */
   DSL_uint16_t TrainingBTRange : 2;
   /** Reserved */
   DSL_uint16_t Res1 : 3;
   /** Filter Detection Process Indication, Bit 14:13 */
   DSL_uint16_t FilterDetectStatus : 2;
   /** Reserved */
   DSL_uint16_t Res0 : 1;
   /** Metric1 QLN level, Bit 2:0 */
   DSL_uint16_t M1QlnLevel : 3;
   /** "No Confidence" Bit, Bit 3 */
   DSL_uint16_t M1NoConfidence : 1;
   /** Metric1 for NLNF, Bit 15:4 */
   DSL_uint16_t M1metric : 12;
   /** Metric2 */
   DSL_int16_t M2Metric;
#endif
} __PACKED__ ;


/**
   Requests the loop length estimates done by the microfilter detection
   function.In Showtime the data are always available.The availability of data
   is indicated by the flag TrainingLoopLengthReady in ACK_MFD_ResultsGet.In
   Showtime the data are always available.
*/
struct CMD_MFD_LooplengthGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Reports the loop length estimates done by the microfilter detection (MFD)
   function.
*/
struct ACK_MFD_LooplengthGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** AWG26 Length Estimate */
   DSL_uint16_t Awg26Length;
   /** AWG24 Length Estimate */
   DSL_uint16_t Awg24Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** AWG26 Length Estimate */
   DSL_uint16_t Awg26Length;
   /** AWG24 Length Estimate */
   DSL_uint16_t Awg24Length;
#endif
} __PACKED__ ;


/**
   Requests hybrid information used by the microfilter detection function.In
   Showtime the data are always available.The availability of data is indicated
   by the flag TrainingHybridInfoReady in ACK_MFD_ResultsGet. In Showtime the
   data are always available.
*/
struct CMD_MFD_HybridInfoGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Reports the hybrid information used by the microfilter detection function.
*/
struct ACK_MFD_HybridInfoGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Hybrid Max Index */
   DSL_uint16_t HybridMaxIndex;
   /** Hybrid Max Metric */
   DSL_uint16_t HybridMaxMetric;
   /** Hybrid Second Best Index */
   DSL_uint16_t Hybrid2ndBestIndex;
   /** Hybrid Second Best Metric */
   DSL_uint16_t Hybrid2ndBestMetric;
   /** Hybrid Zero Metric */
   DSL_uint16_t Hybrid0Metric;
   /** Loaded Hybrid Index */
   DSL_uint16_t LoadHybridIndex;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Hybrid Max Index */
   DSL_uint16_t HybridMaxIndex;
   /** Hybrid Max Metric */
   DSL_uint16_t HybridMaxMetric;
   /** Hybrid Second Best Index */
   DSL_uint16_t Hybrid2ndBestIndex;
   /** Hybrid Second Best Metric */
   DSL_uint16_t Hybrid2ndBestMetric;
   /** Hybrid Zero Metric */
   DSL_uint16_t Hybrid0Metric;
   /** Loaded Hybrid Index */
   DSL_uint16_t LoadHybridIndex;
#endif
} __PACKED__ ;




#ifdef __cplusplus
}
#endif

#ifdef __PACKED_DEFINED__
   #if !(defined (__GNUC__) || defined (__GNUG__))
      #pragma pack()
   #endif
   #undef __PACKED_DEFINED__
#endif /* __PACKED_DEFINED__ */

/** @} */

#endif /** _DRV_DSL_CPE_VRX_MSG_GHS_STATUS_H_*/
