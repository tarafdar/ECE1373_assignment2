
create_project mig_shell ./mig_shell_ila_proj -part xcvu095-ffvc1517-2-e
set_property  ip_repo_paths  {../ocl_ips} [current_project]
update_ip_catalog

source mig_shell_ila_bd.tcl 
update_compile_order -fileset sources_1

make_wrapper -files [get_files mig_shell_ila_proj/mig_shell.srcs/sources_1/bd/static_region/static_region.bd] -top
add_files -norecurse mig_shell_ila_proj/mig_shell.srcs/sources_1/bd/static_region/hdl/static_region_wrapper.v
update_compile_order -fileset sources_1

generate_target {synthesis implementation} [get_files mig_shell_ila_proj/mig_shell.srcs/sources_1/bd/static_region/static_region.bd]
export_ip_user_files -of_objects [get_files mig_shell_ila_proj/mig_shell.srcs/sources_1/bd/static_region/static_region.bd] -no_script -sync -force -quiet
create_ip_run [get_files -of_objects [get_fileset sources_1] mig_shell_ila_proj/mig_shell.srcs/sources_1/bd/static_region/static_region.bd]
launch_runs [get_runs *_synth*] -jobs 12
foreach run_name [get_runs *_synth*] {
  wait_on_run ${run_name}
}


add_files -fileset constrs_1 -norecurse { \
bitstream.xdc  \
ddr4sdram_b0_lane8.xdc \
ddr4sdram_b0_unused.xdc \
ddr4sdram_locs_b0_twin_die.xdc \
ddr4sdram_locs_b0_x64.xdc \
ddr4sdram_locs_b0_x72.xdc \
ddr4sdram_props_b0_twin_die.xdc \
pcie_constr.xdc}

synth_design -top static_region_wrapper

create_pblock pblock_pr_region
add_cells_to_pblock [get_pblocks pblock_pr_region] [get_cells -quiet [list static_region_i/pr_region]]
resize_pblock [get_pblocks pblock_pr_region] -add {CLOCKREGION_X0Y5:CLOCKREGION_X4Y7}
set_property SNAPPING_MODE ON [get_pblocks pblock_pr_region]
set_property HD.RECONFIGURABLE true [get_cells static_region_i/pr_region]

opt_design
place_design
route_design
write_checkpoint config1_routed.dcp -force
write_bitstream config1 -force
write_debug_probes mig_shell_ila.ltx -force
write_checkpoint -cell static_region_i/pr_region rp1_route_design.dcp -force
update_design -cell static_region_i/pr_region -black_box
lock_design -level routing
write_checkpoint static_routed.dcp -force
