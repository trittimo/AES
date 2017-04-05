#include <stdio.h>
#include "aes.h"
#include "util.h"

#define DEBUG

int main(int argc, char* argv[]) {
  unsigned char state[16] = {0x32, 0x43, 0xf6, 0xa8,
                             0x88, 0x5a, 0x30, 0x8d,
                             0x31, 0x31, 0x98, 0xa2,
                             0xe0, 0x37, 0x07, 0x34};
  unsigned char key[16] = {0x2b, 0x7e, 0x15, 0x16, 
                           0x28, 0xae, 0xd2, 0xa6,
                           0xab, 0xf7, 0x15, 0x88,
                           0x09, 0xcf, 0x4f, 0x3c};
  encrypt(state, key, stdout);
  return 0;
}

void print(FILE* out, unsigned char* state, unsigned char* key) {
  fprintf(out, " Input key\n");
  writeOutput(out, key);
  fprintf(out, "\n\nInput block\n");
  writeOutput(out, state);
  fprintf(out, "\n");
}

void encrypt(unsigned char* state, unsigned char* key, FILE* out) {
  // Initial round
  #ifdef DEBUG
  print(out, state, key);
  #endif
}