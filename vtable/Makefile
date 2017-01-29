INCLUDES = -I$(BOOST)
LIBS = 
DEBUG = -g

CXX = g++
CXXFLAGS = -O0 -Wall -m64 -pthread -std=c++14 -D_REENTRANT $(INCLUDES) $(DEBUG) -fdump-class-hierarchy=vtable.report

all: clean vtable 

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

.cpp:
	$(CXX) $(CXXFLAGS) $< -o $@ 

BOOK_SRC = main.cpp 
BOOK_OBJ = $(addsuffix .o, $(basename $(BOOK_SRC)))

vtable: $(BOOK_OBJ)
	$(CXX) $(CXXFLAGS) -masm=intel -o $@ $(BOOK_OBJ) \
	-Wl,--rpath=$(GLIBC_DEBUG) \
	-Wl,--dynamic-linker=$(GLIBC_DEBUG)/ld-linux-x86-64.so.2 \

clean:
	rm -f $(BOOK_OBJ) vtable
