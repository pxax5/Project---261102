#include <SFML/Graphics.hpp>
#include <vector>
#include <stack>
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace sf;
using namespace std;

const int CELL_SIZE = 30;
const int MAZE_SIZE = 20;
const int WINDOW_SIZE = CELL_SIZE * MAZE_SIZE;
const int UI_HEIGHT = 100;

enum GameState { MENU, PLAY };

struct Cell {
    bool visited = false;
    bool walls[4] = {true, true, true, true};
};

void generateMaze(vector<vector<Cell>>& maze) {
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            maze[i][j].visited = false;
            for (int k = 0; k < 4; k++) maze[i][j].walls[k] = true;
        }
    }

    stack<pair<int, int>> s;
    int currX = 0, currY = 0;
    maze[currY][currX].visited = true;
    s.push({currX, currY});

    while (!s.empty()) {
        int x = s.top().first;
        int y = s.top().second;

        vector<int> neighbors;
        if (y > 0 && !maze[y - 1][x].visited) neighbors.push_back(0);
        if (x < MAZE_SIZE - 1 && !maze[y][x + 1].visited) neighbors.push_back(1);
        if (y < MAZE_SIZE - 1 && !maze[y + 1][x].visited) neighbors.push_back(2);
        if (x > 0 && !maze[y][x - 1].visited) neighbors.push_back(3);

        if (!neighbors.empty()) {
            int dir = neighbors[rand() % neighbors.size()];
            int nx = x, ny = y;

            if (dir == 0) { maze[y][x].walls[0] = false; ny--; maze[ny][nx].walls[2] = false; }
            if (dir == 1) { maze[y][x].walls[1] = false; nx++; maze[ny][nx].walls[3] = false; }
            if (dir == 2) { maze[y][x].walls[2] = false; ny++; maze[ny][nx].walls[0] = false; }
            if (dir == 3) { maze[y][x].walls[3] = false; nx--; maze[ny][nx].walls[1] = false; }

            maze[ny][nx].visited = true;
            s.push({nx, ny});
        }
        else s.pop();
    }
}

