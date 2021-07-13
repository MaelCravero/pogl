#pragma once

#include <vector>

#include "gl/gl.hh"

namespace gl
{
    class VBO : public Object
    {
    public:
        using data_t = std::vector<GLfloat>;

        VBO(data_t data);
        ~VBO() = default;
        VBO(const VBO& other) = delete;

        void update() const;
        void bind(GLuint index, GLuint dim) const;

    private:
        inline std::size_t data_size() const
        {
            return data_.size() * sizeof(float);
        }

        data_t data_;
    };
} // namespace gl
