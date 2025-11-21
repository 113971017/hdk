#
# The following rules are defined in this Makefile
#   - add-protection-key:
#       To add Protection Key to the application hex file
#   - add-loader:
#       To add to application hex: loader.hex, Protection Key and
#       all required CRCs
#
#   Currently, these rules are invoked from the %.hex building rule
#   defined in Rules.mk:
#       $(MAKE) add-loader --directory=$(PLTF_DIR)/../loader \
#           -f Loader.mk \
#           HEXFILE=$(abspath $@) \
#           LOADERSIZE=$(LOADERSIZE) \
#           PROTECTIONKEY64bit="$(PROTECTIONKEY64bit)"
#
#********************************************************************


ifeq ($(OS),Windows_NT)
	CRCToHex_SREC_CAT = ../platform_amalthea/bin/srec_cat.exe

# If cypath is not found assume mingw and use Linux path style
ifeq ($(shell where cygpath),)
	CRCToHex_InputFile = "$(HEXFILE)"
	CRCToHex_OutputFile = "$(basename $(HEXFILE))_APP.hex"
	LoaderHex = "$(LOADER_HEX)"
else
	CRCToHex_InputFile = "$(shell cygpath -w $(HEXFILE))"
	CRCToHex_OutputFile = "$(basename $(shell cygpath -w $(HEXFILE)))_APP.hex"
	LoaderHex = "$(shell cygpath -w $(LOADER_HEX))"
endif

else
# Assume Linux
	CRCToHex_SREC_CAT = srec_cat

	CRCToHex_InputFile = "$(HEXFILE)"
	CRCToHex_OutputFile = "$(basename $(HEXFILE))_APP.hex"
	LoaderHex = "$(LOADER_HEX)"
endif

ifndef PROTECTIONKEY64bit
    # Define key in unprotected state (0x00 or 0xFF), if PROTECTIONKEY64bit
	# variable is empty, i.e. not provided by the application's Makefile
    #override PROTECTIONKEY64bit :=0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
    override PROTECTIONKEY64bit :=0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
    #$(warning "Protection key is not defined, Flash content is not protected")
endif

LDRDIR = .
CRCToHex_SectorSize = 1024
CRCToHex_FlashSizeInSectors = 16
CRCToHex_FlashStart = 0x6000
CRCToHex_FlashEnd = $$(( $(CRCToHex_FlashStart) + $(CRCToHex_FlashSizeInSectors) * $(CRCToHex_SectorSize) ))

# Loader size in sectors is $(LOADERSIZE)
CRCToHex_LoaderStart = $(CRCToHex_FlashStart)
CRCToHex_AppSizeInSectors = $$(( $(CRCToHex_FlashSizeInSectors) - $(LOADERSIZE) ))
CRCToHex_AppStart = $$(( $(CRCToHex_LoaderStart) + $(LOADERSIZE) * $(CRCToHex_SectorSize) ))

CRCToHex_AppCrcSize = $$(( $(CRCToHex_AppSizeInSectors) * 2 ))
CRCToHex_PKeySize = 8
CRCToHex_CrcLastSectorSize = 2
CRCToHex_ProtectionAreaSize = $$(( $(CRCToHex_AppCrcSize) + $(CRCToHex_PKeySize) + $(CRCToHex_CrcLastSectorSize) ))
CRCToHex_ProtectionAreaStart = $$(( $(CRCToHex_FlashEnd) - $(CRCToHex_ProtectionAreaSize) ))
CRCToHex_PKeyStart = $$(( $(CRCToHex_FlashEnd) - $(CRCToHex_CrcLastSectorSize) - $(CRCToHex_PKeySize) ))

CRCToHex_FillGapSymbol = 0x00

