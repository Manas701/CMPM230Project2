#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "ball.h"
#include "score.h"

sf::Clock deltaClock;
std::string pathToAssets;
sf::Font scoreFont;
int fontSize = 48;

const float windowLength = 800.0f;
const float windowHeight = 600.0f;
const sf::Color ballColor(93, 115, 126);

Ball ball;

const float ballThickness = 2.0f;
const float ballRadius = 5.0f;
const sf::Vector2f ballStartVelocity(100.0f, 200.0f);
const float ballAcceleration = 1.0f;

Score score;

sf::Vector2f leftScorePos(windowLength/4, 50.0f);
sf::Vector2f rightScorePos(windowLength*3/4, 50.0f);

float dt;

const float scoreTime = 1.0f;
const float serveTime = 0.5f;

enum Side {LEFT, RIGHT};
Side winningSide = RIGHT;

enum GameState {PAUSE, PLAY, SCORE, SERVE};
GameState prevState;
GameState currState = SERVE;

bool canClick = true;

sf::RenderWindow window(sf::VideoMode(windowLength, windowHeight), "PONG!", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);

void InitWindow()
{
    window.setFramerateLimit(120u);
}

void ScoreGoal(Side scoredSide)
{
    winningSide = scoredSide;
    if (scoredSide == LEFT)
    {
        score.leftScore++;
    }
    else
    {
        score.rightScore++;
    }
    
    std::cout << "Left Score: " << score.leftScore << " || Right Score: " << score.rightScore << std::endl;
    
    ball.Reset();
    ball.Serve();
}

void Ball::Init()
{
    this->radius = ballRadius;
    this->diameter = ballRadius * 2;
    this->acceleration = ballAcceleration;
    this->bounces = 0;
    this->sprite = sf::CircleShape(ballRadius);
    // this->sprite.setOutlineColor(sf::Color::Black);
    // this->sprite.setOutlineThickness(2.0f);
    this->sprite.setFillColor(ballColor);
    this->Reset();
    this->Serve();
}

void Ball::Reset()
{
    currState = SERVE;
    this->sprite.setPosition(sf::Vector2f((windowLength/2) - this->radius, (windowHeight/2) - this->radius));
    this->velocity = sf::Vector2f();
}

void Ball::Update()
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
        ScoreGoal(RIGHT);
    }
    else if ((currX) >= windowLength)
    {
        ScoreGoal(LEFT);
    }
}


void Ball::Serve()
{
    float xVel = (winningSide == RIGHT) ? ballStartVelocity.x : -ballStartVelocity.x;
    this->velocity = sf::Vector2f(xVel, ballStartVelocity.y);
}

void Ball::CheckServe()
{
    if (dt >= serveTime)
    {
        currState = PLAY;
        deltaClock.restart();
    }
}

void Score::Init()
{
    this->leftScore = 0;
    this->rightScore = 0;
    this->leftNum.setFont(scoreFont);
    this->rightNum.setFont(scoreFont);
    this->leftNum.setCharacterSize(fontSize);
    this->rightNum.setCharacterSize(fontSize);
    this->leftNum.setString(std::to_string(this->leftScore));
    this->rightNum.setString(std::to_string(this->rightScore));

    this->leftNum.setPosition(leftScorePos);
    this->rightNum.setPosition(rightScorePos);
}

void InitObjects()
{
    InitWindow();
    score.Init();
    ball.Init();
}

void GameUpdate()
{
    dt = deltaClock.getElapsedTime().asSeconds();
    // std::cout << dt << std::endl;
    if (currState == PLAY)
    {
        deltaClock.restart();
        ball.Update();
    }
    else if (currState == SERVE)
    {
        ball.CheckServe();
    }
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

int main()
{
     #if _WIN32
        pathToAssets = "../../../src/assets/";
    #elif __APPLE__
        pathToAssets = "../../src/assets/";
    #endif

    if (!scoreFont.loadFromFile(pathToAssets+"Fonts/Pong.ttf"))
    {
        std::cout << "Font failed to load." << std::endl;
    }

    InitObjects();

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

        if (currState != PAUSE)
        {
            GameUpdate();
        }

        window.draw(score.leftNum);
        window.draw(score.rightNum);
        window.draw(ball.sprite);

        window.display();
    }
}