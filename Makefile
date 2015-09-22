all: melo.o

melo.o: melo.c
	/usr/bin/gcc -o melo.o -c melo.c

melo.c: melo.ac
	python airy.py