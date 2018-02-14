#include <algorithm>
#include <float.h>
#include <ap_utils.h>
#include "conv_layer.h"

void conv_layer(
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
	            const int k)
{
#pragma HLS INTERFACE m_axi port=mem depth=2147483648
#pragma HLS INTERFACE s_axilite port=b bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=od bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=ox bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=oy bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=id bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=ix bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=iy bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=s bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=k bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=input_offset
#pragma HLS INTERFACE s_axilite port=output_offset
#pragma HLS INTERFACE s_axilite port=return bundle=CTRL_BUS

  int num_weights = id*od*k*k;
  int num_biases = od;
  int num_input = b*id*ix*iy;
  int num_output = b*od*ox*oy;

//  float weights[MAX_INPUT_DIMS * MAX_OUTPUT_DIMS * MAX_KERNEL_SIZE * MAX_KERNEL_SIZE];
//  float biases[MAX_OUTPUT_DIMS];
//  float input[MAX_CONV_INPUT*MAX_BATCH];
//  float output[MAX_CONV_OUTPUT*MAX_BATCH];

//  volatile float * weights = (mem + input_offset/sizeof(float));
//  volatile float * biases = (mem + input_offset/sizeof(float) + num_weights);
//  volatile float * input = (mem + input_offset/sizeof(float) + num_weights + num_biases);
//  volatile float * output = (mem + output_offset/sizeof(float));

  //memcpy(weights, mem + input_offset/sizeof(float), num_weights*sizeof(float)); 
  //memcpy(biases, mem + (input_offset/sizeof(float)) + num_weights, num_biases*sizeof(float)); 
  //memcpy(input, mem + (input_offset/sizeof(float)) + (num_biases+num_weights), num_input*sizeof(float)); 



  
  // Batch

  for (int b_=0; b_< b; b_++)
#pragma HLS loop_tripcount min=1 max=10
#pragma HLS UNROLL
  {
    // Output Dimensions (Feature Maps)
    for (int o_d = 0; o_d < od; o_d++)
#pragma HLS loop_tripcount min=1 max=256
    {
      // Output Y Dimension
      for (int o_y = 0; o_y < oy; o_y++)
#pragma HLS loop_tripcount min=1 max=1024
      {
        // Output X Dimension
        for (int o_x = 0; o_x < ox; o_x++)
#pragma HLS loop_tripcount min=1 max=1024
        {
          // Set bias
	  //float output_element = *(mem+input_offset/sizeof(float) + num_weights + o_d); 
	  float output_element = FLT_MAX * (-1.0); 
//	  float output_element = FLT_MAX * (-1.0); 
//          output[b_*od*ox*oy +o_d*ox*oy + o_y*ox + o_x] = biases[o_d];

          // Weighted Sum:
   
          // Input Dimensions (Feature Maps)
          for (int i_d = 0; i_d < id; i_d++)
#pragma HLS loop_tripcount min=1 max=256
          {
            // Input Y Dimension
            for (int i_y = o_y*s, iiy = 0; i_y < o_y*s+k; i_y++, iiy++)
#pragma HLS loop_tripcount min=1 max=1024
            {
              // Input X Dimension
              for (int i_x = o_x*s, iix = 0; i_x < o_x*s+k; i_x++, iix++)
              {
#pragma HLS loop_tripcount min=1 max=1024
#pragma HLS PIPELINE
		//output_element += 
                //       *(mem+input_offset/sizeof(float) + num_weights+num_biases + b_*id*ix*iy + i_d*ix*iy + i_y*ix + i_x)  *
                //       *(mem+input_offset/sizeof(float) + o_d*id*k*k + i_d*k*k + iiy*k + iix);
                output_element = std::max(output_element,  *(mem+input_offset/sizeof(float) +  b_*id*ix*iy + i_d*ix*iy + i_y*ix + i_x));
		//output[b_*od*ox*oy + o_d*ox*oy + o_y*ox + o_x] += 
                //       (input[b_*id*ix*iy + i_d*ix*iy + i_y*ix + i_x] *
                //       weights[o_d*id*k*k + i_d*k*k + iiy*k + iix]);
              }
            }
          }

          // Activaton Function
	  //*(mem+output_offset/sizeof(float) + b_*od*ox*oy + o_d*ox*oy + o_y*ox + o_x) = std::max(0.0f, output_element);
	  *(mem+output_offset/sizeof(float) + b_*od*ox*oy + o_d*ox*oy + o_y*ox + o_x) = output_element;
//          output[b_*od*ox*oy + o_d*ox*oy + o_y*ox + o_x] = 
//                 std::max(0.0f, output_element);
        }
      }
    }
  }

    
//  memcpy(mem + (output_offset/sizeof(float)), output, num_output*sizeof(float)); 
}

