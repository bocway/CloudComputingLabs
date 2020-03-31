CXXFLAGS+=-O2 -ggdb -DDEBUG
CXXFLAGS+=-Wall -Wextra

all: sudoku_solve

sudoku_solve: main.cc neighbor.cc sudoku_basic.cc sudoku_dancing_links.cc -lpthread
	sudo g++ -O2 -o $@ $^ 

clean:
	$(RM) sudoku_solve