#pragma once

#include "gl/gl.hh"
#include "gl/shader.hh"

namespace gl
{
    class Program
    {
    public:
        Program();
        ~Program();
        Program(const Program& other) = delete;

        void attach(const Shader& shader) const;
        void link() const;

        void use() const;

        inline operator GLuint() const
        {
            return id_;
        }

    private:
        GLuint id_;
    };

} // namespace gl
