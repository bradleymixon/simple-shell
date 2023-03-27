//Bradley Mixon
//CS3377.505 Project 1
//3-25-23

//NOTE: Part 3 - Pipes is unfinished. I spent significant time working on this feature but was not able to get it working by the deadline

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXARGS 128 //maximum # of args

int MAXLINE = 1024; //maximum line length

enum builtin_t {
    NONE, EXIT, CD, HISTORY //possible built-in commands that the user can enter
};

struct command_t{
    int argc;
    char *argv[MAXARGS];
    enum builtin_t builtin;
};

//HISTORY FUNCTIONALITY
typedef struct{
	char* cmd;
} Command;

Command history[100];// Array of Command structs with 100 spots
int history_count = 0; //Count of how many entries are in history
void add_to_history(char* cmd){
	if(history_count == 100){ //check if histroy is currently full. i.e. at 100 entries
		for(int i = 1; i < 100; i++){
			history[i-1] = history[i]; //shift all entries to the left
		}
		history_count--; //decrement history_count to 99
	}
	history[history_count].cmd = strdup(cmd); //current cmd is placed in history
	history_count++; //history_count increment (max possible value is 100) 
}

//function to parse user input and determine if system command (executable) or built in command or invalid command
int parse(const char *cmdline, struct command_t *cmd) { 
    const char delim[10] = " \t\r\n";
    char *line;
    char *token;
    char *endline;

    if(cmdline == NULL) { //check if cmdline is NULL or not
        perror("command line is NULL\n");
        return -1;
    }

    line = malloc(MAXLINE); //allocate space for line
    if(line == NULL){ //check if allocation was successful
    	perror("malloc failed");
	return -1;
    }

    (void) strncpy(line, cmdline, MAXLINE);
    endline = line + strlen(line); //find endline

    cmd->argc = 0;

    while(line < endline) {
        line += strspn(line, delim);
        if(line >= endline)
            break;
        token = line + strcspn(line, delim);
        *token = '\0';

        cmd->argv[cmd->argc++] = line;

        if(cmd->argc >= MAXARGS-1)
            break;
        line = token + 1;
    }

    cmd->argv[cmd->argc] = NULL;

    if(cmd->argc == 0)
        return 1;

    cmd->builtin = NONE;

    if(strcmp(cmd->argv[0], "exit") == 0) { //check if command = "exit"
        cmd->builtin = EXIT;
    } else if(strcmp(cmd->argv[0], "cd") == 0) { //check if command = "cd"
        cmd->builtin = CD;
    } else if(strcmp(cmd->argv[0], "history") == 0) { //check if command = "history"
        cmd->builtin = HISTORY;
    }

   return 0;
}
	
void runSystemCommand(struct command_t *cmd) {
    pid_t childPid;

    if((childPid = fork()) < 0)
        perror("fork() error");
    else if(childPid == 0) {
        if(execvp(cmd->argv[0], cmd->argv) < 0) {
            printf("%s: Command not found\n", cmd->argv[0]);
            exit(0);
        }
    } else { //parent
            wait(NULL);
    }
}


void runBuiltinCommand(struct command_t *cmd) {
    switch(cmd->builtin) {
        case EXIT:
            exit(0);
            break;
        case CD:
	    if(chdir(cmd->argv[1]) != 0)
	    	perror("chdir() failed");
            break;
        case HISTORY:
            if(cmd->argv[1] == NULL){
	    	//printf("Command is history\n");
		for(int i = 0; i < history_count; i++){
			printf("%d %s\n", i, history[i].cmd);
		}
	    }
	    else if(strcmp(cmd->argv[1], "-c") == 0){
	    	//printf("Command is history -c\n");
		for(int i = 0; i < history_count; i++){
			free(history[i].cmd);
		}
		history_count = 0;
	    } else {
	    	//printf("Command is history [offset]\n");
		int offset = atoi(cmd->argv[1]);
		if(offset >= 0 && offset <= history_count){ //check if offset is valid
			struct command_t history_cmd;
			parse(history[offset].cmd, &history_cmd);
			if (history_cmd.builtin != NONE) {
    				runBuiltinCommand(&history_cmd);
			} else {
    			runSystemCommand(&history_cmd);
			}	
		}
	    }
	    break;
        default:
            printf("sish: internal error\n");
            break;
    }
}

int main() {
    char cmdline[MAXLINE];
    struct command_t cmd;

    while(1) {
        printf("sish> ");
        fflush(stdout);

        if(fgets(cmdline, MAXLINE, stdin) == NULL)
            exit(0);

        parse(cmdline, &cmd);

	char cmd_as_string[MAXLINE];
	memset(cmd_as_string, '\0', sizeof(cmd_as_string));
	for(int i = 0; i < cmd.argc; i++){
		strcat(cmd_as_string, cmd.argv[i]);
		strcat(cmd_as_string, " ");
	}
	//add_to_history(cmd_as_string); <-- This was causing an issue when testing hist100 because the command at index 1 was replaced
	//					before history 1 was executed

	if(cmd.argc == 0){
	//printf("\n");
	}
        else if (cmd.builtin != NONE) {
	    runBuiltinCommand(&cmd);
       	    add_to_history(cmd_as_string); //command needed to be added to history AFTER execution
       } else {
	    runSystemCommand(&cmd);
            add_to_history(cmd_as_string); //command needed to be added to history AFTER execution
	}
    }

    return 0;
}
//Bradley Mixon
//CS3377.505 Project 1
//3-25-23

