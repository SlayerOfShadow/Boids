#include <stddef.h>
#include <cmath>
#include <cstdlib>
#include <vector>
#include "glm/detail/qualifier.hpp"
#include "glm/fwd.hpp"
#include "imgui.h"
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include "../include/boid.h"
#include "doctest/doctest.h"

int main(int argc, char* argv[])
{
    { // Run the tests
        if (doctest::Context{}.run() != 0)
            return EXIT_FAILURE;
        // The CI does not have a GPU so it cannot run the rest of the code.
        const bool no_gpu_available = argc >= 2 && strcmp(argv[1], "-nogpu") == 0; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        if (no_gpu_available)
            return EXIT_SUCCESS;
    }

    // Actual app
    auto ctx = p6::Context{{.title = "Simple-p6-Setup"}};
    ctx.maximize_window();

    // Parameters
    std::vector<Boid> boids;
    size_t            nb_boids          = 10;
    float             boid_size         = 0.05f;
    float             boid_rotate_speed = 0.1f;
    float             window_offset     = 0.3f;
    glm::vec2         boid_speed{0.01f, 0.01f};
    glm::vec2         min_window_size{-ctx.aspect_ratio() + window_offset, -1 + window_offset};
    glm::vec2         max_window_size{ctx.aspect_ratio() - window_offset, 1 - window_offset};

    for (size_t i = 0; i < nb_boids; i++)
    {
        boids.push_back(Boid(boid_size, boid_rotate_speed, p6::random::point(glm::vec2(min_window_size), glm::vec2(max_window_size)), p6::random::direction(), boid_speed));
    }

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::Blue);
        for (size_t i = 0; i < nb_boids; i++)
        {
            ctx.equilateral_triangle(
                p6::Center{boids[i].get_position()},
                p6::Radius{boids[i].get_size()},
                p6::Rotation{boids[i].get_direction()}
            );
            boids[i].move_boid();
            boids[i].avoid_walls(min_window_size, max_window_size);
        }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}