#********************************************************************
MergeAppWithLoader = \
		loc_start=$(CRCToHex_LoaderStart); \
		loc_end=$$(( $(CRCToHex_AppStart) - 2 )); \
		printf "Added Loader              (FROM: 0x%4X TO: 0x%4X)\n" $$((loc_start)) $$(( $$((loc_end)) - 1 )); \
		$(CRCToHex_SREC_CAT) \
			'(' \
				$(CRCToHex_InputFile) --intel \
			')' \
			'(' \
				$(LDRDIR)/$(LoaderHex) --intel \
				--fill $(CRCToHex_FillGapSymbol) $$((loc_start)) $$((loc_end)) \
			')' \
			--output $(CRCToHex_InputFile) --intel \
			--address-length=2 --line-length=44

#********************************************************************
StoreLoaderCrc = \
		loc_start=$(CRCToHex_LoaderStart); \
		loc_end=$$(( $(CRCToHex_AppStart) - 2 )); \
		printf "CRC for LOADER data       (FROM: 0x%4X TO: 0x%4X PLACE AT: 0x%4X)\n" $$((loc_start)) $$(( $$((loc_end)) - 1 )) $$((loc_end)); \
		$(CRCToHex_SREC_CAT) \
			'(' \
				$(CRCToHex_InputFile) --intel \
			')' \
			'('	\
				$(CRCToHex_InputFile) --intel \
				--crop $$((loc_start)) $$((loc_end)) \
				--byte_swap 2 \
				--CRC16_Little_Endian  $$((loc_end)) \
				--xmodem \
				--crop $$((loc_end)) $$(( $$((loc_end)) + 2 )) \
			')' \
			--output $(CRCToHex_InputFile) --intel \
			--address-length=2 --line-length=44

#********************************************************************
# Calculate and store CRC of specified User Sectors (strting from 0)
StoreUserSectorCrc = \
		loc_start=$$(( $(CRCToHex_AppStart) + $(1) * $(CRCToHex_SectorSize) )); \
		loc_end=$$(( $$((loc_start)) + $(CRCToHex_SectorSize) )); \
		loc_storeat=$$(( $(CRCToHex_ProtectionAreaStart) + $(1) * 2 )); \
 		printf "CRC for %2d-Kb of APP data (FROM: 0x%4X TO: 0x%4X PLACE AT: 0x%4X)\n" $$(($(1)+1)) $$((loc_start)) $$(($$((loc_end))-1)) $$((loc_storeat)); \
		$(CRCToHex_SREC_CAT) \
			'(' \
				$(CRCToHex_InputFile) --intel \
				--fill $(CRCToHex_FillGapSymbol) $$((loc_start)) $$((loc_end)) \
			')' \
			'('	\
				$(CRCToHex_InputFile) --intel \
				--fill $(CRCToHex_FillGapSymbol) $$((loc_start)) $$((loc_end)) \
				--crop $$((loc_start)) $$((loc_end)) \
				--byte_swap 2 \
				--CRC16_Little_Endian  $$((loc_storeat)) \
				--xmodem \
				--crop $$((loc_storeat)) $$(( $$((loc_storeat)) + 2)) \
			')' \
			--output $(CRCToHex_InputFile) --intel \
			--address-length=2 --line-length=44
#********************************************************************
StoreZeroInLastCrcSlot = \
		loc_storeat=$$(( $(CRCToHex_ProtectionAreaStart) + $(CRCToHex_AppCrcSize) - 2 )); \
		$(CRCToHex_SREC_CAT) \
			'(' \
				$(CRCToHex_InputFile) --intel \
			')' \
			'(' \
				--generate $$((loc_storeat)) $$(( $$(( loc_storeat )) + 2 )) -constant 0x00 \
			')' \
		--output $(CRCToHex_InputFile) --intel \
		--address-length=2 --line-length=44
