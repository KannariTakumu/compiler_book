#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "token.h"
#include "tokenized_str.h"
#include "error.h"

// 前方宣言
bool is_reserved_char(char c);
bool is_reserved_str(char *str);
bool is_ident_char(char c);
bool is_expected_reserved_token(char *op, Token *token);

// 新しいトークンを作成してcurに繋げる
Token *new_token(TokenKind kind, Token *cur, char *str, TokenizedStr *ts, int len)
{
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    tok->owner = ts;
    tok->len = len;
    cur->next = tok;
    return tok;
}

// 入力文字列pをトークナイズしてそれを返す
TokenizedStr *tokenize(char *p)
{
    Token head;
    head.next = NULL;
    Token *cur = &head;
    TokenizedStr *ts = calloc(1, sizeof(TokenizedStr));
    ts->value = p;

    while (*p)
    {
        // 空白文字をスキップ
        if (isspace(*p))
        {
            p++;
            continue;
        }

        if (is_reserved_str(p)) {
            cur = new_token(TK_RESERVED, cur, p, ts, 2);
            p += 2;
            continue;
        }

        if (is_reserved_char(*p))
        {
            cur = new_token(TK_RESERVED, cur, p, ts, 1);
            p += 1;
            continue;
        }

        if (is_ident_char(*p)){
            cur = new_token(TK_IDENT, cur, p, ts, 1);
            p += 1;
            continue;
        }

        if (isdigit(*p))
        {
            cur = new_token(TK_NUM, cur, p, ts, 0);
            cur->val = strtol(p, &p, 10);
            continue;
        }

        error_at(p, p, "トークナイズできません");
    }

    new_token(TK_EOF, cur, p, ts, 0);
    ts->head = head.next;
    return ts;
}

bool is_reserved_char(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == '<' || c == '>' || c == ';' || c == '=';
}

bool is_reserved_str(char *str)
{
    return memcmp(str, "==", 2) == 0 || memcmp(str, "!=", 2) == 0 || memcmp(str, "<=", 2) == 0 || memcmp(str, ">=", 2) == 0;
}

bool is_ident_char(char c)
{
    return 'a' <= c && c <= 'z';
}

// 次のトークンが期待している記号のときには、トークンを1つ読み進めて
// 真を返す。それ以外の場合には偽を返す。
bool consume(char *op, Token **token)
{
    if (!is_expected_reserved_token(op, *token))
    {
        return false;
    }

    *token = (*token)->next;
    return true;
}

// 次のトークンが識別子のときには、トークンを1つ読み進めて
// そのトークンを返す。それ以外の場合にはNULLを返す。
Token *consume_ident(Token **token)
{
    if ((*token)->kind != TK_IDENT)
    {
        return NULL;
    }

    Token *tok = *token;
    *token = (*token)->next;
    return tok;
}

// 次のトークンが期待している記号のときには、トークンを1つ読み進める。
// それ以外の場合にはエラーを報告する。
void expect(char *op, Token **token)
{
    if (!is_expected_reserved_token(op, *token))
    {
        error_at((*token)->str, (*token)->owner->value, "'%s'ではありません", op);
    }

    *token = (*token)->next;
}

// 次のトークンが数値の場合、トークンを1つ読み進めてその数値を返す。
// それ以外の場合にはエラーを報告する。
int expect_number(Token **token)
{
    if ((*token)->kind != TK_NUM)
    {
        error_at((*token)->str, (*token)->owner->value, "数ではありません");
    }

    int val = (*token)->val;
    *token = (*token)->next;
    return val;
}

bool is_expected_reserved_token(char *op, Token *token)
{
    // トークンの型が記号でない場合
    if (token->kind != TK_RESERVED)
    {
        return false;
    }

    // 演算子の長さが異なる場合
    if (strlen(op) != token->len)
    {
        return false;
    }

    // 文字列の内容が異なる場合
    if (memcmp(token->str, op, token->len) != 0)
    {
        return false;
    }

    return true;
}

bool at_eof(Token *token)
{
    return token->kind == TK_EOF;
}