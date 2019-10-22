#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include "trie.h"
#include "union_set.h"
#include "check_alloc.h"
#include "stack.h"

#define INV -1
#define INV_EN 0

static void make_null(trie_t *x){
  for (int i = 0; i < NO_OF_SON; i++){
    x -> son[i] = NULL;
  }
}

void init_trie(trie_t **x){
  *x = malloc(sizeof(trie_t));
  check_alloc(x);
  make_null(*x);
  (*x) -> parent = NULL;
}

static trie_t *init_node(){
  trie_t *new = malloc(sizeof(trie_t));
  check_alloc(new);
  make_null(new);
  new -> parent = NULL;
  return new;
}

void add_trie(trie_t *root, particle_t x){
  for (int i = 0, which; i < x.length; i++){
    which = x.seq[i] - '0';
    if (root -> son[which] == NULL){
      root -> son[which] = init_node();
    }
    root = root -> son[which];
  }
}

trie_t *in_trie(trie_t *root, particle_t x){
  bool ok = true;
  for (int i = 0, which; i < x.length && ok; i++){
    which = x.seq[i] - '0';
    if (root -> son[which] == NULL){
      ok = false;
    }
    else{
      root = root -> son[which];
    }
  }
  if (ok){
    return root;
  }
  else{
    return NULL;
  }
}

void clean_sub(trie_t *root){
  stack__t *cleaning;
  trie_t *aux;
  init_stack(&cleaning);
  if (root){
    add_stack(&cleaning, root);
  }
  while (!empty_stack(cleaning)){
    pop_stack(&cleaning, &aux);
    for (int i = 0; i < NO_OF_SON; i++){
      if (aux -> son[i]){
        add_stack(&cleaning, aux -> son[i]);
      }
    }
    free(aux);
  }
}

void remove_trie(trie_t *root, particle_t x){
  x.length--;
  trie_t *pref = in_trie(root, x);
  if (pref){
    int which = x.seq[x.length] - '0';
    if (pref -> son[which]){
      clean_sub(pref -> son[which]);
      pref -> son[which] = NULL;
    }
  }
}

bool merge(trie_t *root, particle_t x, particle_t y){
  trie_t *first = in_trie(root, x);
  trie_t *second = in_trie(root, y);
  if (!first || !second){
    return false;
  }
  if (first == second){
    return true;
  }
  if (!first -> parent && !second -> parent){
    return false;
  }
  else if (!first -> parent && second -> parent){
    union_t *groupB = find_set(second -> parent);
    first -> parent = groupB;
    return true;
  }
  else if (first -> parent && !second -> parent){
    union_t *groupA = find_set(first -> parent);
    second -> parent = groupA;
    return true;
  }
  else{
    union_t *groupA = find_set(first -> parent);
    union_t *groupB = find_set(second -> parent);
    groupB -> parent = groupA;
    bool bonus = (groupA -> energy % 2 == 1 && groupB -> energy % 2 == 1);
    groupA -> energy = ((groupA -> energy / 2) + (groupB -> energy / 2)) + bonus;
    return true;
  }
}

bool change_energy(trie_t *root, particle_t x, uint64_t e, alloced_t **nodes){
  trie_t *node = in_trie(root, x);
  if (!node){
    return false;
  }
  if (!node -> parent){
    node -> parent = init_union(nodes);
  }
  union_t *group = find_set(node -> parent);
  group -> energy = e;
  return true;
}

uint64_t get_energy(trie_t *root, particle_t x){
  trie_t *node = in_trie(root, x);
  if (!node || !node -> parent){
    return INV_EN;
  }
  union_t *group = find_set(node -> parent);
  return group -> energy;
}
