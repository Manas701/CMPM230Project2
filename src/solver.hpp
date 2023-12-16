// Base code taken from https://github.com/johnBuffer/VerletSFML/blob/main/solver.hpp

#pragma once
#include <vector>
#include <cmath>
#include <map>
#include <set>
#include <SFML/Graphics.hpp>

#include "utils/math.hpp"

const float radius_multiplier = 6;
const float y_neg_cap = -0.5f;
const float x_cap = 1.0f;

std::map<int, sf::Color>fruit_map= {{1,  sf::Color::Red},
                                    {2,  sf::Color(255, 102, 0)},
                                    {3,  sf::Color::Yellow},
                                    {4,  sf::Color::Green},
                                    {5,  sf::Color::Blue},
                                    {6, sf::Color::Cyan},
                                    {7,  sf::Color(75, 0, 130)},
                                    {8,  sf::Color(127, 0, 255)},
                                    {9, sf::Color::Magenta},
                                    {10,  sf::Color::White},
                                    {11,  sf::Color::Black}};

struct VerletObject
{
    sf::Vector2f position;
    sf::Vector2f position_last;
    sf::Vector2f acceleration;
    int          level         = 1;
    float        radius        = 4.0f;
    sf::Color    color         = sf::Color::White;

    VerletObject() = default;
    VerletObject(sf::Vector2f position_, float radius_)
        : position{position_}
        , position_last{position_}
        , acceleration{0.0f, 0.0f}
        , radius{radius_}
        , level{static_cast<int>(static_cast<int>(radius_) / radius_multiplier)}
        , color{fruit_map[level]}
    {}

    void update(float dt)
    {
        // Compute how much we moved
        sf::Vector2f displacement = position - position_last;

        // Cap velocity
        if (displacement.y < y_neg_cap) { displacement = sf::Vector2f(displacement.x, y_neg_cap); }
        if (displacement.x < -x_cap)    { displacement = sf::Vector2f(-x_cap, displacement.y); }
        if (displacement.x > x_cap)    { displacement = sf::Vector2f(x_cap, displacement.y); }

        // Update position
        position_last = position;
        position      = position + displacement + acceleration * (dt * dt);
        // Reset acceleration
        acceleration  = {};
    }

    void accelerate(sf::Vector2f a)
    {
        acceleration += a;
    }

    void setVelocity(sf::Vector2f v, float dt)
    {
        position_last = position - (v * dt);
    }

    void addVelocity(sf::Vector2f v, float dt)
    {
        position_last -= v * dt;
    }

    [[nodiscard]]
    sf::Vector2f getVelocity(float dt) const
    {
        return (position - position_last) / dt;
    }
};


class Solver
{
public:
    Solver() = default;

    VerletObject& addObject(sf::Vector2f position, int level)
    {
        return m_objects.emplace_back(position, static_cast<float>(level*radius_multiplier));
    }

    void update()
    {
        m_time += m_frame_dt;
        const float step_dt = getStepDt();
        for (uint32_t i{m_sub_steps}; i--;) {
            applyGravity();
            checkCollisions(step_dt);
            checkDelete(step_dt);
            applyConstraint();
            updateObjects(step_dt);
        }
    }

    void setSimulationUpdateRate(uint32_t rate)
    {
        m_frame_dt = 1.0f / static_cast<float>(rate);
    }

    void setConstraint(sf::Vector2f position, float width, float height)
    {
        m_constraint_center = position;
        m_constraint_width  = width;
        m_constraint_height = height;
    }

    void setSubStepsCount(uint32_t sub_steps)
    {
        m_sub_steps = sub_steps;
    }

    void setObjectVelocity(VerletObject& object, sf::Vector2f v)
    {
        object.setVelocity(v, getStepDt());
    }

    [[nodiscard]]
    const std::vector<VerletObject>& getObjects() const
    {
        return m_objects;
    }

    [[nodiscard]]
    sf::Vector2f getConstraintPosition() const
    {
        return {m_constraint_center.x, m_constraint_center.y};
    }

    [[nodiscard]]
    sf::Vector2f getConstraintSize() const
    {
        return {m_constraint_width, m_constraint_height};
    }

    [[nodiscard]]
    uint64_t getObjectsCount() const
    {
        return m_objects.size();
    }

    [[nodiscard]]
    float getTime() const
    {
        return m_time;
    }

    [[nodiscard]]
    float getStepDt() const
    {
        return m_frame_dt / static_cast<float>(m_sub_steps);
    }

private:
    uint32_t                  m_sub_steps          = 1;
    sf::Vector2f              m_gravity            = {0.0f, 1000.0f};
    sf::Vector2f              m_constraint_center;
    float                     m_constraint_width  = 100.0f;
    float                     m_constraint_height  = 100.0f;
    std::vector<VerletObject> m_objects;
    std::set<int>    add_objects;
    std::set<int>             del_objects;
    float                     m_time               = 0.0f;
    float                     m_frame_dt           = 0.0f;

