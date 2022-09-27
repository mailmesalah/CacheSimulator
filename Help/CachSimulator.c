#include <stdio.h>
#include <stdlib.h>


//Global variables

//Basic Configuration
int noOfSets = 0;
int associative = 0;
int sizeOfLine = 0;

//Cache Read Write Result Variables
char access;
unsigned address;
int tag=0;
int index=0;
int offset=0;
bool result=false;
bool memRef=false;

//Cache Memory variables
int tagArr[100][100];
int validArr[100][100];
//For implementing Write Allocate and Write Back policies
int writeAllocate[100][100];


//Read Cache
void readCache(unsigned sAddress){
	
	//hexadecimal value in string is converted to interger value
	int address = sAddress;
	//Ofset index is obtained
	int offsetIndex = address % lineSize;
	int partAdd = address / lineSize;
	//tag index is obtained
	int tagIndex = partAdd / (noOfSets / associative);
	//index value is obtained
	int indexValue = partAdd % (noOfSets / associative);

	//Console.WriteLine(indexValue+" tag->"+tagIndex+" add->"+address);
	bool hit = false;

	//Check if there is a hit
	for (int i = 0; i <= associative - 1; i++)
	{
		//checking if valid and tag index already in cache
		if (validArr[i][indexValue] != 0 && tagArr[i][indexValue] == tagIndex)
		{
			hit = true;
			validArr[i][indexValue]++;//increment to set it used recently
			tagArr[i][indexValue] = tagIndex;

			//Return Cache Status
			access = 'R';
			address = sAddress;
			tag = tagIndex;
			index = indexValue;
			offset = offsetIndex;
			result = true;
			memRef = false;

			break;
		}
	}

	//If not hit register for first time in proper place
	if (!hit)
	{
		bool found = false;
		//Check if free space available
		for (int i = 0; i <= associative - 1; i++)
		{
			if (validArr[i][indexValue] == 0)
			{
				found = true;
				validArr[i][indexValue]++;//increment to set it used recently
				tagArr[i][indexValue] = tagIndex;

				//Return Cache Status
				access = 'R';
				address = sAddress;
				tag = tagIndex;
				index = indexValue;
				offset = offsetIndex;
				result = false;
				memRefs = true;

				break;
			}
		}

		//if no freespace available, over write the least used cache block
		if (!found)
		{
			//Find the least used set
			int lowest = validArr[0][indexValue];
			int ind = 0;
			for (int i = 1; i <= associative - 1; i++)
			{
				if (lowest < validArr[i][indexValue])
				{
					lowest = validArr[i][indexValue];
					ind = i;
				}
			}

			//LRU replacement
			validArr[ind][indexValue]++;//increment to set it used recently
			tagArr[ind][indexValue] = tagIndex;

			//Return Cache Status
			access = 'R';
			address = sAddress;
			tag = tagIndex;
			index = indexValue;
			offset = offsetIndex;
			result = false;
			memRefs = true;
		}
	}
	
}

void writeCache(unsigned sAddress)
{	
	//Converting hexadecimal value in sting to Integer
	int address = sAddress;
	//offset index is obtained
	int offsetIndex = address % lineSize;
	int partAdd = address / lineSize;
	//Tag Index is obtained
	int tagIndex = partAdd / (noOfSets / associative);
	//Index value is obtained from address
	int indexValue = partAdd % (noOfSets / associative);
	
	//Initialise array to 0

	//Console.WriteLine(indexValue+" tag->"+tagIndex+" add->"+address);
	bool hit = false;

	//Check if there is a hit
	for (int i = 0; i <= associative - 1; i++)
	{
		//Check if valid and tag Index is already in Cache
		if (validArr[i][indexValue] != 0 && tagArr[i][indexValue] == tagIndex)
		{
			hit = true;
			validArr[i][indexValue]++;//increment to set it used recently
			tagArr[i][indexValue] = tagIndex;

			//Write Allocate
			writeAllocate[i][indexValue] = 1;

			//Return Cache Status
			access = 'W';
			address = sAddress;
			tag = tagIndex;
			index = indexValue;
			offset = offsetIndex;
			result = true;
			memRefs = false;

			break;
		}
	}

	//If not hit register for first time in proper place
	if (!hit)
	{
		bool found = false;
		//Check if free space available
		for (int i = 0; i <= associative - 1; i++)
		{
			if (validArr[i][indexValue] == 0)
			{
				found = true;
				validArr[i][indexValue]++;//increment to set it used recently
				tagArr[i][indexValue] = tagIndex;

				//Write Allocate
				writeAllocate[i][indexValue] = 1;

				//Return Cache Status
				access = 'W';
				address = sAddress;
				tag = tagIndex;
				index = indexValue;
				offset = offsetIndex;
				result = false;
				memRefs = false;

				break;
			}
		}

		//If no free space available, over write the least used cache block
		if (!found)
		{
			//Find the least used set
			int lowest = validArr[0][indexValue];
			int ind = 0;
			for (int i = 1; i <= associative - 1; i++)
			{
				if (lowest < validArr[i][indexValue])
				{
					lowest = validArr[i][indexValue];
					ind = i;
				}
			}

			//LRU replacement
			validArr[ind][indexValue]++;//increment to set it used recently
			tagArr[ind][indexValue] = tagIndex;

			//Return Cache Status
			access = 'W';
			address = sAddress;
			tag = tagIndex;
			index = indexValue;
			offset = offsetIndex;
			result = false;
			memRefs = writeAllocate[ind][indexValue] == 1 ? true : false;

			//Write Allocate
			writeAllocate[ind][indexValue] = 1;
		}
	}

}

void initialiseMemories(){
	//Initialises the variables
	
	int limit = noOfSets / associative;
	for (int i = 0; i < associative; i++)
	{
		for (int j = 0; j < limit; j++)
		{
			tagArr[i][j] = 0;
			validArr[i][j] = 0;
			writeAllocate[i][j] = 0;
		}
	}

}
int main()
{
	char str1[20];	
	unsigned hexAdd;
	char rW;
	int refSize = 0,count=1;
	float hit = 0f, miss = 0f;
	FILE * fr;
	FILE * fw;

	//Opening trace.config file to read
	fr = fopen("trace.config", "r");	
	
	//Reading configuration from file
	fscanf(fr, "%s %d", str1, &noOfSets);
	fscanf(fr, "%s %d", str1, &associative);
	fscanf(fr, "%s %d", str1, &sizeOfLine);

	//Printing the Configuration details
	printf("Cache Configuration");
	printf("\t%d %d- way set associative entries",&noOfSets,&associative);
	printf("\tof line size %d bytes",%sizeOfLine);
	
	//Closing the configuration file
	fclose(fr);
	
	//Initializes Caches to all set to '0'
	initialiseMemories();

	printf("Result for Each Reference");
	printf("Ref\tAccess\tAddress\tTag\tIndex\tOffset\tResult\tMemrefs");

	fr = fopen("trace.config", "r");

	while (fscanf(fr,"%c:%d:%x",&rW,&refSize,&hexAdd))
	{
		//Check if it is a Read or Write statement
		if (rW=='R')
		{
			//Read from Cache sending the hex value
			readCache(hexAdd);

		}
		else
		{
			//Write to Cache sending the hex value
			writeCache(hexAdd);
		}

		//Counting Miss and Hit
		if (result)
		{
			++hit;
		}
		else
		{
			++miss;
		}

		//Print out the result
		printf("%d\t%c\t%x\t%d\t%d\t%d\t%d\t%d",&count,&access,&address,&tag,%index,&offset,&result,&memRef);
		count++;
	}

	return(0);

}