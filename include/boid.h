#include "glm/fwd.hpp"
#include "p6/p6.h"

class Boid {
private:
    float                  m_size;
    float                  m_rotate_speed;
    int                    m_trail_length;
    glm::vec2              m_position;
    glm::vec2              m_direction;
    glm::vec2              m_speed;
    std::vector<glm::vec2> m_last_positions;

public:
    // Constructors
    Boid() = default;

    ~Boid() = default;

    Boid(const float& size, const float& rotate_speed, const int& trail_length, const glm::vec2& position, const glm::vec2& direction, const glm::vec2& speed)
        : m_size(size), m_rotate_speed(rotate_speed), m_trail_length(trail_length), m_position(position), m_direction(direction), m_speed(speed) {}

    // Getters
    float get_size() const;

    glm::vec2 get_position() const;

    glm::vec2 get_direction() const;

    std::vector<glm::vec2> get_last_positions() const;

    // Methods
    void move_boid();

    void turn(float axis_speed, glm::mat2 rotation1, glm::mat2 rotation2);

    void avoid_walls(glm::vec2 min_window_size, glm::vec2 max_window_size);

    void display_boid(p6::Context& context);
};