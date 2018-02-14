set_property PACKAGE_PIN AA7 [get_ports {pcie_clk_clk_p[0]}]
set_property IOSTANDARD LVCMOS18 [get_ports reset_rtl]
set_property PACKAGE_PIN AJ31 [get_ports reset_rtl]
set_false_path -from [get_ports reset_rtl]
set_max_delay -from [get_ports reset_rtl] 100.000
