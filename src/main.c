#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/sysinfo.h>
#include <openssl/crypto.h> // not used yet but will be soon
#include <math.h>

/* Gonna put some notes right here...

   DES_ecb_encrypt() is the basic DES encryption routine that encrypts or
   decrypts a single 8-byte DES_cblock in electronic code book (ECB) mode.
   It always transforms the input data, pointed to by input, into the output
   data, pointed to by the output argument. If the encrypt argument is
   non-zero (DES_ENCRYPT), the input (cleartext) is encrypted in to the
   output (ciphertext) using the key_schedule specified by the schedule
   argument, previously set via DES_set_key. If encrypt is zero
   (DES_DECRYPT), the input (now ciphertext) is decrypted into the output
   (now cleartext). Input and output may overlap. DES_ecb_encrypt() does not
   return a value. (https://www.openssl.org/docs/man1.0.2/man3/des.html)

   That site includes more information about it - need to set key/schedule/etc

*/

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

	printf("%d; %s\n", argc, argv[0]); // just to remove 'unused variable's

	int i, rc;
	pthread_t threads[NPROCS];
	long long unsigned * keyspace = find_keyspaces();

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
