#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "invertedindex.h"
#define NUMBER_OF_BUCKETS 100


int main() {
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
  data->documents.cap = 4;
  data->documents.v = (int*)malloc(sizeof(int) *
                      data->documents.cap);
  data->documents.n = 0;
  data->documents.v[data->documents.n] = file_num;
  return data;
}

void put_doc(Map_t *map, char *key, int docID) {

  /* Daca bucketul este gol, insereaza primul nod */
  Node_t *bucket = map->buckets[hash((unsigned char*)key) % map->size];
  if (!bucket) {
    bucket = (Node_t*)malloc(sizeof(Node_t));
    bucket->data = initialize_entry(key, docID);
    bucket->next = NULL;
    map->buckets[hash((unsigned char*)key) % map->size] = bucket;
  }

  /* Altfel, adauga docID la Array_t-ul corespunzator cheii;
   * creeaza o noua intrare daca cheia nu exista */
  else {
    int found = 0;
    int pos;
    Node_t *prev;
    prev = bucket;
    while (bucket) {
      if (strcmp(bucket->data->word, key) == 0) {
        found = 1;
        if (!is_in_array(bucket->data->documents, docID)) {
          if (bucket->data->documents.n == bucket->data->documents.cap - 1) {
            bucket->data->documents.cap *= 2;
            bucket->data->documents.v = realloc(bucket->data->documents.v, 
                                        bucket->data->documents.cap);
          }
          bucket->data->documents.n += 1;
          pos = bucket->data->documents.n;
          bucket->data->documents.v[pos] = docID;
        }
        break;
      }
      prev = bucket;
      bucket = bucket->next;
    }
    if (!found) {
      Node_t *new_node = (Node_t*)malloc(sizeof(Node_t));
      new_node->data = initialize_entry(key, docID);
      new_node->next = NULL;
      prev->next = new_node;
    }
  }
}

int is_in_array(Array_t documents, int docID) {
  int i;
  for (i = 0; i <= documents.n; i++)
    if (documents.v[i] == docID)
      return 1;
  return 0;
}

Array_t get_docs(Map_t *map, char *key) {
  Array_t result;
  Node_t *start_node;
  int found = 0;
  start_node = map->buckets[hash((unsigned char*)key) % map->size];
  while (start_node) {
    if (strcmp(start_node->data->word, key) == 0) {
      result = start_node->data->documents;
      found = 1;
      break;
    }
    start_node = start_node->next;
  }

  if (!found) {
    result.n = -1;
    result.cap = 4;
  }

  return result;
}

Array_t intersection(const Array_t files1, const Array_t files2) {
  int i, j;
  Array_t result;
  result.n = -1;
  result.cap = 4;
  result.v = (int*)malloc(result.cap * sizeof(int));
  i = 0;
  j = 0;
  while (i <= files1.n && j <= files2.n) {
    if (files1.v[i] == files2.v[j]) {
      append(&result, files1.v[i]);
      i++;
      j++;
    }
    else if (files1.v[i] < files2.v[j]) {
      i++;
    }
    else if (files1.v[i] > files2.v[j]) {
      j++;
    }
  }
  return result;
}

Array_t reunion(const Array_t files1, const Array_t files2) {
  int i, j, k, tmp;
  Array_t result;
  result.n = -1;
  result.cap = files1.cap;
  result.v = (int*)malloc(result.cap * sizeof(int));
  i = 0;
  j = 0;

  while (i <= files1.n && j <= files2.n) {
    if (files1.v[i] < files2.v[j]) {
      tmp = files1.v[i];
      i++;
    }
    else if (files1.v[i] > files2.v[j]) {
      tmp = files2.v[j];
      j++;
    }
    else {
      tmp = files1.v[i];
      i++;
      j++;
    }
    append(&result, tmp);
  }

  if (i <= files1.n) {
    for (k = i; k <= files1.n; k++) {
      append(&result, files1.v[k]);
    }
  }
  else if (j <= files2.n) {
    for (k = j; k <= files2.n; k++) {
      append(&result, files2.v[k]);
    }
  }

  return result;
}

