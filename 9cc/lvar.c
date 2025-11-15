#include "lvar.h"
#include "token.h"
#include <stdlib.h>
#include <string.h>

// グローバルなローカル変数リストの実体
LVar *locals;

// 変数を名前で検索する。見つからなかった場合はNULLを返す。
LVar *find_lvar(Token *tok)
{
    for (LVar *var = locals; var; var = var->next)
        if (var->len == tok->len && !memcmp(tok->str, var->name, var->len))
            return var;
    return NULL;
}

// 新しい変数を作成する
LVar *new_lvar(Token *tok)
{
    LVar *lvar = calloc(1, sizeof(LVar));
    lvar->next = locals;
    lvar->name = tok->str;
    lvar->len = tok->len;
    if (locals)
        lvar->offset = locals->offset + 8;
    else
        lvar->offset = 8;
    locals = lvar;
    return lvar;
}