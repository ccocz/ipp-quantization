#include <stdlib.h>

void check_alloc(void *ptr){
  if (ptr == NULL){
    exit(1);
  }
}