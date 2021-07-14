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
        TEST_OPENGL_ERROR();
        glBindBuffer(GL_ARRAY_BUFFER, id_);
        TEST_OPENGL_ERROR();
        glBufferData(GL_ARRAY_BUFFER, data_size(), data_.data(),
                     GL_STATIC_DRAW);
        TEST_OPENGL_ERROR();
    }

    template <typename T>
    void VBO<T>::update() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, id_);
        TEST_OPENGL_ERROR();
        glBufferData(GL_ARRAY_BUFFER, data_size(), NULL, GL_STREAM_DRAW);
        TEST_OPENGL_ERROR();
        glBufferSubData(GL_ARRAY_BUFFER, 0, data_size(), data_.data());
        TEST_OPENGL_ERROR();
    }

    template <typename T>
    void VBO<T>::bind(GLuint index, GLuint dim, GLenum type,
                      GLenum normalize) const
    {
        glEnableVertexAttribArray(index);
        TEST_OPENGL_ERROR();
        glBindBuffer(GL_ARRAY_BUFFER, id_);
        TEST_OPENGL_ERROR();
        glVertexAttribPointer(index, dim, type, normalize, 0, (void*)0);
        TEST_OPENGL_ERROR();
    }
} // namespace gl
