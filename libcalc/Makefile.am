## Process this file with automake to generate a Makefile.in

lib_LTLIBRARIES = libcalc.la

#EXTRA_LTLIBRARIES = libcalc.la

libdir = @LIBVISUAL_PLUGINS_BASE_DIR@

LIBS += -L. -L$(prefix)/lib $(GLIB_LIBS) @LIBVISUAL_LIBS@ -L$(prefix)/libcalc/.libs

AM_CFLAGS = $(GLIB_CFLAGS) @LIBVISUAL_CFLAGS@

INCLUDES = $(all_includes) -I$(top_srcdir) -I$(top_srcdir)/common

libcalc_la_LDFLAGS = -rpath $(libdir) -module -avoid-version -laudcore

libcalc_la_SOURCES = dict.c \
		       dict.h \
		       execute.c \
		       execute.h \
		       function.c \
		       function.h \
		       parser.c \
                       parser.h \
		       storage.c \
		       storage.h

