#pragma once

#include <string>

#include "gl/gl.hh"

namespace gl
{
    class Shader
    {
    public:
        Shader(std::string path, GLenum type);
        ~Shader();
        Shader(const Shader& other) = delete;

        operator GLuint();

    protected:
        GLuint id_;
    };

    class VertexShader : public Shader
    {
    public:
        VertexShader(std::string path)
            : Shader(path, GL_VERTEX_SHADER)
        {}
    };

    class FragmentShader : public Shader
    {
    public:
        FragmentShader(std::string path)
            : Shader(path, GL_FRAGMENT_SHADER)
        {}
    };

} // namespace gl
