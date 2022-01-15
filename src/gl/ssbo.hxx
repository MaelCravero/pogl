#pragma once

#include <type_traits>

#include "gl/ssbo.hh"

namespace gl
{
    template <typename T>
    SSBO<T>::SSBO(data_t data, GLuint index)
        : data_(data)
    {
        glGenBuffers(1, &id_);
        TEST_OPENGL_ERROR();
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, id_);
        TEST_OPENGL_ERROR();
        glBufferData(GL_SHADER_STORAGE_BUFFER, data_size(), data_.data(),
                     GL_DYNAMIC_DRAW);
        TEST_OPENGL_ERROR();
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, id_);
        TEST_OPENGL_ERROR();
    }

    template <typename T>
    void SSBO<T>::update() const
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, id_);
        TEST_OPENGL_ERROR();
        glBufferData(GL_SHADER_STORAGE_BUFFER, data_size(), NULL,
                     GL_STREAM_DRAW);
        TEST_OPENGL_ERROR();
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, data_size(), data_.data());
        TEST_OPENGL_ERROR();
    }

    template <typename T>
    void SSBO<T>::bind() const
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, id_);
    }

    template <typename T>
    void SSBO<T>::map_buffer()
    {
        auto ptr = (T*)glMapNamedBuffer(id_, GL_READ_WRITE);
        TEST_OPENGL_ERROR();
        data_.assign(ptr, ptr + data_.size());
    }
} // namespace gl
