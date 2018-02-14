set projName pr_region_test
create_project pr_region ./$projName -part xcvu095-ffvc1517-2-e
set_property  ip_repo_paths  {./ocl_ips ../vivado_hls_proj/conv_proj/solution1/impl/ip ../vivado_hls_proj/fc_proj/solution1/impl/ip } [current_project]
update_ip_catalog

#source pr_region_2_bd.tcl
source nn_bd.tcl

