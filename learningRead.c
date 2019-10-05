#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<math.h>
#include<time.h>

// Dr Potikas slides and https://www.geeksforgeeks.org/insertion-sort/ used for reference have written the code on my own but used those resources were used as a reference.
void insertion_sort(long long array[], long n)
{
    long  i, key, j;
    for (long i = 1; i < n; i++)
    {
        key = array[i];
        j = i-1;
        while (j >= 0 && array[j] > key)
        {
            array[j+1] = array[j];
            j = j-1;
        }
        array[j+1] = key;
    }
}



int main(int argc, char *argv[])
{

    
    char *eptr;
    long long number = strtoll(argv[1],&eptr,10);  //strtoll 
    int childPipes[5];           //Using 5 pipes to talk to the Parent function  
    pid_t pid;      //Process
    
    
  for(int i=0;i<5;i++) // Using 5 copies of the process. 
    { 
        
        int pipeArray[2];          // Argument for read and Write.
        int  rc = pipe(pipeArray); //Initializing the Pipe 
        pid = fork();
        
        
        if(rc < 0)  // Were not able to create the pipe.
        {
            printf(" Pipe Failed");  //The pipe Failed.
            exit(0);                //The program exits.
        }
        
        
        if(pid<0)
        {
            printf("The process creation failed"); // Process was not created
            exit(0);        //The program exits.
        }
        if(pid == 0) 
        { 
            
        
            //printf(" %f", sqrt(number));
                long long start  = (long long) ((i)*sqrt(number))/5; //The work splitting step for the child Processes.
            //printf("%lld",start);
                long long end = (long long) ((i+1)*sqrt(number))/5;
                //The work Splitting step for child Processes
                int counter = 0;
           // printf("%lld",end);
                if(i==4)
                {
                    end = end + 1;  // THe last increment for.
                }
                    
            for(long long factor = start;factor <end; factor++)
            {
                if(factor == 0)
                    continue;
                if(number%factor == 0 )
                {
                   write(pipeArray[1], &factor, 8);
                    if(factor != sqrt(number))
                    {
                        long long complement =  (long long) number/factor;
                        //printf(" %lld ",complement);
                        write(pipeArray[1], &complement, 8);
                    }
                }
            }
            
            close(pipeArray[1]);
            exit(0); 
        } 
           
        else if(pid > 0)
        {
            
            close(pipeArray[1]); //Closing the other side of the pipe.
            childPipes[i] = pipeArray[0]; // 
                
        }
        
        
    }
 
    
    long long list[100];
    int ctr = 0;
    for(int j = 0;j<=4;j++)
    {
        
        long long buffer;
        
        while(read(childPipes[j],&buffer,8)>0)
            {
                list[ctr++] = buffer;
               // printf("%lld \n", buffer);
            }
        
    }
    
    insertion_sort(list, ctr);
    
    for(int i = 0; i < ctr; ++i)
    {
        printf("%lld  ", list[i]);
    }
    return 0;
}


