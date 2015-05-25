## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = foreign 1.9 nostdinc

SUBDIRS = src

DISTCHECK_CONFIGURE_FLAGS=@CONFIGURE_OPTIONS@

drv_dsl_cpe_api_docdir = ${prefix}/doc/drv_dsl_cpe_api

drv_dsl_cpe_api_doc_DATA = \
    ChangeLog

drv_dsl_cpe_api_doc_DEVICE = \
    doc/doxyconfig.danube \
    doc/doxyconfig.vrx

EXTRA_DIST = $(drv_dsl_cpe_api_doc_DATA) \
    $(drv_dsl_cpe_api_doc_DEVICE) \
    build_vxworks/drv_dsl_cpe_api.wpj \
    README \
    AUTHORS \
    NEWS \
    TODO \
    doc/footer.html \
    doc/header.html \
    doc/dsl_cpe_api.h \
    doc/api_sw_structure_overview.jpg \
    doc/html/logo.gif \
    doc/html/spacer.gif \
    doc/html/stylesheet.css


# Copy all the spec files. Of cource, only one is actually used.
dist-hook:
	for specfile in *.spec; do \
		if test -f $$specfile; then \
			cp -p $$specfile $(distdir); \
		fi \
	done

# Check for correct line ends
distcheck-hook:
	chmod a+w $(distdir)
	echo "Checking line ends ...!!!"; \
	find $(distdir) -type f -exec file {} \; | grep -e "CRLF" -e "Non-ISO" && exit 1; \
	echo "Checking line ends ... done."

generate-doc-danube:
	( cd @srcdir@/doc;  \
	doxygen doxyconfig.danube; )


generate-xml:
	( cd @srcdir@/doc/xml;  \
	echo generate complete.xml ...; \
	xsltproc --timing -o complete.xml combine.xslt index.xml; \
	echo generate API reference ...; \
	xsltproc doxy2wg.xsl complete.xml | sed 's/^ *//g' > @srcdir@/doc/api/fm/lib_dsl_api_reference.xml; \
    )

lint:
	@target=lint; \
	list='$(SUBDIRS)'; \
	for subdir in $$list; do \
		echo "Making $$target in $$subdir"; \
		if test "$$subdir" = "."; then \
		dot_seen=yes; \
		local_target="$$target-am"; \
		else \
		local_target="$$target"; \
		fi; \
		(cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) $$local_target) \
		|| eval $$failcom; \
	done;

