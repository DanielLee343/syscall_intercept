
CC=gcc
CPP=g++
CFLAGS=-lsyscall_intercept -lnuma -fpic -shared

all: mmap_test test_rand shim.so test_rand_cpp static_placement
# static_placement

mmap_test: mmap_test.c
	$(CC) -o $@ $^

test_rand: test_rand.c
	$(CC) -g -o $@ $^

test_rand_cpp: test_rand.cpp
	$(CPP) -g -o $@ $^

shim.so: shim.c
	$(CC) -fpic -shared -o $@ $^ -ldl

static_placement: static_placement.c
	$(CC) $^ $(CFLAGS) -o $@.so 

clean: 
	rm -rf static_placement.so mmap_test test_rand test_rand_cpp shim.so