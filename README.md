# ECE1373_assignment2

assignment2

Welcome to the first assignment in ECE1373

This will describe how to run the provided sample code in this directory. 
This description assumes you have read the assignment handout in doc/assignment2.pdf



The source code is organized as follows:
-fc directory has files for fully connected layer
-conv directory has files for the convolution layer
-nn_params stores binaries for the weights, biases, inputs and reference output. This also contains a script
extractParams to create new binaries for other layers. 
-util directory has the shared function to read input.
-vivado_hls_proj contains tcl scripts to create a vivado_hls project for convolution and fc.
-pci_tests includes tests to read and write to pcie.

To create a project with the sample unoptimized code and run csim and synth design do the following:


Pre-requisites

1)Install vivado 2017.2 in /opt/ 

2)Install caffe in /opt/

3)source sourceme.sh 



FPGA Hypervisor 

We use partial reconfiguration to program your applcation into an already programmed FPGA (we call this the Hypervisor)
The Hypervisor contains the following
-A PCIe module that can address into 1 MB of control registers through AXI-Lite and 2 GB of off-chip memory
-An off-chip memory controller interfacing with 8GB of off-chip memory (first 2 GB is shared with PCIe addressable space)
-Partial region to be programmed with partial reconfiguration, contains an AXI-Lite Slave interface to receive control information from PCIe
and contains an AXI-Master interface connecting to the off-chip memory controller.

To create the hypervisor (must be done before creating an application). This will take about 45 minutes but only needs to be done once.

cd ./8v3_shell
vivado -mode batch -source create_mig_shell.tcl


Creating the Data
To create the data run the extractParams_imagenet.py in the nn_params directory.
You can as an argument provide the number of batches, and the network model.
To specify the batch size, you can modify the model file.
This creates a directory per batch in the data/vgg_batches/ directory.
Each batch contains the input, output, weights, biases, a description of the parameters for each layer, and the labels for the images.



Running an Example

We have provided an example with PCIe connecting to a convolution layer and an fc layer directly through PCIe. These modules are using only off-chip memory
to communicate with the host. (is this the best way to do it?)
We have verified functionality of the fully-connected layer (conv-layer is untested).
We have also provided a driver application that sets up the control registers of the fully-connected layer.

First you need to build the hls ip that will be imported into the partial region.

cd ./vivado_hls_proj
vivado_hls fc_hls.tcl
vivado_hls conv_hls.tcl

You can look at the generated slave_axi registers to see which offsets you need to program the control registers.
For example for the fc look at ./vivado_hls_proj/fc_proj/solution1/impl/verilog/fc_layer_CTRL_BUS_s_axi.v

Now to create the pr_region. We provided one script.

cd ./8v3_shell
vivado -mode batch -source create_pr2_nn.tcl

This calls two scripts create_pr2_0.tcl and create_pr2_1.tcl

The first script creates the partial application. If you want to modify it. Comment out the sourcing of create_pr2_1.tcl and modify the block diagram.

At this point you can run the test application.
Look at the Makefile to see the tests. To build for example a sample conv_layer in hardware it is. 
make 
./hw_conv_layer

This writes control registers, copies data into the off-chip memory, starts the application and reads data out. 

Verifying Accuracy
The above test creates a file in the data directory for each batch layer called dma_out. 
The nn_params/softMax.py script pumps the data out of the last fully connected layer  through a softmax and compares the results with the label information.
This will give you an accuracy result of your network. 





