aes: aes.c aes.h
	gcc aes.c -o3 -Werror -Wpedantic -std=c99 -o aes

debug: aes.c aes.h
	gcc aes.c -o3 -Werror -Wpedantic -std=c99 -o aes_debug -DDEBUG

test: debug
	./aes_debug aesinput.txt aesoutput_mine.txt

clean:
	rm -rf *.o *_mine.txt aes aes_debug