//NOTE: Part 3 - Pipes is unfinished. I spent significant time working on this feature but was not able to get it working by the deadline

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXARGS 128 //maximum # of args

int MAXLINE = 1024; //maximum line length

enum builtin_t {
    NONE, EXIT, CD, HISTORY //possible built-in commands that the user can enter
};

struct command_t{
    int argc;
    char *argv[MAXARGS];
    enum builtin_t builtin;
};

//HISTORY FUNCTIONALITY
typedef struct{
	char* cmd;
} Command;

Command history[100];// Array of Command structs with 100 spots
int history_count = 0; //Count of how many entries are in history
void add_to_history(char* cmd){
	if(history_count == 100){ //check if histroy is currently full. i.e. at 100 entries
		for(int i = 1; i < 100; i++){
			history[i-1] = history[i]; //shift all entries to the left
		}
		history_count--; //decrement history_count to 99
	}
	history[history_count].cmd = strdup(cmd); //current cmd is placed in history
	history_count++; //history_count increment (max possible value is 100) 
}

//function to parse user input and determine if system command (executable) or built in command or invalid command
int parse(const char *cmdline, struct command_t *cmd) { 
    const char delim[10] = " \t\r\n";
    char *line;
    char *token;
    char *endline;

    if(cmdline == NULL) { //check if cmdline is NULL or not
        perror("command line is NULL\n");
        return -1;
    }

    line = malloc(MAXLINE); //allocate space for line
    if(line == NULL){ //check if allocation was successful
    	perror("malloc failed");
	return -1;
    }

    (void) strncpy(line, cmdline, MAXLINE);
    endline = line + strlen(line); //find endline

    cmd->argc = 0;

    while(line < endline) {
        line += strspn(line, delim);
        if(line >= endline)
            break;
        token = line + strcspn(line, delim);
        *token = '\0';

        cmd->argv[cmd->argc++] = line;

        if(cmd->argc >= MAXARGS-1)
            break;
        line = token + 1;
    }

    cmd->argv[cmd->argc] = NULL;

    if(cmd->argc == 0)
        return 1;

    cmd->builtin = NONE;

    if(strcmp(cmd->argv[0], "exit") == 0) { //check if command = "exit"
        cmd->builtin = EXIT;
    } else if(strcmp(cmd->argv[0], "cd") == 0) { //check if command = "cd"
        cmd->builtin = CD;
    } else if(strcmp(cmd->argv[0], "history") == 0) { //check if command = "history"
        cmd->builtin = HISTORY;
    }

   return 0;
}
	
void runSystemCommand(struct command_t *cmd) {
    pid_t childPid;

    if((childPid = fork()) < 0)
        perror("fork() error");
    else if(childPid == 0) {
        if(execvp(cmd->argv[0], cmd->argv) < 0) {
            printf("%s: Command not found\n", cmd->argv[0]);
            exit(0);
        }
    } else { //parent
            wait(NULL);
    }
}


void runBuiltinCommand(struct command_t *cmd) {
    switch(cmd->builtin) {
        case EXIT:
            exit(0);
            break;
        case CD:
	    if(chdir(cmd->argv[1]) != 0)
	    	perror("chdir() failed");
            break;
        case HISTORY:
            if(cmd->argv[1] == NULL){
	    	//printf("Command is history\n");
		for(int i = 0; i < history_count; i++){
			printf("%d %s\n", i, history[i].cmd);
		}
	    }
	    else if(strcmp(cmd->argv[1], "-c") == 0){
	    	//printf("Command is history -c\n");
		for(int i = 0; i < history_count; i++){
			free(history[i].cmd);
		}
		history_count = 0;
	    } else {
	    	//printf("Command is history [offset]\n");
		int offset = atoi(cmd->argv[1]);
		if(offset >= 0 && offset <= history_count){ //check if offset is valid
			struct command_t history_cmd;
			parse(history[offset].cmd, &history_cmd);
			if (history_cmd.builtin != NONE) {
    				runBuiltinCommand(&history_cmd);
			} else {
    			runSystemCommand(&history_cmd);
			}	
		}
	    }
	    break;
        default:
            printf("sish: internal error\n");
            break;
    }
}

int main() {
    char cmdline[MAXLINE];
    struct command_t cmd;

    while(1) {
        printf("sish> ");
        fflush(stdout);

        if(fgets(cmdline, MAXLINE, stdin) == NULL)
            exit(0);

        parse(cmdline, &cmd);

	char cmd_as_string[MAXLINE];
	memset(cmd_as_string, '\0', sizeof(cmd_as_string));
	for(int i = 0; i < cmd.argc; i++){
		strcat(cmd_as_string, cmd.argv[i]);
		strcat(cmd_as_string, " ");
	}
	//add_to_history(cmd_as_string); <-- This was causing an issue when testing hist100 because the command at index 1 was replaced
	//					before history 1 was executed

	if(cmd.argc == 0){
	//printf("\n");
	}
        else if (cmd.builtin != NONE) {
	    runBuiltinCommand(&cmd);
       	    add_to_history(cmd_as_string); //command needed to be added to history AFTER execution
       } else {
	    runSystemCommand(&cmd);
            add_to_history(cmd_as_string); //command needed to be added to history AFTER execution
	}
    }

    return 0;
}
