build: *.c
	gcc -o invertedindex invertedindex.c -Wall --pedantic
run:
	./invertedindex
clean:
	rm invertedindex
