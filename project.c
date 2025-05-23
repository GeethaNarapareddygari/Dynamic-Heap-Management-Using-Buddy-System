#include <stdio.h>
#include <stddef.h>  
#include <math.h>
#define MAX_SIZE 8392
char memory[16416];

typedef struct meta_block
{
    size_t size;  //an unsigned integer to store size of object contained in stddef.h        
    int free;                
    struct meta_block *next; 
}meta_block;
 
struct meta_block* store[13]; //creating an array with 13 elements where each pointer stores address of meta block structure
void initialize()
{
    struct meta_block *temp=(void*)memory;
    temp->size=0;   
    temp->free=0;   
    temp->next=NULL;  
    store[0]=temp; 
    int add=2;
    for(int i=1;i<13;i++)
    {
        temp=(void *)((void *)store[i-1]+ add + sizeof(meta_block));//address of each block when we are divided into power of 2 
        store[i]=temp;
        temp->size=0;
        temp->free=0;
        temp->next=NULL;
        add=add*2;
        
    }
}
int findrange(int k)
{
  int count=0;
  int temp=k;
  // checking whether  size is exactly in terms of 2^n 
  if(k%2==0)
  {
      int flag=0;
     while(temp%2==0&&temp>2)
    {
         temp=temp/2;
         if(temp%2!=0)
         {
              flag=1;
          }
      
    }
     if(flag==0)
    {
      count--;      
    }
  }
  while(k>1)
  {
      count++;
      k=k/2;
  }
    return (count+1);
    
}
// My_Alloc function to allocate the given size from heap
void* My_Alloc(int noofBytes)
{
   
    if(noofBytes>MAX_SIZE)
    {
        printf("our memory range is not sufficient");
        return NULL;   
    }
      meta_block* retval=NULL;
      int done=0;
    int index=findrange(noofBytes); 
  
    while(!done&&index<13)
    {
        if(store[index]->free==0&&store[index]->next==NULL)
        {
                store[index]->free=1;
                store[index]->size=noofBytes;
                store[index]->next=NULL;
                retval=store[index];
                done=1;
        }
        else if(store[index]->free==0&&store[index]->size>=noofBytes)
        {
               store[index]->free=1;
                store[index]->size=noofBytes;
                retval=store[index];
                done=1;   
        } 
        else if(store[index]->size+noofBytes<pow(2,index+1))
        {
            
            meta_block* t=store[index];
            int size=t->size+sizeof(meta_block);
            while(t->next!=NULL&&!done)
            {
                if(t->free==0&&t->size>=noofBytes)
                {
                   t->size=noofBytes;
                   t->free=1;
                   done=1;   
                }
                t=t->next;
                size=size+t->size+sizeof(meta_block);//update the total allocated memory
            }

            if(!done)
           { 
            if(size+noofBytes+sizeof(meta_block)<pow(2,index+1))
            {
                meta_block* temp=(void *)(((void*) t )+ t->size+sizeof(meta_block));//move right after the t block 
                temp->size=noofBytes;
                temp->free=1;
                temp->next=NULL;
                t->next=temp;//previous linked list is connected to this
                done=1;
                retval=temp;
            }
            if(done==0)
            {
                index++;
            }
           }
            
        }    
        else
        {
            index++;
        }
    }
    if(done==0)
    {
        printf("\n your memory is not sufficient  "); 
    }
    else
    {
        printf("\n your memory allocation done successfully for size %d",noofBytes);
    } 
   return (void *)retval;
}

int mergeindex(void* ptr)
{
    int index=0;
    int found=0;
    int i=0;
    while(!found)
    {
        if((meta_block*)store[i]<(meta_block*)ptr && (meta_block*)ptr<(meta_block*)store[i+1])
        {
          index=i;
          found=1;
        }
        i++;
    }
  return index;
}

void Merge(int index)
{  
    /*if(store[index]->free==0&&store[index]->next!=NULL&&store[index]->next->free==0)
    {
        store[index]->size+=store[index]->next->size;
        store[index]->next=store[index]->next->next;
    }*/
    meta_block* ptr = store[index]->next;

    while(ptr != NULL && ptr->next != NULL)
    {
        if(ptr->free == 0 && ptr->next->free == 0)
        {
            ptr->size += ptr->next->size;
            ptr->next = ptr->next->next; 
        }
        else
        {
            ptr = ptr->next;
        }
    }
}

void displayMemory()
{
    for (int i = 0; store[i] != NULL; i++)
    {
        printf("Index %d:\n", i);
        meta_block *current = store[i];
        while (current != NULL)
        {
            printf("  Block @ %p | Size: %zu | Free: %d | Next: %p\n", (void *)current, current->size, current->free, (void *)current->next);
            current = current->next;
        }
    }
    printf("\n");
}

void My_Free(void *ptr)
{
    if (((void *)memory <= ptr) && (ptr <= (void *)(memory +16416 )))
    {
       meta_block* temp=ptr;
          temp->free=0;
          printf("\n deallocation done succesfully");
          int index=mergeindex(ptr);
          Merge(index);  
          //displayMemory();
    }
    else
        printf("Please provide a valid pointer allocated\n");

}
int main()
{
    initialize();
    int *p = (int *)My_Alloc(150 * sizeof(int));
    //printf("\n%d",*p);
    char *q = (char *)My_Alloc(200 * sizeof(char));
    int *r = (int *)My_Alloc(1000* sizeof(int));
    char *w = (char *)My_Alloc(700);
    My_Free(w);
    displayMemory();
    int *k = (int *)My_Alloc(500 * sizeof(int));
    My_Free(p);
    My_Free(r);
    int *l = (int *)My_Alloc(500 * sizeof(int));
    int *x = (int *)My_Alloc(500 * sizeof(int));
    int *a = (int *)My_Alloc(500 * sizeof(int));
    int *s = (int *)My_Alloc(500 * sizeof(int));
    int *h = (int *)My_Alloc(500 * sizeof(int)); 
    return 0;
}