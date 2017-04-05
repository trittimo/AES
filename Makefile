aes: aes.c aes.h
	gcc aes.c -o3 -Werror -Wpedantic -std=c99 -o aes

debug: aes.c aes.h
	gcc aes.c -o3 -Werror -Wpedantic -std=c99 -o aes_debug -DDEBUG

run: aes
	./aes aesinput.txt aesoutput_mine1.txt
	./aes aesinput2.txt aesoutput_mine2.txt

test: debug
	./aes_debug aesinput.txt aesoutput_mine.txt

clean:
	rm -rf *.o *_mine*.txt aes aes_debug