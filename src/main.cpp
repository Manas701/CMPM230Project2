#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "solver.hpp"
#include "renderer.hpp"
#include "utils/number_generator.hpp"
#include "utils/math.hpp"

// Create window
const float windowWidth  = 700.0f;
const float windowHeight = 600.0f;
const sf::Color bgColor(223,199,167);
const float constraintWidth = 250;
const float constraintHeight = 350;
const sf::Vector2f constraintPos(windowWidth/2, windowHeight*3/5);
const float dropHeight = constraintPos.y - constraintHeight/2 - 10.0f;

sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Suika", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);

int newLevel = 1;

Solver   solver;
Renderer renderer{window};

int getRandomLevel() {
    return (rand() % 5) + 1;
}

sf::CircleShape makeIndicator() {
    newLevel = getRandomLevel();
    sf::CircleShape circle{static_cast<float>(newLevel) * radius_multiplier};
    circle.setPointCount(32);
    circle.setOrigin(circle.getRadius(), circle.getRadius());
    circle.setFillColor(fruit_map[newLevel]);
    return circle;
}

void updateIndicatorPos(sf::CircleShape &indicator) {
    sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
    float object_radius = indicator.getRadius();
    if ((mousePos.x + object_radius <= constraintPos.x + constraintWidth/2) && (mousePos.x - object_radius >= constraintPos.x-constraintWidth/2)) {
        indicator.setPosition(mousePos.x, dropHeight);
    } else if (mousePos.x + object_radius > constraintPos.x + constraintWidth/2) {
        indicator.setPosition(constraintPos.x+constraintWidth/2-object_radius, dropHeight);
    } else {
        indicator.setPosition(constraintPos.x-constraintWidth/2+object_radius, dropHeight);
    }
}

int32_t main(int32_t, char*[])
{
    srand(time(0));

    sf::ContextSettings settings;
    settings.antialiasingLevel = 1;
    const uint32_t frame_rate = 60;
    window.setFramerateLimit(frame_rate);

    // Solver configuration
    solver.setConstraint(constraintPos, constraintWidth, constraintHeight);
    solver.setSubStepsCount(8);
    solver.setSimulationUpdateRate(frame_rate);

    sf::CircleShape indicator = makeIndicator();

    float timer = 0.0f;
    bool unheld = false;

    sf::Clock clock;
    // Main loop
    while (window.isOpen()) {
        sf::Event event{};
        sf::Time dt = clock.restart();

        if (unheld) { timer += dt.asSeconds(); } else { updateIndicatorPos(indicator); }
        if (timer >= 1.0f)
        {
            indicator = makeIndicator();
            unheld = false;
            timer = 0.0f;
        }

        while (window.pollEvent(event)) {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                case sf::Event::MouseButtonReleased:
                    if (!unheld)
                    {
                        solver.addObject(indicator.getPosition(), newLevel);
                        unheld = true;
                    }
            }
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
        }

        solver.update();
        window.clear(bgColor);
        renderer.render(solver);
        if (!unheld) { window.draw(indicator); }
		window.display();
    }

    return 0;
}
