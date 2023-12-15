// Base code taken from https://github.com/johnBuffer/VerletSFML/blob/main/renderer.hpp

#pragma once
#include "solver.hpp"


class Renderer
{
public:
    explicit
    Renderer(sf::RenderTarget& target)
        : m_target{target}
    {

    }

    void render(const Solver& solver) const
    {
        // Render constraint
        const sf::Vector2f constraintPos  = solver.getConstraintPosition();
        const sf::Vector2f constraintSize = solver.getConstraintSize();
        // sf::CircleShape constraint_background{constraint.z};
        sf::RectangleShape constraint_background{sf::Vector2f(constraintSize.x, constraintSize.y)};
        constraint_background.setOrigin(constraintSize.x/2, constraintSize.y/2);
        constraint_background.setFillColor(sf::Color(202,182,158));
        constraint_background.setPosition(constraintPos.x, constraintPos.y);
        // constraint_background.setPointCount(128);
        m_target.draw(constraint_background);

        // Render objects
        sf::CircleShape circle{1.0f};
        circle.setPointCount(32);
        circle.setOrigin(1.0f, 1.0f);
        const auto& objects = solver.getObjects();
        for (const auto& obj : objects) {
            circle.setPosition(obj.position);
            circle.setScale(obj.radius, obj.radius);
            circle.setFillColor(obj.color);
            m_target.draw(circle);
        }
    }

private:
    sf::RenderTarget& m_target;
};