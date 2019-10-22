#include <inttypes.h>

#ifndef UNION_SET_H
#define UNION_SET_H

struct element {
  uint64_t energy;
  struct element *parent;
};

struct alloced{
  void *node;
  struct alloced *next;
};

typedef struct element union_t;
typedef struct alloced alloced_t;

union_t *init_union(alloced_t **nodes);

union_t *find_set(union_t *x);

void free_union(alloced_t *nodes);

#endif
