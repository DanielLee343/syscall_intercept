#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>

int rand() {
    int (*original_rand)(void) = dlsym(RTLD_NEXT, "rand");
    // int (*original_rand)(void) = dlsym("libstdc++.so.6", "std::cout");
    return original_rand() % 100;
}