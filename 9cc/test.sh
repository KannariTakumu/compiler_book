#!/bin/bash

# テストケースディレクトリ
TEST_DIR="tests"

# 実行するテストファイル一覧を動的に取得
TEST_FILES=()
if [ -d "$TEST_DIR" ]; then
  while IFS= read -r -d '' file; do
    TEST_FILES+=("$(basename "$file")")
  done < <(find "$TEST_DIR" -name "*.txt" -type f -print0 | sort -z)
fi

assert() {
  expected="$1"
  input="$2"

  ./9cc "$input" > tmp.s
  
  # 関数テストの場合は外部関数とリンク
  if [ "$CURRENT_TEST_FILE" = "func.txt" ]; then
    cc -o tmp tmp.s test-func.o
  else
    cc -o tmp tmp.s
  fi
  
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
  
  # 現在のテストファイルを保存
  export CURRENT_TEST_FILE="$test_file"
  
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