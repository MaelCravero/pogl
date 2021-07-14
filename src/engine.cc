#include "engine.hh"

void Engine::init(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitContextVersion(4, 5);
    glutInitContextProfile(GLUT_CORE_PROFILE | GLUT_DEBUG);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(300, 300);
    glutCreateWindow("POGL");

    glClearColor(0, 0, 0, 0);
    TEST_OPENGL_ERROR();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* glMatrixMode(GL_PROJECTION); */
    /* TEST_OPENGL_ERROR(); */
    /* glOrtho(-5, 5, -5, 5, 5, 15); */
    /* TEST_OPENGL_ERROR(); */
    /* glMatrixMode(GL_MODELVIEW); */
    /* TEST_OPENGL_ERROR(); */
    /* gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0); */
    /* TEST_OPENGL_ERROR(); */

    glewInit();
}

void Engine::init_shaders(const gl::VertexShader& vertex_shader,
                          const gl::FragmentShader& fragment_shader) const
{
    program_.attach(vertex_shader);
    program_.attach(fragment_shader);

    program_.link();

    program_.use();
}

void Engine::main_loop(callback_t display, callback_t idle) const
{
    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glutMainLoop();
}
