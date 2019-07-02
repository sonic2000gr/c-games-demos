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
  int p = 1;
  int r = 0;
  int i;
  for(i = a - 1; i >= 0; i--)  {
    r = r + d[i] * p;
    p = p * 10;
  }
  return r;
}

int descend(int a, int *d)  {
  int p = 1;
  int r = 0;
  int i;
  for(i = 0; i < a; i++)    {
    r = r + d[i] * p;
    p = p * 10;
  }
  return r;
}

int main(void)  {
  int d[4];
  int n = 0, a, e, f = 0;
  int t = 0;


  while(n < 1000 || n > 9998 || f == 0)  {
    printf("Dwse enan tetrapsifio arithmo: ");
    scanf("%d", &n);
    digits(n, d);
    sort(4, d);
    a = ascend(4, d);
    e = descend(4, d);
    f = e - a;
  }
  while(n != 6174)  {
    digits(n, d);
    sort(4, d);
    a = ascend(4, d);
    e = descend(4, d);
    f = e - a;
    printf("%d\n%d\n----------\n%d\n\n", e,a,f);
    if(f < 1000)
      f = f * 10;
    n = f;
    t++;
  }
  printf("Vrethike se: %d prospathies", t);

  return 0;
}
