#include <stdio.h>
#include <unistd.h>

int main(){
	int pid;
	printf("[%d] Before fork()\n",getpid());
	pid = fork();
	printf("[%d] Process : retuen value = %d\n",getpid(), pid);
}
