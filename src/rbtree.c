#include "rbtree.h"

#include <stdlib.h>
#include <stddef.h>

void RB_ERASE_FIXUP(rbtree *t, node_t *x);

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  p->nil = (node_t *)calloc(1, sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->root = p->nil;

  return p;
}

void left_rotate(rbtree *t, node_t *x){
  node_t *y = x->right; // y를 설정한다.
  x->right = y->left;   // y의 왼쪽 서브 트리를 x의 오른쪽 서브 트리로 옮긴다.
  
  if (y->left != t->nil){
    y->left->parent = x; // x의 부모를 y로 연결한다.
  }
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
  if (x->right != t->nil){
    x->right->parent = y;
  }

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

void delete_node(rbtree *t, node_t *n){
  if (n != t->nil){
    delete_node(t, n->left);
    delete_node(t, n->right);
    free(n);
    n = NULL;
  }
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  delete_node(t, t->root);
  free(t->nil);
  t->nil = NULL;
  free(t);
  t = NULL;
}

void rb_insert_fixup(rbtree *t, node_t *z){
  node_t *y;
  // 부모가 black이라면 반복문을 거치지 않고, root를 black으로 변경 후 함수 종료
  while (z->parent->color == RBTREE_RED){
    if (z->parent == z->parent->parent->left){
      y = z->parent->parent->right;

      // case 1
      // z, parent, uncle(tmp)가 모두 red일 경우
      if (y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        // 조부모를 새로운 z로 두고 while 루프를 돌면서 색을 바꿔줌.
        z = z->parent->parent;
      }
      // case 2
      else {
        if (z == z->parent->right){
          z = z->parent;
          left_rotate(t, z);
        }
        // case 3
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t,z->parent->parent);
      }
    }
    else{
      y = z->parent->parent->left;
      // case 1
      if (y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      // case 2
      else {
        if (z == z->parent->left){
          z = z->parent;
          right_rotate(t, z);
        }
        // case 3
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t,z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}
node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *z = malloc(sizeof(node_t)); // 노드를 삽입하기 위한 노드 생성 및 메모리 할당
  node_t *x = t->root;
  node_t *y = t->nil;
  z->key = key; // 삽입할 노드에 key값 저장

  while (x != t->nil){
    y = x;
    x = (z->key < y->key) ? x->left : x->right;
  }
  z->parent = y;
  if (y == t->nil){
    t->root = z;
  }
  else{
    if (z->key < y->key){
      y->left = z;
    }
    else {
      y->right = z;
    }
  }
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;
  rb_insert_fixup(t, z);
  return z;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *s_node = t->root; // 임시 노드 생성(현재노드) -> 루트 노드부터 시작
  
  // 현재 노드가 nil노드가 아니면서 key를 아직 찾지 못했다면 반복해서 수행
  // nil 노드까지 (leaf노드까지) 방문했거나, key를 찾으면 종료
  while (s_node != t->nil && s_node->key != key){
    // 삼항연산자 사용
    // 현재 노드의 키가 찾을 키보다 크다면 왼쪽으로 이동
    // 반대의 경우라면 오른쪽으로 이동
    s_node = (s_node->key > key) ? s_node->left : s_node->right;
  }
  // 일치하는 값이 있다면 해당 노드에 대한 정보를 리턴
  if (s_node->key == key){
    return s_node;
  }
  
  // 없다면 NULL 반환
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *min_node = t->root;

  while (min_node->left != t->nil){
    min_node = min_node->left;
  }
  return min_node;
}


node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *max_node = t->root;
  while (max_node->right != t->nil){
    max_node = max_node->right;
  }
  return max_node;
}


void rb_transplant(rbtree *t, node_t *u, node_t *v){
  // 삭제할 노드의 자식 노드를 부모노드에 연결해주는 함수
  // u의 위치에 v를 이식하는 작업
  // u->parent의 자식노드를 u대신 v로 바꿔준다.
  if (u->parent == t->nil){
    t->root = v;
  }
  else if (u == u->parent->left){
    u->parent->left = v;
  }
  else{
    u->parent->right = v;
  }
  v->parent = u->parent;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  if (p == NULL){
    return 0;
  }
  node_t *y = p;
  node_t *x; // fixed_up의 기준이 될 노드
  color_t y_original_color = p->color;

  // 노드의 한 자식이 nil이거나, 두개 다 nil일 경우
  // 나머지 자식을 x로 두고, y를 교체한다.
  // case1 : 왼쪽 노드가 nil일 경우
  if (p->left == t->nil){
    x = p->right;
    rb_transplant(t, p, p->right);
  }
  // case2 : 오른쪽 노드가 nil일 경우
  else if (p->right == t->nil){
    x = p->left;
    rb_transplant(t, p, p->left);
  }
  // case3 : 왼쪽 , 오른쪽 모두 nil이 아닐 경우
  else{
    // 
    //successor -> 지울 키값 보다 큰 수 중 제일 작은 값 찾기
    y = p->right;
    while (y->left != t->nil){
      y = y->left;
    }
    y_original_color = y->color;
    x = y->right;
    // case1 : p의 자식이 y일 때
    if (y->parent == p){
      x->parent = y;
    }
    else{
      // 그 외의 케이스 : y의 부모노드를 y->right와 연결해준다.
      rb_transplant(t, y, y->right);
      // p의 오른쪽 노드를 y와 연결한다.
      y->right = p->right;
      p->right->parent = y;
    }
    // p의 부모노드를 y와 연결한다.
    rb_transplant(t,p,y);
    y->left = p->left;
    p->left->parent = y;
    y->color = p->color;
  }
  // delete하며 바뀌거나 삭제된 노드의 색이 black이라면, 규칙5번 위반
  // 이를 해결하기 위해, 빈 공간을 대체한 노드 x에 가상의 black을 하나 더 추가.
  // 즉, 함수가 시작될 때, x는 여분의 b를 하나 더 들고 있음
  if (y_original_color == RBTREE_BLACK){
    RB_ERASE_FIXUP(t, x);
  }
  free(p);
  return 0;
}

void RB_ERASE_FIXUP(rbtree *t, node_t *x){
  node_t *w;
  // fix 시작 조건
  while (x != t->root && x->color == RBTREE_BLACK){
    // 기준 노드가 왼쪽에 위치할 때
    if (x == x->parent->left){
      w = x->parent->right;
      // case1 : 삼촌 노드가 RED일 때
      if(w->color == RBTREE_RED){
        // 형제 노드를 검은색으로 칠한다.
        w->color = RBTREE_BLACK;
        // 부모노드를 빨간색으로 칠한다.
        x->parent->color = RBTREE_RED;
        // 부모노드를 기준으로 좌회전
        left_rotate(t, x->parent);
        w = x->parent->right;
      }
      // case2 : 삼촌노드가 두개의 BLACK 노드를 가지고 있을 경우
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK){
        // 형제노드만 빨간색으로 만들고 타겟을 부모로 변경한다.
        w->color = RBTREE_RED;
        x = x->parent;
      }
      // 자식 중 하나는 빨간색일 경우
      else{
        // case3 : 삼촌이 BLACK이고, 왼쪽 자식이 RED, 오른쪽 자식은 BLACK일 경우
        if (w->right->color == RBTREE_BLACK){
          // 부모 노드의 색을 형제로 넘긴다.
          w->left->color = RBTREE_BLACK;
          // 부모노드와 형제 노드의 오른쪽 자식을 검은색으로 칠한다.
          w->color = RBTREE_RED;
          // 부모 노드 기준으로 우회전한다
          right_rotate(t, w);
          w = x->parent->right;
        }
        // case4 : 삼촌이 BLACK이고, 오른쪽 자식이 RED일 ㄷ경우
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t,x->parent);
        x = t->root;
      }
    }
    // 기준이 되는 노드가 오른쪽에 위치할 때
    else{
      w = x->parent->left;
      if (w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t,x->parent);
        w = x->parent->left;
      }
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK){
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else{
        if (w->left->color == RBTREE_BLACK){
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t,x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

int inorder(node_t *p, key_t *arr, const rbtree *t ,int i){
  if (p == t->nil){
    return i;
  }

  i = inorder(p->left, arr, t, i);
  arr[i++] = p->key;
  i = inorder(p->right, arr, t, i);
  return i;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  node_t *tmp = t->root;
  inorder(tmp, arr, t, 0);
  return 0;
}
