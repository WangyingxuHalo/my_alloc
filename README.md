# my_malloc project
For this project, I implement my own version of several memory allocation functions from the C standard library.  <br>

I implement two functions: <br>
1) void *malloc(size_t size); <br>
2) void free(void *ptr); <br>

For each functions, I implement 2 ways, one is first fit and one is best fit. <br>
For first fit, I will find the first fit free memory to allocate. <br> 
For best fit, I will find the best fit memory to allocate. <br>

## malloc()
If there is free memory space, I will allocate from free memory space list. If there's no free memory space, I will use sbrk() to create more memory space.

## free()
To free a memory space, I put them into free memory space list for future allocate.