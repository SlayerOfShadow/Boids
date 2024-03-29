#include <stddef.h>
#include <cmath>
#include <cstdlib>
#include <vector>
#include "glm/detail/qualifier.hpp"
#include "glm/fwd.hpp"
#include "imgui.h"
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include "../include/boid.hpp"
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
    size_t            nb_boids         = 50;
    float             boid_size        = 0.02f;
    float             boid_speed       = 0.01f;
    float             wall_distance    = 0.35f;
    float             background_alpha = 0.1f;
    glm::vec2         min_window_size{-ctx.aspect_ratio(), -1};
    glm::vec2         max_window_size{ctx.aspect_ratio(), 1};

    float separation_distance = 0.5f;
    float alignement_distance = 0.5f;
    float cohesion_distance   = 0.5f;
    float separation_strength = 0.05f;
    float alignement_strength = 0.05f;
    float cohesion_strength   = 0.05f;

    for (size_t i = 0; i < nb_boids; i++)
    {
        boids.push_back(Boid(boid_size, boid_speed, p6::random::point(glm::vec2(min_window_size + wall_distance), glm::vec2(max_window_size - wall_distance)), p6::random::direction()));
    }

    // Display parameters
    ctx.imgui = [&]() {
        ImGui::Begin("Parameters");
        ImGui::SliderFloat("Size", &boid_size, 0.005f, 0.05f);
        ImGui::SliderFloat("Speed", &boid_speed, 0.005f, 0.02f);
        ImGui::SliderFloat("Separation", &separation_strength, 0.0f, 0.1f);
        ImGui::SliderFloat("Alignement", &alignement_strength, 0.0f, 0.1f);
        ImGui::SliderFloat("Cohesion", &cohesion_strength, 0.0f, 0.1f);
        ImGui::SliderFloat("Wall distance", &wall_distance, 0.25f, 0.75f);
        ImGui::SliderFloat("Background alpha", &background_alpha, 0.f, 1.f);
        ImGui::End();
    };

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.fill = {0.f, 0.f, 0.f, background_alpha};
        ctx.rectangle(
            p6::Center(),
            glm::vec2(ctx.aspect_ratio()),
            p6::Angle()
        );

        ctx.fill       = {1.0f, 1.0f, 1.0f};
        ctx.use_stroke = false;
        for (auto& boid : boids)
        {
            boid.display_boid(ctx);
            boid.move_boid();
            boid.avoid_walls(min_window_size, max_window_size, wall_distance);
            boid.update_boid(boid_size, boid_speed);
            boid.separate(boids, separation_distance, separation_strength);
            boid.align(boids, alignement_distance, alignement_strength);
            boid.cohesion(boids, cohesion_distance, cohesion_strength);
        }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    return 0;
}