cd hls_proj
open_project fc_proj 
set_top fc_layer
add_files ../fc_test/fc_layer.cpp 
add_files -tb ../fc_test/fc_layer_test.cpp -cflags "-I . -std=c++0x"
add_files -tb ../util/shared.cpp
add_files -tb ../data
open_solution "solution1"
set_part {xcvu095-ffvc1517-2-e} -tool vivado
create_clock -period 250MHz -name default
config_interface -m_axi_addr64 -m_axi_offset off -register_io off
#csim_design -compiler gcc
csynth_design
#cosim_design
export_design -format ip_catalog
