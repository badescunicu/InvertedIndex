#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "invertedindex.h"
#define NUMBER_OF_BUCKETS 100


int main() {
 /* put_doc(map, "test", 4);
  put_doc(map, "test2", 5);
  printf("%d %s\n", 
        map->buckets[hash((unsigned char*)"test") %
                    NUMBER_OF_BUCKETS]->data->documents.v[0],
        map->buckets[hash((unsigned char*)"test") %
                    NUMBER_OF_BUCKETS]->data->word);
  */
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
          /*printf("pos:%d\n", pos);*/
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

Array_t get_docs(Map_t *map, char *key) {
  Node_t *start_node;
  start_node = map->buckets[hash((unsigned char*)key) % map->size];
  while(start_node) {
    if(strcmp(start_node->data->word, key) == 0) {
      return start_node->data->documents;
    }
    start_node = start_node->next;
  }
}

void solve() {
  FILE *fin = fopen("date.in", "r");
  int i, lines;
  char **files, tmp[150];
  Map_t *map;
  Array_t docIDs;

  map = initialize_map(NUMBER_OF_BUCKETS);
  fgets(tmp, 150, fin);
  sscanf(tmp, "%d", &lines);
  files = (char**)malloc(lines * sizeof(char*));
  for(i = 0; i < lines; i++) {
    fgets(tmp, 150, fin);
    tmp[strlen(tmp) - 1] = '\0';
    files[i] = (char*)malloc((sizeof(tmp) + 1) * sizeof(char));
    strcpy(files[i], tmp);
    printf("%s\n", files[i]);
  }
  fclose(fin);

  for(i = 0; i < lines; i++) {
    fin = fopen(files[i], "r");
    while(fscanf(fin, "%s", tmp) != EOF) {
      put_doc(map, tmp, i);
    }
    fclose(fin);
  }

}
