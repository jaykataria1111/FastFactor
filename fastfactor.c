#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<math.h>

/**
*The following codes finds the factors of a nummbe as fast as possible.
*Can be improved by  using better Sorting algorithms, due to time constraint
*I used insertion sort. because it is very easy to write and works good on sorted output.
*
*@author Jay Katariya
*/


/**
 *The following code sorts a given array
 * @param long long array
 * @param n which is the length of the
 *Dr/ Professor Potikas slides and https://www.geeksforgeeks.org/insertion-sort/ are used for reference purposes.
 *I have written the code on my own !
*/
void insertion_sort(long long array[], long length)
{
    long  initial, key, lookup;
    for (initial = 1; initial < length; initial++)
    {
        key = array[initial];
        lookup = initial-1;
        while (lookup >= 0 && array[lookup] > key)
        {
            array[lookup+1] = array[lookup];
            lookup--;
        }
        array[lookup+1] = key;
    }
}



/*
 * The following method represents a find factor function.
 * This method splits the work among simoultaneous processes and uses pipes to commincate
 *  from the child processes to the parent process
 *  Professor Ben Reed helped explain the by writing sample codes in office hour.
 * Used stack overflow to  get a deeper understanding of what was going on.
 *  Have written all the code on my own!
 *  @param number whose factors we want to find.
*/
int findFactor(long long number)
{

    //printf("%lld : ",number);
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
            long long end = (long long) ((i+1)*sqrt(number))/5; // The following code does computation till the square, root of the number, and finds its complements too.
            //The work Splitting step for child Processes
            int counter = 0;
            // printf("%lld",end);
            if(i==4)
            {
                end = end + 1;  // THe last increment for checking the number itself, contact the tudent for a deeper explanation.
            }
            
            for(long long factor = start;factor <end; factor++)
            {
                if(factor == 0)  // was dividing by zero was up the whole night, lol.
                    continue;
                if(number%factor == 0 )
                {
                    write(pipeArray[1], &factor, 8);
                    if(factor != sqrt(number))
                    {
                        long long complement =  (long long) number/factor;
                        //printf(" %lld ",complement);
                        write(pipeArray[1], &complement, 8); // Writing from the children.
                    }
                }
            }
            
            close(pipeArray[1]);   // Closing the pipe.
            exit(0);
        }
        
        else if(pid > 0)
        {
            
            close(pipeArray[1]); //Closing the other side of the pipe.
            childPipes[i] = pipeArray[0]; //
            
        }
        
        
    }
    
    
    long long list[1000];
    int ctr = 0;
    for(int j = 0;j<=4;j++)
    {
        
        long long buffer;
        
        while(read(childPipes[j],&buffer,8)>0)  // Reading on the childs side.
        {
            list[ctr++] = buffer;           // adding the content to the list
            // printf("%lld \n", buffer);
        }
        
    }
    
    insertion_sort(list, ctr);
    printf("%lld: ",list[ctr-1]);               // Printing out the facors of the list.
    for(int i = 0; i < ctr; ++i)
    {
        printf("%lld  ", list[i]);
    }

    return 0;
}




/**
 *
 * The main method takes argument forom the comman line interface
 * Professor Ben Reed's canvas code for strtoll and getline was used.
 */
int main(int argc, char *argv[])
{
    
    
    char *ptr;
    size_t n = 0;
    size_t s;
    
    
    if(argc == 1) // If there is no argument, i.e. a file might have been passed
    {
        while ((s = getline(&ptr, &n,stdin)) != -1)
        {
            char *eptr;
            long long number = strtoll(ptr,&eptr,10);  //strtoll , to accept the integer
            if(number<=0)
            {
                printf("The file does not have a proper number" );
                return -1;
            }
            findFactor(number);
            printf("\n");
        }
    }
    
    else
    {
        for(int i=1;i<argc;i++)
        {
            char *eptr;
            long long num = strtoll(argv[i],&eptr,10);  //Reading and converting the arguments.
            if(num<=0)
            {
                printf("Please enter a valid number, %s is not a valid number \n",argv[i] );
                return -1;
            }
    
            findFactor(num);
            printf("\n");
        
        }
        return 0;
    }
}





