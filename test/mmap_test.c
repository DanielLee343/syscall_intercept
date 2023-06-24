#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/mman.h>
#define PAGE_SIZE 4096

void do_malloc(){
    // int new_val = 64 * 1024;
    // if (mallopt(M_MMAP_THRESHOLD, new_val) == 0) {
    //     printf("updating thresh failed!\n");
    //     return 1;
    // }
    size_t size = 2 * 1024; // 1MB
    char* buffer = (char*)malloc(size);
    if (buffer == NULL) {
        printf("Memory allocation failed!\n");
        // return 1;
        exit(1);
    }
    memset(buffer, 1, size);

    free(buffer);

}

void do_brk() {
    void* initial_break = sbrk(0);  // Get the initial program break

    // Allocate 128 bytes of memory using brk
    int increment = 4*1024;
    // void* new_break = (char*)initial_break + increment;
    void* old_addr = sbrk(increment);

    // if (result == -1) {
    //     printf("Memory allocation failed!\n");
    //     // return 1;
    //     exit(1);

    // }

    printf("Initial break: %p\n", initial_break);
    // printf("New break: %p\n", new_addr);

    // return 0;
}
int get_nearest_multiple(int number, int multiple) {
    int remainder = number % multiple;
    int nearest_multiple;

    if (remainder <= (multiple / 2)) {
        nearest_multiple = number - remainder;
    } else {
        nearest_multiple = number + (multiple - remainder);
    }

    return nearest_multiple;
}
// uintptr_t align_to_page(uintptr_t address, size_t page_size) {
//     uintptr_t page_mask = ~(page_size - 1);
//     return (address + (page_size - 1)) & page_mask;
// }
long int align_to_page(long int address) {
    long int page_mask = ~(PAGE_SIZE - 1);
    return (address + (PAGE_SIZE - 1)) & page_mask;
}

long int alignTo4096(long int value) {
    long int alignedValue = (value + 4095) / 4096 * 4096;
    return alignedValue;
}

int calculateIntersectedArea_(long int A, long int B, long int C, long int D, long int* start, long int* end) {
  int intersectedArea = 0;
  if (C < A && A < D && D < B) { // case 1: need to align D
    printf("1st case\n"); 
    *start = A; // A must be aligned already
    *end = align_to_page(D);
  } else if (A < C && C < D && D < B) { // case 2: need to align both C and D
    printf("2nd case\n"); 
    *start = align_to_page(C);
    *end = align_to_page(D);
  } else if (A < C && C < B && B < D) { // case 3: need to align C
    printf("3rd case\n"); 
    *start = align_to_page(C);
    *end = align_to_page(B); // in case B is not aligned
  } 
  if (*end > align_to_page(B)) {
    *end = align_to_page(B);
  }
  intersectedArea = (int)(*end - *start);
  // sanity check, intersectedArea must be multiple of PAGE_SIZE
    fprintf(stderr, "start: %ld, end: %ld\n", *start, *end);
  if (align_to_page(intersectedArea) != (long int)intersectedArea) {
    perror("failed");
  }
  if (intersectedArea > 0) {
      return intersectedArea;
  } else {
      return 0;
  }
}

void do_align() {
    // uintptr_t address = (uintptr_t)malloc(5000);  // Example address
    void* address = mmap(NULL, 5000, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    // int increment = 3;
    // void* address = sbrk(increment);

    // long int aligned_address = align_to_page((uintptr_t)address);

    printf("mmaped Address: %ld\n", (long int)address);
    // printf("Aligned Address : %p\n", (void*)aligned_address);

    // long int A = 1, B = 8999, C = 2, D = 40000;
    // long int potential_to_DRAM_start, potential_to_DRAM_end;
    // int potentialIntersect = calculateIntersectedArea_(A,B,C,D,&potential_to_DRAM_start, &potential_to_DRAM_end);
    memset(address, 1, 5000);
}

int main() {
    for (int i = 0; i < 1; i++) {
        // do_malloc();
        // do_brk();
        do_align();
    }
}
