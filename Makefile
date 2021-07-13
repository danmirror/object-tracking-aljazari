
FILES = *.cpp
OBJ = demo
CFLAGS = `pkg-config --libs --cflags opencv`
CC=g++


all:
	$(CC) $(FILES) -o $(OBJ) $(CFLAGS)
clean:
	rm -f demo


