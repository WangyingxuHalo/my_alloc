#include "my_malloc.h"
#include "assert.h"

// Split blocks into 2
void *split_into_two(memory_t *block, size_t req_size) {
	size_t memory_left = block->size - req_size - memory_size;
	memory_t *alloc_memory = (memory_t*)((void*)block + memory_size + memory_left);
	alloc_memory->size = req_size;
	//Leave current 
	block->size = memory_left;
	return (void*)alloc_memory + memory_size;
}

// Remove a block from the list
void *remove_from_list(memory_t *block, size_t req_size) {
	if (block->prev != NULL) {
		block->prev->next = block->next;
	} else {
		first = block->next;
	}
	if (block->next != NULL) {
		block->next->prev = block->prev;
	}
	return (void*)block + memory_size;
}

// Find first fit in the list
void *find_first_from_List(size_t req_size) {
	memory_t *cur = first;
	// Find from freed list
	while (cur != NULL) {
		// Split into 2 parts
		if (cur->size > req_size + memory_size) {
			return split_into_two(cur, req_size);
		// Malloc these spaces 
		} else if (cur->size >= req_size) {
			return remove_from_list(cur, req_size);
		} else {
			cur = cur->next;
		}
	}
	// If cannot find from list
	return NULL;
}

// Find best fit in the list
void *find_best_from_List(size_t req_size) {
	memory_t *cur = first;
	memory_t *best = NULL;
	size_t larger_min = (size_t)-1;
	// Find best fit from freed list
	while (cur != NULL) {
		// best match, directly return
		if (cur->size == req_size) {
			return remove_from_list(cur, req_size);
		} else if (cur->size > req_size) {
			if (cur->size < larger_min) {
				best = cur;
				larger_min = cur->size;
			}
		}
		cur = cur->next;
	}
	// If cannot find, return NULL
	if (best == NULL) {
		return NULL;
	} else {
		if (best->size > req_size + memory_size) {
			return split_into_two(best, req_size);
		} else if (best->size >= req_size) {
			return remove_from_list(best, req_size);
		}
	}
	return NULL;
}

// Find the first fit memory space to allocate resources.
void *ff_malloc(size_t req_size) {
	void *block = find_first_from_List(req_size);
	// malloc_memory += req_size + memory_size;
	if (block != NULL) {
		return block;
	} else {
		return create_mem(req_size);
	}
}

// free the first fit place
void ff_free(void *ptr) {
	free_memory(ptr);
}

// Find the best fit memory space to allocate resources.
void *bf_malloc(size_t req_size) {
	if (req_size <= 0) {
		return NULL;
	}
	void *block = find_best_from_List(req_size);
	// malloc_memory += req_size + memory_size;
	if (block != NULL) {
		return block;
	} else {
		return create_mem(req_size);
	}
}

// free the best fit place
void bf_free(void *ptr) {
	free_memory(ptr);
}

// Using sbrk() to generate new memory space
void *create_mem(size_t req_size) {
	if (req_size <= 0) {
		return NULL;
	}
	size_t new_size = req_size + memory_size;
	memory_t *newMem = sbrk(req_size + memory_size);
	total_memory += req_size + memory_size;
	newMem->next = NULL;
	newMem->prev = NULL;
	newMem->size = req_size;
	return (void*)newMem + memory_size;
}

void insert_into_freed_list(memory_t *prevOne, memory_t *cur, memory_t *ptr_begin) {
	// Freed list is empty or it is the first one, insert into the begining
	if (prevOne == NULL) {
		// insert into begining
		if (first != NULL) {
			ptr_begin->prev = NULL;
			ptr_begin->next = first;
			first = ptr_begin;
			ptr_begin->next->prev = ptr_begin;
			// No first, assign first to this one
		} else {
			first = ptr_begin;
			ptr_begin->prev = NULL;
			ptr_begin->next = NULL;
		}
	} else {
		// In other situation, directly inserting into list
		ptr_begin->prev = prevOne;
		ptr_begin->next = cur;
		if (cur != NULL) {
			cur->prev = ptr_begin;
		}
		prevOne->next = ptr_begin;
	}
}

// Merge all the freed list
void merge_all_list(memory_t *block) {
	memory_t *prevOne = block->prev;
	memory_t *nextOne = block->next;
	// If current is adjacent to next one
	if (nextOne != NULL) {
		if ((void*)block + memory_size + block->size == (void*)nextOne) {
			block->size += memory_size + nextOne->size;
			block->next = nextOne->next;
			if (block->next != NULL) {
				block->next->prev = block;
			}
		}
	}

	// If previous one is adjacent to current one
	if (prevOne != NULL) {
		if ((void*)prevOne + memory_size + prevOne->size == (void*)block) {
			prevOne->size += memory_size + block->size;
			prevOne->next = block->next;
			if (prevOne->next != NULL) {
				prevOne->next->prev = prevOne;
			}
		}
	}
}

// help function to free memory
void free_memory(void *ptr) {
	memory_t *ptr_begin = (memory_t*)((void*)ptr - memory_size);
	memory_t *prevOne = NULL;
	memory_t *cur = first;
	// Find a place to insert
	while (cur != NULL) {
		if (ptr_begin < cur) {
			break;
		}
		prevOne = cur;
		cur = cur->next;
	}

	// Insert into freed list
	insert_into_freed_list(prevOne, cur, ptr_begin);
	
	// Merge
	merge_all_list(ptr_begin);
}

//Get total data segment size
unsigned long get_data_segment_size() {
	return total_memory;
}

// Get total space of free and meta data
unsigned long get_data_segment_free_space_size() {
	unsigned long res = 0;
	memory_t *cur = first;
	while (cur) {
		res += cur->size + memory_size;
		cur = cur->next;
	}
	return res;
}

// Print the total list as well as the size for debugging
void print_list() {
	memory_t * eachOne = first;
	int index = 0;
	while(eachOne != NULL) {
		printf("address:%p\nnext:%p\nprev:%p\nsize:%zu\n", eachOne, eachOne->next, eachOne->prev,eachOne->size);
		eachOne = eachOne->next;
		index++;
	}
	printf("total size = %d\n",index);
}