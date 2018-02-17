#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include "fc_layer.h"
#include <assert.h>
#include <chrono>
#include "util/shared.h"

#define HW_CTRL_ADDR 0x00010000

using namespace std;

#define PRINT
int run_single_test(string imageDir, map<string, int> layer_params, float *dma_input, float * gold_outputs)
{


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

#ifdef PRINT
    cout << "Begin Test\n"
       << "Batch Size: " << b << endl
       << "Num Inputs: " << num_inputs << endl
       << "Num Outputs: " << num_outputs << endl
       << "Enable ReLU: " << er << endl;
#endif

    // Run Accelerator
    #ifdef HW_TEST
    hw_fc_layer(HW_CTRL_ADDR, dma_input, 0,
                sizeof(float)*(b*num_inputs+num_biases + num_weights),
                b, num_inputs, num_outputs, er);
    #else
    fc_layer(dma_input, 0, sizeof(float)*(b*num_inputs+num_biases + num_weights),
             b, num_inputs, num_outputs, er);
    #endif
   

  }

  return 0;
}


int main()
{

  string imageRootDir = "data/vgg_batches/batch_";
//  string imageRootDir = "../data/vgg_batches/batch_";
  int numBatches = 10;
  string layer = "fc8";
  string imageDir;
  cout << "Starting Test with " << numBatches << " batches" <<  endl;

  vector<map<string, int> > batch_layer_params = readBatchParams(imageRootDir, numBatches, layer);
  vector<float *> dma_input_vec;
  vector<float *> gold_outputs_vec;
  if(readInputBatches(imageRootDir, batch_layer_params, numBatches, layer, MAX_WEIGHT_SIZE+MAX_OUTPUT_SIZE+MAX_BATCH*MAX_INPUT_SIZE+MAX_BATCH*MAX_OUTPUT_SIZE, dma_input_vec, false))
	return 1;
  if(readOutputBatches(imageRootDir, batch_layer_params, numBatches, layer, MAX_BATCH*MAX_OUTPUT_SIZE, gold_outputs_vec, false)) return 1;

  auto start = chrono::system_clock::now(); 
  for(int i=0; i<numBatches; i++){
    
    ostringstream ss;
    ss << i;
   
#ifdef PRINT
    cout << "Running batch" << i << endl;
#endif
    imageDir = imageRootDir + ss.str() + "/" + layer;
    cout << "ImageDir is " << imageDir << endl;  
    if(run_single_test(imageDir, batch_layer_params[i], dma_input_vec[i], gold_outputs_vec[i])!=0)
	return 1;
  }
  auto end = chrono::system_clock::now(); 
  auto elapsed = end - start;

  float avg_error = get_mean_squared_error_and_write_file(dma_input_vec, gold_outputs_vec, numBatches, batch_layer_params, imageRootDir, layer, false);
  
  cout << "Mean Square Error " << avg_error << endl;
  cout << "Computation took  " << chrono::duration_cast<chrono::milliseconds> (elapsed).count() << " ms" << endl;
  std::cout << "DONE" << std::endl;
  return 0;
}

