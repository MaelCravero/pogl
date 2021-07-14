#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "gl/gl.hh"
#include "gl/program.hh"
#include "gl/shader.hh"
#include "gl/vao.hh"
#include "gl/vbo.hh"
#include "noise/perlin3d.hh"
#include "particle.hh"

#define VERTEX_SHADER "src/vertex.glsl"
#define FRAGMENT_SHADER "src/fragment.glsl"

static GLuint program_id;
GLuint VAO_id;
noise::Perlin3D perlin;

const std::size_t nb_particles = 10000;
const float life_span = 50.0;

auto update = [](std::size_t i, float time, gl::Point4D& pos, gl::Color& color,
                 float& life) {
    pos.y += 0.01;
    color.g -= std::pow(1 - life / life_span, 2);
    life -= perlin.normalized_noise(i, time, pos.y) - pos.y;
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

    // circle of radius 0.2 centered in (0, -0.8)
    auto radius = 0.1f;

    return {{radius * x, radius * y - 0.8f, 0.0f, 1.0f}, initial_color};
};

Particle* particle_ptr;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    // glColor3b(color, 0, 0);
    // glutWireTeapot(3);
    // glFlush();

    TEST_OPENGL_ERROR();
    glBindVertexArray(VAO_id);
    TEST_OPENGL_ERROR();
    particle_ptr->draw();
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    // TEST_OPENGL_ERROR();
    glBindVertexArray(0);
    TEST_OPENGL_ERROR();
    glutSwapBuffers();
    TEST_OPENGL_ERROR();
}

void init_shaders(const gl::VertexShader& vertex_shader,
                  const gl::FragmentShader& fragment_shader,
                  const gl::Program& program)
{
    std::cerr << "Initializing shaders" << std::endl;

    program.attach(vertex_shader);
    program.attach(fragment_shader);

    program.link();

    program.use();

    std::cerr << "Done - Initializing shaders" << std::endl;
}

void init_particles()
{
    gl::VAO vao;
    vao.bind();

    static Particle particle(
        {
            -0.01f, -0.01f, 0.0f, // left
            0.01f, -0.01f, 0.0f, // right
            0.0f, 0.01f, 0.0f, // top
            0.0f, -0.02f, 0.0f // top
        },
        nb_particles, life_span, update, spawn);

    particle.bind();

    gl::unbind();

    VAO_id = vao;
    particle_ptr = &particle;
}

void idle()
{
    static std::uint8_t frame = 0;

    float val = frame / 255.0f;

    // auto loc = glGetUniformLocation(program_id, "color");
    // glUniform4f(loc, 1.0f, val, 0.0f, 1.0f);

    // loc = glGetUniformLocation(program_id, "pos");
    // glUniform1f(loc, val);

    particle_ptr->update(val);

    frame++;
    glutPostRedisplay();
}

void init_glut(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitContextVersion(4, 5);
    glutInitContextProfile(GLUT_CORE_PROFILE | GLUT_DEBUG);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(300, 300);
    glutCreateWindow("POGL");

    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glClearColor(0, 0, 0, 0);
    TEST_OPENGL_ERROR();
    /* glMatrixMode(GL_PROJECTION); */
    /* TEST_OPENGL_ERROR(); */
    /* glOrtho(-5, 5, -5, 5, 5, 15); */
    /* TEST_OPENGL_ERROR(); */
    /* glMatrixMode(GL_MODELVIEW); */
    /* TEST_OPENGL_ERROR(); */
    /* gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0); */
    /* TEST_OPENGL_ERROR(); */
}

int main(int argc, char* argv[])
{
    init_glut(argc, argv);
    glewInit();

    gl::VertexShader vertex_shader(VERTEX_SHADER);
    gl::FragmentShader fragment_shader(FRAGMENT_SHADER);

    gl::Program program;

    init_shaders(vertex_shader, fragment_shader, program);
    // init_vbo(program);
    init_particles();

    program_id = program;

    glutMainLoop();

    return 0;
}
