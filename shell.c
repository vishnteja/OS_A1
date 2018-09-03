#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define clear() printf("\033[H\033[J")
#define BUFFSIZE 1024 
#define DELIM " " // Delimitter
#define COMMLEN 8 // Command Length

// Printing User Prompt
void printPrompt(){
	char cwd[1024];
	char * usrname = getenv("USER");
	getcwd(cwd, sizeof(cwd));
	printf("%s:~%s$ ", usrname, cwd);
}

// Every Shell Start 
void startShell(){
	clear();
	printf("Shell Created By VishnuTeja\n");
	printf("~.~.~.~.~.~.~.~.~.~.~.~.~.~\n");
}

// Read Input from user
char* readInput(){
	int size = BUFFSIZE;
	char* buffer = malloc(sizeof(char)*size);
	int len = 0;

	while(1){
		char temp = getchar();
		// Store Input in Buffer and end after enter or EOF
		if(temp==EOF||temp=='\n'){
			buffer[len] = '\0';
			return buffer;
		}else{
			buffer[len] = temp;
		}
		len++;

		// Reallocate Buffer Size if exceeded 
		if(len>=BUFFSIZE){
			size += BUFFSIZE;
			buffer = realloc(buffer, size);
		}
	}
}
// Add History 

// Tokenize the String
char** tokenInput(char* inp){
	int len = COMMLEN;
	char ** command = malloc(len*sizeof(char*));
	char* token = strtok(inp, DELIM);
	int index = 0;

	while(token != NULL){
		command[index] = token;
		// printf("%s", command[index]);
		index++;
		if(index>=len){
			len += COMMLEN;
			command = realloc(command, len*sizeof(char*));
			if(!command){
				printf("Memory Allocation Error");
			}
		}
		token = strtok(NULL, DELIM);
	}
	command[index] = NULL;
	return command;
} 

// Check if command exists in Command Table 

// Execute Command 

void exeCd(char** inp){
	if(!inp[1]){
		char cwd[1024];
		getcwd(cwd, sizeof(cwd));
		printf("Current Directory: %s\n", cwd);
	}
	else if(chdir(inp[1])<0){
		perror(inp[1]);
	}
}

void exeEcho(char** inp){
	printf("%s\n", inp[1]);
}

void exeLs(char** inp){
	int child_pid;
	int status; 

	child_pid = fork();
	if(child_pid==0){
		execvp(inp[0], inp);
		printf("Failed to Execute\n");

	}else{
		waitpid(child_pid, &status, WUNTRACED);
	}
}

void exeWc(char** inp){
	int child_pid;
	int status;

	child_pid = fork();
	if(child_pid==0){
		execvp(inp[0], inp);
		printf("Failed to Execute\n");
	}else{
		waitpid(child_pid, &status, WUNTRACED);
	}
}

void executeCmd(char** inp){
	if(!strcmp(inp[0],"cd")){
		exeCd(inp);
	}else if(!strcmp(inp[0], "echo")){
		exeEcho(inp);
	}else if(!strcmp(inp[0], "ls")){
		exeLs(inp);
	}else if(!strcmp(inp[0], "wc")){
		exeWc(inp);
	}else{
		printf("Error: Command Does Not Exist\n");
	}
}

// Signal Handler

int main()
{
	startShell();
	signal(SIGINT, SIG_IGN);
	while(1){
		printPrompt();
		char* inp = readInput();
		if(!strcmp(inp,"quit")){
			break;
		}
		char** command = tokenInput(inp);
		executeCmd(command);
		free(command);
	}
	return 0;
}