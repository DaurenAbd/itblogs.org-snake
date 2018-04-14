#include <iostream>

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
    #define CLEAR "cls"
#else //In any other OS
    #include "console.h"
    #define CLEAR "clear"
#endif

using namespace std;

int snake_size, change_x, change_y, coordinates_x[1000], coordinates_y[1000], food_x = -1, food_y = -1;

char symbol, a[1000][1000];
const int N = 13, M = 17, INTERVAL = 200;

void change_direction() {
    symbol = getch();

    switch (symbol) {
        case 'w':
            if (change_x != 1 || change_y != 0) {
                change_x = -1;
                change_y = 0;
            }
            break;
        case 'a':
            if (change_x != 0 || change_y != 1) {
                change_x = 0;
                change_y = -1;
            }
            break;
        case 's':
            if (change_x != -1 || change_y != 0) {
                change_x = 1;
                change_y = 0;
            }
            break;
        case 'd':
            if (change_x != 0 || change_y != -1) {
                change_x = 0;
                change_y = 1;
            }
            break;
        case 'q':
            exit(0);
        default :
            break;
    }
}

void show_table() {
    system(CLEAR);

    for (int i = 0; i <= N + 1; ++i) {
        for (int j = 0; j <= M + 1; ++j) {
            cout << (i == 0 || j == 0 || i == N + 1 || j == M + 1 ? '#' : (a[i][j] == 0 ? ' ' : a[i][j]))
                 << (j <= M ? "" : "\n");
        }
    }
}

void clear_snake_on_table() {
    for (int i = 1; i <= snake_size; ++i) {
        a[coordinates_x[i]][coordinates_y[i]] = ' ';
    }
}

void show_snake_on_table() {
    if (change_x == 1 && change_y == 0) {
        a[coordinates_x[1]][coordinates_y[1]] = 'v';
    }
    if (change_x == -1 && change_y == 0) {
        a[coordinates_x[1]][coordinates_y[1]] = '^';
    }
    if (change_x == 0 && change_y == 1) {
        a[coordinates_x[1]][coordinates_y[1]] = '>';
    }
    if (change_x == 0 && change_y == -1) {
        a[coordinates_x[1]][coordinates_y[1]] = '<';
    }

    for (int i = 2; i <= snake_size; ++i) {
        a[coordinates_x[i]][coordinates_y[i]] = '@';
    }
}

bool game_over() {
    for (int i = 2; i <= snake_size; ++i) {
        if (coordinates_x[1] == coordinates_x[i] && coordinates_y[1] == coordinates_y[i]) {
            return true;
        }
    }
    return false;
}

void check_coordinates() {
    if (coordinates_x[1] > N) {
        coordinates_x[1] = 1;
    }
    if (coordinates_x[1] < 1) {
        coordinates_x[1] = N;
    }
    if (coordinates_y[1] > M) {
        coordinates_y[1] = 1;
    }
    if (coordinates_y[1] < 1) {
        coordinates_y[1] = M;
    }
}

void next_step() {
    clear_snake_on_table();

    for (int i = snake_size; i >= 2; --i) {
        coordinates_x[i] = coordinates_x[i - 1];
        coordinates_y[i] = coordinates_y[i - 1];
    }

    coordinates_x[1] += change_x;
    coordinates_y[1] += change_y;

    check_coordinates();

    if (coordinates_x[1] == food_x && coordinates_y[1] == food_y) {
        snake_size++;
        food_x = -1;
        food_y = -1;
    }

    show_snake_on_table();

    if (game_over()) {
        cout << "You're looser! \nPress any key to quit...";
        getch();
        exit(0);
    }
}

bool food_check() {
    return !(food_x == -1 && food_y == -1);
}

void place_food() {
    srand((unsigned int) time(nullptr));
    for (int i = 1; i <= 9; ++i) {
        auto x = (int) random();
        auto y = (int) random();
        if (x < 0) {
            x *= -1;
        }
        if (y < 0) {
            y *= -1;
        }
        x = x % N + 1;
        y = y % M + 1;
        if (a[x][y] != '@' && a[x][y] != 'v' && a[x][y] != '^' && a[x][y] != '<' && a[x][y] != '>') {
            food_x = x;
            food_y = y;
            a[x][y] = '+';
            return;
        }
    }
}

void standart_settings() {
    snake_size = 2;

    coordinates_x[1] = 1;
    coordinates_y[1] = 2;
    coordinates_x[2] = 1;
    coordinates_y[2] = 1;

    change_x = 0;
    change_y = 1;
}

int main() {
    standart_settings();

    while (1) {
        #ifdef _WIN32
            if (kbhit()) {
                change_direction();
            }
        #else //In any other OS
            enable_raw_mode();
            if (kbhit()) {
                change_direction();
            }

            disable_raw_mode();
            tcflush(0, TCIFLUSH);
        #endif

        next_step();

        if (!food_check()) {
            place_food();
        }

        show_table();

        #ifdef _WIN32
            Sleep(INTEVAL);
        #else //In any other OS
            usleep(INTERVAL * 1000);
        #endif
    }
}