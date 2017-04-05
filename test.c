#include <stdio.h>
#include "util.h"

int main() {
  unsigned char key[16] = {0x2b, 0x28, 0xab, 0x09,
                           0x7e, 0xae, 0xf7, 0xcf,
                           0x15, 0xd2, 0x15, 0x4f,
                           0x16, 0xa6, 0x88, 0x3c};
  nextRoundKey(key, 0);
  writeOutput(stdout, key);
  printf("\n");
}