void print_map(Map_t *map) {
  int i, j;
  Node_t *start_bucket;
  for (i = 0; i < NUMBER_OF_BUCKETS; i++) {
    if (map->buckets[i]) {
      start_bucket = map->buckets[i];
      while (start_bucket) {
        printf("%s apare in : ", start_bucket->data->word);
        for (j = 0; j <= start_bucket->data->documents.n; j++) {
          printf("%d ", start_bucket->data->documents.v[j]); 
        }
        printf("\n");
        start_bucket = start_bucket->next;
      }
    }
  }
}

void append(Array_t *vector, int value) {
  if (vector->n == vector->cap - 1) {
    vector->cap *= 2;
    vector->v = realloc(vector->v, vector->cap * sizeof(int));
  }
  (vector->n)++;
  vector->v[vector->n] = value;
}

void strip_word(char *s) {
  char *punc_marks = ",.?!'][}{)(\"";
  char *aux;
  int i;
  i = strlen(s) - 1;
  while (strchr(punc_marks, s[i])) {
    i--;
  }
  s[i + 1] = '\0';

  i = 0;
  while (strchr(punc_marks, s[i])) {
    i++;
  }
  aux = strdup(s + i);
  strcpy(s, aux);
}

void solve() {
  FILE *fin = fopen("input.in", "r");
  FILE *fout = fopen("output.out", "w");
  FILE *cur_file;
  Array_t set1, set2;
  if (fin) {
    int i, number_of_files, queries, k;
    char **files, tmp[2000];
    char *line, *word, *operator;
    Map_t *map;
    map = initialize_map(NUMBER_OF_BUCKETS);

    /* Citeste nr de fisiere si retine intr-un vector de cuvinte
     * numele acestora */
    fgets(tmp, 2000, fin);
    sscanf(tmp, "%d", &number_of_files);
    files = (char**)malloc(number_of_files * sizeof(char*));

    for (i = 0; i < number_of_files; i++) {
      fgets(tmp, 150, fin);
      /* Retine in "line" numele fisierului, fara CRLF-ul de la final */
      line = strtok(tmp, "\r\n");
      files[i] = (char*)malloc((sizeof(tmp) + 1) * sizeof(char));
      strcpy(files[i], line);
    }

    /* Parcurge cuvintele din fiecare fisier si le adauga in map */
    for (i = 0; i < number_of_files; i++) {
      cur_file = fopen(files[i], "r");
      if (cur_file) {
        while (fscanf(cur_file, "%s", tmp) != EOF) {
          strip_word(tmp);
          put_doc(map, tmp, i);
        }
        fclose(cur_file);
      }
      else {
        printf("Eroare la deschiderea fisierului %s!\n", files[i]);
      }
    }

    /* Citeste numarul de interogari*/
    fgets(tmp, 2000, fin);
    sscanf(tmp, "%d", &queries);

    /* Proceseaza fiecare interogare, efectuand operatii pe
     * vectorii ce contin fisierele in care apar respectivele chei */
    for (i = 0; i < queries; i++) {
      fgets(tmp, 2000, fin);

      /* Retine in "line" interogarea, fara CRLF-ul de la final */
      line = strtok(tmp, "\r\n");
      fprintf(fout, "%s:", line);
      word = strtok(line, " \n\r");
      set1 = get_docs(map, word);
      operator = strtok(NULL, " \n\r");

      while (operator) {
        word = strtok(NULL, " \n\r");
        set2 = get_docs(map, word);
        if (strcmp(operator, "&") == 0) {
          set1 = intersection(set1, set2);
        }
        else {
          set1 = reunion(set1, set2);
        }
        operator = strtok(NULL, " \n\r");
      }

      for (k = 0; k <= set1.n; k++) {
        fprintf(fout, " %d", set1.v[k]);
      }
      fprintf(fout, " \n");
    }

    fclose(fin);
    fclose(fout);
  }

  else {
    printf("Eroare la deschiderea fisierului input.in!\n");
  }
}
