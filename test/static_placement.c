#include <errno.h>
#include <execinfo.h>
#include <fcntl.h>
#include <libsyscall_intercept_hook_point.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <syscall.h>
#include <time.h>
#include <unistd.h>
#define _GNU_SOURCE
#include <pthread.h>
#define SIZE 4096
#include <numa.h>
#include <numaif.h>
#include <sys/mman.h>
#include <sys/resource.h>
#define MAX_ 2000
#define RECORD
#define PAGE_SIZE 4096
// #define RECORD_ENABLED
// #define ALL_DRAM

FILE *g_fp = NULL;
char *g_line = NULL;
size_t len = 0;
pthread_mutex_t count_mutex;
struct to_DRAM
{
  // long int mmap_start;
  long int heat_start;
  long int heat_end;
};
struct to_DRAM globalArray[MAX_];

void redirect_stdout(char *filename)
{
  int fd;
  if ((fd = open(filename, O_CREAT | O_WRONLY, 0666)) < 0)
  {
    perror(filename);
    exit(1);
  }
  close(1);
  if (dup(fd) != 1)
  {
    fprintf(stderr, "Unexpected dup failure\n");
    exit(1);
  }
  close(fd);

  g_fp = fopen("call_stack.txt", "w+");
  if (g_fp == NULL)
  {
    printf("Error when try to use fopen!!\n");
  }
}
void inspect_heats(char *filename)
{
  FILE *file = fopen(filename, "r");
  if (file == NULL)
  {
    printf("Failed to open the file.\n");
    exit(1);
  }
  char line[55];
  int counter = 0;
  while (fgets(line, sizeof(line), file) != NULL)
  {
    // printf("%s", line);
    // fprintf(stderr, "%s", line);
    char first[15];
    char second[15];
    char third[15];
    char *token;
    token = strtok(line, ",");
    if (token != NULL)
    {
      strcpy(first, token);
      globalArray[counter].heat_start = atol(first);
      token = strtok(NULL, ",");
      if (token != NULL)
      {
        strcpy(second, token);
        globalArray[counter].heat_end = atol(second);
        // token = strtok(NULL, ",");
        // if (token != NULL) {
        //   strcpy(third, token);
        //   globalArray[counter].heat_end = atol(third);
        // }
      }
    }
    counter++;
  }
  fclose(file);
}

long int hash(char *word)
{
  unsigned int hash = 0;
  for (int i = 0; word[i] != '\0'; i++)
  {
    hash = 31 * hash + word[i];
  }
  // return hash % TABLE_SIZE;
  return abs(hash);
}

void get_call_stack(int size_allocation, char *call_stack_concat)
{
  int static mmap_id = 0;
  int nptrs;
  void *buffer[SIZE];
  ssize_t read;
  char *addr;
  char size[20] = "";
  int j;
  char **strings;

  nptrs = backtrace(buffer, SIZE);
  // fprintf(stderr, "nptrs: %d\n", nptrs);
  backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO);
  fflush(stdout);

  // int i; // callstack_line_index;
  // int k=0;
  // // const char* substring = getenv("APP");
  // const char* substring = "mmap_test";

  // sprintf(size, ":%d", size_allocation);

  // char *p;
  // // g_fp = fopen("call_stack.txt", "r");
  // // while ((read = getline(&g_line, &len, g_fp)) != -1) {
  // for(int callstack_line_index=0; callstack_line_index < nptrs;
  // callstack_line_index++){
  //     read = getline(&g_line, &len, g_fp);

  //     p = strstr(g_line,substring);
  //     if(p){
  //         for(i=0;i<len;i++)
  //         {
  //             if(g_line[i] == '[')
  //             {
  //                 break;
  //             }
  //         }
  //         for(i=i+1; i<len;i++)
  //         {
  //             if(g_line[i] ==']')
  //                 break;
  //             call_stack_concat[k] = g_line[i];
  //             k++;
  //         }
  //         call_stack_concat[k] = ':';
  //         k++;
  //         // fprintf(stderr, "tmp call_stack_concat: %s\n",
  //         call_stack_concat);
  //    }
  // }
  // call_stack_concat[k-1] = '\0';
  // strcat(call_stack_concat, size);
  // fprintf(stderr, "cleaned call_stack_concat: %s\n", call_stack_concat);
}

