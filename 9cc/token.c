#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "token.h"
#include "tokenized_str.h"
#include "error.h"

// 新しいトークンを作成してcurに繋げる
Token *new_token(TokenKind kind, Token *cur, char *str, TokenizedStr *ts)
{
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    tok->owner = ts;
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

        if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p == ')')
        {
            cur = new_token(TK_RESERVED, cur, p++, ts);
            continue;
        }

        if (isdigit(*p))
        {
            cur = new_token(TK_NUM, cur, p, ts);
            cur->val = strtol(p, &p, 10);
            continue;
        }

        error_at(p, p, "トークナイズできません");
    }

    new_token(TK_EOF, cur, p, ts);
    ts->head = head.next;
    return ts;
}

// 次のトークンが期待している記号のときには、トークンを1つ読み進めて
// 真を返す。それ以外の場合には偽を返す。
bool consume(char op, Token **token)
{
    if ((*token)->kind != TK_RESERVED || (*token)->str[0] != op)
    {
        return false;
    }

    *token = (*token)->next;
    return true;
}

// 次のトークンが期待している記号のときには、トークンを1つ読み進める。
// それ以外の場合にはエラーを報告する。
void expect(char op, Token **token)
{
    if ((*token)->kind != TK_RESERVED || (*token)->str[0] != op)
    {
        error("tst");
        error_at((*token)->str, (*token)->owner->value, "数ではありません");
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

bool at_eof(Token *token)
{
    return token->kind == TK_EOF;
}