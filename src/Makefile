RGSRC=../../../../../../
include $(RGSRC)/envir.mak

DSL_CPE_API_MOD=drv_dsl_cpe_api_mod.o
MOD_TARGET+=$(DSL_CPE_API_MOD)
RAMDISK_MODULES_PERMANENT_FILES+=$(DSL_CPE_API_MOD)

O_OBJS_$(DSL_CPE_API_MOD)+=common/drv_dsl_cpe_api.o \
  common/drv_dsl_cpe_fifo.o common/drv_dsl_cpe_autoboot.o \
  common/drv_dsl_cpe_debug.o common/drv_dsl_cpe_os_linux.o \
  common/drv_dsl_cpe_timeout.o pm/drv_dsl_cpe_api_pm.o \
  pm/drv_dsl_cpe_pm_core.o pm/drv_dsl_cpe_api_pm_vrx.o \
  g997/drv_dsl_cpe_api_g997.o g997/drv_dsl_cpe_api_g997_vrx.o \
  ceoc/drv_dsl_cpe_ceoc.o ceoc/drv_dsl_cpe_intern_ceoc.o \
  device/drv_dsl_cpe_device_vrx.o \
  device/drv_dsl_cpe_msg_vrx.o \
  mib/drv_dsl_cpe_adslmib.o

ARCHCONFIG_FIRST_TASKS+=dsl_cpe_api_subdirs
dsl_cpe_api_subdirs:
	$(MKDIR) $(PWD_BUILD)/{common,pm,ceoc,g997,led,device,mib,sar}

DSL_CPE_API_EXPORT_HEADERS=include/drv_dsl_cpe_api_bnd.h \
  include/drv_dsl_cpe_api_pm.h include/drv_dsl_cpe_intern_pm.h \
  include/drv_dsl_cpe_device_pm.h include/drv_dsl_cpe_pm_core.h \
  include/drv_dsl_cpe_api_ceoc_intern.h include/drv_dsl_cpe_api_ceoc.h \
  include/drv_dsl_cpe_intern_ceoc.h include/drv_dsl_cpe_device_ceoc.h \
  include/drv_dsl_cpe_api.h include/drv_dsl_cpe_fifo.h \
  include/drv_dsl_cpe_api_error.h include/drv_dsl_cpe_api_g997.h \
  include/drv_dsl_cpe_api_ioctl.h \
  include/drv_dsl_cpe_api_types.h include/drv_dsl_cpe_autoboot.h \
  include/drv_dsl_cpe_debug.h include/drv_dsl_cpe_device.h \
  include/drv_dsl_cpe_timeout.h include/drv_dsl_cpe_intern.h \
  include/drv_dsl_cpe_intern_g997.h include/drv_dsl_cpe_device_g997.h \
  include/drv_dsl_cpe_intern_mib.h \
  include/drv_dsl_cpe_os_lint_map.h \
  include/drv_dsl_cpe_os.h include/drv_dsl_cpe_os_linux.h \
  include/drv_dsl_cpe_vrx_msg_config_pmd.h \
  include/drv_dsl_cpe_vrx_msg_dmt_scope.h \
  include/drv_dsl_cpe_vrx_msg_ghs_status.h \
  include/drv_dsl_cpe_vrx_msg_olr.h include/drv_dsl_cpe_vrx_msg_perf_pmd.h \
  include/drv_dsl_cpe_device_vrx.h \
  include/drv_dsl_cpe_msg_vrx.h include/drv_dsl_cpe_vrx_msg_const.h \
  include/drv_dsl_cpe_vrx_msg_tc_counters.h include/drv_dsl_cpe_vrx_ctx.h \
  include/drv_dsl_cpe_api_adslmib.h \
  include/drv_dsl_cpe_api_adslmib_ioctl.h include/drv_dsl_cpe_api_config.h

EXPORT_HEADERS_DIR=vendor/dsl
JPKG_EXPORTED_FILES+=$(DSL_CPE_API_EXPORT_HEADERS)
EXPORT_HEADERS=$(DSL_CPE_API_EXPORT_HEADERS)

ARCHCONFIG_FIRST_TASKS+=$(DSL_CPE_API_INCLUDE_DIR)
$(DSL_CPE_API_INCLUDE_DIR):
	$(MKDIR) $@
	$(RG_CP_LN) $(foreach f,$(DSL_CPE_API_EXPORT_HEADERS),$(PWD_SRC)/$(f)) $@

RAMDISK_LAST_TASKS+=dsl_cpe_api_devs
dsl_cpe_api_devs:
	$(MKDIR) $(RAMDISK_DEV_DIR)/dsl_cpe_api 
	$(MKDIR) $(RAMDISK_DEV_DIR)/mei_cpe 
	$(RG_MKNOD) $(RAMDISK_DEV_DIR)/dsl_cpe_api/0 c 109 0
	$(RG_MKNOD) $(RAMDISK_DEV_DIR)/mei_cpe/0 c 105 0

include $(RGMK)
