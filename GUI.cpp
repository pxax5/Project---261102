#include <SFML/Graphics.hpp>
using namespace sf;

enum GameState { MENU, PLAY };

int main()
{
    RenderWindow window(VideoMode(Vector2u(800, 600)), "Puzzle Game GUI");

    GameState state = MENU;

    Font font;
    if (!font.openFromFile("Barriecito-Regular.ttf"));
        return -1;

    RectangleShape startButton(Vector2f(200.f, 60.f));
    startButton.setPosition(Vector2f(300.f, 250.f));
    startButton.setFillColor(Color::Blue);

    Text startText(font, "Start Game", 24);
    startText.setPosition(Vector2f(330.f, 265.f));

    Text gameText(font, "GAME SCREEN", 40);
    gameText.setPosition(Vector2f(250.f, 250.f));

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();

            if (state == MENU && event->is<Event::MouseButtonPressed>())
            {
                Vector2i mousePos = Mouse::getPosition(window);
                if (startButton.getGlobalBounds().contains(Vector2f(mousePos)))
                {
                    state = PLAY;
                }
            }
        }

        if (state == MENU)
        {
            Vector2i mousePos = Mouse::getPosition(window);
            if (startButton.getGlobalBounds().contains(Vector2f(mousePos)))
                startButton.setFillColor(Color::Green);
            else
                startButton.setFillColor(Color::Blue);
        }

        window.clear(Color::Black);

        if (state == MENU)
        {
            window.draw(startButton);
            window.draw(startText);
        }
        else if (state == PLAY)
        {
            window.draw(gameText);
        }

        window.display();
    }
}
