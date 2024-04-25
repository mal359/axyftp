AC_INIT([AxY FTP],[0.5.1])
AC_CONFIG_SRCDIR([src/axyftp.c])
AC_CONFIG_AUX_DIR([.])
AC_CONFIG_MACRO_DIR([macro])
AM_INIT_AUTOMAKE

AC_CONFIG_FILES([Makefile])
AC_CONFIG_HEADERS([config.h])

AC_DEFINE([XTHREADS], [], [Check for Xthreads])

AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_MAKE_SET

AC_LANG([C])
AC_COMPILE_IFELSE([
  AC_LANG_PROGRAM([[
  ]], [[
    int main() {
      return 0;
    }
  ]])
], [], [
  AC_MSG_ERROR([No ANSI C - compatible compiler found.])
])

if test -z "$axyftp_gui" || test "x$axyftp_gui" = xmotif ; then
  AC_PATH_XTRA
  AXY_PATH_MOTIF
  AXY_PATH_XPM

  AC_CACHE_CHECK([for X, Motif, and XPM], [axyftp_can_do_motif],
    [ax_cv_motif=
     ax_cv_xpm=
     ax_cv_x11=

     if test x$no_x != xyes; then
       ax_cv_x11=yes
     fi

     if test x$no_motif != xyes; then
       ax_cv_motif=yes
     fi

     if test x$no_xpm != xyes; then
       ax_cv_xpm=yes
     fi

     if test "x$ax_cv_x11" = "xyes" && test "x$ax_cv_motif" = "xyes" && test "x$ax_cv_xpm" = "xyes"; then
       axyftp_can_do_motif=yes
     else
       axyftp_can_do_motif=no
     fi
    ])
    
    if test "x$axyftp_can_do_motif" != "xyes"; then
    AC_MSG_ERROR([Required X libraries and Motif are not available])
  fi
fi

AC_OUTPUT
