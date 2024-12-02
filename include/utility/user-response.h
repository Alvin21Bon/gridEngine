#pragma once

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// this is simply a wrapper for the getline function that also prints the prompt to the screen
// @param {responsePtr} same as getline, should be freed
static inline ssize_t GET_USER_RESPONSE_WITH_PROMPT(char** responsePtr, size_t* allocatedBufferSize, const char* const prompt)
{
	ssize_t numCharsRead;

	printf("%s", prompt);
	numCharsRead = getline(responsePtr, allocatedBufferSize, stdin);
	printf("\n");

	return numCharsRead;
}

// @returns {char} the char the user responded with 
// @returns {EOF} if next getline reaches EOF
static inline char PROMPT_CHAR_RESPONSE_FROM_USER(const char* const charResponseList, const char* const prompt)
{
	char* retrievedLineBufferPtr = nullptr;
	size_t sizeOfRetrievedLineBuffer = 0;
	ssize_t numCharsRead;
	char userResponseChar;

	while ((numCharsRead = GET_USER_RESPONSE_WITH_PROMPT(&retrievedLineBufferPtr, &sizeOfRetrievedLineBuffer, prompt)) != EOF)
	{
		if (numCharsRead > 2) continue; // only 1 character + \n
		
		userResponseChar = retrievedLineBufferPtr[0];
		if (strchr(charResponseList, userResponseChar)) break;
	}
	free(retrievedLineBufferPtr);

	return numCharsRead == EOF ? EOF : userResponseChar;
}
