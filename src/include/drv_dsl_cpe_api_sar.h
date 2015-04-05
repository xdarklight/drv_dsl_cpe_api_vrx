/******************************************************************************

                               Copyright (c) 2011
                            Lantiq Deutschland GmbH
                     Am Campeon 3; 85579 Neubiberg, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef _DRV_DSL_API_SAR_H
#define _DRV_DSL_API_SAR_H

#ifdef __cplusplus
   extern "C" {
#endif

#include "drv_dsl_cpe_api.h"

/** \file
   SAR interface definitions
*/

/** \addtogroup DRV_DSL_CPE_SAR
 @{ */

/**
   Number of maximum supported entries in the
   Segmentation forwarding table \ref DSL_SAR_SegmentationTable_t */
#define DSL_SAR_MAX_SEG_TABLE_ENTRIES  8
/**
   Number of maximum supported entries in the
   Reassembly table \ref DSL_SAR_ReassemblyTable_t */
#define DSL_SAR_MAX_REA_TABLE_ENTRIES  8


/**
   Defines the classification filter in the ethernet packet.
   This filter is common for all segmentation classifications.
*/
typedef enum
{
   /**
   Classify using the MAC address */
   DSL_SAR_FILTER_MAC_DEST_ADDR = 0,
   /**
   Classify using the VLAN ID */
   DSL_SAR_FILTER_VLAN_ID = 1,
   /**
   Classify using the VLAN Priority */
   DSL_SAR_FILTER_VLAN_PRIO = 2,
   /**
   Classify using the MAC source address */
   DSL_SAR_FILTER_MAC_SRC_ADDR = 3,
   /**
   Delimiter */
   DSL_SAR_FILTER_LAST
} DSL_SAR_SegClassFilter_t;

/**
   Multiplexing method types LLC or VC-Mux.
   See RFC2684 Chapter 3. */
typedef enum
{
   /**
   VC multiplexing method */
   DSL_SAR_MUX_VC_MUX = 0,
   /**
   LLC encapsulation */
   DSL_SAR_MUX_LLC = 1,
   /**
   Delimiter */
   DSL_SAR_MUX_LAST
} DSL_SAR_MuxType_t;

/**
   SAR Configuration Data
*/
typedef struct
{
   /**
   Defines the classification filter type in the ethernet packet.
   This filter is common for all segmentation classifications. */
   DSL_CFG DSL_SAR_SegClassFilter_t nSegClassFilter;
   /**
   Defines the default VPI to be used for all mismatch classifications.
   For the Segmentation process only. */
   DSL_CFG DSL_uint16_t nDefVpi;
   /**
   Defines the default VCI to be used for all mismatch classifications.
   For the Segmentation process only. */
   DSL_CFG DSL_uint16_t nDefVci;
   /**
   Selection of the Multiplexing method LLC or VC-Mux.
   See RFC2684 Chapter 3. */
   DSL_CFG DSL_SAR_MuxType_t nMuxType;
   /**
   Defines wether the FCS shall be preserved in the PDU or not.
   See RFC2684 Chapter 5.2. */
   DSL_CFG DSL_boolean_t bFcsPreserve;
} DSL_SAR_ConfigData_t;

/**
   This structure has to be used for ioctl
   - \ref DSL_FIO_SAR_CONFIG_SET
   - \ref DSL_FIO_SAR_CONFIG_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Specifies for which (bearer) channel the function will apply */
   DSL_IN DSL_uint8_t nChannel;
   /**
   Structure that contains all necessary configuration data */
   DSL_CFG DSL_SAR_ConfigData_t data;
} DSL_SAR_Config_t;


/**
   Segmentation Filter and forwarding Table Entries
*/
typedef struct
{
   DSL_CFG DSL_uint8_t pMacAddress[DSL_MAC_ADDRESS_OCTETS];
   /**
   VLAN Identifier. Correct range 0 < nVlanId < 4095*/
   DSL_CFG DSL_uint32_t nVlanId;
   /**
   VLAN priority. 0<= nVlanPrio <= 7*/
   DSL_CFG DSL_uint32_t nVlanPrio;
   /**
   Defines the VPI. 0 <= nVpi <=255*/
   DSL_CFG DSL_uint16_t nVpi;
   /**
   Defines the VCI. No 31 < nVciVC <= 65535*/
   DSL_CFG DSL_uint16_t nVci;
} DSL_SAR_SegmentationTableEntry_t;

/**
   Segmentation table data
*/
typedef struct
{
   /**
   Number of used table entries in this structure */
   DSL_CFG DSL_uint8_t nNumEntries;
   /**
   Array of Forwarding table entries */
   DSL_CFG DSL_SAR_SegmentationTableEntry_t pTableEntry[DSL_SAR_MAX_SEG_TABLE_ENTRIES];
} DSL_SAR_SegmentationTableData_t;

/**
   This structure has to be used for ioctl
   - \ref DSL_FIO_SAR_SEGMENTATION_TABLE_SET
   - \ref DSL_FIO_SAR_SEGMENTATION_TABLE_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Specifies for which (bearer) channel the function will apply */
   DSL_IN DSL_uint8_t nChannel;
   /**
   Structure that contains all necessary configuration data */
   DSL_CFG DSL_SAR_SegmentationTableData_t data;
} DSL_SAR_SegmentationTable_t;

