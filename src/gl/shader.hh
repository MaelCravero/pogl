#pragma once

#include <string>

#include "gl/gl.hh"

namespace gl
{
    class Shader : public Object
    {
    public:
        Shader(std::string path, GLenum type);
        ~Shader();
        Shader(const Shader& other) = delete;
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

    class ComputeShader : public Shader
    {
    public:
        ComputeShader(std::string path)
            : Shader(path, GL_COMPUTE_SHADER)
        {}
    };

} // namespace gl
