#include "raylib.h"
#include <iostream>
#include <limits>

const int SIZE = 3;
char board[SIZE][SIZE] = { {' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '} };
char currentMarker = 'X';
int currentPlayer = 1;
const int CELL_SIZE = 150;
bool isOnePlayer = true;
bool gameEnded = false;
std::string winnerMessage = "";

// Function to check winner
bool checkWinner() {
    for (int i = 0; i < SIZE; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ') return true;
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != ' ') return true;
    }
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ') return true;
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != ' ') return true;

    return false;
}

bool isBoardFull() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == ' ') return false;
        }
    }
    return true;
}

// Minimax algorithm for AI moves
int minimax(bool isMaximizing) {
    if (checkWinner()) return isMaximizing ? -1 : 1;
    if (isBoardFull()) return 0;

    int bestScore = isMaximizing ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = isMaximizing ? 'O' : 'X';
                int score = minimax(!isMaximizing);
                board[i][j] = ' ';

                bestScore = isMaximizing ? std::max(bestScore, score) : std::min(bestScore, score);
            }
        }
    }
    return bestScore;
}

void aiMove() {
    int bestScore = std::numeric_limits<int>::min();
    int bestMove[2] = { -1, -1 };

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = 'O';
                int score = minimax(false);
                board[i][j] = ' ';

                if (score > bestScore) {
                    bestScore = score;
                    bestMove[0] = i;
                    bestMove[1] = j;
                }
            }
        }
    }
    board[bestMove[0]][bestMove[1]] = 'O';
}

void resetGame() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = ' ';
        }
    }
    currentMarker = 'X';
    currentPlayer = 1;
    gameEnded = false;
    winnerMessage = "";
}

int main() {
    InitWindow(450, 450, "Tic Tac Toe - Minimax AI");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(LIGHTGRAY);

        if (!gameEnded) {
            // Draw grid
            for (int i = 1; i < SIZE; i++) {
                DrawLine(i * CELL_SIZE, 0, i * CELL_SIZE, 450, DARKGRAY);
                DrawLine(0, i * CELL_SIZE, 450, i * CELL_SIZE, DARKGRAY);
            }

            // Highlight cell under mouse
            int mouseX = GetMouseX() / CELL_SIZE;
            int mouseY = GetMouseY() / CELL_SIZE;
            if (mouseX >= 0 && mouseX < SIZE && mouseY >= 0 && mouseY < SIZE && board[mouseY][mouseX] == ' ') {
                DrawRectangle(mouseX * CELL_SIZE, mouseY * CELL_SIZE, CELL_SIZE, CELL_SIZE, Fade(SKYBLUE, 0.3f));
            }

            // Handle mouse click
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                int x = GetMouseX() / CELL_SIZE;
                int y = GetMouseY() / CELL_SIZE;

                if (x >= 0 && x < SIZE && y >= 0 && y < SIZE && board[y][x] == ' ') {
                    board[y][x] = 'X';

                    if (checkWinner()) {
                        winnerMessage = "Player wins!";
                        gameEnded = true;
                    }
                    else if (isBoardFull()) {
                        winnerMessage = "It's a draw!";
                        gameEnded = true;
                    }
                    else {
                        aiMove();
                        if (checkWinner()) {
                            winnerMessage = "AI wins!";
                            gameEnded = true;
                        }
                    }
                }
            }

            // Draw markers
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    if (board[i][j] == 'X') {
                        DrawText("X", j * CELL_SIZE + 50, i * CELL_SIZE + 30, 80, RED);
                    }
                    else if (board[i][j] == 'O') {
                        DrawText("O", j * CELL_SIZE + 50, i * CELL_SIZE + 30, 80, BLUE);
                    }
                }
            }
        }
        else {
            // Display winner message
            DrawText(winnerMessage.c_str(), 100, 200, 30, GREEN);
            DrawText("Press R to Restart", 100, 300, 20, DARKGRAY);

            if (IsKeyPressed(KEY_R)) {
                resetGame();
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}