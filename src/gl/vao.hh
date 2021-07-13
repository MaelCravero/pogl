#pragma once

#include "gl/gl.hh"

namespace gl
{
    class VAO : public Object
    {
    public:
        VAO();
        ~VAO() = default;
        VAO(const VAO& other) = delete;
        void bind() const;
    };

} // namespace gl
