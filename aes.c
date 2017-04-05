/*
AES encryption implementation by Michael Trittin
*/

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
  byte key[MAX_CHUNK_SIZE];
  byte input[MAX_CHUNK_SIZE];

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

  // Print out the input information to the output file
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

  // Do the encryption
  encrypt(iterations, rounds, key, input);

  // Print out the result to the output file
  fprintf(out, "****** END ENCRYPTION ******\nFinal block: ");
  for (int i = 0; i < MAX_CHUNK_SIZE; i++) {
    fprintf(out, "%x", output[i]);
  }
  printf("%x", getSubByte(0x00));
  fclose(out);
  return 0;
}


void encrypt(int iterations, int rounds, int* key, int* input, int* output) {
  // Key expansion
  int expandedKey[EXPANDED_KEY_SIZE];
  keyExpansion(key, expandedKey);

  // Initial round
  addRoundKey(key, input);

  // Rounds
  for (int i = 0; i < rounds; i++) {
    subBytes();
    shiftRows();
    mixColumns();
    addRoundKey();
  }

  // Final round
  subBytes();
  shiftRows();
  addRoundKey();
}

void keyExpansion(int* key, int* expandedKey) {
  int iteration = 1;
  // The first 16 bytes of our expanded key is just the key
  for (int i = 0; i < MAX_CHUNK_SIZE; i++) {
    expandedKey[i] = key[i];
  }
  for (int i = 16; i < EXPANDED_KEY_SIZE; i+= 16) {
    expandedKey[i] = expandedKey[i-4];
    expandedKey[i+1] = expandedKey[i-3];
    expandedKey[i+2] = expandedKey[i-2];
    expandedKey[i+3] = expandedKey[i-1];
    
    keySchedule(current 4 bytes, rcon iteration);

    // rotate 1 byte to the left
    // s-box each part
    // xor first byte with rcon[iteration]

    iteration++;
    expandedKey[i] = expandedKey[i] ^ expandedKey[i-4];
    expandedKey[i+1] = expandedKey[i+1] ^ expandedKey[i-3];
    expandedKey[i+2] = expandedKey[i+2] ^ expandedKey[i-2];
    expandedKey[i+3] = expandedKey[i+3] ^ expandedKey[i-1];


    // TODO: unroll
    for (int k = i+4; k < i+16; k+=4) {
      expandedKey[k] = expandedKey[k] ^ expandedKey[k-4];
      expandedKey[k+1] = expandedKey[k+1] ^ expandedKey[k-3];
      expandedKey[k+2] = expandedKey[k+2] ^ expandedKey[k-2];
      expandedKey[k+3] = expandedKey[k+3] ^ expandedKey[k-1];
    }
  }
}

void addRoundKey(int* key, int* input) {
  for (int i = 0; i < MAX_CHUNK_SIZE; i++) {
    input[i] = input[i] ^ key[i];
  }
}

void subBytes(int* input) {
  for (int i = 0; i < MAX_CHUNK_SIZE; i++) {
    input[i] = getSubByte(input[i]);
  }
}

void shiftRows(int* input) {
  int i0;
  int s0 = input[ROW_SIZE-1];
}

int getSubByte(int byte) {
  int left = byte >> 4;
  int right = byte & 0x0F;
  return sbox[right + (left * 16)];
}

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