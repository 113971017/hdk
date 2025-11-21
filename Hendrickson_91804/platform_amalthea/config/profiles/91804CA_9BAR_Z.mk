#
# Copyright (C) 2018 Melexis N.V.
#
# Software Platform Configuration File
#
#

# --- Chip revision specific configuration (PLEASE DO NOT CHANGE THIS FILE) ---

# -- RFTX
# Enable internal load capacitors on the crystal:
#       -DPLL_EN_XTAL_INT_CAP_ID=n
#   where n
#   0: external caps are used
#   1: internal caps for 6pF diff load
#   2: internal caps for 8pF diff load
#   3: internal caps for 12pF diff load
CPPFLAGS += -DPLL_EN_XTAL_INT_CAP_ID=1

# Configure RF module
#
# Available RF options:
# 1) RF TX central frequency in ranges: 300000..330000 or 420000..450000 kHz
# E.g. for 433920 kHz
#     -DRFTX_CENTER_FREQ_IN_KHZ=433920
#
# 2) Uncomment:
#     -DHAS_RF_POWER_5_DBM to set 5 dBm; otherwise 8 dBm is used
#     -DHAS_RF_VPA_BYPASS to enable VPA bypass
CPPFLAGS += -DRFTX_CENTER_FREQ_IN_KHZ=433920
CPPFLAGS += -DRF_XTAL_FREQ_IN_KHZ=26000
CPPFLAGS += -DHAS_RF_POWER_5_DBM
#CPPFLAGS += -DHAS_RF_VPA_BYPASS


# Use calibrated accel gain
CPPFLAGS += -DHAS_ACCEL_CALIBRATED_GAIN

# Specify which acceleration range will be used: uncomment ONLY ONE from below
CPPFLAGS += -DHAS_ACCEL_SYMMETRICAL_RANGE
##CPPFLAGS += -DHAS_ACCEL_POSITIVE_RANGE
##CPPFLAGS += -DHAS_ACCEL_NEGATIVE_RANGE

# Specify if chip was calibrated for the 5 bar pressure range. Otherwise,
# it's assumed that chip is calibrated for the 9 or 14 bars
##CPPFLAGS += -DHAS_PRESSURE_CALIBRATION_FOR_5_BARS

# Specify if chip has X-axis and/or Z-axis acceleration sensor
##CPPFLAGS += -DHAS_ACCEL_X_SENSOR
CPPFLAGS += -DHAS_ACCEL_Z_SENSOR

# Acceleration capfilter is refreshed by hardware
CPPFLAGS += -DHAS_ACCEL_CAPFILTER_REFRESH_BY_HW

# Acceleration polarity is inverted early in acquisition chain
CPPFLAGS += -DHAS_ACCEL_POL_SDATA_CONTROL

# Acceleration offset delta has 10 bits
CPPFLAGS += -DHAS_ACCEL_DOFF_DELTA_10_BITS

CPPFLAGS += -DHAS_VERSION_CA
