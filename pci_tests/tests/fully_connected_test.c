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
/* ltoh: little to host */
/* htol: little to host */
#if __BYTE_ORDER == __LITTLE_ENDIAN
#  define ltohl(x)       (x)
#  define ltohs(x)       (x)
#  define htoll(x)       (x)
#  define htols(x)       (x)
#elif __BYTE_ORDER == __BIG_ENDIAN
#  define ltohl(x)     __bswap_32(x)
#  define ltohs(x)     __bswap_16(x)
#  define htoll(x)     __bswap_32(x)
#  define htols(x)     __bswap_16(x)
#endif
  
#define FATAL do { fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n", __LINE__, __FILE__, errno, strerror(errno)); exit(1); } while(0)
 
#define MAP_SIZE (1024UL*1024UL)
#define MAP_MASK (MAP_SIZE - 1)

static void timespec_sub(struct timespec *t1, const struct timespec *t2)
{
  assert(t1->tv_nsec >= 0);
  assert(t1->tv_nsec < 1000000000);
  assert(t2->tv_nsec >= 0);
  assert(t2->tv_nsec < 1000000000);
  t1->tv_sec -= t2->tv_sec;
  t1->tv_nsec -= t2->tv_nsec;
  if (t1->tv_nsec >= 1000000000)
  {
    t1->tv_sec++;
    t1->tv_nsec -= 1000000000;
  }
  else if (t1->tv_nsec < 0)
  {
    t1->tv_sec--;
    t1->tv_nsec += 1000000000;
  }
}

int main(int argc, char **argv) {
  int fd;
  volatile void *map_base;
  volatile  void *virt_addr; 
  uint32_t writeval;
  off_t target;


  char * ctrl_device = "/dev/xdma0_user";
  char * dma_to_device = "/dev/xdma0_h2c_0";
  char * dma_from_device = "/dev/xdma0_c2h_0";
  

  int ctrl_fd =  open(ctrl_device, O_RDWR | O_SYNC);
  map_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, ctrl_fd, 0);
  printf("Memory mapped at address %p.\n", map_base); 


  target = 0x10000; //address of fully connected layer in the interconnect 

  virt_addr = map_base + target +0x10; // first argument (input offset)
  *((uint32_t *) virt_addr) = 0x0;
  
  virt_addr = map_base + target + 0x18; // second argument (output offset)
  *((uint32_t *) virt_addr) = 0x1000000;
  
  virt_addr = map_base + target + 0x20; // third argument (batch)
  *((uint32_t *) virt_addr) = 10;
  
  virt_addr = map_base + target + 0x28; // fourth argument (num of inputs)
  *((uint32_t *) virt_addr) = 4096;
  
  virt_addr = map_base + target + 0x30; // fifth argument (num of outputs)
  *((uint32_t *) virt_addr) = 1000;

  char *in_buffer = NULL;
  char *allocated = NULL;
  int size = 16551840; //total size to dma_in 
  
  posix_memalign((void **)&allocated, 4096/*alignment*/, size + 4096);
  in_buffer = allocated;
  
  //write input
  int input_file_fd =  open("fc8/dma_in", O_RDONLY);
  read(input_file_fd, in_buffer, size);

  int dma_to_device_fd = open(dma_to_device, O_RDWR);
  uint32_t addr = 0; //input_offset 0
//  in_buffer = malloc(size);
  lseek(dma_to_device_fd, addr, SEEK_SET);
  write(dma_to_device_fd, in_buffer, size);

  free(in_buffer); 
  //start device
  struct timespec ts_start, ts_end;
  uint32_t * read_result;
  posix_memalign((void **)&allocated, 4096/*alignment*/, sizeof(uint32_t) + 4096);
  read_result = allocated;

  
  virt_addr = map_base + target; 
  writeval = 0x1; 
  writeval = htoll(writeval);          
  *((uint32_t *) virt_addr) = writeval;
  clock_gettime(CLOCK_MONOTONIC, &ts_start);

  //poll on done
  do{
  	//read_result = *((uint32_t *) virt_addr);
  	read(ctrl_fd, read_result, sizeof(uint32_t));
  	*read_result = ltohl(*read_result);
	printf("read_result %x\n", *read_result);
  }while((((*read_result) >> 2 && 0x1) == 0x0) && (((*read_result) >> 1 && 0x1) == 0x0));


  free(read_result);
  clock_gettime(CLOCK_MONOTONIC, &ts_end);
  timespec_sub(&ts_end, &ts_start);
  printf("CLOCK_MONOTONIC reports %ld.%09ld seconds (total) for core\n", ts_end.tv_sec, ts_end.tv_nsec);


  // read output
  char *out_buffer = NULL;
  int output_file_fd = open("dma_out.file", O_RDWR | O_CREAT | O_TRUNC | O_SYNC, 0666);

  int dma_from_device_fd  = open(dma_from_device, O_RDWR | O_NONBLOCK);
 

  size = 40000;
  addr = 0x4000000; //offset address 

  //out_buffer = malloc(size);
  posix_memalign((void **)&allocated, 4096/*alignment*/, size + 4096);
  out_buffer = allocated;
  lseek(dma_from_device_fd, addr, SEEK_SET);

  read(dma_from_device_fd, out_buffer, size);
  int rc = write(output_file_fd, out_buffer, size);
  printf("rc %d\n", rc);

  free(out_buffer);
  close(dma_from_device_fd);
  close(dma_to_device_fd);
  close(input_file_fd);
  close(output_file_fd);
}


