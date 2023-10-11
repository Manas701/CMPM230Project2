#include <iostream>
#include <SFML/Graphics.hpp>

void drawGrid(sf::RenderWindow &window, float lineWidth, float gridLength, int gridSize)
{
    sf::RectangleShape line(sf::Vector2f(gridLength, lineWidth));

    for (int i=0; i <= gridSize + 1; i++)
    {
        line.setPosition(0, gridLength/gridSize * i);
        window.draw(line);
    }

    for (int i=0; i <= gridSize + 1; i++)
    {
        line.setRotation(90);
        line.setPosition(gridLength/gridSize * i, 0);
        window.draw(line);
    }
}

int main()
{

    const float windowHeight = 500.0f;
    const float windowLength = 500.0f;
    const float lineWidth = 1.0f;
    const float gridLength = 500.0f;
    const int gridSize = 32; 

    sf::RenderWindow window(sf::VideoMode(windowHeight, windowLength), "Tilemap :)");

    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear();

        //drawing stuff here
        
        //draw grid
        drawGrid(window, lineWidth, gridLength, gridSize);

        window.display();
    }
}