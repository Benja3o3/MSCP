CC=g++
CFLAGS=-Wall -std=c++14 -O3
BINS=main MSCGreedy.o MSCP_EXHAUSTIVO.o MSCGreedyOpt.o
all: clean main

main: MSCGreedy.o MSCP_EXHAUSTIVO.o MSCGreedyOpt.o
	$(CC) $(CFLAGS) -o main main.cpp MSCGreedy.o MSCP_EXHAUSTIVO.o MSCGreedyOpt.o

MSCGreedy.o: MSCGreedy.cpp
	$(CC) $(CFLAGS) -c MSCGreedy.cpp

MSCP_EXHAUSTIVO.o: MSCP_EXHAUSTIVO.cpp
	$(CC) $(CFLAGS) -c MSCP_EXHAUSTIVO.cpp

MSCGreedyOpt.o: MSCGreedyOpt.cpp
	$(CC) $(CFLAGS) -c MSCGreedyOpt.cpp

clean:
	@echo " [CLN] Removing binary files"
	@rm -f $(BINS)