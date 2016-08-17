# makefile

CC=clang

run: jess
	./jess text.txt

jess: jess.o