#********************************************************************
StoreProtectionKey = \
		loc_start=$(CRCToHex_PKeyStart); \
		loc_end=$$(( $(CRCToHex_PKeyStart) + $(CRCToHex_PKeySize) )); \
		printf "Added Protection Key      (FROM: 0x%4X TO: 0x%4X); " $$((loc_start)) $$(( $$((loc_end)) - 1 )); \
		printf "Protection Key content = %s\n" "$(1)"; \
		$(CRCToHex_SREC_CAT) \
			'(' \
				$(CRCToHex_InputFile) --intel \
			')' \
			'('	\
				--generate $$((loc_start)) $$((loc_end)) --repeat-data $(1) \
				--byte_swap 2 \
			')' \
			--output $(CRCToHex_InputFile) --intel \
			--address-length=2 --line-length=44
#********************************************************************
StoreLastSectorCrc = \
		loc_start=$$(( $(CRCToHex_FlashEnd) - $(CRCToHex_SectorSize) )); \
		loc_end=$$(( $(CRCToHex_FlashEnd) - $(CRCToHex_CrcLastSectorSize) )); \
 		printf "CRC for %2d-Kb of APP data (FROM: 0x%4X TO: 0x%4X PLACE AT: 0x%4X)\n" \
			$(CRCToHex_AppSizeInSectors) $$((loc_start)) $$(($$((loc_end))-1)) $$((loc_end)); \
		$(CRCToHex_SREC_CAT) \
			'(' \
				$(CRCToHex_InputFile) --intel \
				--fill $(CRCToHex_FillGapSymbol) $$((loc_start)) $$((loc_end)) \
			')' \
			'('	\
				$(CRCToHex_InputFile) --intel \
				--fill $(CRCToHex_FillGapSymbol) $$((loc_start)) $$((loc_end)) \
				--crop $$((loc_start)) $$((loc_end)) \
				--byte_swap 2 \
				--CRC16_Little_Endian  $$((loc_end)) \
				--xmodem \
				--crop $$((loc_end)) $$(( $$((loc_end)) + $(CRCToHex_CrcLastSectorSize) )) \
			')' \
		--output $(CRCToHex_InputFile) --intel \
		--address-length=2 --line-length=44
#********************************************************************
SaveAppHex = \
		$(CRCToHex_SREC_CAT) \
			$(CRCToHex_InputFile) --intel \
			--exclude $(CRCToHex_LoaderStart) $(CRCToHex_AppStart)\
			--output $(CRCToHex_OutputFile) --intel \
			--obs=16 --address-length=2
#********************************************************************
.PHONY: add-protection-key
add-protection-key:
	$(call StoreProtectionKey,$(PROTECTIONKEY64bit))

# add-loader rule description:
# - merge Application hex with Loader hex
# - store CRC of Loader area to the last word of the Loader Area
# - store CRC of the each 1Kbyte sector from the User Flash Area except the
#   last sector; CRCs are stored in _flash_protection_area.CRC16_of_Flash_Sector[] array
# - store 0x0000 to the last element of _flash_protection_area.CRC16_of_Flash_Sector[],
#   since last sector's CRC is stored separetly
# - store ProtectionKey in _flash_protection_area.Protection_Key
# - store CRC of the last Flash sector in _flash_protection_area.Config_CRC16,
#	located in the last word of the Flash
# - save application hex file without loader area as <name>_APP.hex file
.PHONY: add-loader
add-loader:
	$(call MergeAppWithLoader ); \
	$(call StoreLoaderCrc ); \
		cnt=0 ; while [ $$((cnt)) -lt $$(($(CRCToHex_AppSizeInSectors)-1)) ] ; do \
			$(call StoreUserSectorCrc, cnt) ; \
			cnt=$$((cnt+1)) ; \
		done; \
	$(call StoreZeroInLastCrcSlot ); \
	$(call StoreProtectionKey,$(PROTECTIONKEY64bit)); \
	$(call StoreLastSectorCrc ); \
	$(call SaveAppHex ); \
	$(call "printf "Starting address of an APPLICATION: 0x%4X\n" $$(($(CRCToHex_AppStart)))" )
#********************************************************************
