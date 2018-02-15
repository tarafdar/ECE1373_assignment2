#ifndef _FC_LAYER_H
#define _FC_LAYER_H

#define MAX_BATCH 10
#define MAX_INPUT_SIZE 25088 
#define MAX_OUTPUT_SIZE 4096
#define MAX_WEIGHT_SIZE MAX_INPUT_SIZE*MAX_OUTPUT_SIZE

void fc_layer(
              float * mem,
	      int input_offset,
	      int output_offset, 
              const int batch_size,
              const int num_inputs,
              const int num_outputs,
              const int enable_relu);

#endif