    void applyGravity()
    {
        for (auto& obj : m_objects) {
            obj.accelerate(m_gravity);
        }
    }

    void checkCollisions(float dt)
    {
        const float    response_coef = 0.75f;
        const int objects_count = m_objects.size();
        // Iterate on all objects
        for (int i{0}; i < objects_count; ++i) {
            VerletObject& object_1 = m_objects[i];
            // Iterate on object involved in new collision pairs
            for (int k{i + 1}; k < objects_count; ++k) {
                VerletObject&      object_2 = m_objects[k];
                const sf::Vector2f v        = object_1.position - object_2.position;
                const float        dist2    = v.x * v.x + v.y * v.y;
                const float        min_dist = object_1.radius + object_2.radius;
                // Check overlapping
                if (dist2 < min_dist * min_dist) {
                    if ((object_1.level == object_2.level) && (object_1.level < 11)) {
                        del_objects.insert(i);
                        del_objects.insert(k);
                        if (object_1.position.y < object_2.position.y) {
                            add_objects.insert(k);
                        } else {
                            add_objects.insert(i);
                        }
                    }
                    else {
                        const float        dist  = sqrt(dist2);
                        const sf::Vector2f n     = v / dist;
                        const float mass_ratio_1 = object_1.radius / (object_1.radius + object_2.radius);
                        const float mass_ratio_2 = object_2.radius / (object_1.radius + object_2.radius);
                        const float delta        = 0.5f * response_coef * (dist - min_dist);
                        // Update positions
                        object_1.position -= n * (mass_ratio_2 * delta);
                        object_2.position += n * (mass_ratio_1 * delta);
                    }
                }
            }
        } 
    }

    void checkDelete(float dt)
    {
        std::vector<VerletObject> adding_obj;
        int add_count = add_objects.size();
        for (int i=0; i < add_count; i++) {
            adding_obj.push_back(m_objects[*add_objects.begin()]);
            add_objects.erase(add_objects.begin());
        }

        int del_count = del_objects.size();
        for (int i=0; i < del_count; i++) {
            m_objects.erase(m_objects.begin()+*del_objects.begin()-i);
            del_objects.erase(*del_objects.begin());
        }

        for (int i=0; i < add_count; i++) {
            sf::Vector2f new_pos = adding_obj[0].position;
            float new_level = static_cast<float>(adding_obj[0].level+1);
            float diff_pos_x = (new_pos.x + new_level*static_cast<float>(radius_multiplier)) - (m_constraint_center.x + m_constraint_width/2);
            float diff_neg_x = (new_pos.x - new_level*static_cast<float>(radius_multiplier)) - (m_constraint_center.x - m_constraint_width/2);
            float diff_pos_y = (new_pos.y + new_level*static_cast<float>(radius_multiplier)) - (m_constraint_center.y + m_constraint_height/2);
            float diff_neg_y = (new_pos.y - new_level*static_cast<float>(radius_multiplier)) - (m_constraint_center.y - m_constraint_height/2);
            if (diff_pos_x > 0) {
                new_pos = sf::Vector2f(new_pos.x - diff_pos_x, new_pos.y);
            } else if (diff_neg_x < 0) {
                new_pos = sf::Vector2f(new_pos.x - diff_neg_x, new_pos.y);
            }
            if (diff_pos_y > 0) {
                new_pos = sf::Vector2f(new_pos.x, new_pos.y - diff_pos_y);
            } else if (diff_neg_y < 0) {
                new_pos = sf::Vector2f(new_pos.x, new_pos.y - diff_neg_y);
            }
            addObject(new_pos, static_cast<int>(new_level));
            adding_obj.erase(adding_obj.begin());
        }
    }

    void applyConstraint()
    {
        for (auto& obj : m_objects) {
            const sf::Vector2f v    = m_constraint_center - obj.position;
            if (abs(v.x) > (m_constraint_width/2 - obj.radius) && -(v.y) > (m_constraint_height/2 - obj.radius)) {
                obj.position = obj.position_last;
            } else if (abs(v.x) > (m_constraint_width/2 - obj.radius)) {
                obj.setVelocity(obj.getVelocity(getStepDt()), getStepDt());
                obj.position = sf::Vector2f(obj.position_last.x, obj.position.y);
            } else if (-(v.y) > (m_constraint_height/2 - obj.radius)) {
                obj.position = sf::Vector2f(obj.position.x, obj.position_last.y);
            }
        }
    }

    void updateObjects(float dt)
    {
        for (auto& obj : m_objects) {
            obj.update(dt);
        }
    }
};
