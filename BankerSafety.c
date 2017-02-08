/*////////////////////////
 * Dev: Ben Albritton
 * CSC 345 BankerSafety.c
 * 
 * Instructions for use:
 * 		*for input from cli - disable STDINre
 * 		*for input from input.txt - enable STDINre
 * 
 * Output:
 * 		*1st, it will check if there is a safe sequence
 * 		*2nd, it will show the correct squence along with the processess' request
 * 
 * How it became messy:
 *		*I hard coded the arrays.
 *		*I then made it to where arrays could have info input from stdin
 **/

#include<stdio.h>

//making bool a thing for c
typedef enum {
  false = 0,
  true = 1
} bool;

//turn on for debuging
bool debug = false;

//turn off to enable STDIN from cli
bool STDINre = true;

//flag for output
bool safe;

//Global Variables
int processes = 0;
int resources = 0;

//function prototypes, too messy to try to make it work without formal declarations :(
void calcAvailable(int p, int r, int WORK[], int RESOURCE_INSTANCES[], int INITIAL_ALLOCATION[p][r]);
void generateNeed(int p, int r, int INITIAL_ALLOCATION[p][r], int NEED[p][r], int MAXIMUM_REQUIREMENTS[p][r]);
bool checkrow(int p, int r, int ROW, int NEED[p][r], int WORK[]);
void check_each_process(int p, int r, bool SAFETY[], int NEED[p][r], int WORK[], int INITIAL_ALLOCATION[p][r]);
void update(int p, int r, int ROW, int WORK[], bool SAFETY[], int INITIAL_ALLOCATION[p][r]);
void print_request(int p, int r, int ROW, int NEED[p][r]);

int main()
{	
	//This is handling standard input, the file below is for testing!
	if(STDINre) freopen("input.txt","r",stdin);//redirects standard input
	//GETTING PROCESSES
	if(!STDINre)printf("Enter number of processes: ");
	scanf("%d", &processes);
	//GETTING RESOURCES
	if(!STDINre)printf("Enter number of resources: ");
	scanf("%d", &resources);
	
	//initailize arrays. No use for pointers, its always a fixed size after user input
	int INITIAL_ALLOCATION[processes][resources];
	int MAXIMUM_REQUIREMENTS[processes][resources];
	int RESOURCE_INSTANCES[resources];
	int NEED[processes][resources]; //NEED = MAX - ALLOCATION
	int WORK[processes];
	//bool arrays
	bool SAFETY[processes]; //for safety check
	bool FINISH[processes]; //for the actual process
	
	//Handle input of Resource instances
	for(int col = 0; col < resources; col++)
	{
		if(!STDINre)printf("Enter Resource Instances: Col: %d= " , col);
		scanf("%d", &(RESOURCE_INSTANCES[col]));
	}
	//Handle INitial ALlocation
	for(int row = 0; row < processes; row++)
	{
		SAFETY[row] = false; //set to false as defualt
		FINISH[row] = false; // "     "
		for(int col = 0; col < resources; col++)
		{	
			{
				if(!STDINre)printf("Enter Initial Allocation: Proccess: %d Resources: %d= " , row, col);
				scanf("%d", &(INITIAL_ALLOCATION[row][col]));
			}
		}
	}
	//HAndle max req
	for(int row = 0; row < processes; row++)
	{
		for(int col = 0; col < resources; col++)
		{
			{
				if(!STDINre)printf("Enter Maximum Requirements: Proccess: %d Resources: %d= " , row, col);
				scanf("%d", &(MAXIMUM_REQUIREMENTS[row][col]));
			}
		}
	}
	//Get NEED
	generateNeed(processes, resources, INITIAL_ALLOCATION, NEED, MAXIMUM_REQUIREMENTS);
	//Calculate WORK
	calcAvailable(processes, resources, WORK, RESOURCE_INSTANCES, INITIAL_ALLOCATION);
	//check for safety
	for(int row = 0; row < processes; row++)
	{
		check_each_process(processes, resources, SAFETY, NEED, WORK, INITIAL_ALLOCATION);
	}
	safe = true;
	for(int row = 0; row < processes; row++)
	{
		if(!(SAFETY[row]))
		{
			safe = false;
			printf("There is a deadlock");
		}
	}
	
	//generate safe sequence
	
	if(safe) 
	{
		printf("Everything is safe, Generating Safe sequence\n");
		generateNeed(processes, resources, INITIAL_ALLOCATION, NEED, MAXIMUM_REQUIREMENTS);
		
		//recalculate WORK
		calcAvailable(processes, resources, WORK, RESOURCE_INSTANCES, INITIAL_ALLOCATION);
		for(int row = 0; row < processes; row++)
		{
			//We pass it the FINISH array this time
			check_each_process(processes, resources, FINISH, NEED, WORK, INITIAL_ALLOCATION);
		}
	}
	return 0;
}
//METHOD check_each_process
//I passed it p and r by reference so the double arrays could be
//passed by address. All arrays passed by address
//
void check_each_process(int p, int r, bool SAFETY[], int NEED[p][r], int WORK[], int INITIAL_ALLOCATION[p][r])
{	
	int processes = p;
	int resources = r;
	
	if(debug) printf("Checking Each proccess: \n ");
	for(int row = 0; row < processes; row++)
	{
		if(!(SAFETY[row])&&(safe))print_request(processes, resources, row, NEED);
		if(checkrow(processes, resources, row, NEED, WORK) && !(SAFETY[row]))
		{
			update(processes, resources, row, WORK, SAFETY, INITIAL_ALLOCATION);
			SAFETY[row] = true;
		}
	
	}
}
//METHOD: 
void update(int p, int r, int ROW, int WORK[], bool SAFETY[], int INITIAL_ALLOCATION[p][r])
{
	int processes = p;
	int resources = r;
	int i = ROW;
	for(int j = 0; j < resources; j++)
	{
		WORK[j] = WORK[j] + INITIAL_ALLOCATION[i][j];
	}
	if(safe) printf("Request granted by system to p%d\n\n",i);
	SAFETY[i] = true;
}

