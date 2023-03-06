#include "glm/fwd.hpp"
#include "p6/p6.h"

class Boid {
private:
    float     m_size;
    float     m_rotate_speed;
    glm::vec2 m_position;
    glm::vec2 m_direction;
    glm::vec2 m_speed;

public:
    // Constructors
    Boid() = default;

    ~Boid() = default;

    Boid(const Boid& boid)
        : m_size(boid.m_size), m_rotate_speed(boid.m_rotate_speed), m_position(boid.m_position), m_direction(boid.m_direction), m_speed(boid.m_speed) {}

    Boid(const float& size, const float& rotate_speed, const glm::vec2& position, const glm::vec2& direction, const glm::vec2& speed)
        : m_size(size), m_rotate_speed(rotate_speed), m_position(position), m_direction(direction), m_speed(speed) {}

    // Getters
    float get_size() const;

    float get_rotate_speed() const;

    glm::vec2 get_position() const;

    glm::vec2 get_direction() const;

    glm::vec2 get_speed() const;

    // Setters
    void set_size(const float& size);

    void set_rotate_speed(const float& size);

    void set_position(const glm::vec2& position);

    void set_direction(const glm::vec2& direction);

    void set_speed(const glm::vec2& speed);

    // Methods
    void move_boid();

    void avoid_walls(glm::vec2 min_window_size, glm::vec2 max_window_size);
};