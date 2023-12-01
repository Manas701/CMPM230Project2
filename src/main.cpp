#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "ball.h"

std::string pathToAssets;

const float windowLength = 800.0f;
const float windowHeight = 600.0f;

Ball ball;

const float ballDiameter = 5.0f;
const float ballStartVelocity = 10.0f;
const float ballAcceleration = 1.0f;

enum Side {Left, Right};
Side winningSide = Right;

sf::RenderWindow window(sf::VideoMode(windowLength, windowHeight), "PONG!", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);

void InitWindow()
{
    window.setFramerateLimit(60);
}

void Ball::Init()
{
    this->diameter = ballDiameter;
    this->velocity = ballStartVelocity;
    this->acceleration = ballAcceleration;
    this->bounces = 0;
    this->sprite = sf::CircleShape(ballDiameter);
    this->Reset();
}

void Ball::Reset()
{
    this->sprite.setPosition(sf::Vector2f((windowLength/2) - this->diameter, (windowHeight/2) - this->diameter));
}

void Ball::Update()
{

}


void Ball::Serve()
{

}

void InitObjects()
{
    InitWindow();
    ball.Init();
}


void SpaceDebugCommand()
{
    ball.Reset();
}

int main()
{
    InitObjects();

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

        window.draw(ball.sprite);

        window.display();
    }
}