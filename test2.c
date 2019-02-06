#include <stdio.h>

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

int logBase2( int a)
{
 int total = ((int)sizeof(int))*8;
 if (a < 0)
    return; 
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

int main()
{
  /*int base = 2;
  for(int i = 0; i < 11; i++)
  {
    //printf("power = %d\nbase = %d\nresult = %d\n",i,base,powFunc(i,base));
    printf("power = %d\nresult = %d\n",i,powFunc(i,base));
    
  }*/
  /*
  int size = 2049;
  printf("size = %d\n index: = %d\n",size, getIndex(size));*/
   for(int i = 1; i < 512; i= i<<1)
   {
    printf("log of %d = %d\n",i,logBase2(i));
    if(i == 0)
        i = 1;
   }
  //printf("%ld",sizeof(int));
}