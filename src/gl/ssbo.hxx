#pragma once

#include <cassert>
#include <type_traits>

#include "gl/ssbo.hh"

namespace gl
{
    template <typename T>
    SSBO<T>::SSBO(vbo_t& vbo, GLuint index)
        : vbo_(vbo)
    {
        // glGenBuffers(1, &id_);
        // TEST_OPENGL_ERROR();
        // glBindBuffer(GL_SHADER_STORAGE_BUFFER, id_);
        // TEST_OPENGL_ERROR();
        // glBufferData(GL_SHADER_STORAGE_BUFFER, data_size(),
        //              vbo_.data_get().data(), GL_DYNAMIC_COPY);
        // TEST_OPENGL_ERROR();
        id_ = vbo_;
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
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, data_size(),
                        vbo_.data_get().data());
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
        vbo_.data_get().assign(ptr, ptr + vbo_.data_get().size());
    }

    template <typename T>
    void SSBO<T>::unmap()
    {
        glUnmapNamedBuffer(id_);
        TEST_OPENGL_ERROR();
    }
} // namespace gl
