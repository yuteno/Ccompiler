9cc: 9cc.c node.c token.c util.c container.c

test: 9cc
	./9cc -test
	./test.sh

clean:
	rm -f 9cc *.o *~ tmp*


