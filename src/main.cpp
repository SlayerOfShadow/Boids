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
#include "../src-common/glimac/common.hpp"
#include "../src-common/glimac/sphere_vertices.hpp"
#include "doctest/doctest.h"

struct Vertex2DColor {
    glm::vec2 position;
    glm::vec3 color;

    Vertex2DColor();
    Vertex2DColor(glm::vec2 pos, glm::vec3 col)
    {
        position = pos;
        color    = col;
    };
};

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

    // OpenGL setup
    const p6::Shader shader = p6::load_shader(
        "../shaders/shader.vs.glsl",
        "../shaders/shader.fs.glsl"
    );
    // Création d'un Vertex Buffer Object et d'un Vertex Array Object
    GLuint vbo, vao;

    // Allocation d'un Vertex Buffer Object:
    glGenBuffers(1, &vbo);

    // "vbo" devient le VBO courant sur la cible GL_ARRAY_BUFFER:
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Tableau des attributs des sommets
    const std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(1.f, 32, 16);

    // Stockage des données du tableau vertices dans le vbo placé sur GL_ARRAY_BUFFER (c'est à dire "vbo" ici) :
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glimac::ShapeVertex), vertices.data(), GL_STATIC_DRAW);

    // Plus de VBO sur la cible GL_ARRAY_BUFFER:
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Allocation d'un Vertex Array Objet:
    glGenVertexArrays(1, &vao);

    // "vao" devient le VAO courant:
    glBindVertexArray(vao);

    static constexpr GLuint VERTEX_ATTR_POSITION = 0;
    static constexpr GLuint VERTEX_ATTR_COLOR    = 1;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_COLOR);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const GLvoid*)offsetof(Vertex2DColor, position));
    glVertexAttribPointer(VERTEX_ATTR_COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const GLvoid*)offsetof(Vertex2DColor, color));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Plus de VAO courant:
    glBindVertexArray(0);

    // Parameters
    std::vector<Boid> boids;
    size_t            nb_boids         = 20;
    float             boid_size        = 0.02f;
    float             boid_speed       = 0.01f;
    float             wall_distance    = 0.35f;
    float             background_alpha = 0.1f;
    glm::vec2         min_window_size{-ctx.aspect_ratio(), -1};
    glm::vec2         max_window_size{ctx.aspect_ratio(), 1};

    float separation_distance = 0.2f;
    float alignement_distance = 0.75f;
    float cohesion_distance   = 0.75f;
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
        /*
        ctx.fill = {0.f, 0.f, 0.f, background_alpha};
        ctx.rectangle(
            p6::Center(),
            glm::vec2(ctx.aspect_ratio()),
            p6::Angle()
        );

        ctx.fill       = {1.0f, 1.0f, 1.0f};
        ctx.use_stroke = false;
        for (size_t i = 0; i < nb_boids; i++)
        {
            boids[i].display_boid(ctx);
            boids[i].move_boid();
            boids[i].avoid_walls(min_window_size, max_window_size, wall_distance);
            boids[i].update_boid(boid_size, boid_speed);
            boids[i].separate(boids, separation_distance, separation_strength);
            boids[i].align(boids, alignement_distance, alignement_strength);
            boids[i].cohesion(boids, cohesion_distance, cohesion_strength);
        }*/

        glClear(GL_COLOR_BUFFER_BIT);

        // "vao" devient le VAO courant, OpenGL l'utilisera lors du dessin pour avoir des informations sur les sommets
        glBindVertexArray(vao);

        // OpenGL doit dessiner des triangles en utilisant 3 sommets (donc un seul triangle)
        shader.use();
        // => On utilise glDrawElements à la place de glDrawArrays
        // Cela indique à OpenGL qu'il doit utiliser l'IBO enregistré dans le VAO
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // Plus de VAO courant:
        glBindVertexArray(0);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    return 0;
}