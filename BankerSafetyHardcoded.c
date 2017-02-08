/*////////////////////////
 * Dev: Ben Albritton
 * CSC 345 BANKER2.c
 * 
 *
 * 
 * 
 **/
#include<stdio.h>
#define	n	5
#define m	3

//making bool a thing
typedef enum {
  false = 0,
  true = 1
} bool;

//deugger
bool crap = false;

//SINGLE UNIT ARRAYS
int RESOURCE_INSTANCES[m] = {10, 5, 7};
int AVAILABLE[m]; //3,3,2
int ALLOCATED[m];
bool FINISH[n] = {false, false, false, false, false};
int WORK[m];
int REQUEST[m];

//DOUBLE ARRAYS
//Maximum Requirements for each process
int MAX[n][m] = { {7,5,3}, {3,2,2}, {9,0,2}, {2,2,2}, {4,3,3}};
//Initial allocations
int INITIAL_ALLOCATION[n][m] = { {0, 1, 0}, {2, 0, 0}, {3, 0, 2}, {2, 1, 1},{ 0, 0, 2} };
int ALLOCATION[n][m];
//NEED = MAX - ALLOCATION
int NEED[n][m];

//protoypes
void generateNeed();
void compute_banker_safety();
void calcAvailable();
void check_each_process();
bool checkrow(int);
void update(int);
void print_request(int);

int main()
{
	compute_banker_safety();
	return 0;
}

void compute_banker_safety()
{	
	//fills need array
	generateNeed();
	//calculate available, same as work...
	calcAvailable();
	//interate through to free initial alocations, we do this n times, to catch the N's we need
	for(int row = 0; row < n; row++)
	{
		check_each_process();
	}
	for(int row = 0; row < n; row++)
	{
		if(!(FINISH[row]))
			printf("There is a deadlock");
	}
}
/*
 * METHOD: check_each_process()
 * checks process once to see if we can execute process
 */
void check_each_process()
{	
	if(crap) printf("Checking Each proccess: \n ");
	for(int row = 0; row < n; row++)
	{
		if(!(FINISH[row]))print_request(row);
		if(checkrow(row) && !(FINISH[row]))
		{
			update(row);
			FINISH[row] = true;
		}
	
	}
}

/*
 * METHOD: print_request(row)
 * prints the request made by the processes
 */
void print_request(int ROW)
{
	int row = ROW;
	printf("Generated by the request placed by process p%i\nRequest =<",row);
	for(int col = 0; col < m; col++)
		printf("%d ", NEED[row][col]);
	printf(">\n\n");
}
/*
 * METHOD: update(row)
 * updates work array
 * prints that request was granted to process #
 */
void update(int x)
{
	int i = x;
	for(int j = 0; j < m; j++)
	{
		WORK[j] = WORK[j] + INITIAL_ALLOCATION[i][j];
	}
	printf("Request granted by system to p%d\n\n",i);
	FINISH[i] = true;
}

/*
 * METHOD: checkrow(row)
 * Returns true if the row is less than or equal to our work row
 */
bool checkrow(int ROW)
{
	int row = ROW;
	for(int col = 0; col < m; col++)
	{
		//printf("CHECKING | NEED : %d <= WORK : %d \n", NEED[i][j],AVAILABLE[0]);
		if((NEED[row][col] <= WORK[col]))
		{
			//Do nothing
		}
		else
		{
			//We found an error
			return false;
		}
	}
	//everything passes!
	return true;
	
}


///////////////////ONE TIME USE OF CODE///////////////////////////
void calcAvailable()
{
	int sum[m];
	if(crap) printf("WORK: ");
	for(int i =0; i < m; i++)
	{
		sum[i] = 0;
		for(int j =0; j < n; j++)
		{
			sum[i] = sum[i] + INITIAL_ALLOCATION[j][i];
		}
		AVAILABLE[i] = RESOURCE_INSTANCES[i]-sum[i];
		
		WORK[i] = AVAILABLE[i];
		if(crap) printf("%d", WORK[i]);
	}
}


void generateNeed()
{
	for(int i =0; i < n; i++)
	{
		if(crap)printf("ROW %d: ", i);
		for(int j =0; j < m; j++)
		{
			NEED[i][j] = (MAX[i][j] - INITIAL_ALLOCATION[i][j]);
			if(crap) printf("%d", NEED[i][j]);
		}
		if(crap) printf("\n");
	}
}
