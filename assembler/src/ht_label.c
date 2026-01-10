#define type_key_type string_view
#define type_key_name string
#define type_value_type label
#define type_value_name label
#include "assembler_data.h"
#include "parser.h"
#include <string.h>
#include <stdio.h>

int ht_string_label_hash(void* key, int len) {
  string_view sv = *(string_view*)key;
  // const char* s = *(const char**)key;
  int hash = 0;
  for (int i = 0; i < sv.len; i++) {
    char c = *(char*)(sv.s + i);
    hash += c * 3 * i;
  }
  return hash < 0 ? -hash : hash;
}
int ht_string_label_ext_keycmp(string_view key1, string_view key2) {
  if (key1.len != key2.len) return 1;
  return strncmp(key1.s, key2.s, key1.len);
}

void ht_string_label_print_key(string_view key) {
  printf("%.*s", (int)key.len, key.s);
}

void ht_string_label_print_value(label label) {
  parser_print_label(label);
}

#define ALLOW_DIRECT_INCLUDE
#include "dsa/hashtable.c"
