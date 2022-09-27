#include <stdio.h>
#include <stdlib.h>


//Global variables
//boolean type define
typedef enum { false, true } bool;
//Basic Configuration
int noOfSets = 0;
int associative = 0;
int sizeOfLine = 0;

//Cache Read Write Result Variables
char access;
unsigned addressV;
int tag=0;
int indexV=0;
int offset=0;
bool result=false;
bool memRef=false;

//Cache Memory variables
int tagArr[100][100];
int validArr[100][100];
//For implementing Write Allocate and Write Back policies
int writeAllocate[100][100];

//General variables
int i=0;

//Read Cache
void readCache(unsigned sAddress){
	
	//hexadecimal value in string is converted to interger value
	int address = sAddress;
	//Ofset index is obtained
	int offsetIndex = address % sizeOfLine;
	int partAdd = address / sizeOfLine;
	//tag index is obtained
	int tagIndex = partAdd / (noOfSets / associative);
	//index value is obtained
	int indexValue = partAdd % (noOfSets / associative);

	//Console.WriteLine(indexValue+" tag->"+tagIndex+" add->"+address);
	bool hit = false;

	//Check if there is a hit
	for (i = 0; i <= associative - 1; i++)
	{
		//checking if valid and tag index already in cache
		if (validArr[i][indexValue] != 0 && tagArr[i][indexValue] == tagIndex)
		{
			hit = true;
			validArr[i][indexValue]++;//increment to set it used recently
			tagArr[i][indexValue] = tagIndex;

			//Return Cache Status
			access = 'R';
			addressV = sAddress;
			tag = tagIndex;
			indexV= indexValue;
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
		for (i = 0; i <= associative - 1; i++)
		{
			if (validArr[i][indexValue] == 0)
			{
				found = true;
				validArr[i][indexValue]++;//increment to set it used recently
				tagArr[i][indexValue] = tagIndex;

				//Return Cache Status
				access = 'R';
				addressV = sAddress;
				tag = tagIndex;
				indexV= indexValue;
				offset = offsetIndex;
				result = false;
				memRef = true;

				break;
			}
		}

		//if no freespace available, over write the least used cache block
		if (!found)
		{
			//Find the least used set
			int lowest = validArr[0][indexValue];
			int ind = 0;
			for (i = 1; i <= associative - 1; i++)
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
			addressV = sAddress;
			tag = tagIndex;
			indexV= indexValue;
			offset = offsetIndex;
			result = false;
			memRef = true;
		}
	}
	
}

void writeCache(unsigned sAddress)
{	
	//Converting hexadecimal value in sting to Integer
	int address = sAddress;
	//offset index is obtained
	int offsetIndex = address % sizeOfLine;
	int partAdd = address / sizeOfLine;
	//Tag Index is obtained
	int tagIndex = partAdd / (noOfSets / associative);
	//Index value is obtained from address
	int indexValue = partAdd % (noOfSets / associative);
	
	//Initialise array to 0

	//Console.WriteLine(indexValue+" tag->"+tagIndex+" add->"+address);
	bool hit = false;

	//Check if there is a hit
	for (i = 0; i <= associative - 1; i++)
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
			addressV = sAddress;
			tag = tagIndex;
			indexV= indexValue;
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
		for (i = 0; i <= associative - 1; i++)
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
				addressV = sAddress;
				tag = tagIndex;
				indexV= indexValue;
				offset = offsetIndex;
				result = false;
				memRef = false;

				break;
			}
		}

		//If no free space available, over write the least used cache block
		if (!found)
		{
			//Find the least used set
			int lowest = validArr[0][indexValue];
			int ind = 0;
			for (i = 1; i <= associative - 1; i++)
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
			addressV = sAddress;
			tag = tagIndex;
			indexV= indexValue;
			offset = offsetIndex;
			result = false;
			memRef = writeAllocate[ind][indexValue] == 1 ? true : false;

			//Write Allocate
			writeAllocate[ind][indexValue] = 1;
		}
	}

}

void initialiseMemories(){
	//Initialises the variables
	int j=0;
	
	int limit = noOfSets / associative;
	for (i = 0; i < associative; i++)
	{
		for (j = 0; j < limit; j++)
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
	int hit = 0, miss = 0;
	float sum=0.0;
	FILE * fr;
	FILE * fw;

	//Opening trace.config file to read
	fr = fopen("trace.config", "r");	
	
	//Reading configuration from file
	fscanf(fr, "%s %s %s %d", str1, str1, str1, &noOfSets);
	fscanf(fr, "%s %s %d", str1, str1, &associative);
	fscanf(fr, "%s %s %d", str1, str1, &sizeOfLine);

	fw = fopen("trace.stats", "w");

	//Printing the Configuration details
	fprintf(fw,"Cache Configuration\n");
	fprintf(fw,"\t%d %d- way set associative entries\n",noOfSets,associative);
	fprintf(fw,"\tof line size %d bytes\n\n\n",sizeOfLine);
	
	//Closing the configuration file
	fclose(fr);
	
	//Initializes Caches to all set to '0'
	initialiseMemories();

	fprintf(fw,"Result for Each Reference\n\n");
	//fprintf(fw,"Ref\tAccess\tAddress\tTag\tIndex\tOffset\tResult\tMemrefs\n");
	fprintf(fw,"Ref   Access  Address   Tag  Index  Offset  Result  Memrefs\n");
	fprintf(fw,"-----------------------------------------------------------\n");

	fr = fopen("trace.dat", "r");
	while (fscanf(fr," %c:%d:%x",&rW,&refSize,&hexAdd)!=EOF)
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
		//fprintf(fw,"%d\t%c\t%x\t%d\t%d\t%d\t%d\t%d\n",count,access,addressV,tag,indexV,offset,result,memRef);
		fprintf(fw,"%*d   %*s  %*x   %*d  %*d  %*d  %*s  %*d\n",3,count,6,(access=='W'?"Write":"Read"),7,addressV,3,tag,5,indexV,6,offset,6,(result==1?"hit":"miss"),7,memRef);
		count++;
	}

	//Printing the summary
	fprintf(fw,"\nSimulation Summary Statistics");
	fprintf(fw,"\n-----------------------------");
	fprintf(fw,"\nTotal hits\t\t: %d",hit);
	fprintf(fw,"\nTotal misses\t: %d",miss);
	fprintf(fw,"\nTotal accesses\t: %d",(hit+miss));
	sum=hit+miss;
	fprintf(fw,"\nHit ratio\t\t: %f",(hit / sum));
	fprintf(fw,"\nMiss ratio\t\t: %f",(miss / sum));

	//Close the output file
	fclose(fw);
	fclose(fr);

	return(0);

}
