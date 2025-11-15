#!/bin/bash

# テストケースディレクトリ
TEST_DIR="tests"

# 実行するテストファイル一覧
TEST_FILES=(
  "basic_arithmetic.txt"
  "comparison.txt"
  "variables.txt"
  "return.txt"
  "integration.txt"
)

assert() {
  expected="$1"
  input="$2"

  ./9cc "$input" > tmp.s
  cc -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

# 単一のテストファイルを実行する関数
run_test_file() {
  local test_file="$1"
  local full_path="$TEST_DIR/$test_file"
  
  if [ ! -f "$full_path" ]; then
    echo "Warning: Test file $full_path not found, skipping..."
    return
  fi
  
  echo "Running tests from $test_file..."
  
  while read -r line || [ -n "$line" ]; do
    # コメント行と空行をスキップ
    if [[ "$line" =~ ^#.*$ ]] || [[ -z "$line" ]]; then
      continue
    fi
    
    # 最初のスペースで分割
    expected=$(echo "$line" | cut -d' ' -f1)
    input=$(echo "$line" | cut -d' ' -f2-)
    
    # テストを実行
    assert "$expected" "$input"
  done < "$full_path"
}

# 引数がある場合は特定のテストファイルのみ実行
if [ $# -eq 1 ]; then
  # 単一テストファイル実行
  test_file="$1"
  run_test_file "$test_file"
else
  # 全テストファイルを実行
  for test_file in "${TEST_FILES[@]}"; do
    run_test_file "$test_file"
  done
fi

echo OK