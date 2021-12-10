#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<pthread.h>
#include<fcntl.h>
#include<limits.h>
#include<time.h>


typedef struct 
{
	double *array;
	unsigned int index_low;
	unsigned int index_high;
	unsigned int size;
	unsigned int count;
	
}parameters;

void *sub_array_sort (void *);
void sub_array_merge(double *a,unsigned int,unsigned int,unsigned int);
void print_array(double *a, unsigned int size,unsigned int,unsigned int);
void sequential_merge_sort(double *a, unsigned int low,unsigned int high);
void multithreaded_merge(double *a , unsigned int size, pthread_t* thread, int number_of_threads);
void multithreaded_merge_time_only(double *a , unsigned int size, pthread_t* thread, int number_of_threads);
void set_parameters(parameters *par,double *a,unsigned int low,unsigned int high,unsigned int size,unsigned int count);
void merge_sorted_threads(double *a , unsigned int size , int number_of_threads);
void merge_sorted_threads_time_only(double *a , unsigned int size, int number_of_threads);
void InputAnalysis(void);
void TimeAnalysis(void);

int main()
{
	char c;
	printf("------ENTER '1' FOR TESTING ON INPUT ARRAYS-------\n");
	printf("---ENTER '2' FOR TIME ANALYSIS ON RANDOM ARRAYS---\n");
	scanf("%c",&c);
	switch(c)
	{
		case '1':
			InputAnalysis();
			break;
		
		case '2':		
			TimeAnalysis();
			break;
		default:
		    printf("Invalid Input!,input either '1' or '2' ");
		    break;
		}
}
void InputAnalysis(void)
{
	
	int number_of_threads;
	unsigned int size;
	printf("----Enter Number of threads------\n");
	scanf("%d",&number_of_threads);
	//number_of_threads = 4;

	
	pthread_t thread[number_of_threads];
	
	printf("----Enter Size Of Array----------\n");
	scanf("%ud",&size);
	//size = 12;
	
	if(number_of_threads>size)
	{
		fprintf(stderr,"Too many threads of execution required,threads must be smaller than size!\n");
		exit(-1);
	}
	
	//since the array behaves as data that needs to be shared between several threads, it would be beneficial to define it on heap
	double *a = (double *)malloc(size * sizeof(double));
	
	printf("----Enter Array Elements---------\n");
	for (int i =0 ; i<size ; i++)
	{
     	scanf("%lf",&a[i]);
	    //a[i] = 3.4 - 0.1*i;
	}
	
    printf("Original array :\n");	
    print_array(a,size,0,size-1);
    
    multithreaded_merge(a,size,thread,number_of_threads);
    
    FILE *f;
    f = fopen("SortedArray.txt","w");
    fprintf(f,"SORTED ARRAY \n");
    for (int i =0 ;i<size; i++)
    {
		fprintf(f,"%.2f ",a[i]);
	}
	fclose(f);
    
    printf("--------Final Merged Array(Output File Created)----------- :\n");
	print_array(a,size,0,size-1);

	 
	
}
void TimeAnalysis(void)
{
	int number_of_threads[] = {1,2,4,8};
	unsigned int size;
	clock_t start,end,start_seq,end_seq;
	int count = 0;
	printf("---------THREAD ANALYSIS-------\n");
	while(count<4)
    {
	
			pthread_t thread[number_of_threads[count]];
			
			size = 10000;

			double *a = (double *)malloc(size * sizeof(double));
			double *b = (double *)malloc(size * sizeof(double));
			for (int i =0 ; i<size ; i++)
			{
				a[i] = rand()%1000;
				b[i] = a[i];
			}
			
			
			start_seq = clock();
			sequential_merge_sort(b,0,size-1);
			end_seq = clock();
			printf("Threads =  %d | ",number_of_threads[count]);
			printf("Size = %d ",size);
			
			double time_spent_seq = ((double)(end_seq - start_seq))/CLOCKS_PER_SEC;
			printf("| SEQUENTIAL TIME : %lf ",time_spent_seq);
			
			start = clock();
			multithreaded_merge_time_only(a,size,thread,number_of_threads[count]);
			end = clock();
			
			double time_spent = ((double)(end -start))/CLOCKS_PER_SEC;
			printf("| TIME SPENT MUTLITHREADED |: %lf \n",time_spent);
			count = count +1;
  }
    printf("----------SIZE ANALYSIS--------\n");
    int num_of_threads = 16;
    unsigned int size_array[] = {10000,20000,30000,40000,50000};
    count = 0;
	while(count<5)
    {
	        size = size_array[count];
			pthread_t thread[num_of_threads];
			

			double *a = (double *)malloc(size * sizeof(double));
			double *b = (double *)malloc(size * sizeof(double));
			for (int i =0 ; i<size ; i++)
			{
				a[i] = rand()%1000;
				b[i] = a[i];
			}
			
			
			start_seq = clock();
			sequential_merge_sort(b,0,size-1);
			end_seq = clock();
			printf("Threads = %d | ",num_of_threads);
			printf("Size = %d ",size);
			
			double time_spent_seq = ((double)(end_seq - start_seq))/CLOCKS_PER_SEC;
			printf("| SEQUENTIAL TIME : %lf ",time_spent_seq);
			
			start = clock();
			multithreaded_merge_time_only(a,size,thread,num_of_threads);
			end = clock();
			
			double time_spent = ((double)(end -start))/CLOCKS_PER_SEC;
			printf("| TIME SPENT MUTLITHREADED |: %lf \n",time_spent);
			count = count +1;
  }

  
	
	 
	
}
void set_parameters(parameters *par,double *a,unsigned int low,unsigned int high,unsigned int size,unsigned int count)
{
	   par->array = a;
	   par->size = size;
	   par->index_low = low;
	   par->index_high = high;
	   par->count = count;
}
void multithreaded_merge_time_only(double *a ,unsigned int size , pthread_t *thread , int number_of_threads)
{
	
	if(size%number_of_threads ==0)
    {
				
				int step = size/(number_of_threads);
				
				int count = 0;
				
				
				for(unsigned int j = 0; j<size ; j+=step)
				{
					   parameters *par = (parameters *)malloc(sizeof(parameters));
	
					   set_parameters(par,a,j,j+step-1,size,count);
					   pthread_create(&thread[count],NULL,sub_array_sort,(void*)par);
					   count = count +1;
				}
				
				for (int i =0 ; i<number_of_threads ; i++)
				{
					  pthread_join(thread[i],NULL);
				}
			
				merge_sorted_threads_time_only(a,size,number_of_threads);				
				
	}
	
}
void multithreaded_merge(double *a ,unsigned int size,pthread_t *thread,int number_of_threads)
{
	if(size%number_of_threads ==0)
    {
				
				int step = size/(number_of_threads);
				
				int count = 0;
				
				
				for(unsigned int j = 0; j<size ; j+=step)
				{
					   parameters *par = (parameters *)malloc(sizeof(parameters));

					   printf("Thread :%u interval = %u,%u\n",count+1,j,j+step-1);
					   set_parameters(par,a,j,j+step-1,size,count);
					   pthread_create(&thread[count],NULL,sub_array_sort,(void*)par);
					   count = count +1;
				}
				
				for (int i =0 ; i<number_of_threads ; i++)
				{
					  pthread_join(thread[i],NULL);
				}
						
				printf("--------After Thread Merge sort------- :\n");			
				merge_sorted_threads(a,size,number_of_threads);				
				print_array(a,size,0,size-1);
				
	}
	else
	{
		
		
		
		        int step = size/(number_of_threads);
		        int final_low = step*(number_of_threads-1);
		        int final_high = size-1;
				
				int count = 0;
				
				
				for(unsigned int j = 0; j< final_low ; j+=step)
				{
					   parameters *par = (parameters *)malloc(sizeof(parameters));
					   printf("Thread :%u interval = %u,%u\n",count+1,j,j+step-1);
					   set_parameters(par,a,j,j+step-1,size,count);
					   pthread_create(&thread[count],NULL,sub_array_sort,(void*)par);
					   count = count +1;
				}
				
				parameters *par_final = (parameters*)malloc(sizeof(parameters));

				printf("Thread :%u interval = %u,%u\n",count+1,final_low,final_high);
				set_parameters(par_final,a,final_low,final_high,size,count);
				pthread_create(&thread[count],NULL,sub_array_sort,(void*)par_final);
				
				
				
				for (int i =0 ; i<number_of_threads ; i++)
				{
					  pthread_join(thread[i],NULL);
				}
				
				printf("--------After Thread Merge sort------- :\n");
				print_array(a,size,0,size-1);
				merge_sorted_threads(a,final_low,number_of_threads-1);
				print_array(a,size,0,size-1);
				sub_array_merge(a,0,final_low-1,final_high);
					
				
		
	}
}

