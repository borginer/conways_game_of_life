#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define BOARD_WIDTH 120
#define BOARD_HEIGHT 36
#define FPS 22

static char LIVE = '@';
static char DEAD = ' ';
static char CLEAR_TERM_ANSI[] = "\033[H\033[J";

int idx(int i, int j) { return i * BOARD_WIDTH + j; }

void printBoard(char(*board)) {
    write(STDOUT_FILENO, CLEAR_TERM_ANSI, sizeof(CLEAR_TERM_ANSI));
    char boardStr[BOARD_HEIGHT * (BOARD_WIDTH + 1)];

    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            boardStr[i * (BOARD_WIDTH + 1) + j] = board[idx(i, j)];
        }
        boardStr[i * (BOARD_WIDTH + 1) + BOARD_WIDTH] = '\n';
    }

    write(STDOUT_FILENO, boardStr, BOARD_HEIGHT * (BOARD_WIDTH + 1));
}

int countNeighbors(char *board, int i, int j) {
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

char clacEvo(char *board, int i, int j) {
    int neighbors = countNeighbors(board, i, j);

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

void move(char *board) {
    char new_board[(BOARD_HEIGHT + 2) * (BOARD_WIDTH + 2)] = {};
    for (int i = 1; i < BOARD_HEIGHT + 1; i++) {
        for (int j = 1; j < BOARD_WIDTH + 1; j++) {
            new_board[idx(i, j)] = clacEvo(board, i, j);
        }
    }
    for (int i = 1; i < BOARD_HEIGHT + 1; i++) {
        for (int j = 1; j < BOARD_WIDTH + 1; j++) {
            board[idx(i, j)] = new_board[idx(i, j)];
        }
    }
}

int main() {
    char board[BOARD_HEIGHT * BOARD_WIDTH] = {};
    memset(board, DEAD, BOARD_WIDTH * BOARD_HEIGHT);

    board[idx(11, 60)] = LIVE;
    board[idx(12, 60)] = LIVE;
    board[idx(13, 60)] = LIVE;
    board[idx(12, 59)] = LIVE;
    board[idx(13, 61)] = LIVE;

    clock_t start;
    double rate = 1.0 / FPS;

    while (1) {
        start = clock();
        printBoard(board);
        move(board);
        while ((double)((clock() - start)) / CLOCKS_PER_SEC < rate) {
        }
    }
}