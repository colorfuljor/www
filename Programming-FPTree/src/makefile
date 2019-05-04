CXX = g++

HEADER_DIR = ../include

FPTREE_HEADER_DIR = $(HEADER_DIR)/fptree

CXXFLAGS = -std=c++11

CFLAGS = -fPIC -std=c99 -O3 -msse4.2 -mpclmul -march=native -funroll-loops -Wstrict-overflow -Wstrict-aliasing -Wall -Wextra -pedantic -Wshadow

MAIN = ./bin/main

YCSB = ./bin/ycsb

LYCSB = ./bin/lycsb

BIN = bin

FPTREE = fptree.o

UTILITY = utility.o

PALLOCATOR = pallocator.o

CLHASH = clhash.o

ALL := $(LYCSB) $(YCSB) $(MAIN) $(FPTREE) $(UTILITY) $(PALLOCATOR) $(CLHASH)

#============================TODO: all below============================
$(MAIN) : main.cpp $(FPTREE) $(UTILITY) $(PALLOCATOR) $(CLHASH) 
	$(CXX) $(CXXFLAGS) $^ -I $(HEADER_DIR) -g -o $@ -lpmem

$(FPTREE) : fptree.cpp $(FPTREE_HEADER_DIR)/fptree.h
	$(CXX) $(CXXFLAGS) -c fptree.cpp -I $(HEADER_DIR) -g -o $@ -lpmem

$(UTILITY) : utility.cpp $(HEADER_DIR)/utility/utility.h $(HEADER_DIR)/utility/clhash.h
	$(CXX) $(CXXFLAGS) -c utility.cpp -I $(HEADER_DIR) -g -o $@ -lpmem

$(PALLOCATOR) : p_allocator.cpp $(HEADER_DIR)/utility/p_allocator.h
	$(CXX) $(CXXFLAGS) -c p_allocator.cpp -I $(HEADER_DIR) -g  -o $@ -lpmem

$(CLHASH) : clhash.c $(HEADER_DIR)/utility/clhash.h
	$(CC) $(CFLAGS) -c clhash.c -I $(HEADER_DIR) -g -o $@

$(BIN):
	mkdir -p bin

$(LYCSB) : lycsb.cpp 
	$(CXX) $^ $(CXXFLAGS) -lleveldb -lpthread -g -o $@

$(YCSB) : ycsb.cpp $(FPTREE)
	$(CXX) $^ $(CXXFLAGS) -lleveldb -lpthread -I $(HEADER_DIR) -g -o $@

all : $(ALL)

clean :
	rm -rf *.o ./bin/* ../data/*

cleand : 
	rm -rf ../data/*