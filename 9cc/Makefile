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

# 個別テストケース実行
test-arithmetic: 9cc
	./test.sh basic_arithmetic.txt

test-comparison: 9cc
	./test.sh comparison.txt

test-variables: 9cc
	./test.sh variables.txt

test-return: 9cc
	./test.sh return.txt

test-if: 9cc
	./test.sh if.txt

test-integration: 9cc
	./test.sh integration.txt

# ヘルプ表示
help:
	@echo "Available test targets:"
	@echo "  test            - Run all test cases"
	@echo "  test-arithmetic - Run basic arithmetic tests"
	@echo "  test-comparison - Run comparison operator tests"
	@echo "  test-variables  - Run variable and assignment tests"
	@echo "  test-return     - Run return statement tests"
	@echo "  test-if         - Run if statement tests"
	@echo "  test-integration- Run integration tests"

# 掃除
clean:
	rm -f 9cc *.o *~ tmp*

.PHONY: test test-arithmetic test-comparison test-variables test-return test-if test-integration help clean