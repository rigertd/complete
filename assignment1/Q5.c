/* CS261- Assignment 1 - Q.5*/
/* Name: David Rigert
 * Date: April 1, 2015
 * Solution description: Prompts the user for a word of no more than 255 chars,
 *                       converts it to sticky caps, and displays it.
 */
 
#include <stdio.h>
#include <stdlib.h>

/*converts ch to upper case, assuming it is in lower case currently*/
char toUpperCase(char ch){
     return ch-'a'+'A';
}

/*converts ch to lower case, assuming it is in upper case currently*/
char toLowerCase(char ch){
     return ch-'A'+'a';
}

void sticky(char* word){
    /*Convert to sticky caps*/
    char current = *word;  /* get first char of word */
    int index = 0;         /* keep track of letter index */
    
    /* loop through string until NULL */
    while (current) {
        if (index % 2 == 0) {
            /* even index */
            if (current >= 'a' && current <= 'z') {
                /* current index is lowercase */
                word[index] = toUpperCase(current);
            }
            /* current index is uppercase */
        } else {
            /* odd index */
            if (current >= 'A' && current <= 'Z') {
                /* current index is uppercase */
                word[index] = toLowerCase(current);
            }
            /* current index is lowercase */
        }
        
        /* set current to next letter */
        current = word[++index];
    }
}

int main(){
    /* allocate memory for input buffer */
    char *word = malloc(256 * sizeof(char));
    
    /*Read word from the keyboard using scanf*/
    printf("Enter a word of no more than 255 characters: ");
    scanf("%255s", word);
    
    /*Call sticky*/
    sticky(word);
    
    /*Print the new word*/
    printf("Sticky caps: %s", word);
    
    return 0;
}
