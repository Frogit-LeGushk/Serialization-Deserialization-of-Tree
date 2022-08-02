CC=g++
CFLAGS=-Wall -Werror -std=gnu++14
LFLAGS=
OBJ=main.cpp
INC=Errors.hpp Buffer.hpp Core.hpp Types.hpp Tree.hpp
GARBAGE=*.txt a.out exe
ALERTS_TO_TRASH=2>/dev/null


server: $(OBJ) $(INC)
	$(CC) $(CFLAGS) $(LFLAGS) $(OBJ) -o exe && ./exe s

client: $(OBJ) $(INC)
	$(CC) $(CFLAGS) $(LFLAGS) $(OBJ) -o exe && ./exe c

clear:
	-rm $(GARBAGE) $(ALERTS_TO_TRASH)
