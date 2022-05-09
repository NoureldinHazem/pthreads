#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#define SIZE 1000

//Global Variables to know & store variables 
int row1,column1,row2,column2;
int array1[SIZE][SIZE];
int array2[SIZE][SIZE];
int result[SIZE][SIZE];

//To store time taken by each case
double total1,total2;

//struct to know the row and number (it is used in case 1)
struct Number
{
   int row;
   int column;
}n1[SIZE];

//function to read matrix from file and store them in global variables
int readMatrix(char *filename)
{
         FILE *f1;
         f1=fopen(filename,"r");
        if(f1)
        {
           //to read number of rows & columns of array 1
           fscanf(f1,"%d %d",&row1,&column1);
           fscanf(f1,"\n");
            //to read the data of array 1 and store them
             for (int i = 0; i < row1; i++)
             {
                     for (int j = 0; j < column1; j++)
                        {
                           fscanf(f1,"%d ",&array1[i][j]);
                        }
                        fscanf(f1,"\n");
             }
             //to read number of rows & columns of array 2
            fscanf(f1,"%d %d",&row2,&column2);
            fscanf(f1,"\n");
             //to read the data of array 2 and store them
                for (int i = 0; i < row2; i++)
                  {
                     for (int j = 0; j < column2; j++)
                        {
                           fscanf(f1,"%d ",&array2[i][j]);
                        }
                        fscanf(f1,"\n");
                  }
                  fclose(f1); 
                  // to check matrix multiplication can be done or not 
                  if(column1 != row2)
                  {
                     puts("\nMatrix Multiplication can't be done!!");
                     return 1;                  
                  }  
            }
   return 0; 
} 

//thread function in case 1
//it make local struct to know the row & column and then start calculating the sum that will be stored in this row & column of result matrix
void *calculate(void *arg)
{
   struct Number* temp=(struct Number*) arg;
   int sum =0;
   for(int i=0;i<column1;i++)
   {   
     sum = sum + array1[temp->row][i] * array2[i][temp->column];
   }
   result[temp->row][temp->column]=sum;
   
   pthread_exit(0);
}

//thread function in case 2
//it make local array to know number of array the it will calculate all the columns of it and store the result of the whole row in the result matrix
void *calc(void *arg)
{
   int *data = (int*) arg;
   int sum=0;
   for(int i=0;i<column2;i++)
   {
      for(int j=0;j<column1;j++)
      {
         sum=sum+array1[data[0]][j]*array2[j][i];
      }
      result[data[0]][i]=sum;
      sum=0;
   }
   pthread_exit(0);
}

//function that know number of threads and create them in case 1
int matrixMultiply1()
{
   int threads=row1*column2;
   int count=0;
   

   pthread_t th[threads]; 

   for(int i=0; i<row1;i++)
   {
      for(int j=0;j<column2;j++)
      {
         n1[count].row=i;
         n1[count].column=j;
        pthread_create(&(th[count]),NULL,calculate,(void *)&n1[count]);
        count=count+1;
      }
      
   }
   for (int i = 0; i < threads; i++)
    {
      pthread_join(th[i],NULL);     
    }
   return 0;
}

//function that know number of threads and create them in case 2
int matrixMultiply2()
{
   int threads=row1;
   int count=0;
   int Numbers[row1];

   pthread_t th[threads]; 

   for(int i=0; i<row1;i++)
   { 
      Numbers[count]=i;
      pthread_create(&(th[count]),NULL,calc,&Numbers[count]);
      count=count+1;
   }
   for (int i = 0; i < threads; i++)
    {
      pthread_join(th[i],NULL);     
    }
   return 0;
}

//function that write the result of the matrix multiplication & time taken into file (for both cases)
void fileWrite(int x)
{
   FILE *f1;
   if (x==0)
   {
    f1=fopen("result.txt","w");
    fprintf(f1,"By Element:\n");
   }
   else if (x==1)
   {
      f1=fopen("result.txt","a");
      fprintf(f1,"By Row:\n");
   }
   else 
   {
    f1=fopen("result.txt","w");
    fprintf(f1,"Matrix Multiplication can't be done!!\n");
    fclose(f1);
    exit (0);
   }
   for (int i = 0; i < row1; i++)
      {
         for (int j = 0; j < column2; j++)
            {
               fprintf(f1,"%d\t", result[i][j]);
            }
         fprintf(f1,"\n");
      }
   if(x==0)
   {
      fprintf(f1,"END1\tTime Taken = %f sec\n\n",total1);
   }
   else
   {
      fprintf(f1,"END2\tTime Taken = %f sec\n",total2);
   }
      
   fclose(f1);
   return;
}

//function to set all the result matrix elememts to 0 after case 1 finish to use same result matrix for case 2
void ResetMatrix()
{
   for (int i = 0; i < SIZE; i++)
    for (int j = 0; j < SIZE; j++)
        result[i][j] = 0;
}

int main(int argc,char* argv[])
{
clock_t start, end;
int check;  //to know if the 2 matrix can do multiplication or not
int flag;

check=readMatrix(argv[1]);
if(check == 0)
{
   
   start = clock();
   matrixMultiply1();
   puts("\nMultiplication by element done");
   end = clock();
   //to calculte time taken in case 1
   total1 = ((double) (end - start)) / CLOCKS_PER_SEC;
   flag=0;
   fileWrite(flag);

   ResetMatrix();

   start = clock();
   matrixMultiply2();
   puts("\nMultiplication by row done");
   end = clock();
   //to calculte time taken in case 2
   total2= ((double) (end - start)) / CLOCKS_PER_SEC;
   flag =1;
   fileWrite(flag);
} 
else
{
   flag=2;
   fileWrite(flag);
}

return 0;
}