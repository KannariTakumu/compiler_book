#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "error.h"

int main(int argc, char **argv)
{
    // 現在着目しているトークン
    Token *token;

    if (argc != 2)
    {
        error("引数の個数が正しくありません");
        return 1;
    }

    // トークナイズする
    token = tokenize(argv[1]);

    // アセンブリの前半部分を出力
    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");

    // 式の最初は数でなければならないので、それをチェックして
    // 最初のmov命令を出力
    printf("  mov rax, %d\n", expect_number(&token));

    // `+ <数>`あるいは`- <数>`というトークンの並びを消費しつつ
    // アセンブリを出力
    while (!at_eof(token))
    {
        if (consume('+', &token))
        {
            printf("  add rax, %d\n", expect_number(&token));
            continue;
        }

        expect('-', &token);

        printf("  sub rax, %d\n", expect_number(&token));
    }

    printf("  ret\n");
    return 0;
}