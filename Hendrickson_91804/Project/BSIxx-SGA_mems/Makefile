#
# Copyright (C) 2014-2018 Melexis N.V.
#
# Software Platform
#

# Application name (same as a folder name)
TARGET := $(notdir $(CURDIR))

PROFILE = 91804CA_14BAR_XZ

SG_TEST=1

LOADER=APP
#LOADERSIZE=3
#PROTECTIONKEY64bit = 0x12 0x34 0x56 0x78 0x9A 0xBC 0xDE 0xF0

PLTF_DIR = ../../platform_amalthea

include $(PLTF_DIR)/config/Config.mk


# search path for extra files
vpath
vpath %.ld $(PRODUCT_LIBDIR)

#
# SOURCE FILES LIST
#

SRCS = main.c 
SRCS += spi_process.c 
SRCS += measurements.c 
SRCS += processing.c 
SRCS += sg_common.c 
SRCS += sg_dbg.c 
SRCS += sg_lf.c #sg_rf.c

OBJS  = $(patsubst %.S, $(OBJDIR)/%.o, $(filter %.S, $(SRCS)))
OBJS += $(patsubst %.c, $(OBJDIR)/%.o, $(filter %.c, $(SRCS)))

# Enable debug
CPPFLAGS += -DDEBUG_ENABLED

# Disable optimization to simplify debugging
#CPPFLAGS += -DINLINE=inline
#CFLAGS += -O0

# Enable cast alignment warning
#CFLAGS += -Wcast-align


LDFLAGS := -N -L $(LIBDIR) -l$(PROFILE)
LDFLAGS += -L $(PRODUCT_LIBDIR) -T $(LD_SCRIPT)
LDFLAGS += $(INSTSET)
LDFLAGS += -Wl,-Map,$(TARGET).map

# Main rules
.PHONY: all
all: start $(TARGET).elf $(TARGET).hex $(TARGET).lss end

$(TARGET).elf : $(OBJS) $(LIBDIR)/lib$(PROFILE).a $(LD_SCRIPT)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

.PHONY: clean
clean:
	-$(RM) $(OBJDIR) *.MDS *.cod
	-$(RM) $(TARGET).map $(TARGET).lss $(TARGET).elf
	-$(RM) $(TARGET).elf $(TARGET).hex

include $(PLTF_DIR)/config/Rules.mk

.SILENT:
