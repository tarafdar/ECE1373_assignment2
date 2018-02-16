#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <map>
#include <string>
#include "conv_layer.h"
#include "util/shared.h"
#include <sstream>
#include <assert.h>
#include <ctime>
#include <chrono>
using namespace std;


//#define PRINT





int run_single_test(string imageDir, map<string, int> layer_params, float * &dma_input, float * gold_outputs){
  
  


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

#ifdef PRINT
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
#endif

    // Run Accelerator
    conv_layer(dma_input, 0, sizeof(float)*(b*num_inputs+num_biases + num_weights),
               b, od, ox, oy, id, ix, iy, s, k);

  }

  return 0;

}


int main()
{

  string imageRootDir = "data/vgg_batches/batch_";
  //string imageRootDir = "../data/vgg_batches/batch_";
  int numBatches = 2;
  string layer = "conv3_3";
  string imageDir;
  ostringstream ss;
  float total_error = 0.0;
  cout << "Reading Input for " << numBatches << " batches" <<  endl;

  vector<map<string, int> > batch_layer_params = readBatchParams(imageRootDir, numBatches, layer);
  vector<float *> dma_input_vec;
  vector<float *> gold_outputs_vec;
  if(readInputBatches(imageRootDir, batch_layer_params, numBatches, layer, MAX_WEIGHT_SIZE+MAX_OUTPUT_DIMS+MAX_BATCH*MAX_CONV_INPUT+MAX_BATCH*MAX_CONV_OUTPUT, dma_input_vec, true))
	return 1;
  if(readOutputBatches(imageRootDir, batch_layer_params, numBatches, layer, MAX_BATCH*MAX_CONV_OUTPUT, gold_outputs_vec, true)) return 1;


  cout << "Starting Test with " << numBatches << " batches" <<  endl;


  auto start = chrono::system_clock::now(); 
  for(int i=0; i<numBatches; i++){
    ss << i;
#ifdef PRINT
    cout << "Running batch" << i << endl;
#endif
    imageDir = imageRootDir + ss.str() + "/" + layer;
    
    if(run_single_test(imageDir, batch_layer_params[i], dma_input_vec[i], gold_outputs_vec[i])!=0)
	return 1;
  }
  auto end = chrono::system_clock::now(); 
  auto elapsed = end - start;

  float avg_error = get_mean_squared_error_and_write_file(dma_input_vec, gold_outputs_vec, numBatches, batch_layer_params, imageRootDir, layer, true);

  cout << "Mean Square Error " << avg_error << endl;
  cout << "Computation took  " << chrono::duration_cast<chrono::seconds> (elapsed).count() << " seconds" << endl;
  std::cout << "DONE" << std::endl;
  return 0;
}

