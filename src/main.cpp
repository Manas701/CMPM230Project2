#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "solver.hpp"
#include "renderer.hpp"
#include "utils/number_generator.hpp"
#include "utils/math.hpp"


int32_t main(int32_t, char*[])
{
    // Create window
    const float windowWidth  = 1000.0f;
    const float windowHeight = 800.0f;
    const sf::Color bgColor(223,199,167);
    const float constraintWidth = windowWidth*0.75f;
    const float constraintHeight = windowHeight*0.75f;
    const sf::Vector2f constraintPos(windowWidth/2, windowHeight/2);

    sf::ContextSettings settings;
    settings.antialiasingLevel = 1;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Verlet", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
    const uint32_t frame_rate = 60;
    window.setFramerateLimit(frame_rate);

    Solver   solver;
    Renderer renderer{window};

    // Solver configuration
    solver.setConstraint(constraintPos, constraintWidth, constraintHeight);
    solver.setSubStepsCount(8);
    solver.setSimulationUpdateRate(frame_rate);

    // Set simulation attributes 
    const int        largest_level     = 11;
    const float      radius_multiplier = 5;
    int level = 1;

    sf::Clock clock;
    // Main loop
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                case sf::Event::MouseButtonReleased:
                    sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                    float object_radius = static_cast<float>(level*radius_multiplier);
                    // A truly awful line of code that just checks whether you're spawning the ball within the constraint or not
                    if (((mousePos.x+object_radius) <= (constraintPos.x + constraintWidth/2)) && ((mousePos.y+object_radius) <= (constraintPos.y + constraintHeight/2)) && ((mousePos.x-object_radius) >= (constraintPos.x - constraintWidth/2)) && ((mousePos.y-object_radius) >= (constraintPos.y - constraintHeight/2))) {
                        solver.addObject(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)), level);
                        level++;
                        if (level > 11) { level = 1; }
                    }
            }
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
        }

        solver.update();
        window.clear(bgColor);
        renderer.render(solver);
		window.display();
    }

    return 0;
}