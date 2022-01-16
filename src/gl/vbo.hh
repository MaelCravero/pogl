#pragma once

#include <vector>

#include "gl/gl.hh"

namespace gl
{
    template <typename T>
    class VBO : public Object
    {
    public:
        using value_type = T;
        using data_t = std::vector<T>;

        VBO(data_t data);
        VBO(std::size_t size);
        ~VBO() = default;
        VBO(const VBO& other) = delete;

        void update() const;
        void bind(GLuint index, GLuint dim, GLenum type,
                  GLenum normalize) const;

        inline data_t data_get() const
        {
            return data_;
        }

        inline data_t& data_get()
        {
            return data_;
        }

        inline T& operator[](std::size_t i)
        {
            return data_[i];
        }

        inline const T operator[](std::size_t i) const
        {
            return data_[i];
        }

    private:
        inline std::size_t data_size() const
        {
            return data_.size() * sizeof(T);
        }

        data_t data_;
    };
} // namespace gl

#include "gl/vbo.hxx"
