#include "my_malloc.h"
#include <stdint.h>

/* You *MUST* use this macro when calling my_sbrk to allocate the 
 * appropriate size. Failure to do so may result in an incorrect
 * grading!
 */
#define SBRK_SIZE 2048
#define MAXBUCKET 7

/* If you want to use debugging printouts, it is HIGHLY recommended
 * to use this macro or something similar. If you produce output from
 * your code then you will receive a 20 point deduction. You have been
 * warned.
 */
#ifdef DEBUG
#define DEBUG_PRINT(x) printf x
#else
#define DEBUG_PRINT(x)
#endif

//#include <string.h>


/* make sure this always points to the beginning of your current
 * heap space! if it does not, then the grader will not be able
 * to run correctly and you will receive 0 credit. remember that
 * only the _first_ call to my_malloc() returns the beginning of
 * the heap. sequential calls will return a pointer to the newly
 * added space!
 * Technically this should be declared static because we do not
 * want any program outside of this file to be able to access it
 * however, DO NOT CHANGE the way this variable is declared or
 * it will break the autograder.
 */
void* heap;
void* heapEnd;

/* our freelist structure - this is where the current freelist of
 * blocks will be maintained. failure to maintain the list inside
 * of this structure will result in no credit, as the grader will
 * expect it to be maintained here.
 * Technically this should be declared static for the same reasons
 * as above, but DO NOT CHANGE the way this structure is declared
 * or it will break the autograder.
 */
metadata_t* freelist[8];
/**** SIZES FOR THE FREE LIST ****
 * freelist[0] -> 16
 * freelist[1] -> 32
 * freelist[2] -> 64
 * freelist[3] -> 128
 * freelist[4] -> 256
 * freelist[5] -> 512
 * freelist[6] -> 1024
 * freelist[7] -> 2048
 */

// the first run gets us the 8kb  got to check this so we dont repeat
int initialRun = 0;

void* my_malloc(size_t size)
{
  /* FIX ME */
  DEBUG_PRINT(("in malloc"));
  
    int metaDataSize = sizeof(metadata_t);
    
    //can't allocate negative size
    if(size <= 0) { return NULL; }
    
     if(size > (2048-metaDataSize)) { return NULL; }
    
    
    int index = getIndex(metaDataSize+size);
    
    if( index > MAXBUCKET) { return NULL; }
        

    if(!freelist[index] && (splitBlock(index) == -1)) { return NULL; }

    metadata_t *ptr = freelist[index];
        
    if(ptr->next) 
    {
        ptr->next->prev = NULL;
        freelist[index] = ptr->next;
    }
    else
    {
        freelist[index] = NULL;
    }
    
    
    ptr->in_use = 1;
    return ptr+1;
        
}

void* my_realloc(void* ptr, size_t new_size)
{
  /* FIX ME */
  DEBUG_PRINT(("in realloc\n"));
  //printf("in realloc\n");
  if(!ptr) return my_malloc(new_size); 
  
  else if(!new_size)
  {
    my_free(ptr);
    return NULL;
  }
  
  else
  {
      metadata_t* ptr2 = (metadata_t*)((char*)ptr-sizeof(metadata_t));
      metadata_t* ptr3 = ptr2;
     if(ptr2->size-sizeof(metadata_t) >= new_size)
     {
        int indexOfCurr, indexOfRealloc;
        indexOfRealloc = getIndex(sizeof(metadata_t)+new_size);
        if( indexOfRealloc > MAXBUCKET) { return NULL; }
        indexOfCurr = getIndex(sizeof(metadata_t)+ptr2->size);
        
        //case where its in the same bucket already 
        //(ie not a large enough size increase)
        if(indexOfCurr == indexOfRealloc) { return ptr; }
     }
        
    //if its not malloc more memory
    ptr2 = my_malloc(new_size);
    if(!ptr2) { return NULL; }
        
    //write code that transfers block of memory
    //from one address to another
    if(ptr3->size-sizeof(metadata_t) >= new_size) {
        my_memcpy(ptr2,ptr,new_size);
    }
    else {
        my_memcpy(ptr2,ptr,ptr3->size-sizeof(metadata_t));
    }
    
    my_free(ptr);
    return ptr2;
     
  }
     
  
  return NULL;
}

