#include "my_malloc.h"
#include <stdio.h>

/***********REMOVE WHEN DONE*********/
extern void* heap;

extern  void* heapEnd;

extern metadata_t* freelist[8];

/***********************************/

void printFreeList()
{
    for(int i = 0; i < 8; i++)
    {
        metadata_t* curr = freelist[i];
        int count = 0;
        while(curr)
        {
            count++;
            curr = curr->next;
        }
        
        printf("freeList[%d] elments = %d\n",i,count);
        
    }
    printf("\n");
}





void printVisualFreeList()
{
    for(int i = 0; i < 8; i++)
    {
        metadata_t* curr = freelist[i];
        int count = 0;
        printf("|*********************|\n|*                   *|\n|*  Bucket:  %d       *| ===>",i);
        while(curr)
        {
            count++;
            printf("Node %d: Block Size is %d ===>",count,curr->size);
            curr = curr->next;
        }
        printf("\n");
        printf("|*                   *|\n|*********************|\n");
        
        //printf("freeList[%d] elments = %d\n",i,count);
        
    }
    printf("\n");
}

int main() {
    /*
     * Test code goes here
     */
     
     /*int *a = my_malloc(sizeof(int));
     if(a == 0)
     {
        printf("malloc failed");
        return 0;
     }
     *a = 5;
     printf("ptr: %p\n dereferenced pointer: %d\n",(void*)a,*a);
     
     //printVisualFreeList();
     
     my_free(a);
     
     printVisualFreeList();
     
     short *b = my_malloc(sizeof(short));
     if(b == 0)
     {
        printf("malloc failed");
        return 0;
     }
     
    *b = 20;
    printf("ptr: %p\n dereferenced pointer: %d\n",(void*)b,*b);
      
     
    b = my_realloc(b,sizeof(int));
     
    *b = 8;
    printf("ptr: %p\n dereferenced pointer: %d\n",(void*)b,*b);
    
    b = my_realloc(b,100);
     
    *b = 80;
    printf("ptr: %p\n dereferenced pointer: %d\n",(void*)b,*b);
    
    b = my_realloc(b,1000);
     
    *b = 60;
    printf("ptr: %p\n dereferenced pointer: %d\n",(void*)b,*b);
    
    //printVisualFreeList();
    
    printf("size of metadata_t header: %d\n", sizeof(metadata_t));
    
    b = my_realloc(b,1001);
     
    *b = 67;
    printf("ptr: %p\n dereferenced pointer: %d\n",(void*)b,*b);
    
    //printVisualFreeList();
    
    b = my_realloc(b,2000);
     
    *b = 600;
    printf("ptr: %p\n dereferenced pointer: %d\n",(void*)b,*b);*/
    
    
      void* a =  my_malloc(470);
      void* b =  my_malloc(37);
      void* c =  my_malloc(2);
      printVisualFreeList();
      printf("\n");
      
      void* d =  my_malloc(500);
      void* e =  my_malloc(3);
      void* f =  my_malloc(200);
      
      printVisualFreeList();
      printf("\n");
      
      my_free(d);
      my_free(e);
      my_free(f);
      
      printVisualFreeList();
      printf("\n");

     
      a = my_realloc(a,1000);
      b = my_realloc(b,200);
      c = my_realloc(c,2000);
      printVisualFreeList();
      printf("\n");
      
      my_free(c);
      my_free(b);
      my_free(a);
      printVisualFreeList();
      printf("\n");
      
      void* g =  my_malloc(-1);
      if(!g)
        printf("e is NULL\n");
        
    
     g =  my_malloc(1012);
     void* h =  my_malloc(500);
     printVisualFreeList();
     my_free(h);
     my_free(g);
     
    return 0;
}
