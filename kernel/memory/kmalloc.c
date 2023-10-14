#include "kmalloc.h"
#include "heap.h"
#include "drivers/video/vga.h"
#include "string/string.h"
#include "math/math.h"

#define BYTES_TO_PAGES(x) (x >> 12)

// TODO: Update approach to support non-contiguous heap memory
// TODO: Keep track of heap pages and allocation here, instead of in heap.c

typedef struct Chunk Chunk;

struct Chunk {
	char free;
	Chunk* next;
}__attribute((packed))__;

Chunk* kheap_start = NULL;
Chunk* last_chunk = NULL; // The last real chunk
Chunk* end_chunk = NULL; // Empty chunk pointing to the first unavailable address
uint64_t heap_length = 0;

void update_chunks(void);

int kmalloc_init(void) {
	heap_length = extend_heap();
	kheap_start = HEAP_VIRTADDR;
	if (kheap_start == NULL) {
		vga_println("ERR: allocator_init: unable to allocate heap\n");
		return -1;
	}
	// char start[20];
	// itoa(kheap_start, start, 16);
	// vga_print("allocator_init: heap at ");
	// vga_println(start);
	end_chunk = (uint64_t)kheap_start + heap_length - sizeof(Chunk);
	end_chunk->free = 0;
	end_chunk->next = NULL;
	update_chunks();
	Chunk* first_chunk = kheap_start;
	first_chunk->free = 1;
	first_chunk->next = end_chunk;
	last_chunk = first_chunk;
	return 1;
}

void update_chunks(void) {
	uint64_t heap_end = (uint64_t)kheap_start + heap_length - sizeof(Chunk);
	if (end_chunk < heap_end) {
		last_chunk = end_chunk;
		last_chunk->free = 1;
		end_chunk = heap_end;
		end_chunk->free = 0;
		end_chunk->next = NULL;
		last_chunk->next = end_chunk;
	}
}

void* kmalloc(uint64_t bytes) {
	if (kheap_start == NULL) {
		return NULL;
	}
	void* out = NULL;
	Chunk* current_chunk = kheap_start;
	while (1) {
		if (current_chunk->free) {
			Chunk* next_taken_chunk = current_chunk->next;
			while (next_taken_chunk->free && (next_taken_chunk->next != NULL)) {
				// printf("alloc: merging %p with %p\n", next_taken_chunk, current_chunk);
				next_taken_chunk = next_taken_chunk->next;
				current_chunk->next = next_taken_chunk;
			}
			uint64_t free_size = (uint64_t)next_taken_chunk - ((uint64_t)current_chunk + sizeof(Chunk));
			if (bytes < free_size) {
				// printf("alloc: found sufficient free space at %p\n", current_chunk);
				current_chunk->free = 0;
				Chunk* new_chunk = (Chunk*)(current_chunk + sizeof(Chunk) + bytes);
				if ((new_chunk + sizeof(Chunk)) < next_taken_chunk) {
					// printf("alloc: room for new chunk at %p\n", new_chunk);
					new_chunk->next = next_taken_chunk;
					new_chunk->free = 1;
					current_chunk->next = new_chunk;
				}
				return (current_chunk + sizeof(Chunk));
			}
		}
		if (current_chunk->next == NULL) {
			// printf("alloc: No free chunks\n");
			uint64_t bytes_to_alloc = bytes;
			for (int i = 0; i < ceil(bytes/(float)0x1000); i++) {
				heap_length = extend_heap(); // TODO: Error detection
			}
			update_chunks();
			current_chunk = kheap_start;
			// return NULL;
		} else {
			current_chunk = current_chunk->next;
		}
	}
}

void kfree(void* ptr) {
	Chunk* current_chunk = kheap_start;
	while ((current_chunk + sizeof(Chunk)) != ptr) {
		if (current_chunk == NULL) {
			vga_println("kfree: couldn't find target chunk");
			// printf("kfree: couldn't find target chunk of %p\n", ptr);
			return;
		}
		current_chunk = current_chunk->next;
	}
	current_chunk->free = 1;
	// TODO: Check if last page is empty and shrink heap
}

// void heap_list() {
// 	printf("heap_list: heap start at %p, heap end at %p\n", kheap_start, (kheap_start + heap_length));
// 	Chunk* current_chunk = kheap_start;
// 	do {
// 		printf("heap_list: chunk at %p:\n", current_chunk);
// 		printf("heap_list:         free: %d\n", current_chunk->free);
// 		printf("heap_list:         length: %ld\n", (current_chunk->next - (current_chunk + sizeof(Chunk))));
// 		printf("heap_list:         data: %p\n", (current_chunk + sizeof(Chunk)));
// 		printf("heap_list:         next: %p\n", current_chunk->next);
// 		current_chunk = current_chunk->next;
// 	} while (current_chunk != NULL);
// 	//printf("last chunk at %p, free = %d\n", current_chunk, current_chunk->free);
// }