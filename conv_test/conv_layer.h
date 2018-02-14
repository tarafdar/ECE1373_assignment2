
#ifndef _CONV_LAYER_H
#define _CONV_LAYER_H

// Limits
#define MAX_BATCH 10
#define MAX_KERNEL_SIZE 7
#define MAX_INPUT_DIMS 3
#define MAX_OUTPUT_DIMS 64
#define MAX_INPUT_WIDTH 230
#define MAX_INPUT_HEIGHT 230
#define MAX_OUTPUT_WIDTH 112
#define MAX_OUTPUT_HEIGHT 112
#define MAX_CONV_INPUT MAX_INPUT_DIMS*MAX_INPUT_WIDTH*MAX_INPUT_HEIGHT 
#define MAX_CONV_OUTPUT MAX_OUTPUT_DIMS*MAX_OUTPUT_WIDTH*MAX_OUTPUT_HEIGHT 

void conv_layer(//float weights[MAX_INPUT_DIMS * MAX_OUTPUT_DIMS * MAX_KERNEL_SIZE * MAX_KERNEL_SIZE],
                //float biases[MAX_OUTPUT_DIMS],
                //float input[MAX_CONV_INPUT*MAX_BATCH],
                //float output[MAX_CONV_OUTPUT*MAX_BATCH],
		    float * mem,
		    int input_offset,
		    int output_offset,
	            const int b,
	            const int od,
	            const int ox,
	            const int oy,
	            const int id,
	            const int ix,
	            const int iy,
	            const int s,
	            const int k);
#endif
