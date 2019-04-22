#!/bin/bash
try(){
	expected="$1"
	input="$2"

	./9cc "$input" > tmp.s
	gcc -o tmp tmp.s
	./tmp
	actual="$?"

	if [ "$actual" = "$expected" ]; then
		echo "$input => $actual"
	else
		echo "$expected expected, but got $actual"
		exit 1
	fi
}

try 0 '0;'
try 42 '42;'
try 21 '5+20-4;'
try 41 " 12 + 34 -5;"
try 47 "5+6*7;"
try 15 "5*(9-6);"
try 4 "(3+5)/2;"
try 4 "a = b = 2;a+b;"
try 10 "a=5;b=2;a*b;"
try 100 "a=5;b=2;a * a * b * 2;"
try 5 "return 5; return 4;"
try 10 "a = 5; b = 2; return a * b;"
try 10 "foo = 5; bar = 2; return foo * bar;"
try 15 "foo = 10; bar = 5; return foo + bar;"
try 20 "foo = 40; bar = 2; return foo / bar;"
try 10 "ax1 = 5; ax2 = 2; return ax1 * ax2;"
try 10 "ax1 = -5; ax2 = -2; return ax1 * ax2;"
try 2 "ax1 = -5; ax2 = 7; return ax1 + ax2;"
echo OK
