#include <iostream>
#include <vector>
#include <conio.h> 

using namespace std;

const int WIDTH = 10;
const int HEIGHT = 10;

struct Point {
    int x, y;
};

char maze[HEIGHT][WIDTH] = {
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
    {'#', 'P', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#'},
    {'#', '#', '#', ' ', '#', ' ', '#', '#', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#'},
    {'#', ' ', '#', '#', '#', '#', ' ', '#', ' ', '#'},
    {'#', ' ', '#', ' ', ' ', ' ', ' ', '#', ' ', '#'},
    {'#', ' ', '#', ' ', '#', '#', '#', '#', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', '#', '#', '#', ' ', '#', '#', 'E', '#'},
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}
};

Point player = {1, 1};
int steps = 0; 

void drawMaze() {
    system("cls"); 
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == player.y && j == player.x)
                cout << '@';
            else
                cout << maze[i][j];
        }
        cout << endl;
    }
    cout << "-----------------------" << endl;
    cout << "Steps: " << steps << endl; 
    cout << "Use W,A,S,D to move." << endl;
    cout << "-----------------------" << endl;
}

int main() {
    char input;
    bool win = false;

    while (!win) {
        drawMaze();
        input = _getch();

        int nextX = player.x;
        int nextY = player.y;

        if (input == 'w' || input == 'W') nextY--;
        else if (input == 's' || input == 'S') nextY++;
        else if (input == 'a' || input == 'A') nextX--;
        else if (input == 'd' || input == 'D') nextX++;

        if (maze[nextY][nextX] != '#') {
            player.x = nextX;
            player.y = nextY;
            steps++; 
        }

        if (maze[player.y][player.x] == 'E') {
            win = true;
        }
    }

    drawMaze();
    cout << "Congratulations! You found the exit in " << steps << " steps!" << endl;

    return 0;
}