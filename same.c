#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// This function reads from input "board" variable. 
// Note that the "board" variable is coming from the main function. 
// Then, prints the board as a matrix.

void print_board(int board[7][8]) {
    int i, j;
    for (i = 0; i < 8; i++){
        if (i == 0)
            printf(" ");
        else
            printf("%d", i - 1);
        for (j = 0; j < 8; j++){
            if (i == 0)
                printf(" %d", j);
            else
                printf(" %c", board[i-1][j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("1. Play / 2. Quit [1-2]?\n");
    return;
}

// This function reads from the file "map.txt".
//
// Then, it stores the information in the "map.txt" into the
// input variable "board".
//
//
//  map.txt                "board"
//  -----------            -----------
//  Character 0    -->>    Character a
//  Character 1    -->>    Character b
//  Character 2    -->>    Character c

void read_board(int board[7][8], int memory[7][8][10]) {
    FILE *f = fopen("map.txt", "r");
    int i, j;
    for (i = 0; i < 7; i++){
        for (j = 0; j < 8; j++){
            fscanf(f, "%d", &board[i][j]);
            if (board[i][j] == 0)
                board[i][j] = 'a';
            if (board[i][j] == 1)
                board[i][j] = 'b';
            if (board[i][j] == 2)
                board[i][j] = 'c';
        }
    }
    for (i = 0; i < 7; i++){
        for (j = 0; j < 8; j++){
            memory[i][j][0] = board[i][j];
        }
    }
    fclose(f);
    return;
}

int ask_row(){
    int R;
    printf("Row [0-6]?\n");
    scanf("%d", &R);
    while (R > 6 || R < 0){
        printf("Invalid row input. Try again.\n");
        printf("Row [0-6]?\n");
        scanf("%d", &R);
    }
    return R;
}
int ask_column(){
    int C;
    printf("Column [0-7]?\n");
    scanf("%d", &C);
    while (C > 7 || C < 0){
        printf("Invalid column input. Try again.\n");
        printf("Column [0-7]?\n");
        scanf("%d", &C);
    }
    return C;
}
int valid(int board[7][8], int R, int C){
    int k = 0;
    while (board[R][C] == '.'){
        printf("Invalid location: (%d,%d) is empty. Try again.\n", R, C);
        R = ask_row();
        C = ask_column();
    }
    if (R - 1 >= 0){
        if (board[R][C] == board[R - 1][C])
            k++;
    }
    if (R + 1 <= 6){
        if (board[R][C] == board[R + 1][C])
            k++;
    }
    if (C - 1 >= 0){
        if (board[R][C] == board[R][C - 1])
            k++;
    }
    if (C + 1 <= 7){
        if (board[R][C] == board[R][C + 1])
            k++;
    }
    while (k == 0){
        printf("Invalid location: (%d,%d) has no neighbor. Try again.\n", R, C);
        R = ask_row();
        C = ask_column();
        if (R - 1 >= 0){
            if (board[R][C] == board[R - 1][C])
                k++;
        }
        if (R + 1 <= 6){
            if (board[R][C] == board[R + 1][C])
                k++;
        }
        if (C - 1 >= 0){
            if (board[R][C] == board[R][C - 1])
                k++;
        }
        if (C + 1 <= 7){
            if (board[R][C] == board[R][C + 1])
                k++;
        }
    }
    return 10*R+C;
}

// "cell_removal" is the recursive function to change the set of selected
// cells to empty cells.
//
// "board" is passed from the main function, and should be
// updated by this function.
// 
// "start_row" and "start_col" specify the location of the selected cell.

void cell_removal(int board[7][8], int x, int y) {
    int tmp;
    tmp = board[x][y];
    if (board[x][y] == '.')
        return;
    board[x][y] = '.';
    if (x - 1 >= 0){
        if (tmp == board[x - 1][y])
            cell_removal(board, x - 1, y);
    }
    if (y - 1 >= 0){
        if (tmp == board[x][y - 1])
            cell_removal(board, x, y - 1);
    }
    if (x + 1 <= 6){
        if (tmp == board[x + 1][y])
            cell_removal(board, x + 1, y);
    }
    if (y + 1 <= 7){
        if (tmp == board[x][y + 1])
            cell_removal(board, x, y + 1);
    }
    return;
}

// "pull_down" updates the "board" variable passed from the main function.
//
// The job of this function is to fill the vertical gaps after the call to
// the "cell_removal" function.
//
// This function may update the "board" variable.

void pull_down(int board[7][8]) {
    int i, j, tmp, n;
    for (j = 0; j < 8; j++){
        n = 1;
        while (n != 0){
            for (i = 6; i > 0; i--){
                if (board[i][j] == '.'&&board[i - 1][j] != '.'){
                    tmp = board[i][j];
                    board[i][j] = board[i - 1][j];
                    board[i - 1][j] = tmp;
                }
            }
            n = 0;
            for (i = 6; i > 0; i--){
                if (board[i][j] == '.'&&board[i - 1][j] != '.'){
                    n++;
                }
            }
        }
    }
    return;
}

// "pull_left" updates the "board" variable passed from the main function.
//
// The job of this function is to fill the empty columns after the call to
// the "cell_removal" and the "pull_down" functions.
//
// This function may update the "board" variable.

void pull_left(int board[7][8]) {
    int A[8] = { 0 }, i, j, tmp, n;
    for (j = 0; j < 8; j++){
        for (i = 0; i < 7; i++){
            if (board[i][j] != '.'){
                A[j]++;
            }
        }
    }
    n = 1;
    while (n != 0){
        for (j = 0; j < 7; j++){
            if (A[j] == 0 && A[j + 1] != 0){
                for (i = 0; i < 7; i++){
                    tmp = board[i][j];
                    board[i][j] = board[i][j + 1];
                    board[i][j + 1] = tmp;
                    tmp = A[j];
                    A[j] = A[j + 1];
                    A[j + 1] = tmp;
                }
            }
        }
        n = 0;
        for (j = 0; j < 7; j++){
            if (A[j] == 0 && A[j + 1] != 0){
                n++;
            }
        }
    }
    return;
}

// "ask_for_command" is a function that returns the command which is
// correctly read from the player.

int ask_for_command(void) {
    int command = 0;
    scanf("%d", &command);
    while (command != 1 && command != 2){
        printf("Command should be 1 or 2. Try again.\n");
        printf("1. Play / 2. Quit [1-2]?\n");
        scanf("%d", &command);
    }
    return command;
}

int win(int board[7][8]){
    int i, j, n = 0;
    for (i = 0; i < 7; i++){
        for (j = 0; j < 8; j++){
            if (board[i][j] != '.')
                n++;
        }
    }
    return n;
}

int lose(int board[7][8]){
    int i, j, n = 0;
    for (i = 0; i < 7; i++){
        for (j = 0; j < 8; j++){
            if (board[i][j] != '.'){
                if (i - 1 >= 0){
                    if (board[i][j] == board[i - 1][j])
                        n++;
                }
                if (j - 1 >= 0){
                    if (board[i][j] == board[i][j - 1])
                        n++;
                }
                if (i + 1 <= 6){
                    if (board[i][j] == board[i + 1][j])
                        n++;
                }
                if (j + 1 <= 7){
                    if (board[i][j] == board[i][j + 1])
                        n++;
                }
            }
        }
    }
    if (n == 0)
        return 0;
    return 1;
}
void take_memory(int board[7][8], int memory[7][8][10]){
    int i, j, k, n = 0, m = 0;
    for (k = 9; k >= 0; k--){
        for (i = 0; i < 7; i++){
            for (j = 0; j < 8; j++){
                if (memory[i][j][k] == 0)
                    n++;
            }
        }
        if (n == 56){
            m++;
            n = 0;
        }
    }
    if (m != 0){
        for (i = 0; i < 7; i++){
            for (j = 0; j < 8; j++){
                memory[i][j][10 - m] = board[i][j];
            }
        }
    }
    if (m == 0){
        for (k = 1; k < 10; k++){
            for (i = 0; i < 7; i++){
                for (j = 0; j < 8; j++){
                    memory[i][j][k - 1] = memory[i][j][k];
                }
            }
        }
        for (i = 0; i < 7; i++){
            for (j = 0; j < 8; j++){
                memory[i][j][9] = board[i][j];
            }
        }
    }
    return;
}

void go_back(int board[7][8], int memory[7][8][10]){
    int input, i, j, k, count = 0, n,gobacknumber = 0;
    printf("1. Go back / 2. Continue [1-2]?\n");
    scanf("%d", &input);
    while (input != 1 && input != 2){
        printf("The input should be 1 or 2. Try again.\n");
        printf("1. Go back / 2. Continue [1-2]?\n");
        scanf("%d", &input);
    }
    if (input == 1)
        gobacknumber++;
    while (input == 1){
        for (k = 9; k >= 0; k--){
            for (i = 0; i < 7; i++){
                for (j = 0; j < 8; j++){
                    if (board[i][j] == memory[i][j][k])
                        count++;
                }
            }
            if (count == 56){
                count = 0;
                n = k;
                break;
            }
        }
        if (n -(2*gobacknumber-1) >= 0){
            for (i = 0; i < 7; i++){
                for (j = 0; j < 8; j++){
                    board[i][j] = memory[i][j][n -(2*gobacknumber-1)];
                }
            }
            for (i = 0; i < 8; i++){
                if (i == 0)
                    printf(" ");
                else
                    printf("%d", i - 1);
                for (j = 0; j < 8; j++){
                    if (i == 0)
                        printf(" %d", j);
                    else
                        printf(" %c", board[i - 1][j]);
                }
                printf("\n");
            }
            printf("\n");
            take_memory(board, memory);
        }
        else{
            printf("Sorry, you cannot go back any more.\n");
            break;
        }
        printf("1. Go back / 2. Continue [1-2]?\n");
        scanf("%d", &input);
        while (input != 1 && input != 2){
            printf("The input should be 1 or 2. Try again.\n");
            printf("1. Go back / 2. Continue [1-2]?\n");
            scanf("%d", &input);
        }
        if (input == 1)
            gobacknumber++;
    }
    return;
}

int main(void) {
    int board[7][8], memory[7][8][10] = {0};
    int command = -1, row = -1, column = -1, x, y, z;

    read_board(board, memory);

    while(1) {
        print_board(board);
        command = ask_for_command();
        if (command == 2)
            break;
        go_back(board, memory);
        x = ask_row();
        y = ask_column();
        z = valid(board, x, y);
        row = z / 10;
        column = z % 10;

        cell_removal(board, row, column);
        pull_down(board);
        pull_left(board);
        take_memory(board, memory);

        if (win(board) == 0){
            printf("You win\n");
            break;
        }
        if (lose(board) == 0){
            printf("You lose\n");
            break;
        }
    }

    return 0;
}

