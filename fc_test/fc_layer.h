#ifndef _FC_LAYER_H
#define _FC_LAYER_H

#define MAX_BATCH 10
#define MAX_INPUT_SIZE 4096 
#define MAX_OUTPUT_SIZE 4096

void fc_layer(
              float * mem,
	      int input_offset,
	      int output_offset, 
              const int batch_size,
              const int num_inputs,
              const int num_outputs);

#endif
