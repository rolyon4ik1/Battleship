#include <stdio.h>
#include <stdlib.h>
#include "console.h"
#include <windows.h>
#include <time.h>
#include "conio.h"
////////////////////////////////////////////////////////////////////
///                           ANDRII                             ///
////////////////////////////////////////////////////////////////////
char halfBlock = 220;
char lightShade = 176; // ░
#define ROWS 5
#define COLS 5
#define LEFT_TOP_X 10
#define LEFT_TOP_Y 5
#define G_LEFT_TOP_X 50
#define G_LEFT_TOP_Y 5
int ship_x_position;
int ship_y_position;
int gx;
int gy;
int computerHits = 0; // Количество попаданий компьютера
int playerShips = 5;  // Общее количество кораблей игрока



int playerBoard[ROWS][COLS] = {0};  // Player's game board
int enemyBoard[ROWS][COLS] = {0}; // Enemy's game board
int computerAttacks[ROWS][COLS] = {0}; // 0 - не атаковано, 1 - атаковано

void clearTemporaryShips(int y, int x) {
    if (x >= 0 && x < ROWS && y >= 0 && y < COLS) {
        if (playerBoard[x][y] == 0) { // Только если позиция не занята
            gotoxy(y * 4 + LEFT_TOP_X, x * 2 + LEFT_TOP_Y);
            textbackground(BLUE);
            textcolor(BLUE);
            printf("%c %c", halfBlock, halfBlock);
            if (y == COLS - 1) {
                printf("\n");
            }
            textbackground(BLACK);
            textcolor(WHITE);
        }
    }
}

void drawTemporaryShips(int y, int x) {
    if (x >= 0 && x < ROWS && y >= 0 && y < COLS) {
            gotoxy(y * 4 + LEFT_TOP_X, x * 2 + LEFT_TOP_Y);
            textbackground(RED);
            textcolor(RED);
            printf("%c %c", halfBlock, halfBlock);
        if (y == COLS - 1) {
            printf("\n");
        }
            textbackground(BLACK);
            textcolor(WHITE);
    }
}

void drawTemporaryEnemyShips(int y, int x) {
    if (x >= 0 && x < ROWS && y >= 0 && y < COLS) {
        gotoxy(y * 4 + G_LEFT_TOP_X, x * 2 + G_LEFT_TOP_Y);

        if (enemyBoard[x][y] == 2) { // Попадание
            textbackground(GREEN);
            textcolor(GREEN);
        } else if (enemyBoard[x][y] == -1) { // Промах
            textbackground(WHITE);
            textcolor(WHITE);
        } else { // Неизвестное состояние
            textbackground(YELLOW);
            textcolor(YELLOW);
        }

        printf("%c %c", halfBlock, halfBlock);
        textbackground(BLACK);
        textcolor(WHITE);
    }
}

void clearTemporaryEnemyShips(int y, int x) {
    if (x >= 0 && x < ROWS && y >= 0 && y < COLS) {
        gotoxy(y * 4 + G_LEFT_TOP_X, x * 2 + G_LEFT_TOP_Y);

        if (enemyBoard[x][y] == 2) { // Попадание
            textbackground(GREEN);
            textcolor(GREEN);
        } else if (enemyBoard[x][y] == -1) { // Промах
            textbackground(WHITE);
            textcolor(WHITE);
        } else { // Пустая клетка
            textbackground(BLUE);
            textcolor(BLUE);
        }

        printf("%c %c", halfBlock, halfBlock);
        textbackground(BLACK);
        textcolor(WHITE);
    }
}

void drawShips(int y, int x, int *fehler) {
    if (x >= 0 && x < ROWS && y >= 0 && y < COLS) {
        if (playerBoard[x][y] == 0) {
            playerBoard[x][y] = 1;
            gotoxy(y * 4 + LEFT_TOP_X, x * 2 + LEFT_TOP_Y);
            textbackground(RED);
            textcolor(RED);
            printf("%c %c", halfBlock, halfBlock);
            if (
                    y == COLS - 1) {
                printf("\n");
            }
            *fehler = FALSE;

            textbackground(BLACK);
            textcolor(WHITE);
        } else {
            printf("Error 0x001, es ist bereits ein Schiff hier\n");
            *fehler = TRUE;
        }
    } else {
        printf("Error 0x002, weit über das Spielfeld hinaus\n");
        *fehler = TRUE;
    }
}

