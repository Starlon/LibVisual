## Process this file with automake to generate a Makefile.in

lib_LTLIBRARIES = $(AVS_SUPERSCOPE)

EXTRA_LTLIBRARIES = actor_avs_superscope.la

libdir = @LIBVISUAL_PLUGINS_BASE_DIR@/actor

LIBS += -L. -L$(prefix)/lib @LIBVISUAL_LIBS@

AM_CFLAGS = @LIBVISUAL_CFLAGS@ -DXP_UNIX

INCLUDES = $(all_includes) \
	-I$(top_srcdir) \
	-I$(top_srcdir)/../libvisual-avs/common/ \
	-Ijs/src/

actor_avs_superscope_la_LDFLAGS = -rpath $(libdir) -module -avoid-version 

actor_avs_superscope_la_SOURCES = actor_avs_superscope.c

actor_avs_superscope_la_LIBADD = libjs.a

