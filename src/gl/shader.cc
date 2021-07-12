#include "gl/shader.hh"

#include <fstream>
#include <iostream>
#include <sstream>

namespace gl
{
    Shader::Shader(std::string path, GLuint type)
    {
        std::ifstream file(path);
        std::stringstream stream;

        stream << file.rdbuf();

        auto shader = stream.str();

        id_ = glCreateShader(type);

        const GLchar* chars = shader.c_str();

        glShaderSource(id_, 1, &chars, 0);

        glCompileShader(id_);

        auto compile_status = GL_TRUE;
        glGetShaderiv(id_, GL_COMPILE_STATUS, &compile_status);

        if (!compile_status)
        {
            GLint log_size;
            glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &log_size);

            std::string log;
            log.reserve(log_size + 1);

            GLchar* log_str = const_cast<char*>(log.c_str());

            glGetShaderInfoLog(id_, log_size, &log_size, log_str);

            std::cerr << "Error compiling shader: " << log_str << std::endl;
        }
    }

    Shader::~Shader()
    {
        glDeleteShader(id_);
    }

    Shader::operator GLuint()
    {
        return id_;
    }

} // namespace gl
