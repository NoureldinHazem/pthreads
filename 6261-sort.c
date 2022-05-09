#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#define SIZE 100

//struct to take variables of array &send them in thread function
typedef struct  data
{
int first;
int middle;
int last;
}numbers;

//defining global variable for arr and array size
int *arr=NULL; 
int arraysize;

//function to merge the array
void merge(int first, int middle, int last)
{
	int n = middle - first + 1;
	int m = last - middle;
    
    int temp1[n], temp2[m];

	for (int i = 0; i < n; i++)
		temp1[i] = arr[first + i];
	for (int j = 0; j < m; j++)
		temp2[j] = arr[middle + 1 + j];

	
	int i = 0,j = 0,k=first;
	while (i < n && j < m) {
		if (temp1[i] <= temp2[j]) {
			arr[k] = temp1[i];
			i++;
		}
		else {
			arr[k] = temp2[j];
			j++;
		}
		k++;
	}

	while (i < n) {
		arr[k] = temp1[i];
		i++;
		k++;
	}

	while (j < m) {
		arr[k] = temp2[j];
		j++;
		k++;
	}
}

//thread function to divide the given array to 2 subarrays and create thread for each
void *Sort(void *args)
{
    //local struct to catch the variables send to the function and use them
    numbers *temp = malloc(sizeof(numbers));
    temp=(numbers*) args;

	if (temp->first < temp->last) {

        //creating array of struct of size 2
        numbers *subarray= malloc(sizeof(numbers)*2);

        //creating array of thread of 2 
        pthread_t *thread= malloc(sizeof(pthread_t)*2);

        //passing elements of the right subarray 
        subarray[0].first=temp->first;
        subarray[0].last=temp->middle;
        subarray[0].middle=temp->first + (temp->middle - temp->first)/2;

        //passing elements of the left subarray
        subarray[1].first=(temp->middle)+1;
        subarray[1].last=temp->last;
        subarray[1].middle= ((temp->middle)+1) + (temp->last - ((temp->middle)+1)) /2;
        
        //creating the 2 threads each subarray have its thread
        for (int  i = 0; i < 2; i++)
        {
            pthread_create(&thread[i],NULL,Sort,&subarray[i]);  
        }
        
        //waiting for this 2 threads to finish their work
        for (int i = 0; i < 2; i++)
        {
            pthread_join(thread[i],NULL);
        }
        
        //making the merge function
        merge(temp->first,temp->middle,temp->last);
        
	}
}

void mergeSort(int first, int last)
{
	if (first < last) {
        numbers passedvariables;
		int middle = first + (last - first) / 2;

        //passing the elements of the original array and creating thread
		pthread_t thread;
        passedvariables.first=first;
        passedvariables.last=last;
        passedvariables.middle=middle;
        pthread_create(&thread,NULL,Sort,&passedvariables);

        pthread_join(thread,NULL);
	}
}

//function to print the array
void printArray(int size)
{
	int i;
	for (i = 0; i < size; i++)
		printf("%d ", arr[i]);
	printf("\n");
}

//function to read elements from the given file
void readArray(char *filename)
{
    FILE *f1;
    char text[]=".txt";
    f1=fopen(filename,"r");
        if(f1)
        {
            //taking the first element and this is the array size
           fscanf(f1,"%d",&arraysize);
           fscanf(f1,"\n");
           arr = calloc(arraysize,sizeof(int));

           //taking elements of array from file
             for (int i = 0; i < arraysize; i++)
             {
                fscanf(f1,"%d ",&arr[i]);
             }
                  fclose(f1); 
            }
} 

void fileWrite()
{
   FILE *f1;
  
    f1=fopen("sorted.txt","w");
    fprintf(f1,"Sorted Array:\n");
   for (int i = 0; i < arraysize; i++)
      {
               fprintf(f1,"%d\t",arr[i]);
      }
   fclose(f1);
   return;
}


int main(int argc,char* argv[])
{
 
    readArray(argv[1]);
	printf("Given array is \n");
	printArray(arraysize);

	mergeSort(0,arraysize - 1);

	printf("\nSorted array is \n");
	printArray(arraysize);
   	fileWrite();
	return 0;
}