int isValidPlacement(int x, int y) {
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (x + i >= 0 && x + i < ROWS && y + j >= 0 && y + j < COLS) {
                if (enemyBoard[x + i][y + j] == 1) {
                    return 0; // Invalid placement
                }
            }
        }
    }
    return 1; // Valid placement
}

void drawEnemyShips() {
    srand(time(NULL));
    int shipCount = 5;
    int placedShips = 0;

    while (placedShips < shipCount) {
        gx = rand() % ROWS;
        gy = rand() % COLS;

        if (enemyBoard[gx][gy] == 0 && isValidPlacement(gx, gy)) {
            enemyBoard[gx][gy] = 1; // Только ставим корабль, не отображаем его
            placedShips++;
        }
    }
}

int attempts = 0;

void placeShips() {
    int fehler = 0;
    int maxFehlerzahl = 5;
    int isPlaced = 0;

    // Переменные для хранения предыдущих координат
    int prev_x;
    int prev_y;

    while (attempts < maxFehlerzahl) {
        if (_kbhit()) {
            char key = _getch();

            // Сохраняем предыдущие координаты
            prev_x = ship_x_position;
            prev_y = ship_y_position;

            switch (key) {
                case RIGHT: // Вправо
                    if (ship_x_position < COLS - 1) { // Проверка на выход за пределы
                        ship_x_position++;
                    }
                    break;
                case LEFT: // Влево
                    if (ship_x_position > 0) { // Проверка на выход за пределы
                        ship_x_position--;
                    }
                    break;
                case UP: // Вверх
                    if (ship_y_position > 0) { // Проверка на выход за пределы
                        ship_y_position--;
                    }
                    break;
                case DOWN: // Вниз
                    if (ship_y_position < ROWS - 1) { // Проверка на выход за пределы
                        ship_y_position++;
                    }
                    break;
                case 13: // Enter
                    drawShips(ship_x_position, ship_y_position, &fehler);
                    if (fehler == 0) {
                        attempts++;
                        isPlaced = 1;
                    }
                    break;
                default:
                    break;
            }

            if (prev_x != ship_x_position || prev_y != ship_y_position) {
                clearTemporaryShips(prev_x, prev_y); // Стираем предыдущую позицию
            }

            drawTemporaryShips(ship_x_position, ship_y_position); // Рисуем новую позицию

            gotoxy(5, 20);
            printf("Du bist jetzt hier: X = %d, Y = %d ", ship_x_position, ship_y_position);
        }

        if (isPlaced) {
            isPlaced = 0;
        }
    }

    printf("Du hast schon alle 5 Schiefen!\n");
}

void drawGameBoard() {
    gotoxy(LEFT_TOP_X, 2);
    printf("Player's Board\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            gotoxy(j * 4 + LEFT_TOP_X, i * 2 + LEFT_TOP_Y);
            textbackground(BLUE);
            textcolor(BLUE);
            printf("%c %c", halfBlock, halfBlock);
            if (j == COLS - 1) {
                printf("\n");
            }
        }
        textbackground(BLACK);
        textcolor(WHITE);
    }
    gotoxy(G_LEFT_TOP_X, 2);
    printf("Enemy Board\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            gotoxy(j * 4 + G_LEFT_TOP_X, i * 2 + G_LEFT_TOP_Y);
            textbackground(BLUE);
            textcolor(BLUE);
            printf("%c %c", halfBlock, halfBlock);
            if (j == COLS - 1) {
                printf("\n");
            }
        }
        textbackground(BLACK);
        textcolor(WHITE);
    }
}

