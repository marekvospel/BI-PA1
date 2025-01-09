#ifndef __PROGTEST__
#include "progtest.h"

#endif /* __PROGTEST__ */

void allocerr() {
  printf("Alloc error.\n");
  abort();
}

bool invalidPhone(const char *phone) {
  for (size_t i = 0;; i++) {
    if (phone[i] == '\0')
      break;

    if (phone[i] < '0' || phone[i] > '9')
      return true;
  }
  return false;
}

TNODE *createNode() {
  TNODE *node = (TNODE *)malloc(sizeof(TNODE));
  if (!node)
    allocerr();
  memset(node->m_Child, 0, sizeof(node->m_Child));
  node->m_Name = NULL;

  return node;
}

void delNode(TNODE *node) {
  if (!node)
    return;

  for (size_t i = 0; i < PHONE_DIGITS; i++) {
    delNode(node->m_Child[i]);
  }
  free(node->m_Name);
  free(node);
}

/// Returns true if old value was overwritten
bool addPhoneInternal(TNODE *node, const char *phone, const char *name) {
  if (*phone == '\0') {
    bool retval = false;
    if (node->m_Name) {
      retval = true;
    }
    node->m_Name =
        (char *)realloc(node->m_Name, sizeof(char) * (strlen(name) + 1));
    if (!node->m_Name)
      allocerr();

    strcpy(node->m_Name, name);
    return retval;
  }
  size_t index = *phone - '0';
  TNODE *nextNode = node->m_Child[index];

  if (!nextNode) {
    nextNode = createNode();
    node->m_Child[index] = nextNode;
  }

  phone += 1;
  return addPhoneInternal(nextNode, phone, name);
}

const char *findPhoneInternal(TNODE *node, const char *phone) {
  if (!node)
    return NULL;
  if (*phone == '\0')
    return node->m_Name;
  if (*phone < '0' || *phone > '9')
    return NULL;

  size_t index = *phone - '0';
  TNODE *nextNode = node->m_Child[index];

  phone += 1;
  const char *result = findPhoneInternal(nextNode, phone);
  if (!result) {
    return node->m_Name;
  }
  return result;
}

bool toPruneNode(TNODE *node) {
  if (node->m_Name)
    return false;

  for (size_t i = 0; i < PHONE_DIGITS; i++) {
    if (node->m_Child[i])
      return false;
  }

  return true;
}

/// 0 = not deleted
/// 1 = deleted, not pruned
/// 2 = deleted, pruned
int delPhoneInternal(TNODE *node, const char *phone) {
  if (!node)
    return 0;
  if (*phone == '\0') {
    if (!node->m_Name)
      return 0;
    free(node->m_Name);
    node->m_Name = NULL;

    return toPruneNode(node) ? 2 : 1;
  }
  if (*phone < '0' || *phone > '9')
    return 0;
  size_t index = *phone - '0';
  TNODE *nextNode = node->m_Child[index];

  phone += 1;
  int result = delPhoneInternal(nextNode, phone);
  if (result == 2) {
    node->m_Child[index] = NULL;
    delNode(nextNode);

    if (toPruneNode(node))
      return 2;
    result = 1;
  }
  return result;
}

bool addPhone(TPHONEBOOK *book, const char *phone, const char *name) {
  if (invalidPhone(phone))
    return false;

  if (!book->m_Root) {
    book->m_Root = createNode();
  }
  bool overwritten = addPhoneInternal(book->m_Root, phone, name);

  if (!overwritten) {
    book->m_Size += 1;
  }

  return true;
}

void delBook(TPHONEBOOK *book) {
  if (!book)
    return;

  delNode(book->m_Root);
  book->m_Size = 0;
}
bool delPhone(TPHONEBOOK *book, const char *phone) {
  if (!book)
    return false;

  int result = delPhoneInternal(book->m_Root, phone);
  if (result == 1 || result == 2) {
    book->m_Size -= 1;
  }
  if (result == 2) {
    TNODE *root = book->m_Root;
    book->m_Root = NULL;
    delNode(root);
  }

  return result;
}

const char *findPhone(TPHONEBOOK *book, const char *phone) {
  if (!book)
    return NULL;

  return findPhoneInternal(book->m_Root, phone);
}
