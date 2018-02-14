#
# This file contains constraints for signals of SDRAM bank 0 that are not used
# by MIG as of Vivado 2015.4.
#
# The pins referenced in this file are tied off to constant levels in order to
# avoid spurious transitions but must nevertheless be constrained.
#

# Use ODT (output only) as a model for PAR (output only)
set_property PACKAGE_PIN      G7                     [get_ports "c0_ddr4_par"]
set_property IOSTANDARD       SSTL12_DCI        [get_ports "c0_ddr4_par"]
set_property OUTPUT_IMPEDANCE RDRV_40_40  [get_ports "c0_ddr4_par"]
set_property SLEW             FAST              [get_ports "c0_ddr4_par"]

# Use RESET# (output only) as a model for TEN (output only)
set_property PACKAGE_PIN      J10                    [get_ports "c0_ddr4_ten"]
set_property IOSTANDARD       LVCMOS12      [get_ports "c0_ddr4_ten"]
set_property DRIVE            8           [get_ports "c0_ddr4_ten"]

# ALERT# is open-drain
set_property PACKAGE_PIN      H7                     [get_ports "c0_ddr4_alert_n"]
set_property IOSTANDARD       LVCMOS12               [get_ports "c0_ddr4_alert_n"]

# Use ADR0 (output only) as a model for ADR17 (output only)
set_property PACKAGE_PIN      H9                     [get_ports "c0_ddr4_adr[17]"]
set_property IOSTANDARD       SSTL12_DCI       [get_ports "c0_ddr4_adr[17]"]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports "c0_ddr4_adr[17]"]
set_property SLEW             FAST             [get_ports "c0_ddr4_adr[17]"]

