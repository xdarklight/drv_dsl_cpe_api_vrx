/******************************************************************************

                              Copyright (c) 2014
                            Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef _DRV_DSL_CPE_OS_WIN32_H
#define _DRV_DSL_CPE_OS_WIN32_H

#ifdef __cplusplus
   extern "C" {
#endif

#include "drv_dsl_cpe_api.h"

/** IFXOS includes*/
#include "ifxos_common.h"
#include "ifxos_copy_user_space.h"
#include "ifxos_debug.h"
#include "ifxos_event.h"
#include "ifxos_thread.h"
#include "ifxos_memory_alloc.h"
#include "ifxos_memory_map.h"
#include "ifxos_lock.h"
#include "ifxos_time.h"
#include "ifxos_select.h"
#include "ifxos_mutex.h"
#include "ifxos_device_io.h"

#include "errno.h"

#ifndef DSL_DRV_STACKSIZE
#define DSL_DRV_STACKSIZE 2048
#endif

#ifndef DSL_DRV_PRIORITY
#define DSL_DRV_PRIORITY  64
#endif

#define DSL_BYTE_ORDER           IFXOS_BYTE_ORDER
#define DSL_LITTLE_ENDIAN        IFXOS_LITTLE_ENDIAN
#define DSL_BIG_ENDIAN           IFXOS_BIG_ENDIAN

typedef IFX_int_t                DSL_DRV_size_t;
typedef IFX_uint32_t             DSL_DRV_TimeVal_t;
typedef IFXOS_lock_t             DSL_DRV_Mutex_t;
typedef IFXOS_event_t            DSL_DRV_Event_t;
typedef IFXOS_drvSelectQueue_t   DSL_DRV_WaitQueue_t;

#define  IOCPARM_MASK   0x7f     /* parameters must be < 128 bytes */
#define  IOC_VOID       0x20000000  /* no parameters */
#define  IOC_OUT         0x40000000 /* copy out parameters */
#define  IOC_IN          0x80000000 /* copy in parameters */
#define  IOC_INOUT      (IOC_IN|IOC_OUT)

#ifndef _IO
#define  _IO(x,y)    (IOC_VOID|((x)<<8)|y)
#endif /* _IO*/
#ifndef _IOR
#define  _IOR(x,y,t)    (IOC_OUT|((sizeof(t)&IOCPARM_MASK)<<16)|((x)<<8)|y)
#endif /* _IOR*/
#ifndef _IOW
#define  _IOW(x,y,t)    (IOC_IN|((sizeof(t)&IOCPARM_MASK)<<16)|((x)<<8)|y)
#endif /* _IOW*/
#ifndef _IOWR
#define  _IOWR(x,y,t)   (IOC_INOUT|((sizeof(t)&IOCPARM_MASK)<<16)|((x)<<8)|y)
#endif /* _IOWR*/

#define _IOC_TYPE(x)                      (((x)>>8) & 0xFF)

/* HAS TO BE CLARIFIED*/
#define DSL_Le2Cpu(x)   (x)

#define DSL_DRV_SIGNAL_PENDING               0

/** IFXOS specific mapping for the system calls*/
#define DSL_DRV_Malloc(nSize)                IFXOS_MemAlloc(nSize)
#define DSL_DRV_MemFree(pPtr)                IFXOS_MemFree(pPtr)
#define DSL_DRV_PMalloc(nSize)               IFXOS_BlockAlloc(nSize)
#define DSL_DRV_PFree(pPtr)                  IFXOS_BlockFree(pPtr)
#define DSL_DRV_MUTEX_INIT(id)               IFXOS_LockInit(&(id))
#define DSL_DRV_MUTEX_LOCK(id)               IFXOS_LockGet(&(id))
#define DSL_DRV_MUTEX_UNLOCK(id)             IFXOS_LockRelease(&(id))
#define DSL_DRV_INIT_WAKELIST(name,queue)    IFXOS_DrvSelectQueueInit(&(queue))
#define DSL_DRV_WAKEUP_WAKELIST(queue)       IFXOS_DrvSelectQueueWakeUp(&(queue), IFXOS_DRV_SEL_WAKEUP_TYPE_RD)
#define DSL_DRV_ADD_TASK_WAKELIST            IFXOS_DrvSelectQueueAddTask

#define DSL_DRV_INIT_EVENT(name,ev)          IFXOS_EventInit(&(ev));
#define DSL_DRV_WAIT_EVENT_TIMEOUT(ev,t)     IFXOS_EventWait(&(ev), (t), IFX_NULL)
#define DSL_DRV_WAIT_EVENT(ev)               IFXOS_EventWait(&(ev), 0xFFFFFFFF, IFX_NULL)
#define DSL_DRV_WAKEUP_EVENT(ev)             IFXOS_EventWakeUp(&(ev));
#define DSL_DRV_ElapsedTimeMSecGet(t)        IFXOS_ElapsedTimeMSecGet(t)
#define DSL_DRV_TimeMSecGet()                IFXOS_ElapsedTimeMSecGet(0)
#define DSL_WAIT(ms)                         IFXOS_MSecSleep(ms)
#define DSL_DRV_MSecSleep(msec)              IFXOS_MSecSleep(msec)

#define DSL_IsTimeNull(t)                    ((t) == 0)
#define DSL_DRV_TimeSecGet(t)                (t)

#define DSL_DRV_OS_ModUseCountIncrement()
#define DSL_DRV_OS_ModUseCountDecrement()

#define DSL_DRV_Phy2VirtMap                  IFXOS_Phy2VirtMap
#define DSL_DRV_Phy2VirtUnmap                IFXOS_Phy2VirtUnmap

/** IFXOS specific mapping*/
typedef IFXOS_ThreadParams_t     DSL_DRV_ThreadParams_t;
typedef IFXOS_ThreadFunction_t   DSL_DRV_ThreadFunc_t;
typedef IFXOS_ThreadCtrl_t       DSL_DRV_ThreadCtrl_t;

#define DSL_DRV_THREAD(a, b, c, d)        IFXOS_ThreadInit((a), (b), (c), DSL_DRV_STACKSIZE, DSL_DRV_PRIORITY, (d), 0)
#define DSL_DRV_WAIT_COMPLETION(a)        IFXOS_ThreadShutdown((a), 3000)
#define DSL_DRV_THREAD_DELETE(a, b)       ((void)0)

#define DSL_DRV_CRLF "\n"

/*
   System Time Get
*/
DSL_uint32_t DSL_DRV_SysTimeGet(DSL_uint32_t nOffset);

/*
   DSL CPE API driver Device Create
*/
DSL_int_t DSL_DRV_SIM_DeviceCreate(DSL_void_t);

/*
   DSL CPE API driver Device Delete
*/
DSL_void_t DSL_DRV_SIM_DeviceDelete(DSL_void_t);

#ifdef __cplusplus
}
#endif

#endif
