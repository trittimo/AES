/*
  Program written by Michael Trittin
  A few notes:
  - The majority of work happens in the functions defined in util.h
  - Constants (such as sbox, rcon, etc.) were taken from Wikipedia
  - Debug printing can be toggled by defining the DEBUG macro -- this is done in the
    make file, so it shouldn't be necessary to do so manually
*/

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

  // We're ready to begin encryption
  fprintf(out, "\n******** BEGIN ENCRYPTION ********");


  // Set up some variables we can store the transformed data in
  unsigned char state[16];
  unsigned char mykey[16];
  unsigned char plaintext[16];
  unsigned char original[16];

  // Copy the state and key in via column order
  readColumnOrder(state, input);
  readColumnOrder(mykey, key);

  // Save the plaintext and original key for later use
  for (int i = 0; i < 16; i++) {
    plaintext[i] = state[i];
    original[i] = mykey[i];
  }

  // For the number of iterations, do the encryption
  for (int j = 0; j < iterations; j++) {
    encrypt(state, mykey, rounds, out);

    // The state is xor'd with the plaintext if it's not the last iteration
    // We also revert the key to its original state
    if (j != iterations-1) {
      for (int i = 0; i < 16; i++) {
        state[i] ^= plaintext[i];
        mykey[i] = original[i];
      }
    }
  }

  // Finished encrypting, printing the final state
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

/* Reads the given input array column-major order into the destination array */
void readColumnOrder(unsigned char* dest, unsigned char* in) {
  for (int i = 0; i < 4; i++) {
    dest[i] = in[i*4];
    dest[i + 4] = in[i*4 + 1];
    dest[i + 8] = in[i*4 + 2];
    dest[i + 12] = in[i*4 + 3];
  }
}

/* 
  Does the actual encryption given the state, key, and number of rounds to perform
  If debug is on, will print out each step along the way as well
*/
void encrypt(unsigned char* state, unsigned char* key, int rounds, FILE* out) {
  print(out, "\nInput key\n", key);
  print(out, "\n\nInput block\n", state)
  
  // Initial round
  addRoundKey(state, key);
  print(out, "\n\nInitial AddRoundKey\n", state);

  for (int i = 0; i < rounds-1; i++) {
    printformatif(out, "\n\nRound %d\n", i + 1);

    // SubBytes
    subBytes(state);
    print(out, "SubBytes\n", state);

    // ShiftRows
    shiftRows(state);
    print(out, "\n\nShiftRows\n", state);

    // MixColumns
    mixColumns(state);
    print(out, "\n\nMixColumns\n", state);

    // Get the next round key
    nextRoundKey(key, i);
    print(out, "\n\nRound Key\n", key);

    // AddRoundKey
    addRoundKey(state, key);
    print(out, "\n\nAddRoundKey\n", state);
  }

  // The final round
  printif(out, "\n\nFinal Round\n");

  // SubBytes
  subBytes(state);
  print(out, "SubBytes\n", state);

  // ShiftRows
  shiftRows(state);
  print(out, "\n\nShiftRows\n", state);

  // Get the next round key
  nextRoundKey(key, rounds-1);
  print(out, "\n\nRound Key\n", key);

  // AddRoundKey
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