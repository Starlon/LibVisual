dnl Process this file with autoconf to produce a configure script.
AC_PREREQ(2.57)

dnl Version numbers
m4_define([libvisual_version_major], [0])
m4_define([libvisual_version_minor], [5])
m4_define([libvisual_version_micro], [0])

m4_define([libvisual_version], [libvisual_version_major.libvisual_version_minor.libvisual_version_micro])

AC_INIT([Libvisual Library], [libvisual_version], [synap@nerds-incorporated.org], [libvisual])
AM_INIT_AUTOMAKE([1.7.0 dist-bzip2])

dnl TODO: Add these into lv_config.h (or maybe lv_version.h)
LIBVISUAL_VERSION_MAJOR=libvisual_version_major
LIBVISUAL_VERSION_MINOR=libvisual_version_minor
LIBVISUAL_VERSION_MICRO=libvisual_version_micro
LIBVISUAL_VERSION=libvisual_version

LIBVISUAL_VERSION_SUFFIX="$LIBVISUAL_VERSION_MAJOR.$LIBVISUAL_VERSION_MINOR"
AC_SUBST(LIBVISUAL_VERSION_SUFFIX)

AC_CONFIG_HEADER(config.h)

AC_CANONICAL_HOST

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

AC_DISABLE_STATIC
AC_LIBTOOL_WIN32_DLL
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL

AC_PATH_PROG(PKG_CONFIG, [pkg-config], [no])
if test x$PKG_CONFIG = xno ; then
  AC_MSG_WARN([*** pkg-config not found. See http://www.freedesktop.org/software/pkgconfig/.
	       *** You will need pkg-config to compile against Libvisual Library])
fi
if $PKG_CONFIG --atleast-pkgconfig-version 0.14 ; then
  :
else
  AC_MSG_WARN([*** pkg-config too old; version 0.14 or better recommended.])
fi

# Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h sys/types.h stdlib.h string.h sys/time.h unistd.h sched.h sys/sched.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_HEADER_TIME

AC_MSG_CHECKING(for ISO C99 varargs macros in C)
AC_TRY_COMPILE([],[
		int a(int p1, int p2, int p3);
		#define call_a(...) a(1,__VA_ARGS__)
		call_a(2,3);
		],lv_have_iso_c_varargs=yes,lv_have_iso_c_varargs=no)
AC_MSG_RESULT($lv_have_iso_c_varargs)

AC_MSG_CHECKING(for GNUC varargs macros)
AC_TRY_COMPILE([],[
		int a(int p1, int p2, int p3);
		#define call_a(params...) a(1,params)
		call_a(2,3);
		],lv_have_gnuc_varargs=yes,lv_have_gnuc_varargs=no)
AC_MSG_RESULT($lv_have_gnuc_varargs)

AC_C_BIGENDIAN(bigendian=yes, bigendian=no, )

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_SELECT_ARGTYPES
AC_CHECK_FUNCS([gettimeofday usleep nanosleep select memset sqrt strdup strndup sysconf])

AC_CHECK_LIB([dl], [dlsym], [LIBS="-ldl $LIBS"],
	     AC_MSG_WARN([*** You need dlsym() to make use of Libvisual.]))
AC_CHECK_LIB([m], [sqrt], [LIBS="-lm $LIBS"],
	     AC_MSG_ERROR([*** You need the Standard C Math Library to build Libvisual.]))

# Internationalization
GETTEXT_PACKAGE="libvisual-${LIBVISUAL_VERSION_SUFFIX}"
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE", [GETTEXT package name])
AC_DEFINE_UNQUOTED(LOCALEDIR, "${prefix}/share/locale", [String catalog location])
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.14.1])

dnl ******************************
dnl Debugging stuff 
dnl ******************************
DEBUG_CFLAGS=""
AC_ARG_ENABLE([profiling],
  AC_HELP_STRING([--enable-profiling],
		[Enable emision of profile data to be used by gprof @<:@default=disabled@:>@]),
		[profiling=$enableval],
		[profiling=no])
