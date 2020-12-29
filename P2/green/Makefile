$(CC) = gcc

CFLAGS=-g -lrt -I.
CFLAGS2= -DPOSIX -pthread -Wno-implicit-function-declaration $(CFLAGS)
DEPS=green.h
OBJ = queue.o green.o green_test.o

all: $(OBJ) green_test posix_test


%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

green_test:	$(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) 


posix_test: green_test.c posix.h
	$(CC) -o $@ $< $(CFLAGS2)

.PHONY: clean

clean:	
	rm *.o green_test posix_test peda*
