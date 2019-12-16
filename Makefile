CXXFLAGS =	-std=c++1y -O2	 -g -Wall
OBJS =		AOC2019.o
TARGET =	AOC2019

$(OBJS):	include/*.hpp Makefile

$(TARGET):	$(OBJS) 
	$(CXX) -o $(TARGET) $(OBJS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
