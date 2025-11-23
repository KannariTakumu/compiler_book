#include <stdio.h>
#include "generator.h"
#include "node.h"
#include "error.h"

int labelseq = 0;

void gen_lval(Node *node)
{
  if (node->kind != ND_LVAR)
    error("代入の左辺値が変数ではありません");

  printf("  mov rax, rbp\n");
  printf("  sub rax, %d\n", node->offset);
  printf("  push rax\n");
}

void gen(Node *node)
{
  if (node->kind == ND_RETURN)
  {
    gen(node->lhs);
    printf("  pop rax\n");
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
    return;
  }

  if (node->kind == ND_IF)
  {
    int current_label = labelseq++;
    gen(node->lhs); // 条件式の生成
    printf("  pop rax\n");
    printf("  cmp rax, 0\n");

    // elseがあるかチェック
    if (node->rhs && node->rhs->kind == ND_ELSE)
    {
      // if-else文の場合
      printf("  je .Lelse%d\n", current_label);
      gen(node->rhs->lhs);
      printf("  jmp .Lend%d\n", current_label);
      printf(".Lelse%d:\n", current_label);
      gen(node->rhs->rhs);
      printf(".Lend%d:\n", current_label);
    }
    else
    {
      // elseのないif文の場合
      printf("  je .Lend%d\n", current_label);
      gen(node->rhs);
      printf(".Lend%d:\n", current_label);
    }
    return;
  }

  if (node->kind == ND_WHILE)
  {
    int current_label = labelseq++;
    printf(".Lbegin%d:\n", current_label);
    gen(node->lhs); // 条件式の生成
    printf("  pop rax\n");
    printf("  cmp rax, 0\n");
    printf("  je .Lend%d\n", current_label);
    gen(node->rhs);
    printf("  jmp .Lbegin%d\n", current_label);
    printf(".Lend%d:\n", current_label);
    return;
  }

  if (node->kind == ND_FOR)
  {
    int current_label = labelseq++;

    // 初期化文
    if (node->for_init)
    {
      gen(node->for_init);
      printf("  pop rax\n"); // 初期化文の結果を破棄
    }

    printf(".Lbegin%d:\n", current_label);

    // 条件式
    if (node->for_cond)
    {
      gen(node->for_cond);
      printf("  pop rax\n");
      printf("  cmp rax, 0\n");
      printf("  je .Lend%d\n", current_label);
    }

    // for文本体
    gen(node->for_body);

    if (node->for_update)
    {
      gen(node->for_update);
      printf("  pop rax\n"); // 更新文の結果を破棄
    }

    printf("  jmp .Lbegin%d\n", current_label);
    printf(".Lend%d:\n", current_label);
    return;
  }

  switch (node->kind)
  {
  case ND_NUM:
    printf("  push %d\n", node->val);
    return;
  case ND_LVAR:
    gen_lval(node);
    printf("  pop rax\n");
    printf("  mov rax, [rax]\n");
    printf("  push rax\n");
    return;
  case ND_ASSIGN:
    gen_lval(node->lhs);
    gen(node->rhs);

    printf("  pop rdi\n");
    printf("  pop rax\n");
    printf("  mov [rax], rdi\n");
    printf("  push rdi\n");
    return;
  }

  gen(node->lhs);
  gen(node->rhs);

  printf("  pop rdi\n");
  printf("  pop rax\n");

  switch (node->kind)
  {
  case ND_ADD:
    printf("  add rax, rdi\n");
    break;
  case ND_SUB:
    printf("  sub rax, rdi\n");
    break;
  case ND_MUL:
    printf("  imul rax, rdi\n");
    break;
  case ND_DIV:
    printf("  cqo\n");
    printf("  idiv rdi\n");
    break;
  case ND_EQ:
    printf("  cmp rax, rdi\n");
    printf("  sete al\n");
    printf("  movzb rax, al\n");
    break;
  case ND_NE:
    printf("  cmp rax, rdi\n");
    printf("  setne al\n");
    printf("  movzb rax, al\n");
    break;
  case ND_LT:
    printf("  cmp rax, rdi\n");
    printf("  setl al\n");
    printf("  movzb rax, al\n");
    break;
  case ND_LE:
    printf("  cmp rax, rdi\n");
    printf("  setle al\n");
    printf("  movzb rax, al\n");
    break;
  }

  printf("  push rax\n");
}