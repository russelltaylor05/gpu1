# General test makefile for multi-target test
# General definitions

CPP = gcc
CPPFLAGS = -Wall -O3

MAIN = main.o

default: main
     
main: $(MAIN) 
	$(CPP) $(MAIN) -o main

clean: 
	rm main *.o

test: clean main
	@main


%.o: %.c
	$(CPP) $(CPPFLAGS) -c $< -o $@


# Header file interdependencies

%.c: %.h
	touch $@

