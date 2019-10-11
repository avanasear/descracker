#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/des.h>

// 64-bit DES key
const_DES_cblock des_key = {0x00,0x00,0x00,0x11,0x22,0x33,0x44,0x55};

void print_data(const void * data, int len);

int main(){
	// 64-bit DES input data
	const_DES_cblock des_input = {0x6c,0x6f,0x6c,0x6c,0x6f,0x73,0x65,0x72};
	// Don't need an IV if we're using ECB.

	// Initialize some buffers to hold our encrypted data and decrypted data
	DES_cblock enc_out[sizeof(des_input)];
	DES_cblock dec_out[sizeof(des_input)];

	// Create DES key schedule
	DES_key_schedule key_sched;
	DES_set_key_unchecked(&des_key, &key_sched);

	// Encrypt our input and decrypt our encrypted output
	DES_ecb_encrypt(&des_input, enc_out, &key_sched, DES_ENCRYPT);
	DES_ecb_encrypt(enc_out, dec_out, &key_sched, DES_DECRYPT);

	// Print our stuff out
	printf("Input: \n");
	print_data(des_input, 8);
	printf("Encrypted: \n");
	print_data(enc_out, 8);
	printf("Decrypted: \n");
	print_data(dec_out, 8);

	printf("Writing our encrypted data to the `encrypted.txt` file...\n");
	FILE * fptr = fopen("encrypted.txt", "w");
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