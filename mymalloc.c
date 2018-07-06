#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

int max_size = sizeof(myblock); // size of 5000 array
int min_alloc = 1;  // min aloc 
Metadata* first_metadata = (Metadata*)&myblock[0];
/*
 * Metadata take four param
 * 1.block size
 * 2.alloc cation flag : 0/1
 * 3.prev block sie
 * 4.last flag
 * */
void init(){
	memset(myblock,0,sizeof(myblock));
	Metadata* initial_metadata = first_metadata;
	initial_metadata->size = 5000 - sizeof(Metadata);
	initial_metadata->alloc_flag = 0;
	initial_metadata->prev_size = 0;
	initial_metadata->last_flag = 1;
}


Metadata* next(Metadata* cur){
	//return the metadata pointer of the next block	
	//check last flag before calling this method
	char* ret;
	ret = (char*)cur;
	ret += sizeof(Metadata);
	ret += cur->size;
	return (Metadata*)ret;
}

Metadata* prev(Metadata* cur){
	//return the metadata pointer of the previous block
	char* ret;
	ret = (char*)cur;
	ret -= cur->prev_size;
	ret -= sizeof(Metadata);
	return (Metadata*)ret;
}

int allocated(Metadata* cur){
	if(cur->alloc_flag){
		return 1;
	}else{
		return 0;
	}
}

int get_size(Metadata* cur){
	return cur->size;
}

int is_last(Metadata* cur){
	return cur->last_flag;
}

void* get_address(Metadata* cur){
	char* ret;
	ret = (char*)cur;
	ret += sizeof(Metadata)/sizeof(char);
	return (void*)ret;
}



void* mymalloc(size_t req_size,const char* file_name, int line_number){

	if(req_size + sizeof(Metadata) > max_size){

		return NULL;
	}

	if(req_size<min_alloc){
		req_size = min_alloc;
		//the minimal allocated size is 8 bytes
	}

	Metadata* cur = first_metadata;//Start from the first metadata
	while(!(cur < first_metadata || (char *)cur > &myblock[max_size-1])){
		if(!allocated(cur) && get_size(cur)>=req_size){
			//this block fits. Allocate memory here
			int old_size = get_size(cur);
			int old_last_flag = cur->last_flag;
			cur->alloc_flag = 1;
			Metadata* nxt = next(cur);			

			if( (get_size(cur)-req_size) >= (sizeof(Metadata)+min_alloc) ){//if the space left is enough for one block
				cur->size = req_size;
				Metadata* new_metadata;
				char* temp = (char*)cur;
				temp += (sizeof(Metadata))/sizeof(char);
				temp += get_size(cur);
				new_metadata = (Metadata*)temp;

				//set attributes for new metadata
				new_metadata->alloc_flag = 0;
				new_metadata->size = old_size-sizeof(Metadata)-cur->size;
				new_metadata->last_flag = old_last_flag;
				cur->last_flag = 0;
				new_metadata->prev_size = get_size(cur);  
				nxt->prev_size = get_size(new_metadata);
			}
			return get_address(cur);
		}else if(is_last(cur)){
            printf("No block available\n\n");
			//This is the last block but no match
			return NULL;
		}else{
			//go to next blcok
			cur = next(cur);
		}
	};
	return NULL;
}
        
void myfree(void* ptr, const char* file_name, int line_number){

	if(ptr == NULL){
        printf("Block is already Free.\n\n");
		return;
	}

	Metadata* meta_ptr = (Metadata*)(ptr - sizeof(Metadata));
		
	if(meta_ptr < first_metadata || (char *)ptr > &myblock[max_size-1]){
		printf("Invalid Block\n\n");
		return;
	}
	
	if(!allocated(meta_ptr)){
		printf("Block is already Free.\n\n");
        return;
	}
    
	
	meta_ptr->alloc_flag = 0;
	int new_size;
	
	//merge right
	Metadata* nxt = next(meta_ptr);
	if(meta_ptr->last_flag == 0 && nxt->alloc_flag == 0){
		new_size = get_size(meta_ptr) + get_size(nxt) + sizeof(Metadata);
		meta_ptr->size = new_size;
		meta_ptr->last_flag = nxt->last_flag;

		if(!meta_ptr->last_flag){
			Metadata* new_nxt = next(meta_ptr);
			new_nxt->prev_size = get_size(meta_ptr);
		}
		//printf("After merging right, new size is %d, ", new_size);
	}
			
	//merge left
	Metadata* prv = prev(meta_ptr);
	//printf("size is of prev is %d and alloc is %d", prv->size, prv->alloc_flag);

	if(meta_ptr != first_metadata && prv->alloc_flag == 0){
		prv->size += get_size(meta_ptr) + sizeof(Metadata);
		prv->last_flag = meta_ptr->last_flag;

		if(!meta_ptr->last_flag){
			Metadata* new_nxt = next(meta_ptr);
			new_nxt->prev_size = get_size(prv);
		}
		//printf("After merging left, new size is %d", get_size(prv));
	}
    printf("Successfully block dealocated\n\n");
}

void print_stats(){
	Metadata* cur = first_metadata;
	int blocks = 0;
	int allocated_blocks = 0;
	int unallocated_blocks = 0;
	int allocated_space = 0;
	int unallocated_space = 0;

	while(1){
		blocks++;
        printf("*******************************************************************\n");
		if(allocated(cur)){
			printf("allocated: 8 + %d = %d\n", get_size(cur), get_size(cur) + 8);
			allocated_blocks++;
			allocated_space += get_size(cur);
		}else{
			printf("free: 8 + %d = %d\n", get_size(cur), get_size(cur) + 8);
			unallocated_blocks++;
			unallocated_space += get_size(cur);
		}

		if(is_last(cur)){
            printf("*******************************************************************\n \n");
			break;
		}
		cur = next(cur);
	}
}
