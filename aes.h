#ifndef AES_H
#define AES_H

void print(FILE* out, char* step, unsigned char* input);
void encrypt(unsigned char* state, unsigned char* key, int rounds, FILE* out);
void getByteArray(FILE* file, int length, unsigned char* out);
void readColumnOrder(unsigned char* dest, unsigned char* in);

#endif