#include <cstdlib>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include <vector>

#include "camera.hh"
#include "engine.hh"
#include "gl/gl.hh"
#include "gl/program.hh"
#include "gl/shader.hh"
#include "gl/vao.hh"
#include "gl/vbo.hh"
#include "noise/perlin3d.hh"
#include "particles/fire.hh"
#include "particles/sun.hh"

#define VERTEX_SHADER "src/vertex.glsl"
#define FRAGMENT_SHADER "src/fragment.glsl"

static std::unique_ptr<Engine> engine = nullptr;

const std::size_t nb_particles = 100000;
const float life_span = 12.0;

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
    auto loc = glGetUniformLocation(engine->program_, "mvp");
    TEST_OPENGL_ERROR();
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mvp));
    engine->update();
    glutPostRedisplay();
}

int main(int argc, char* argv[])
{
    Engine::init(argc, argv);

    engine.reset(new Engine());
    cam.reset(new camera::Camera());

    gl::VertexShader vertex_shader(VERTEX_SHADER);
    gl::FragmentShader fragment_shader(FRAGMENT_SHADER);

    engine->init_shaders(vertex_shader, fragment_shader);

    engine->bind_vao();

    particles::Sun particle(nb_particles, life_span);

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
