#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include "mymalloc.h"

void function(){
    void* p_array[10000];
	int length = 0;
    
    int a=1,size=0;
    printf("Enter 0 : for exit\n");
    printf("Enter 1 : for allocation\n");
    printf("Enter 2 : for free\n");
    printf("Enter 3 : for status\n");
    int i=0;
    
    while(1)
    {
        scanf("%d",&a);
        if(a==0)
        {
            break;
        }
        else if(a==1)
        {
            printf("Enter size for allocation\n");
            scanf("%d",&length);
            printf("\n");
           if(length>5000){
            printf("Invalid Block size\n\n");   
           }
           else{
                int blocksize = length;
                p_array[size] = malloc(blocksize);
                size++;
        }
        }
        else if(a==2)
        {
            printf("Enter block number you wants to free\n");
            scanf("%d",&length);
            printf("\n");
			if(length == (size - 1)){
				free(p_array[length]);
				p_array[length] = 0;
                //printf("Successfully block dealocated\n\n");
			}
			else{
                if(length<size-1)
                {
                    free(p_array[length]);
                    
                }
				
                else if(length>=size)
                    printf("Invalid block\n\n");

			}
			
            
        }
        else
        {
            printf("\n");
            print_stats();
        }
        
    printf("Enter 0 : for exit\n");
    printf("Enter 1 : for allocation\n");
    printf("Enter 2 : for free\n");
    printf("Enter 3 : for status\n");
    }
}

void main(){
	
	srand((unsigned int)time(NULL));	

	int i;
	    init();
        function();
print_stats();
	
}
