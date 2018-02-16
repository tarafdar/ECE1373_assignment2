all: conv_layer fc_layer hw_conv_layer hw_fc_layer

conv_layer: conv_test/* util/*
	g++ conv_test/*.cpp conv_test/*.c util/*.cpp -I conv_test -I./ -o conv_layer -std=c++11

hw_conv_layer: conv_test/* util/*
	g++ -DHW_TEST conv_test/*.cpp conv_test/*.c util/*.cpp -I conv_test -I./ -o hw_conv_layer -std=c++11

fc_layer: fc_test/* util/*
	g++ fc_test/*.cpp fc_test/*.c util/*.cpp -I fc_test -I./ -o fc_layer -std=c++11

hw_fc_layer: fc_test/* util/*
	g++ -DHW_TEST fc_test/*.cpp fc_test/*.c util/*.cpp -I fc_test -I./ -o hw_fc_layer -std=c++11
