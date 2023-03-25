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

void runBuiltinCommand(struct command_t *cmd, int bg) {
    switch(cmd->builtin) {
        case EXIT:
            exit(0);
            break;
        case CD:
            //printf("sish: cd command not implemented yet\n");
	    if(chdir(cmd->argv[1]) != 0)
	    	perror("chdir() to failed");
            break;
        case HISTORY:
            printf("sish: history command not implemented yet\n");
            break;
        default:
            printf("sish: internal error\n");
            break;
    }
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

	if(cmd.argc == 0){
	}
        else if (cmd.builtin != NONE) {
            runBuiltinCommand(&cmd, is_bg);
        } else {
            runSystemCommand(&cmd, is_bg);
        }
    }

    return 0;
}
