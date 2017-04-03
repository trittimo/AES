#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "aes.h"

int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("Usage:\n");
    printf("\taes [input file] [output file]\n");
    printf("\t\tInput file = [iterations] [rounds] [key] [plaintext]\n");
    exit(1);
  }
  // Initialize our variables
  int iterations;
  int rounds;
  int key[MAX_CHUNK_SIZE];
  int input[MAX_CHUNK_SIZE];
  int output[MAX_CHUNK_SIZE];

  FILE* file = fopen(argv[1], "r");
  if (!file) {
    fprintf(stderr, "No file named '%s' exists\n", argv[1]);
    exit(2);
  }

  FILE* out = fopen(argv[2], "w");
  if (!out) {
    fprintf(stderr, "Unable to open handle for '%s'\n", argv[2]);
    exit(2);
  }
  // Set the variables from the input file
  fscanf(file, "%d %d", &iterations, &rounds);
  getByteArray(file, MAX_CHUNK_SIZE, key);
  getByteArray(file, MAX_CHUNK_SIZE, input);
  fclose(file);

  fprintf(out, "Number of iterations: %d\n", iterations);
  fprintf(out, "Number of rounds: %d\n", rounds);
  fprintf(out, "Key: ");
  for (int i = 0; i < MAX_CHUNK_SIZE; i++) {
    fprintf(out, "%x", key[i]);
  }
  fprintf(out, "\nPlaintext: ");
  for (int i = 0; i < MAX_CHUNK_SIZE; i++) {
    fprintf(out, "%x", input[i]);
  }
  fprintf(out, "\n****** BEGIN ENCRYPTION ******\n");

  encrypt(iterations, rounds, key, input, output);

  fprintf(out, "****** END ENCRYPTION ******\nFinal block: ");
  for (int i = 0; i < MAX_CHUNK_SIZE; i++) {
    fprintf(out, "%x", output[i]);
  }

  return 0;
}

void encrypt(int iterations, int rounds, int* key, int* input, int* output) {
  for (int i = 0; i < MAX_CHUNK_SIZE; i++) {
    output[i] = input[i];
  }
}

/* Reads the file for hexedecimal input and puts the output into the out array */
void getByteArray(FILE* file, int length, int* out) {
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