aes: aes.c aes.h
	gcc aes.c -o3 -Werror -Wpedantic -std=c99 -o aes

clean:
	rm -rf *.o aes