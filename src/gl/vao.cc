#include "gl/vao.hh"

namespace gl
{
    VAO::VAO()
    {
        glGenVertexArrays(1, &id_);
    }

    void VAO::bind() const
    {
        glBindVertexArray(id_);
    }

} // namespace gl
