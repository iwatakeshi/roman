all:
	g++ -g -std=c++17 -Wno-conversion-null main.cpp

memcheck:
	valgrind  --undef-value-errors=no ./a.out