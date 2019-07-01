#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>


void printStars(int a)  {
    for(int i = 0; i < a; i++)
        printf("*");

}

void printSpace(int s) {
    for(int i = 0; i < s; i++)
        printf(" ");
}

void printShape(int a)  {
    int i, p, s = 1;
    HANDLE  hConsole;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    p = a - 1;
    printSpace(p--);
    printStars(1);
    printf("\n");
    for(i = 0; i < a - 2; i++, p--) {
        SetConsoleTextAttribute(hConsole, rand() % 8 + 1 );
        printSpace(p);
        printStars(1);
        s = 2 * i + 1;
        printSpace(s);
        printStars(1);
        printf("\n");
    }
    printStars(2 * a - 1);
    printf("\n");
}


int main(void)  {
    srand(time(NULL));
    printShape(15);
    return 0;
}
