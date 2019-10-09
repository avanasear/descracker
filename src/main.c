#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <crypt.h>
#include <math.h>

/* cryptlib's usage restrictions require that we credit Peter Gutmann
   if we decide to make this public after we finish. I don't know SHIT
   about licensing, so I guess we'll need to figure that out later.    */

// make sure we use every available processor
#define NPROCS get_nprocs()

struct thread_args {
	// keyspace arguments to pass into a thread
	long long unsigned start;
	long long unsigned end;
};

long long unsigned * find_keyspaces(void){
	/* Function to break up the available keyspace into NPROCS chunks */

	int i;
	long long unsigned max_key = pow(2, 56);
	long long unsigned * keyspace = malloc(sizeof(long long unsigned) * NPROCS);

	for (i=0; i<NPROCS; i++){
		keyspace[i] = max_key / NPROCS * i;
	}

	return keyspace;
}

void * print_thread(void * inputs){
	/* Test function to see if threads are properly created.
	   I'm not a great programmer; don't judge me. */

	// Get our needed variables from the function inputs
	struct thread_args * space = inputs;
	long long unsigned start = space->start;
	long long unsigned end = space->end;

	// Print every key % a billion (save some cycles by using mod)
	long long unsigned i;
	for (i=start; i<end; i++){
		if (i % 1000000000 == 0){
			printf("%llu\n", i);
		}
	}
}

int main(const int argc, const char ** argv){
	/* Main function. Arg handling, function calling, etc. */

	int i, rc;
	pthread_t threads[NPROCS];
	long long unsigned * keyspace = find_keyspaces();

	/*
	// Print the keyspace for some of the threads
	printf("Thread 0 using keyspace: %llu::%llu\n", keyspace[0], keyspace[1]-1);
	printf("Thread 1 using keyspace: %llu::%llu\n", keyspace[1], keyspace[2]-1);
	printf("Thread 2 using keyspace: %llu::%llu\n", keyspace[2], keyspace[3]-1);
	printf("Thread 3 using keyspace: %llu::%llu\n", keyspace[3], keyspace[4]-1);
	printf("Thread 4 using keyspace: %llu::%llu\n", keyspace[4], keyspace[5]-1);
	printf("Thread 5 using keyspace: %llu::%llu\n", keyspace[5], keyspace[6]-1);
	printf("Thread 6 using keyspace: %llu::%llu\n", keyspace[6], keyspace[7]-1);
	printf("Thread 7 using keyspace: %llu::72057594037927936\n\n", keyspace[7]); */

	// Create all our threads and give them their keyspaces.
	struct thread_args *args;
	for (i=0; i<NPROCS; i++){
		args = malloc(sizeof(struct thread_args));
		args->start = keyspace[i];
		args->end = keyspace[i] + keyspace[1];
		// keyspace[1] will always be the same size as one broken-up keyspace
		rc = pthread_create(&threads[i], NULL, print_thread, args);
		if (rc) {
			printf("Error - pthread_create() returned %d\n", rc);
			exit(1);
		}
	}

	// Don't judge me.
	for (i=0; i<NPROCS; i++){
		pthread_join(threads[i], NULL);
	}

	free(keyspace);
	free(args);

	return 0;
}
