AC_PREREQ(2.59)
AC_INIT(ltspfs2, [0.1], [sbalneav@ltsp.org])
AC_CONFIG_SRCDIR([src/server.c])
AM_INIT_AUTOMAKE

AM_CONFIG_HEADER([config.h])
AM_MAINTAINER_MODE

dnl Check for compiler.
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_INSTALL

dnl Check for libraries.
PKG_CHECK_MODULES(LTSPFS2, [fuse, x11])

dnl Checks for header files.
AC_HEADER_STDC
AC_HEADER_DIRENT

dnl Output
AC_CONFIG_FILES(Makefile src/Makefile)
AC_OUTPUT
