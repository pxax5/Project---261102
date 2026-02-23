#include <SFML/Graphics.hpp>
#include <vector>
#include <stack>
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;
using namespace sf;

const int CELL_SIZE = 30;
const int MAZE_SIZE = 20;
const int WINDOW_SIZE = CELL_SIZE * MAZE_SIZE;
const int UI_HEIGHT = 100;

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
        pair<int, int> curr = s.top();
        int x = curr.first, y = curr.second;
        vector<int> neighbors;
        if (y > 0 && !maze[y - 1][x].visited) neighbors.push_back(0);
        if (x < MAZE_SIZE - 1 && !maze[y][x + 1].visited) neighbors.push_back(1);
        if (y < MAZE_SIZE - 1 && !maze[y + 1][x].visited) neighbors.push_back(2);
        if (x > 0 && !maze[y][x - 1].visited) neighbors.push_back(3);
        if (!neighbors.empty()) {
            int next_dir = neighbors[rand() % neighbors.size()];
            int nx = x, ny = y;
            if (next_dir == 0) { maze[y][x].walls[0] = false; ny--; maze[ny][nx].walls[2] = false; }
            else if (next_dir == 1) { maze[y][x].walls[1] = false; nx++; maze[ny][nx].walls[3] = false; }
            else if (next_dir == 2) { maze[y][x].walls[2] = false; ny++; maze[ny][nx].walls[0] = false; }
            else if (next_dir == 3) { maze[y][x].walls[3] = false; nx--; maze[ny][nx].walls[1] = false; }
            maze[ny][nx].visited = true;
            s.push({nx, ny});
        } else s.pop();
    }
}

int main() {
    srand(time(0));
    RenderWindow window(VideoMode(WINDOW_SIZE, WINDOW_SIZE + UI_HEIGHT), "Maze Game Pro");
    window.setFramerateLimit(60);

    Font font;
    font.loadFromFile("arial.ttf");

    Text statsText;
    statsText.setFont(font);
    statsText.setCharacterSize(20);
    statsText.setFillColor(Color::White);
    statsText.setPosition(20, WINDOW_SIZE + 10);

    Text winText;
    winText.setFont(font);
    winText.setString("YOU WIN!");
    winText.setCharacterSize(60);
    winText.setFillColor(Color::Yellow);
    winText.setOutlineColor(Color::Black);
    winText.setOutlineThickness(3);
    FloatRect tw = winText.getLocalBounds();
    winText.setOrigin(tw.left + tw.width / 2.0f, tw.top + tw.height / 2.0f);
    winText.setPosition(WINDOW_SIZE / 2.0f, WINDOW_SIZE / 2.0f);

    RectangleShape restartButton(Vector2f(120, 40));
    restartButton.setFillColor(Color(70, 70, 70));
    restartButton.setOutlineThickness(2);
    restartButton.setOutlineColor(Color::White);
    restartButton.setPosition(WINDOW_SIZE - 140, WINDOW_SIZE + 30);

    Text btnText;
    btnText.setFont(font);
    btnText.setString("RESTART");
    btnText.setCharacterSize(18);
    btnText.setPosition(WINDOW_SIZE - 117, WINDOW_SIZE + 38);

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
            if (event.type == Event::Closed) window.close();
            if (event.type == Event::MouseButtonPressed) {
                if (restartButton.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y)) {
                    generateMaze(maze);
                    playerX = 0; playerY = 0; steps = 0; hasWon = false; gameClock.restart();
                }
            }
            if (!hasWon && event.type == Event::KeyPressed) {
                bool moved = false;
                if (event.key.code == Keyboard::Up && !maze[playerY][playerX].walls[0]) { playerY--; moved = true; }
                else if (event.key.code == Keyboard::Right && !maze[playerY][playerX].walls[1]) { playerX++; moved = true; }
                else if (event.key.code == Keyboard::Down && !maze[playerY][playerX].walls[2]) { playerY++; moved = true; }
                else if (event.key.code == Keyboard::Left && !maze[playerY][playerX].walls[3]) { playerX--; moved = true; }
                if (moved) steps++;
            }
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
        for (int i = 0; i < MAZE_SIZE; i++) {
            for (int j = 0; j < MAZE_SIZE; j++) {
                float x = j * CELL_SIZE, y = i * CELL_SIZE;
                if (maze[i][j].walls[0]) { Vertex l[] = {Vertex(Vector2f(x, y)), Vertex(Vector2f(x + CELL_SIZE, y))}; window.draw(l, 2, Lines); }
                if (maze[i][j].walls[1]) { Vertex l[] = {Vertex(Vector2f(x + CELL_SIZE, y)), Vertex(Vector2f(x + CELL_SIZE, y + CELL_SIZE))}; window.draw(l, 2, Lines); }
                if (maze[i][j].walls[2]) { Vertex l[] = {Vertex(Vector2f(x, y + CELL_SIZE)), Vertex(Vector2f(x + CELL_SIZE, y + CELL_SIZE))}; window.draw(l, 2, Lines); }
                if (maze[i][j].walls[3]) { Vertex l[] = {Vertex(Vector2f(x, y)), Vertex(Vector2f(x, y + CELL_SIZE))}; window.draw(l, 2, Lines); }
            }
        }
        player.setPosition(playerX * CELL_SIZE + 5, playerY * CELL_SIZE + 5);
        window.draw(goal); window.draw(player); window.draw(statsText);
        window.draw(restartButton); window.draw(btnText);
        if (hasWon) window.draw(winText);
        window.display();
    }
    return 0;
}