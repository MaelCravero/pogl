#pragma once

#include <vector>

#include "gl/gl.hh"

namespace gl
{
    template <typename T>
    class SSBO : public Object
    {
    public:
        using data_t = std::vector<T>;

        SSBO(data_t data, GLuint index);
        ~SSBO() = default;
        SSBO(const SSBO& other) = delete;

        void update() const;
        void bind() const;
        void map_buffer();

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

#include "gl/ssbo.hxx"
