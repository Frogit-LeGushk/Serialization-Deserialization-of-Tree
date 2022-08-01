
target: main.cpp
	g++ -Wall -Werror -std=gnu++14 main.cpp && ./a.out

clear:
	-rm *.txt a.out 2>/dev/null
