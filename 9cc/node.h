#pragma once

#include "token.h"

// 抽象構文木のノードの種類
typedef enum
{
  ND_ADD,    // +
  ND_SUB,    // -
  ND_MUL,    // *
  ND_DIV,    // /
  ND_EQ,     // ==
  ND_NE,     // !=
  ND_LT,     // <
  ND_LE,     // <=
  ND_ASSIGN, // =
  ND_LVAR,   // ローカル変数
  ND_NUM,    // 整数
  ND_RETURN, // return文
  ND_IF,     // if文
  ND_ELSE,   // else文
  ND_WHILE,  // while文
  ND_FOR,    // for文
  ND_BLOCK,  // ブロック
  ND_FUNC    // 関数
} NodeKind;

typedef struct Node Node;

// 抽象構文木のノードの型
struct Node
{
  NodeKind kind; // ノードの型
  Node *lhs;     // 左辺
  Node *rhs;     // 右辺
  int val;       // kindがND_NUMの場合のみ使う
  int offset;    // kindがND_LVARの場合のみ使う。RBPからのオフセット

  // for文用のメンバー
  Node *for_init;   // for文の初期化文 (A)
  Node *for_cond;   // for文の条件式 (B)
  Node *for_update; // for文の更新文 (C)
  Node *for_body;   // for文の本体 (D)

  // ブロック文用のメンバー
  Node **stmts;   // ブロック内の文の配列
  int stmt_count; // 文の数

  // 関数用のメンバー
  char *func_name; // 関数名
  int func_name_len; // 関数名の長さ
};

// ノード生成関数
Node *new_node(NodeKind kind, Node *lhs, Node *rhs);
Node *new_node_num(int val);
Node *new_node_lvar(Token *token);
Node *new_node_return(Token **token);

// 構文解析関数（演算子の優先順位順）
void program(Node **code, Token **token);
Node *stmt(Token **token);
Node *expr(Token **token);
Node *assign(Token **token);
Node *equality(Token **token);
Node *relational(Token **token);
Node *add(Token **token);
Node *mul(Token **token);
Node *primary(Token **token);