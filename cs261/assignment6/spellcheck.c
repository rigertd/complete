#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include "hashMap.h"

/*
 the getWord function takes a FILE pointer and returns you a string which was
 the next word in the in the file. words are defined (by this function) to be
 characters or numbers seperated by periods, spaces, or newlines.
 
 when there are no more words in the input file this function will return NULL.
 
 this function will malloc some memory for the char* it returns. it is your job
 to free this memory when you no longer need it.
 */
char* getWord(FILE *file);

/*
 Load the contents of file into hashmap ht
 */
void loadDictionary(FILE* file, struct hashMap* ht);
void wordToLower(char *);

int main (int argc, const char * argv[]) {
  clock_t timer;
  struct hashMap* hashTable;
  int tableSize = 1000;
  timer = clock();

  hashTable = createMap(tableSize);
  
  FILE* dictionary = fopen("dictionary.txt", "r");
  assert(dictionary != 0);
  
  loadDictionary(dictionary,hashTable);
  timer = clock() - timer;
	printf("Dictionary loaded in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
  fclose(dictionary);
  
  char* word = (char*)malloc(256*sizeof(char));
  int quit=0;
  while(!quit){
    printf("Enter a word: ");
    scanf("%s",word);
    /*
      ... spell checker code goes here ...
      ... You write this               ...
    */
    /* convert to lowercase */
    wordToLower(word);
    
    if (!containsKey(hashTable, word)) {
      printf("You misspelled '%s'.\n", word);
    } else if (strcmp(word, "quit") != 0) {
      printf("You spelled '%s' correctly.\n", word);
    }
    /* Don't remove this. It is used for grading*/
    if(strcmp(word,"quit")==0)
      quit=!quit;
  }
  free(word);
  deleteMap(hashTable);
     
  return 0;
}

/* converts an entire word to lowercase */
void wordToLower(char *str) {
  int i, len = strlen(str);
  
  for (i = 0; i < len; ++i) {
    str[i] = tolower(str[i]);
  }
}

void loadDictionary(FILE* file, struct hashMap* ht)
{
  char *word = getWord(file);
  while (word != 0) {
    wordToLower(word);
    insertMap(ht, word, 0);
    word = getWord(file);
  } 
}

char* getWord(FILE *file)
{
	int length = 0;
	int maxLength = 16;
	char character;
    
	char* word = (char*)malloc(sizeof(char) * maxLength);
	assert(word != NULL);
    
	while( (character = fgetc(file)) != EOF)
	{
		if((length+1) > maxLength)
		{
			maxLength *= 2;
			word = (char*)realloc(word, maxLength);
		}
		if((character >= '0' && character <= '9') || /*is a number*/
		   (character >= 'A' && character <= 'Z') || /*or an uppercase letter*/
		   (character >= 'a' && character <= 'z') || /*or a lowercase letter*/
		   (character == 39)) /*or is an apostrophy*/
		{
			word[length] = character;
			length++;
		}
		else if(length > 0)
			break;
	}
    
	if(length == 0)
	{
		free(word);
		return NULL;
	}
	word[length] = '\0';
	return word;
}

void printValue(ValueType val) {
    printf("%d", val);
}
