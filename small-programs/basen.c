#include <stdio.h>

void swapi(int *, int *);
void swapc(char *, char *);
int dec2bin(int, int *);
int dec2hex(int, char *);

void swapi(int *a, int *b)  {
  int c;
  c = *a;
  *a = *b;
  *b = c;
}

void swapc(char *a, char *b)  {
  int c;
  c = *a;
  *a = *b;
  *b = c;
}

int dec2bin(int a, int *d)  {
  int i = 0;

  while(a != 0)  {
    d[i++] = a % 2;
    a = a / 2;
  }

  for(int j = 0; j < i / 2; j++)
    swapi(&d[j], &d[i - 1 - j]);

  return i;
}

int dec2hex(int a , char *d)  {
  const char t[] = {'0','1','2','3','4','5','6','7','8','9',
              'A','B','C','D','E','F'};
  int i = 0;

  while(a != 0)  {
    d[i++] = t[a % 16];
    a = a / 16;
  }

  for(int j = 0; j < i / 2; j++)
    swapc(&d[j], &d[i - 1 - j]);

  d[i] = '\0';

  return i;
}

int main(void)  {
  int d[16];
  char c[16];
  int k = dec2bin(1420, d);
  dec2hex(1420, c);

  for(int i = 0; i < k; i++)
    printf("%d", d[i]);
  printf("\n");

  printf("%s", c);
  return 0;
}
