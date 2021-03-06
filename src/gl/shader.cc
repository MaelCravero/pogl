#include "gl/shader.hh"

#include <fstream>
#include <iostream>
#include <sstream>

namespace gl
{
    Shader::Shader(std::string path, GLenum type)
    {
        std::ifstream file(path);
        std::stringstream stream;

        if (file.fail())
        {
            std::cerr << "Failed reading " << path << std::endl;
        }

        std::string directive, argument;

        file >> directive >> argument;

        if (directive == "#include")
        {
            // Remove quotes
            argument.erase(0, 1);
            argument.erase(argument.size() - 1);

            std::cerr << "Including " << argument << std::endl;
            std::ifstream include(argument);

            if (include.fail())
            {
                std::cerr << "Failed reading " << argument << std::endl;
                stream << directive << ' ' << argument << '\n';
            }
            else
                stream << include.rdbuf();
        }
        else
            stream << directive << ' ' << argument << '\n';

        stream << file.rdbuf();

        id_ = glCreateShader(type);
        TEST_OPENGL_ERROR();

        auto shader = stream.str();
        const GLchar* chars = shader.c_str();

        glShaderSource(id_, 1, &chars, 0);
        TEST_OPENGL_ERROR();

        glCompileShader(id_);
        TEST_OPENGL_ERROR();

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

            std::cerr << "Error compiling " << path << ": " << log_str
                      << std::endl;
        }
        else
        {
            std::cout << path << " compiled!" << std::endl;
        }
    }

    Shader::~Shader()
    {
        glDeleteShader(id_);
    }

} // namespace gl