int main() {
    srand(time(0));

    RenderWindow window(VideoMode(WINDOW_SIZE, WINDOW_SIZE + UI_HEIGHT), "Maze Game");
    window.setFramerateLimit(60);

    Font font;
    if (!font.loadFromFile("Barriecito-Regular.ttf"))
        return -1;

    GameState state = MENU;

    RectangleShape startButton(Vector2f(220, 60));
    startButton.setPosition(WINDOW_SIZE / 2 - 110, WINDOW_SIZE / 2 - 30);
    startButton.setFillColor(Color::Blue);

    Text startText("START", font, 30);
    startText.setPosition(WINDOW_SIZE / 2 - 60, WINDOW_SIZE / 2 - 20);

    RectangleShape exitButton(Vector2f(220, 60));
    exitButton.setPosition(WINDOW_SIZE / 2 - 110, WINDOW_SIZE / 2 + 50);
    exitButton.setFillColor(Color::Red);

    Text exitText("EXIT", font, 30);
    exitText.setPosition(WINDOW_SIZE / 2 - 50, WINDOW_SIZE / 2 + 60);

    Text title("MAZE GAME", font, 40);
    title.setPosition(WINDOW_SIZE / 2 - 110, 100);

    RectangleShape restartButton(Vector2f(120, 40));
    restartButton.setPosition(WINDOW_SIZE - 140, WINDOW_SIZE + 30);
    restartButton.setFillColor(Color(70, 70, 70));
    restartButton.setOutlineThickness(2);
    restartButton.setOutlineColor(Color::White);

    Text restartText("RESTART", font, 18);
    restartText.setPosition(WINDOW_SIZE - 120, WINDOW_SIZE + 38);

    Text statsText("", font, 20);
    statsText.setPosition(20, WINDOW_SIZE + 10);

    Text winText("YOU WIN!", font, 60);
    winText.setFillColor(Color::Yellow);
    winText.setPosition(WINDOW_SIZE / 2 - 140, WINDOW_SIZE / 2 - 40);

    vector<vector<Cell>> maze(MAZE_SIZE, vector<Cell>(MAZE_SIZE));
    generateMaze(maze);

    int playerX = 0, playerY = 0, steps = 0;
    bool hasWon = false;
    Clock gameClock;
    Time finalTime;

    CircleShape player(CELL_SIZE / 3);
    player.setFillColor(Color::Green);

    RectangleShape goal(Vector2f(CELL_SIZE - 10, CELL_SIZE - 10));
    goal.setFillColor(Color::Red);
    goal.setPosition((MAZE_SIZE - 1) * CELL_SIZE + 5, (MAZE_SIZE - 1) * CELL_SIZE + 5);

    while (window.isOpen()) {

        Event event;
        while (window.pollEvent(event)) {

            if (event.type == Event::Closed)
                window.close();

            if (state == MENU && event.type == Event::MouseButtonPressed) {
                Vector2i mousePos = Mouse::getPosition(window);

                if (startButton.getGlobalBounds().contains(Vector2f(mousePos))) {
                    state = PLAY;
                    gameClock.restart();
                }

                if (exitButton.getGlobalBounds().contains(Vector2f(mousePos))) {
                    window.close();
                }
            }

            if (state == PLAY && event.type == Event::MouseButtonPressed) {
                Vector2i mousePos = Mouse::getPosition(window);
                if (restartButton.getGlobalBounds().contains(Vector2f(mousePos))) {
                    generateMaze(maze);
                    playerX = playerY = steps = 0;
                    hasWon = false;
                    gameClock.restart();
                }
            }

            if (state == PLAY && event.type == Event::KeyPressed) {

                if (event.key.code == Keyboard::Escape)
                    state = MENU;

                if (!hasWon) {
                    bool moved = false;

                    if (event.key.code == Keyboard::Up && !maze[playerY][playerX].walls[0]) { playerY--; moved = true; }
                    if (event.key.code == Keyboard::Right && !maze[playerY][playerX].walls[1]) { playerX++; moved = true; }
                    if (event.key.code == Keyboard::Down && !maze[playerY][playerX].walls[2]) { playerY++; moved = true; }
                    if (event.key.code == Keyboard::Left && !maze[playerY][playerX].walls[3]) { playerX--; moved = true; }

                    if (moved) steps++;
                }
            }
        }

        if (state == MENU) {
            Vector2i mousePos = Mouse::getPosition(window);

            startButton.setFillColor(
                startButton.getGlobalBounds().contains(Vector2f(mousePos)) ? Color::Green : Color::Blue
            );

            exitButton.setFillColor(
                exitButton.getGlobalBounds().contains(Vector2f(mousePos)) ? Color(200,0,0) : Color::Red
            );
        }

        if (playerX == MAZE_SIZE - 1 && playerY == MAZE_SIZE - 1 && !hasWon) {
            hasWon = true;
            finalTime = gameClock.getElapsedTime();
        }

        stringstream ss;
        float currentTime = hasWon ? finalTime.asSeconds() : gameClock.getElapsedTime().asSeconds();
        ss << "Steps: " << steps << "\nTime: " << fixed << setprecision(1) << currentTime << "s";
        statsText.setString(ss.str());

        window.clear(Color::Black);

        if (state == MENU) {
            window.draw(title);
            window.draw(startButton);
            window.draw(startText);
            window.draw(exitButton);
            window.draw(exitText);
        }

        if (state == PLAY) {

            for (int i = 0; i < MAZE_SIZE; i++) {
                for (int j = 0; j < MAZE_SIZE; j++) {
                    float x = j * CELL_SIZE;
                    float y = i * CELL_SIZE;

                    if (maze[i][j].walls[0]) {
                        Vertex line[2];
                        line[0].position = Vector2f(x, y);
                        line[1].position = Vector2f(x + CELL_SIZE, y);
                        window.draw(line, 2, Lines);
                    }
                    if (maze[i][j].walls[1]) {
                        Vertex line[2];
                        line[0].position = Vector2f(x + CELL_SIZE, y);
                        line[1].position = Vector2f(x + CELL_SIZE, y + CELL_SIZE);
                        window.draw(line, 2, Lines);
                    }
                    if (maze[i][j].walls[2]) {
                        Vertex line[2];
                        line[0].position = Vector2f(x, y + CELL_SIZE);
                        line[1].position = Vector2f(x + CELL_SIZE, y + CELL_SIZE);
                        window.draw(line, 2, Lines);
                    }
                    if (maze[i][j].walls[3]) {
                        Vertex line[2];
                        line[0].position = Vector2f(x, y);
                        line[1].position = Vector2f(x, y + CELL_SIZE);
                        window.draw(line, 2, Lines);
                    }
                }
            }

            player.setPosition(playerX * CELL_SIZE + 5, playerY * CELL_SIZE + 5);

            window.draw(goal);
            window.draw(player);
            window.draw(statsText);
            window.draw(restartButton);
            window.draw(restartText);

            if (hasWon)
                window.draw(winText);
        }

        window.display();
    }
}