open_checkpoint 8v3_shell/static_routed.dcp
read_checkpoint -cell static_region_i/pr_region 8v3_shell/$projName.dcp
opt_design
place_design
route_design
#write_checkpoint config2_routed.dcp
#write_checkpoint -cell static_region_i/pr_region rp2_route_design.dcp
write_bitstream -force 8v3_shell/$projName.bit 

