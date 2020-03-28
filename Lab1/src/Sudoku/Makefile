CXXFLAGS+=-O2 -ggdb -DDEBUG
CXXFLAGS+=-Wall -Wextra

all: sudoku

sudoku: main.cc neighbor.cc sudoku_basic.cc -lpthread
	sudo g++ -O2 -o $@ $^ 

clean:
	$(RM) sudoku