#pragma once

#include "gl/gl.hh"
#include "gl/shader.hh"

namespace gl
{
    class Program : public Object
    {
    public:
        Program();
        ~Program();
        Program(const Program& other) = delete;

        void attach(const Shader& shader) const;
        void link() const;

        void use() const;
    };

} // namespace gl
