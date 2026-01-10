#ifndef BST_H
#define BST_H
#include <stdbool.h>

#ifndef type_key_type
#define type_key_type int
#error "Must define key_type macro"
#endif

#ifndef type_key_name
#define type_key_name int
#error "Must define key_name macro"
#endif

#ifndef type_value_type
#define type_value_type int
#error "Must define value_type macro"
#endif

#ifndef type_value_name
#define type_value_name int
#error "Must define value_name macro"
#endif


#define JOIN2(a, b, sep) a##sep##b
#define join2(a, b, sep) JOIN2(a,b,sep)

#define JOIN3(a, b, c, sep) a##sep##b##sep##c
#define join3(a, b, c, sep) JOIN3(a,b,c,sep)

#define JOIN4(a, b, c, d, sep) a##sep##b##sep##c##sep##d
#define join4(a, b, c, d, sep) JOIN4(a,b,c,d,sep)

#define func_paste4(a, b, c, d) a##_##b##_##c##_##d
#define func_expand(a, b, c, d) func_paste4(a, b, c, d)
#define func(namespace, name)\
  func_expand(namespace, type_key_name, type_value_name, name)

#define ht()\
  join3(ht, type_key_name, type_value_name, _)

#define ht_node()\
  join4(ht, type_key_name, type_value_name, list_node, _)

#define ht_iter()\
  join4(ht, type_key_name, type_value_name, iter, _)

typedef struct ht_node() {
  type_key_type key;
  type_value_type value;
  struct ht_node() *next, *prev;
} ht_node();

typedef struct ht() {
  ht_node()** buckets;
  int buckets_count;
} ht();

typedef struct ht_iter() {
  int bucket, idx;
  ht_node()* node;
} ht_iter();

ht()  func(ht, new)(int buckets_count);
void func(ht,free)(ht()* ht);

int func(ht,ext_keycmp)(type_key_type key1, type_key_type key2);
int func(ht,hash)(void* key, int keysize);
ht_node()* func(ht,put)(ht()* ht, type_key_type key, type_value_type value);
type_value_type* func(ht,get)(ht()* ht, type_key_type key);
void func(ht,rem)(ht()* ht, type_key_type key);

void func(ht,print_key)(type_key_type);
void func(ht,print_value)(type_value_type);
void func(ht,print)(ht()* ht);

ht_iter() func(ht,begin)(ht()* ht);
bool      func(ht,end)(ht()* ht, ht_iter() it);
ht_iter() func(ht,next)(ht()* ht, ht_iter() it);

#endif
