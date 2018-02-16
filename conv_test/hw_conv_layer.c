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
#include "xconv_layer_hw.h"

#define MAP_SIZE (1024UL*1024UL)

void hw_conv_layer(int target,             // control address in the system
                   float * mem,            // global memory pointer
                   int input_offset,       // offset of inputs
                   int output_offset,      // offset of outputs
                   const int b,            // batch size
                   const int od,           // output dimensions
                   const int ox,           // output width
                   const int oy,           // output height
                   const int id,           // input dimensions
                   const int ix,           // input width
                   const int iy,           // input height
                   const int s,            // stride
                   const int k)            // kernel size
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
  write_int(map_base, target + XCONV_LAYER_CTRL_BUS_ADDR_INPUT_OFFSET_DATA, input_offset); 
  write_int(map_base, target + XCONV_LAYER_CTRL_BUS_ADDR_OUTPUT_OFFSET_DATA, output_offset);
  write_int(map_base, target + XCONV_LAYER_CTRL_BUS_ADDR_B_DATA, b);
  write_int(map_base, target + XCONV_LAYER_CTRL_BUS_ADDR_OD_DATA, od);
  write_int(map_base, target + XCONV_LAYER_CTRL_BUS_ADDR_OX_DATA, ox);
  write_int(map_base, target + XCONV_LAYER_CTRL_BUS_ADDR_OY_DATA, oy);
  write_int(map_base, target + XCONV_LAYER_CTRL_BUS_ADDR_ID_DATA, id);
  write_int(map_base, target + XCONV_LAYER_CTRL_BUS_ADDR_IX_DATA, ix);
  write_int(map_base, target + XCONV_LAYER_CTRL_BUS_ADDR_IY_DATA, iy);
  write_int(map_base, target + XCONV_LAYER_CTRL_BUS_ADDR_S_DATA, s);
  write_int(map_base, target + XCONV_LAYER_CTRL_BUS_ADDR_K_DATA, k);

  char* in_buffer = NULL;
  char* allocated = NULL;
  // Size to DMA in
  int size = id*od*k*k+od+id*ix*iy*b;
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
  int out_size = od*oy*ox*b;

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


