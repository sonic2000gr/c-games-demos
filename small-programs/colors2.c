/*
   Using colors
   in Windows Console (terminal)
*/

#include <stdio.h>
#include <conio.h>
#include <windows.h>

int main()
{
  HANDLE  hConsole;
  int i;

  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

  for(i = 0; i < 16; i++)
  {
    SetConsoleTextAttribute(hConsole, i);
    printf("Hello World of Colours!\n");
  }

  getch();
  return 0;
}
