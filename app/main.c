#include <stdio.h>
#include <stdlib.h>
#include "../src/lab.h"
#include <stdlib.h>
#include <sys/time.h> /* for gettimeofday system call */



int main(int argc, char **argv)
{
  if (argc < 3)
  {
    printf("usage: %s <array_size> <num_threads>\n", argv[0]);
    return 1;
  }

  // Parse and validate array size
  int size = atoi(argv[1]);
  if (size <= 0)
  {
    printf("Array size must be positive\n");
    return 1;
  }

  // Parse and validate thread count
  int t = atoi(argv[2]);
  if (t <= 0)
  {
    printf("Number of threads must be positive\n");
    return 1;
  }
  if (t > MAX_THREADS)
  {
    printf("Warning: Reducing thread count to maximum allowed (%d)\n", MAX_THREADS);
    t = MAX_THREADS;
  }

  // Allocate and initialize array
  int *A_ = malloc(sizeof(int) * size);
  if (A_ == NULL)
  {
    printf("Failed to allocate memory\n");
    return 1;
  }

  // Initialize with random values
  srandom(1);  // Fixed seed for reproducibility
  for (int i = 0; i < size; i++)
    A_[i] = random() % 100000;

  // Time the sort
  double end = 0;
  double start = getMilliSeconds();
  mergesort_mt(A_, size, t);
  end = getMilliSeconds();
  
  // Output in required format
  printf("%f %d\n", end-start, t);

  // Clean up
  free(A_);
  return 0;
}


