//Bradley Mixon
//CS3377.505 Project 1
// 3/24/23

#include <stdio.h>
#include <stdlib.h>
#include <pwring.h>

/*
 * This program acts as a simple shell that is capable of handling exit, cd, history
 * as well as piped commands.
 * Terminates when user enters the 'exit' command
 */

int
main(int argc, char *argv[])
{

	// define the required variables
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	char *str1, *str2, *argument, *command;
	char *saveptr1, *saveptr2;
	int j;

	char* delim1 = " ";
	char* delim2 = " ";

	fp = stdin;

	while (1) {
		printf("sish> ");
		//getline() to get the input string
		read = getline(&line, &len, fp);			
		if(read == -1) //error checking
			break;
		line[strcspn(line, "\n")] = '\0';

		command = strtok_r(line, delim1, &saveptr1);
		if(command == NULL)
			break;
		if(strcmp(command, "exit") == 0) //if user enters "exit", exit program
			exit(EXIT_SUCCESS);
        else if(strcmp(command, "cd") == 0){ //ch [dir]
            argument = strtok_r(line, delim1, &saveptr1); //How to get this to read the second word in the line????
            argument = strtok_r(line, delim1, &saveptr1);

            int change = chdir(argument);
            if(change == -1) //What are parameters and return value for chdir()???????
                //print error
        } 
        else if(strcmp(command, "history") == 0){ //need to implement data structure for recording history. Double sided stack????
            argument = strtok_r(line, delim1, &saveptr1); //How to get this to read the second word in the line????
            argument = strtok_r(line, delim1, &saveptr1);

            //if history reaches more than 100 entries, delete the oldest entry to make room for new ones
            //history itself is a command and should appear in history
            //invald commands entered by user should also appear in history

            //if no argument
            for(int i = 0; i < 100; i++){
                //print i + command
            }

            //if argument == "-c"
            //clear the entire history

            //if argument == offset
            //execute command at given offset
            //print error if offset is invalid 
        }



            for (j = 1, str1 = line; ; j++, str1 = NULL) {

			str2 = token;
			while ((tokens = strtok_r(str2, delim2, &saveptr2)) != NULL){
				str2 = NULL;
			}

			printf("sentence %d: tokens %d\n", numOfSentences, numOfTokens);
		}
}
    exit(EXIT_SUCCESS);
}
