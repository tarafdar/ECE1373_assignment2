all: conv_layer fc_layer hw_conv_layer hw_fc_layer

conv_layer: conv_test/* util/*
	g++ conv_test/*.cpp conv_test/*.c util/*.cpp -I conv_test -I./ -o conv_layer -std=c++11

hw_conv_layer: conv_test/* util/*
	g++ -DHW_TEST conv_test/*.cpp conv_test/*.c util/*.cpp -I conv_test -I./ -o hw_conv_layer -std=c++11

fc_layer: fc_test/* util/*
	g++ fc_test/*.cpp fc_test/*.c util/*.cpp -I fc_test -I./ -o fc_layer -std=c++11

hw_fc_layer: fc_test/* util/*
	g++ -DHW_TEST fc_test/*.cpp fc_test/*.c util/*.cpp -I fc_test -I./ -o hw_fc_layer -std=c++11


pr: 
	vivado_hls vivado_hls_proj/conv_hls.tcl
	vivado_hls vivado_hls_proj/fc_hls.tcl
	vivado -mode tcl -source 8v3_shell/create_pr2_nn.tcl 

static:
	vivado -mode tcl -source 8v3_shell/create_mig_shell.tcl 

pr_modify:
	vivado -mode gui -source 8v3_shell/create_pr2_0.tcl


clean_sw: 
	rm -rf fc_layer hw_fc_layer conv_layer hw_conv_layer

clean_pr:
	rm -rf 8v3_shell/pr_region_test 

clean_static:
	rm -rf 8v3_shell/mig_shell_ila_proj


