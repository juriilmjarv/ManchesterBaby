# This is a comment

CC = g++ -std=c++11
#-std=c++11
CFLAGS = -Wall -pedantic -Wextra

Simulator: Simulator.cpp
	${CC} -o Simulator ${CFLAGS} Simulator.cpp baby.h

Assembler: Assembler.cpp
	
	${CC} -o Assembler ${CFLAGS} Assembler.cpp

run:
	./Assembler

clean:
	rm -f Simulator
	rm -f Assembler
