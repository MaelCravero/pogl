#pragma once

#include <type_traits>

#include "gl/vbo.hh"

namespace gl
{
    template <typename T>
    VBO<T>::VBO(data_t data)
        : data_(data)
    {
        glGenBuffers(1, &id_);
        glBindBuffer(GL_ARRAY_BUFFER, id_);
        glBufferData(GL_ARRAY_BUFFER, data_size(), data_.data(),
                     GL_STATIC_DRAW);
    }

    template <typename T>
    void VBO<T>::update() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, id_);
        glBufferData(GL_ARRAY_BUFFER, data_size(), NULL, GL_STREAM_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, data_size(), data_.data());
    }

    template <typename T>
    void VBO<T>::bind(GLuint index, GLuint dim, GLenum type,
                      GLenum normalize) const
    {
        glEnableVertexAttribArray(index);
        glBindBuffer(GL_ARRAY_BUFFER, id_);
        glVertexAttribPointer(index, dim, type, normalize, 0, (void*)0);
    }
} // namespace gl
