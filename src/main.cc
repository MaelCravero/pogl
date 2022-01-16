#include <cstdlib>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include <vector>

#include "camera.hh"
#include "data-pack.hh"
#include "engine.hh"
#include "gl/gl.hh"
#include "gl/program.hh"
#include "gl/shader.hh"
#include "gl/vao.hh"
#include "gl/vbo.hh"
#include "noise/perlin3d.hh"
#include "particles/compute-particle.hh"
#include "particles/fire.hh"
#include "particles/fire3d.hh"
#include "particles/sun.hh"
#include "particles/sun3d.hh"

#define VERTEX_SHADER "src/vertex.glsl"
#define FRAGMENT_SHADER "src/fragment.glsl"

static std::unique_ptr<Engine> engine = nullptr;

static std::size_t nb_particles = 1000000;

static std::unique_ptr<camera::Camera> cam = nullptr;

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
    auto mvp = cam->compute_mvp();
    TEST_OPENGL_ERROR();
    auto loc = glGetUniformLocation(engine->program, "mvp");
    TEST_OPENGL_ERROR();
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mvp));
    engine->update();
    glutPostRedisplay();
}

int main(int argc, char* argv[])
{
    Engine::init(argc, argv);

    if (argc != 3 && argc != 2)
    {
        std::cerr << "usage: " << argv[0] << " data-path [nb_particles]\n";
        return 1;
    }

    DataPack data(argv[1]);

    if (argc == 3)
        nb_particles = std::stoi(argv[2]);

    engine.reset(new Engine());
    cam.reset(new camera::Camera());

    gl::VertexShader vertex_shader(VERTEX_SHADER);
    gl::FragmentShader fragment_shader(FRAGMENT_SHADER);

    engine->init_shaders(vertex_shader, fragment_shader);

    engine->bind_vao();

    // particles::Fire3D particle(nb_particles, life_span, engine->program);
    static particles::ComputeParticle::vertices_t::data_t vertices{
        -0.01f, -0.01f, 0.0f, // left
        0.01f,  -0.01f, 0.0f, // right
        0.0f,   0.01f,  0.0f, // top
        0.0f,   -0.02f, 0.0f // top
    };
    particles::ComputeParticle particle(data, nb_particles, engine->program);
    // particles::Fire particle(nb_particles, life_span);

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
