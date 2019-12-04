CXXFLAGS =	-O0 -g3 -Wall
OBJS =		AOC2019.o
TARGET =	AOC2019

$(OBJS):	include/*.hpp

$(TARGET):	$(OBJS) Makefile
	$(CXX) -o $(TARGET) $(OBJS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
