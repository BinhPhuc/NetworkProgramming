#include <stdio.h>
#include <stdlib.h>

struct Node {
  int value;
  struct Node *next;
};

struct Node *newNode(int value) {
  struct Node *node = (struct Node *)malloc(sizeof(struct Node));
  node->value = value;
  node->next = NULL;
  return node;
}

void print(struct Node *head) {
  struct Node *current = head;
  while (current != NULL) {
    printf("%d -> ", current->value);
    current = current->next;
  }
  printf("NULL\n");
}

void insertHead(struct Node **head, int value) {
  struct Node *new_head = newNode(value);
  new_head->next = *head;
  *head = new_head;
}

int main() {
  struct Node *head = newNode(5);
  head->next = newNode(10);
  print(head);
  insertHead(&head, 7);
  print(head);
  return 0;
}
