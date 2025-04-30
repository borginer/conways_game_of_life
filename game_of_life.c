#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define BOARD_WIDTH 160
#define BOARD_HEIGHT 40
#define FPS 8

static char LIVE = '@';
static char DEAD = ' ';

int idx(int i, int j) { return i * BOARD_WIDTH + j; }

void print_board(char (*board)) {
    for (int i = 1; i < BOARD_HEIGHT - 1; i++) {
        for (int j = 1; j < BOARD_WIDTH - 1; j++) {
            putc(board[idx(i, j)], stdout);
        }
        printf("\n");
    }
}

int get_neighbors(char *board, int i, int j) {
    int neighbors = 0;

    for (int r = i - 1; r <= i + 1; r++) {
        for (int c = j - 1; c <= j + 1; c++) {
            if (board[idx(r, c)] == LIVE) {
                neighbors++;
            }
        }
    }

    if (board[idx(i, j)] == LIVE) {
        neighbors -= 1;
    }
    return neighbors;
}

char calc_evo(char *board, int i, int j) {
    int neighbors = get_neighbors(board, i, j);

    if (board[idx(i, j)] == LIVE) {
        if (neighbors < 2 || neighbors > 3) {
            return DEAD;
        } else {
            return LIVE;
        }
    } else { // DEAD
        if (neighbors == 3) {
            return LIVE;
        } else {
            return DEAD;
        }
    }
}

void clear_board_boarder(char *board) {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        board[idx(i, 0)] = DEAD;
        board[idx(i, BOARD_WIDTH - 1)] = DEAD;
    }
    for (int j = 0; j < BOARD_WIDTH; j++) {
        board[idx(0, j)] = DEAD;
        board[idx(BOARD_HEIGHT - 1, j)] = DEAD;
    }
}

void move(char *board) {
    char new_board[(BOARD_HEIGHT + 2) * (BOARD_WIDTH + 2)] = {};
    for (int i = 1; i < BOARD_HEIGHT + 1; i++) {
        for (int j = 1; j < BOARD_WIDTH + 1; j++) {
            new_board[idx(i, j)] = calc_evo(board, i, j);
        }
    }
    for (int i = 1; i < BOARD_HEIGHT + 1; i++) {
        for (int j = 1; j < BOARD_WIDTH + 1; j++) {
            board[idx(i, j)] = new_board[idx(i, j)];
        }
    }

    clear_board_boarder(board);
}

int main() {
    char board[(BOARD_HEIGHT + 2) * (BOARD_WIDTH + 2)] = {};
    memset(board, DEAD, (BOARD_WIDTH + 2) * (BOARD_HEIGHT + 2));

    board[idx(11, 60)] = LIVE;
    board[idx(12, 60)] = LIVE;
    board[idx(13, 60)] = LIVE;
    board[idx(12, 59)] = LIVE;
    board[idx(13, 61)] = LIVE;

    clock_t start;
    double rate = 1.0 / FPS;
    
    while (1) {
        start = clock();
        system("clear");
        print_board(board);
        move(board);
        while ((double)((clock() - start))/CLOCKS_PER_SEC < rate) {}
    }
}