build: *.c
	gcc -o invertedindex *.c
run:
	./invertedindex
clean:
	rm invertedindex
