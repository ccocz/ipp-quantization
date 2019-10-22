#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include <limits.h>
#include "trie.h"
#include "union_set.h"
#include "check_alloc.h"

#define INV -1
#define COM_NO 5
#define INV_EN 0
#define MULT 3
#define DIV 2

const char *commands[] = {"DECLARE", "REMOVE", "VALID", "ENERGY", "EQUAL"};
const int sizes_of_commands[] = {7, 6, 5, 6, 5};
enum commands_enum{DECLARE, REMOVE, VALID, ENERGY, EQUAL};

struct operation{
  int type;
  trie_t *root;
  particle_t a;
  particle_t b;
  uint64_t energy;
};

typedef struct operation operation_t;

/* read from current line until you meet space or
 * newline, store what you met in last and 
 * return sequence s as well as store length of it */
static char *get_next(char *last, int *n){
  int x;
  int current = 0;
  int overall = 0;
  char *s = NULL;
  while ((x = getchar()) != '\n' && x != ' ' && x != EOF){
    if (current == overall){
      overall = 1 + overall * MULT / DIV;
      s = realloc(s, sizeof(char) * overall);
      check_alloc(s);
    }
    s[current++] = x;
  }
  *n = current;
  *last = x;
  return s;
}
/* if line is invalid get all stuff */
void readline(){
  int x;
  while ((x = getchar()) != '\n' && x != EOF);
}

void error(char c){
  fprintf(stderr, "ERROR\n");
  if (c != '\n'){
    readline();
  }
}
/* determine command type */
int which(char *s, int n){
  if (!s){
    return INV;
  }
  bool ok = true;
  for (int i = 0; i < COM_NO; i++){
    if (sizes_of_commands[i] == n){
      ok = true;
      for (int j = 0; j < n && ok; j++){
        if (commands[i][j] != s[j]){
          ok = false;
        }
      }
      if (ok){
        return i;
      }
    }
  }
  return INV;
}

bool is_valid(particle_t x){
  for (int i = 0; i < x.length; i++){
    if (!(x.seq[i] >= '0' && x.seq[i] <= '3')){
      return false;
    }
  }
  return x.length;
}
/* commands require one parametr */
bool only_one(int command){
  return command != ENERGY && command != EQUAL;
}
/* known command and it is valid, do the command */
void fulfill(operation_t x, alloced_t **nodes){
  uint64_t aux;
  switch (x.type){
    case DECLARE:
      add_trie(x.root, x.a);
      puts("OK");
    break;
    case VALID:
      if (in_trie(x.root, x.a)){
        puts("YES");
      }
      else{
        puts("NO");
      }
    break;
    case REMOVE:
      remove_trie(x.root, x.a);
      puts("OK");
    break;
    case EQUAL:
      if (merge(x.root, x.a, x.b)){
        puts("OK");
      }
      else{
        fprintf(stderr, "ERROR\n");
      }
    break;
    case ENERGY:
      if (x.energy == INV_EN){
        aux = get_energy(x.root, x.a);
        if (aux != INV_EN){
          printf("%" PRIu64 "\n", aux);
        }
        else{
          fprintf(stderr, "ERROR\n");
        }
      }
      else{
        if (change_energy(x.root, x.a, x.energy, nodes)){
          puts("OK");
        }
        else{
          fprintf(stderr, "ERROR\n");
        }
      }
    break;
  }
}

bool is_big(char *x, char *y, int n){
  for (int i = 0; i < n; i++){
    if (x[i] > y[i]) return true;
    else if (x[i] < y[i]) return false;
  }
  return false;
}

/* convert string energy to uns long */
uint64_t convert(particle_t x){
  if (x.length == 0 || x.length > 20){
    return INV_EN;
  }
  for (int i = 0; i < x.length; i++){
    if (!(x.seq[i] >= '0' && x.seq[i] <= '9')){
      return INV_EN;
    }
  }
  if (x.seq[0] == '0'){
    return INV_EN;
  }
  char *max = "18446744073709551615";
  int size_max = 20;
  if (x.length == size_max && is_big(x.seq, max, size_max)){
    return INV_EN;
  }
  uint64_t base = 1;
  uint64_t number = 0;
  for (int i = x.length - 1; i >= 0; i--){
    number = (number + base * (x.seq[i] - '0'));
    base *= 10;
  }
  return number;
}
/* try to read input, and figure out what is going on with input line */
void start(){
  int x, length, type;
  char *command, last;
  particle_t partA, partB;
  uint64_t energy = INV_EN;
  bool no_error = true;
  trie_t *root;
  operation_t send;
  alloced_t *nodes = NULL;
  init_trie(&root);
  while (x != EOF && (x = getchar()) != EOF){
    if (x == '#'){
      readline();
    }
    else if (x != '\n'){
      ungetc(x, stdin);
      no_error = true;
      energy = INV_EN;
      partA.seq = partB.seq = NULL;
      command = NULL;
      command = get_next(&last, &length);
      type = which(command, length);
      if (type == INV || last == '\n' || last == EOF){
        error(last);
      }
      else{
        partA.seq = get_next(&last, &partA.length);
        if (!is_valid(partA) || last == EOF){
          no_error = false;
        }
        if (last == ' '){
          if (only_one(type)){
            no_error = false;
          }
          else if (type == EQUAL){
            partB.seq = get_next(&last, &partB.length);
            if (!is_valid(partB) || last == ' ' || last == EOF){
              no_error = false;
            }
          }
          else if (type == ENERGY){
            partB.seq = get_next(&last, &partB.length);
            energy = convert(partB);
            if (energy == INV_EN || last == ' ' || last == EOF){
              no_error = false;
            }
          }
        }
        else{
          if (type == EQUAL){
            no_error = false;
          }
        }
        if (!no_error){
          error(last);
        }
        else{
          send.type = type;
          send.root = root;
          send.a = partA;
          send.b = partB;
          send.energy = energy;
          fulfill(send, &nodes);
        }
      }
      free(partA.seq);
      free(partB.seq);
      free(command);
    }
  }
  clean_sub(root);
  free_union(nodes);
}

int main(void){
  start();
  return 0;
}
