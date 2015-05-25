/******************************************************************************

                              Copyright (c) 2013
                            Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef _DRV_DSL_CPE_PROCFS_LINUX_H
#define _DRV_DSL_CPE_PROCFS_LINUX_H

#ifdef DSL_INTERN

#include "drv_dsl_cpe_api.h"

#ifndef _lint
#include "drv_dsl_cpe_os_linux.h"
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#else
#include "drv_dsl_cpe_os_lint_map.h"
#endif /* #ifndef _lint*/

#ifdef __cplusplus
   extern "C" {
#endif

#ifdef INCLUDE_DSL_CPE_PROCFILES

#define DSL_DRV_PROC_ENTRY_ADD(name, pFshow, pFget, bDevSpecific) \
   {name, pFshow, pFget, bDevSpecific, {{0}}}

typedef enum
{
   /* Format as hey values */
   DSL_ARRAY_FORMAT_HEX,
   /* Format as string */
    DSL_ARRAY_FORMAT_STRING
} DSL_DRV_ArrayPrintFormat_t;

typedef DSL_int_t (*DSL_DRV_PROC_EntryShow_t)(struct seq_file*);
typedef DSL_void_t * (*DSL_DRV_PROC_EntryDataGet_t)(struct seq_file*);

typedef struct proc_dir_entry DSL_DRV_PROC_DirEntry_t;

typedef struct
{
   /**
   Iteration number*/
   DSL_uint32_t v;
   /**
   Sequence end flag*/
   DSL_boolean_t bEnd;
   /**
   Private entry data*/
   DSL_void_t *pPrivateData;
   /**
   Printout data pointer*/
   DSL_void_t *pPrintData;
} DSL_DRV_PROC_SecFilePrivateData_t;

typedef struct
{
   /**
   device number*/
   DSL_int_t devNum;
   /**
   proc entry SHOW handler*/
   DSL_DRV_PROC_EntryShow_t    pProcShow;
   /**
   proc entry DATA handler*/
   DSL_DRV_PROC_EntryDataGet_t pProcDataGet;
   DSL_DRV_PROC_SecFilePrivateData_t SecPrivateData;
} DSL_DRV_PROC_EntryData_t;

typedef struct
{
   /**
   proc entry string identifier*/
   const DSL_char_t * const string;
   /**
   proc entry handler*/
   DSL_DRV_PROC_EntryShow_t    const pProcShow;
   DSL_DRV_PROC_EntryDataGet_t const pProcDataGet;
   /**
   is entry device specific*/
   const DSL_boolean_t bDevSpecific;
   /**
   device proc entry data, data which is passed to the "read_proc" system call*/
   DSL_DRV_PROC_EntryData_t entryData[DSL_DRV_MAX_ENTITIES];
} DSL_DRV_PROC_EntryTable_t;

extern DSL_DRV_PROC_EntryTable_t procEntryTable[];

#ifndef SWIG
DSL_int_t DSL_DRV_InstallProcEntry(DSL_void_t);
#endif /* SWIG */

#ifndef SWIG
DSL_int_t DSL_DRV_RemoveProcEntry(DSL_void_t);
#endif /* SWIG */

#endif /* #ifdef INCLUDE_DSL_CPE_PROCFILES*/

#ifdef __cplusplus
}
#endif

#endif /* DSL_INTERN*/

#endif /* _DRV_DSL_CPE_PROCFS_LINUX_H*/
