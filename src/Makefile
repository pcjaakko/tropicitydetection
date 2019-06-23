
CXX=g++

FLAGS=-O2 -Wshadow -Wunused -std=c++14

HEADERS=geometry3.hh auxiliary.hh trajectory.hh cube.hh
OBJECTS=geometry3.o trajectory.o
OBJECTS_P=$(patsubst %.o, build/%.o, $(OBJECTS))

build/%.o: %.cc $(HEADERS) Makefile
	$(CXX) $(FLAGS) -c $< -o $@

main: $(OBJECTS_P) main.cc Makefile
	$(CXX) $(OBJECTS_P) main.cc -o $@


.PHONY: clean
clean:
	rm -f a.out build/*.o
