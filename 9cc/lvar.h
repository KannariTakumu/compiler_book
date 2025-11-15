#pragma once
#include "token.h"

typedef struct LVar LVar;

// ローカル変数の型
struct LVar
{
    LVar *next; // 次の変数かNULL
    char *name; // 変数の名前
    int len;    // 名前の長さ
    int offset; // RBPからのオフセット
};

// ローカル変数のリスト
extern LVar *locals;

// ローカル変数を名前で検索する
LVar *find_lvar(Token *tok);

// 新しいローカル変数を作成してリストに追加
LVar *new_lvar(Token *tok);
