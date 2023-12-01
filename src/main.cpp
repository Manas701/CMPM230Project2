#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "ball.h"

sf::Clock deltaClock;
std::string pathToAssets;

const float windowLength = 800.0f;
const float windowHeight = 600.0f;

Ball ball;

const float ballRadius = 5.0f;
const sf::Vector2f ballStartVelocity(100.0f, 200.0f);
const float ballAcceleration = 1.0f;

enum Side {LEFT, RIGHT};
Side winningSide = RIGHT;

enum GameState {PLAY, SCORE, SERVE, PAUSE};
GameState prevState;
GameState currState = PLAY;

bool canClick = true;

sf::RenderWindow window(sf::VideoMode(windowLength, windowHeight), "PONG!", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);

void InitWindow()
{
    window.setFramerateLimit(120u);
}

void Ball::Init()
{
    this->radius = ballRadius;
    this->diameter = ballRadius * 2;
    this->acceleration = ballAcceleration;
    this->bounces = 0;
    this->sprite = sf::CircleShape(ballRadius);
    this->Reset();
    this->Serve();
}

void Ball::Reset()
{
    this->sprite.setPosition(sf::Vector2f((windowLength/2) - this->radius, (windowHeight/2) - this->radius));
    this->velocity = sf::Vector2f();
}

void Ball::Update(float dt)
{
    this->sprite.move(this->velocity*dt);

    // Checking if colliding with vertical bounds
    float currX = this->sprite.getPosition().x;
    float currY = this->sprite.getPosition().y;
    if ((currY <= 0) || ((currY+this->diameter) >= windowHeight))
    {
        this->velocity.y *= -1;
    }

    // Checking if colliding with horizontal bounds
    if ((currX+this->diameter <= 0))
    {
        winningSide = RIGHT;
        ball.Reset();
        ball.Serve();
    }
    else if ((currX) >= windowLength)
    {
        winningSide = LEFT;
        ball.Reset();
        ball.Serve();
    }
}


void Ball::Serve()
{
    float xVel = (winningSide == RIGHT) ? ballStartVelocity.x : -ballStartVelocity.x;
    this->velocity = sf::Vector2f(xVel, ballStartVelocity.y);
}

void InitObjects()
{
    InitWindow();
    ball.Init();
}

void GameUpdate()
{
    float dt = deltaClock.restart().asSeconds();
    ball.Update(dt);
}

void CheckPause()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::P) && canClick)
    {
        canClick = false;
        if (currState != PAUSE)
        {
            prevState = currState;
            currState = PAUSE;
        }
        else
        {
            currState = prevState;
            deltaClock.restart();
        }
    }
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::P))
    {
        canClick = true;
    }
}

// void SpaceDebugCommand()
// {
//     ball.Reset();
// }

int main()
{
    InitObjects();

     #if _WIN32
        pathToAssets = "../../../src/assets/";
    #elif __APPLE__
        pathToAssets = "../../src/assets/";
    #endif


    deltaClock.restart();
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

        CheckPause();

        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        // {
        //     SpaceDebugCommand();
        // }

        if (currState != PAUSE)
        {
            GameUpdate();
        }

        window.draw(ball.sprite);

        window.display();
    }
}