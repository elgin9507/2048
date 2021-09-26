all: 2048

2048: main.c board.c board.h render.c render.h
	cc main.c board.c render.c -lncurses -o 2048

clean:
	rm ./2048
