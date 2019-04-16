9cc: 9cc.c node.c token.c util.c

test: 9cc
	./test.sh

clean:
	rm -f 9cc *.o *~ tmp*


