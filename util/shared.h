#ifndef _SHARED_H
#define _SHARED_H

#include <string>
#include <vector>
#include <map>

#define MAX_BATCH 10
#define NUM_BIAS_DIMENSIONS 1
float get_mean_squared_error_and_write_file(std::vector<float *> mem, std::vector <float *> golden_output, int numBatches, std::vector<std::map<std::string, int> >, std::string imageRootDir, std::string layer, bool conv );
std::vector<std::map<std::string, int> > readBatchParams(std::string imageRootDir, int numBatches, std::string layer);
int readInputBatches(std::string imageRootDir, std::vector<std::map<std::string, int> > batch_layer_params, int numBatches, std::string layer, const int max_alloc, std::vector<float *> &ptr, bool conv);
int readOutputBatches(std::string imageRootDir, std::vector<std::map<std::string, int> > batch_layer_params, int numBatches, std::string layer, const int max_alloc, std::vector<float *> &ptr, bool conv);

std::vector<int> readFile(const std::string fname,
                          float *& fptr,
                          const int max_alloc);
int readRawFile(const std::string fname,
                float *& fptr,
                const int read_alloc,
                const int max_alloc);
std::map<std::string, int> readParams(const std::string fname);
enum LayerTypes {Convolution, Pooling, InnerProduct};
void timespec_sub(struct timespec *t1, const struct timespec *t2);
int read_int(volatile void* map_base, int offset);
void write_int(volatile void* map_base, int offset, int value);
#endif
