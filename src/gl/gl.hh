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
            std::cerr << "OpenGL ERROR! " << __FILE__ << ":" << __FUNCTION__   \
                      << ":" << __LINE__ << " : " << gluErrorString(err)       \
                      << std::endl;                                            \
    } while (0)

namespace gl
{
    using id_t = GLuint;

    class Object
    {
    public:
        inline operator id_t() const
        {
            return id_;
        }

    protected:
        id_t id_;
    };

    inline void unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    struct Point4D
    {
        GLfloat x, y, z, size;
    };

    struct Color
    {
        GLfloat r, g, b, a;
    };

    inline GLfloat rand()
    {
        return ((double)std::rand()) / RAND_MAX;
    }

} // namespace gl
