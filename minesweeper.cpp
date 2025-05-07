#include <bits/stdc++.h>
using namespace std;

#define BEGINNER 0
#define INTERMEDIATE 1
#define ADVANCED 2
#define MAXSIDE 25
#define MAXMINES 99
#define MOVESIZE 526

int SIDE;
int MINES;

bool isValid(int row, int col)
{
    return (row >= 0) && (row < SIDE) && (col >= 0) && (col < SIDE);
}

class Board {
public:
    char** board;
    Board()
    {
        board = new char*[MAXSIDE + 1];
        for (int i = 0; i <= MAXSIDE; i++) {
            board[i] = new char[MAXSIDE + 1];
            for (int j = 0; j <= MAXSIDE; j++) {
                board[i][j] = '-';
            }
        }
    }

    bool isMine(int row, int col)
    {
        return (board[row][col] == '*');
    }

    void makeMove(int* x, int* y, int moves[][2], int currentMoveIndex)
    {
        *x = moves[currentMoveIndex][0];
        *y = moves[currentMoveIndex][1];
        printf("\nMy move is (%d, %d)\n", *x, *y);
    }

    void assignMoves(int moves[][2], int movesLeft)
    {
        bool mark[MAXSIDE * MAXSIDE];
        memset(mark, false, sizeof(mark));

        for (int i = 0; i < movesLeft;) {
            int random = rand() % (SIDE * SIDE);
            int x = random / SIDE;
            int y = random % SIDE;

            if (mark[random] == false) {
                moves[i][0] = x;
                moves[i][1] = y;
                mark[random] = true;
                i++;
            }
        }
    }

    void printBoard()
    {
        int i, j;
        printf("    ");
        for (i = 0; i < SIDE; i++)
            printf("%d ", i);
        printf("\n\n");

        for (i = 0; i < SIDE; i++) {
            printf("%d   ", i);
            for (j = 0; j < SIDE; j++)
                printf("%c ", board[i][j]);
            printf("\n");
        }
    }

    int countAdjacentMines(int row, int col, int mines[][2])
    {
        int i, count = 0;
        int dx[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
        int dy[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };

        for (int d = 0; d < 8; d++) {
            int newRow = row + dx[d];
            int newCol = col + dy[d];

            if (isValid(newRow, newCol)) {
                if (isMine(newRow, newCol))
                    count++;
            }
        }
        return (count);
    }

    void placeMines(int mines[][2])
    {
        bool mark[MAXSIDE * MAXSIDE];
        memset(mark, false, sizeof(mark));

        for (int i = 0; i < MINES;) {
            int random = rand() % (SIDE * SIDE);
            int x = random / SIDE;
            int y = random % SIDE;

            if (mark[random] == false) {
                mines[i][0] = x;
                mines[i][1] = y;
                board[mines[i][0]][mines[i][1]] = '*';
                mark[random] = true;
                i++;
            }
        }
    }

    void replaceMine(int row, int col)
    {
        for (int i = 0; i < SIDE; i++) {
            for (int j = 0; j < SIDE; j++) {
                if (board[i][j] != '*') {
                    board[i][j] = '*';
                    board[row][col] = '-';
                    return;
                }
            }
        }
    }
};

class Game {
public:
    bool playMinesweeperUtil(Board& myBoard, Board& realBoard, int mines[][2], int row, int col, int* movesLeft)
    {
        if (myBoard.board[row][col] != '-')
            return (false);

        int i, j;
        if (realBoard.board[row][col] == '*') {
            myBoard.board[row][col] = '*';
            for (i = 0; i < MINES; i++)
                myBoard.board[mines[i][0]][mines[i][1]] = '*';

            myBoard.printBoard();
            printf("\nYou lost!\n");
            return (true);
        }
        else {
            int count = realBoard.countAdjacentMines(row, col, mines);
            (*movesLeft)--;

            myBoard.board[row][col] = count + '0';

            if (!count) {
                int dx[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
                int dy[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };

                for (int d = 0; d < 8; d++) {
                    int newRow = row + dx[d];
                    int newCol = col + dy[d];

                    if (isValid(newRow, newCol)) {
                        if (realBoard.isMine(newRow, newCol) == false)
                            playMinesweeperUtil(myBoard, realBoard, mines, newRow, newCol, movesLeft);
                    }
                }
            }
            return (false);
        }
    }

    void cheatMinesweeper(Board& realBoard)
    {
        printf("The mines locations are-\n");
        realBoard.printBoard();
    }

    void playMinesweeper(Board& realBoard, Board& myBoard)
    {
        bool gameOver = false;
        int movesLeft = SIDE * SIDE - MINES, x, y;
        int mines[MAXMINES][2];
        int moves[MOVESIZE][2];

        realBoard.placeMines(mines);

        myBoard.assignMoves(moves, movesLeft);

        int currentMoveIndex = 0;
        while (gameOver == false) {
            printf("Current Status of Board : \n");
            myBoard.printBoard();
            myBoard.makeMove(&x, &y, moves, currentMoveIndex);

            if (currentMoveIndex == 0) {
                if (realBoard.isMine(x, y) == true)
                    realBoard.replaceMine(x, y);
            }

            currentMoveIndex++;

            gameOver = playMinesweeperUtil(myBoard, realBoard, mines, x, y, &movesLeft);

            if ((gameOver == false) && (movesLeft == 0)) {
                printf("\nYou won !\n");
                gameOver = true;
            }
        }
    }
};

void chooseDifficultyLevel()
{
    int level;
    printf("Enter the Difficulty Level\n");
    printf("Press 0 for BEGINNER (9 * 9 Cells and 10 Mines)\n");
    printf("Press 1 for INTERMEDIATE (16 * 16 Cells and 40 Mines)\n");
    printf("Press 2 for ADVANCED (24 * 24 Cells and 99 Mines)\n");
    scanf("%d", &level);

    if (level == BEGINNER) {
        SIDE = 9;
        MINES = 10;
    }

    if (level == INTERMEDIATE) {
        SIDE = 16;
        MINES = 40;
    }

    if (level == ADVANCED) {
        SIDE = 24;
        MINES = 99;
    }
}

int main()
{
    Board myBoard, realBoard;
    Game* game = new Game();
    chooseDifficultyLevel();
    game->playMinesweeper(myBoard, realBoard);
    return 0;
}
