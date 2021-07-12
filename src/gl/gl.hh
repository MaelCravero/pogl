#pragma once

#include <GL/glew.h>

// Freeglut must be imported after glew
#include <GL/freeglut.h>
#include <iostream>

#define TEST_OPENGL_ERROR()                                                    \
    do                                                                         \
    {                                                                          \
        GLenum err = glGetError();                                             \
        if (err != GL_NO_ERROR)                                                \
            std::cerr << "OpenGL ERROR!" << __LINE__ << std::endl;             \
    } while (0)
