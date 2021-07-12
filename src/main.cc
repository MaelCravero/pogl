#include <iostream>
#include <vector>

#include "gl/gl.hh"
#include "gl/program.hh"
#include "gl/shader.hh"
#include "object_vbo.hh"

#define VERTEX_SHADER "src/vertex.glsl"
#define FRAGMENT_SHADER "src/fragment.glsl"

static uint8_t color = 0;

GLuint teapot_vao_id;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3b(color, 0, 0);
    glutWireTeapot(3);
    glFlush();

    /* TEST_OPENGL_ERROR(); */
    /* glBindVertexArray(teapot_vao_id); */
    /* TEST_OPENGL_ERROR(); */
    /* glDrawArrays(GL_TRIANGLES, 0, vertex_buffer_data.size()); */
    /* TEST_OPENGL_ERROR(); */
    /* glBindVertexArray(0); */
    /* TEST_OPENGL_ERROR(); */
    /* glutSwapBuffers(); */
    /* TEST_OPENGL_ERROR(); */
}

void init_shaders()
{
    std::cerr << "Initializing shaders" << std::endl;

    gl::VertexShader vertex_shader(VERTEX_SHADER);
    gl::FragmentShader fragment_shader(FRAGMENT_SHADER);

    gl::Program program;

    program.attach(vertex_shader);
    program.attach(fragment_shader);

    program.link();

    program.use();

    std::cerr << "Done - Initializing shaders" << std::endl;
}

void init_vbo()
{
    // TODO
}

void idle()
{
    color++;
    glutPostRedisplay();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(300, 300);
    glutCreateWindow("POGL");

    glClearColor(0, 0, 0, 0);
    glMatrixMode(GL_PROJECTION);
    glOrtho(-5, 5, -5, 5, 5, 15);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);

    // shaders
    glewInit();
    init_shaders();
    // vbo
    init_vbo();

    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glutMainLoop();

    return 0;
}
