#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>

bool isprime(uint64_t n) {
uint64_t i = 3;
bool done = false;

if(n == 1)
    return false;
if(n == 2)
    return true;
if(n % 2 == 0)
    return false;
while(i <= sqrt(n) && !done)    {
    if(n % i == 0)
        done = true;
    else
        i = i+2;
}
    return !done;
}

void factor(uint32_t n) {
 uint64_t i = 2;
 while (n != 1) {
  if (n % i == 0) {
    printf("%d\t",i);
    n = n / i;
  }  else
   i += 1;
 }
}


int main(void)  {
for (uint64_t i = 2; i <= 100; i++)
    if (isprime(i))
      if (isprime(pow(2,i)-1))
        printf("%d\t",i);
printf("\n");
factor(176389);
return 0;
}
