#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/mman.h>

#define WORST_FIT 2
#define BEST_FIT 1
#define FULL 1
#define EMPTY 0
int freeBytes;
int holes;
int i = 0;
int maxVal;
int holes =1;

        typedef struct dataStructure {
                int size;
		int flag; //to check if it is empty or not for the chunk
                void *start_mem;
                void *end_mem;
        } dataStructure;

        typedef struct node {
               dataStructure *current;
               dataStructure *next;
               dataStructure *prev;
		void *ptr;		
		int size;
		
        } node;


node *head;

int mall_type;
int pageSize;

void setup(int malloc_type, int mem_size, void* start_of_memory){

	freeBytes = mem_size;

                mall_type = malloc_type;
		head = malloc(mem_size * sizeof(*head));
		head->current = malloc(mem_size * sizeof(*head->current));
		head[0].ptr = start_of_memory;
		head[0].size = mem_size;
}

void *my_malloc(int size){

int *p;
int k;
int besti=0;
int worsti=0;
int bestVal = 9999999;
int worstVal = 0;	
freeBytes = freeBytes -size -4;   // tracking free bytes	

if(mall_type == 0){			////////////////////////////////////FIRST-FIT/////////////////////////////////
 
	if(holes>1){	
	    for(k=0;k<=maxVal; k++){
	        if((head[0].current[k].flag == EMPTY) && (size <= head[0].current[k].size)){  //firstfit
			head[0].current[k].size = size;
			head[0].current[k].flag = FULL;
		//	int *tempB, *b;
		//	tempB= p-4;
		//	b = malloc(4);
		//	*b = size;
		//	memcpy(tempB, b, 4);
			p = head[0].current[k].start_mem;				
			break;			//we break bc first come	
	        }
   	    }
	}
	else{
	    head[0].current[i].size = size;
	    if(i == 0){
		head[0].current[i].start_mem = (head[0].ptr + 4);
	    }
	    else if(i> 0){
		head[0].current[i].start_mem = (head[0].current[(i-1)].start_mem+ (head[0].current[i-1].size) +4); 
	    }
	    head[0].current[i].flag = FULL;
	    p = head[0].current[i].start_mem;
	    i++;
	    maxVal = i;
	}
	return(p);		
}
if(mall_type == 1){ ///////////////////////BEST-FIT///////////////////////////////
	
	 if(holes>1){	
	    for(k=1;k<=maxVal; k++){	
	        if(head[0].current[k].flag == EMPTY && size <= head[0].current[k].size){  //bestfit	
			if(bestVal > (head[0].current[k].size - size)){
				bestVal = (head[0].current[k].size -size);
				besti = k;
			}
	
		}
	      }	
			head[0].current[besti].flag= FULL;
			p = head[0].current[besti].start_mem;
            }
	    else{
	    head[0].current[i].size = size;
	    if(i == 0){
		head[0].current[i].start_mem = (head[0].ptr + 4);
	    }
	    else if(i> 0){
		head[0].current[i].start_mem = (head[0].current[(i-1)].start_mem + (head[0].current[i-1].size) +4); 
	    }
	    head[0].current[i].flag = FULL;
	    p = head[0].current[i].start_mem;
	    i++;
	    maxVal = i;
	   }
            return(p);
			 }

if(mall_type == 2){/////////////////////WORST-FIT////////////////////

	 if(holes>1){	
	    for(k=1;k<=maxVal; k++){	
	        if(head[0].current[k].flag == EMPTY && size <= head[0].current[k].size){  //worst-fit
			if(worstVal < (head[0].current[k].size - size)){
				worstVal = (head[0].current[k].size -size);
				worsti = k;
			}
	
		}
	      }
		if(worstVal< head[0].size){
			//try this
			head[0].current[i].size = size;
	    		head[0].size = head[0].size - size;
	    	
	    		if(i> 0){
			head[0].current[i].start_mem = (head[0].current[(i-1)].start_mem + (head[0].current[i-1].size) +4); 
	 	   	}
	   	 	head[0].current[i].flag = FULL;
	    		p = head[0].current[i].start_mem;
	    		i++;
	    		maxVal = i;
	
		}
		else{	
			head[0].current[worsti].flag= FULL;
			p = head[0].current[worsti].start_mem;
		}
            }
	    else{
	    head[0].current[i].size = size;
	    head[0].size = head[0].size - size;
	    if(i == 0){
		head[0].current[i].start_mem = (head[0].ptr + 4);
	    }
	    else if(i> 0){
		head[0].current[i].start_mem = (head[0].current[(i-1)].start_mem + (head[0].current[i-1].size) +4); 
	    }
	    head[0].current[i].flag = FULL;
	    p = head[0].current[i].start_mem;
	    i++;
	    maxVal = i;
	   }
            return(p);




}



		
}
void my_free(void *ptr){

	int k= 0;
	holes++;
	for(k =0; k<=maxVal; k++){
		if(ptr == head[0].current[k].start_mem){
			head[0].current[k].flag = EMPTY;
		//	if((head[0].current[-1].flag == EMPTY) || (head[0].current[1].flag == EMPTY)){  	//if adjacent(bitisik) add free's together	
		//	}
		//	head[0].current[k].size = 0;
			freeBytes = freeBytes + head[0].current[k].size+ 4;	
		}
	}
}

