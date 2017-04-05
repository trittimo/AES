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

void scheduleKey(unsigned char* current) {

}

/* Substitute the bytes in the array for the appropriate replacement from the sbox */
void subBytes(unsigned char* state, int length) {
  for (int i = 0; i < length; i++) {
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