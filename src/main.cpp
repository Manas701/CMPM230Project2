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

std::tuple<int, int> getTilePosition(sf::RenderWindow &window, float windowLength, float indentWidth, float gridLength, int gridSize, float squareLength)
{
    float gridPosX = sf::Mouse::getPosition(window).x - indentWidth;
    float gridPosY = sf::Mouse::getPosition(window).y - indentWidth;
    // if mouse is not within grid, don't give a tile position
    if (gridPosX < 0 || gridPosX > gridLength || gridPosY < 0 || gridPosY > gridLength)
    {
        return std::make_tuple(-1,-1);
    }
    int xSquare = gridPosX / squareLength;
    int ySquare = gridPosY / squareLength;
    std::cout << "Square Pos: (" << xSquare << ", " << ySquare << ")\n";
    return std::make_tuple(xSquare, ySquare);
}

void drawIndicator(sf::RenderWindow &window, float windowLength, float indentWidth, float gridLength, float gridSize, float squareLength, float lineWidth)
{
    std::tuple<int, int> tilePosition = getTilePosition(window, windowLength, indentWidth, gridLength, gridSize, squareLength);
    if (std::get<0>(tilePosition) >= 0)
    {
        sf::RectangleShape hoverIndicator(sf::Vector2f(squareLength-lineWidth, squareLength-lineWidth));
        hoverIndicator.setPosition(indentWidth+(std::get<0>(tilePosition)*squareLength), indentWidth+lineWidth+(std::get<1>(tilePosition)*squareLength));
        hoverIndicator.setFillColor(sf::Color(255, 255, 255, 100));
        window.draw(hoverIndicator);
    }
}

int main()
{

    const float windowLength = 800.0f;
    const float indentPercent = 0.05;
    const float indentWidth = windowLength * indentPercent; // 40
    const float lineWidth = 1.0f;
    const float gridLength = windowLength - (2*indentWidth); // 720
    const int gridSize = 8; // 40 to 760
    const float squareLength = gridLength / gridSize; // 22.5

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

        //draw transparent gray box over selected tile
        drawIndicator(window, windowLength, indentWidth, gridLength, gridSize, squareLength, lineWidth);
        
        //draw grid
        drawGrid(window, lineWidth, indentWidth, gridLength, gridSize);

        window.display();
    }
}