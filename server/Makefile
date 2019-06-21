SRC = ${wildcard src/*.cpp}
OBJS = ${patsubst %.cpp, %.o, $(SRC)}
all: $(OBJS)

%.o: %.cpp
	g++ -o $@ $^

.PHONY:clean
clean:
	find . -name *.o|xargs rm -f
