#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *arvg[]){
	int child, pid, status;
	pid = fork();
	if (pid == 0) {/*child process*/
	       printf("PGRP of child = %d\n", getpgrp());
       		while(1){
			printf("child is waiting...\n");
			sleep(1);
       		}
	} else {/*parent process*/
	       printf("PGRP of parent = %d\n", getpgrp());
		sleep(5);
	       kill(-getpid(), 9);

	       printf("[%d] child %d is terminated \n", getpid(),pid);
	       printf("/t...with status %d\n",status>>8);
	       printf("parent is sleeping...\n");
	       sleep(5);
	}
       return 0;
}

		

		 
