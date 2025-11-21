#
# Copyright (C) 2013-2019 Melexis N.V.
#
# Software Platform
#
#

LDFLAGS += -Wl,-gc-sections
# LDFLAGS += -Wl,-print-gc-sections

# --- Messages ----------------------------------------------------------------

MSG_START = --- Building process started ...
MSG_END   = --- Building process finished

GENDEPFLAGS = -MMD -MP

# --- Default rules -----------------------------------------------------------
$(OBJDIR)/%.o : %.c
	$(CC) -c $(INSTSET) $(CFLAGS) $(CPPFLAGS) $(GENDEPFLAGS) $< -o $@

$(OBJDIR)/%.o : %.S
	$(CC) -c    $(INSTSET) $(ASFLAGS) $(CPPFLAGS) $(GENDEPFLAGS) $< -o $@

# Regenerate all obj files if Makefile or <profile>.mk is changed
#
# Rationale:
#   Makefile may change -D defines or include new <profile.mk> which makes
#   current obj files outdated
$(OBJS): $(PROFILE_DIR)/$(PROFILE_NAME).mk Makefile

# Regenerate library if any <profile.mk> from `config` directory is changed
#
# Rationale:
#   Profile provides configuration for the library via -D options
$(LIBDIR)/lib$(PROFILE_NAME)$(LIBNAME_POSTFIX).a: $(wildcard $(PROFILE_DIR)/*.mk)
	$(MAKE) libs || exit 1

# Regenerate linker script if template (ldt) or Makefile is changed
#
# Rationale:
#   Linker script template might get custome defines (-D) from the
#   user Makefile
$(LD_SCRIPT): $(PRODUCT_LIBDIR)/linker_script.ldt Makefile
	$(CC) -E -P -CC -x c $(INSTSET) $(CFLAGS) $(CPPFLAGS) -o $@ "$<"

# Rule to build platform library
#
# Notes:
# - clean existing libraries before the build to get proper build
# - if failed to build, clean libs to avoid half-constructed library
.PHONY: libs
libs:
	@echo --- Clean and rebuild the library
	$(MAKE) clean --directory=$(PLTF_DIR)/libsrc PROFILE=$(PROFILE)
	$(MAKE) all   --directory=$(PLTF_DIR)/libsrc PROFILE=$(PROFILE) \
		|| ($(MAKE) clean_libs; exit 1)
ifneq ($(LOADER),LDR)  # if we're not building libs for loader
	@echo --- Clean and rebuild the loader
	$(MAKE) clean  --directory=$(PLTF_DIR)/../loader PROFILE=$(PROFILE)
	$(MAKE) all    --directory=$(PLTF_DIR)/../loader PROFILE=$(PROFILE) LOADERSIZE=$(LOADERSIZE)
endif


.PHONY: clean_libs
clean_libs:
	$(MAKE) clean --directory=$(PLTF_DIR)/libsrc PROFILE=$(PROFILE)
ifeq ($(LOADER), APP)
	$(MAKE) clean --directory=$(PLTF_DIR)/../loader PROFILE=$(PROFILE)
endif

# Create hex / bin files from ELF output file
# TODO: move loader.hex build to `libs` rule?
%.hex: %.elf
#	$(OCP) -O ihex $< $@
	$(OCP) -O ihex --remove-section .ep.data $< $@
ifdef LOADER
ifeq ($(LOADER), APP)
	$(MAKE) add-loader --directory=$(PLTF_DIR)/../loader \
		-f Loader.mk \
		HEXFILE=$(abspath $@) \
		LOADER_HEX=loader_$(PROFILE_NAME).hex \
		LOADERSIZE=$(LOADERSIZE) \
		PROTECTIONKEY64bit="$(PROTECTIONKEY64bit)"
else
	# LOADER != APP, i.e. hex for loader
endif
else
# LOADER is not defined, i.e. hex for standalone application
ifneq (,$(filter $(APP_IN_RAM),1))
	# APP_IN_RAM = 1
	# hex for RAM program, do not add Protection Key
else
	# APP_IN_RAM = 0 or not defined
	# hex for Flash, add Protection Key
	$(MAKE) add-protection-key --directory=$(PLTF_DIR)/../loader \
		-f Loader.mk \
		HEXFILE=$(abspath $@) \
		PROTECTIONKEY64bit="$(PROTECTIONKEY64bit)"
endif
endif

%.bin: %.elf
	$(OCP) -O binary $< $@

# Create extended listing file from ELF output file
%.lss: %.elf
#	$(CC) --version > $@; $(ODP) -h -S -z $< >> $@
	$(call versions) > $@; $(ODP) -h -S -z -m mlx16ex $< >> $@ # force mlx16-ex target as workaround for MLXCOMP-304; TODO: should be fixed in mlx16-gcc.1.12.5

# Create a symbol table from ELF output file
%.sym: %.elf
	$(NM) -n $< > $@

# EP section initialization data (normally NVRAM/EEPROM data)
%.eep: %.elf
	-$(OCP) --only-section .ep.data -O ihex $< $@

# Create assembler files from C source files.
%.s : %.c
	$(CC) -S $(INSTSET) $(CFLAGS) $(CPPFLAGS) $< -o $@

# Create preprocessed source
%.i : %.c
	$(CC) -E $(INSTSET) $(CFLAGS) $(CPPFLAGS) $< -o $@

# Create elf file with removed leading underscores from symbols needed for debugging in gdb
%.gdb.elf: %.syms %.elf
	@$(ECHO) --- Generating: $@
	$(HIDE_CMD)$(OCP) --redefine-syms=$^ $@

%.syms: %.elf
	$(eval NM_OUT:= $$(shell $(NM) -f posix --defined-only $$<))
	$(eval NM_SORT:= $(sort $(NM_OUT)))
	$(HIDE_CMD)DIRTY_SYMBOLS="$(filter _%,$(NM_SORT))"; \
	$(RM) $@;\
	for symbol in $${DIRTY_SYMBOLS} ; \
	do \
		echo $${symbol}" "$${symbol#_} >> $@; \
	done

.PHONY: start
start:
	@echo $(MSG_START)

.PHONY: end
end:
	@echo $(MSG_END)

define versions
    { $(CC) --version; \
      printf "Profile: %s\n" "$(PROFILE)"; \
    }
endef

# Include automatic dependencies for all targest except `clean` and `clean_libs`
ifeq (,$(filter $(MAKECMDGOALS),clean clean_libs))
-include $(shell mkdir $(OBJDIR) 2>/dev/null) $(wildcard $(OBJDIR)/*.d)
endif

# -- Lint Rules ---------------------------------------------------------------
ifeq (${MAKECMDGOALS},lint)
ifndef SRCS
$(error SRCS are not defined, nothing to Lint)
endif
endif

# Run static analysis
.PHONY: lint
lint:
	$(ECHO) | $(CC) -E -dM $(INSTSET) $(CFLAGS) $(CPPFLAGS) - > $(LINT_CONFIG_DIR)/lint_cmac.h
	@$(ECHO) --- Files for analysis by Lint: $(filter %.c, $(SRCS))
	$(LINT_EXE) -i$(LINT_CONFIG_DIR_WIN_PATH) std.lnt $(filter %.c, $(SRCS))

.PHONY: lint_clean
lint_clean:
	-$(RM) $(LINT_CONFIG_DIR)/lint_cmac.h