// long int align_to_page(long int address)
// {
//   long int page_mask = ~(PAGE_SIZE - 1);
//   return (address + (PAGE_SIZE - 1)) & page_mask;
// }

int calculateIntersectedAreaNotAligned(long int A, long int B, long int C, long int D, long int *start, long int *end)
{
  if (A <= D && C <= B)
  {
    // has intersection
    *start = (A > C) ? A : C;
    *end = (B < D) ? B : D;
  }
  int intersectedArea = (int)(*end - *start);
  if (intersectedArea > 0)
  {
    return intersectedArea;
  }
  else
  {
    return 0;
  }
}

long align_to_page(long num)
{
  long remainder = num % PAGE_SIZE;
  if (remainder >= PAGE_SIZE / 2)
    return num + (PAGE_SIZE - remainder);
  else
    return num - remainder;
}

int calculateIntersectedArea_(long int A, long int B, long int C, long int D, long int *start, long int *end,
                              long int *rest_start1, long int *rest_size1, long int *rest_start2, long int *rest_size2)
{
  // fprintf(stderr, "A: %ld\n", A);
  int intersectedArea = 0;
  if (C <= A && A < D && D <= B)
  { // case 1
    fprintf(stderr, "1st case\n");
    *start = (A);
    *end = (D);
    if (*end > (B))
    {
      *end = (B);
    }
    else
    {
      *rest_start2 = *end;
      *rest_size2 = (B) - *end; // need to make sure size2 is multiple of 4096
    }
    *rest_start1 = 0;
    *rest_size1 = 0;
  }
  else if (A <= C && C < D && D <= B)
  { // case 2
    fprintf(stderr, "2nd case\n");
    *start = (C);
    *end = (D);
    if (*start < (A))
    {
      *start = (A);
    }
    else
    {
      *rest_start1 = (A);
      *rest_size1 = *start - (A);
    }
    if (*end > (B))
    {
      *end = (B);
    }
    else
    {
      *rest_start2 = *end;
      *rest_size2 = (B) - *end;
    }
  }
  else if (A <= C && C < B && B <= D)
  { // case 3
    fprintf(stderr, "3rd case\n");
    *start = (C);
    *end = (B);
    if (*start < (A))
    {
      *start = (A);
    }
    else
    {
      *rest_start1 = (A);
      *rest_size1 = *start - (A);
    }
    *rest_start2 = 0;
    *rest_size2 = 0;
  }
  else if (C <= A && A < B && B <= D)
  { // case 4
    fprintf(stderr, "4th case\n");
    *start = (A);
    *end = (B);
    *rest_start1 = 0;
    *rest_size1 = 0;
    *rest_start2 = 0;
    *rest_size2 = 0;
  }
  else
  {
    // fprintf(stderr, "no intersection found\n");
    *rest_start1 = 0;
    *rest_size1 = 0;
    *rest_start2 = 0;
    *rest_size2 = 0;
    return 0; // no intersection, return 0
  }
  // if to DRAM region already exceeds mmapped-end, no rest regions to CXL needed
  // if (*end > align_to_page(B))
  // {
  //   *end = align_to_page(B);
  //   *rest_start2 = 0;
  //   *rest_size2 = 0;
  // }
  intersectedArea = (int)(*end - *start);
  // sanity check, intersectedArea must be multiple of PAGE_SIZE
  fprintf(stderr, "start: %ld, end: %ld, range: %d\n", *start, *end, intersectedArea);
  if (align_to_page(*start) != *start)
  {
    perror("mmap start does not align with page size, fail!\n");
    return 0;
  }
  if (align_to_page(intersectedArea) != (long int)intersectedArea)
  {
    perror("length does not align with page size, fail!\n");
    return 0;
  }
  if (intersectedArea > 0)
  {
    return intersectedArea;
  }
  else
  {
    *rest_start1 = 0;
    *rest_size1 = 0;
    *rest_start2 = 0;
    *rest_size2 = 0;
    return 0;
  }
}

