CFLAGS=-std=c11 -g -static

# 最終成果物
9cc: 9cc.o token.o error.o
	$(CC) $(CFLAGS) -o $@ $^

test: 9cc
	./test.sh

clean:
	rm -f 9cc *.o *~ tmp*

.PHONY: test clean