void my_free(void* ptr)
{
  /* FIX ME */
  DEBUG_PRINT(("in free"));
  char *p;
  metadata_t *mPtr;
  int index;
  
  //step 1 offset the block of memory back to the head of the metadata
  //purposely not casting data as a metadata pointer 
  p = ((char*)ptr)-sizeof(metadata_t);
  
  if(p == NULL || ((metadata_t*)p)->in_use != 1) { return; }
  
  mPtr = ((metadata_t*)p);
  index = getIndex(mPtr->size);
  if( index > MAXBUCKET) { return; }
  
    mPtr->in_use = 0;
    mPtr->next = freelist[index];
    mPtr->prev = NULL;
    
    if(mPtr->next) mPtr->next->prev = mPtr;
    
    freelist[index] = mPtr;
    
    //TODO make Buddies function
    merge();
}

void* my_memcpy(void* dest, const void* src, size_t num_bytes)
{
  /* FIX ME */
  //cast to a char
  DEBUG_PRINT(("in memcpy"));
  char *destPtr = dest;
  char const *srcPtr = src;

  while (num_bytes-- > 0) {
    *destPtr++ = *srcPtr++;
  }
    
  return dest;
}

/*HELPER FUNCTIONS*/
//not recursive but justin said that was cool
void merge()
{
    //need to get n
    //int n = logBase2(SIZEOFDATA_INCLUDING_METADATA )
    //compare bit n+1 make sure they match up A&B = A 
    //where A and B represent the 5th bit of numbers a and b
    //to determine address check n bit if 1 second in pair if 0 then first
    // subtract  heap from current position of heapEnd
    /************(int*)heapEnd -(int*)heap;*************/
    //do bit manipulation
    //add back heap
    DEBUG_PRINT(("in merge"));
    char* curr;
    
    for(curr= (char*)heap;curr!=(char*)heapEnd && curr;)
    {
        if(curr+((metadata_t*)curr)->size > (char*)heapEnd) break;
        
        if(((metadata_t*)curr)->size == SBRK_SIZE)
        {
            curr += ((metadata_t*)curr)->size;
            continue;
        }
        
        if(
            ((metadata_t*)(curr+((metadata_t*)curr)->size)) &&
            ((metadata_t*)curr)->size == 
            ((metadata_t*)(curr+((metadata_t*)curr)->size))->size
            &&
            ((metadata_t*)curr)->in_use == 0
            && 
            ((metadata_t*)(curr+((metadata_t*)curr)->size))->in_use == 0)
        {
        
            /* FIND BUDDY CODE*/
            int n = logBase2(((metadata_t*)curr)->size);
            
            int step1, step2, index;
            metadata_t* toMerge1 = (metadata_t*)curr;
            metadata_t* toMerge2 = (metadata_t*)(curr+((metadata_t*)curr)->size);
            
            

            step1 = (intptr_t)curr-(intptr_t)heap;
            step2 = 
                ((intptr_t)(curr+((metadata_t*)curr)->size))-(intptr_t)heap;

            if((step1>>(n+1) & 0x1) != (step2>>(n+1) & 0x1)){
                return;
            }
            
            //size without the metadata
            index = getIndex(((metadata_t*)curr)->size);
            
            if( index > MAXBUCKET) return;

            removeNode(toMerge1,index);
            removeNode(toMerge2,index);
   
            /* Add Nodes to list */
            toMerge1->size *= 2;
            toMerge1->in_use = 0;
            toMerge1->prev = NULL;
            toMerge1->next = freelist[index+1];

            if(freelist[index+1]){
                freelist[index+1]->prev = toMerge1;
            }

            freelist[index+1] = toMerge1;
            curr = heap;
        }else{
            curr += ((metadata_t*)curr)->size;
        }

    }
}       

void removeNode(metadata_t* node, int index){
    metadata_t* curr;
    
    if(node == NULL){
        return;
    }

    for(curr=freelist[index];curr!= NULL;curr=curr->next){
        if(curr == node){
            if(curr->prev){
                curr->prev->next = curr->next;
                if(curr->next){
                    curr->next->prev = curr->prev;
                }
            }else{
                freelist[index] = curr->next;
                if(curr->next){
                    curr->next->prev = NULL;
                }
            }
            
            return;
        }
    }
}

int splitBlock(int index)
{
    DEBUG_PRINT(("in split block"));
    split:
    // their might be a more efficent way to do this
    // need a l
    //printf("%d",index);
    
    for(int i=index+1; i<8; i++)
    {
        if(freelist[i] != NULL)
        {
            while(freelist[index] == NULL)
            {
                
                // need to break  memory block in half until it fills freelist
                //at the index i want (see the pun?)
                splitBlockHelper(i);
                i--;
            }
            return 1;
        }
    }
    // does not handle yet but if no blocks available to split ask for more meomory
    char* temp_heap = my_sbrk(SBRK_SIZE);

    if(temp_heap == (void*)-1) return -1;
    
    /*if(initialRun)
     {
     
        (char*)heap += SBRK_SIZE;
     }*/
    
    if(initialRun == 0)
    {
        heapEnd      = (void*)temp_heap;
        heap = temp_heap;
        initialRun=1;
    }
    // can't increment point because its a void pointer
     //heapEnd += SBRK_SIZE;
    heapEnd = (void*)(((char*)heapEnd) + SBRK_SIZE);
    
    ((metadata_t*)temp_heap)->next = freelist[MAXBUCKET];
    ((metadata_t*)temp_heap)->prev = NULL;
    ((metadata_t*)temp_heap)->in_use = 0;
    ((metadata_t*)temp_heap)->size = SBRK_SIZE;
    freelist[MAXBUCKET] = ((metadata_t*)temp_heap);
    
    if( index == MAXBUCKET)
        return 1;
        
    goto split;
}

void splitBlockHelper(int index)
{

    char *headBlock, *newCutBlock;
    //might be able to cast this to a metadata_t but this might be safer
    // Especially since we don't want the whole memory block to be treated as the metadata 
    //check...
    headBlock = (char*)freelist[index];
    newCutBlock = (char*)freelist[index]+((freelist[index]->size)/2);
    
    freelist[index] = freelist[index]->next;

    if(freelist[index] != NULL){
        freelist[index]->prev = freelist[index];
    }
    
    index--;

    //doubly Linked list setup
    ((metadata_t*)headBlock)->next = (metadata_t*)newCutBlock;
    ((metadata_t*)headBlock)->prev = NULL;
    
    // don't set to null might be a node already in freeList at that index
    ((metadata_t*)newCutBlock)->next = freelist[index];
    ((metadata_t*)newCutBlock)->prev = (metadata_t*)headBlock;
     freelist[index] = (metadata_t*)headBlock;
     
    freelist[index]->in_use = 0;
    freelist[index]->size = (short)(16<<index);
    freelist[index]->next->in_use = 0;
    freelist[index]->next->size = (short)(16<<index);
        
    if(((metadata_t*)newCutBlock)->next)
    {
        ((metadata_t*)newCutBlock)->next->prev = (metadata_t*)newCutBlock;
    }
}

int getIndex(int size)
{
  int index = 0;
  for(int i = 4; i <12; i++)
  {
   if (size > powFunc(i,2))
       index++;
  }
  return index;
}

int powFunc(int power, int base)
{
  int retVal = 1;
  if (power == 0)
    return 1;
  
  else if(base == 2)
    return base <<(power-1);
  
  else if(power == 1)
    return base;
  
  else
  {
    for(int i = 0; i < power; i++)
    {
      retVal *= base;
    }
  }
  return retVal;
}

int logBase2( int a)
{
 int total = ((int)sizeof(int))*8;
 if (a < 0)
 {
    DEBUG_PRINT(("Log of negative number error"));
    return -2;
}   
 if (!a)
    return -1;
 
  int pos = 0;
  //printf("total: %d\n",total);
  for(int i = total>>1 /*total*/; i > 0;i = i>>1)
  {
    //printf("i: %d\t",i);
    if (a >= 1<<i) 
    { 
        a >>= i; 
        pos +=i; 
    }
  }
  if (a >= 1<< 1) pos +=  1;
  
  return pos;
}
