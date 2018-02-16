#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <map>
#include <string>
#include "conv_layer.h"
#include "util/shared.h"

#define HW_CTRL_ADDR 0x00000000

using namespace std;

int main()
{
  float * dma_input;
  float * gold_outputs;
  float * outputs;

  cout << "Starting Test " << endl;
  string imageDir = "nn_params/conv3_3";
  map<string, int> layer_params = readParams(imageDir + "/params");

  // Read inputs
  // Inputs are packed together as weights, biases and input values
  // Allocate enough space for outputs
  if (readRawFile(imageDir + "/dma_in",
                  dma_input,
                  layer_params["input_dim"]*layer_params["output_dim"]*
                  layer_params["kernel_size"]*layer_params["kernel_size"]+
                  layer_params["output_dim"]+
                  layer_params["input_dim"]*layer_params["input_width"]*
                  layer_params["input_height"]*layer_params["batch_size"],
                  MAX_WEIGHT_SIZE+MAX_OUTPUT_DIMS+MAX_BATCH*MAX_CONV_INPUT+
                  MAX_BATCH*MAX_CONV_OUTPUT))
    return 1;
  // Read gold outputs
  if (readRawFile(imageDir + "/output",
                  gold_outputs,
                  layer_params["output_dim"]*layer_params["output_width"]*
                  layer_params["output_height"]*layer_params["batch_size"],
                  MAX_BATCH*MAX_CONV_OUTPUT))
    return 1;

  int num_outputs = layer_params["output_dim"]*layer_params["output_width"]*
                    layer_params["output_height"];
  int num_inputs = layer_params["input_dim"]*layer_params["input_width"]*
                   layer_params["input_height"];
  int num_weights = layer_params["input_dim"]*layer_params["output_dim"]*
                    layer_params["kernel_size"]*layer_params["kernel_size"];
  int num_biases = layer_params["output_dim"];

  // very basic input checking
  if (layer_params["input_dim"] > MAX_INPUT_DIMS ||
      layer_params["input_width"] > MAX_INPUT_WIDTH ||
      layer_params["input_height"] > MAX_INPUT_WIDTH ||
      layer_params["output_dim"] > MAX_OUTPUT_DIMS ||
      layer_params["output_width"] > MAX_OUTPUT_WIDTH ||
      layer_params["output_height"] > MAX_OUTPUT_WIDTH ||
      layer_params["batch_size"] > MAX_BATCH)
  {
    cerr << "Problem with layer params\n";
    return 1;
  } else {
    int b = layer_params["batch_size"];
    int od = layer_params["output_dim"];
    int ox = layer_params["output_width"];
    int oy = layer_params["output_height"];
    int id = layer_params["input_dim"];
    int ix = layer_params["input_width"];
    int iy = layer_params["input_height"];
    int k = layer_params["kernel_size"];
    int s = layer_params["stride"];

    cout << "Begin Test\n"
       << "Batch Size: " << b << endl
       << "Num Inputs: " << num_inputs << endl
       << "Num Outputs: " << num_outputs << endl
       << "Num Weights: " << num_weights << endl 
       << "Num Biases: " << num_biases << endl 
       << "Input Dimensions " << b << " x " << id << " x " << ix << " x " << iy << endl
       << "Output Dimensions " << b << " x " << od << " x " << ox << " x " << oy << endl
       << "Kernel Dimensions " << od << " x " << id << " x " << k << " x " << k << endl
       << "Stride Size: " << s << endl;
 
    // Output Offset
    outputs = dma_input + b*num_inputs+num_biases+num_weights;

    // Run Accelerator
    #ifdef HW_TEST
    hw_conv_layer(HW_CTRL_ADDR, dma_input, 0,
                  sizeof(float)*(b*num_inputs+num_biases + num_weights),
                  b, od, ox, oy, id, ix, iy, s, k);
    #else
    conv_layer(dma_input, 0, sizeof(float)*(b*num_inputs+num_biases + num_weights),
               b, od, ox, oy, id, ix, iy, s, k);
    #endif

    std::cout << "DONE" << std::endl;
    // Check outputs
    float total = 0.0f;
    for (int i = 0; i < b*num_outputs; i++)
    {
      float err = fabs(outputs[i] - gold_outputs[i]);
      total += err*err;
    }
    float avg_error = total/(b *num_outputs);
    cout << "Mean Square Error " << avg_error << endl;
  }

  return 0;
}

