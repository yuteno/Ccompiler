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

try_function_call(){
	function_file="$1"
	expected="$2"
	input="$3"

	./9cc "$input" > tmp.s
	cc -c -o func_temp.o ${function_file}
	as -o asemble_temp.o tmp.s
	gcc -v -o 9cc_func_call asemble_temp.o func_temp.o &> /dev/null


	actual=`./9cc_func_call`

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
try 0 "ax1 = -5; ax2 = 7; return ax1 == ax2;"
try 1 "ax1 = -5; ax2 = 7; return ax1 != ax2;"
try 0 "ax1 = 7; ax2 = 7; return ax1 != ax2;"
try 1 "ax1 = 7; ax2 = 7; return ax1 == ax2;"
try 0 "ax1 = 7; ax2 = 6; return ax1 < ax2;"
try 1 "ax1 = 7; ax2 = 7; return ax1 <= ax2;"
try 1 "ax1 = 7; ax2 = 6; return ax1 > ax2;"
try 0 "ax1 = 6; ax2 = 7; return ax1 >= ax2;"
try_function_call func_sample.c "function foo OK" "foo();"
try_function_call func_sample.c "function bar is called" "bar();"
try 9 "a=1; if(a) return 4 + 5; x = 10; return x;"
try 10 "a=0; if(a) return 5; x = 10; return x;"
try 5 "a=5; if(a >= 4) return 5; x = 10; return x;"
try 0 "a = 5; while(a != 0) a = a - 1; return a;"
try 10 "a = 5; for ( i = 0; i < 5; i = i + 1)  a = a + 1; return a;"
echo OK
