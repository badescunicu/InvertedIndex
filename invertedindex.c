#include "invertedindex.h"

int main() {
  Map_t *map;
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

void solve() {

}
