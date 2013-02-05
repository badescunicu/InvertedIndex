#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "invertedindex.h"

int main() {
  Map_t *map;
  Entry_t *data;
  data = initialize_entry("token1", 2);
  printf("%d %d %s", data->documents.cap, data->documents.v[0],
         data->word);
  map = initialize_map(20);
	solve();
	return 0;
}

Map_t* initialize_map(int size) {
  Map_t *map = (Map_t*)malloc(sizeof(Map_t));
  map->size = size;
  map->buckets = (Node_t**)malloc(size * sizeof(Node_t*));
  return map;
}

Entry_t* initialize_entry(char *s, int file_num) {
  Entry_t *data;
  data = (Entry_t*)malloc(sizeof(Entry_t));
  data->word = (char*)malloc((strlen(s) + 1) * sizeof(char));
  strcpy(data->word, s);
  data->documents.cap = 5;
  data->documents.v = (int*)malloc(sizeof(int) *
                      data->documents.cap);
  data->documents.n = 0;
  data->documents.v[data->documents.n] = file_num;
  return data;
}

void solve() {

}
