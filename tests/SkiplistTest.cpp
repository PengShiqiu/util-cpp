#include <iostream>
#include <vector>

#include "util/skiplist/skiplist.h"

int main(void) {
  SkipList* skiplist = new SkipList();
  skiplist->Add(100);

  for (int i = 0; i < 20; i++) {
    skiplist->Add(i);
  }

  printf("search 100 skiplist ret: %d\n", skiplist->Search(100));
  printf("search 200 skiplist ret: %d\n", skiplist->Search(200));
  printf("search 300 skiplist ret: %d\n", skiplist->Search(300));

  skiplist->Erase(100);
  skiplist->Erase(200);
  printf("search 100 skiplist ret: %d\n", skiplist->Search(100));
  printf("search 100 skiplist ret: %d\n", skiplist->Search(200));

  delete skiplist;
  return 0;
}