int num_holes(){

	
	return (holes);
}

int num_free_bytes(){
	
	//for(i; )

	return (freeBytes);
}

int main(){


    int size, FIRST_FIT=0;
    int RAM_SIZE=1<<20;//1024*1024
    void* RAM=malloc(RAM_SIZE);//1024*1024
/*       
//	printf("first fit:%d, ram_size:%d, ram: %d \n", FIRST_FIT, RAM_SIZE, RAM);
	 setup(FIRST_FIT,RAM_SIZE,RAM);//First Fit, Memory size=1024*1024, Start of memory=RAM
	
	//test 1
	size=20*1024;
	printf( "Allocating, size=%d\n" ,  size );
        void* a=my_malloc(size);//We have 4 bytes header to save the size of that chunk in memory so the output starts at 4
	if(num_holes()>0)
        {
           printf( "Average hole size = %.1lf [%d/%d]\n", (float)num_free_bytes()/num_holes(),num_free_bytes(), num_holes());
        }
        else
        {
                printf( "Total Free Bytes = %d, Num Holes = %d\n", num_free_bytes(), num_holes());
        }
    	if ((int)a==-1)
        printf("This size can not be allocated!");
    	else
    	{
//	printf("a:%d, RAM:%d", (int)(a), (int)(RAM));
        printf("start of the chunk a: %d\n",(int)(a-RAM));
        printf("End of the chunk a: %d\n\n",(int)(a+size-RAM));
    	}
	size=30*1024;
        printf("Allocating %s, size=%d\n"," void* b " ,  size );
   	void* b=my_malloc(size);
        if(num_holes()>0)
        {
                printf("Average hole size = %.1lf [%d/%d]\n", (float)num_free_bytes()/num_holes(),num_free_bytes(), num_holes());
        }
        else
        {
                printf("Total Free Bytes = %d, Num Holes = %d\n", num_free_bytes(), num_holes());
        }
	if ((int)b==-1)
        printf("This size can not be allocated!");
    	else
    	{
//		printf("b:%d, RAM:%d", (int)(b), (int)(RAM));

	        printf("start of the chunk b: %d\n",(int)(b-RAM));

        printf("End of the chunk b: %d\n\n",(int)(b+size-RAM));
    	}
	    size=15*1024;
        printf("Allocating %s, size=%d\n"," void* c " ,  size );
        void* c=my_malloc(size);
        if(num_holes()>0)
        {
                printf("Average hole size = %.1lf [%d/%d]\n", (float)num_free_bytes()/num_holes(),num_free_bytes(), num_holes());
        }
        else
        {
                printf("Total Free Bytes = %d, Num Holes = %d\n", num_free_bytes(), num_holes());
        }
	    if ((int)c==-1)
        printf("This size can not be allocated!");
    	else
    	{
        printf("start of the chunk c: %d\n",(int)(c-RAM));
        printf("End of the chunk c: %d\n\n",(int)(c+size-RAM));
    	}

	   size=25*1024;
        printf("Allocating %s, size=%d\n"," void* d " ,  size );
        void* d=my_malloc(size);
        if(num_holes()>0)
        {
                printf("Average hole size = %.1lf [%d/%d]\n", (float)num_free_bytes()/num_holes(),num_free_bytes(), num_holes());
        }
        else
        {
                printf("Total Free Bytes = %d, Num Holes = %d\n", num_free_bytes(), num_holes());
        }
	    if ((int)d==-1)
        printf("This size can not be allocated!");
   	 else
   	{
        printf("start of the chunk d: %d\n",(int)(d-RAM));
        printf("End of the chunk d: %d\n\n",(int)(d+size-RAM));
    	}

    size=35*1024;
        printf("Allocating %s, size=%d\n"," void* e " ,  size );
        void* e=my_malloc(size);
        if(num_holes()>0)
        {
                printf("Average hole size = %.1lf [%d/%d]\n", (float)num_free_bytes()/num_holes(),num_free_bytes(), num_holes());
        }
        else
        {
                printf("Total Free Bytes = %d, Num Holes = %d\n", num_free_bytes(), num_holes());
        }
   	 if ((int)e==-1)
        printf("This size can not be allocated!");
    	else
    	{
        printf("start of the chunk e: %d\n",(int)(e-RAM));
        printf("End of the chunk e: %d\n\n",(int)(e+size-RAM));
    	}	
	
	size=35*1024;
        printf("Allocating %s, size=%d\n"," void* f " ,  size );
        void* f=my_malloc(size);
        if(num_holes()>0)
        {
                printf("Average hole size = %.1lf [%d/%d]\n", (float)num_free_bytes()/num_holes(),num_free_bytes(), num_holes());
        }
        else
        {
                printf("Total Free Bytes = %d, Num Holes = %d\n", num_free_bytes(), num_holes());
        }
    	if ((int)f==-1)
        printf("This size can not be allocated!");
    	else
    	{
        printf("start of the chunk f: %d\n",(int)(f-RAM));
        printf("End of the chunk f: %d\n\n",(int)(f+size-RAM));
   	 }

    	size=25*1024;
        printf("Allocating %s, size=%d\n"," void* g " ,  size );
        void* g=my_malloc(size);
        if(num_holes()>0)
        {
                printf("Average hole size = %.1lf [%d/%d]\n", (float)num_free_bytes()/num_holes(),num_free_bytes(), num_holes());
        }
        else
        {
                printf("Total Free Bytes = %d, Num Holes = %d\n", num_free_bytes(), num_holes());
        }
    	if ((int)g==-1)
        printf("This size can not be allocated!");
    	else
    	{
        printf("start of the chunk g: %d\n",(int)(g-RAM));
        printf("End of the chunk g: %d\n\n",(int)(g+size-RAM));
    	}
	
	printf("Freeing %s, size=%d\n"," b " , (int)(*(int *)((char*) b -4) ));
	my_free(b);
        if(num_holes()>0)
        {
                printf("Average hole size = %.1lf [%d/%d]\n", (float)num_free_bytes()/num_holes(),num_free_bytes(), num_holes());
        }
        else
        {
                printf("Total Free Bytes = %d, Num Holes = %d\n", num_free_bytes(), num_holes());
        }
       // printf("Freeing %s, size=%d\n"," d " , (int)(*(int *)((char*) d -4) ));
	printf("Freeing %s, size=%d\n"," d " , (int)(*(int *)((char*) d -4) ));
        my_free(d);
        if(num_holes()>0)
        {
         printf("Average hole size = %.1lf [%d/%d]\n", (float)num_free_bytes()/num_holes(),num_free_bytes(), num_holes());
        }
        else
        {
                printf( "Total Free Bytes = %d, Num Holes = %d\n", num_free_bytes(), num_holes());
        }
        printf("Freeing %s, size=%d\n"," f " , (int)(*(int *)((char*) f -4) ));
        my_free(f);
        if(num_holes()>0)
        {
                printf("Average hole size = %.1lf [%d/%d]\n", (float)num_free_bytes()/num_holes(),num_free_bytes(), num_holes());
        }
        else
        {
                printf("Total Free Bytes = %d, Num Holes = %d\n", num_free_bytes(), num_holes());
        }

    size=25*1024;
        printf("Allocating %s, size=%d\n"," void* h " ,  size );
        void* h=my_malloc(size);
        if(num_holes()>0)
        {
                printf("Average hole size = %.1lf [%d/%d]\n", (float)num_free_bytes()/num_holes(),num_free_bytes(), num_holes());
        }
        else
        {
                printf("Total Free Bytes = %d, Num Holes = %d\n", num_free_bytes(), num_holes());
        }
    if ((int)h==-1)
        printf("This size can not be allocated!");
    else
    {
        printf("start of the chunk h: %d\n",(int)(h-RAM));
        printf("End of the chunk h: %d\n\n",(int)(h+size-RAM));
    }


*/
/*
setup(BEST_FIT,RAM_SIZE,RAM);//Best Fit, Memory size=1024*1024, Start of memory=RAM
//test 2
    size=20*1024;
	printf( "Allocating %s, size=%d\n"," void* a " ,  size );
	void* a=my_malloc(size);//We have 4 bytes header to save the size of that chunk in memory so the output starts at 4
	if(num_holes()>0)
	{
		printf("Average hole size = %.1lf [%d/%d]\n", (float)num_free_bytes()/num_holes(),num_free_bytes(), num_holes());
	}
	else
	{
		printf("Total Free Bytes = %d, Num Holes = %d\n", num_free_bytes(), num_holes());
	}
    if ((int)a==-1)
        printf("This size can not be allocated!");
    else
    {
    	printf("start of the chunk a: %d\n",a-RAM);
    	printf("End of the chunk a: %d\n\n",a+size-RAM);
    }

    size=30*1024;
	printf( "Allocating %s, size=%d\n"," void* b " ,  size );
    void* b=my_malloc(size);
	if(num_holes()>0)
	{
		printf("Average hole size = %.1lf [%d/%d]\n", (float)num_free_bytes()/num_holes(),num_free_bytes(), num_holes());
	}
	else
	{
		printf("Total Free Bytes = %d, Num Holes = %d\n", num_free_bytes(), num_holes());
	}
    if ((int)b==-1)
        printf("This size can not be allocated!");
    else
    {
    	printf("start of the chunk b: %d\n",b-RAM);
    	printf("End of the chunk b: %d\n\n",b+size-RAM);
    }

    size=15*1024;
	printf("Allocating %s, size=%d\n"," void* c " ,  size );
	void* c=my_malloc(size);
	if(num_holes()>0)
	{
		printf("Average hole size = %.1lf [%d/%d]\n", (float)num_free_bytes()/num_holes(),num_free_bytes(), num_holes());
	}
	else
	{
		printf("Total Free Bytes = %d, Num Holes = %d\n", num_free_bytes(), num_holes());
	}
    if ((int)c==-1)
        printf("This size can not be allocated!");
    else
    {
    	printf("start of the chunk c: %d\n",c-RAM);
    	printf("End of the chunk c: %d\n\n",c+size-RAM);
    }

    size=25*1024;
	printf("Allocating %s, size=%d\n"," void* d " ,  size );
	void* d=my_malloc(size);
	if(num_holes()>0)
	{
		printf("Average hole size = %.1lf [%d/%d]\n", (float)num_free_bytes()/num_holes(),num_free_bytes(), num_holes());
	}
	else
	{
		printf("Total Free Bytes = %d, Num Holes = %d\n", num_free_bytes(), num_holes());
	}
    if ((int)d==-1)
        printf("This size can not be allocated!");
    else
    {
    	printf("start of the chunk d: %d\n",d-RAM);
    	printf("End of the chunk d: %d\n\n",d+size-RAM);
    }

    size=35*1024;
	printf("Allocating %s, size=%d\n"," void* e " ,  size );
	void* e=my_malloc(size);
	if(num_holes()>0)
	{
		printf("Average hole size = %.1lf [%d/%d]\n", (float)num_free_bytes()/num_holes(),num_free_bytes(), num_holes());
	}
	else
	{
		printf("Total Free Bytes = %d, Num Holes = %d\n", num_free_bytes(), num_holes());
	}
    if ((int)e==-1)
        printf("This size can not be allocated!");
    else
    {
    	printf("start of the chunk e: %d\n",e-RAM);
    	printf("End of the chunk e: %d\n\n",e+size-RAM);
    }

    size=35*1024;
	printf("Allocating %s, size=%d\n"," void* f " ,  size );
	void* f=my_malloc(size);
	if(num_holes()>0)
	{
		printf("Average hole size = %.1lf [%d/%d]\n", (float)num_free_bytes()/num_holes(),num_free_bytes(), num_holes());
	}
	else
	{
		printf("Total Free Bytes = %d, Num Holes = %d\n", num_free_bytes(), num_holes());
	}
    if ((int)f==-1)
        printf("This size can not be allocated!");
    else
    {
    	printf( "start of the chunk f: %d\n",f-RAM);
    	printf("End of the chunk f: %d\n\n",f+size-RAM);
    }

    size=25*1024;
	printf("Allocating %s, size=%d\n"," void* g " ,  size );
	void* g=my_malloc(size);
	if(num_holes()>0)
	{
		printf("Average hole size = %.1lf [%d/%d]\n", (float)num_free_bytes()/num_holes(),num_free_bytes(), num_holes());
	}
	else
	{
		printf( "Total Free Bytes = %d, Num Holes = %d\n", num_free_bytes(), num_holes());
	}
    if ((int)g==-1)
        printf("This size can not be allocated!");
    else
    {
    	printf("start of the chunk g: %d\n",g-RAM);
    	printf( "End of the chunk g: %d\n\n",g+size-RAM);
    }

	printf("Freeing %s, size=%d\n"," b " , (int)(*(int *)((char*) b -4) ));
	my_free(b);
	if(num_holes()>0)
	{
		printf("Average hole size = %.1lf [%d/%d]\n", (float)num_free_bytes()/num_holes(),num_free_bytes(), num_holes());
	}
	else
	{
		printf("Total Free Bytes = %d, Num Holes = %d\n", num_free_bytes(), num_holes());
	}
	printf("Freeing %s, size=%d\n"," d " , (int)(*(int *)((char*) d -4) ));
	my_free(d);
	if(num_holes()>0)
	{
		printf( "Average hole size = %.1lf [%d/%d]\n", (float)num_free_bytes()/num_holes(),num_free_bytes(), num_holes());
	}
	else
	{
		printf("Total Free Bytes = %d, Num Holes = %d\n", num_free_bytes(), num_holes());
	}
	printf( "Freeing %s, size=%d\n"," f " , (int)(*(int *)((char*) f -4) ));
	my_free(f);
	if(num_holes()>0)
	{
		printf("Average hole size = %.1lf [%d/%d]\n", (float)num_free_bytes()/num_holes(),num_free_bytes(), num_holes());
	}
	else
	{
		printf( "Total Free Bytes = %d, Num Holes = %d\n", num_free_bytes(), num_holes());
	}

    size=25*1024;
	printf("Allocating %s, size=%d\n"," void* h " ,  size );
	void* h=my_malloc(size);
	if(num_holes()>0)
	{
		printf("Average hole size = %.1lf [%d/%d]\n", (float)num_free_bytes()/num_holes(),num_free_bytes(), num_holes());
	}
	else
	{
		printf("Total Free Bytes = %d, Num Holes = %d\n", num_free_bytes(), num_holes());
	}
    if ((int)h==-1)
        printf("This size can not be allocated!");
    else
    {
    	printf( "start of the chunk h: %d\n",h-RAM);
    	printf( "End of the chunk h: %d\n\n",h+size-RAM);
    }
*/

setup(WORST_FIT,RAM_SIZE,RAM);//Worst Fit, Memory size=1024*1024, Start of memory=RAM
//test 3
    size=20*1024;
	printf( "Allocating %s, size=%d\n"," void* a " ,  size );
	void* a=my_malloc(size);//We have 4 bytes header to save the size of that chunk in memory so the output starts at 4
	if(num_holes()>0)
	{
		printf("Average hole size = %.1lf [%d/%d]\n", (float)num_free_bytes()/num_holes(),num_free_bytes(), num_holes());
	}
	else
	{
		printf("Total Free Bytes = %d, Num Holes = %d\n", num_free_bytes(), num_holes());
	}
    if ((int)a==-1)
        printf("This size can not be allocated!");
    else
    {
    	printf( "start of the chunk a: %d\n",a-RAM);
    	printf( "End of the chunk a: %d\n\n",a+size-RAM);
    }

    size=30*1024;
	printf("Allocating %s, size=%d\n"," void* b " ,  size );
    void* b=my_malloc(size);
	if(num_holes()>0)
	{
		printf("Average hole size = %.1lf [%d/%d]\n", (float)num_free_bytes()/num_holes(),num_free_bytes(), num_holes());
	}
	else
	{
		printf( "Total Free Bytes = %d, Num Holes = %d\n", num_free_bytes(), num_holes());
	}
    if ((int)b==-1)
        printf("This size can not be allocated!");
    else
    {
    	printf( "start of the chunk b: %d\n",b-RAM);
    	printf( "End of the chunk b: %d\n\n",b+size-RAM);
    }

    size=15*1024;
	printf("Allocating %s, size=%d\n"," void* c " ,  size );
	void* c=my_malloc(size);
	if(num_holes()>0)
	{
		printf("Average hole size = %.1lf [%d/%d]\n", (float)num_free_bytes()/num_holes(),num_free_bytes(), num_holes());
	}
	else
	{
		printf("Total Free Bytes = %d, Num Holes = %d\n", num_free_bytes(), num_holes());
	}
    if ((int)c==-1)
        printf("This size can not be allocated!");
    else
    {
    	printf( "start of the chunk c: %d\n",c-RAM);
    	printf( "End of the chunk c: %d\n\n",c+size-RAM);
    }

    size=25*1024;
	printf( "Allocating %s, size=%d\n"," void* d " ,  size );
	void* d=my_malloc(size);
	if(num_holes()>0)
	{
		printf("Average hole size = %.1lf [%d/%d]\n", (float)num_free_bytes()/num_holes(),num_free_bytes(), num_holes());
	}
	else
	{
		printf("Total Free Bytes = %d, Num Holes = %d\n", num_free_bytes(), num_holes());
	}
    if ((int)d==-1)
        printf("This size can not be allocated!");
    else
    {
    	printf("start of the chunk d: %d\n",d-RAM);
    	printf("End of the chunk d: %d\n\n",d+size-RAM);
    }

    size=35*1024;
	printf("Allocating %s, size=%d\n"," void* e " ,  size );
	void* e=my_malloc(size);
	if(num_holes()>0)
	{
		printf("Average hole size = %.1lf [%d/%d]\n", (float)num_free_bytes()/num_holes(),num_free_bytes(), num_holes());
	}
	else
	{
		printf( "Total Free Bytes = %d, Num Holes = %d\n", num_free_bytes(), num_holes());
	}
    if ((int)e==-1)
        printf("This size can not be allocated!");
    else
    {
    	printf("start of the chunk e: %d\n",e-RAM);
    	printf( "End of the chunk e: %d\n\n",e+size-RAM);
    }

    size=35*1024;
	printf("Allocating %s, size=%d\n"," void* f " ,  size );
	void* f=my_malloc(size);
	if(num_holes()>0)
	{
		printf( "Average hole size = %.1lf [%d/%d]\n", (float)num_free_bytes()/num_holes(),num_free_bytes(), num_holes());
	}
	else
	{
		printf( "Total Free Bytes = %d, Num Holes = %d\n", num_free_bytes(), num_holes());
	}
    if ((int)f==-1)
        printf( "This size can not be allocated!");
    else
    {
    	printf("start of the chunk f: %d\n",f-RAM);
    	printf("End of the chunk f: %d\n\n",f+size-RAM);
    }

    size=25*1024;
	printf("Allocating %s, size=%d\n"," void* g " ,  size );
	void* g=my_malloc(size);
	if(num_holes()>0)
	{
		printf( "Average hole size = %.1lf [%d/%d]\n", (float)num_free_bytes()/num_holes(),num_free_bytes(), num_holes());
	}
	else
	{
		printf( "Total Free Bytes = %d, Num Holes = %d\n", num_free_bytes(), num_holes());
	}
    if ((int)g==-1)
        printf("This size can not be allocated!");
    else
    {
    	printf("start of the chunk g: %d\n",g-RAM);
    	printf( "End of the chunk g: %d\n\n",g+size-RAM);
    }

	printf("Freeing %s, size=%d\n"," b " , (int)(*(int *)((char*) b -4) ));
	my_free(b);
	if(num_holes()>0)
	{
		printf("Average hole size = %.1lf [%d/%d]\n", (float)num_free_bytes()/num_holes(),num_free_bytes(), num_holes());
	}
	else
	{
		printf("Total Free Bytes = %d, Num Holes = %d\n", num_free_bytes(), num_holes());
	}
	printf( "Freeing %s, size=%d\n"," d " , (int)(*(int *)((char*) d -4) ));
	my_free(d);
	if(num_holes()>0)
	{
		printf( "Average hole size = %.1lf [%d/%d]\n", (float)num_free_bytes()/num_holes(),num_free_bytes(), num_holes());
	}
	else
	{
		printf("Total Free Bytes = %d, Num Holes = %d\n", num_free_bytes(), num_holes());
	}
	printf( "Freeing %s, size=%d\n"," f " , (int)(*(int *)((char*) f -4) ));
	my_free(f);
	if(num_holes()>0)
	{
		printf("Average hole size = %.1lf [%d/%d]\n", (float)num_free_bytes()/num_holes(),num_free_bytes(), num_holes());
	}
	else
	{
		printf("Total Free Bytes = %d, Num Holes = %d\n", num_free_bytes(), num_holes());
	}

    size=25*1024;
	printf( "Allocating %s, size=%d\n"," void* h " ,  size );
	void* h=my_malloc(size);
	if(num_holes()>0)
	{
		printf("Average hole size = %.1lf [%d/%d]\n", (float)num_free_bytes()/num_holes(),num_free_bytes(), num_holes());
	}
	else
	{
		printf("Total Free Bytes = %d, Num Holes = %d\n", num_free_bytes(), num_holes());
	}
    if ((int)h==-1)
        printf("This size can not be allocated!");
    else
    {
    	printf("start of the chunk h: %d\n",h-RAM);
    	printf("End of the chunk h: %d\n\n",h+size-RAM);
    }
	printf("endofprogram\n");

return 0;
}
