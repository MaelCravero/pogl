#include "gl/vbo.hh"

namespace gl
{
    VBO::VBO(data_t data)
        : data_(data)
    {
        glGenBuffers(1, &id_);
        glBindBuffer(GL_ARRAY_BUFFER, id_);
        glBufferData(GL_ARRAY_BUFFER, data_size(), data_.data(),
                     GL_STATIC_DRAW);
    }

    void VBO::update() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, id_);
        glBufferData(GL_ARRAY_BUFFER, data_size(), NULL, GL_STREAM_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, data_size(), data_.data());
    }

    void VBO::bind(GLuint index, GLuint dim) const
    {
        glEnableVertexAttribArray(index);
        glBindBuffer(GL_ARRAY_BUFFER, id_);
        glVertexAttribPointer(index, dim, GL_FLOAT, GL_FALSE, 0, (void*)0);
    }
} // namespace gl
