# ECE1373_assignment2

Welcome to the second assignment in ECE1373

This will describe how to run the provided sample code in this directory. 
This description assumes you have read the assignment handout in doc/assignment2.pdf

##Code Organization

The source code is organized as follows:
- fc directory has files for fully connected layer
- conv directory has files for the convolution layer
- nn_params stores binaries for the weights, biases, inputs and reference output. This also contains a script extractParams to create new binaries for other layers. 
- util directory has the shared function to read input.
- vivado_hls_proj contains tcl scripts to create a vivado_hls project for convolution and fc.
- pci_tests includes tests to read and write to pcie.
- 8v3_shell contains files and projects for the hypervisor and user appplications

To create a project with the sample unoptimized code and run csim and synth design do the following:


##Pre-requisites

1. Install vivado 2017.2 in /opt/  (Already done in the container
2. Install caffe in /opt/  (Already done in the container)
3. ``source sourceme.sh`` 



##FPGA Hypervisor 

We use partial reconfiguration to program your applcation into an already programmed FPGA (we call this the Hypervisor)
The Hypervisor contains the following:

- A PCIe module that can address into 1 MB of control registers through AXI-Lite and 2 GB of off-chip memory
- An off-chip memory controller interfacing with 8GB of off-chip memory (first 2 GB is shared with PCIe addressable space)
- Partial region to be programmed with partial reconfiguration, contains an AXI-Lite Slave interface to receive control information from PCIe and contains an AXI-Master interface connecting to the off-chip memory controller.

**NOTE: WE HAVE CREATED THE HYPERVISOR FOR YOU AND IT CAN BE FOUND IN http://www.eecg.toronto.edu/~tarafda1/hypervisors/adm-8v3/static_routed_v1.dcp **

Do not create a new hypervisor as you will be sharing this FPGA with your colleagues who will be building with the same hypervisor
The Makefile is modified to pull the DCP as needed. 


##Creating the Data

To create the data run the extractParams_imagenet.py in the nn_params directory. 
You can as an argument provide the number of batches, and the network model.
To specify the batch size, you can modify the model file.
This creates a directory per batch in the data/vgg_batches/ directory.
Each batch contains the input, output, weights, biases, a description of the parameters for each layer, and the labels for the images.



##Running an Example

We have provided an example with PCIe connecting to a convolution layer and an fc layer directly through PCIe. These modules are using only off-chip memory
to communicate with the host. (is this the best way to do it?)
We have verified functionality of the fully-connected layer (conv-layer is untested).
We have also provided a driver application that sets up the control registers of the fully-connected layer.

First you need to build the hls ip that will be imported into the partial region.


``make pr``  
- This will create the vivado_hls projects in vivado_hls_proj,  and create a new application in a partial region using the hypervisor provided.
           

You can look at the generated slave_axi registers to see which offsets you need to program the control registers.
For example for the fc look at ./hls_proj/fc_proj/solution1/impl/verilog/fc_layer_CTRL_BUS_s_axi.v

Look at the Makefile to see the tests. To build for example a sample conv_layer in hardware it is. 
make 
``./hw_conv_layer``

This writes control registers, copies data into the off-chip memory, starts the application and reads data out. 


##Creating Your Own Application

Run the following:
``make pr_modify``

This does the same thing as the first half of our ``make pr``. This imports the hypervisor dcp, and makes the example we provided. This then opens the block diagram in the GUI. You can modify the block diagram to import your own IPs. Remember to also modify your addresses for your interconnect in Address Editor.
Afterwards you will run the following in the tcl console within vivado:

``source 8v3_shell/create_pr2_1.tcl`` 

This will create the wrapper, and synthesize the bitstream. The bitstream generated will be in ./8v3_shell/pr_region_test_pblock_partial.bit. 



##Programming The FPGA

To program the FPGA you will need to use the program script provided in /opt/util/progutil/program.sh

You will give it two arguments, the first being your partial bitstream and the second being the clear bitstream. You will see both in the 8v3_shell directory.
It is important you give both bitstreams and you use this script to program the partial regions or there is a chance the FPGA gets stuck in a bad state.

Programming template:
``/opt/util/prog_util/program.sh <partial.bit file> <partial_clear.bit file>``


##Verifying Accuracy

The above test creates a file in the data directory for each batch layer called dma_out. 
The nn_params/softMax.py script pumps the data out of the last fully connected layer  through a softmax and compares the results with the label information.
This will give you an accuracy result of your network. 
