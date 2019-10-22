#include <stdlib.h>
#include <stdio.h>
#include "union_set.h"
#include "check_alloc.h"

static void add_to_free(alloced_t **nodes, void *this){
  if (!*nodes){
    *nodes = malloc(sizeof(alloced_t));
    check_alloc(nodes);
    (*nodes) -> node = this;
    (*nodes) -> next = NULL;
  }
  else{
    alloced_t *aux = malloc(sizeof(alloced_t));
    check_alloc(aux);
    (*aux).next = (*nodes) -> next;
    (*aux).node = this;
    (*nodes) -> next = aux;
  }
}

void free_union(alloced_t *nodes){
  alloced_t *aux;
  while (nodes){
    free(nodes -> node);
    aux = nodes -> next;
    free(nodes);
    nodes = aux;
  }
}

union_t *init_union(alloced_t **nodes){
  union_t *new = malloc(sizeof(union_t));
  check_alloc(new);
  new -> energy = 0;
  new -> parent = new;
  add_to_free(nodes, new);
  return new;
}

union_t *find_set(union_t *x){
  if (x == x -> parent){
    return x;
  }
  return x -> parent = find_set(x -> parent);
}
