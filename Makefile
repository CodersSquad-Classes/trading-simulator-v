CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall -Wextra

OBJS = main.o order.o book.o engine.o

all: clob

clob: $(OBJS)
	$(CXX) $(CXXFLAGS) -o clob $(OBJS)

run: clob
	./clob

main.o: main.cpp engine.h book.h order.h colors.h
	$(CXX) $(CXXFLAGS) -c main.cpp

order.o: order.cpp order.h
	$(CXX) $(CXXFLAGS) -c order.cpp

book.o: book.cpp book.h order.h
	$(CXX) $(CXXFLAGS) -c book.cpp

engine.o: engine.cpp engine.h book.h order.h
	$(CXX) $(CXXFLAGS) -c engine.cpp

clean:
	rm -f $(OBJS) clob
