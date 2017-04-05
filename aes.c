#include <stdio.h>
#include <stdlib.h>
#include "aes.h"
#include "util.h"

#ifdef DEBUG
#define print(out, step, item) fprintf(out, "%s", step); writeOutput(out, item);
#define printformatif(a, b, c) fprintf(a, b, c)
#define printif(a, b) fprintf(a, b)
#else
#define print(out, step, item)
#define printformatif(a, b, c)
#define printif(a, b)
#endif

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
  unsigned char key[16];
  unsigned char input[16];


  // Open the input and output files
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
  getByteArray(file, 16, key);
  getByteArray(file, 16, input);
  fclose(file);

  fprintf(out, "Number of iterations: %d\n", iterations);
  fprintf(out, "Number of rounds: %d\n", rounds);
  fprintf(out, "Key: \n");
  for (int i = 0; i < 16; i++) {
    fprintf(out, "%02x", key[i]);
  }
  fprintf(out, "\nPlaintext: \n");
  for (int i = 0; i < 16; i++) {
    fprintf(out, "%02x", input[i]);
  }
  fprintf(out, "\n******** BEGIN ENCRYPTION ********");

  unsigned char state[16];
  unsigned char mykey[16];
  unsigned char plaintext[16];
  unsigned char original[16];

  readColumnOrder(state, input);
  readColumnOrder(mykey, key);

  for (int i = 0; i < 16; i++) {
    plaintext[i] = state[i];
    original[i] = mykey[i];
  }

  for (int j = 0; j < iterations; j++) {
    encrypt(state, mykey, rounds, out);
    if (j != iterations-1) {
      for (int i = 0; i < 16; i++) {
        state[i] ^= plaintext[i];
        mykey[i] = original[i];
      }
    }
  }

  fprintf(out, "\n********  END ENCRYPTION  ********\nFinal block:\n");
  for (int i = 0; i < 4; i++) {
    fprintf(out, "%02x", state[i]);
    fprintf(out, "%02x", state[i+4]);
    fprintf(out, "%02x", state[i+8]);
    fprintf(out, "%02x", state[i+12]);
  }

  fclose(out);

  return 0;
}

void readColumnOrder(unsigned char* dest, unsigned char* in) {
  for (int i = 0; i < 4; i++) {
    dest[i] = in[i*4];
    dest[i + 4] = in[i*4 + 1];
    dest[i + 8] = in[i*4 + 2];
    dest[i + 12] = in[i*4 + 3];
  }
}

void encrypt(unsigned char* state, unsigned char* key, int rounds, FILE* out) {
  print(out, "\nInput key\n", key);
  print(out, "\n\nInput block\n", state)
  
  // Initial round
  addRoundKey(state, key);
  print(out, "\n\nInitial AddRoundKey\n", state);
  for (int i = 0; i < rounds-1; i++) {
    printformatif(out, "\n\nRound %d\n", i + 1);
    subBytes(state);
    print(out, "SubBytes\n", state);
    shiftRows(state);
    print(out, "\n\nShiftRows\n", state);
    mixColumns(state);
    print(out, "\n\nMixColumns\n", state);
    nextRoundKey(key, i);
    print(out, "\n\nRound Key\n", key);
    addRoundKey(state, key);
    print(out, "\n\nAddRoundKey\n", state);
  }
  printif(out, "\n\nFinal Round\n");
  subBytes(state);
  print(out, "SubBytes\n", state);
  shiftRows(state);
  print(out, "\n\nShiftRows\n", state);
  nextRoundKey(key, rounds-1);
  print(out, "\n\nRound Key\n", key);
  addRoundKey(state, key);
  print(out, "\n\nAddRoundKey\n", state);
}

/* Reads the file for hexedecimal input and puts the output into the out array */
void getByteArray(FILE* file, int length, unsigned char* out) {
  char str[2];
  for (int i = 0; i < length; i++) {
    str[0] = (char) fgetc(file);
    while (str[0] == '\n') {
      str[0] = (char) fgetc(file);
    }
    str[1] = (char) fgetc(file);
    out[i] = (unsigned char) strtol(str, NULL, 16);
  }
}