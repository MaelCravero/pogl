#include "gl/vao.hh"

namespace gl
{
    VAO::VAO()
    {
        glGenVertexArrays(1, &id_);
        TEST_OPENGL_ERROR();
    }

    void VAO::bind() const
    {
        glBindVertexArray(id_);
        TEST_OPENGL_ERROR();
    }

} // namespace gl
