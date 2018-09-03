#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
	char** argv = malloc(sizeof(char*)*2);
	argv[0] = "ls";
	argv[1] = NULL;
	execvp("ls", argv);
	return 0;
}