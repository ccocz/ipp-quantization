CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2

.PHONY: all clean

all: quantization

quantization: main.o stack.o trie.o union_set.o check_alloc.o
	$(CC) -o $@ $^

main.o: main.c trie.h union_set.h
	$(CC) $(CFLAGS) -c $<

stack.o: stack.c stack.h
	$(CC) $(CFLAGS) -c $<

trie.o: trie.c union_set.h stack.h trie.h
	$(CC) $(CFLAGS) -c $<

union_set.o: union_set.c union_set.h
	$(CC) $(CFLAGS) -c $<

check_alloc.o: check_alloc.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o quantization
