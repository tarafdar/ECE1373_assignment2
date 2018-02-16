#ifndef _FC_LAYER_H
#define _FC_LAYER_H

#define MAX_BATCH 10
#define MAX_INPUT_SIZE 25088 
#define MAX_OUTPUT_SIZE 4096
#define MAX_WEIGHT_SIZE MAX_INPUT_SIZE*MAX_OUTPUT_SIZE

void fc_layer(float * mem,              // global memory pointer 
	      int input_offset,         // offset of inputs
	      int output_offset,        // offset of outputs
              const int batch_size,     // batch size
              const int num_inputs,     // number of input (dimensions)
              const int num_outputs,    // number of output (dimensions)
              const int enable_relu);   // flag to enable/disable ReLU
void hw_fc_layer(int target,               // control offset
                 float * mem,              // global memory pointer 
	         int input_offset,         // offset of inputs
	         int output_offset,        // offset of outputs
                 const int batch_size,     // batch size
                 const int num_inputs,     // number of input (dimensions)
                 const int num_outputs,    // number of output (dimensions)
                 const int enable_relu);   // flag to enable/disable ReLU
#endif
