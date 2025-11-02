#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "node.h"
#include "generator.h"
#include "error.h"

int main(int argc, char **argv)
{
    // 現在着目しているトークン
    Token *token;
    // tokenizeされたユーザー入力
    TokenizedStr *user_input;

    if (argc != 2)
    {
        error("引数の個数が正しくありません");
        return 1;
    }

    // トークナイズする
    user_input = tokenize(argv[1]);
    token = user_input->head;
    Node *node = expr(&token);

    // アセンブリの前半部分を出力
    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");

    /// 抽象構文木を下りながらコード生成
    gen(node);

    // スタックトップに式全体の値が残っているはずなので
    // それをRAXにロードして関数からの返り値とする
    printf("  pop rax\n");
    printf("  ret\n");
    return 0;
}