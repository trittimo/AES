aes: aes.c aes.h
	gcc aes.c -o3 -Werror -Wpedantic -std=c99 -o aes

test: aes
	./aes aesinput.txt aesoutput_mine.txt

clean:
	rm -rf *.o *_mine.txt aes