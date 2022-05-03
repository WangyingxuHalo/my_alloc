#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct memory {
	size_t size;
	struct memory * prev;
	struct memory * next;
} memory_t;

size_t memory_size = sizeof(memory_t);
unsigned long total_memory = 0;
memory_t *first = NULL;
void *ff_malloc(size_t size);
void ff_free(void *ptr);
void *bf_malloc(size_t size);
void bf_free(void *ptr);
void *find_first_from_List(size_t req_size);
void *find_best_from_List(size_t req_size);
void *split_into_two(memory_t *block, size_t req_size);
void *remove_from_list(memory_t *block, size_t req_size);
void insert_into_freed_list(memory_t *prevOne, memory_t *cur, memory_t *ptr_begin);
void merge_all_list(memory_t *block);
void *create_mem(size_t size);
void free_memory(void *ptr);
unsigned long get_data_segment_size();
unsigned long get_data_segment_free_space_size();
void print_list();
