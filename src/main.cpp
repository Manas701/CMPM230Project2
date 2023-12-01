#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>

std::string pathToAssets;

const float windowLength = 800.0f;
const float windowHeight = 600.0f;
const float ballDiameter = 5.0f;

// OBJECTIFY THIS
const float ballVelocity = 10.0f;

enum Side {Left, Right};
Side winningSide = Right;

sf::RenderWindow window(sf::VideoMode(windowLength, windowHeight), "PONG!", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
sf::CircleShape ball(ballDiameter);

void InitWindow()
{
    std::cout << "Initializing window\n";
    window.setFramerateLimit(60);
    std::cout << "Window initialized\n";
}

void BallReset()
{
    ball.setPosition(sf::Vector2f(windowLength/2-ballDiameter, windowHeight/2-ballDiameter));
}

void Serve()
{

}


void SpaceDebugCommand()
{
    BallReset();
}

int main()
{
    std::cout << "Beginning of main\n";

    InitWindow();

     #if _WIN32
        pathToAssets = "../../../src/assets/";
    #elif __APPLE__
        pathToAssets = "../../src/assets/";
    #endif

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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            SpaceDebugCommand();
        }

        window.draw(ball);

        window.display();
    }
}