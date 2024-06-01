/*Temp file to take cmd line arg*/

#include <stdio.h>
#include <stdlib.h> // for exit 0
int main(int argc, char const *argv[])
{
	//if the arguments are less than 2 give error and exit
	if (argc < 2)
	{
		perror("\n\nError Enter the Argument");
		exit(0);
	}//if ends
	/* if at least 2 arguments are given then go to 
	else condition and print the arguments*/
	else
	{
		printf("\nHello wold");
		for (int i = 1; i < argc ; ++i)
		{
			printf(" %s", argv[i]);
		}//for ends
		printf("\n\n");
	}//else ends
	return 0;
}