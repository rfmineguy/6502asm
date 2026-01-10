#include "dsa/hashtable.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifndef ALLOW_DIRECT_INCLUDE
#error "This file is not intended to be compiled directly"
#endif

ht() func(ht, new)(int buckets_count) {
  ht() b = {0};
  b.buckets_count = buckets_count;
  b.buckets = malloc(buckets_count * sizeof(ht_node()));
  return b;
}
void func(ht, free)(ht()* ht) {
  for (int i = 0; i < ht->buckets_count; i++) {
    ht_node()* n = ht->buckets[i];
    while (n) {
      ht_node()* t = n;
      n = n->next;
      free(t);
    }
    ht->buckets[i] = 0;
  }
  free(ht->buckets);
  ht->buckets = 0;
  ht->buckets_count = 0;
}
ht_node()* func(ht,put)(ht() *ht, type_key_type key, type_value_type value) {
  int hash = func(ht,hash)((void*)&key, sizeof(key)) % ht->buckets_count;
  ht_node()* n = ht->buckets[hash];
  if (!n) {
    ht->buckets[hash] = calloc(1, sizeof(ht_node()));
    ht->buckets[hash]->value = value;
    ht->buckets[hash]->key = key;
    return ht->buckets[hash];
  }
  ht_node()* new = calloc(1, sizeof(ht_node()));
  new->key = key;
  new->value = value;
  ht->buckets[hash] = new;
  new->next = n;
  return new;
}
type_value_type* func(ht,get)(ht() *ht, type_key_type key) {
  int hash = func(ht,hash)((void*)&key, sizeof(key)) % ht->buckets_count;
  ht_node()* n = ht->buckets[hash];
  if (!n) return 0;
  while (n && func(ht,ext_keycmp)(n->key, key) != 0) n = n->next;
  if (n) return &n->value;
  return 0;
}
void func(ht,rem)(ht() *ht, type_key_type key) {
  int hash = func(ht,hash)((void*)&key, sizeof(key)) % ht->buckets_count;
  ht_node()* n = ht->buckets[hash];

  // Find node with key
  while (n && func(ht,ext_keycmp)(n->key, key) != 0) n = n->next;
  if (n == 0) return;

  // If the node is the head
  if (!n->prev) {
    ht->buckets[hash] = n->next;
    free(n);
    return;
  }
  else if (n) {
    n->prev->next = n->next;
    free(n);
  }
}
void func(ht,print)(ht()* ht) {
  printf("=========================\n");
  for (int i = 0; i < ht->buckets_count; i++) {
    printf("%d: ", i);
    ht_node()* n = ht->buckets[i];
    while (n) {
      printf("{key: ");
      func(ht,print_key)(n->key);
      printf(", value: ");
      func(ht,print_value)(n->value);
      printf("}");
      n = n->next;
    }
    printf("\n");
  }
  printf("=========================\n");
}

ht_iter() func(ht,begin)(ht() *ht) {
  for (int i = 0; i < ht->buckets_count; i++) {
    if (ht->buckets[i]) {
      return (ht_iter()) {.bucket = i, .node = ht->buckets[i], .idx = 0};
    }
  }
  return (ht_iter()) {.node = 0};
}

bool func(ht,end)(ht()* ht, ht_iter() it) {
  return it.node == 0;
}

ht_iter() func(ht,next)(ht() *ht, ht_iter() it) {
  // go to next node in bucket
  if (it.node && it.node->next) {
    it.node = it.node->next;
    it.idx++;
    return it;
  }

  // find next bucket with something in it
  int i;
  for (i = it.bucket + 1; i < ht->buckets_count; i++) {
    if (ht->buckets[i]) {
      it.bucket = i;
      it.node = ht->buckets[i];
      break;
    }
  }
  if (i == ht->buckets_count) return (ht_iter()){.node = 0};
  it.idx++;
  return it;
}
