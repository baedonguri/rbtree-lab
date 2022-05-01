#include "rbtree.h"

#include <stdlib.h>
#include <stddef.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  nil->color = RBTREE_BLACK;
  p->root = nil;
  p->nil = nil;
  return p;
}

void left_rotate(rbtree *t, node_t *x){
  node_t *y = x->right; // y를 설정한다.
  x->right = y->left;   // y의 왼쪽 서브 트리를 x의 오른쪽 서브 트리로 옮긴다.
  
  if (y->left != t->nil)
    y->left->parent = x; // x의 부모를 y로 연결한다.

  y->parent = x->parent;

  if (x->parent == t->nil){
    t->root = y;
  }
  else if (x == x->parent->left){
    x->parent->left = y;
  }
  else{
    x->parent->right = y;
  }
  y->left = x; // x를 y의 왼쪽으로 놓는다.
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *y){
  node_t *x = y->left;
  y->left = x->right;
  if (x->right != t->nil)
    x->right->parent = y;

  x->parent = y->parent;

  if (y->parent == t->nil){
    t->root = x;
  }
  else if (y == y->parent->left){
    y->parent->left = x;
  }
  else{
    y->parent->right = x;
  }
  x->right = y;
  y->parent = x;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *z = malloc(sizeof(node_t)); // 노드를 삽입하기 위한 노드 생성 및 메모리 할당
  node_t *x = t->root;
  node_t *y = t->nil;
  z->key = key; // 삽입할 노드에 key값 저장

  while (x != t->nil){
    y = x;
    if (z->key < x->key){
      x = x->left;
    }
    else{
      x = x->right;
    }
  }
  z->parent = y;
  if (y == t->nil){
    t->root = z;
  }
  else if (z->key < y->key){
    y->left = z;
  }
  else {
    y->right = z;
  }

  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;
  RB_INSERT_FIXUP(t, z);
  return z;
}

void RB_INSERT_FIXUP(rbtree *t, node_t *z){
  node_t *tmp = malloc(sizeof(node_t));
  // 부모가 black이라면 반복문을 거치지 않고, root를 black으로 변경 후 함수 종료
  while (z->parent->color == RBTREE_RED){
    if (z->parent == z->parent->parent->left){
      tmp = z->parent->parent->right;

      // case 1
      // z, parent, uncle(tmp)가 모두 red일 경우
      if (tmp->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        tmp->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        // 조부모를 새로운 z로 두고 while 루프를 돌면서 색을 바꿔줌.
        z = z->parent->parent;
      }
      // case 2
      else if (z == z->parent->right){
        z = z->parent;
        left_rotate(t, z);
      }
      // case 3
      z->parent->color = RBTREE_BLACK;
      z->parent->parent->color = RBTREE_RED;
      right_rotate(t,z->parent->parent);
    }
    else{
      tmp = z->parent->parent->left;
      // case 1
      if (tmp->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        tmp->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      // case 2
      else if (z == z->parent->left){
        z = z->parent;
        right_rotate(t, z);
      }
      // case 3
      z->parent->color = RBTREE_BLACK;
      z->parent->parent->color = RBTREE_RED;
      left_rotate(t,z->parent->parent);
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
