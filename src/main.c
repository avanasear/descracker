#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <openssl/des.h>
#include <math.h>

// make sure we use every available processor
#define NPROCS get_nprocs()

unsigned short found_a_key = 0;
//const_DES_cblock plaintext = {0x6c,0x6f,0x6c,0x6c,0x6f,0x73,0x65,0x72};
DES_cblock plaintext[8];
char * HELP = "Syntax: descracker <ciphertext file> <plaintext file>";

struct thread_args {
	// keyspace arguments to pass into a thread
	long long unsigned start;
	long long unsigned end;
	const_DES_cblock cipher;
};

// initialize functions
long long unsigned * find_keyspaces(void);
void * decrypt_thread(void * inputs);
void print_data(const void * data, int len);
void print_plain(const void * data, int len);

int main(const int argc, const char ** argv){
	// Main function. Arg handling, function calling, etc.

	const_DES_cblock ciphertext[8];
	if ((argc == 2) && (!(strcmp(argv[1], "--help"))
					|| (!(strcmp(argv[1], "-h"))))){
		printf("%s\n", HELP);
		exit(0);
	}
	else if (argc == 3){
		// open the files specified and get the data ready to use
		FILE * cptr;
		if ((cptr = fopen(argv[1], "r"))){
			fread(ciphertext, sizeof(unsigned char), 8, cptr);
			fclose(cptr);
			printf("Attempting to decrypt: ");
			print_data(ciphertext, 8);
		}
		else {
			printf("Couldn't open file: %s\n", argv[1]);
			exit(1);
		}

		FILE * pptr;
		if ((pptr = fopen(argv[2], "r"))){
			fread(plaintext, sizeof(unsigned char), 8, pptr);
			fclose(pptr);
		}
		else {
			printf("Couldn't open file: %s\n", argv[2]);
			exit(1);
		}
	}
	else {
		printf("Incorrect arguments.\n%s\n", HELP);
		exit(1);
	}

	int i, rc;
	pthread_t threads[NPROCS];
	long long unsigned * keyspace = find_keyspaces();

	// Create all our threads and give them their keyspaces.
	struct thread_args *args = {0};
	for (i=0; i<NPROCS; i++){
		args = malloc(sizeof(struct thread_args));
		args->start = keyspace[i];
		args->end = keyspace[i] + keyspace[1];
		memcpy(args->cipher, ciphertext, 8);
		// keyspace[1] will always be the same size as one broken-up keyspace
		rc = pthread_create(&threads[i], NULL, decrypt_thread, args);
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

	if (found_a_key == 0){
		printf("Unable to find a working key.\n");
		printf("Maybe the ciphertext and plaintext don't correlate?\n");
	}

	return 0;
}

long long unsigned * find_keyspaces(void){
	// Function to break up the available keyspace into NPROCS chunks.

	int i;
	long long unsigned max_key = pow(2, 56);
	long long unsigned * keyspace = malloc(sizeof(long long unsigned) * NPROCS);

	for (i=0; i<NPROCS; i++){
		keyspace[i] = max_key / NPROCS * i;
	}

	return keyspace;
}

void * decrypt_thread(void * inputs){
	// Attempt to decrypt the ciphertext.

	// Get our needed variables from the function inputs
	struct thread_args * space = inputs;
	long long unsigned start = space->start;
	long long unsigned end = space->end;
	const_DES_cblock des_input[8];
	memcpy(des_input, space->cipher, 8);

	DES_cblock dec_out[8];
	DES_key_schedule key_sched;

	// Print every key % a billion (save some cycles by using mod)
	long long unsigned i;
	for (i=start; i<end; i++){
		if (found_a_key == 0){
			// create key candidate to try
			const_DES_cblock key_candidate = {
				(unsigned char)(((i >> 49)       ) | 0x00),
				(unsigned char)(((i >> 42) & 0x7f) | 0x00),
				(unsigned char)(((i >> 35) & 0x7f) | 0x00),
				(unsigned char)(((i >> 28) & 0x7f) | 0x00),
				(unsigned char)(((i >> 21) & 0x7f) | 0x00),
				(unsigned char)(((i >> 14) & 0x7f) | 0x00),
				(unsigned char)(((i >>  7) & 0x7f) | 0x00),
				(unsigned char)(((i)       & 0x7f) | 0x00)
			};

			// set keyschedule and decrypt
			DES_set_key_unchecked(&key_candidate, &key_sched);
			DES_ecb_encrypt(des_input, dec_out, &key_sched, DES_DECRYPT);

			// if we found a key, tell all the other processes to stop
			if (memcmp(dec_out, plaintext, 8) == 0){
				found_a_key = 1;
				printf("Found a working key: ");
				print_data(key_candidate, 8);
				printf("Plaintext bytes: ");
				print_data(dec_out, 8);
				printf("Plaintext: ");
				print_plain(dec_out, 8);
				break;
			}
			else {
				continue;
			}
		}
		else {
			break;
		}
	}
}

void print_data(const void * data, int len){
	/* Function to print (usually 8) bytes so that I don't have to make a 
	   for loop every time */

	int i;
	const unsigned char * p = (const unsigned char *) data;

	// print the array
	for (i=0; i<len; ++i){
		printf("%02x ", *p++);
	}
	printf("\n");
}

void print_plain(const void * data, int len){
	/* Function to print (usually 8) bytes so that I don't have to make a 
	   for loop every time */

	int i;
	const unsigned char * p = (const unsigned char *) data;

	// print the array
	for (i=0; i<len; ++i){
		printf("%c", *p++);
	}
	printf("\n");
}