#include <inttypes.h>
#include <stdbool.h>
#include "union_set.h"

#ifndef TRIE_H
#define TRIE_H
#define NO_OF_SON 4

struct trie{
  struct trie *son[NO_OF_SON];
  union_t *parent;
};

typedef struct trie trie_t;

struct particle{
  char *seq;
  int length;
};

typedef struct particle particle_t;

void init_trie(trie_t **root);
void add_trie(trie_t *root, particle_t x);
trie_t *in_trie(trie_t *root, particle_t x);
void remove_trie(trie_t *root, particle_t x);
bool merge(trie_t *root, particle_t x, particle_t y);
bool change_energy(trie_t *root, particle_t x, uint64_t e, alloced_t **nodes);
uint64_t get_energy(trie_t *root, particle_t x);
void clean_sub(trie_t *root);

#endif
