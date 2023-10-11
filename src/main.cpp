#include <iostream>
#include <SFML/Graphics.hpp>

void drawGrid(sf::RenderWindow &window, float lineWidth, float indentWidth, float gridLength, int gridSize)
{
    sf::RectangleShape line(sf::Vector2f(gridLength, lineWidth));

    for (int i=0; i <= gridSize; i++)
    {
        line.setPosition(0, gridLength/gridSize * i);
        line.move(indentWidth, indentWidth);
        window.draw(line);
    }

    for (int i=0; i <= gridSize; i++)
    {
        line.setRotation(90);
        line.setPosition(gridLength/gridSize * i, 0);
        line.move(indentWidth, indentWidth);
        window.draw(line);
    }
}

int main()
{

    const float windowLength = 800.0f;
    const float indentPercent = 0.05;
    const float indentWidth = windowLength * indentPercent;
    const float lineWidth = 1.0f;
    const float gridLength = windowLength - (2*indentWidth);
    const int gridSize = 32; 

    sf::RenderWindow window(sf::VideoMode(windowLength, windowLength), "Tilemap :)");

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

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            std::cout << sf::Mouse::getPosition(window).x << ", " << sf::Mouse::getPosition(window).y << "\n";
        }
        
        //draw grid
        drawGrid(window, lineWidth, indentWidth, gridLength, gridSize);

        window.display();
    }
}