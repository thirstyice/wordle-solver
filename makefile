objs = main.o wordle.o
CXXFLAGS = -std=c++11
CXX = clang++
LD = clang++

wordle-solver : $(objs)
	$(CXX) -o wordle-solver $(objs)

main.o : wordle.h
wordle.o : wordle.h answers.h

clean :
	rm wordle-solver $(objs)
