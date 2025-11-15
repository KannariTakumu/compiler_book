CFLAGS = -std=c11 -g -static
CC     = gcc

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

# 最終成果物
9cc: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# 個別コンパイル規則
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# テスト実行
test: 9cc
	./test.sh

# 掃除
clean:
	rm -f 9cc *.o *~ tmp*

.PHONY: test clean