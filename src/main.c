#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <gcrypt.h>
#include <math.h>

#define NPROCS get_nprocs()

int main(const int argc, const char ** argv){
	/* Main function. Arg handling, function calling, etc. */

	int i, pid;

	for (i=0; i<NPROCS; i++){
		pid = fork();
		if (pid < 0){
			printf("Error creating process.\n");
			exit(1);
		}
		else if (pid == 0){
			printf("proc %d::\npid: %d\nargc: %d\n", i, getpid(), argc);
			int j = 0;
			while (argv[j]){
				printf("argv[%d]: %s\n", j, argv[j]);
				j++;
			}
			printf("\n");
		}
		else {
			wait(NULL);
		}
	}

	return 0;
}