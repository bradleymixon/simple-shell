# simple-shell
CS/SE 3377 Sys. Prog. in Unix and Other Envs. Project 1

In this project, you will implement a command line interpreter or shell. The shell should operate in this basic way: when you type in a command (in response to its prompt), the shell creates a child process that executes the command you entered and then prompts for more user input when it has finished.

## Part 1 - The Simple Shell
- Shell executable must be named **sish**. Source code must be in file **sish.c**
- Shell should run continuously and display a prompt when waiting for input. The prompt should be EXACTLY **sish>** . (Space after >)
- Shell should read a line from stdin one at a time. This line should be parsed out into a command and all its arguments via tokenizing.
  - The only supported delimiter is the whitespace character (ASCII character number 32).
  - Shell does not need to handle "special" characters (quotation marks, backslashes, and tab characters). Therefore the shell will be unable to support    arguments with spaces in them. For example, the shell will not support file paths with spaces in them.
  - The shell should handle input lines of any length.
- After parsing the command, the shell should execute it. A command can either be a reference to an executable OR a built-in shell command (see Part 2). 
  - Executing commands that are not shell built-ins and are just the executable name (and not a full path to the executable) is done by invoking fork() and then invoking execvp().
  - You may NOT use the system() function, as it just invokes the /bin/sh shell to do all the work.
  
## Part 2 - Implement Built-in Commands: exit, cd, history
- exit - Simply exits your shell after performing any necessary clean up.
- cd [dir] - Short for "change directory" and will be used to change the current working
directory of your shell. Only implement cd such that it takes a single command line argument: The directory to change to. To change directory, use the chdir()system call with the argument supplied by the user. If chdir fails, it is an error.
- history [-c] [offset] - Like the Bash built-in history command, but much simpler.
  - history (without arguments) displays the last 100 commands the user ran, with
an offset next to each command. The offset is the index of the command in the list, and valid values are 0 to 99, inclusive. 0 is the oldest command. Do not worry about saving this list to a file; just store it in memory. Once more than 100 commands are executed, remove the oldest entry from the list to make room for the newer commands. Note that history is also a command itself and therefore should also appear in the list of commands. If the user ran invalid commands, those should also appear in the list.
  - history -c clears the entire history, removing all entries. For example, running history immediately after history -c should show history as the sole entry in the list.
  - history [offset] executes the command in history at the given offset. Print an error message of your choosing if the offset is not valid.
  
## Part 3 - Pipes
- Augment your shell to be capable of executing a sequence of programs that communicate through a pipe. For example, if the user enters ls | wc, your program should fork the two programs, which together will calculate the number of files in the directory. For this you will need to replace stdin and stdout with pipe file descriptors using dup2.
- While this example shows two processes communicating through a pipe, your shell should support pipes between multiple processes, not just two.
- You need not support built-in commands to work with pipes.

## References
- [EnthusiastiCon - Stefanie Schirmer](https://www.youtube.com/watch?v=k6TTj4C0LF0)
