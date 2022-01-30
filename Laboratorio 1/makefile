CC = gcc
CFLAGS = -Wall -lm -lniceprint
out = lab1
source = lab1.c functions.c
objects = $(source:.c=.o)
headers = functions.h estructuras.h


$(out): $(objects)
	$(CC) -o $@ $^ $(CFLAGS) -L .

$(objects):
	$(CC) -c $*.c

clean:
	rm -f $(objects) $(out)
run:
	./lab1

