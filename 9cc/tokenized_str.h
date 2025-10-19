#pragma once

#include "token.h"

typedef struct Token Token;
typedef struct TokenizedStr TokenizedStr;

// Tokenizeされた文字列の型
struct TokenizedStr{
    Token *head; // 先頭のトークンのアドレス
    char *value; // Tokenizeされた文字列
};