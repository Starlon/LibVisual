## Process this file with automake to generate a Makefile.in

actor_plugin_LTLIBRARIES = actor_superscope.la

actor_superscope_la_LIBADD = $(LIBVISUAL_LIBS)

AM_CFLAGS = $(LIBVISUAL_CFLAGS)

INCLUDES = $(all_includes) -I$(top_srcdir) 

actor_superscope_la_LDFLAGS = -module -avoid-version 

actor_superscope_la_SOURCES = actor_superscope.c \
			  actor_superscope.h \
			  bump_scope.c \
			  bump_scope.h