// int try_static_allocate(long int start_addr, long int end_addr, long *result, long whole_size, char *malloc_type)
// {
//   unsigned long g_nodemask;
//   long int potential_to_DRAM_start, potential_to_DRAM_end;
//   int potential_intersection;
//   for (int i = 0; i < MAX_; i++)
//   {
//     if (globalArray[i].heat_start != 0)
//     {
//       potential_intersection = calculateIntersectedArea_(start_addr, end_addr, globalArray[i].heat_start, globalArray[i].heat_end, &potential_to_DRAM_start, &potential_to_DRAM_end);
//       if (potential_intersection)
//       {
//         g_nodemask = 1;
//         if (mbind((void *)potential_to_DRAM_start, potential_intersection, MPOL_BIND, &g_nodemask, sizeof(g_nodemask), 0) != 0)
//         {
//           g_nodemask = 2;
//           if (mbind((void *)*result, whole_size, MPOL_BIND, &g_nodemask, sizeof(g_nodemask), 0) != 0)
//           {
//             perror("mbind failed in try_static_alloc");
//             return 1;
//           }
//           fprintf(stderr, "fallbacknode1,%ld,%s\n", start_addr, malloc_type);
//           return 0;
//         }
//         fprintf(stderr, "node0,%ld,%d,%s\n", start_addr, potential_intersection, malloc_type);
//         return 0;
//       }
//     }
//   }
//   return -1;
// }

