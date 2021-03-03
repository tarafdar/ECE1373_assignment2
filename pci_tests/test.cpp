
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include "../util/shared.h"

using namespace std;
int main()
{

  const char * dma_to_device = "/dev/xdma0_h2c_0";
  const char * dma_from_device = "/dev/xdma0_c2h_0";

  char* in_buffer = NULL;
  char* allocated = NULL;
  int size = 100;
  int buff_arr[100];
  for(int i=0; i<100; i++)
	buff_arr[i] = i;




  // Create aligned memory alloc for DMA (should do this during initial read) 
  posix_memalign((void **)&allocated, 4096/*alignment*/, size*sizeof(float) + 4096);
  in_buffer = allocated;
  memcpy(in_buffer, (void*)buff_arr, size*sizeof(int));
  printf("Copied input to buffer\n");

  int dma_to_device_fd = open(dma_to_device, O_RDWR | O_NONBLOCK);
  uint32_t addr = 0;
  lseek(dma_to_device_fd, addr, SEEK_SET);
  write(dma_to_device_fd, (char*)in_buffer, size*sizeof(int));
  free(in_buffer); 
  printf("Wrote inputs via DMA\n");



  char *out_buffer = NULL;
  int dma_from_device_fd  = open(dma_from_device, O_RDWR | O_NONBLOCK);
  posix_memalign((void **)&allocated, 4096/*alignment*/, size*sizeof(int) + 4096);
  out_buffer = allocated;
  lseek(dma_from_device_fd, 0, SEEK_SET);
  read(dma_from_device_fd, out_buffer, size*sizeof(int));
  printf("Outputs read via DMA\n");
  
 
  bool err = false;

  int * out_ptr = (int *) out_buffer;

  int index = -1;
  for(int i=0; i<100; i++){
	if(buff_arr[i] != out_ptr[i]){
		err = true;
		break;
	}
  }


  if(err) {
	printf("ERROR: Output did not match input\n");
	for (int i=0; i<100; i++){
		printf("INPUT[%d]: %d, OUTPUT[%d]: %d\n", i, buff_arr[i], i, out_ptr[i]);

	}
  }
  else
	printf("Success: Output matched input\n");


}




