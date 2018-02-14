# ECE1373_assignment1
assignment1

Welcome to the first assignment in ECE1373

This will describe how to run the provided sample code in this directory. 
This description assumes you have read the assignment handout in doc/assignment1.pdf



The source code is organized as follows:
-fc directory has files for fully connected layer
-conv directory has files for the convolution layer
-nn_params stores binaries for the weights, biases, inputs and reference output. This also contains a script
extractParams to create new binaries for other layers. 
-util directory has the shared function to read input.
-vivado_hls_proj contains tcl scripts to create a vivado_hls project for convolution and fc.

To create a project with the sample unoptimized code and run csim and synth design do the following:

1)Install vivado 2017.2 

2)source <xilinx install path>/Vivado/2017.2/settings64.sh 

3)cd vivado_hls_proj

4)vivado_hls conv_hls.tcl

Running cosim on the unoptimized code will take a really long time. It has been commented out. Feel free to add it back to an optimized version.

If you look at the testing functions (fc/fc_layer_test.cpp conv/conv_layer_test.cpp) you will notice that there is an imageDir string.
This test function checks for weights, biase, input and golden reference to be in nn_params / (layer name) / weights, nn_params / (layer name) / biases, etc.

To test other layers you would have to change the string to point to another directory.

We provide a three fc layers which you can run through (fc1, fc2, fc3) and three convolution layers (conv1_7x7_s2, conv2_3x3_reduce, inception3a_1x1)

You can make more binaries by running the extractParams.py. This requires an install of caffe (please refer online on how to get caffe).
We will also provide a docker shortly with vivado and caffe preinstalled. 

