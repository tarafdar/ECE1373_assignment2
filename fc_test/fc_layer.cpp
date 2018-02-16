#include <algorithm>
#include "fc_layer.h"

void fc_layer(float * mem,
              int input_offset,
              int output_offset,
              const int batch_size,
              const int num_inputs,
              const int num_outputs,
              const int enable_relu)
{
#pragma HLS INTERFACE m_axi port=mem depth=2147483648
#pragma HLS INTERFACE s_axilite port=input_offset bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=output_offset bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=batch_size bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=num_inputs bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=num_outputs bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=enable_relu bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=return bundle=CTRL_BUS

  const int num_weights = num_inputs*num_outputs;
  const int num_biases =  num_outputs;

  for (int b = 0; b < batch_size; b++) {

    // Output Node Iterator
    for (int o = 0; o < num_outputs; o++) {

      // Set bias
      float output_element = mem[input_offset/sizeof(float) + num_weights + o];
        
      // Accumulate weighted sum
      for (int i = 0; i < num_inputs; i++) {
        float input_element = mem[input_offset/sizeof(float) + num_weights + num_biases + b*num_inputs+i];
        float weight_element = mem[input_offset/sizeof(float) + o*num_inputs+i];
        output_element += input_element * weight_element;
      }

      // Compute activation
      if (enable_relu)
         mem[output_offset/sizeof(float) + b*num_outputs+o] = std::max(0.0f, output_element);
      else
         mem[output_offset/sizeof(float) + b*num_outputs+o] = output_element;
    }
  }
}
