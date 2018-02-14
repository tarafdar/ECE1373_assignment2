#
# This file contains constraints for the 9th byte lane (byte lane 8) for DDR4
# SDRAM bank 0 that are not driven by MIG when it is in an x64 configuration.
#
# The pins referenced in this file are tied off to constant levels in order to
# avoid spurious transitions but must nevertheless be constrained.
#

# Get IOSTANDARD & OUTPUT_IMPEDANCE for DQ[0] pin, which should use the same
# values as DQ[71:64].

## Pad Function: IO_L12N_T1U_N11_GC_48
set_property PACKAGE_PIN H16 [ get_ports "c0_ddr4_dq[64]" ]
set_property IOSTANDARD       POD12_DCI       [ get_ports "c0_ddr4_dq[64]" ]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [ get_ports "c0_ddr4_dq[64]" ]
set_property SLEW             FAST             [ get_ports "c0_ddr4_dq[64]" ]

## Pad Function: IO_L11P_T1U_N8_GC_48
set_property PACKAGE_PIN K15 [ get_ports "c0_ddr4_dq[65]" ]
set_property IOSTANDARD       POD12_DCI       [ get_ports "c0_ddr4_dq[65]" ]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [ get_ports "c0_ddr4_dq[65]" ]
set_property SLEW             FAST             [ get_ports "c0_ddr4_dq[65]" ]

## Pad Function: IO_L12P_T1U_N10_GC_48
set_property PACKAGE_PIN J16 [ get_ports "c0_ddr4_dq[66]" ]
set_property IOSTANDARD       POD12_DCI       [ get_ports "c0_ddr4_dq[66]" ]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [ get_ports "c0_ddr4_dq[66]" ]
set_property SLEW             FAST             [ get_ports "c0_ddr4_dq[66]" ]

## Pad Function: IO_L9P_T1L_N4_AD12P_48
set_property PACKAGE_PIN J14 [ get_ports "c0_ddr4_dq[67]" ]
set_property IOSTANDARD       POD12_DCI       [ get_ports "c0_ddr4_dq[67]" ]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [ get_ports "c0_ddr4_dq[67]" ]
set_property SLEW             FAST             [ get_ports "c0_ddr4_dq[67]" ]

## Pad Function: IO_L8N_T1L_N3_AD5N_48
set_property PACKAGE_PIN K13 [ get_ports "c0_ddr4_dq[68]" ]
set_property IOSTANDARD       POD12_DCI       [ get_ports "c0_ddr4_dq[68]" ]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [ get_ports "c0_ddr4_dq[68]" ]
set_property SLEW             FAST             [ get_ports "c0_ddr4_dq[68]" ]

## Pad Function: IO_L8P_T1L_N2_AD5P_48
set_property PACKAGE_PIN L13 [ get_ports "c0_ddr4_dq[69]" ]
set_property IOSTANDARD       POD12_DCI       [ get_ports "c0_ddr4_dq[69]" ]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [ get_ports "c0_ddr4_dq[69]" ]
set_property SLEW             FAST             [ get_ports "c0_ddr4_dq[69]" ]

## Pad Function: IO_L9N_T1L_N5_AD12N_48
set_property PACKAGE_PIN H14 [ get_ports "c0_ddr4_dq[70]" ]
set_property IOSTANDARD       POD12_DCI       [ get_ports "c0_ddr4_dq[70]" ]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [ get_ports "c0_ddr4_dq[70]" ]
set_property SLEW             FAST             [ get_ports "c0_ddr4_dq[70]" ]

## Pad Function: IO_L11N_T1U_N9_GC_48
set_property PACKAGE_PIN J15 [ get_ports "c0_ddr4_dq[71]" ]
set_property IOSTANDARD       POD12_DCI       [ get_ports "c0_ddr4_dq[71]" ]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [ get_ports "c0_ddr4_dq[71]" ]
set_property SLEW             FAST             [ get_ports "c0_ddr4_dq[71]" ]

# Get IOSTANDARD & OUTPUT_IMPEDANCE for DM[0] pin, which should use the same
# values as DM[8].
set c0_dm_dbi_n_iostandard       [ get_property IOSTANDARD       [ get_ports "c0_ddr4_dm_dbi_n[0]" ] ]
set c0_dm_dbi_n_output_impedance [ get_property OUTPUT_IMPEDANCE [ get_ports "c0_ddr4_dm_dbi_n[0]" ] ]
set c0_dm_dbi_n_slew             [ get_property SLEW             [ get_ports "c0_ddr4_dm_dbi_n[0]" ] ]

## Pad Function: IO_L7P_T1L_N0_QBC_AD13P_48
set_property PACKAGE_PIN J13 [ get_ports "c0_ddr4_dm_dbi_n[8]" ]
set_property IOSTANDARD       $c0_dm_dbi_n_iostandard       [ get_ports "c0_ddr4_dm_dbi_n[8]" ]
set_property OUTPUT_IMPEDANCE $c0_dm_dbi_n_output_impedance [ get_ports "c0_ddr4_dm_dbi_n[8]" ]
set_property SLEW             $c0_dm_dbi_n_slew             [ get_ports "c0_ddr4_dm_dbi_n[8]" ]


# Get IOSTANDARD & OUTPUT_IMPEDANCE for DQS#[0] pin, which should use the same
# values as DQS#[8].

## Pad Function: IO_L10N_T1U_N7_QBC_AD4N_48
set_property PACKAGE_PIN K16 [ get_ports "c0_ddr4_dqs_c[8]" ]
set_property IOSTANDARD       DIFF_POD12_DCI       [ get_ports "c0_ddr4_dqs_c[8]" ]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [ get_ports "c0_ddr4_dqs_c[8]" ]
set_property SLEW             FAST             [ get_ports "c0_ddr4_dqs_c[8]" ]

# Get IOSTANDARD & OUTPUT_IMPEDANCE for DQS[0] pin, which should use the same
# values as DQS[8].

## Pad Function: IO_L10P_T1U_N6_QBC_AD4P_48
set_property PACKAGE_PIN K17 [ get_ports "c0_ddr4_dqs_t[8]" ]
set_property IOSTANDARD       DIFF_POD12_DCI       [ get_ports "c0_ddr4_dqs_t[8]" ]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [ get_ports "c0_ddr4_dqs_t[8]" ]
set_property SLEW             FAST             [ get_ports "c0_ddr4_dqs_t[8]" ]

