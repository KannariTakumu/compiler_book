#!/bin/bash
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

assert 0 '0;'
assert 42 '42;'
assert 21 "5+20-4;"
assert 9 "2 + 8 - 1;"
assert 47 '5+6*7;'
assert 15 '5*(9-6);'
assert 4 '(3+5)/2;'

assert 0 '0==1;'
assert 1 '42==42;'
assert 1 '0!=1;'
assert 0 '42!=42;'

assert 1 '0<1;'
assert 0 '1<1;'
assert 0 '2<1;'
assert 1 '0<=1;'
assert 1 '1<=1;'
assert 0 '2<=1;'

assert 1 '1>0;'
assert 0 '1>1;'
assert 0 '1>2;'
assert 1 '1>=0;'
assert 1 '1>=1;'
assert 0 '1>=2;'

assert 14 'a = 3; b = 5 * 6 - 8; a + b / 2;'
assert 14 'a = 3; b = 5 * 6 - 8; c = a + b / 2; c;'
assert 40 'a=3;b=5;c=7;d=9;e=11;a+b*c-d+e;'
assert 3 'a=3; a;'
assert 5 'a=3; b=5; a=b; a;'

assert 14 'aa = 3; b = 5 * 6 - 8; aa + b / 2;'
assert 14 'abc = 3; b = 5 * 6 - 8; abc + b / 2;'
assert 6 'foo = 1; bar = 2 + 3; foo + bar;'

echo OK