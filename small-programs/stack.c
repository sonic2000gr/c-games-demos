#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_SIZE 10

bool is_empty(void);
bool is_full(void);
void push(int);
int pop(void);
int peek(void);

int stack[MAX_SIZE];
int top = -1;

bool is_empty(void) {
  return top == -1;
}

bool is_full(void) {
  return top == MAX_SIZE;
}

void push(int item) {
  if (!is_full()) {
    stack[++top] = item;
  }
}

int pop(void) {
  if (!is_empty())
    return stack[top--];
}

int peek(void) {
  if (!is_empty())
    return stack[top];
}

int main(void) {
 push(10);
 push(20);
 push(30);
 push(40);

 while (!is_empty())
    printf("%d\t", pop());
 printf("\n");

 return 0;
}
