#include "glm/fwd.hpp"
#include "p6/p6.h"

class Boid {
private:
    float     m_size;
    glm::vec2 m_position;
    glm::vec2 m_direction;
    glm::vec2 m_speed;

public:
    // Constructors
    Boid() = default;

    ~Boid() = default;

    Boid(const Boid& boid)
        : m_size(boid.m_size), m_position(boid.m_position), m_direction(boid.m_direction), m_speed(boid.m_speed) {}

    Boid(const float& size, const glm::vec2& position, const glm::vec2& direction, const glm::vec2& speed)
        : m_size(size), m_position(position), m_direction(direction), m_speed(speed) {}

    // Getters
    float get_size() const { return m_size; }

    glm::vec2 get_position() const { return m_position; }

    glm::vec2 get_direction() const { return m_direction; }

    glm::vec2 get_speed() const { return m_speed; }

    // Setters
    void set_size(const float& size) { m_size = size; }

    void set_position(const glm::vec2& position) { m_position = position; }

    void set_direction(const glm::vec2& direction) { m_direction = direction; }

    void set_speed(const glm::vec2& speed) { m_speed = speed; }

    // Methods
    void move_boid() { m_position += m_direction * m_speed; }
};