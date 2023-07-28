#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>

typedef int (*rand_like_function)();
int rand()
{
    // int (*original_rand)(void) = dlsym(RTLD_NEXT, "rand");
    // return original_rand() % 100;
    rand_like_function my_rand = (rand_like_function)dlsym(RTLD_NEXT, "rand");
    if (my_rand == NULL)
    {
        printf("Failed to find the symbol.\n");
        dlclose(my_rand);
        return 1;
    }

    Dl_info info;
    if (dladdr(my_rand, &info) == 0)
    {
        printf("Failed to retrieve symbol information.\n");
        dlclose(my_rand);
        return 1;
    }

    printf("Symbol name: %s\n", info.dli_sname);
    printf("Symbol address: %p\n", info.dli_saddr);
    printf("Library name: %s\n", info.dli_fname);
    printf("Library base address: %p\n", info.dli_fbase);
    return my_rand() % 100;
}
