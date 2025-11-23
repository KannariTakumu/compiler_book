#pragma once

#include <stdbool.h>
#include "tokenized_str.h"

// トークンの種類
typedef enum
{
    TK_RESERVED, // 記号
    TK_IDENT,    // 識別子
    TK_NUM,      // 整数トークン
    TK_RETURN,   // returnキーワード
    TK_IF,       // ifキーワード
    TK_ELSE,     // elseキーワード
    TK_EOF,      // 入力の終わりを表すトークン
} TokenKind;

typedef struct Token Token;
typedef struct TokenizedStr TokenizedStr;

// トークン型
struct Token
{
    TokenKind kind;      // トークンの型
    Token *next;         // 次の入力トークン
    int val;             // kindがTK_NUMの場合、その数値
    char *str;           // トークン文字列
    TokenizedStr *owner; // トークンが属している文字列
    int len;
};

Token *new_token(TokenKind kind, Token *cur, char *str, TokenizedStr *ts, int len);
TokenizedStr *tokenize(char *p);
bool consume(char *op, Token **token);
Token *consume_return(Token **token);
Token *consume_ident(Token **token);
void expect(char *op, Token **token);
int expect_number(Token **token);
bool at_eof(Token *token);