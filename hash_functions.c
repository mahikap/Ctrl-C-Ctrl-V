#include <stdio.h>
#include <stdlib.h>

/*
 * Compute an xor-based hash of the data provided on STDIN. The result
 * should be placed in the array of length block_size pointed to by
 * hash_val.
 */
char *hash(FILE *f) {

    char *hash_val = malloc(sizeof(char) * 8);

    for (int i = 0; i < 8; i++) {
	hash_val[i] = '\0';
    }

    char input;
    int i = 0;
    while ((input = fgetc(f)) != EOF) {
	hash_val[i] = hash_val[i] ^ input;

	if (hash_val[i] == '0') {
	    hash_val[i] = '\0';
	}

	/* updates the ith index and keeps looping back to the
	 * beginning of the block_size once it reaches the end of the
	 * block_size.
	*/
	i = i + 1;
	i = i % 8;
    }
    return hash_val;

}
