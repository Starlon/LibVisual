dnl Borrowed from Audacious music player
dnl ADD_PKG_REQUIRES([requirement])

AC_DEFUN([ADD_PC_REQUIRES],
[
   test -z "$PC_REQUIRES" && PC_REQUIRES="$1" || PC_REQUIRES="$PC_REQUIRES, $1"
   AC_SUBST(PC_REQUIRES)
]
)

