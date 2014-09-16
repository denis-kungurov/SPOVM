OBJ = main.c
CFLAGS = -Wall -std=c99

all: bin

bin: $(OBJ)
	gcc $(CFLAGS) $(OBJ) -o bin -lncurses

%.o : %.c
	gcc $(CFLAGS) -c $<

clean:
	rm bin $(OBJ)
