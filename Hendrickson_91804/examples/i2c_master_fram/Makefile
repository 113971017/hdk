#
# Copyright (C) 2016-2021 Melexis N.V.
#
# Software Platform
#

# Application name (same as a folder name)
TARGET := $(notdir $(CURDIR))

PROFILE = DEFAULT

#LOADER = APP

# Protection key is disabled if it includes all zeros or all 0xFFs
PROTECTIONKEY64bit = 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00

PLTF_DIR = ../../platform_amalthea

include $(PLTF_DIR)/config/Config.mk

#
# SOURCE FILES LIST
#

SRCS = main.c
SRCS += fram.c
SRCS += i2c_sw_master.S

OBJS  = $(patsubst %.S, $(OBJDIR)/%.o, $(filter %.S, $(SRCS)))
OBJS += $(patsubst %.c, $(OBJDIR)/%.o, $(filter %.c, $(SRCS)))

# Enable debug
#CPPFLAGS += -DDEBUG_ENABLED

# Disable optimization to simplify debugging
#CPPFLAGS += -DINLINE=inline
#CFLAGS += -O0

LDFLAGS := -N -L $(LIBDIR) -l$(PROFILE)
LDFLAGS += -L $(PRODUCT_LIBDIR) -T $(LD_SCRIPT)
LDFLAGS += $(INSTSET)
LDFLAGS += -Wl,-Map,$(TARGET).map

# Main rules
.PHONY: all
all: start $(TARGET).elf $(TARGET).gdb.elf $(TARGET).hex $(TARGET).lss end

$(TARGET).elf : $(OBJS) $(LIBDIR)/lib$(PROFILE).a $(LD_SCRIPT)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

.PHONY: clean
clean:
	-$(RM) $(OBJDIR) *.MDS *.cod
	-$(RM) $(TARGET).map $(TARGET).lss $(TARGET).elf
	-$(RM) $(TARGET).hex $(TARGET)_APP.hex

include $(PLTF_DIR)/config/Rules.mk

$(V).SILENT:
