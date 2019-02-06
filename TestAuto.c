#include <stdio.h>
#include "my_malloc.h"
#include <stdlib.h>


void heap_report(){
    printf("in heap report\n");
    char* curr = heap;
    char* highest = heapEnd;   
    while(curr != highest){
        if(curr != NULL){
            printf("Address: %d\n in_use: %d\n Size: %d\n\n",
                (int)curr - (int)highest  ,
                ((metadata_t*)curr)->in_use           ,
                ((metadata_t*)curr)->size            );
            curr += ((metadata_t*)curr)->size;
                        
        }else{
            return;
        }
    }
}

/*
 * myfgets(), as seen in lab4
 *
 * Slightly modified version from the
 * encoder/decoder HW I did.
 *
 * Changes:
 *  - ignores '\n', replaces with '\0'
 *       returns
 *  - takes in *fp to allow to read from file
 *  - replaces '\t' with ' '
 *
 */ 
char *myfgets(FILE *fp) {
   char *prgc, *prgcT;
   int cTemp;
   int iLength;
   prgc = NULL;
   
   /* Changed to fgetc(FILE *) to allow file reading */
   for(iLength=0;;iLength++){
       cTemp = fgetc(fp);
       
       if(cTemp==EOF && prgc == NULL){
            return prgc;
       }
       else if(cTemp==EOF && prgc != NULL){
            prgcT = my_realloc(prgc,(iLength+1)*sizeof(char));
            if(prgcT == NULL){
                free(prgc);
                return NULL;
            }
            prgc = prgcT;
            *(prgc+iLength) = '\0';
            return prgc;
       }
       else if(cTemp == '\n'){
            prgcT = my_realloc(prgc,(iLength+1)*sizeof(char));
            if(prgcT == NULL){
                free(prgc);
                return NULL;
            }
            prgc = prgcT;
            *(prgc+iLength) = '\0';
            return prgc;
       }
       else if(cTemp == '\t'){
            prgcT = my_realloc(prgc,(iLength+1)*sizeof(char));
            if(prgcT == NULL){
                free(prgc);
                return NULL;
            }
            prgc = prgcT;
            *(prgc+iLength) = ' ';
       }
       else{
            prgcT = my_realloc(prgc,(iLength+1)*sizeof(char));
            if(prgcT == NULL){
                free(prgc);
                return NULL;
            }
            prgc = prgcT;
            *(prgc+iLength) = (char)cTemp;
       }
   }
}
   

int main(){
    char *p, *p2, *p3, *p4, *p5, *p6;
    
    p = my_malloc(30);
    {
        char i;
        int j;
        for(i='a',j=0;i<='z';i++,j++){
            p[j] = i;
        }
    }
    p[26] = '\0';

    p2 = my_malloc(30);
    {
        char i;
        int j;
        for(i='a',j=0;i<='z';i++,j++){
            p2[j] = i;
        }
    }
    p2[26] = '\0';

    printf("%s\n%s\n",p,p2);


    my_free(p);
    my_free(p2);

    heap_report();

    p = my_malloc(30);
    {
        char i;
        int j;
        for(i='a',j=0;i<='z';i++,j++){
            p[j] = i;
        }
    }
    p[26] = '\0';

    p2 = my_malloc(30);
    {
        char i;
        int j;
        for(i='a',j=0;i<='z';i++,j++){
            p2[j] = i;
        }
    }
    p2[26] = '\0';

    printf("%s\n%s\n",p,p2);
    

    
    
    p3 = my_malloc(1024);
    p4 = my_malloc(3);
    p5 = my_malloc(2000);
    p6 = my_malloc(3);
    
    p4 = my_realloc(p4,232);
    p5 = my_realloc(p5,3);
    
    
    

    my_free(p);
    my_free(p2);
    

    
    
    
    p3 = my_realloc(p3,11);
    p4 = my_realloc(p4,342);
    p5 = my_realloc(p5,321);
    p6 = my_realloc(p6,233);
    p4 = my_realloc(p4,12);
    p3 = my_realloc(p3,11);
    p4 = my_realloc(p4,23);
    p3 = my_realloc(p3,232);
    p4 = my_realloc(p4,68);
    p3 = my_realloc(p3,1025);
    
    p3 = my_realloc(p3,231);
    p4 = my_realloc(p4,145);
    
    
    if(!p3 || !p4 || !p5 || !p6 ){
        printf("malloc error\n");
        return 0;
    }


    my_free(p3);
    my_free(p4);
    my_free(p5);
    my_free(p6);

    heap_report();

    

    while((p = myfgets(stdin)) != NULL){
        p2 = myfgets(stdin);
        printf("%s\n%s\n",p,p2);

        heap_report();
        my_free(p);
        my_free(p2);
        heap_report();
    }

    


    return 0;
}


