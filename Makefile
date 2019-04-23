CFLAGS=-Wall -std=c11
SRCS=$(wildcard *.c)
ONJS=$(SRCS:.c=.o)


#9cc: 9cc.c node.c token.c util.c container.c
9cc: $(OBJS)
	$(CC) -o 9cc $(OBJS) $(LDFLAGS)


$(OBJS): node.h token.h util.h container.h

test: 9cc
	./9cc -test
	./test.sh

clean:
	rm -f 9cc *.o *~ tmp*


