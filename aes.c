#include <stdio.h>
#include "aes.h"
#include "util.h"

int main(int argc, char* argv[]) {
  unsigned char state[16] = {0x32, 0x43, 0xf6, 0xa8,
                             0x88, 0x5a, 0x30, 0x8d,
                             0x31, 0x31, 0x98, 0xa2,
                             0xe0, 0x37, 0x07, 0x34};
  unsigned char key[16] = {0x2b, 0x7e, 0x15, 0x16, 
                           0x28, 0xae, 0xd2, 0xa6,
                           0xab, 0xf7, 0x15, 0x88,
                           0x09, 0xcf, 0x4f, 0x3c};
  int iterations = 2;
  int rounds = 10;

  unsigned char plaintext[16];
  unsigned char original[16];

  for (int i = 0; i < 16; i++) {
    plaintext[i] = state[i];
    original[i] = key[i];
  }

  for (int j = 0; j < iterations; j++) {
    print(stdout, "Input block\n", state);
    print(stdout, "\n\nInput key\n", key);
    encrypt(state, key, rounds, stdout);
    fprintf(stdout, "\nFinal result\n");
    for (int i = 0; i < 4; i++) {
      fprintf(stdout, "%02x", state[i]);
      fprintf(stdout, "%02x", state[i+4]);
      fprintf(stdout, "%02x", state[i+8]);
      fprintf(stdout, "%02x", state[i+12]);
    }
    fprintf(stdout, "\n");
    if (j != iterations) {
      for (int i = 0; i < 16; i++) {
        state[i] ^= plaintext[i];
        key[i] = original[i];
      }
    }
  }

  
  return 0;
}

void print(FILE* out, char* step, unsigned char* item) {
  #ifdef DEBUG
  fprintf(out, "%s", step);
  writeOutput(out, item);
  #endif
}

void encrypt(unsigned char* state, unsigned char* key, int rounds, FILE* out) {
  // Initial round
  addRoundKey(state, key);
  print(out, "\n\nInitial AddRoundKey\n", state);
  for (int i = 0; i < rounds-1; i++) {
    fprintf(out, "\n\nRound %d\n", i + 1);
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
  fprintf(out, "\n\nFinal Round\n");
  subBytes(state);
  print(out, "SubBytes\n", state);
  shiftRows(state);
  print(out, "\n\nShiftRows\n", state);
  nextRoundKey(key, rounds-1);
  print(out, "\n\nRound Key\n", key);
  addRoundKey(state, key);
  print(out, "\n\nAddRoundKey\n", state);
}