CC=gcc
CFLAGS=-g 
.PHONY: all run clean 

run : all
	@./build/bin/ctermui

all: ctermui_widget.o ctermui_component.o ctermui_screen.o main.o
	@$(CC) $(CFLAGS) build/ctermui_widget.o build/ctermui_component.o build/ctermui_screen.o build/main.o -o build/bin/ctermui

main.o : 
	@$(CC) $(CFLAGS) -c main.c -o build/main.o

ctermui_widget.o:
	@$(CC) $(CFLAGS) -c ctermui/ctermui_widget.c -o build/ctermui_widget.o

ctermui_component.o:
	@$(CC) $(CFLAGS) -c ctermui/ctermui_component.c -o build/ctermui_component.o

# ctermui_pencil.o:
# 	@$(CC) $(CFLAGS) -c ctermui/ctermui_pencil.c -o build/ctermui_pencil.o

ctermui_screen.o:
	@$(CC) $(CFLAGS) -c ctermui/ctermui_screen.c -o build/ctermui_screen.o
clean:
	@rm  -rf build/*.o build/ctermui 

