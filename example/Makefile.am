bin_PROGRAMS = shell shelltest
shell_SOURCES = shell.c
AM_CPPFLAGS = $(DEPS_CFLAGS) -g
shell_LDADD = $(DEPS_LIBS) -lpthread ../src/libnetvirt-@LIBNETVIRT_API_VERSION@.la

shelltest_SOURCES = shelltest.c
shelltest_LDADD = $(DEPS_LIBS) -lpthread ../src/libnetvirt-@LIBNETVIRT_API_VERSION@.la

