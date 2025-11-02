CFLAGS = -std=c11 -g -static
CC     = gcc

# オブジェクトファイル一覧
OBJS = 9cc.o token.o error.o tokenized_str.o node.o generator.o

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