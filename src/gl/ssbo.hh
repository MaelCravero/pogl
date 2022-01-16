#pragma once

#include <vector>

#include "gl/gl.hh"
#include "gl/vbo.hh"

namespace gl
{
    template <typename T>
    class SSBO : public Object
    {
    public:
        using vbo_t = gl::VBO<T>;
        using data_t = typename vbo_t::data_t;

        SSBO(vbo_t& vbo, GLuint index);
        ~SSBO() = default;
        SSBO(const SSBO& other) = delete;

        void update() const;
        void bind() const;
        void map_buffer();
        void unmap();

        inline data_t data_get() const
        {
            return vbo_.data_get();
        }

        inline data_t& data_get()
        {
            return vbo_.data_get();
        }

        inline T& operator[](std::size_t i)
        {
            return data_get()[i];
        }

        inline const T operator[](std::size_t i) const
        {
            return data_get()[i];
        }

    private:
        inline std::size_t data_size() const
        {
            return data_get().size() * sizeof(T);
        }

        vbo_t& vbo_;
    };
} // namespace gl

#include "gl/ssbo.hxx"
