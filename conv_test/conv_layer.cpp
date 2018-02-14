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
//  float input[MAX_CONV_INPUT*MAX_BATCH];
//  float output[MAX_CONV_OUTPUT*MAX_BATCH];
 

  int num_weights = id*od*k*k;
  int num_biases = od;
  int num_input = b*id*ix*iy;
  int num_output = b*od*ox*oy;
 
//  memcpy(input, mem + input_offset/sizeof(float), num_input*sizeof(float)); 
  // Batch
  for (int b_=0; b_< b; b_++)
  {
    // Output Dimensions (Feature Maps)
    for (int o_d = 0; o_d < od; o_d++)
    {
      // Output Y Dimension
      for (int o_y = 0; o_y < oy; o_y++)
      {
        // Output X Dimension
        for (int o_x = 0; o_x < ox; o_x++)
        {
          // Set bias 
	  float output_element = *(mem+input_offset/sizeof(float) + num_weights + o_d); 
          //output[b_*od*ox*oy +o_d*ox*oy + o_y*ox + o_x] = FLT_MAX*(-1.0);

          // Weighted Sum:
   
          // Input Dimensions (Feature Maps)
          for (int i_d = 0; i_d < id; i_d++)
          {
            // Input Y Dimension
            for (int i_y = o_y*s, iiy = 0; i_y < o_y*s+k; i_y++, iiy++)
            {
              // Input X Dimension
              for (int i_x = o_x*s, iix = 0; i_x < o_x*s+k; i_x++, iix++)
              {
		output_element += 
                       *(mem+input_offset/sizeof(float) + num_weights+num_biases + b_*id*ix*iy + i_d*ix*iy + i_y*ix + i_x)  *
                       *(mem+input_offset/sizeof(float) + o_d*id*k*k + i_d*k*k + iiy*k + iix);


                    
              }
            }
          }
	  *(mem + output_offset/sizeof(float) + b_*od*ox*oy + o_d*ox*oy + oy*ox + o_x) = std::max(0.0f, output_element);

        }
      }
    }
  }
//  memcpy(mem + (output_offset/sizeof(float)), output, num_output*sizeof(float)); 
}