AC_MSG_CHECKING([whether to enable profiling])
if test x$profiling = xyes; then
  AC_MSG_RESULT([yes])
  if test x"$GCC" = xyes; then
  DEBUG_CFLAGS="$DEBUG_CFLAGS -pg"
else
    AC_MSG_WARN([Profiling only works when using a gcc compiler.])
  fi
else
  AC_MSG_RESULT([no])
fi

AC_ARG_ENABLE([debug],
            AC_HELP_STRING([--enable-debug],
                           [Enable debug @<:@default=disabled@:>@]),
	    [debug=$enableval],
	    [debug=no])
AC_MSG_CHECKING([whether to enable debug])
if test x$debug = xyes; then
  AC_MSG_RESULT([yes])
  if test x"$GCC" = xyes; then
    DEBUG_CFLAGS="$DEBUG_CFLAGS -ggdb3 -Wall"
    DEBUG_CFLAGS="$DEBUG_CFLAGS -Wmissing-braces -Wimplicit -Wunused -Wmissing-prototypes"
  # IMHO -Werror should be here also. --VVB
    lv_save_CFLAGS=$CFLAGS
    CFLAGS="$CFLAGS -Wno-unused-variable"
    AC_MSG_CHECKING([whether gcc version is greater than 2.95])
    AC_TRY_COMPILE([#include <stdlib.h>],
		   [exit(0);],
		   [lv_gcc_gt_2_95=yes],
		   [lv_gcc_gt_2_95=no])
    CFLAGS=$lv_save_CFLAGS
    if test x$lv_gcc_gt_2_95 = xyes ; then
      AC_MSG_RESULT([yes])
      DEBUG_CFLAGS="$DEBUG_CFLAGS -Wno-unused-variable"
    else
      AC_MSG_RESULT([no])
    fi
  fi
else
  AC_MSG_RESULT([no])
fi
AC_SUBST(DEBUG_CFLAGS)

AC_ARG_ENABLE([extra-optimization],
           AC_HELP_STRING([--enable-extra-optimization],
                          [Enable extra optimizations @<:@default=disabled@:>@]),
           [extra_opt=$enableval],
           [extra_opt=no])
AC_MSG_CHECKING([whether to enable extra optimizations])
if test x$extra_opt = xyes; then
  AC_MSG_RESULT([yes])
  if test x"$GCC" = xyes; then
  OPT_CFLAGS="-O3 -fexpensive-optimizations"
else
    AC_MSG_WARN([Extra optimizations only works with a gcc compiler.])
  fi
else
  AC_MSG_RESULT([no])
  if test x$debug = xyes && test x"$GCC" = xyes; then
    OPT_CFLAGS="-O0"
  else
    OPT_CFLAGS=""
  fi
fi

AC_SUBST(CFLAGS, "${CFLAGS} ${DEBUG_CFLAGS} ${OPT_CFLAGS}")

libs_dynamic_loader="-ldl"
AC_SUBST(LIBS_DYNAMIC_LOADER, "$libs_dynamic_loader")

dnl Installation paths
LIBVISUAL_INCLUDE_DIR="${includedir}/libvisual-${LIBVISUAL_VERSION_SUFFIX}"
LIBVISUAL_PLUGINS_BASE_DIR="${libdir}/libvisual-${LIBVISUAL_VERSION_SUFFIX}"
AC_SUBST(LIBVISUAL_INCLUDE_DIR)
AC_SUBST(LIBVISUAL_PLUGINS_BASE_DIR)

dnl Output the LibVisual config file
				    
AC_CONFIG_FILES([
m4/Makefile
Makefile
src/Makefile
])

AC_OUTPUT

echo "
=====================================================================
LIBVISUAL, AN AUDIO VISUALISATION ABSTRACTION LIBRARY, VERSION $VERSION
=====================================================================

install path                         : ${prefix}/lib
plugins base directory               : $plugins_base_dir
compiler                             : ${CC}
debug enabled                        : ${debug}
"
