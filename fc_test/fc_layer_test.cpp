#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <map>
#include <string>
#include "fc_layer.h"
#include "util/shared.h"

#define HW_CTRL_ADDR 0x00010000

using namespace std;

int main()
{
  float * dma_input;
  float * gold_outputs;
  float * outputs;

  cout << "Starting Test " << endl;
  string imageDir = "nn_params/fc7";
  map<string, int> layer_params = readParams(imageDir + "/params");

  // Read inputs
  // Inputs are packed together as weights, biases and input values
  // Allocate enough space for outputs
  if (readRawFile(imageDir + "/dma_in",
                  dma_input,
                  layer_params["input_dim"]*layer_params["output_dim"]+
                  layer_params["output_dim"]+
                  layer_params["batch_size"]*layer_params["input_dim"],
                  MAX_WEIGHT_SIZE+MAX_OUTPUT_SIZE+
                  MAX_BATCH*MAX_INPUT_SIZE+
                  MAX_BATCH*MAX_OUTPUT_SIZE))
    return 1;
  // Read gold outputs
  if (readRawFile(imageDir + "/output",
                  gold_outputs,
                  layer_params["batch_size"]*layer_params["output_dim"],
                  MAX_BATCH*MAX_OUTPUT_SIZE))
    return 1;

  int num_outputs = layer_params["output_dim"];
  int num_inputs = layer_params["input_dim"];
  int num_weights = layer_params["input_dim"]*layer_params["output_dim"];
  int num_biases = layer_params["output_dim"];

  // very basic input checking
  if (layer_params["input_dim"] > MAX_INPUT_SIZE ||
      layer_params["output_dim"] > MAX_OUTPUT_SIZE ||
      layer_params["batch_size"] > MAX_BATCH)
  {
    cerr << "Problem with layer params\n";
    return 1;
  } else {
    int b = layer_params["batch_size"];
    int er = layer_params["enable_relu"];

    cout << "Begin Test\n"
       << "Batch Size: " << b << endl
       << "Num Inputs: " << num_inputs << endl
       << "Num Outputs: " << num_outputs << endl
       << "Enable ReLU: " << er << endl;
 
    // Output Offset
    outputs = dma_input + b*num_inputs+num_biases+num_weights;

    // Run Accelerator
    #ifdef HW_TEST
    hw_fc_layer(HW_CTRL_ADDR, dma_input, 0,
                sizeof(float)*(b*num_inputs+num_biases + num_weights),
                b, num_inputs, num_outputs, er);
    #else
    fc_layer(dma_input, 0, sizeof(float)*(b*num_inputs+num_biases + num_weights),
             b, num_inputs, num_outputs, er);
    #endif
   

    std::cout << "DONE" << std::endl;
    // Check outputs
    float total = 0.0f;
    for (int i = 0; i < b*num_outputs; i++)
    {
      //cout << outputs[i] << " != " << gold_outputs[i] << endl;
      float err = fabs(outputs[i] - gold_outputs[i]);
      total += err*err;
    }
    float avg_error = total/(b *num_outputs);
    cout << "Mean Square Error " << avg_error << endl;
  }

  return 0;
}

