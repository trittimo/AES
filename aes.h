#ifndef AES_H
#define AES_H

void print(FILE* out, char* step, unsigned char* input);
void encrypt(unsigned char* state, unsigned char* key, int rounds, FILE* out);

#endif