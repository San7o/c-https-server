AC_INIT([chttps], [0.0.1], [santigio2003@gmail.com], [],
	[README.org])
AM_INIT_AUTOMAKE([foreign subdir-objects])

# Check of Openssl
PKG_CHECK_MODULES([OPENSSL],[openssl >= 1.1],[], [
  AC_MSG_ERROR([openssl library not found or too old])
])

AC_PROG_CC
AC_CONFIG_MACRO_DIRS([m4])
LT_INIT
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
