#set projName pr_region_Naif
#create_project pr_region ./$projName -part xcvu095-ffvc1517-2-e
#set_property  ip_repo_paths  {../ocl_ips} [current_project]
#update_ip_catalog
#
#source pr_region_2_bd.tcl

make_wrapper -files [get_files $projName/pr_region.srcs/sources_1/bd/pr_region_2/pr_region_2.bd] -top
add_files -norecurse $projName/pr_region.srcs/sources_1/bd/pr_region_2/hdl/pr_region_2_wrapper.v
update_compile_order -fileset sources_1

generate_target {synthesis implementation} [get_files $projName/pr_region.srcs/sources_1/bd/pr_region_2/pr_region_2.bd]
export_ip_user_files -of_objects [get_files $projName/pr_region.srcs/sources_1/bd/pr_region_2/pr_region_2.bd] -no_script -sync -force -quiet
create_ip_run [get_files -of_objects [get_fileset sources_1] $projName/pr_region.srcs/sources_1/bd/pr_region_2/pr_region_2.bd]
launch_runs [get_runs *_synth*] -jobs 12
foreach run_name [get_runs *_synth*] {
  wait_on_run ${run_name}
}
synth_design -top pr_region_2_wrapper -mode out_of_context
write_checkpoint -force $projName.dcp
source gen_pr2.tcl
 
#read_checkpoint -cell static_region_i/pr_region rp2_synth.dcp
#opt_design
#place_design
#route_design
#write_checkpoint config2_routed.dcp
#write_checkpoint -cell static_region_i/pr_region rp2_route_design.dcp
#write_bitstream -force config2
