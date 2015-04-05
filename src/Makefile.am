## Process this file with automake to produce Makefile.in

# Target definition depending on kernel version:
if KERNEL_2_6
bin_PROGRAMS = drv_dsl_cpe_api.ko
if DSL_CPE_API_LIBRARY_BUILD_2_6
noinst_LIBRARIES = lib_dsl_cpe_api.a
endif
else
bin_PROGRAMS = drv_dsl_cpe_api
noinst_LIBRARIES = lib_dsl_cpe_api.a
endif


if DEVICE_DANUBE
bin_SCRIPTS = ../scripts/inst_drv_dsl_cpe_api_danube.sh
endif

if DEVICE_VINAX
bin_SCRIPTS = ../scripts/inst_drv_dsl_cpe_api_vinax.sh
endif

## sources for BND module
drv_bnd_sources = \
	bnd/drv_dsl_cpe_api_bnd.c

## extra for BND
drv_bnd_extra = \
	include/drv_dsl_cpe_api_bnd.h \
	include/drv_dsl_cpe_api_bnd_vinax.h \
	include/drv_dsl_cpe_api_bnd_vinax_fpga.h

## sources for PM module
drv_pm_sources = \
	pm/drv_dsl_cpe_api_pm.c \
	pm/drv_dsl_cpe_pm_core.c

## extra for PM
drv_pm_extra = \
	include/drv_dsl_cpe_api_pm.h \
	include/drv_dsl_cpe_intern_pm.h \
	include/drv_dsl_cpe_device_pm.h \
	include/drv_dsl_cpe_pm_core.h

## sources for CEOC module
drv_ceoc_sources = \
	ceoc/drv_dsl_cpe_ceoc.c \
	ceoc/drv_dsl_cpe_intern_ceoc.c

## extra for CEOC
drv_ceoc_extra = \
	include/drv_dsl_cpe_api_ceoc_intern.h \
	include/drv_dsl_cpe_api_ceoc.h \
	include/drv_dsl_cpe_intern_ceoc.h \
	include/drv_dsl_cpe_device_ceoc.h

## sources for ADSL MIB module
drv_adsl_mib_sources = \
	mib/drv_dsl_cpe_adslmib.c

## extra for ADSL MIB
drv_adsl_mib_extra= \
	include/drv_dsl_cpe_api_adslmib.h \
	include/drv_dsl_cpe_api_adslmib_ioctl.h

## common driver sources always needed for build
drv_common_sources = \
	common/drv_dsl_cpe_api.c \
	common/drv_dsl_cpe_fifo.c \
	common/drv_dsl_cpe_autoboot.c \
	common/drv_dsl_cpe_debug.c \
	common/drv_dsl_cpe_os_linux.c \
	common/drv_dsl_cpe_timeout.c \
	g997/drv_dsl_cpe_api_g997.c \
	led/drv_dsl_cpe_led.c

## common driver sources always included additionally in the distribution
drv_common_extra = \
	include/drv_dsl_cpe_api.h \
	include/drv_dsl_cpe_fifo.h \
	include/drv_dsl_cpe_api_error.h \
	include/drv_dsl_cpe_api_g997.h \
	include/drv_dsl_cpe_api_ioctl.h \
	include/drv_dsl_cpe_api_sar.h \
	include/drv_dsl_cpe_api_types.h \
	include/drv_dsl_cpe_autoboot.h \
	include/drv_dsl_cpe_debug.h \
	include/drv_dsl_cpe_device.h \
	include/drv_dsl_cpe_timeout.h \
	include/drv_dsl_cpe_intern.h \
	include/drv_dsl_cpe_intern_g997.h \
	include/drv_dsl_cpe_device_g997.h \
	include/drv_dsl_cpe_intern_led.h \
	include/drv_dsl_cpe_intern_mib.h \
	include/drv_dsl_cpe_intern_sar.h \
	include/drv_dsl_cpe_os_lint_map.h \
	include/drv_dsl_cpe_os.h \
	include/drv_dsl_cpe_os_linux.h \
	common/drv_dsl_cpe_os_vxworks.c \
	include/drv_dsl_cpe_os_vxworks.h \
	common/drv_dsl_cpe_os_rtems.c \
	include/drv_dsl_cpe_os_rtems.h \
	include/drv_dsl_cpe_api_config.h.in

