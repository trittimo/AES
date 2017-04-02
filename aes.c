#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "aes.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage:\n");
    printf("\taes [input file]\n");
    printf("\t\tInput file = [iterations] [rounds] [key] [plaintext]\n");
    exit(1);
  }
  // Initialize our variables
  uint8_t iterations;
  uint8_t rounds;
  uint8_t key[MAX_CHUNK_SIZE];
  uint8_t input[MAX_CHUNK_SIZE];

  FILE* file = fopen(argv[1], "r");
  if (!file) {
    fprintf(stderr, "No file named '%s' exists\n", argv[1]);
    exit(2);
  }
  // Set the variables from the input file
  fscanf(file, "%" SCNu8 " %" SCNu8, &iterations, &rounds);
  getByteArray(file, MAX_CHUNK_SIZE, key);
  getByteArray(file, MAX_CHUNK_SIZE, input);
  fclose(file);

  // Encrypt!
  // TODO

  return 0;
}



/* Reads the file for hexedecimal input and puts the output into the out array */
void getByteArray(FILE* file, int length, uint8_t* out) {
  char str[2];
  for (int i = 0; i < length; i++) {
    str[0] = (char) fgetc(file);
    while (str[0] == '\n') {
      str[0] = (char) fgetc(file);
    }
    str[1] = (char) fgetc(file);
    out[i] = (uint8_t) strtol(str, NULL, 16);
  }
}