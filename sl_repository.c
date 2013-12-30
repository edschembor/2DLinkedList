/*
 * Project 3
 * Edward Schembor
 *
 * This program creates a "Super List" which is a sorted linked list in which
 * a pre-determined amount of elements have a corresponding element on "levels"
 * above them. This allows for higher efficiency when traversing the list
 * to insert, delete, or get elements.
 */

#include <stdio.h>
#include <malloc.h>

#define MAX_LEVELS 40

/*Creates the element data type for the linked list. Each element had a key
 * and pointers to the elements to the left and below it and a pointer to
 * its data. The pointer to its data allows for the data to only be allocated
 * once, therefore saving memory.*/
typedef struct list_element
{
	int key;
	int *data;
	struct list_element *next;
	struct list_element *down;
} element;

/*Creation of global variables --- Designated by upper case*/
int Steps;
int Percent;
int Levels; /*The number of levels in the super list*/
unsigned int seed = 0;
element sentinals[MAX_LEVELS];

void Repository_init(int p)
{
/* Initializes the globals for the repository. Also creates the array of dummy
 * elements*/

	int j;

	Steps = 0;
	Levels = 0;
	Percent = p;

	for( j = 0; j < MAX_LEVELS; j++)
	{
		if( j == 0)
		{
			sentinals[j].down = NULL;
		}else{
			sentinals[j].down = &sentinals[j-1];
		}
		sentinals[j].key = 0;
		sentinals[j].next = NULL;
		sentinals[j].data = NULL; 
	}

	srand100( seed );
}

int Repository_insert( int key, int data)
{
/*Inserts a key,data combo into the repository, sorted by the key. Returns 1 if
 * the combo was inserted, 0 if it was not, and -1 if there was an error. Keys
 * are unique.*/

	element *pointer;
	int counter; /*How many levels have been inserted on*/
	int insert; /*Int used as bool-determines if a level needs to be added*/
	int i; /*Keeps tracj going up*/
	int k; /*For loop*/ 
	int z; /*Keeps track going down*/
	int *dataPointer; /*Used to create a new data element*/
	int rand; /*Holds the rand created by rand() method*/
	static element *tempPointers[MAX_LEVELS]; /*Tracks pointers movement*/
	element *tempPointer; /*Used when inserting a new element*/
	counter = 0;
	pointer = &sentinals[Levels];
	insert = 1; /*Using as a bool -> 0 = False, 1 = True*/
	i = 0; /*Keeps track when going up*/
	z = 0; /*Keeps track when going down*/

	/*Initializes the array of temporary pointers to point to the sentinal nodes*/
	for( k = 0; k < MAX_LEVELS; k++)
	{
		tempPointers[k] = &sentinals[k];
	}

	/*Brings the pointer to were the key,data combo should be inserted on the bottom level and sets up the temporary pointers. Returns 0 if it finds the key already in the list*/
	while((pointer->down != NULL) || ((pointer->next != NULL) && (pointer->next->key <= key)) )
	{
		if(( (pointer->next == NULL) || (pointer->next->key > key)) && (pointer->down != NULL) )
		{
			tempPointers[Levels-z] = pointer;
			pointer = pointer->down;
			Steps++;
			z++;
		}else if (pointer->next != NULL)
		{
			pointer = pointer->next;
			Steps++;
			if( pointer->key == key) return 0;
		}
	}

	tempPointers[0] = pointer;

/*Inserts the combo on the bottom level. Inserts it on higher levels if the random number generated is less than the percent entered*/
	while( insert == 1 )
	{
		tempPointer = malloc( sizeof(element) );
		if( tempPointer == NULL) return -1;
		if( i != 0)
		{
			tempPointer->down = tempPointers[i-1]->next;
			tempPointer->data = tempPointer->down->data;
		}else{
			dataPointer = malloc(sizeof(int));
			if( dataPointer == NULL) return -1;
			*dataPointer = data;
			tempPointer->data = dataPointer;
			tempPointer->down = NULL;
		}
		tempPointer->key = key;
		tempPointer->next = pointer->next;
		pointer->next = tempPointer;
		
		/*Checks if necessary to add top level and change/keep bool*/
		rand = rand100();
		if( rand < Percent)
		{
			counter++;
			i++;
			pointer = tempPointers[i];
		}else insert = 0; 
	}
	if( counter >= Levels ) Levels = counter;
	return 1;
}

