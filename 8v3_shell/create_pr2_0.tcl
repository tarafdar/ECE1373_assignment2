set projName pr_region_Naif
create_project pr_region ./$projName -part xcvu095-ffvc1517-2-e
set_property  ip_repo_paths  {../ocl_ips ../../ECE1373_assignment1/vivado_hls_proj/conv_proj/solution1/impl/ip ../../ECE1373_assignment1/vivado_hls_proj/fc_proj/solution1/impl/ip ../../ECE1373_assignment1/vivado_hls_proj/maxpool_proj/solution1/impl/ip} [current_project]
update_ip_catalog

#source pr_region_2_bd.tcl
source nn_bd.tcl