bool checkrow(int p, int r, int ROW, int NEED[p][r], int WORK[])
{
	int processes = p;
	int resources = r;
	int row = ROW;
	for(int col = 0; col < resources; col++)
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

void print_request(int p, int r, int ROW, int NEED[p][r])
{
	int processes = p;
	int resources = r;
	int row = ROW;
	if(safe) printf("Generated by the request placed by process p%i\nRequest =<",row);
	for(int col = 0; col < resources; col++)
		printf("%d ", NEED[row][col]);
	printf(">\n\n");
}

////////////////////////RUN ONCE//////////////////////////////////////
void calcAvailable(int p, int r, int WORK[], int RESOURCE_INSTANCES[], int INITIAL_ALLOCATION[p][r])
{	
	int processes = p;
	int resources = r;
	int sum[resources];
	if(debug) printf("WORK: ");
	for(int i =0; i < resources; i++)
	{
		sum[i] = 0;
		for(int j =0; j < processes; j++)
		{
			sum[i] = sum[i] + INITIAL_ALLOCATION[j][i];
		}
		WORK[i] = RESOURCE_INSTANCES[i]-sum[i];
		
		if(debug) printf("%d", WORK[i]);
	}
}
void generateNeed(int p, int r, int INITIAL_ALLOCATION[p][r], int NEED[p][r], int MAXIMUM_REQUIREMENTS[p][r])
{
	int processes = p;
	int resources = r;
	if(debug) printf("\n");
	for(int i =0; i < processes; i++)
	{
		
		if(debug)printf("ROW %d: ", i);
		for(int j =0; j < resources; j++)
		{
			NEED[i][j] = (MAXIMUM_REQUIREMENTS[i][j] - INITIAL_ALLOCATION[i][j]);
			if(debug) printf("%d", NEED[i][j]);
		}
		if(debug) printf("\n");
	}
}
