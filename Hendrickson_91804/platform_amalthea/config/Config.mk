#
# Copyright (C) 2013-2019 Melexis N.V.
#
# Software Platform
#
#


#--- Common directories -------------------------------------------------------
# Identify the root directory of the SW Platform (escape spaces in the path)
empty :=
space := $(empty) $(empty)
ROOTDIR := $(subst $(space),\$(space),$(realpath $(dir $(filter %/Config.mk, $(MAKEFILE_LIST)))..))
LIBDIR  := $(ROOTDIR)/lib
OBJDIR := obj

#--- Common flags -------------------------------------------------------------
INSTSET := -mlx16-ex
OPTIMIZATION = -Os

# Prepocessor flags
CPPFLAGS  = -I . -I $(ROOTDIR)/include

# C flags
CFLAGS  = $(OPTIMIZATION)
CFLAGS += -g
CFLAGS += -std=gnu99
CFLAGS += -mram-align-word
CFLAGS += -mdpage-explicit  # globals in nodp by default

CFLAGS += -fms-extensions #	enable tag for unnamed struct and union (enabled in C11 by default)
CFLAGS += -ffunction-sections -fdata-sections

#CFLAGS += -fshort-enums
#CFLAGS += -fno-defer-pop #	force to pop function's arguments from the stack after each call

CFLAGS += -Wall -Wextra
# CFLAGS += -Wconversion #	for gcc > 4.4
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wundef
CFLAGS += -Wcast-align
#CFLAGS += -Wno-packed-bitfield-compat  # false warning on bitfields
#CFLAGS += -Wlogical-op
CFLAGS += -Winline

# Asm flags
ASFLAGS  = $(OPTIMIZATION) -gdwarf-2

#--- Product specific configuration -------------------------------------------
ifndef PROFILE
$(error Error: Variable PROFILE is not defined)
endif

# Strip file extension and spaces (if any)
PROFILE_NAME := $(notdir $(strip $(subst $(empty) ,?,$(basename $(strip $(PROFILE))))))
PROFILE_NAME := $(subst ?,$(empty),$(PROFILE_NAME))

PROFILE_DIR    := $(ROOTDIR)/config/profiles
PRODUCT_DIR    := $(ROOTDIR)/custom
PRODUCT_LIBDIR := $(ROOTDIR)/custom/lib

CPPFLAGS += -I $(PRODUCT_DIR)/include

# Path/name for generated linker script
# NB: Do not override LD_SCRIPT variable!
LD_SCRIPT = $(OBJDIR)/linker_script.ld

include $(ROOTDIR)/config/Extra.mk

#--- Toolchain and utilities --------------------------------------------------
# All tools shall be in the path
CC   = mlx16-gcc
AS   = mlx16-gcc -x assembler-with-cpp
AR   = mlx16-ar
ODP  = mlx16-objdump
OCP  = mlx16-objcopy
NM   = mlx16-nm
SIZE = mlx16-size

# Standard utilities (MUST be in the environment PATH)
ECHO  = echo
RM    = rm -fr
CP    = cp -f
MKDIR = mkdir -p

ifeq ($(OS),Windows_NT)

# Lint
LINT_CONFIG_DIR := $(ROOTDIR)/config/lint
LINT_EXE = lint-nt.exe

# If cypath is not found assume mingw and use Linux path style
ifeq ($(shell where cygpath),)
	LINT_CONFIG_DIR_WIN_PATH := $(LINT_CONFIG_DIR)
else
	LINT_CONFIG_DIR_WIN_PATH := $(shell cygpath -wm $(LINT_CONFIG_DIR))
endif

endif

# EOF