## Files specific for the Danube
## driver sources
drv_danube_specific_sources = \
	device/drv_dsl_cpe_device_danube.c \
	g997/drv_dsl_cpe_api_g997_danube.c

if INCLUDE_DSL_PM
drv_pm_danube_specific_sources = \
	pm/drv_dsl_cpe_api_pm_danube.c
endif

if INCLUDE_DSL_CEOC
drv_ceoc_danube_specific_sources = \
	ceoc/drv_dsl_cpe_danube_ceoc.c
endif

## extra distribution for Danube
drv_danube_specific_extra = \
	include/drv_dsl_cpe_device_danube.h \
	include/drv_dsl_cpe_danube.h \
	include/drv_dsl_cpe_cmv_danube.h \
	include/drv_dsl_cpe_danube_ctx.h \
	../scripts/inst_drv_dsl_cpe_api_danube.sh

## Files specific for the Vinax
## driver sources
drv_vinax_specific_sources = \
	device/drv_dsl_cpe_device_vxx.c \
	device/drv_dsl_cpe_device_vinax.c \
	device/drv_dsl_cpe_msg_vinax.c \
	g997/drv_dsl_cpe_api_g997_vinax.c \
	sar/drv_dsl_cpe_sar.c

## Files specific for the VRx
## driver sources
drv_vrx_specific_sources = \
	device/drv_dsl_cpe_device_vxx.c \
	device/drv_dsl_cpe_device_vrx.c \
	device/drv_dsl_cpe_msg_vrx.c \
	g997/drv_dsl_cpe_api_g997_vrx.c \
	sar/drv_dsl_cpe_sar.c

if INCLUDE_DSL_PM
drv_pm_vinax_specific_sources = \
	pm/drv_dsl_cpe_api_pm_vinax.c
drv_pm_vrx_specific_sources = \
	pm/drv_dsl_cpe_api_pm_vrx.c
endif

if INCLUDE_DSL_BONDING
drv_bnd_vinax_specific_sources = \
	bnd/drv_dsl_cpe_api_bnd_vinax.c \
	bnd/drv_dsl_cpe_api_bnd_vinax_fpga.c
endif

if INCLUDE_DSL_CEOC
drv_ceoc_vinax_specific_sources = \
	ceoc/drv_dsl_cpe_vinax_ceoc.c
drv_ceoc_vrx_specific_sources = \
	ceoc/drv_dsl_cpe_vrx_ceoc.c
endif

## extra distribution for Vinax
drv_vinax_specific_extra = \
	include/drv_dsl_cpe_msg_vinax.h \
	include/drv_dsl_cpe_device_vinax.h \
	include/drv_dsl_cpe_device_vxx.h \
	include/drv_dsl_cpe_vinax_msg_config_pmd.h \
	include/drv_dsl_cpe_vinax_msg_const.h \
	include/drv_dsl_cpe_vinax_msg_debug.h \
	include/drv_dsl_cpe_vinax_msg_dmt_scope.h \
	include/drv_dsl_cpe_vinax_msg_ghs_status.h \
	include/drv_dsl_cpe_vinax_msg_olr.h \
	include/drv_dsl_cpe_vinax_msg_perf_pmd.h \
	include/drv_dsl_cpe_vinax_msg_tc_counters.h \
	include/drv_dsl_cpe_vinax_msg_utopia_posphy.h \
	include/drv_dsl_cpe_vinax_msg_xtc.h \
	include/drv_dsl_cpe_vinax_ctx.h \
	include/drv_dsl_cpe_vinax_mdio.h \
	../scripts/inst_drv_dsl_cpe_api_vinax.sh

## extra distribution for VRx
drv_vrx_specific_extra = \
	include/drv_dsl_cpe_msg_vrx.h \
	include/drv_dsl_cpe_device_vrx.h \
	include/drv_dsl_cpe_device_vxx.h \
	include/drv_dsl_cpe_vrx_msg_config_pmd.h \
	include/drv_dsl_cpe_vrx_msg_const.h \
	include/drv_dsl_cpe_vrx_msg_debug.h \
	include/drv_dsl_cpe_vrx_msg_dmt_scope.h \
	include/drv_dsl_cpe_vrx_msg_ghs_status.h \
	include/drv_dsl_cpe_vrx_msg_olr.h \
	include/drv_dsl_cpe_vrx_msg_perf_pmd.h \
	include/drv_dsl_cpe_vrx_msg_tc_counters.h \
	include/drv_dsl_cpe_vrx_ctx.h \
	../scripts/inst_drv_dsl_cpe_api_vrx.sh

