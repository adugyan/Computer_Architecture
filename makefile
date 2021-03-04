#
# PROGRAM: Assign5
# AUTHOR:  Kofi Adu-Gyan
#

CXX = g++
CPPFLAGS = -g -ansi -pedantic -Wall -Werror -std=c++14

main: main.cpp rv32i.cpp memory.cpp registerfile.cpp hex.cpp
	$(CXX) $(CPPFLAGS) -c -o main.o main.cpp
	$(CXX) $(CPPFLAGS) -c -o rv32i.o rv32i.cpp 
	$(CXX) $(CPPFLAGS) -c -o memory.o memory.cpp
	$(CXX) $(CPPFLAGS) -c -o registerfile.o registerfile.cpp
	$(CXX) $(CPPFLAGS) -c -o hex.o hex.cpp
	$(CXX) $(CPPFLAGS) -c -o rv32i main.o rv32i.o memory.o registerfile.o hex.o

clean:
	rm -f *.o main
