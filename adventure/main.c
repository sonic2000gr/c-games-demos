/*  The Adventure
    Original story:
    (c) 1985 Pixel Magazine
    This version:
    (c) 2013 - 2018 Manolis Kiagias
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ROOMS 13
#define LOSSROOMS 3

int checkwin(int, int);
int checkloss(int*, int);
void printdescription(const char *[], int);
int getinput(int [][4], int);

int checkwin(int winroom, int room) {
    return room == winroom;
}

int checkloss(int *loss, int room) {
    for (int i = 0; i < LOSSROOMS; i++ )
        if (room == loss[i])
            return 1;
    return 0;
}

void printdescription(const char *rooms[], int room) {
    if (room >=0 && room <= ROOMS)
        printf("%s\n", rooms[room]);
}

int getinput(int moves[][4], int room) {
    int direction;
    int valid = 0;
    const char *directions[] = { "Boreia", "Notia", "Anatolika", "Dytika" };

    printf("Dinates kiniseis:\n");
    for (int i = 0; i < 4; i++)
        if (moves[room][i] != -1)
            printf("%s = %d\t", directions[i], i);

    printf("\n");

    do {
        printf("Dose arithmo kinisis:");
        do {
            scanf("%d", &direction);
            if (direction < 0 || direction > 4)
                printf("Mono apo 0 os 4 parakalo!\n");
        } while (direction < 0 || direction >4);
        if (moves[room][direction]==-1)
            printf("Ayth h kinisi den einai dinati!\n");
        else
            valid = 1;
    } while (!valid);

    return direction;
}

int main() {
    /* current room */

    int room = 0;

    /* current direction */

    int direction;

    /* Textual description of each room */

    const char *rooms[] = { "Briskesai ston kipo. Pantou Skotadi",
                            "Briskesai sto mpanio. Akous thorivo",
                            "Briskesai sto hall. Paraligo na skontapseis. Skala Anatolika",
                            "Briskesai sthn apothiki. H atmosfaira einai apopniktiki. Skala dytika, monopati notia",
                            "Eftases sto kauistiko. Brikes ton patera sou",
                            "Briskesai sto saloni. Akougetai mousiki.",
                            "Briskesai sthn trapezaria. Ta panta einai anastata",
                            "Briskesai sto diadromo. Einai skoteina. Skala notia",
                            "Eisai sthn kouzina. Akous fones",
                            "Eisai ston pano diadromo. Pantou hsyxia. Skala boreia",
                            "Eisai sto domation tou aderfou sou. O aderfos sou se martyra!",
                            "Eisai sto domatio ton goneon sou. H mhtera sou se epiase!",
                            "Eisai sto domatio sou. Eisai asfalis."
                          };

    /* Possible moves from each room, for each direction. -1 indicates impossible move */

    int moves[][4] = {  { -1, 2, -1, -1 },
                        { -1, -1, 2, -1 },
                        { 0, 5, 3, 1 },
                        { -1, 8, -1, 2 },
                        { -1, 6, 5, -1 },
                        { 2, 8, -1, 4 },
                        { 4, 7, -1, -1 },
                        { 6, 9, 8, -1 },
                        { 5, -1, -1, 7 },
                        { 7, 11, 12, 10 },
                        { -1, -1, 9, -1 },
                        { 9, -1, -1, -1 },
                        { -1, -1, -1, 9 }
                    };

    /* Game is won when winroom is reached */

    int winroom = 12;

    /* Game is lost when one of these rooms is reached */

    int lossrooms[] = { 10, 11, 4 };

    int endgame = 0;
    do {
        printdescription(rooms, room);
        if (checkloss(lossrooms, room)) {
            printf("Exases! To paixnidi teleiose!");
            endgame = 1;
        } else if (checkwin(winroom, room)) {
            printf("Kerdises! To paixnidi teleiose!");
            endgame = 1;
        } else {
            direction = getinput(moves, room);
            room = moves[room][direction];
        }
    } while (!endgame);

    return 0;
}