if INCLUDE_DSL_PM
drv_pm_specific = $(drv_pm_sources)
endif

if INCLUDE_DSL_BONDING
drv_bnd_specific = $(drv_bnd_sources)
endif

if INCLUDE_DSL_CEOC
drv_ceoc_specific = $(drv_ceoc_sources)
endif

if INCLUDE_DSL_ADSL_MIB
drv_mib_specific = $(drv_adsl_mib_sources)
endif

## all sources for the current driver build
drv_dsl_cpe_api_SOURCES = \
	$(drv_common_sources) \
	$(drv_pm_specific) \
	$(drv_bnd_specific) \
	$(drv_ceoc_specific) \
	$(drv_mib_specific) \
	$(drv_specific_sources)

if DEVICE_DANUBE
drv_dsl_cpe_api_SOURCES += \
	$(drv_danube_specific_sources) \
	$(drv_pm_danube_specific_sources) \
	$(drv_ceoc_danube_specific_sources)
endif

if DEVICE_VINAX
drv_dsl_cpe_api_SOURCES += \
	$(drv_vinax_specific_sources) \
	$(drv_pm_vinax_specific_sources) \
	$(drv_bnd_vinax_specific_sources) \
	$(drv_ceoc_vinax_specific_sources)
endif

if DEVICE_VRX
drv_dsl_cpe_api_SOURCES += \
	$(drv_vrx_specific_sources) \
	$(drv_pm_vrx_specific_sources) \
	$(drv_ceoc_vrx_specific_sources)
endif

lib_dsl_cpe_api_a_SOURCES = \
	$(drv_common_sources) \
	$(drv_pm_specific) \
	$(drv_ceoc_specific) \
	$(drv_mib_specific) \
	$(drv_specific_sources)

if DEVICE_DANUBE
lib_dsl_cpe_api_a_SOURCES += \
	$(drv_danube_specific_sources) \
	$(drv_pm_danube_specific_sources) \
	$(drv_ceoc_danube_specific_sources)
endif

if DEVICE_VINAX
lib_dsl_cpe_api_a_SOURCES += \
	$(drv_vinax_specific_sources) \
	$(drv_pm_vinax_specific_sources) \
	$(drv_bnd_vinax_specific_sources) \
	$(drv_ceoc_vinax_specific_sources)
endif

if DEVICE_VRX
lib_dsl_cpe_api_a_SOURCES += \
	$(drv_vrx_specific_sources) \
	$(drv_pm_vrx_specific_sources) \
	$(drv_ceoc_vrx_specific_sources)
endif

## all extra sources for the current library distribution
EXTRA_DIST = \
	../LICENSE \
	$(drv_danube_specific_sources) \
	$(drv_pm_danube_specific_sources) \
	$(drv_ceoc_danube_specific_sources) \
	$(drv_vinax_specific_sources) \
	$(drv_pm_vinax_specific_sources) \
	$(drv_bnd_vinax_specific_sources) \
	$(drv_ceoc_vinax_specific_sources) \
	$(drv_vrx_specific_sources) \
	$(drv_pm_vrx_specific_sources) \
	$(drv_ceoc_vrx_specific_sources) \
	$(drv_common_extra) \
	$(drv_pm_extra) \
	$(drv_bnd_extra) \
	$(drv_ceoc_extra) \
	$(drv_adsl_mib_extra) \
	$(drv_danube_specific_extra) \
	$(drv_vinax_specific_extra) \
        $(drv_vrx_specific_extra)

# common driver cflags
if DSL_CPE_API_LIBRARY_BUILD_2_6
drv_dsl_cpe_api_common_mod_cflags = -D"KBUILD_STR(s)=\#s" -D"KBUILD_MODNAME=KBUILD_STR($(subst .o,,$(subst lib_dsl_cpe_api_a-,,$@)))"
else
drv_dsl_cpe_api_common_mod_cflags =
endif

drv_dsl_cpe_api_common_cflags = -DLINUX -D__LINUX__ -D__KERNEL__ -DEXPORT_SYMTAB \
    -pipe -Wall -Wformat -Wimplicit -Wunused -Wswitch -Wcomment -Winline \
    -Wuninitialized -Wparentheses -Wsign-compare -Wreturn-type \
    -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common

if DSL_DBG_MAX_LEVEL_SET
drv_dsl_cpe_api_common_cflags += -DDSL_DBG_MAX_LEVEL=$(DSL_DBG_MAX_LEVEL_PRE)
endif

# target cflags, specified by configure options
drv_dsl_cpe_api_target_cflags = $(ADD_DRV_CFLAGS)

# compile cflags
drv_dsl_cpe_api_compile_cflags = $(EXTRA_DRV_CFLAGS)

if !KERNEL_2_6
# the headerfile of linux kernels 2.6.x contain to much arithmetic
# with void pointers (which is allowed for gcc!)
drv_dsl_cpe_api_common_cflags += -Wpointer-arith
endif

drv_dsl_cpe_api_CFLAGS =  $(drv_dsl_cpe_api_common_cflags) \
                          $(drv_dsl_cpe_api_target_cflags) \
                          $(drv_dsl_cpe_api_device_flags) \
                          $(drv_dsl_cpe_api_compile_cflags)

lib_dsl_cpe_api_a_CFLAGS = $(drv_dsl_cpe_api_common_mod_cflags) \
                           $(drv_dsl_cpe_api_common_cflags) \
                           $(drv_dsl_cpe_api_target_cflags) \
                           $(drv_dsl_cpe_api_device_flags) \
                           $(drv_dsl_cpe_api_compile_cflags)

AM_CPPFLAGS= \
    -I@srcdir@ -I@srcdir@/include \
    $(DSL_DRIVER_INCL_PATH) \
	$(IFXOS_INCLUDE_PATH) \
    -I@KERNEL_BUILD_PATH@/include \
    -I@KERNEL_INCL_PATH@


drv_dsl_cpe_api_LDFLAGS = -r -nostdlib

# linux 2.6 kernel object
drv_dsl_cpe_api_ko_SOURCES = ../README

if KERNEL_2_6

drv_dsl_cpe_api_OBJS = "$(subst .c,.o,$(filter %.c,$(drv_dsl_cpe_api_SOURCES)))"

drv_dsl_cpe_api.ko: $(drv_dsl_cpe_api_SOURCES)
	@echo -e "drv_dsl_cpe_api: Making Linux 2.6.x kernel object"
	if test ! -e common/drv_dsl_cpe_api.c ; then \
		echo "copy source files (as links only!)"; \
		for f in $(filter %.c,$(drv_dsl_cpe_api_SOURCES)); do \
			mkdir -p $(PWD)/`dirname $$f`/ ; \
			cp -s $(addprefix @abs_srcdir@/,$$f) $(PWD)/`dirname $$f`/ ; \
		done \
	fi
	@echo -e "# drv_dsl_cpe_api: Generated to build Linux 2.6.x kernel object" > $(PWD)/Kbuild
	@echo -e "obj-m := $(subst .ko,.o,$@)"			>> $(PWD)/Kbuild
	@echo -e "$(subst .ko,,$@)-y := $(drv_dsl_cpe_api_OBJS)"	>> $(PWD)/Kbuild
	@echo -e "EXTRA_CFLAGS := $(CFLAGS) -DHAVE_CONFIG_H $(drv_dsl_cpe_api_CFLAGS) $(DSL_DRIVER_INCL_PATH) $(IFXOS_INCLUDE_PATH) -I@abs_srcdir@/include -I$(PWD)/include"	>> $(PWD)/Kbuild
	$(MAKE) ARCH=@KERNEL_ARCH@ -C @KERNEL_BUILD_PATH@ O=@KERNEL_BUILD_PATH@ M=$(PWD) modules

clean-generic:
	@echo "drv_dsl_cpe_api: Cleanup Linux 2.6.x kernel object build"
	@- find . -name ".*.cmd" | xargs rm -f
	@- find . -name "*.o" | xargs rm -f
	@- rm -f Module.symvers Kbuild
	@- rm -rf .tmp_versions *.mod.c *.order

endif

lint:
		@flint -vm \
		-i$(shell dirname `$(CC) -print-file-name=include`)/include \
		-i@top_srcdir@ std_lx.lnt env-cw6.lnt $(AM_CPPFLAGS) -I$(PWD)/include\
		$(AM_CFLAGS) -DDSL_INTERN  -DHAVE_CONFIG_H -D__KERNEL__ -D__LINUX__ -DMODULE $(drv_dsl_cpe_api_SOURCES)
