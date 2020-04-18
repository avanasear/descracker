#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/des.h>

// 64-bit DES key
const_DES_cblock des_key = {0x00,0x00,0x00,0x00,0x16,0x4B,0x45,0x59};

void print_data(const void * data, int len);

int main(void){
	char plaintext[9] = {0};
	printf("Enter 8 characters of plaintext: ");
	fgets(plaintext, 9, stdin);
	printf("Encrypting '%s'...\n", plaintext);

	// 64-bit DES input data
	const_DES_cblock des_input = {
		plaintext[0],
		plaintext[1],
		plaintext[2],
		plaintext[3],
		plaintext[4],
		plaintext[5],
		plaintext[6],
		plaintext[7]
	};
	// Don't need an IV if we're using ECB.

	// Initialize some buffers to hold our encrypted data and decrypted data
	DES_cblock enc_out[sizeof(des_input)];

	// Create DES key schedule
	DES_key_schedule key_sched;
	DES_set_key_unchecked(&des_key, &key_sched);

	// Encrypt our input and decrypt our encrypted output
	DES_ecb_encrypt(&des_input, enc_out, &key_sched, DES_ENCRYPT);

	// Print our stuff out
	printf("Unencrypted bytes: \n");
	print_data(des_input, 8);
	printf("Encrypted bytes: \n");
	print_data(enc_out, 8);

	printf("Writing our unencrypted data to the `plaintext.txt` file...\n");
	FILE * fptr = fopen("plaintext.txt", "w");
	fwrite(&des_input, 1, 8, fptr);
	fclose(fptr);
	printf("Writing our encrypted data to the `ciphertext.txt` file...\n");
	fptr = fopen("ciphertext.txt", "w");
	fwrite(&enc_out, 1, 8, fptr);
	fclose(fptr);
}

void print_data(const void * data, int len){
	int i;
	const unsigned char * p = (const unsigned char *) data;

	// Print the array
	for (i=0; i<len; ++i){
		printf("%02x ", *p++);
	}
	printf("\n");
}