static int hook(long syscall_number, long arg0, long arg1, long arg2, long arg3,
                long arg4, long arg5, long *result)
{
  int static mmap_id = 0;
  // fprintf(stderr, "syscall_number: %ld\n", syscall_number);
  // fprintf(stderr, "arg0: %ld\n", arg0);
  struct timespec ts;
  char call_stack_concat[SIZE] = "";
  unsigned long g_nodemask;
  // pthread_mutex_init(&count_mutex, NULL);
  // printf("%ld\n", syscall_number);
  if (syscall_number == SYS_mmap)
  {
    *result = syscall_no_intercept(syscall_number, arg0, arg1, arg2, arg3, arg4,
                                   arg5);

    long int start_addr = (long int)(void *)*result;
    long int end_addr = start_addr + arg1;
    long start_addr_aligned = align_to_page(start_addr);
    long end_addr_aligned = align_to_page(end_addr);
    long int potential_to_DRAM_start, potential_to_DRAM_end, rest_start1, rest_size1, rest_start2, rest_size2;
    int potential_intersection;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    fprintf(stderr, "%ld.%ld,mmap,%ld,%p,%ld\n", ts.tv_sec, ts.tv_nsec,
            start_addr, (void *)*result, arg1);
#ifdef RECORD_ENABLED
    return 0; // follow memory allocatio affinity by `numactl`
#endif
#ifdef ALL_DRAM
    goto to_mmap_all;
#endif
    if (globalArray[0].heat_start == 0)
    {
      // fprintf(stderr, "recording...\n");
      // fprintf(stderr, "no heats found, mmap to node1,%ld\n", start_addr);
      // g_nodemask = 2;
      // if (mbind((void *)*result, arg1, MPOL_BIND, &g_nodemask, sizeof(g_nodemask), 0) != 0) {
      //     perror("mbind for failed 1");
      //     return 1;
      // }
      goto to_mmap_all;
      // return 0;
    }
    else
    { // static place mmaped region
      for (int i = 0; i < MAX_; i++)
      {
        if (globalArray[i].heat_start != 0)
        {
          // pthread_mutex_lock(&count_mutex);
          potential_intersection = calculateIntersectedArea_(start_addr_aligned, end_addr_aligned, globalArray[i].heat_start, globalArray[i].heat_end, &potential_to_DRAM_start, &potential_to_DRAM_end, &rest_start1, &rest_size1, &rest_start2, &rest_size2);
          // pthread_mutex_unlock(&count_mutex);
          if (potential_intersection)
          {
            g_nodemask = 1;
            fprintf(stderr, "mmap has intersection from %ld to %ld, length %d\n", potential_to_DRAM_start, potential_to_DRAM_end, potential_intersection);
            if (mbind((void *)potential_to_DRAM_start, (unsigned long)potential_intersection, MPOL_BIND, &g_nodemask, sizeof(g_nodemask), MPOL_MF_MOVE) != 0)
            {
              fprintf(stderr, "mbind mmap fallback to cxl\n");
              goto to_mmap_all;
              // g_nodemask = 2;
              // perror("mbind mmap failed temp to node0");
              // if (mbind((void *)*result, (unsigned long)arg1, MPOL_BIND, &g_nodemask, sizeof(g_nodemask), MPOL_MF_MOVE) != 0)
              // {
              //   perror("mbind for failed mmap1");
              //   return 1;
              // }
              // return 0;
            }
            fprintf(stderr, "node0mmap,%ld,%d\n", start_addr_aligned, potential_intersection);
            // binding rest to CXL
            if (rest_size1 != 0)
            {
              fprintf(stderr, "trying to bind size1 %ld from addr %ld\n", rest_size1, rest_start1);
              g_nodemask = 2;
              if (mbind((void *)rest_start1, (unsigned long)rest_size1, MPOL_BIND, &g_nodemask, sizeof(g_nodemask), MPOL_MF_MOVE) != 0)
              {
                perror("mbind rest1 failed mmap");
              }
            }
            if (rest_size2 != 0)
            {
              fprintf(stderr, "trying to bind size2 %ld from addr %ld\n", rest_size2, rest_start2);
              g_nodemask = 2;
              if (mbind((void *)rest_start2, (unsigned long)rest_size2, MPOL_BIND, &g_nodemask, sizeof(g_nodemask), MPOL_MF_MOVE) != 0)
              {
                perror("mbind rest2 failed mmap");
              }
            }
            return 0;
          }
          potential_to_DRAM_start = 0;
          potential_to_DRAM_end = 0;
          rest_start1 = 0;
          rest_size1 = 0;
          rest_start2 = 0;
          rest_size2 = 0;
        }
      }
    to_mmap_all:
#ifdef ALL_DRAM
      g_nodemask = 1;
#else
      g_nodemask = 2;
#endif
      if (start_addr_aligned >= end_addr_aligned)
      {
        fprintf(stderr, "shouldn't be here, fail\n");
        return 0;
      }
#ifdef ALL_DRAM
      g_nodemask = 1;
      fprintf(stderr, "node0mmap,%ld,%ld\n", start_addr_aligned, arg1);
#else
      g_nodemask = 2;
      fprintf(stderr, "not detected, mmap to node1,%ld\n", start_addr_aligned);
#endif
      if (mbind((void *)start_addr, (unsigned long)(arg1), MPOL_BIND, &g_nodemask, sizeof(g_nodemask), MPOL_MF_MOVE) != 0)
      {
        perror("mbind for failed mmap2");
        return 1;
      }
      return 0;
      // }
    }

    // g_nodemask = 2;
    // unsigned long lower_bound = 140732705907789;
    // unsigned long upper_bound = 140732747783694;
    // unsigned long lower_bound_2 = 140732901328679;
    // unsigned long upper_bound_2 = 140732943204584;
    // unsigned long lower_bound_3 = 140737270381434;
    // void *lower_bound_addr = (void *)lower_bound;
    // void *upper_bound_addr = (void *)upper_bound;
    // void *lower_bound_addr_2 = (void *)lower_bound_2;
    // void *upper_bound_addr_2 = (void *)upper_bound_2;
    // void *lower_bound_addr_3 = (void *)lower_bound_3;

    // if (((void *)*result >= lower_bound_addr && (void *)*result <= upper_bound_addr) ||
    //     ((void *)*result >= lower_bound_addr_2 && (void *)*result <= upper_bound_addr_2) ||
    //     (void *)*result >= lower_bound_addr_3) {
    //   fprintf(stderr, "gotcha!! Moving you back to local DRAM\n");
    //   g_nodemask = 1;
    // } else {
    //   fprintf(stderr, "You are staying in CXL\n");
    //   g_nodemask = 2;
    // }

    // if (mbind((void *)*result, (unsigned long)arg1, MPOL_BIND, &g_nodemask,
    //           sizeof(g_nodemask), MPOL_MF_MOVE) == -1) {
    //   fprintf(stderr, "Error:%d\n", errno);
    //   perror("Error description");
    // }

    // return 0;
  }
  else if (syscall_number == SYS_munmap)
  {
    if (globalArray[0].heat_start == 0)
    {
    }
    *result = syscall_no_intercept(syscall_number, arg0, arg1, arg2, arg3, arg4, arg5);
    clock_gettime(CLOCK_MONOTONIC, &ts);
    long int start_addr = (long int)(void *)arg0;
    fprintf(stderr, "%ld.%ld,munmap,%ld,%p,%ld\n", ts.tv_sec, ts.tv_nsec,
            start_addr, (void *)arg0, arg1);
    return 0;
  }
  // else if (syscall_number == SYS_alloc_hugepages){
  //     /* pass it on to the kernel */
  //     *result = syscall_no_intercept(syscall_number, arg0, arg1, arg2, arg3,
  //     arg4, arg5); clock_gettime(CLOCK_REALTIME, &ts); fprintf(stderr,
  //     "%ld.%ld, alloc_hp,%p,%ld\n", ts.tv_sec,ts.tv_nsec, (void *)arg0,
  //     arg1); return 0;
  // }
  else if (syscall_number == SYS_brk)
  {
    // void* init_brk_addr = sbrk(0);
    long int initial_brk_dec = (long int)sbrk(0);
    fprintf(stderr, "init brk:%ld\n", initial_brk_dec);
    /* pass it on to the kernel */
    // char *malloc_type = "brk";
    *result = syscall_no_intercept(syscall_number, arg0, arg1, arg2, arg3, arg4, arg5);
    clock_gettime(CLOCK_MONOTONIC, &ts);
    long int new_addr = (long int)(void *)arg0;
    long int start_addr = new_addr - arg1;
    fprintf(stderr, "%ld.%ld,brk,%ld,%ld,%ld\n", ts.tv_sec, ts.tv_nsec,
            start_addr, arg1, new_addr);
    long start_addr_aligned = align_to_page(start_addr);
    long end_addr_aligned = align_to_page(new_addr);
#ifdef RECORD_ENABLED
    return 0; // follow memory allocatio affinity by `numactl`
#endif
#ifdef ALL_DRAM
    goto to_brk_all;
#endif
    if (globalArray[0].heat_start == 0)
    {
      // do something??

      goto to_brk_all;
      // return 0;
    }
    else
    {
      // fprintf(stderr, "brk all calls to node0,%ld\n", start_addr);
      // g_nodemask = 1;
      // if (mbind((void *)start_addr, arg1, MPOL_BIND, &g_nodemask, sizeof(g_nodemask), 0) != 0) {
      //     perror("mbind brk to node0 failed");
      //     return 1;
      // }
      // return 0;

      long int potential_to_DRAM_start, potential_to_DRAM_end;
      long int rest_start1, rest_size1, rest_start2, rest_size2;
      int potential_intersection;
      for (int i = 0; i < MAX_; i++)
      {
        if (globalArray[i].heat_start != 0)
        {
          // pthread_mutex_lock(&count_mutex);
          potential_intersection = calculateIntersectedArea_(start_addr_aligned, end_addr_aligned, globalArray[i].heat_start, globalArray[i].heat_end, &potential_to_DRAM_start, &potential_to_DRAM_end, &rest_start1, &rest_size1, &rest_start2, &rest_size2);
          // pthread_mutex_unlock(&count_mutex);
          if (potential_intersection)
          {
            g_nodemask = 1;
            fprintf(stderr, "brk has intersection from %ld to %ld, length %d\n", potential_to_DRAM_start, potential_to_DRAM_end, potential_intersection);
            if (mbind((void *)potential_to_DRAM_start, (unsigned long)potential_intersection, MPOL_BIND, &g_nodemask, sizeof(g_nodemask), MPOL_MF_MOVE) != 0)
            {
              fprintf(stderr, "mbind brk fallback to node1\n");
              goto to_brk_all;
              // g_nodemask = 2;
              // long size_aligned = align_to_page(arg1);
              // if (size_aligned == 0)
              // {
              //   return 0; // by default -> cpu0 -> DRAM
              // }
              // if (mbind((void *)start_addr_aligned, (unsigned long)size_aligned, MPOL_BIND, &g_nodemask, sizeof(g_nodemask), MPOL_MF_MOVE) != 0)
              // {
              //   perror("mbind failed brk1");
              //   return 1;
              // }
              // fprintf(stderr, "fallbacknode1brk,%ld,%ld\n", start_addr_aligned, arg1);
              // return 0;
            }
            fprintf(stderr, "node0brk,%ld,%d\n", start_addr_aligned, potential_intersection);
            // binding rest to CXL
            if (rest_size1 != 0)
            {
              fprintf(stderr, "trying to bind size1 %ld from addr %ld\n", rest_size1, rest_start1);
              g_nodemask = 2;
              if (mbind((void *)rest_start1, (unsigned long)rest_size1, MPOL_BIND, &g_nodemask, sizeof(g_nodemask), MPOL_MF_MOVE) != 0)
              {
                perror("mbind rest1 failed brk");
              }
            }
            if (rest_size2 != 0)
            {
              fprintf(stderr, "trying to bind size2 %ld from addr %ld\n", rest_size2, rest_start2);
              g_nodemask = 2;
              if (mbind((void *)rest_start2, (unsigned long)rest_size2, MPOL_BIND, &g_nodemask, sizeof(g_nodemask), MPOL_MF_MOVE) != 0)
              {
                perror("mbind rest2 failed brk");
              }
            }
            return 0;
          }
          potential_to_DRAM_start = 0;
          potential_to_DRAM_end = 0;
        }
      }
      // if (arg1 == 0) {
      //   fprintf(stderr, "bind whatever you to node0,%ld\n", start_addr);
      //   g_nodemask = 1;
      //   if (mbind((void *)start_addr, (unsigned long)arg1, MPOL_BIND, &g_nodemask, sizeof(g_nodemask), MPOL_MF_MOVE) != 0) {
      //       perror("mbind failed brk2");
      //       return 1;
      //   }
      // }
    to_brk_all:
#ifdef ALL_DRAM
      g_nodemask = 1;
#else
      g_nodemask = 2;
#endif
      // start_addr = align_to_page(start_addr);
      if (start_addr_aligned >= end_addr_aligned) // bind previous page to cxl
      {
        long start_addr_prev = start_addr_aligned - PAGE_SIZE;

#ifdef ALL_DRAM
        g_nodemask = 1;
        fprintf(stderr, "node0brk,%ld,%d\n", start_addr_prev, 4096);
#else
        g_nodemask = 2;
        fprintf(stderr, "brked too small, binding previous addr,%ld\n", (start_addr_prev));
#endif
        if (mbind((void *)(start_addr_prev), (unsigned long)PAGE_SIZE, MPOL_BIND, &g_nodemask, sizeof(g_nodemask), MPOL_MF_MOVE) != 0)
        {
          perror("mbind failed brk aaa");
          return 1;
        }
        return 0;
      }
      else
      {
// start_addr = start_addr - PAGE_SIZE;
#ifdef ALL_DRAM
        g_nodemask = 1;
        fprintf(stderr, "node0brk,%ld,%d\n", start_addr_aligned, align_to_page(arg1));
#else
        g_nodemask = 2;
        fprintf(stderr, "binding cxl from %ld with size %ld\n", (start_addr_aligned), align_to_page(arg1));
#endif
        if (mbind((void *)start_addr_aligned, (unsigned long)align_to_page(arg1), MPOL_BIND, &g_nodemask, sizeof(g_nodemask), MPOL_MF_MOVE) != 0)
        {
          perror("mbind failed brk bbb");
          return 1;
        }
        return 0;
      }
      // if (mbind((void *)start_addr, (unsigned long)arg1, MPOL_BIND, &g_nodemask, sizeof(g_nodemask), MPOL_MF_MOVE) != 0)
      // {
      //   perror("mbind failed brk2");
      //   return 1;
      // }
      // return 0;
    }
  }
  else
  {
    /*
     * Ignore any other syscalls
     * i.e.: pass them on to the kernel
     * as would normally happen.
     */
    return 1;
  }
}

static __attribute__((constructor)) void init(void)
{
  // Set up the callback function
  setvbuf(stdout, NULL, _IONBF, 0); // avoid buffer from printf
  redirect_stdout("/home/cc/call_stack.txt");
  inspect_heats("/home/cc/res.txt");
  intercept_hook_point = hook;
}
