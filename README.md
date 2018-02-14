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
cd ./pcie_tests/tests
make
./fully_connected_test

This writes control registers, copies data into the off-chip memory, starts the application and reads data out. Look at fully_connected.c for details.


Creating More Layers

To do this you will have to use the extractParams.py script in nn_params

You will modify the layers array at the top to include all layers you want the input and output for. Refer to /opt/caffe/models/bvlc_alexnet/deploy.prototxt for layer names.


