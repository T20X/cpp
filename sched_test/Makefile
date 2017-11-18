INCLUDES = -I$(BOOST)
LIBS = 
DEBUG = -g

CXX = g++
CXXFLAGS = -O0 -Wall -m64 -pthread -std=c++11 -D_REENTRANT $(INCLUDES) $(DEBUG) 

all: clean app 

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

.cpp:
	$(CXX) $(CXXFLAGS) $< -o $@ 

BOOK_SRC = main.cpp 
BOOK_OBJ = $(addsuffix .o, $(basename $(BOOK_SRC)))

app: $(BOOK_OBJ)
	$(CXX) $(CXXFLAGS) -masm=intel -o $@ $(BOOK_OBJ) 

clean:
	rm -f $(BOOK_OBJ) vtable
