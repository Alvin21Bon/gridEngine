#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
	// POSSIBLE USER INPUTS. 
	// dumbass ctrl-d which eliminates all stdin i think, i will need to break here, test against EOF state?
	// a new line right at the start
	// some random amount of characters and then a newline. 
	//
	// valid states: only 1 non-whitespace character inputed
	// if in valid state, now we can work. test if it is y or n, and if not repeat again.
	// 
	// if user did not input valid state, we will need to flush the input buffer by continuing to get chars until we get a newline or an EOF
	//
	// if first char got is not alpha, then this is not a valid state.
	// if first char got is alpha, but next character is not whitespace of EOF, then this is not valid state
	
	char retrievedChar = 0;
	char* linePtr = NULL;
	size_t bufferSize = 0;
	do
	{
		printf("(y/n): ");
		int returnValue = getline(&linePtr, &bufferSize, stdin);
		printf("\n");
		if (returnValue == EOF) goto CLEAN; 
		if (returnValue > 2) continue;

		retrievedChar = linePtr[0];
	}
	while (retrievedChar != 'y' && retrievedChar != 'n');

	CLEAN:
	free(linePtr);
	return 0;
}
