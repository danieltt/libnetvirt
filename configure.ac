AC_INIT([libnetvirt], [0.1], [danieltt@kth.se])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_MACRO_DIR([m4])
PKG_CHECK_MODULES([DEPS], [libxml-2.0 >= 2])
AC_PROG_CC
LT_INIT([disable-static])
CFLAGS="$CFLAGS -g `python-config --cflags`"
LDFLAGS="$LDFLAGS `pkg-config --libs python` -lxml2"
AC_CONFIG_HEADERS([config.h])
AC_SUBST([LIBNETVIRT_SO_VERSION], [1:2:0])
AC_SUBST([LIBNETVIRT_API_VERSION], [0.1])
SWIG_ENABLE_CXX
SWIG_PYTHON

AC_CONFIG_FILES([
 Makefile
 src/Makefile
 example/Makefile
])
AC_OUTPUT
