/******************************************************************************

                               Copyright (c) 2011
                            Lantiq Deutschland GmbH
                     Am Campeon 3; 85579 Neubiberg, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef _DRV_DSL_CPE_INTERN_SAR_H
#define _DRV_DSL_CPE_INTERN_SAR_H

#ifdef __cplusplus
   extern "C" {
#endif

#include "drv_dsl_cpe_api.h"
#include "drv_dsl_cpe_api_sar.h"

/** \file
   This file specifies the internal functions that are used for the SAR
   specific implementation of the ioctl interface.
   It is intendet to be used within the DSL CPE API driver ONLY.
*/

DSL_Error_t DSL_DRV_SAR_ConfigCheck(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_SAR_ConfigData_t *pData);

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_SAR_CONFIG_SET
*/
DSL_Error_t DSL_DRV_SAR_ConfigSet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_SAR_Config_t *pData
);

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_SAR_CONFIG_SET
*/
#ifdef INCLUDE_DSL_CONFIG_GET
DSL_Error_t DSL_DRV_SAR_ConfigGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_SAR_Config_t *pData
);
#endif /* INCLUDE_DSL_CONFIG_GET*/

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_SAR_SEGMENTATION_TABLE_SET
*/
DSL_Error_t DSL_DRV_SAR_SegmentationTableSet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_SAR_SegmentationTable_t *pData
);

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_SAR_SEGMENTATION_TABLE_GET
*/
DSL_Error_t DSL_DRV_SAR_SegmentationTableGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_SAR_SegmentationTable_t *pData
);

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_SAR_REASSEMBLY_TABLE_SET
*/
DSL_Error_t DSL_DRV_SAR_ReassemblyTableSet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_SAR_ReassemblyTable_t *pData
);

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_SAR_REASSEMBLY_TABLE_GET
*/
DSL_Error_t DSL_DRV_SAR_ReassemblyTableGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_SAR_ReassemblyTable_t *pData
);

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_SAR_SEGMENTATION_COUNTERS_GET
*/
DSL_Error_t DSL_DRV_SAR_SegmentationCountersGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_SAR_SegmentationCounters_t *pData
);

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_SAR_REASSEMBLY_COUNTERS_GET
*/
DSL_Error_t DSL_DRV_SAR_ReassemblyCountersGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_SAR_ReassemblyCounters_t *pData
);

#ifdef __cplusplus
}
#endif

#endif /* _DRV_DSL_CPE_INTERN_SAR_H */
