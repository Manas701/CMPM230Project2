#ifndef PADDLE_H
#define PADDLE_H

#include <iostream>
#include <SFML/Graphics.hpp>

enum Side {LEFT, RIGHT};


class Paddle
{
    public:
        Side side;
        sf::Keyboard::Key upKey;
        sf::Keyboard::Key downKey;
        float height;
        float width;
        float speed;
        sf::RectangleShape sprite;
        void Init();
        void Update();
};

#endif