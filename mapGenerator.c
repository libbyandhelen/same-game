#include <stdio.h>
#include <stdlib.h>

int main(void) {
    FILE *fptr;
    int seed, count, i, j, n;
    int board[7][8];
    printf("seed:");
    scanf("%d", &seed);
    srand(seed);
    count = 0;
    while (count == 0){
        for (i = 0; i<7; i++){
            for (j = 0; j<8; j++){
                n = rand() % 3;
                board[i][j] = n;
            }
        }
        for (i = 1; i<7; i++){
            for (j = 1; j<8; j++){
                if (i - 1 >= 0){
                    if (board[i][j] == board[i - 1][j])
                        count++;
                }
                if (i + 1 <= 6){
                    if (board[i][j] == board[i + 1][j])
                        count++;
                }
                if (j - 1 >= 0){
                    if (board[i][j] == board[i][j - 1])
                        count++;
                }
                if (j + 1 <= 7){
                    if (board[i][j] == board[i][j + 1])
                        count++;
                }
            }
        }
    }
    fptr = fopen("map.txt", "w");
    for (i = 0; i<7; i++){
        for (j = 0; j<8; j++){
            if (j == 7){
                fprintf(fptr, "%d\n", board[i][j]);
            }
            else
                fprintf(fptr, "%d ", board[i][j]);
        }
    }
    fclose(fptr);
    printf("map.txt is generated\n");
    return 0;
}

