build: *.c
	gcc -o invertedindex *.c -Wall --pedantic
run:
	./invertedindex
clean:
	rm invertedindex
