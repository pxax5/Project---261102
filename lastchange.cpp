#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cmath>

using namespace std;
using namespace sf;

const int CELL_SIZE = 25; 
const int MAP_W = 19;     
const int MAP_H = 11;     

const int originalMap[MAP_H][MAP_W] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,0,1},
    {1,0,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,0,1,1,0,1,1,2,1,1,0,1,1,0,1,1,1},
    {1,0,0,0,0,0,0,1,2,2,2,1,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1},
    {1,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,1},
    {1,0,1,1,0,0,0,1,1,1,1,1,0,0,0,1,1,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

int mapData[MAP_H][MAP_W];
float px, py, ex, ey;
int score, totalCoins;
bool isGameOver, isWin;
float gameTime;

void resetGame(Clock& totalTimeClock) {
    for(int i = 0; i < MAP_H; i++) 
        for(int j = 0; j < MAP_W; j++) 
            mapData[i][j] = originalMap[i][j];
    px = 9; py = 4; ex = 9; ey = 6;
    score = 0; gameTime = 0;
    isGameOver = false; isWin = false; totalCoins = 0;
    for(int i = 0; i < MAP_H; i++) 
        for(int j = 0; j < MAP_W; j++) 
            if(mapData[i][j] == 0) totalCoins++;
    totalTimeClock.restart();
}

int main() {
    RenderWindow window(VideoMode(MAP_W * CELL_SIZE, MAP_H * CELL_SIZE + 80), "Pac-Man Arial System");
    window.setFramerateLimit(60);

    Font font;
    bool fontLoaded = font.loadFromFile("arial.ttf");

    Text scoreText, timeText, statusText, retryHint;
    if (fontLoaded) {
        scoreText.setFont(font); scoreText.setCharacterSize(18); scoreText.setFillColor(Color::Yellow);
        timeText.setFont(font); timeText.setCharacterSize(18); timeText.setFillColor(Color::White);
        statusText.setFont(font); statusText.setCharacterSize(40); statusText.setStyle(Text::Bold);
        retryHint.setFont(font); retryHint.setCharacterSize(15); retryHint.setString("Click the box to Retry");
    }

    Clock inputClock, enemyClock, totalTimeClock;
    resetGame(totalTimeClock);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();
            if ((isGameOver || isWin) && event.type == Event::MouseButtonPressed) {
                Vector2i m = Mouse::getPosition(window);
                if (m.x >= 100 && m.x <= 380 && m.y >= 75 && m.y <= 175) resetGame(totalTimeClock);
            }
        }

        if (!isGameOver && !isWin) {
            gameTime = totalTimeClock.getElapsedTime().asSeconds();
            if (inputClock.getElapsedTime().asSeconds() > 0.12f) {
                int nx = px, ny = py;
                if (Keyboard::isKeyPressed(Keyboard::W)) ny--;
                else if (Keyboard::isKeyPressed(Keyboard::S)) ny++;
                else if (Keyboard::isKeyPressed(Keyboard::A)) nx--;
                else if (Keyboard::isKeyPressed(Keyboard::D)) nx++;
                if (mapData[ny][nx] != 1) { 
                    px = nx; py = ny; inputClock.restart(); 
                    if (mapData[(int)py][(int)px] == 0) {
                        mapData[(int)py][(int)px] = 2; score += 10; totalCoins--;
                        if (totalCoins <= 0) isWin = true;
                    }
                }
            }
            if (enemyClock.getElapsedTime().asSeconds() > 0.15f) {
                float dx = px - ex, dy = py - ey;
                if (abs(dx) > abs(dy)) {
                    if (dx > 0 && mapData[(int)ey][(int)ex + 1] != 1) ex++;
                    else if (dx < 0 && mapData[(int)ey][(int)ex - 1] != 1) ex--;
                } else {
                    if (dy > 0 && mapData[(int)ey + 1][(int)ex] != 1) ey++;
                    else if (dy < 0 && mapData[(int)ey - 1][(int)ex] != 1) ey--;
                }
                enemyClock.restart();
            }
            if ((int)px == (int)ex && (int)py == (int)ey) isGameOver = true;
        }

        window.clear(Color::Black);
        for (int i = 0; i < MAP_H; i++) {
            for (int j = 0; j < MAP_W; j++) {
                if (mapData[i][j] == 1) {
                    RectangleShape w(Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
                    w.setOutlineThickness(1); w.setOutlineColor(Color(33, 33, 255));
                    w.setFillColor(Color::Transparent); w.setPosition(j * CELL_SIZE + 1, i * CELL_SIZE + 1);
                    window.draw(w);
                } else if (mapData[i][j] == 0) {
                    CircleShape c(2); c.setFillColor(Color(255, 183, 174));
                    c.setPosition(j * CELL_SIZE + 11, i * CELL_SIZE + 11); window.draw(c);
                }
            }
        }

        CircleShape p(10); p.setFillColor(Color::Yellow); p.setPosition(px * CELL_SIZE + 2, py * CELL_SIZE + 2);
        CircleShape g(10); g.setFillColor(Color::Red); g.setPosition(ex * CELL_SIZE + 2, ey * CELL_SIZE + 2);
        window.draw(p); window.draw(g);

        if (fontLoaded) {
            scoreText.setString("Score: " + to_string(score));
            scoreText.setPosition(15, MAP_H * CELL_SIZE + 15);
            window.draw(scoreText);

            timeText.setString("Time: " + to_string((int)gameTime) + "s");
            timeText.setPosition(MAP_W * CELL_SIZE - 100, MAP_H * CELL_SIZE + 15);
            window.draw(timeText);

            if (isGameOver || isWin) {
                RectangleShape bg(Vector2f(280, 110));
                bg.setPosition(100, 70);
                bg.setFillColor(isWin ? Color(0, 150, 0, 235) : Color(150, 0, 0, 235));
                bg.setOutlineThickness(3); bg.setOutlineColor(Color::White);
                window.draw(bg);

                statusText.setString(isWin ? "YOU WIN!" : "GAME OVER");
                statusText.setFillColor(Color::White);
                statusText.setPosition(140, 80);
                window.draw(statusText);

                retryHint.setPosition(175, 140);
                window.draw(retryHint);
            }
        }
        window.display();
    }
    return 0;
}