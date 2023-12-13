#include <iostream>
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

    sf::ContextSettings settings;
    settings.antialiasingLevel = 1;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Verlet", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
    const uint32_t frame_rate = 60;
    window.setFramerateLimit(frame_rate);

    Solver   solver;
    Renderer renderer{window};

    // Solver configuration
    solver.setConstraint({windowWidth/2, windowHeight/2}, windowWidth*0.75f, windowHeight*0.75f);
    solver.setSubStepsCount(8);
    solver.setSimulationUpdateRate(frame_rate);

    // Set simulation attributes
    const float        object_spawn_delay    = 0.025f;
    const float        object_spawn_speed    = 1200.0f;
    const sf::Vector2f object_spawn_position = {500.0f, 200.0f};
    const float        object_min_radius     = 1.0f;
    const float        object_max_radius     = 20.0f;
    const uint32_t     max_objects_count     = 1000;
    const float        max_angle             = 1.0f;

    sf::Clock clock;
    // Main loop
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
        }

        if (solver.getObjectsCount() < max_objects_count && clock.getElapsedTime().asSeconds() >= object_spawn_delay) {
            clock.restart();
            auto&       object = solver.addObject(object_spawn_position, RNGf::getRange(object_min_radius, object_max_radius));
            const float t      = solver.getTime();
            const float angle  = max_angle * sin(t) + Math::PI * 0.5f;
            solver.setObjectVelocity(object, object_spawn_speed * sf::Vector2f{cos(angle), sin(angle)});
        }

        solver.update();
        window.clear(sf::Color::White);
        renderer.render(solver);
		window.display();
    }

    return 0;
}