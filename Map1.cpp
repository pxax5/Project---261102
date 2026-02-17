#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

std::vector<std::vector<int>> mapLayout = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,0,0,1,1,1,0,0,1,0,1},
    {1,0,1,0,0,0,0,0,0,1,0,0,1,0,1},
    {1,0,1,1,1,0,0,1,1,1,0,0,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

int main()
{
   sf::RenderWindow window(
    sf::VideoMode({800u, 600u}),
    "SFML Map Technique",
    sf::Style::Default
);

    float cellSize = 16.f;
    sf::RectangleShape wall({cellSize, cellSize});
    wall.setFillColor(sf::Color::Red);

    std::vector<sf::RectangleShape> walls;

    for (int y = 0; y < mapLayout.size(); y++) {
        for (int x = 0; x < mapLayout[y].size(); x++) {
            if (mapLayout[y][x] == 1) {
               wall.setPosition({ x * cellSize, y * cellSize });

                walls.push_back(wall);
            }
        }
    }

    sf::Texture mapTexture;
    if (!mapTexture.loadFromFile("map_design.png")) {
        std::cout << "Could not load map image\n";
    }

    sf::Sprite mapSprite(mapTexture);

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(mapSprite);

        for (auto& w : walls)
            window.draw(w);

        window.display();
    }

    return 0;
}