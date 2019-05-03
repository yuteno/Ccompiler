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


try 0 'main(){
		return 0;
	}'
try 21 'main(){
		return 5+20-4;
	}'
try 15 "main(){
		return 5*(9-6);
	}"
try 4 "main(){
	a = b = 2;
	return a+b;}"
try 10 "main(){
	a=5;
	b=2;
	return a*b;
}"
try 5 "main(){
	return 5;
	return 4;
}"
try 10 "main(){
	foo = 5;
	bar = 2;
	return foo * bar;
}"
try 10 "main(){
	ax1 = 5;
	ax2 = 2;
	return ax1 * ax2;
}"
try 10 "main(){
	ax1 = -5;
	ax2 = -2;
	return ax1 * ax2;
}"
try 2 "main(){
	ax1 = -5;
	ax2 = 7;
	return ax1 + ax2;
}"

try 0 "main(){
	ax1 = -5;
   	ax2 = 7;
   	return ax1 == ax2;
}"

try 1 "main(){
	ax1 = -5;
	ax2 = 7;
	return ax1 != ax2;
}"


try 0 "main(){
	ax1 = 7;
   	ax2 = 7;
   	return ax1 != ax2;
}"
try 1 "main(){
	ax1 = 7;
   	ax2 = 7;
   	return ax1 <= ax2;
}"
try 0 "main(){
	ax1 = 6;
	ax2 = 7;
	return ax1 >= ax2;
}"
#try_function_call func_sample.c "function foo OK" "foo();"
#try_function_call func_sample.c "function bar is called" "bar();"
try 9 "main(){
	a=1;
   	if(a)
	   	return 4 + 5;
	x = 10;
	return x;
}"
try 10 "main(){
	a=0;
   	if(a)
	   	return 5;
	x = 10;
	return x;
}"
try 32 "main(){
	x = 1;
	b = 1;
   	a = 5;
   	while(a != 0) {
		a = a - 1;
	   	b = b * 2;
	   	x = x + 2;
	}
	return b;
}"
try 10 "main(){
	a = 5;
	for ( i = 0; i < 5; i = i + 1)
	   	a = a + 1;
	return a;
}"
try 10 "main(){
	a = 5;
   	for ( i = 0; i < 5; i = i + 1)  {
		a = a + 1;
	}
	return a;
}"
try_function_call func_sample.c "function buzz is called, result: 7" "main(){buzz(3, 4);}"
try_function_call func_sample.c "function buzz3 is called, result: 13" "buzz3(3, 4, 6);"
try_function_call func_sample.c "function buzz3 is called, result: 13" "buzz3(3,4,6);"
try_function_call func_sample.c "function buzz3 is called, result: 13" "xx = 3; yy = 4; zz = 6;  buzz3(xx, yy, zz);"
try_function_call func_sample.c "function buzz is called, result: 7" "x = 3; y = 4; buzz(x, 4);"
try 10 "a = 5; for ( i = 0; i < 5; i = i + 1)  {a = a + 1;} return a;"


echo OK