void print_array(double *a ,unsigned int size,unsigned int start ,unsigned int end)
{
	for (int i = start ;i<=end ; i+=1)
	{
		printf ("%.2f ",a[i]);
		
	}
	printf("\n");
}
void sub_array_merge(double *a, unsigned int low,unsigned int mid ,unsigned int high)
{

	unsigned int size_left = mid - low + 1;
	unsigned int size_right = high - mid;
	
	double *left = (double *)malloc((size_left + 1)*sizeof(double));
	double *right = (double *)malloc((size_right +1)*sizeof(double));
	
	for (int i = 0 ; i<size_left; i++)
	{
		left[i] = a[low + i];
	}
	for( int i = 0 ; i<size_right; i++)
	{
		right[i] = a[mid + i + 1];
	}

    
	
	left[size_left] = (double)(INT_MAX);
	right[size_right] = (double)(INT_MAX);
	int i =0;
	int j =0;
	
	for (int k = low ; k <= high ; k++)
	{
	    if(left[i] <= right[j])
	    {
			a[k] = left[i];
			i = i+1;
		}
		else
		{
			a[k] = right[j];
			j = j + 1;
		}       	 
	}

}
void *sub_array_sort(void * param)
{
	 
     parameters* par = (parameters*)param;
     unsigned int low = par->index_low;
     unsigned int high = par->index_high;
     unsigned int size = par->size;
     unsigned int count = par->count;
     double * a = par->array;

  
     
     //now its time to call the merge sort function to sort the array !
     if(low < high){
		  unsigned int mid = ((low + high)/2);
		  parameters *left_part = (parameters*)malloc(sizeof(parameters)); 
		  parameters *right_part = (parameters*)malloc(sizeof(parameters)); 

	      set_parameters(left_part,a,low,mid,size,count);
	      
	      sub_array_sort((void *)left_part);
	  
	      set_parameters(right_part,a,mid+1,high,size,count);
	      
	      sub_array_sort((void *)right_part);
	      
	      sub_array_merge(a,low,mid,high);

	        	 
	 }
	 
return NULL;
}
void merge_sorted_threads_time_only(double *a , unsigned int size, int number_of_threads)
{
	int step = size/(number_of_threads);
	if(number_of_threads <=1)
	{
		return;
	}
	if(number_of_threads%2 ==0)
	{
		int low = 0;
		int high = 0;
		int mid = 0;
		int cnt  = 1;
		    while(1)
		    {
			   high = (cnt+1)*step -1;
			   mid = cnt*step -1;
			   low = (cnt -1)*step;
			   if(high>=size)
			   {
				   break;
			   }
	     	   sub_array_merge(a,low,mid,high);
	     	   cnt = cnt +1;
	        }
	     number_of_threads = number_of_threads/2;
	     merge_sorted_threads_time_only(a,size,number_of_threads);   
     }
}
void merge_sorted_threads(double *a , unsigned int size, int number_of_threads)
{
	int step = size/number_of_threads;

	int low =0;
	int high = 0;
	int mid =0;
	int cnt = 1;
	   	
	while(1)
	{
	   high =(cnt+1)*step -1;
	   low = 0;
	   mid = cnt*step -1;
	   if(high>=size)
	   {break;}
	   sub_array_merge(a,low,mid,high);
	   cnt = cnt +1;
	}
	
	
}
void sequential_merge_sort(double *a ,unsigned int low,unsigned int high)
{
	unsigned int mid = (low+high)/2;
	
	if(low<high)
	{
		sequential_merge_sort(a,low,mid);
		sequential_merge_sort(a,mid+1,high);
		sub_array_merge(a,low,mid,high);
	}
}

