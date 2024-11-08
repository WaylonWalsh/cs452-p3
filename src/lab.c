#include <stdlib.h>
#include <sys/time.h> /* for gettimeofday system call */
#include "lab.h"

/**
 * @brief Standard insertion sort that is faster than merge sort for small array's
 *
 * @param A The array to sort
 * @param p The starting index
 * @param r The ending index
 */
static void insertion_sort(int A[], int p, int r)
{
  int j;

  for (j = p + 1; j <= r; j++)
    {
      int key = A[j];
      int i = j - 1;
      while ((i > p - 1) && (A[i] > key))
        {
	  A[i + 1] = A[i];
	  i--;
        }
      A[i + 1] = key;
    }
}


void mergesort_s(int A[], int p, int r)
{
  if (r - p + 1 <=  INSERTION_SORT_THRESHOLD)
    {
      insertion_sort(A, p, r);
    }
  else
    {
      int q = (p + r) / 2;
      mergesort_s(A, p, q);
      mergesort_s(A, q + 1, r);
      merge_s(A, p, q, r);
    }

}

void merge_s(int A[], int p, int q, int r)
{
  int *B = (int *)malloc(sizeof(int) * (r - p + 1));

  int i = p;
  int j = q + 1;
  int k = 0;
  int l;

  /* as long as both lists have unexamined elements */
  /*  this loop keeps executing. */
  while ((i <= q) && (j <= r))
    {
      if (A[i] < A[j])
        {
	  B[k] = A[i];
	  i++;
        }
      else
        {
	  B[k] = A[j];
	  j++;
        }
      k++;
    }

  /* now only at most one list has unprocessed elements. */
  if (i <= q)
    {
      /* copy remaining elements from the first list */
      for (l = i; l <= q; l++)
        {
	  B[k] = A[l];
	  k++;
        }
    }
  else
    {
      /* copy remaining elements from the second list */
      for (l = j; l <= r; l++)
        {
	  B[k] = A[l];
	  k++;
        }
    }

  /* copy merged output from array B back to array A */
  k = 0;
  for (l = p; l <= r; l++)
    {
      A[l] = B[k];
      k++;
    }

  free(B);
}

double getMilliSeconds()
{
  struct timeval now;
  gettimeofday(&now, (struct timezone *)0);
  return (double)now.tv_sec * 1000.0 + now.tv_usec / 1000.0;
}

void *parallel_mergesort(void *args) {
    struct parallel_args *pargs = (struct parallel_args *)args;
    mergesort_s(pargs->A, pargs->start, pargs->end);
    return NULL;
}

void merge_sections(int *A, int start, int mid, int end) {
    // Merge two sorted sections of array A
    merge_s(A, start, mid, end);
}

void mergesort_mt(int *A, int n, int num_threads) {
    // Limit number of threads to MAX_THREADS
    if (num_threads > MAX_THREADS) {
        num_threads = MAX_THREADS;
    }
    
    // If only one thread requested or array is small, just do sequential sort
    if (num_threads <= 1 || n <= INSERTION_SORT_THRESHOLD) {
        mergesort_s(A, 0, n - 1);
        return;
    }

    // Calculate chunk size for each thread
    int chunk_size = n / num_threads;
    struct parallel_args args[MAX_THREADS];
    int i;

    // Create threads and assign array chunks
    for (i = 0; i < num_threads - 1; i++) {
        args[i].A = A;
        args[i].start = i * chunk_size;
        args[i].end = (i + 1) * chunk_size - 1;
        pthread_create(&args[i].tid, NULL, parallel_mergesort, &args[i]);
    }

    // Last thread handles remaining elements
    args[i].A = A;
    args[i].start = i * chunk_size;
    args[i].end = n - 1;
    pthread_create(&args[i].tid, NULL, parallel_mergesort, &args[i]);

    // Wait for all threads to complete
    for (i = 0; i < num_threads; i++) {
        pthread_join(args[i].tid, NULL);
    }

    // Merge the sorted chunks
    // We'll use a bottom-up approach to merge chunks
    for (int size = chunk_size; size < n; size = size * 2) {
        for (int left = 0; left < n - size; left += size * 2) {
            int mid = left + size - 1;
            int right = (left + size * 2 - 1) < (n - 1) ? (left + size * 2 - 1) : (n - 1);
            merge_sections(A, left, mid, right);
        }
    }
}