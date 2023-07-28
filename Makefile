all: encrypt decrypt

encrypt: encrypt.o aes.o io.o field.o
	gcc encrypt.o aes.o io.o field.o -o encrypt

decrypt: decrypt.o aes.o io.o field.o
	gcc decrypt.o aes.o io.o field.o -o decrypt

fieldTest: fieldTest.o field.o
	gcc fieldTest.o field.o -o fieldTest

aesTest: aesTest.o aes.o field.o
	gcc aesTest.o aes.o field.o -o aesTest

encrypt.o: encrypt.c aes.h io.h field.h
	gcc -Wall -std=c99 -c -g encrypt.c

decrypt.o: decrypt.c aes.h io.h field.h
	gcc -Wall -std=c99 -c -g decrypt.c

fieldTest.o: fieldTest.c field.h
	gcc -Wall -std=c99 -c -g fieldTest.c

aesTest.o: aesTest.c aes.h field.h
	gcc -Wall -std=c99 -c -g aesTest.c

aes.o: aes.c aes.h field.h
	gcc -Wall -std=c99 -c -g aes.c

io.o: io.c io.h field.h
	gcc -Wall -std=c99 -c -g io.c

field.o: field.c field.h
	gcc -Wall -std=c99 -c -g field.c

clean:
	rm -f *.o
	rm -f encrypt
	rm -f decrypt
	rm -f fieldTest
	rm -f aesTest
	rm -f stderr.txt
	rm -f output.txt