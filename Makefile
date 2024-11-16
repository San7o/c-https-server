PROJECT_DIR    = ${PWD}
LIB_DIR        = ${PROJECT_DIR}/lib
BUILD_DIR      = ${PROJECT_DIR}/build
SOURCE_DIR     = ${PROJECT_DIR}/src
TESTS_DIR      = ${PROJECT_DIR}/test

CC             = gcc
COMPILE_FLAGS  = -Wall -Wextra -Werror -Wno-unused-function
INCLUDE_FLAGS  = -I ${PROJECT_DIR}/include
CCFLAGS        = ${COMPILE_FLAGS} ${INCLUDE_FLAGS}

.PHONY: default builddir clean distclean

## SHARED LIBRARY

default: ${BUILD_DIR}/chttpsd

$(BUILD_DIR)/libchttps.so: builddir $(BUILD_DIR)/server.o ${BUILD_DIR}/logger.o $(BUILD_DIR)/util.o ${BUILD_DIR}/signals.o ${BUILD_DIR}/connections.o ${BUILD_DIR}/parser.o
	${CC} $(BUILD_DIR)/server.o $(BUILD_DIR)/util.o ${BUILD_DIR}/logger.o ${BUILD_DIR}/signals.o ${BUILD_DIR}/connections.o ${BUILD_DIR}/parser.o -o ${BUILD_DIR}/libchttps.so $(CCFLAGS) -shared -fPIC

builddir:
	mkdir ${BUILD_DIR} 2>/dev/null || \:

$(BUILD_DIR)/server.o: $(LIB_DIR)/server.c
	${CC} -c ${LIB_DIR}/server.c -o ${BUILD_DIR}/server.o ${CCFLAGS}
$(BUILD_DIR)/util.o: $(LIB_DIR)/util.c
	${CC} -c ${LIB_DIR}/util.c -o ${BUILD_DIR}/util.o ${CCFLAGS}
$(BUILD_DIR)/logger.o: ${LIB_DIR}/logger.c
	${CC} -c ${LIB_DIR}/logger.c -o ${BUILD_DIR}/logger.o ${CCFLAGS}
$(BUILD_DIR)/signals.o: ${LIB_DIR}/signals.c
	${CC} -c ${LIB_DIR}/signals.c -o ${BUILD_DIR}/signals.o ${CCFLAGS}
$(BUILD_DIR)/connections.o: ${LIB_DIR}/connections.c
	${CC} -c ${LIB_DIR}/connections.c -o ${BUILD_DIR}/connections.o ${CCFLAGS}
$(BUILD_DIR)/parser.o: ${LIB_DIR}/parser.c
	${CC} -c ${LIB_DIR}/parser.c -o ${BUILD_DIR}/parser.o ${CCFLAGS}

## DAEMON

$(BUILD_DIR)/chttpsd: builddir ${BUILD_DIR}/libchttps.so $(BUILD_DIR)/main.o
	$(CC) $(BUILD_DIR)/main.o -o $(BUILD_DIR)/chttpsd $(CCFLAGS) ${BUILD_DIR}/libchttps.so

$(BUILD_DIR)/main.o: $(SOURCE_DIR)/main.c
	${CC} -c $(SOURCE_DIR)/main.c -o $(BUILD_DIR)/main.o $(CCFLAGS) ${BUILD_DIR}/libchttps.so

## TESTS

check: socket_connection_test connection_add_remove_test parse_test
socket_connection_test: ${BUILD_DIR}/libchttps.so ${TESTS_DIR}/socket_connection.c
	${CC} ${BUILD_DIR}/libchttps.so ${TESTS_DIR}/socket_connection.c -o ${BUILD_DIR}/socket_connection ${CCFLAGS} -pthread
	${BUILD_DIR}/socket_connection || echo "Test socket_connection Failed!!"
connection_add_remove_test: ${BUILD_DIR}/libchttps.so  ${TESTS_DIR}/connection_add_remove.c
	${CC} ${BUILD_DIR}/libchttps.so ${TESTS_DIR}/connection_add_remove.c -o ${BUILD_DIR}/connection_add_remove ${CCFLAGS} -pthread
	${BUILD_DIR}/connection_add_remove || echo "Test connection_add_remove Failed!!"
parse_test: ${BUILD_DIR}/libchttps.so  ${TESTS_DIR}/parse.c
	${CC} ${BUILD_DIR}/libchttps.so ${TESTS_DIR}/parse.c -o ${BUILD_DIR}/parse ${CCFLAGS} -pthread
	${BUILD_DIR}/parse || echo "Test parse Failed!!"

## UTILITIES

clean:
	rm $(BUILD_DIR)/*.o
distclean:
	rm -rf ${BUILD_DIR}
