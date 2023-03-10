#include "boid.h"
#include "glm/fwd.hpp"

// Getters
float Boid::get_size() const
{
    return m_size;
}

glm::vec2 Boid::get_position() const
{
    return m_position;
}

glm::vec2 Boid::get_direction() const
{
    return m_direction;
}

std::vector<glm::vec2> Boid::get_last_positions() const
{
    return m_last_positions;
}

// Methods
void Boid::move_boid()
{
    m_position += m_direction * m_speed;

    // Trail (optional)
    m_last_positions.push_back(m_position);
    if (m_last_positions.size() > m_trail_length)
    {
        m_last_positions.erase(m_last_positions.begin());
    }
}

void Boid::turn(float axis_speed, glm::mat2 rotation1, glm::mat2 rotation2)
{
    if (axis_speed > 0)
    {
        m_direction = m_direction * rotation1;
    }
    else
    {
        m_direction = m_direction * rotation2;
    }
}

void Boid::avoid_walls(glm::vec2 min_window_size, glm::vec2 max_window_size)
{
    glm::mat2 rotate_left{glm::vec2(cos(m_rotate_speed), -sin(m_rotate_speed)), glm::vec2(sin(m_rotate_speed), cos(m_rotate_speed))};
    glm::mat2 rotate_right{glm::vec2(cos(m_rotate_speed), sin(m_rotate_speed)), glm::vec2(-sin(m_rotate_speed), cos(m_rotate_speed))};

    // Check left & right walls
    if (m_position.x > max_window_size.x)
    {
        turn(m_direction.y, rotate_left, rotate_right);
    }
    else if (m_position.x < min_window_size.x)
    {
        turn(m_direction.y, rotate_right, rotate_left);
    }

    // Check top & bottom walls
    else if (m_position.y > max_window_size.y)
    {
        turn(m_direction.x, rotate_right, rotate_left);
    }
    else if (m_position.y < min_window_size.y)
    {
        turn(m_direction.x, rotate_left, rotate_right);
    }
}

void Boid::display_boid(p6::Context& context)
{
    // Trail (optional)
    for (int j = 0; j < m_last_positions.size(); j++)
    {
        context.circle(
            p6::Center{m_last_positions[j]},
            p6::Radius{0.005f}
        );
    }

    context.equilateral_triangle(
        p6::Center{m_position},
        p6::Radius{m_size},
        p6::Rotation{m_direction}
    );
}