/* CS261- Assignment 0 - inch2cm
 * Name: 		David Rigert
 * Description:	Prompts the user for a value in inches and
 *				converts the value to centimeters and displays it.
 *				Optionally takes a command line argument.
 */
#include <stdio.h>
#include <stdlib.h>

// conversion factor for inches to centimeters
#define CM_PER_INCH 2.54

int main(int argc, char **argv) {
	float input;		// stores user input for conversion

	if (argc > 1) {
		// user entered command line argument
		input = strtof(argv[1], NULL);
	} else {
		// prompt user for input
		printf("Enter the length in inches: ");
		// read user input
		scanf("%f", &input);
	}

	// display conversion
	printf("%.2f inches is %.2f centimeters.\n", input, input * CM_PER_INCH);

	return EXIT_SUCCESS;
}
