TARGET  = tinyFsDemo
CC       = gcc
LDFLAGS  = -lm
SOURCES  = $(wildcard *.c)
INCLUDES = $(wildcard *.h)
OBJECTS  = $(SOURCES:.c=.o)

all:
	gcc -c tinyFsDemo.c libTinyFS.c tinyFS.c libDisk.c
	gcc -o tinyFsDemo -lm tinyFsDemo.o libTinyFS.o tinyFS.o libDisk.o

clean:
	rm -f  tinyFsDemo -lm tinyFsDemo.o libTinyFS.o tinyFS.o libDisk.o
