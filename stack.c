#include <stdlib.h>
#include "stack.h"
#include "check_alloc.h"

void add_stack(stack__t **x, trie_t *node){
  stack__t *aux = malloc(sizeof(stack__t));
  check_alloc(aux);
  aux -> node = node;
  aux -> next = (*x);
  (*x) = aux;
}

void pop_stack(stack__t **x, trie_t **node){
  stack__t *aux = (*x) -> next;
  (*node) = (*x) -> node;
  free((*x));
  (*x) = aux;
}

void init_stack(stack__t **x){
  (*x) = NULL;
}

bool empty_stack(stack__t *x){
  return x == NULL;
}
