#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "invertedindex.h"

int main() {
  Map_t *map;
  Entry_t *data;
  data = initialize_entry("token1", 2);
  printf("%d %d %s\n", data->documents.cap, data->documents.v[0],
         data->word);
  map = initialize_map(20);
  put_doc(map, "test", 4);
  printf("%d %s\n", 
        map->buckets[hash((unsigned char*)"test") % 20]->data->documents.v[3],
        map->buckets[hash((unsigned char*)"test") % 20]->data->word);
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

void put_doc(Map_t *map, char *key, int docID) {
  /* Daca bucketul este gol, insereaza primul nod */
  Node_t *bucket = map->buckets[hash((unsigned char*)key) % map->size];
  if(!bucket) {
    bucket = (Node_t*)malloc(sizeof(Node_t));
    bucket->data = initialize_entry(key, docID);
    bucket->next = NULL;
    map->buckets[hash((unsigned char*)key) % map->size] = bucket;
  }
  else {
    int found = 0;
    int pos;
    Node_t *prev;
    prev = bucket;
    while(bucket) {
      if(strcmp(bucket->data->word, key) == 0) {
        found = 1;
        if(!is_in_array(bucket->data->documents, docID)) {
          if(bucket->data->documents.n == bucket->data->documents.cap - 1) {
            bucket->data->documents.cap *= 2;
            bucket->data->documents.v = realloc(bucket->data->documents.v, 
                                        bucket->data->documents.cap);
          }
          bucket->data->documents.n += 1;
          pos = bucket->data->documents.n;
          //printf("pos:%d\n", pos);
          bucket->data->documents.v[pos] = docID;
        }
        break;
      }
      prev = bucket;
      bucket = bucket->next;
    }
    if(!found) {
      Node_t *new_node = (Node_t*)malloc(sizeof(Node_t));
      new_node->data = initialize_entry(key, docID);
      new_node->next = NULL;
      prev->next = new_node;
    }
  }
}

int is_in_array(Array_t documents, int docID) {
  int i;
  for(i = 0; i <= documents.n; i++)
    if(documents.v[i] == docID)
      return 1;
  return 0;
}

void solve() {

}
