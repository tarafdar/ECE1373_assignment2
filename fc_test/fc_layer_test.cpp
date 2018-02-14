#include <iostream>
#include <fstream>
#include <cmath>
#include <cassert>
#include "fc_layer.h"
#include "util/shared.h"

using namespace std;

int main()
{
  float * weights;
  float * biases;
  float * inputs;
  float * gold_outputs;
  float * outputs;
  int retval = 0;
  
  std::string imageDir = "nn_params/fc3/";
  vector<int> input_size = readFile(imageDir+"input", inputs, MAX_BATCH*MAX_INPUT_SIZE);
  vector<int> output_size = readFile(imageDir+"output", gold_outputs, MAX_BATCH*MAX_OUTPUT_SIZE);
  vector<int> weight_size = readFile(imageDir+"weights", weights, MAX_OUTPUT_SIZE*MAX_INPUT_SIZE);
  vector<int> bias_size = readFile(imageDir+"biases", biases, MAX_OUTPUT_SIZE);

  assert(input_size.size() == 2);
  assert(output_size.size() == 2);
  assert(weight_size.size() == 2);
  assert(bias_size.size() == 1);

  int batch_size = input_size[0];
  int num_inputs = input_size[1];
  int num_outputs = output_size[1];

  // Do some input checking
  if (num_inputs > MAX_INPUT_SIZE || num_outputs > MAX_OUTPUT_SIZE ||
      input_size[0] != output_size[0] ||
      batch_size > MAX_BATCH || 
      weight_size[0] != num_outputs || weight_size[1] != num_inputs ||
      bias_size[0] != num_outputs)
  {
    cerr << "Problem with input files\n";
    retval = 1;
  }
  else
  {
    cout << "Begin Test\n"
         << "Batch Size: " << batch_size << "\n"
         << "Num Inputs: " << input_size[1] << "\n"
         << "Num Outputs: " << output_size[1] << "\n"
         << "Num Weights: " << weight_size[0]*weight_size[1] << "\n"
         << "Num Biases: " << bias_size[0] << endl;

    // Allocate space for accelerator outputs
    outputs = new float[MAX_BATCH*MAX_OUTPUT_SIZE];

    // Run Accelerator
    fc_layer(weights, biases,
             inputs, outputs,
             batch_size, num_inputs, num_outputs);

    // Check outputs
    float total = 0.0f;
    for (int i = 0; i < batch_size*num_outputs; i++)
    {
      float err = fabs(outputs[i] - gold_outputs[i]);
      total += err*err;
    }

    float avg_error = total/(batch_size*num_outputs);
    cout << "Mean Squared Error " << avg_error << endl;
  }

  return retval;
}
