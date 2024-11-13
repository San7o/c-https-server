PROJECT_DIR   = ${PWD}
LIB_DIR       = ${PROJECT_DIR}/lib
BUILD_DIR     = ${PROJECT_DIR}/build
SOURCE_DIR    = ${PROJECT_DIR}/src

CC            = gcc
COMPILE_FLAGS = -Wall -Wextra -Werror
INCLUDE_FLAGS = -I ${PROJECT_DIR}/include
CCFLAGS       = ${COMPILE_FLAGS} ${INCLUDE_FLAGS}

.PHONY: default builddir clean distclean

## SHARED LIBRARY

default: ${BUILD_DIR}/chttpsd

$(BUILD_DIR)/libchttps.so: builddir $(BUILD_DIR)/server.o
	${CC} $(BUILD_DIR)/server.o -o ${BUILD_DIR}/libchttps.so $(CCFLAGS) -shared -fPIC

builddir:
	mkdir ${BUILD_DIR} 2>/dev/null || echo ""

$(BUILD_DIR)/server.o: $(LIB_DIR)/server.c
	${CC} -c ${LIB_DIR}/server.c -o ${BUILD_DIR}/server.o ${CCFLAGS}

## DAEMON

$(BUILD_DIR)/chttpsd: builddir ${BUILD_DIR}/libchttps.so $(BUILD_DIR)/main.o
	$(CC) $(BUILD_DIR)/main.o -o $(BUILD_DIR)/chttpsd $(CCFLAGS) ${BUILD_DIR}/libchttps.so

$(BUILD_DIR)/main.o: $(SOURCE_DIR)/main.c
	${CC} -c $(SOURCE_DIR)/main.c -o $(BUILD_DIR)/main.o $(CCFLAGS) ${BUILD_DIR}/libchttps.so

## UTILITIES

clean:
	rm $(BUILD_DIR)/*.o

distclean:
	rm -rf ${BUILD_DIR}
