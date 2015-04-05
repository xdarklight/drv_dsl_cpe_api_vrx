/******************************************************************************

                               Copyright (c) 2011
                            Lantiq Deutschland GmbH
                     Am Campeon 3; 85579 Neubiberg, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef DRV_DSL_CPE_VRX_CTX_H
#define DRV_DSL_CPE_VRX_CTX_H

#ifdef __cplusplus
   extern "C" {
#endif

/** \file
   VRx internal interface
*/

/** \addtogroup DRV_DSL_DEVICE
 @{ */

/**
   Selects a mode on how to handle handshake tone groups.
*/
typedef enum
{
   /**
      Not applicable/not initialized
      In case of writing the configuration the actual setting will NOT be
      changed, in case of reading the configuration this setting shall not
      occur and indicates an internal error!
      */
   DSL_DEV_HS_TONE_GROUP_MODE_NA = -1,
   /**
      Enables the automatic mode for sending handshake tones (default).
      This is the default configuration. It will activate all required handshake
      tones with respect to
      - the activated xDSL modes (\ref  DSL_G997_XTUSystemEnablingData_t)
      - the used firmware/hybrid constellation

      \attention Do not change to manual mode unless there are cogent reasons
                 for it! */
   DSL_DEV_HS_TONE_GROUP_MODE_AUTO = 0,
   /**
      Enables the manual mode for sending handshake tones.
      Activating this mode provides the possibility to define user specific
      handshake tones. If activating this mode it is required to also configure
      the following parameters
      - \ref nHsToneGroup_A
      - \ref nHsToneGroup_V
      - \ref nHsToneGroup_AV */
   DSL_DEV_HS_TONE_GROUP_MODE_MANUAL = 1,
   /**
      Delimiter only! */
   DSL_DEV_HS_TONE_GROUP_MODE_LAST = 2
} DSL_DEV_HsToneGroupMode_t;

/**
   Definition for handshake tone group sets.
   This configuration should be used to handle VDSL over ISDN and similar modes.
   The selections influences the handshake tone set according to
   G.994.1 Amendment 2, Table 1/G.994.1.
   */
typedef enum
{
   /**
   Not applicable/not initialized.
   In case of writing the configuration the actual setting will NOT be
   changed, in case of reading the configuration this setting shall not
   occur and indicates an internal error! */
   DSL_DEV_HS_TONE_GROUP_NA = -1,
   /**
   Cleaned */
   DSL_DEV_HS_TONE_GROUP_CLEANED    = 0x0000,
   /**
   Activation of carrier set B43 for VDSL2 with US0 (default). */
   DSL_DEV_HS_TONE_GROUP_VDSL2_B43  = 0x0001,
   /**
   Activation of carrier set A43 for VDSL2 with US0 (No ISDN). */
   DSL_DEV_HS_TONE_GROUP_VDSL2_A43  = 0x0002,
   /**
   Activation of carrier set V43 for VDSL2 without US0. */
   DSL_DEV_HS_TONE_GROUP_VDSL2_V43  = 0x0004,
   /**
   Activation of carrier set V43P for VDSL1 over POTS.
   \attention This configuration is deprecated and will be ignored. */
   DSL_DEV_HS_TONE_GROUP_VDSL1_V43P = 0x0008,
   /**
   Activation of carrier set V43P for VDSL1 over ISDN.
   \attention This configuration is deprecated and will be ignored. */
   DSL_DEV_HS_TONE_GROUP_VDSL1_V43I = 0x0010,
   /**
   Activation of carrier set C43 for Annex C/J. */
   DSL_DEV_HS_TONE_GROUP_ADSL1_C43  = 0x0020,
   /**
   Activation of carrier set J43 for ADSL2/2+ Annex J/M with US0 (No ISDN) */
   DSL_DEV_HS_TONE_GROUP_ADSL2_J43  = 0x0040,
   /**
   Activation of carrier set B43C for ADSL2+ with US0 */
   DSL_DEV_HS_TONE_GROUP_ADSL2_B43C = 0x0080,
   /**
   Activation of carrier set A43C for ADSL2+ with US0 (No ISDN) */
   DSL_DEV_HS_TONE_GROUP_ADSL2_A43C = 0x0100,
   /**
   Delimiter only! */
   DSL_DEV_HS_TONE_GROUP_LAST = 0x0200
} DSL_DEV_HsToneGroup_t;

