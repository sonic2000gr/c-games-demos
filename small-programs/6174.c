#include <stdio.h>

void digits(int a, int *d)  {
  int i = 0;
  while(a != 0)   {
    d[i++] = a % 10;
    a = a / 10;
  }
}

void sort(int a, int *d)  {
  int changed = 1;
  int i,c;
  while(changed) {
    changed = 0;
    for(i = a - 1; i > 0; i--)
      if(d[i] < d[i-1])  {
        c = d[i];
        d[i] = d[i-1];
        d[i-1] = c;
        changed = 1;
      }
  }
}

int ascend(int a, int *d)  {



}

int main(void)  {
  int d[4];

  digits(7891,d);
  sort(4,d);
  for(int i = 0; i < 4; i++)
    printf("%d\n",d[i]);

  return 0;
}
