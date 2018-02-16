#ifndef _SHARED_H
#define _SHARED_H

#include <string>
#include <vector>
#include <map>

#define MAX_BATCH 10
#define NUM_BIAS_DIMENSIONS 1
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
