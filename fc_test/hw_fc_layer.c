#include <stdint.h>
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <byteswap.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/unistd.h>

#include "util/shared.h"
#include "xfc_layer_hw.h"

#define MAP_SIZE (1024UL*1024UL)

void hw_fc_layer(int target,               // control offset
                 float * mem,              // global memory pointer 
	         int input_offset,         // offset of inputs
	         int output_offset,        // offset of outputs
                 const int batch_size,     // batch size
                 const int num_inputs,     // number of input (dimensions)
                 const int num_outputs,    // number of output (dimensions)
                 const int enable_relu)    // flag to enable/disable ReLU
{
  volatile void* map_base;

  const char * ctrl_device = "/dev/xdma0_user";
  const char * dma_to_device = "/dev/xdma0_h2c_0";
  const char * dma_from_device = "/dev/xdma0_c2h_0";

  // Setup control registers
  int ctrl_fd =  open(ctrl_device, O_RDWR | O_SYNC);
  map_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, ctrl_fd, 0);
  printf("Memory mapped at address %p.\n", map_base); 

  // Write arguments
  write_int(map_base, target + XFC_LAYER_CTRL_BUS_ADDR_INPUT_OFFSET_DATA, input_offset); 
  write_int(map_base, target + XFC_LAYER_CTRL_BUS_ADDR_OUTPUT_OFFSET_DATA, output_offset);
  write_int(map_base, target + XFC_LAYER_CTRL_BUS_ADDR_BATCH_SIZE_DATA, batch_size);
  write_int(map_base, target + XFC_LAYER_CTRL_BUS_ADDR_NUM_INPUTS_DATA, num_inputs);
  write_int(map_base, target + XFC_LAYER_CTRL_BUS_ADDR_NUM_OUTPUTS_DATA, num_outputs);
  write_int(map_base, target + XFC_LAYER_CTRL_BUS_ADDR_ENABLE_RELU_DATA, enable_relu);

  char* in_buffer = NULL;
  char* allocated = NULL;
  // Size to DMA in
  int size = batch_size*num_inputs+num_outputs+num_inputs*num_outputs;
  int wait_time = 0;
 
  // Create aligned memory alloc for DMA (should do this during initial read) 
  posix_memalign((void **)&allocated, 4096/*alignment*/, size*sizeof(float) + 4096);
  in_buffer = allocated;
  memcpy(in_buffer, (void*)mem, size*sizeof(float));
  printf("Copied input to buffer\n");

  // DMA input values
  int dma_to_device_fd = open(dma_to_device, O_RDWR | O_NONBLOCK);
  uint32_t addr = input_offset;
  lseek(dma_to_device_fd, addr, SEEK_SET);
  write(dma_to_device_fd, (char*)in_buffer, size*sizeof(float));
  free(in_buffer); 
  printf("Wrote inputs via DMA\n");

  struct timespec ts_start, ts_end;
  clock_gettime(CLOCK_MONOTONIC, &ts_start);
  write_int(map_base, target, 0x1);
  do {
    sleep(1);
    printf("\rSleep wait %d", wait_time++);
    fflush(stdout);
  }
  while (!(read_int(map_base, target) & 0xe));
  clock_gettime(CLOCK_MONOTONIC, &ts_end);
  timespec_sub(&ts_end, &ts_start);
  printf("CLOCK_MONOTONIC reports %ld.%09ld seconds (total) for core\n", ts_end.tv_sec, ts_end.tv_nsec);
  

  // DMA outputs back
  char *out_buffer = NULL;
  int dma_from_device_fd  = open(dma_from_device, O_RDWR | O_NONBLOCK);
  int out_size = batch_size*num_outputs;

  posix_memalign((void **)&allocated, 4096/*alignment*/, out_size*sizeof(float) + 4096);
  out_buffer = allocated;
  lseek(dma_from_device_fd, output_offset, SEEK_SET);
  read(dma_from_device_fd, out_buffer, out_size*sizeof(float));
  printf("Outputs read via DMA\n");

  // copy results to result buffer
  memcpy((void*)(mem+size), out_buffer, out_size*sizeof(float));
  free(out_buffer);
  close(dma_from_device_fd);
  close(dma_to_device_fd);
}


