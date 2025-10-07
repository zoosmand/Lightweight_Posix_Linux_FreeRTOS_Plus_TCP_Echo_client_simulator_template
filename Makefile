# ------------------------------------------------------------------- #
# ------------------------------------------------------------------- #
# ------------------------------------------------------------------- #
CC := gcc
TARGET := all
# debug flag
DEBUG = 1
# optimization
OPT := -O1

BUILD_DIR := build

# ------------------------------------------------------------------- #
CORE_DIR_REL := ./Core
CORE_DIR := $(abspath $(CORE_DIR_REL))

FREERTOS_DIR_REL := ./FreeRTOS
FREERTOS_DIR := $(abspath $(FREERTOS_DIR_REL))

FREERTOS_PLUS_DIR_REL := ./FreeRTOS-Plus
FREERTOS_PLUS_DIR := $(abspath $(FREERTOS_PLUS_DIR_REL))

# ------------------------------------------------------------------- #
INCLUDE_DIRS += -I${CORE_DIR}/Inc
INCLUDE_DIRS += -I${FREERTOS_DIR}/Source/include
INCLUDE_DIRS += -I${FREERTOS_DIR}/Source/portable/ThirdParty/GCC/Posix
INCLUDE_DIRS += -I${FREERTOS_DIR}/Source/portable/ThirdParty/GCC/Posix/utils
INCLUDE_DIRS += -I${FREERTOS_PLUS_DIR}/Source/FreeRTOS-Plus-Trace/Include
INCLUDE_DIRS += -I${FREERTOS_PLUS_DIR}/Source/FreeRTOS-Plus-TCP/portable/NetworkInterface/linux/
INCLUDE_DIRS += -I${FREERTOS_PLUS_DIR}/Source/FreeRTOS-Plus-TCP/include/
INCLUDE_DIRS += -I${FREERTOS_PLUS_DIR}/Source/FreeRTOS-Plus-TCP/portable/Compiler/GCC/


# ------------------------------------------------------------------- #
SOURCE_FILES := $(wildcard ${CORE_DIR}/Src/*.c)
SOURCE_FILES += $(wildcard ${FREERTOS_DIR}/Source/*.c)
# Memory manager (use malloc() / free() )
SOURCE_FILES += ${FREERTOS_DIR}/Source/portable/MemMang/heap_3.c

# Posix port -------------------------------------------------------- #
SOURCE_FILES += ${FREERTOS_DIR}/Source/portable/ThirdParty/GCC/Posix/utils/wait_for_event.c
SOURCE_FILES += ${FREERTOS_DIR}/Source/portable/ThirdParty/GCC/Posix/port.c

# FreeRTOS TCP ------------------------------------------------------- #
SOURCE_FILES += ${FREERTOS_PLUS_DIR}/Source/FreeRTOS-Plus-TCP/FreeRTOS_DNS.c
SOURCE_FILES += ${FREERTOS_PLUS_DIR}/Source/FreeRTOS-Plus-TCP/FreeRTOS_DHCP.c
SOURCE_FILES += ${FREERTOS_PLUS_DIR}/Source/FreeRTOS-Plus-TCP/FreeRTOS_ARP.c
SOURCE_FILES += ${FREERTOS_PLUS_DIR}/Source/FreeRTOS-Plus-TCP/FreeRTOS_TCP_WIN.c
SOURCE_FILES += ${FREERTOS_PLUS_DIR}/Source/FreeRTOS-Plus-TCP/FreeRTOS_Stream_Buffer.c
SOURCE_FILES += ${FREERTOS_PLUS_DIR}/Source/FreeRTOS-Plus-TCP/portable/BufferManagement/BufferAllocation_2.c
SOURCE_FILES += ${FREERTOS_PLUS_DIR}/Source/FreeRTOS-Plus-TCP/FreeRTOS_IP.c
SOURCE_FILES += ${FREERTOS_PLUS_DIR}/Source/FreeRTOS-Plus-TCP/FreeRTOS_TCP_IP.c
SOURCE_FILES += ${FREERTOS_PLUS_DIR}/Source/FreeRTOS-Plus-TCP/FreeRTOS_UDP_IP.c
SOURCE_FILES += ${FREERTOS_PLUS_DIR}/Source/FreeRTOS-Plus-TCP/FreeRTOS_Sockets.c
SOURCE_FILES += ${FREERTOS_PLUS_DIR}/Source/FreeRTOS-Plus-TCP/portable/NetworkInterface/linux/NetworkInterface.c

# Trace library ----------------------------------------------------- #
SOURCE_FILES += ${FREERTOS_PLUS_DIR}/Source/FreeRTOS-Plus-Trace/trcKernelPort.c
SOURCE_FILES += ${FREERTOS_PLUS_DIR}/Source/FreeRTOS-Plus-Trace/trcSnapshotRecorder.c
SOURCE_FILES += ${FREERTOS_PLUS_DIR}/Source/FreeRTOS-Plus-Trace/trcStreamingRecorder.c
SOURCE_FILES += ${FREERTOS_PLUS_DIR}/Source/FreeRTOS-Plus-Trace/streamports/File/trcStreamingPort.c


# ------------------------------------------------------------------- #
C_DEFS += -DprojCOVERAGE_TEST=0
C_DEFS += -D_WINDOWS_
C_DEFS += -DDEBUG -D_BSD_SOURCE

CFLAGS := -ggdb3 ${OPT} $(C_DEFS)

ifeq ($(DEBUG), 1)
	CFLAGS += -g -gdwarf-2
endif

LDFLAGS := -ggdb3 $(OPT) -pthread -lpcap

OBJ_FILES = $(SOURCE_FILES:%.c=$(BUILD_DIR)/%.o)

DEP_FILE = $(OBJ_FILES:%.o=%.d)


# ------------------------------------------------------------------- #
# ------------------------------------------------------------------- #
# ------------------------------------------------------------------- #
${TARGET} : $(BUILD_DIR)/$(TARGET)

${BUILD_DIR}/${TARGET} : ${OBJ_FILES}
	mkdir -p ${@D}
	$(CC) $^ $(CFLAGS) $(INCLUDE_DIRS) ${LDFLAGS} -o $@

-include ${DEP_FILE}

${BUILD_DIR}/%.o : %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) ${INCLUDE_DIRS} -MMD -c $< -o $@


# ------------------------------------------------------------------- #
# ------------------------------------------------------------------- #
# ------------------------------------------------------------------- #
.PHONY: clean


# ------------------------------------------------------------------- #
# ------------------------------------------------------------------- #
# ------------------------------------------------------------------- #
clean:
	rm -rf $(BUILD_DIR)

