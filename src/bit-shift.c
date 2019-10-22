#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>

void insert_bits(long long unsigned key_ind);

int main(void){
	long long unsigned keynum = 72057594037927935ULL;
	printf("KeyNum: %llu\n", keynum);
	printf("Shifted: ");
	insert_bits(keynum);

	printf("\n");
	return 0;
}

void insert_bits(long long unsigned key_ind){
	long long unsigned arr[8];

	/* Note that this doesn't necessarily need to be an array,
	   but doing it this way makes it transfer more easily to
	   doing it with a DES cblock.                            */

	arr[0] = ((key_ind >> 49)       ) | 0x80;
	arr[1] = ((key_ind >> 42) & 0x7f) | 0x80;
	arr[2] = ((key_ind >> 35) & 0x7f) | 0x80;
	arr[3] = ((key_ind >> 28) & 0x7f) | 0x80;
	arr[4] = ((key_ind >> 21) & 0x7f) | 0x80;
	arr[5] = ((key_ind >> 14) & 0x7f) | 0x80;
	arr[6] = ((key_ind >>  7) & 0x7f) | 0x80;
	arr[7] = ((key_ind      ) & 0x7f) | 0x80;

	printf("%02llx ", arr[0]);
	printf("%02llx ", arr[1]);
	printf("%02llx ", arr[2]);
	printf("%02llx ", arr[3]);
	printf("%02llx ", arr[4]);
	printf("%02llx ", arr[5]);
	printf("%02llx ", arr[6]);
	printf("%02llx ", arr[7]);
}
