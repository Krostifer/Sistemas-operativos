CC = gcc
CFLAGS = -Wall -lm -lniceprint
out = lab2
source = lab2.c functions.c bomb.c
objects = $(source:.c=.o)
headers = bomb.h functions.h estructuras.h


$(out): $(objects)
	$(CC) -o $@ $^ $(CFLAGS) -L .

$(objects):
	$(CC) -c $*.c

clean:
	rm -f $(objects) $(out)

run:
	./lab2 -N 35 -p 5 -c 6 -i test1_35.txt -o out1.txt -D
	./lab2 -N 35 -p 5 -c 8 -i test2_35.txt -o out2.txt -D
	./lab2 -N 30000 -p 6 -c 20001 -i test3_30000.txt -o out3.txt
	./lab2 -N 1000000 -p 5 -c 5001 -i test4_1M.txt -o out4.txt 
	