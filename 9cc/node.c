#include <stdlib.h>

#include "node.h"
#include "token.h"
#include "lvar.h"
#include "error.h"

Node *new_node(NodeKind kind, Node *lhs, Node *rhs)
{
  Node *node = calloc(1, sizeof(Node));
  node->kind = kind;
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

Node *new_node_num(int val)
{
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_NUM;
  node->val = val;
  return node;
}

Node *new_node_lvar(Token *token)
{
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_LVAR;
  LVar *lvar = find_lvar(token);
  if (lvar)
  {
    node->offset = lvar->offset;
  }
  else
  {
    node->offset = new_lvar(token)->offset;
  }
  return node;
}

Node *new_node_return(Token **token)
{
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_RETURN;
  node->lhs = expr(token);
  return node;
}

void program(Node **code, Token **token)
{
  int i = 0;
  while (!at_eof(*token))
    code[i++] = stmt(token);
  code[i] = NULL;
}

Node *expr(Token **token)
{
  return assign(token);
}

Node *assign(Token **token)
{
  Node *node = equality(token);
  if (consume("=", token))
    node = new_node(ND_ASSIGN, node, assign(token));
  return node;
}

Node *stmt(Token **token)
{

  Node *node;
  if (consume_return(token))
  {
    node = new_node_return(token);
  }
  else
  {
    node = expr(token);
  }

  if (!consume(";", token))
  {
    error_at((*token)->str, (*token)->owner->value, "';'ではありません");
  }
  return node;
}

Node *equality(Token **token)
{
  Node *node = relational(token);

  for (;;)
  {
    if (consume("==", token))
      node = new_node(ND_EQ, node, relational(token));
    else if (consume("!=", token))
      node = new_node(ND_NE, node, relational(token));
    else
      return node;
  }
}

Node *relational(Token **token)
{
  Node *node = add(token);

  for (;;)
  {
    if (consume("<", token))
      node = new_node(ND_LT, node, add(token));
    else if (consume("<=", token))
      node = new_node(ND_LE, node, add(token));
    else if (consume(">", token))
      node = new_node(ND_LT, add(token), node);
    else if (consume(">=", token))
      node = new_node(ND_LE, add(token), node);
    else
      return node;
  }
}

Node *add(Token **token)
{
  Node *node = mul(token);

  for (;;)
  {
    if (consume("+", token))
      node = new_node(ND_ADD, node, mul(token));
    else if (consume("-", token))
      node = new_node(ND_SUB, node, mul(token));
    else
      return node;
  }
}

Node *mul(Token **token)
{
  Node *node = primary(token);

  for (;;)
  {
    if (consume("*", token))
      node = new_node(ND_MUL, node, primary(token));
    else if (consume("/", token))
      node = new_node(ND_DIV, node, primary(token));
    else
      return node;
  }
}

Node *primary(Token **token)
{
  // 次のトークンが"("なら、"(" expr ")"のはず
  if (consume("(", token))
  {
    Node *node = expr(token);
    expect(")", token);
    return node;
  }

  // 次のトークンが識別子なら、変数のはず
  Token *tok = consume_ident(token);
  if (tok)
  {
    return new_node_lvar(tok);
  }

  // そうでなければ数値のはず
  return new_node_num(expect_number(token));
}