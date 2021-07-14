#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

#include "engine.hh"
#include "gl/gl.hh"
#include "gl/program.hh"
#include "gl/shader.hh"
#include "gl/vao.hh"
#include "gl/vbo.hh"
#include "noise/perlin3d.hh"
#include "particle.hh"

#define VERTEX_SHADER "src/vertex.glsl"
#define FRAGMENT_SHADER "src/fragment.glsl"

static std::unique_ptr<Engine> engine = nullptr;

noise::Perlin3D perlin;

const std::size_t nb_particles = 100000;
const float life_span = 15.0;

auto update = [](std::size_t i, float time, gl::Point4D& pos, gl::Color& color,
                 float& life) {
    auto k = 0.01f;
    auto sparsity = 15.0f;

    if (i > nb_particles / 2)
        sparsity *= 3;

    auto wind_intensity = perlin.generate_noise_point(
        sparsity * pos.x, sparsity * pos.y, sparsity * time);

    pos.y += 0.01;
    pos.x += k * wind_intensity;

    auto color_decrement = std::pow(1 - life / life_span, 2);
    if (color.g > 0)
        color.g -= 4 * color_decrement * (color.g < 0.5 ? 0.1 : 1);
    else
        color.a -= k * color_decrement;
    color.a -= k * (std::abs(wind_intensity) + (pos.y + 1.0));

    life -= std::abs(wind_intensity);
    life -= k * (pos.y + 1.0);
};

auto spawn = [](std::size_t i) -> std::pair<gl::Point4D, gl::Color> {
    gl::Color initial_color = {1.0f, 1.0f, 0.0f, 1.0f};

    // t = 2*pi*random()
    // u = random()+random()
    // r = if u>1 then 2-u else u
    //[r*cos(t), r*sin(t)]
    auto rand = []() { return ((double)std::rand()) / RAND_MAX; };

    auto t = 2 * 3.14 * rand();
    auto u = rand() + rand();
    auto r = u > 1 ? 2 - u : u;

    float x = r * std::cos(t);
    float y = r * std::sin(t);

    // circle of radius centered in (0, -0.8)
    auto x_radius = 0.1f;
    auto y_radius = 0.05f;

    return {{x_radius * x, y_radius * y - 0.8f, 0.0f, 1.0f}, initial_color};
};

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    TEST_OPENGL_ERROR();

    engine->bind_vao();

    engine->draw();

    gl::unbind();

    glutSwapBuffers();
    TEST_OPENGL_ERROR();
}

void idle()
{
    engine->update();
    glutPostRedisplay();
}

int main(int argc, char* argv[])
{
    Engine::init(argc, argv);

    engine.reset(new Engine());

    gl::VertexShader vertex_shader(VERTEX_SHADER);
    gl::FragmentShader fragment_shader(FRAGMENT_SHADER);

    engine->init_shaders(vertex_shader, fragment_shader);

    engine->bind_vao();

    Particle particle(
        {
            -0.01f, -0.01f, 0.0f, // left
            0.01f, -0.01f, 0.0f, // right
            0.0f, 0.01f, 0.0f, // top
            0.0f, -0.02f, 0.0f // top
        },
        nb_particles, life_span, update, spawn);

    particle.bind();

    engine->draw = [&]() { particle.draw(); };
    engine->update = [&]() {
        static std::uint8_t frame = 0;
        float val = frame / 255.0f;
        particle.update(val);
        frame++;
    };

    gl::unbind();

    engine->main_loop(display, idle);

    return 0;
}
