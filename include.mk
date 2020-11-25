ifeq ($(RELEASE),1)
CONFIGURATION	:= release
RELEASE_OPT		?= 3
OPTIMIZATION	:= -O$(RELEASE_OPT) -g
else
CONFIGURATION	:= debug
OPTIMIZATION	:= -Og -g
DEFS			+= DEBUG
endif

ROOTDIR		:= $(dir $(firstword $(MAKEFILE_LIST)))
BASEDIR		:= $(dir $(lastword $(MAKEFILE_LIST)))
MXDIR		:= $(ROOTDIR)$(TARGET).cubemx/
OUTPUT_DIR	:= $(ROOTDIR)build/$(CONFIGURATION)/
OBJDIR		:= $(OUTPUT_DIR)/obj

INCDIRS		+= \
$(BASEDIR)inc \
$(BASEDIR)inc/bus-node-base \
$(BASEDIR)inc/bus-node-base/internal \
$(BASEDIR)chip \

SOURCES		+= \
$(BASEDIR)src/abi.cpp \
$(BASEDIR)src/build_information.cpp \
$(BASEDIR)src/fault_handler.c \
$(BASEDIR)src/hash.cpp \
$(BASEDIR)src/node.cpp \
$(BASEDIR)src/std.cpp \
$(BASEDIR)src/unique_id.cpp

UAVCAN_STM32_NUM_IFACES	?= 1
UAVCAN_MEMPOOL_SIZE ?= 16384
UAVCAN_RX_QUEUE_SIZE ?= 128
TRACEALYZER_SUPPORT ?= 1

ifndef UAVCAN_STM32_TIMER_NUMBER
$(error UAVCAN_STM32_TIMER_NUMBER muste be set)
endif

DEFS		+= STM32_PCLK1=$(STM32_PCLK1) STM32_TIMCLK1=$(STM32_TIMCLK1)

# libuavcan
DEFS		+= UAVCAN_CPP_VERSION=UAVCAN_CPP11
DEFS		+= UAVCAN_STM32_NUM_IFACES=$(UAVCAN_STM32_NUM_IFACES)
DEFS		+= UAVCAN_STM32_TIMER_NUMBER=$(UAVCAN_STM32_TIMER_NUMBER)
DEFS		+= UAVCAN_NO_ASSERTION

# bus_node_base starting up uavcan
DEFS		+= UAVCAN_MEMPOOL_SIZE=$(UAVCAN_MEMPOOL_SIZE)
DEFS		+= UAVCAN_RX_QUEUE_SIZE=$(UAVCAN_RX_QUEUE_SIZE)


ifeq ($(RTOS),none)
DEFS		+= UAVCAN_STM32_BAREMETAL=1 FW_USE_RTOS=0 FW_TYPE_BAREMETAL
else ifeq ($(RTOS),freertos)
DEFS		+= UAVCAN_STM32_FREERTOS=1 FW_USE_RTOS=1 FW_TYPE_FREERTOS
else
$(error Unknown firmware type)
endif

include $(BASEDIR)libuavcan/libuavcan/include.mk
SOURCES += $(LIBUAVCAN_SRC)
INCDIRS += $(LIBUAVCAN_INC)

include $(BASEDIR)uavcan_platform_specific_components/stm32/libuavcan/driver/include.mk
SOURCES += $(LIBUAVCAN_STM32_SRC)
INCDIRS += $(LIBUAVCAN_STM32_INC)

$(info $(shell $(LIBUAVCAN_DSDLC) $(BASEDIR)/libuavcan/dsdl/uavcan))
$(info $(shell $(LIBUAVCAN_DSDLC) $(BASEDIR)/uavcan_datatypes/ottocar))
INCDIRS += dsdlc_generated

# CubeMX
include $(TARGET).cubemx/Makefile

SOURCES += $(foreach source,$(C_SOURCES) $(ASM_SOURCES),$(MXDIR)$(source))
INCDIRS += $(C_INCLUDES:-I%=$(MXDIR)%)
INCDIRS += $(AS_INCLUDES:-I%=$(MXDIR)%)

LDSCRIPT := $(MXDIR)$(LDSCRIPT)


include $(BASEDIR)mk/build-info.mk

# Include actual rules
include $(BASEDIR)mk/rules.mk
