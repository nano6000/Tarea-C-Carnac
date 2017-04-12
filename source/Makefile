CC = gcc
LIBS = -lallegro
OBJS = Window.c board.c
OUT = carnac

compile: $(OBJS)
	$(CC) -o $(OUT) $(OBJS) $(LIBS)

debug: $(OBJS)
	$(CC) -g -o $(OUT) $(OBJS) $(LIBS)
	gdb $(OUT)

run:
	./$(OUT)

clean:
	rm $(OUT)

.PHONY: run debug compile

all: clean compile run