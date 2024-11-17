PROJECT_DIR    = ${top_srcdir}
LIB_DIR        = ${PROJECT_DIR}/libchttps
BUILD_DIR      = ${PROJECT_DIR}/build
DAEMON_DIR     = ${PROJECT_DIR}/daemon
TESTS_DIR      = ${PROJECT_DIR}/test

CC             = @CC@
AM_CFLAGS      = @CFLAGS@ -Wall -Wextra -Werror  \
                  -Wno-unused-function           \
		  -Wno-unused-parameter          \
                  -Wno-unused-variable           \
		  -Wno-int-to-pointer-cast       \
                  -Wno-format -pthread           \
                  @OPENSSL_CFLAGS@
LDADD          = @OPENSSL_LIBS@
PREFIX         = @prefix@
ACLOCAL_AMFLAGS = -I m4 

# =================== LIBRARY ================== #

lib_LTLIBRARIES      = libchttps.la
libchttps_la_SOURCES = ${LIB_DIR}/connections.c  \
                       ${LIB_DIR}/logger.c       \
                       ${LIB_DIR}/parser.c       \
                       ${LIB_DIR}/route.c        \
                       ${LIB_DIR}/router.c       \
                       ${LIB_DIR}/server.c       \
                       ${LIB_DIR}/signals.c      \
                       ${LIB_DIR}/ssl.c          \
                       ${LIB_DIR}/util.c
libchttps_la_CFLAGS = -I${PROJECT_DIR}/include   \
                      -lssl -lcrypto

# =================== DAEMON =================== #

bin_PROGRAMS      = chttpsd test_server
chttpsd_SOURCES   = ${DAEMON_DIR}/args.c         \
                    ${DAEMON_DIR}/main.c
chttpsd_CFLAGS    = -I${PROJECT_DIR}/include     \
                    -lssl -lcrypto
chttpsd_LDADD     = libchttps.la

# A test server to play with
test_server_SOURCES    = ${TESTS_DIR}/test_server.c
test_server_CFLAGS   = -I ${PROJECT_DIR}/include \
                       -lssl -lcrypto
test_server_LDADD      = libchttps.la

# =================== TESTS ==================== #

check_PROGRAMS    = test_conenction_add_remove   \
                    test_parse                   \
                    test_router                  \
                    test_server_router           \
                    test_socket_connection

test_conenction_add_remove_SOURCES = ${TESTS_DIR}/connection_add_remove.c
test_parse_SOURCES                 = ${TESTS_DIR}/parse.c
test_router_SOURCES                = ${TESTS_DIR}/router_test.c
test_server_router_SOURCES         = ${TESTS_DIR}/server_router_test.c
test_socket_connection_SOURCES     = ${TESTS_DIR}/socket_connection.c

test_conenction_add_remove_CFLAGS  = -I ${PROJECT_DIR}/include \
                                     -lssl -lcrypto
test_parse_CFLAGS                  = -I ${PROJECT_DIR}/include \
                                     -lssl -lcrypto
test_router_CFLAGS                 = -I ${PROJECT_DIR}/include \
                                     -lssl -lcrypto
test_server_router_CFLAGS          = -I ${PROJECT_DIR}/include \
                                     -lssl -lcrypto
test_socket_connection_CFLAGS      = -I ${PROJECT_DIR}/include \
                                     -lssl -lcrypto

test_conenction_add_remove_LDADD   = libchttps.la
test_parse_LDADD                   = libchttps.la
test_router_LDADD                  = libchttps.la
test_server_router_LDADD           = libchttps.la
test_socket_connection_LDADD       = libchttps.la

check: ${check_PROGRAMS}
	@for test in ${check_PROGRAMS}; do \
	    ./$$test \
                && echo -e "\033[32m Test" $$test "Passed \033[0m" \
                || echo -e "\033[31m Test" $$test "Failed\033[0m"; \
	done

clean-local:
	@rm config.status configure config.log
	@rm Makefile
	@rm -r autom4te.cache/
	@rm aclocal.m4
	@rm compile install-sh missing Makefile.in
