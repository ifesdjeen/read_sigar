pkglib_LTLIBRARIES = read_sigar.la
read_sigar_la_SOURCES = read_sigar.c
read_sigar_la_LDFLAGS = -module -avoid-version -shared -llibsigar
read_sigar_la_CFLAGS = -I${abs_top_builddir}/collectd/src -I#{abs_top_builddir}/sigar/include -Wall -g -O3
