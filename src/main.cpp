#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "ball.h"
#include "paddle.h"
#include "score.h"

sf::Clock deltaClock;
std::string pathToAssets;
sf::Font scoreFont;
int fontSize = 48;

const float windowLength = 800.0f;
const float windowHeight = 600.0f;
const sf::Color ballColor(93, 115, 126);

const int fadeSpeed = 3;
bool fadedOut;

Ball ball;

const float ballThickness = 2.0f;
const float ballRadius = 5.0f;
const sf::Vector2f ballStartVelocity(400.0f, 200.0f);
const float ballAcceleration = 20.0f;

Paddle leftPaddle, rightPaddle;

const float paddleHeight = 50.0f;
const float paddleWidth = 5.0f;
const float paddleSpeed = 3.0f;
const sf::Vector2f leftPaddlePos(50.0f, windowHeight/2);
const sf::Vector2f rightPaddlePos(windowLength - 50.0f, windowHeight/2);

Score score;

sf::Vector2f leftScorePos(windowLength/4, 50.0f);
sf::Vector2f rightScorePos(windowLength*3/4, 50.0f);

float dt;

const float scoreTime = 1.0f;
const float serveTime = 0.5f;

// enum Side {LEFT, RIGHT};
Side winningSide = RIGHT;

enum GameState {FADEIN, FADEOUT, PAUSE, PLAY, SERVE};
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
    currState = FADEOUT;
    winningSide = scoredSide;

    score.FadeOut();
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
    this->bounces = 0;
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

void Paddle::Init()
{
    this->height = paddleHeight;
    this->width = paddleWidth;
    this->speed = paddleSpeed;
    this->sprite = sf::RectangleShape(sf::Vector2f(paddleWidth, paddleHeight));
    if (this->side == LEFT)
    {
        this->sprite.setPosition(leftPaddlePos);
        this->upKey = sf::Keyboard::W;
        this->downKey = sf::Keyboard::S;
    }
    else
    {
        this->sprite.setPosition(rightPaddlePos);
        this->upKey = sf::Keyboard::Up;
        this->downKey = sf::Keyboard::Down;
    }

}

void Paddle::Update()
{
    if (sf::Keyboard::isKeyPressed(this->upKey))
    {
        this->sprite.move(sf::Vector2f(0, -this->speed));
        if (this->sprite.getPosition().y < 0)
        {
            this->sprite.setPosition(sf::Vector2f(this->sprite.getPosition().x, 0));
        }
    }
    if (sf::Keyboard::isKeyPressed(this->downKey))
    {
        this->sprite.move(sf::Vector2f(0, this->speed));
        if (this->sprite.getPosition().y+this->height > windowHeight)
        {
            this->sprite.setPosition(sf::Vector2f(this->sprite.getPosition().x, windowHeight-this->height));
        }
    }

    // Checking if colliding with ball
    if (ball.sprite.getGlobalBounds().intersects(this->sprite.getGlobalBounds()) && ((this->side == LEFT && ball.velocity.x<0) || (this->side == RIGHT && ball.velocity.x>0)))
    {
        ball.velocity = sf::Vector2f(-ball.velocity.x + ball.acceleration, ball.velocity.y + ball.acceleration);
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

// tween to fade score in
void Score::FadeOut()
{
    sf::Color ogColor = this->leftNum.getFillColor();
    if (ogColor.a > 0)
    {
        ogColor.a -= fadeSpeed;
        if (ogColor.a < 0)
        {
            ogColor.a = 0;
        }
        this->leftNum.setFillColor(ogColor);
        this->rightNum.setFillColor(ogColor);
    }
    else
    {
        if (winningSide == LEFT)
        {
            score.leftScore++;
            score.leftNum.setString(std::to_string(score.leftScore));
        }
        else
        {
            score.rightScore++;
            score.rightNum.setString(std::to_string(score.rightScore));
        }
        currState = FADEIN;
        FadeIn();
    }
}

// tween to fade score in
void Score::FadeIn()
{
    sf::Color ogColor = this->leftNum.getFillColor();
    if (ogColor.a < 255)
    {
        ogColor.a += fadeSpeed;
        if (ogColor.a > 255)
        {
            ogColor.a = 255;
        }
        this->leftNum.setFillColor(ogColor);
        this->rightNum.setFillColor(ogColor);
    }
    else
    {
        if (dt >= scoreTime)
        {
            deltaClock.restart();
            ball.Reset();
            ball.Serve();
        }
    }
}

void InitObjects()
{
    InitWindow();
    score.Init();
    ball.Init();
    leftPaddle.side = LEFT;
    leftPaddle.Init();
    rightPaddle.side = RIGHT;
    rightPaddle.Init();
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
    else if (currState == FADEOUT)
    {
        score.FadeOut();
    }
    else if (currState == FADEIN)
    {
        score.FadeIn();
    }
    leftPaddle.Update();
    rightPaddle.Update();
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

void DrawObjects()
{
    window.draw(score.leftNum);
    window.draw(score.rightNum);

    window.draw(leftPaddle.sprite);
    window.draw(rightPaddle.sprite);
    window.draw(ball.sprite);
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

        DrawObjects();

        window.display();
    }
}