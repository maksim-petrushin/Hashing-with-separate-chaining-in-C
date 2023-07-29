#include <stdio.h>
#include <stdlib.h>
#define SIZE 89
// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
struct HashType
{
	int		id;
	char	name;
	int		order; 
	struct HashType* next;
};

// Compute the hash function
int hash(int x)
{
 return x % SIZE; // modulo key creation..
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implements parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;
	int count=0; // added count just in case
	struct HashType *pTemp;
	for (i=0;i<hashSz;++i)
	{
		// if index is occupied with any records, print all
		
		pTemp=pHashArray+i;
		
		//For each FILLED node "next" will be not NULL, becasue malloc the "next" after filling each node..
		if(pTemp->next != NULL){ //that's why this statement makes sure only filled nodes are displayed.
		
		printf("\t%d\t", i);//index

		while(pTemp!=NULL){ //traversal until we reach the end of linked list of each filled index

		if(pTemp->id !=-1){//the last one will always be filled with -1 id data, so it should be displayed to user.
		printf("%d %c %d  --->  ", pTemp->id, pTemp->name, pTemp->order);//arrow for linked list visualization
		count++;//debugging count to verify that number of items is conserved.
		}
		pTemp=pTemp->next;
		
		}
		printf("NULL\n");//last node in each link list is pointing to null, so it is visualized with this print
		}
	}
	printf("%d\n", count);
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0, i;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation
	
	//initial assignment of Hash Structure
	struct HashType *pHash= malloc(sizeof(struct HashType)*SIZE);
	for(i=0;i<SIZE;i++){
		(pHash+i)->id=-1;
		(pHash+i)->next=NULL;
	}
	

	//Traversal of each record and deciding where to allocated it.
	struct HashType *pTemp;
	for(i=0; i<recordSz; i++){
		pTemp = pHash + hash((pRecords+i)->id);
		//if the index it taken, then the id will be something other than -1
		while(pTemp->id != -1){
			//in that case, we need the next node..
			pTemp=pTemp->next;
		}
		//transfer data
		pTemp->id = (pRecords[i]).id;
		pTemp->name = (pRecords[i]).name;
		pTemp->order = (pRecords[i]).order;

		//Create a next emtpy node for this filled one. This immitates the initial assignment of Hash Structure
		pTemp->next=(struct HashType*)malloc(sizeof(struct HashType));
		pTemp->next->id = -1;
		pTemp->next->next = NULL;
		
	}
	printf("\n\n");
	displayRecordsInHash(pHash, SIZE);

	//freeing memory
	struct HashType *freeTemp;
	for(i=0;i<SIZE;i++){
		pTemp = pHash + i;
		pTemp=pTemp->next;//skip the default one, because its the actual first pointer in array
		//and it will be freed as an array.

		//this block frees all the extra nexts created
		while(pTemp!=NULL && pTemp->id != -1){
			freeTemp = pTemp;
			pTemp=pTemp->next;
			free(freeTemp);
		}
	}
	free(pHash);
	free(pRecords);
}