int Repository_delete( int key )
{
/*Deletes the record in the list with the input key. Returns 1 if succesfull and 0 if the operation failed.*/

	element *pointer;
	element *tempPointer;
	int *dataPointer;
	int currentLevel;
	currentLevel = Levels;
	pointer = &sentinals[Levels];

/*Moves pointer down the list and deletes each instance of key in the list*/
	while( (pointer->down != NULL) || ((pointer->next != NULL) && (pointer->next->key <= key)) )
	{
		if((pointer->next != NULL) && (pointer->next->key > key) && (pointer->down != NULL))
		{
			pointer = pointer->down;
			currentLevel--;
			Steps++;
		}else if((pointer->next != NULL) && (pointer->next->key == key)) {
			tempPointer = pointer->next;
			pointer->next = tempPointer->next;
			if( (pointer->next == NULL) && (sentinals[currentLevel].next == tempPointer) ) Levels--;
			pointer->next = tempPointer->next;
			if( pointer->down == NULL)
			{
				dataPointer = tempPointer->data;
				free( dataPointer);
				free( tempPointer);
				return 1;
			}
			free( tempPointer);
		}else if(pointer->next != NULL) {
			pointer = pointer->next;
			Steps++;
		}
		if( pointer->next == NULL && pointer->down != NULL)
		{
			pointer = pointer->down;
			currentLevel--;
			Steps++;
		}
	}
	
	return 0;	
}

int Repository_get( int key, int *data )
{
/*Check to see if a record with the input key exists in the list. Returns 1 if it does and 0 if it doesn't. If the record exists, the data of the record is put u into the data parameter.*/

	element *pointer;
	pointer = &sentinals[Levels];

	while( pointer->next != NULL )
	{
		if( (pointer->next->key > key) && (pointer->down != NULL) )
		{
			pointer = pointer->down;
			Steps++;

		}else if( pointer->next->key > key) 
		{
			return 0;
		}else if( pointer->next->key == key )
		{
			*data = *(pointer->next->data);
			return 1;
		}else{
			pointer = pointer->next;
			Steps++;
		}
		while( pointer->next == NULL && pointer->down != NULL)
		{
			pointer = pointer->down;
			Steps++;
		}
	}
	return 0;
}

void Repository_print( int print_elements )
{
/*Prints the number of unique recors, the number of levels, the number of records in each level, and the number of steps taken. When the print_elements parameter is 1, the list is printed. If print_elements is 2, the list is printed in a nicer fashion. */

	element *pelement;
	element *pelementBot;
	pelementBot = &sentinals[0];
	int p;
	int elementsInLevel;
	p = Levels;

	printf("\nNumber of levels in repository: %d", Levels + 1);
	printf("\nNumber of steps taken: %d\n", Steps);

	if( print_elements == 2) printf("The full repository: \n");
	
	if( print_elements != 2)
	{
		while( p >= 0)
		{
			elementsInLevel = 0;
			pelement = &sentinals[p];
			if( (p==0) && (print_elements == 1) ) printf("Full List:\n");
			while(pelement->next != NULL)
			{
				elementsInLevel++;
				if( (p == 0) && (print_elements == 1) ) 
				{
					printf("{%d, %d} ", pelement->next->key, *pelement->next->data);
				}
				pelement = pelement->next;
			}
			if( elementsInLevel != 0)
			{
				printf("\nElements in level %d: %d\n", p + 1, elementsInLevel);
			}
			if( p == 0) printf("Total Elements: %d", elementsInLevel);
			p--;
		}
	}

	if(print_elements == 2)
	{
		while( p >= 0)
		{
			elementsInLevel = 0;
			pelement = &sentinals[p];
			pelementBot = &sentinals[0];
			while(pelementBot->next != NULL && pelement->next != NULL )
			{
				elementsInLevel++;
				if(pelementBot->next->key == pelement->next->key)
				{
					
					printf("{%d, %d} ", pelement->next->key, *pelement->next->data);
					pelement = pelement->next;
					pelementBot = pelementBot->next;
				}else{
					printf("         ");
					pelementBot = pelementBot->next;
				}
			}
			printf("\n");
			if( p == 0) printf("Total Elements: %d", elementsInLevel);
			p--;
		}
	}	

	printf("\n-------------\n");
}
