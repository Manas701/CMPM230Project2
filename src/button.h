#ifndef BUTTON_H
#define BUTTON_H

#include <iostream>
#include <SFML/Graphics.hpp>

class Button
{
    public:
        std::string name;
        float length;
        int order;
        sf::RectangleShape rect;
        sf::Vector2f xBounds;
        sf::Vector2f yBounds;
        sf::Vector2f position;
        void initButton(std::string, float);
        bool checkWithinBounds();
        bool checkIfClicked();
};

#endif