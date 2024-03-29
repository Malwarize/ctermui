CC=gcc
CFLAGS=-g 
.PHONY: all run clean 

run : all
	./build/bin/ctermui

all: ctermui_layout.o ctermui_component.o ctermui_screen.o ctermui_events.o ctermui_pencil.o main.o
	@$(CC) $(CFLAGS) build/ctermui_layout.o build/ctermui_component.o build/ctermui_screen.o build/ctermui_events.o build/ctermui_pencil.o build/main.o -lm -o build/bin/ctermui

main.o : 
	@$(CC) $(CFLAGS) -c main.c -o build/main.o 

ctermui_layout.o:
	@$(CC) $(CFLAGS) -c ctermui/ctermui_layout.c -o build/ctermui_layout.o

ctermui_component.o:
	@$(CC) $(CFLAGS) -c ctermui/ctermui_component.c -o build/ctermui_component.o

ctermui_screen.o:
	@$(CC) $(CFLAGS) -c ctermui/ctermui_screen.c  -o build/ctermui_screen.o 

ctermui_events.o:
	@$(CC) $(CFLAGS) -c ctermui/ctermui_events.c -o build/ctermui_events.o

ctermui_pencil.o:
	@$(CC) $(CFLAGS) -c ctermui/ctermui_pencil.c -o build/ctermui_pencil.o
clean:
	@rm  -rf build/*.o build/ctermui 

