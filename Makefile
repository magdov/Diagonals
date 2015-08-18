FLAGS = -O2
all: pascal

common.o: common.h common.c
	g++ ${FLAGS} -c common.h common.c

wielokaty: wielokaty.cpp
	g++ ${FLAGS} wielokaty.cpp -o wielokaty


pascal.o: pascal.cpp 
	g++ ${FLAGS} -c pascal.cpp

pascal: common.o pascal.o
	g++ ${FLAGS} common.o pascal.o -o pascal