/**
   Usage of additional filters for ISDN and POTS.
*/
typedef enum
{
   /**
      Not applicable/not initialized
      In case of writing the configuration the actual setting will NOT be
      changed, in case of reading the configuration this setting shall not
      occur and indicated an internal error!
      */
   DSL_DEV_FILTER_NA = -1,
   /**
      No additional filter is used. */
   DSL_DEV_FILTER_OFF = 0,
   /**
      Additional filter for xDSL over ISDN is used. */
   DSL_DEV_FILTER_ISDN = 1,
   /**
      Additional filter for xDSL over POTS is used
      (VDSL2 with PSD limitations). */
   DSL_DEV_FILTER_POTS = 2,
   /**
      Additional filter for VDSL over POTS is used. */
   DSL_DEV_FILTER_POTS_2 = 3,
   /** VDSL over POTS (Suggested mode for VDSL over
       POTS for double US0 and quad US0)*/
   DSL_DEV_FILTER_POTS_3 = 4,
   /**
      Delimiter only! */
   DSL_DEV_FILTER_LAST
} DSL_DEV_Filter_t;

/**
   Device specific low level configuration.

   \note Currently (E07/B08 2010) there is no DSL PHY firmware available that
         supports to configure the handshake tones within ADSL mode.
         However the API tries to configure it already and interprets the
         resulting error as an internal "not supported" warning. With this
         handling the API could directly support full hanshake configurations
         as defined without further changes.
*/
typedef struct
{
   /**
   Usage of additional filters for ISDN and POTS*/
   DSL_DEV_Filter_t nFilter;
   /**
   Selection for automatic or manual handshake tone set selection. */
   DSL_DEV_HsToneGroupMode_t nHsToneGroupMode;
   /**
   Selects one or more groups of tones to be used during the handshake phase
   in case of activating the link using ADSL xDSL modes only.
   \note This parameter is only used in case of activating manual mode for
         handshake tone group selection (\ref DSL_DEV_HS_TONE_GROUP_MODE_MANUAL)
   \attention In manual mode there is no logic within API internal handling!
         This means that the ADSL related handshake tones has to be selected
         in accordance to the used firmware Annex type. */
   DSL_DEV_HsToneGroup_t nHsToneGroup_A;
   /**
   Selects one or more groups of tones to be used during the handshake phase
   in case of activating the link using VDSL xDSL modes only.
   \note This parameter is only used in case of activating manual mode for
         handshake tone group selection (\ref DSL_DEV_HS_TONE_GROUP_MODE_MANUAL) */
   DSL_DEV_HsToneGroup_t nHsToneGroup_V;
   /**
   Selects one or more groups of tones to be used during the handshake phase
   in case of activating the link using multimode ADSL+VDSL xDSL modes.
   \note This parameter is only used in case of activating manual mode for
         handshake tone group selection (\ref DSL_DEV_HS_TONE_GROUP_MODE_MANUAL)
   \attention In manual mode there is no logic within API internal handling!
         This means that the ADSL related handshake tones has to be selected
         in accordance to the used firmware Annex type. */
   DSL_DEV_HsToneGroup_t nHsToneGroup_AV;
   /**
   Base Address of the VRX device */
   DSL_uint32_t nBaseAddr;
   /**
   IRQ of the VRX device */
   DSL_int8_t nIrqNum;
   /**
   Network Timing Reference Control */
   DSL_boolean_t bNtrEnable;
} DSL_DeviceLowLevelConfig_t;


/**
   Device configuration
*/
struct DSL_DeviceConfig
{
   /**
      Low level device configuration structure includes specific
      settings*/
   DSL_DeviceLowLevelConfig_t cfg;
   /**
   System interface configuration*/
   DSL_SystemInterfaceConfigData_t sysCIF;
   /** MAC configuration settings*/
   DSL_EFM_MacConfigData_t MacCfg;
#ifdef HAS_TO_BE_CROSSCHECKED_FOR_VRX
   /** SAR Configuration*/
   DSL_SAR_ConfigData_t SARcfg;
   /** Segmentation Table*/
   DSL_SAR_SegmentationTableData_t SegmentationTable;
   /** Reassembly Table*/
   DSL_SAR_ReassemblyTableData_t ReassemblyTable;
#endif
   /** Common channel configuration parameters
       (see chapter 7.3.2.1 of G.997.1), for the UPSTREAM and DOWNSTREAM direction
       Only for the bearer channel 0*/
   DSL_G997_ChannelConfigData_t ChannelConfigData[2];
   /** Actual selected Band Plan/Profile*/
   DSL_BandPlanConfigData_t bandPlanStatusNe;
   /**
      Structure to return the current type of firmware request.
      It defines which kind of firmware is needed for the next reboot
      sequence. */
   DSL_FirmwareRequestType_t nFwType;
   /** Multimode related initial configurations. */
   DSL_MultimodeFsmConfigData_t nMultimodeCfg;
   /** Initial configurations for handshake related link activation handling. */
   DSL_ActivationFsmConfigData_t nActivationCfg;
};


/** @} DRV_DSL_DEVICE */

#ifdef __cplusplus
}
#endif

#endif /* DRV_DSL_CPE_VRX_CTX_H */


