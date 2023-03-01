#include <stddef.h>
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

    std::vector<Boid> boids;
    size_t            nb_boids = 20;
    for (size_t i = 0; i < nb_boids; i++)
    {
        boids.push_back(Boid(0.05f, p6::random::point(ctx), p6::random::direction(), glm::vec2(0.01f, 0.01f)));
    }

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::Blue);
        for (size_t i = 0; i < nb_boids; i++)
        {
            ctx.square(
                p6::Center{boids[i].get_position()},
                p6::Radius{boids[i].get_size()}
            );
            boids[i].move_boid();
            if (boids[i].get_position().x > ctx.aspect_ratio() || boids[i].get_position().x < -ctx.aspect_ratio())
            {
                boids[i].set_direction(boids[i].get_direction() *= glm::vec2{-1.0f, 1.0f});
            }
            if (boids[i].get_position().y > 1 || boids[i].get_position().y < -1)
            {
                boids[i].set_direction(boids[i].get_direction() *= glm::vec2{1.0f, -1.0f});
            }
        }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}