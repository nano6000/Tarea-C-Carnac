CC = gcc
LIBS = -lallegro
OBJS = carnac-Bosques-Esteban.c board.c file.c
OUT = carnac

compile: $(OBJS)
	$(CC) -o $(OUT) $(OBJS) `pkg-config --cflags --libs gtk+-3.0` 


debug: $(OBJS)
	$(CC) -g -o $(OUT) $(OBJS) `pkg-config --cflags --libs gtk+-3.0`
	gdb $(OUT)

run:
	./$(OUT)

clean:
	rm $(OUT)

.PHONY: run debug compile

all: compile run