/**
   Reassembly Table Entries
*/
typedef struct
{
   /**
   Defines the VPI. Valid range 0 <= nVpi <=255*/
   DSL_CFG DSL_uint16_t nVpi;
   /**
   Defines the VCI. Valid range 31 < nVci < 65535*/
   DSL_CFG DSL_uint16_t nVci;
} DSL_SAR_ReassemblyTableEntry_t;

/**
   Reassembly Filter Table
*/
typedef struct
{
   /**
   Number of used table entries in this structure */
   DSL_CFG DSL_uint8_t nNumEntries;
   /**
   Array of Forwarding table entries */
   DSL_CFG DSL_SAR_ReassemblyTableEntry_t pTableEntry[DSL_SAR_MAX_REA_TABLE_ENTRIES];
} DSL_SAR_ReassemblyTableData_t;

/**
   This structure has to be used for ioctl
   - \ref DSL_FIO_SAR_REASSEMBLY_TABLE_SET
   - \ref DSL_FIO_SAR_REASSEMBLY_TABLE_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Specifies for which (bearer) channel the function will apply */
   DSL_IN DSL_uint8_t nChannel;
   /**
   Structure that contains all necessary configuration data */
   DSL_SAR_ReassemblyTableData_t data;
} DSL_SAR_ReassemblyTable_t;

/**
   Segmentation counters for all valid VCC entries of the forwarding table.
*/
typedef struct
{
   /**
   Number of received Ethernet frames with no VPI/VCI mapping table entry. */
   DSL_OUT DSL_uint32_t nInvalidFramesRx;
   /**
   Number of received Ethernet frames with default VPI/VCI mapping table entry. */
   DSL_OUT DSL_uint32_t nDefaultFramesRx;
   /**
   ATM cell with default VPI/VCI segmented.
   Depending on the xTC configuration, cells are counted either for the
   transmit or receive direction. */
   DSL_OUT DSL_uint32_t nDefaultAtm;
   /**
   Number of received Ethernet frames to be segmented for VCCn.
   Each counter corresponds to one VCC (VPI/VCI) entry out of the complete
   Ethernet forwarding table. */
   DSL_OUT DSL_uint32_t nFramesRx[DSL_SAR_MAX_SEG_TABLE_ENTRIES];
   /**
   Number of generated ATM segments for one VCCn.
   Each counter corresponds to one VCC (VPI/VCI) entry out of the complete
   Ethernet forwarding table. */
   DSL_OUT DSL_uint32_t nAtm[DSL_SAR_MAX_SEG_TABLE_ENTRIES];
} DSL_SAR_SegmentationCountersData_t;

/**
   Segmentation counters for all valid VCC entries of the forwarding table.
   This structure has to be used for ioctl
   \ref DSL_FIO_SAR_SEGMENTATION_COUNTERS_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Specifies for which (bearer) channel the function will apply */
   DSL_IN DSL_uint8_t nChannel;
   /**
   Structure that contains segmentation counter data */
   DSL_OUT DSL_SAR_SegmentationCountersData_t data;
} DSL_SAR_SegmentationCounters_t;

/**
   Reassembly counters for all valid VCC entries of the forwarding table
*/
typedef struct
{
   /**
   Number of dropped ATM cells due to unsupported VPI/VCI
   (UNI or NNI dependent). */
   DSL_uint32_t nInvalidAtm;

   /**
   Number of dropped Ethernet frames due to unsupported LLC. */
   DSL_uint32_t nInvalidFrames;
   /**
   Number of mismatches in calculated and received CRC of VPI/VCI.
   The counter corresponds to one VCC (VPI/VCI) entry out of the complete
   Ethernet forwarding table.
   */
   DSL_OUT DSL_uint32_t nCrc[DSL_SAR_MAX_REA_TABLE_ENTRIES];
   /**
   Number of transmitted reassembled Ethernet frames of VPI/VCI.
   The counter corresponds to one VCC (VPI/VCI) entry out of the complete
   Ethernet forwarding table.
   */
   DSL_OUT DSL_uint32_t nFrames[DSL_SAR_MAX_REA_TABLE_ENTRIES];
} DSL_SAR_ReassemblyCountersData_t;

/**
   Reassembly counters for all valid VCC entries of the forwarding table
   This structure has to be used for ioctl
   \ref DSL_FIO_SAR_REASSEMBLY_COUNTERS_GET
*/
typedef struct
{
   /**
   Driver control/status structure */
   DSL_IN_OUT DSL_AccessCtl_t accessCtl;
   /**
   Specifies for which (bearer) channel the function will apply */
   DSL_IN DSL_uint8_t nChannel;
   /**
   Structure that contains reassembley counter data */
   DSL_SAR_ReassemblyCountersData_t data;
} DSL_SAR_ReassemblyCounters_t;


/** @} DRV_DSL_CPE_SAR */

#ifdef __cplusplus
}
#endif

#endif /* _DRV_DSL_API_SAR_H */
