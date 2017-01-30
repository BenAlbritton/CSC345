#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define MAXSIZE 100
#define ITERATIONS 1000
int buffer[MAXSIZE]; // buffer
int nextp, nextc; // temporary storage
int count=0;

void printfunction() //void printfunction(void * ptr)
{
	//int count = *(int *) ptr;
	if (count == 0) 
	{
		printf("All items produced are consumed by the consumer \n");
	}
	else
	{
		for (int i=0; i<=count; i=i+1)
		{
			printf("%d, \t",buffer[i]);
		}

		printf("\n");
	}
}

void *producer(void *ptr)
{
	int item = 0;
	int flagp = 0;
	int count2 = 1;
	int in = *(int *) ptr;
	while (flagp<=ITERATIONS)
	{
		item = (rand()%7)%10;
		flagp = flagp + count2;
		nextp = item;
		buffer[in] = nextp;
		in=((in+1)%MAXSIZE);
		if(count <= MAXSIZE)
		{
			count = count+1;
			printf("Count = %d - incremented at producer \t flag = %d\n", count, flagp);
		}
	}
	pthread_exit(NULL);
}

void *consumer(void *ptr)
{
	//int item = ITERATIONS;
	int flag = ITERATIONS;
	int out = *(int *) ptr;
	do
	{
		while (count >0)
		{
			nextc = buffer[out];
			out=(out+1)%MAXSIZE;
			printf("\tCount = %d - decremented at consumer \t flag = %d\n", count, flag);
			count = count-1;
			flag=flag-1;
		}
		if (count <= 0)
		{
			printf("consumer made to wait...faster than producer.\n");
		}
	}
	while (flag>=0);
	pthread_exit(NULL);
}

int main(void) 
{
     int in=0, out=0; //pointers
     pthread_t pro, con;

// Spawn threads
	int rc1 = pthread_create(&pro, NULL, producer, &in);
	int rc2 = pthread_create(&con, NULL, consumer, &out);
	if(rc1!=0)
	{
		printf("ERROR; return code from pthread_create() is %d\n", rc1); 
		exit(-1);
	}
	if(rc2!=0) 
	{
		printf("ERROR; return code from pthread_create() is %d\n", rc2); 
		exit(-1);
	}
	// Wait for the threads to finish
	// Otherwise main might run to the end
	// and kill the entire process when it exits.
	pthread_join(pro, NULL);
	pthread_join(con, NULL);
	printfunction();//printfunction(&count);
}
