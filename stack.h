#include "trie.h"

#ifndef STACK_H
#define STACK_H

struct stack_{
  trie_t *node;
  struct stack_ *next;
};

typedef struct stack_ stack__t;

void add_stack(stack__t **x, trie_t *node);
void pop_stack(stack__t **x, trie_t **node);
void init_stack(stack__t **x);
bool empty_stack(stack__t *x);

#endif
