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

FILE *g_fp = NULL;
char *g_line = NULL;
size_t len = 0;
pthread_mutex_t count_mutex;

void redirect_stdout(char *filename) {
  int fd;
  if ((fd = open(filename, O_CREAT | O_WRONLY, 0666)) < 0) {
    perror(filename);
    exit(1);
  }
  close(1);
  if (dup(fd) != 1) {
    fprintf(stderr, "Unexpected dup failure\n");
    exit(1);
  }
  close(fd);

  g_fp = fopen("call_stack.txt", "w+");
  if (g_fp == NULL) {
    printf("Error when try to use fopen!!\n");
  }
}
long int hash(char *word) {
  unsigned int hash = 0;
  for (int i = 0; word[i] != '\0'; i++) {
    hash = 31 * hash + word[i];
  }
  // return hash % TABLE_SIZE;
  return abs(hash);
}

void get_call_stack(int size_allocation, char *call_stack_concat) {
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

static int hook(long syscall_number, long arg0, long arg1, long arg2, long arg3,
                long arg4, long arg5, long *result) {
  int static mmap_id = 0;
  struct timespec ts;
  char call_stack_concat[SIZE] = "";
  unsigned long g_nodemask;
  pthread_mutex_init(&count_mutex, NULL);
  // printf("%ld\n", syscall_number);
  if (syscall_number == SYS_mmap) {
    *result = syscall_no_intercept(syscall_number, arg0, arg1, arg2, arg3, arg4,
                                   arg5);
    // fprintf(stderr, "mmap addr: %ld, size: %ld, prot: %ld, flags: %ld, fd:
    // %ld, offset: %ld\n", arg0, arg1, arg2, arg3, arg4, arg5);
    pthread_mutex_lock(&count_mutex);
    clock_gettime(CLOCK_REALTIME, &ts);
    // get_call_stack(arg1,call_stack_concat);
    // fprintf(stderr, "%ld.%ld,mmap, %ld,
    // %p,%ld,%s\n",ts.tv_sec,ts.tv_nsec,arg1,(void
    // *)*result,hash(call_stack_concat),call_stack_concat); fprintf(stderr,
    // "%ld.%ld,mmap, %p, %ld\n",ts.tv_sec,ts.tv_nsec, (void *)*result, arg1);
    uintptr_t decimal_addr = (uintptr_t)(void *)*result;
    fprintf(stderr, "%ld.%ld, mmap, %lu, %p, %ld\n", ts.tv_sec, ts.tv_nsec,
            decimal_addr, (void *)*result, arg1);
    pthread_mutex_unlock(&count_mutex);

    g_nodemask = 1;
    //unsigned long lower_bound = 140722954995276;
    //unsigned long upper_bound = 140723192279136;
    //unsigned long lower_bound_2 = 140735303823697;
    //unsigned long upper_bound_2 = 140735793449825;

    //void *lower_bound_addr = (void *)lower_bound;
    //void *upper_bound_addr = (void *)upper_bound;
    //void *lower_bound_addr_2 = (void *)lower_bound_2;
    //void *upper_bound_addr_2 = (void *)upper_bound_2;

    //if (((void *)*result >= lower_bound_addr && (void *)*result <= upper_bound_addr) ||
    //    	    ((void *)*result >= lower_bound_addr_2 && (void *)*result <= upper_bound_addr_2)) {
    //  fprintf(stderr, "gotcha!! Moving you back to local DRAM\n");
    //  g_nodemask = 1;
    //} else {
    //  fprintf(stderr, "You are staying in CXL\n");
    //  g_nodemask = 2;
    //}

    if (mbind((void *)*result, (unsigned long)arg1, MPOL_BIND, &g_nodemask,
              sizeof(g_nodemask), MPOL_MF_MOVE) == -1) {
      fprintf(stderr, "Error:%d\n", errno);
      perror("Error description");
    }

    return 0;
  } else if (syscall_number == SYS_munmap) {
    /* pass it on to the kernel */
    *result = syscall_no_intercept(syscall_number, arg0, arg1, arg2, arg3, arg4,
                                   arg5);
    clock_gettime(CLOCK_REALTIME, &ts);
    uintptr_t decimal_addr = (uintptr_t)(void *)arg0;
    fprintf(stderr, "%ld.%ld, munmap, %lu, %ld\n", ts.tv_sec, ts.tv_nsec,
            decimal_addr, arg1);
    return 0;
  }
  // else if (syscall_number == SYS_alloc_hugepages){
  //     /* pass it on to the kernel */
  //     *result = syscall_no_intercept(syscall_number, arg0, arg1, arg2, arg3,
  //     arg4, arg5); clock_gettime(CLOCK_REALTIME, &ts); fprintf(stderr,
  //     "%ld.%ld, alloc_hp,%p,%ld\n", ts.tv_sec,ts.tv_nsec, (void *)arg0,
  //     arg1); return 0;
  // }
  else if (syscall_number == SYS_brk) {
    /* pass it on to the kernel */
    *result = syscall_no_intercept(syscall_number, arg0, arg1, arg2, arg3, arg4,
                                   arg5);
    clock_gettime(CLOCK_REALTIME, &ts);
    uintptr_t decimal_addr = (uintptr_t)(void *)arg0;
    fprintf(stderr, "%ld.%ld, brk, %lu, %ld\n", ts.tv_sec, ts.tv_nsec,
            decimal_addr, arg1);
    return 0;
  } else {
    /*
     * Ignore any other syscalls
     * i.e.: pass them on to the kernel
     * as would normally happen.
     */
    return 1;
  }
}

static __attribute__((constructor)) void init(void) {
  // Set up the callback function
  //setvbuf(stdout, NULL, _IONBF, 0); // avoid buffer from printf
  //redirect_stdout("/home/cc/call_stack.txt");
  intercept_hook_point = hook;
}
