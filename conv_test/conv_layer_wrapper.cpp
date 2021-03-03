#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <map>
#include <string>
#include "conv_layer.h"
#include "util/shared.h"
#include <sstream>
#include <chrono>

#define HW_CTRL_ADDR 0x00000000

using namespace std;


//#define PRINT





int main()
{







	int _b = 1;
	int _od = 8;
	int _ox = 8;
	int _oy = 8;
	int _id = 8;
	int _ix = 8;
	int _iy = 8;
	int _s = 0;
	int _k = 3;



	int target = HW_CTRL_ADDR;


	int num_weights = _id*_od*_k*_k;
	int num_biases = _od;
	int num_inputs = _b*_id*_ix*_ix; 
	int num_outputs = _b*_id*_ix*_ix; 

//TODO 
// PREPARE MEMORY
// CURRENTLY IN FORM OF BIASES THEN WEIGHTS THEN INPUT THEN OUTPUT
//FEEL FREE TO CHANGE ACCORDINGLY
  	int dma_in_size = _id*_od*_k*_k+_od+_id*_ix*_iy*_b;
  	int dma_out_size = _od*_oy*_ox*_b;
	float * mem;
	mem = new float[dma_in_size+dma_out_size];

    // Run Accelerator
    #ifdef HW_TEST
    hw_conv_layer(HW_CTRL_ADDR, mem, 0,
                  sizeof(float)*(_b*num_inputs+num_biases + num_weights),
                  _b, _od, _ox, _oy, _id, _ix, _iy, _s, _k);
    #else
    conv_layer(mem, 0, sizeof(float)*(_b*num_inputs+num_biases + num_weights),
               _b, _od, _ox, _oy, _id, _ix, _iy, _s, _k);
    #endif


}

