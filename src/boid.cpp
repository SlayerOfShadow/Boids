#include "boid.h"
#include "glm/fwd.hpp"

// Getters
float Boid::get_size() const
{
    return m_size;
}

float Boid::get_rotate_speed() const
{
    return m_rotate_speed;
}

glm::vec2 Boid::get_position() const
{
    return m_position;
}

glm::vec2 Boid::get_direction() const
{
    return m_direction;
}

glm::vec2 Boid::get_speed() const
{
    return m_speed;
}

// Setters
void Boid::set_size(const float& size)
{
    m_size = size;
}

void Boid::set_rotate_speed(const float& rotate_speed)
{
    m_rotate_speed = rotate_speed;
}

void Boid::set_position(const glm::vec2& position)
{
    m_position = position;
}

void Boid::set_direction(const glm::vec2& direction)
{
    m_direction = direction;
}

void Boid::set_speed(const glm::vec2& speed)
{
    m_speed = speed;
}

// Methods
void Boid::move_boid()
{
    m_position += m_direction * m_speed;
}

void Boid::avoid_walls(glm::vec2 min_window_size, glm::vec2 max_window_size)
{
    glm::mat2 rotate_left{glm::vec2(cos(m_rotate_speed), -sin(m_rotate_speed)), glm::vec2(sin(m_rotate_speed), cos(m_rotate_speed))};
    glm::mat2 rotate_right{glm::vec2(cos(m_rotate_speed), sin(m_rotate_speed)), glm::vec2(-sin(m_rotate_speed), cos(m_rotate_speed))};

    // check left & right walls
    if (m_position.x > max_window_size.x)
    {
        if (m_direction.y > 0)
        {
            m_direction = m_direction * rotate_left;
        }
        else
        {
            m_direction = m_direction * rotate_right;
        }
    }
    else if (m_position.x < min_window_size.x)
    {
        if (m_direction.y > 0)
        {
            m_direction = m_direction * rotate_right;
        }
        else
        {
            m_direction = m_direction * rotate_left;
        }
    }

    // check top & bottom walls
    else if (m_position.y > max_window_size.y)
    {
        if (m_direction.x > 0)
        {
            m_direction = m_direction * rotate_right;
        }
        else
        {
            m_direction = m_direction * rotate_left;
        }
    }
    else if (m_position.y < min_window_size.y)
    {
        if (m_direction.x > 0)
        {
            m_direction = m_direction * rotate_left;
        }
        else
        {
            m_direction = m_direction * rotate_right;
        }
    }
}