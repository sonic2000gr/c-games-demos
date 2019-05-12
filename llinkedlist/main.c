#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node {
  int a;
  struct node* next;
} storage;

bool addList(int a, storage** head, storage** last)   {
  storage* current = NULL;

  current = malloc(sizeof(storage));
  if(current == NULL)
    return false;
  current->a = a;
  current->next = NULL;
  if(*head == NULL)  {
    *head = current;
    *last = *head;
    return true;
  }
  (*last)->next = current;
  *last = current;
  return true;
}

void deleteList(storage** head) {
    storage* current = *head;
    storage* next = NULL;

    while(current != NULL)  {
        next = current->next;
        printf("Deleting %d\n", current->a);
        free(current);
        current = next;
    }
    *head = NULL;
}

int main()  {
  storage* head = NULL;
  storage* last = NULL;
  storage* p = NULL;
  int a;

  for(a = 0; a < 100; a++)
    addList(a, &head, &last);

  p = head;
  while(p != NULL)  {
    printf("%d\n",p->a);
    p = p->next;
  }

  deleteList(&head);

  return 0;
}