void computerAttack() {
    int x, y;
    int validAttack = 0;

    while (!validAttack) {
        x = rand() % ROWS;
        y = rand() % COLS;

        if (computerAttacks[y][x] == 0) { // Проверяем, не атаковал ли компьютер эту клетку
            validAttack = 1;
            computerAttacks[y][x] = 1; // Отмечаем клетку как атакованную

            // Проверяем, попал ли компьютер
            if (playerBoard[y][x] == 1) {
                playerBoard[y][x] = 2; // 2 означает попадание
                gotoxy(x * 4 + LEFT_TOP_X, y * 2 + LEFT_TOP_Y);
                textbackground(GREEN);
                textcolor(GREEN);
                printf("%c %c", halfBlock, halfBlock);
                computerHits++; // Увеличиваем счетчик попаданий

                if (computerHits == playerShips) { // Если все корабли уничтожены
                    gotoxy(0, 20); // Переместим курсор для вывода сообщения
                    printf("Компьютер уничтожил все ваши корабли! Игра окончена.\n");
                    exit(0); // Завершаем программу
                }
            } else {
                playerBoard[y][x] = -1; // -1 означает промах
                gotoxy(x * 4 + LEFT_TOP_X, y * 2 + LEFT_TOP_Y);
                textbackground(WHITE);
                textcolor(WHITE);
                printf("%c %c", halfBlock, halfBlock);
            }

            textbackground(BLACK);
            textcolor(WHITE);
        }
    }
}

void SchiffeVersenken() {
    int hits = 0;       // Количество попаданий игрока
    int maxHits = 5;    // Максимальное количество попаданий
    int playerTurn = 1; // Флаг для чередования ходов (1 - ход игрока, 0 - ход компьютера)

    printf("Versenken Sie die Schiffe!\n");

    while (hits < maxHits) {
        if (playerTurn) { // Ход игрока
            if (_kbhit()) {
                char key = _getch();
                int prev_x = ship_x_position;
                int prev_y = ship_y_position;

                switch (key) {
                    case RIGHT: // Вправо
                        if (ship_x_position < COLS - 1) ship_x_position++;
                        break;
                    case LEFT: // Влево
                        if (ship_x_position > 0) ship_x_position--;
                        break;
                    case UP: // Вверх
                        if (ship_y_position > 0) ship_y_position--;
                        break;
                    case DOWN: // Вниз
                        if (ship_y_position < ROWS - 1) ship_y_position++;
                        break;
                    case 13: // Enter (атака)
                        if (enemyBoard[ship_y_position][ship_x_position] == 1) { // Попадание
                            enemyBoard[ship_y_position][ship_x_position] = 2; // Отмечаем попадание
                            gotoxy(ship_x_position * 4 + G_LEFT_TOP_X, ship_y_position * 2 + G_LEFT_TOP_Y);
                            textbackground(GREEN);
                            textcolor(GREEN);
                            printf("%c %c", halfBlock, halfBlock);
                            hits++;
                        } else if (enemyBoard[ship_y_position][ship_x_position] == 0) { // Промах
                            enemyBoard[ship_y_position][ship_x_position] = -1;
                            gotoxy(ship_x_position * 4 + G_LEFT_TOP_X, ship_y_position * 2 + G_LEFT_TOP_Y);
                            textbackground(WHITE);
                            textcolor(WHITE);
                            printf("%c %c", halfBlock, halfBlock);
                        }
                        playerTurn = 0; // Передача хода компьютеру
                        break;
                }

                if (prev_x != ship_x_position || prev_y != ship_y_position) {
                    clearTemporaryEnemyShips(prev_x, prev_y); // Восстанавливаем предыдущее состояние клетки
                }
                drawTemporaryEnemyShips(ship_x_position, ship_y_position); // Рисуем курсор в новом месте

            }
        } else { // Ход компьютера
            computerAttack();
            playerTurn = 1; // Передача хода игроку
        }
    }

    printf("Herzlichen Glückwunsch! Sie haben alle Schiffe versenkt.\n");
}

int main() {
    initConsole();
    clrscr();
    printf("Start");
    getch();

    drawGameBoard();
    drawEnemyShips();
    placeShips();
    SchiffeVersenken();

    getch();
    clrscr();
    closeConsole();
    return 0;
}
