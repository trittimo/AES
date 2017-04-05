#ifndef UTIL_H
#define UTIL_H
#include <stdio.h>
#include "constants.h"

/* ============== CONSTANTS ============== */
#define CHUNK_SIZE 16

/* ============== FUNCTIONS ============== */
void writeOutput(FILE* stream, unsigned char* state) {
  for (int i = 0; i < CHUNK_SIZE; i++) {
    if (i != 0 && i % 4 == 0) {
      fprintf(stream, "\n");
    }
    fprintf(stream, "%02x ", state[i]);
  }
}

/* Rotations always happen on 4 byte chunks, so there is no need to specify a length */
void rotateRow(unsigned char* state) {
  int temp = state[0];
  state[0] = state[1];
  state[1] = state[2];
  state[2] = state[3];
  state[3] = temp;
}

/* Rotataions are again always on 4 byte chunks */
void rotateColumn(unsigned char* arr) {
  unsigned char temp = arr[0];
  arr[0] = arr[4];
  arr[4] = arr[8];
  arr[8] = arr[12];
  arr[12] = temp;
}

/* Substitute the bytes in a column, rather than row order */
void subColumnBytes(unsigned char* state) {
  for (int i = 0; i < CHUNK_SIZE; i+=4) {
    int left = state[i] >> 4;
    int right = state[i] & 0x0F;
    state[i] = sbox[right + (left * 16)];
  }
}

void nextRoundKey(unsigned char* key, int round) {
  unsigned char newKey[CHUNK_SIZE];
  for (int i = 0; i < CHUNK_SIZE; i++) {
    newKey[i] = key[i];
  }
  newKey[0] = key[3];
  newKey[4] = key[7];
  newKey[8] = key[11];
  newKey[12] = key[15];
  rotateColumn(newKey);
  subColumnBytes(newKey);
  newKey[0] ^= rcon[round+1];
  newKey[0] ^= key[0];
  newKey[4] ^= key[4];
  newKey[8] ^= key[8];
  newKey[12] ^= key[12];
  for (int i = 1; i < 4; i++) {
    newKey[i] = newKey[i-1] ^ key[i];
    newKey[i+4] = newKey[i+3] ^ key[i+4];
    newKey[i+8] = newKey[i+7] ^ key[i+8];
    newKey[i+12] = newKey[i+11] ^ key[i+12];
  }
  for (int i = 0; i < CHUNK_SIZE; i++) {
    key[i] = newKey[i];
  }
}

/* Substitute the bytes in the array for the appropriate replacement from the sbox */
void subBytes(unsigned char* state) {
  for (int i = 0; i < CHUNK_SIZE; i++) {
    int left = state[i] >> 4;
    int right = state[i] & 0x0F;
    state[i] = sbox[right + (left * 16)];
  }
}

/* Shifts the rows in the state according to the shiftRows step */
void shiftRows(unsigned char* state) {
  rotateRow(state + 4);
  rotateRow(state + 8);
  rotateRow(state + 8);
  rotateRow(state + 12);
  rotateRow(state + 12);
  rotateRow(state + 12);
}

/* Mixes a single column */
void mixColumn(unsigned char* column) {
  unsigned char copy[4];
  for (int i = 0; i < 4; i++) {
    copy[i] = column[i];
  }
  column[0] = galoisBy2[copy[0]] ^ galoisBy3[copy[1]] ^ copy[2] ^ copy[3];
  column[1] = copy[0] ^ galoisBy2[copy[1]] ^ galoisBy3[copy[2]] ^ copy[3];
  column[2] = copy[0] ^ copy[1] ^ galoisBy2[copy[2]] ^ galoisBy3[copy[3]];
  column[3] = galoisBy3[copy[0]] ^ copy[1] ^ copy[2] ^ galoisBy2[copy[3]];
}

/* Mixes the columns */
void mixColumns(unsigned char* state) {
  unsigned char column[4];
  for (int i = 0; i < 4; i++) {
    column[0] = state[i]; column[1] = state[i + 4];
    column[2] = state[i + 8]; column[3] = state[i + 12];
    mixColumn(column);
    state[i] = column[0];
    state[i + 4] = column[1];
    state[i + 8] = column[2];
    state[i + 12] = column[3];
  }
}

/* Adds the round key to the state */
void addRoundKey(unsigned char* state, unsigned char* key) {
  for (int i = 0; i < CHUNK_SIZE; i++) {
    state[i] = state[i] ^ key[i];
  }
}

#endif