CC=gcc
CFLAGS=-g 
.PHONY: all run clean 

run : all
	@./build/bin/ctermui

all: ctermui.o  main.o
	@$(CC) $(CFLAGS) build/ctermui.o build/main.o -o build/bin/ctermui
main.o : main.c
	@$(CC) $(CFLAGS) -c main.c -o build/main.o

ctermui.o:
	@$(CC) $(CFLAGS) -c ctermui/ctermui.c -o build/ctermui.o


clean:
	@rm  -rf build/*.o build/ctermui 

