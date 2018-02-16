#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <stdint.h>
#include <assert.h>

#include "shared.h"

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

using namespace std;

std::map<std::string, int> readParams(const std::string fname)
{
  std::map<std::string, int> params;
  ifstream in_file(fname.c_str(), ios::in);
  std::string key, svalue;
  int ivalue;

  // Warning: don't do much error checking
  while (in_file >> key)
  {
    if (!key.compare("type")) {
      in_file >> svalue;
      if (!svalue.compare("Convolution"))
        params[key] = Convolution;
      else if (!svalue.compare("InnerProduct"))
        params[key] = InnerProduct;
      else if (!svalue.compare("Pooling"))
        params[key] = Pooling;
      else {
        cerr << "Invalid Layer Type!\n";
        return std::map<std::string, int>();
      }
    } else if (!key.compare("name")) { 
      in_file >> svalue;
      std::cout << "Parsing " << svalue << std::endl;
    }
    else {
       in_file >> ivalue;
       params[key] = ivalue;
    }
  }
  in_file.close();
  std::cout << "DONE\n";
  return params;
}
int readRawFile(const string fname,
                              float *& fptr,
                              const int read_alloc,
                              const int max_alloc)
{
  int retval = 0;

  std::cout << "Reading: " << fname << " size: " << read_alloc << std::endl;
  ifstream in_file(fname.c_str(), ios::in | ios::binary);
  if (in_file.is_open())
  {
    fptr = new float[max_alloc];
    if (read_alloc <= max_alloc) {
      if (!in_file.read(reinterpret_cast<char*>(&fptr[0]), sizeof(float)*read_alloc))
      {
    	  cout << "Read Error" << endl;
          retval = 1;
      }
    } else {
      cerr << "Desired dimensions too large: " << read_alloc << " > " << max_alloc << "\n";
      retval = 1;
    }
    in_file.close();
  }
  else
    cerr << "Couldn't open file: " << fname << endl;

  if (retval) delete [] fptr;
  return retval;
}

std::vector <int> readFile(const string fname,
                     float *& fptr,
                     const int max_alloc)
{
  int* read_sizes = NULL;

  std::vector<int> read_sizes_vec;
  ifstream in_file(fname.c_str(), ios::in | ios::binary);
  if (in_file.is_open())
  {
    // Read header
    int read_dims;
    in_file.read(reinterpret_cast<char*>(&read_dims), sizeof(int));
    fptr = new float[max_alloc];
    read_sizes = new int[read_dims];

    // Read dimension data
    in_file.read(reinterpret_cast<char*>(read_sizes),
                 sizeof(int)*read_dims);

    // Read the array
    // Use a hack to read dma data:
    int read_alloc = 1;
    if (read_dims == 9) {
    	cout << "Reading DMA Input" << endl;
    	read_alloc = read_sizes[0]*read_sizes[1] *   // weights
    			     read_sizes[2]*read_sizes[3] +
    			     read_sizes[4] +                 // biases
					 read_sizes[5]*read_sizes[6] *   // inputs
					 read_sizes[7]*read_sizes[8];
    	for (int i = 0; i < read_dims; i++)
    	  read_sizes_vec.push_back(read_sizes[i]);
    }
    else
    {
    	cout << "READING NORMAL INPUT" << endl;
      read_alloc = 1;
      for (int i = 0; i < read_dims; i++){
    	cout << i << " = " << read_sizes[i] << endl;
        read_alloc *= read_sizes[i];
        read_sizes_vec.push_back(read_sizes[i]);
      }
    }
    cout << "READ ALLOC " << read_alloc << " for " << read_dims << endl;
    if (read_alloc <= max_alloc) {
      if (!in_file.read(reinterpret_cast<char*>(&fptr[0]), sizeof(float)*read_alloc))
      {
    	  cout << "Read Error" << endl;
    	  delete [] read_sizes;
    	  delete [] fptr;
    	  read_sizes = NULL;
      }
    }
    else
    {
      cerr << "Desired dimensions too large: " << read_alloc << " > " << max_alloc << "\n";
      delete [] read_sizes;
      delete [] fptr;
      read_sizes = NULL;
    }

    for (int i = 0; i < 10; i++)
    	cout << fptr[i] << endl;
    in_file.close();
  }
  else
    cerr << "Couldn't open file: " << fname << endl;

  delete [] read_sizes;
  return read_sizes_vec;
}

void write_int(volatile void* map_base, int offset, int value)
{
  volatile void* virt_addr = (volatile void*)((char*)map_base + offset); 
  *((uint32_t *) virt_addr) = htoll(value);
}
int read_int(volatile void* map_base, int offset)
{
  volatile void* virt_addr = (volatile void*)((char*)map_base + offset); 
  return ltohl(*((uint32_t *) virt_addr));
}

void timespec_sub(struct timespec *t1, const struct timespec *t2)
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
