//Bradley Mixon
//CS3377.505 Project 1
//3-25-23

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXARGS 128

int MAXLINE = 1024;

enum builtin_t {
    NONE, EXIT, CD, HISTORY
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
	if(history_count == 100){ //check if histroy is currently full
		//free(history[0].cmd);
		for(int i = 1; i < 100; i++){
			history[i-1] = history[i];
		}
		//memmove(history, history+1, (history_count-1)*sizeof(Command));
		history_count--;
	}
	history[history_count].cmd = strdup(cmd);
	history_count++;
}


int parse(const char *cmdline, struct command_t *cmd) {
    const char delim[10] = " \t\r\n";
    char *line;
    char *token;
    char *endline;
    int is_bg;

    if(cmdline == NULL) {
        perror("command line is NULL\n");
        return -1;
    }

    line = malloc(MAXLINE);
    if(line == NULL){
    	perror("malloc failed");
	return -1;
    }

    (void) strncpy(line, cmdline, MAXLINE);
    endline = line + strlen(line);

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

    if(strcmp(cmd->argv[0], "exit") == 0) {
        cmd->builtin = EXIT;
    } else if(strcmp(cmd->argv[0], "cd") == 0) {
        cmd->builtin = CD;
    } else if(strcmp(cmd->argv[0], "history") == 0) {
        cmd->builtin = HISTORY;
    }

    if((is_bg = (*cmd->argv[cmd->argc-1] == '&')) != 0)
        cmd->argv[--cmd->argc] = NULL;

   // free(line);
    return is_bg;
}
	
void runSystemCommand(struct command_t *cmd, int bg) {
    pid_t childPid;

    if((childPid = fork()) < 0)
        perror("fork() error");
    else if(childPid == 0) {
        if(execvp(cmd->argv[0], cmd->argv) < 0) {
            printf("%s: Command not found\n", cmd->argv[0]);
            exit(0);
        }
    } else { //parent
        if(bg)
            printf("Child process in background [%d]\n", childPid);
        else
            wait(NULL);
    }
}


void runBuiltinCommand(struct command_t *cmd, int bg) {
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
    				runBuiltinCommand(&history_cmd, 0);
			} else {
    			runSystemCommand(&history_cmd, 0);
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
    int is_bg;

    while(1) {
        printf("sish> ");
        fflush(stdout);

        if(fgets(cmdline, MAXLINE, stdin) == NULL)
            exit(0);

        is_bg = parse(cmdline, &cmd);

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
	    runBuiltinCommand(&cmd, is_bg);
       	    add_to_history(cmd_as_string); //command needed to be added to history AFTER execution
       } else {
	    runSystemCommand(&cmd, is_bg);
            add_to_history(cmd_as_string); //command needed to be added to history AFTER execution
	}
    }

    return 0;
}
