all:
	gcc main.c -o qview `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`
install:
	sudo cp ./qview /usr/bin/