#
# Copyright (C) 2014-2019 Melexis N.V.
#
# Software Platform
#

#--- List of modules for the library ------------------------------------------
LIBSUBMODULE := startup math ../custom/src

#--- Profile specific configuration of the library -----------------------------
include $(PROFILE_DIR)/$(PROFILE_NAME).mk

# -- Loader
ifndef LOADERSIZE
    # If LOADERSIZE is not defined in Makefile, set it to 3KB (3 sectors)
    LOADERSIZE=3
endif

# If Loader is defined:
ifdef LOADER
    CPPFLAGS += -D__LOADERSIZE__=$(LOADERSIZE)

    ifeq ($(strip $(LOADER)), APP)
        CPPFLAGS += -D__APPLICATION_WITH_LOADER__   # compile user application linked with loader
    else
        ifeq ($(strip $(LOADER)), LDR)
            CPPFLAGS += -D__LOADER_ONLY__           # compile loader code
        else
            $(error Error: Variable LOADER is defined incorrectly)
        endif
    endif
endif

# EOF
