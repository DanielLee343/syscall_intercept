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

void inspect_source_code(char *filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        exit(1);
    }

    // Read and print each line
    char line[256];  // Assuming lines are not longer than 255 characters
    while (fgets(line, sizeof(line), file) != NULL) {
        // printf("%s", line);
        fprintf(stderr, "%s", line);
    }

    // Close the file
    fclose(file);
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
void* roundUpToPage(void* addr) {
    long pageSize = sysconf(_SC_PAGESIZE); // Get the system's page size
    uintptr_t address = (uintptr_t)addr; // Cast the address to uintptr_t

    // Calculate the number of pages to add to round up the address
    uintptr_t numPages = (address + pageSize - 1) / pageSize;

    // Round up the address to the nearest page address
    uintptr_t roundedAddress = numPages * pageSize;

    return (void*)roundedAddress; // Cast the rounded address back to void*
}

static int hook(long syscall_number, long arg0, long arg1, long arg2, long arg3,
                long arg4, long arg5, long *result) {
  int static mmap_id = 0;
  struct timespec ts;
  char call_stack_concat[SIZE] = "";
  unsigned long g_nodemask;
  unsigned long g_nodemask_2;
  pthread_mutex_init(&count_mutex, NULL);
  // printf("%ld\n", syscall_number);
  if (syscall_number == SYS_mmap) {
    *result = syscall_no_intercept(syscall_number, arg0, arg1, arg2, arg3, arg4,
                                   arg5);
    // fprintf(stderr, "mmap addr: %ld, size: %ld, prot: %ld, flags: %ld, fd:
    // %ld, offset: %ld\n", arg0, arg1, arg2, arg3, arg4, arg5);
    pthread_mutex_lock(&count_mutex);
    clock_gettime(CLOCK_MONOTONIC, &ts);
    long int start_addr = (long int)(void *)*result;
    // get_call_stack(arg1,call_stack_concat);
    // fprintf(stderr, "start addr: %ld\n", start_addr);

    // size_t half_size = (size_t) arg1 / 2;
    // void* part1_addr = (void *)*result;
    // void* part2_addr = (void*)(part1_addr + half_size);
    // fprintf(stderr, "half size: %zu\n", half_size);
    // fprintf(stderr, "part1_addr: %p\n", part1_addr);
    // fprintf(stderr, "part2_addr: %p\n", part2_addr);
    fprintf(stderr, "%ld%ld,mmap,%ld,%p,%ld\n", ts.tv_sec, ts.tv_nsec,
            start_addr, (void *)*result, arg1);
    pthread_mutex_unlock(&count_mutex);


    // if (
    //     // (void *)*result == addr1_to_move ||
    //     //   (void *)*result == addr1_to_move_1 ||
    //     //   (void *)*result == addr2_to_move ||
    //     //   (void *)*result == addr2_to_move_2 ||
    //     (void *)*result == addr1_to_move_pre ||
    //     (void *)*result == addr1_to_move_pre_1 ||
    //     (void *)*result == addr2_to_move_pre ||
    //     (void *)*result == addr3_to_move_pre ||
    //     (void *)*result == addr3_to_move_pre_1 ||
    //     (void *)*result == addr4_to_move_pre) {
    //   fprintf(stderr, "gotcha!! Moving you back to local DRAM\n");
    //   g_nodemask = 1;
    // } else {
    //   fprintf(stderr, "You are staying in CXL\n");
    //   g_nodemask = 2;
    // }

    g_nodemask = 1;
    g_nodemask_2 = 2;
    // if (mbind(part1_addr, half_size, MPOL_BIND, &g_nodemask, sizeof(g_nodemask), 0) != 0) {
    //     perror("mbind for part1 failed");
    //     // ... (unmap and cleanup)
    //     return 1;
    // }
    // if (mbind(part2_addr, 268435456, MPOL_BIND, &g_nodemask_2, sizeof(g_nodemask_2), 0) != 0) {
    //     perror("mbind for part2 failed");
    //     // ... (unmap and cleanup)
    //     return 1;
    // }

    return 0;
  } else if (syscall_number == SYS_munmap) {
    /* pass it on to the kernel */
    *result = syscall_no_intercept(syscall_number, arg0, arg1, arg2, arg3, arg4,
                                   arg5);
    clock_gettime(CLOCK_MONOTONIC, &ts);
    long int start_addr = (long int)(void *)arg0;
    // uintptr_t start_addr = (uintptr_t)(void *)*result;
    fprintf(stderr, "%ld%ld,munmap,%ld,%p,%ld\n", ts.tv_sec, ts.tv_nsec,
            start_addr, (void *)arg0, arg1);
    return 0;
  }
  // else if (syscall_number == SYS_alloc_hugepages){
  //     /* pass it on to the kernel */
  //     *result = syscall_no_intercept(syscall_number, arg0, arg1, arg2, arg3,
  //     arg4, arg5); clock_gettime(CLOCK_MONOTONIC, &ts); fprintf(stderr,
  //     "%ld.%ld, alloc_hp,%p,%ld\n", ts.tv_sec,ts.tv_nsec, (void *)arg0,
  //     arg1); return 0;
  // }
//   else if (syscall_number == SYS_brk) {
//     *result = syscall_no_intercept(syscall_number, arg0, arg1, arg2, arg3, arg4,
//                                    arg5);
//     clock_gettime(CLOCK_MONOTONIC, &ts);
//     uintptr_t start_addr = (uintptr_t)(void *)arg0;
//     fprintf(stderr, "%ld%ld,brk,%lu,%p,%ld\n", ts.tv_sec, ts.tv_nsec,
//             start_addr, (void *)*result, arg1);
//     return 0;
//   } 
  else {
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
  setvbuf(stdout, NULL, _IONBF, 0); // avoid buffer from printf
//   redirect_stdout("/home/cc/call_stack.txt");
//   inspect_source_code("/home/cc/res.txt");
  intercept_hook_point = hook;
}