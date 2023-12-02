#ifndef SCORE_H
#define SCORE_H

#include <iostream>
#include <SFML/Graphics.hpp>

class Score
{
    public:
        int leftScore = 0;
        int rightScore = 0;
        sf::Text leftNum;
        sf::Text rightNum;
        void Init();
        void Flash();
        void CheckScore();
        void FadeOut();
        void FadeIn();
};

#endif