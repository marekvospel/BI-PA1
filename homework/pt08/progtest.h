#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

constexpr int PHONE_DIGITS = 10;

typedef struct TNode {
  char *m_Name;
  struct TNode *m_Child[PHONE_DIGITS];
} TNODE;

typedef struct {
  TNODE *m_Root;
  int m_Size;
} TPHONEBOOK;

bool addPhone(TPHONEBOOK *book, const char *phone, const char *name);
void delBook(TPHONEBOOK *book);
bool delPhone(TPHONEBOOK *book, const char *phone);
const char *findPhone(TPHONEBOOK *book, const char *phone);
