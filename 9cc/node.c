#include <stdlib.h>

#include "node.h"
#include "token.h"

Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = kind;
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

Node *new_node_num(int val) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_NUM;
  node->val = val;
  return node;
}

Node *expr(Token **token) {
  Node *node = mul(token);

  for (;;) {
    if (consume('+', token))
      node = new_node(ND_ADD, node, mul(token));
    else if (consume('-', token))
      node = new_node(ND_SUB, node, mul(token));
    else
      return node;
  }
}

Node *mul(Token **token) {
  Node *node = primary(token);

  for (;;) {
    if (consume('*', token))
      node = new_node(ND_MUL, node, primary(token));
    else if (consume('/', token))
      node = new_node(ND_DIV, node, primary(token));
    else
      return node;
  }
}

Node *primary(Token **token) {
  // 次のトークンが"("なら、"(" expr ")"のはず
  if (consume('(', token)) {
    Node *node = expr(token);
    expect(')', token);
    return node;
  }

  // そうでなければ数値のはず
  return new_node_num(expect_number(token));
}