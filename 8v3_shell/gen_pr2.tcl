open_checkpoint static_routed.dcp
read_checkpoint -cell static_region_i/pr_region $projName.dcp
opt_design
place_design
route_design
#write_checkpoint config2_routed.dcp
#write_checkpoint -cell static_region_i/pr_region rp2_route_design.dcp
write_bitstream -force $projName.bit 

