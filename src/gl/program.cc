#include "gl/program.hh"

#include <stdexcept>

#include "gl/gl.hh"

namespace gl
{
    Program::Program()
    {
        id_ = glCreateProgram();
        TEST_OPENGL_ERROR();

        if (id_ == 0)
            throw std::runtime_error("program_id is 0");
    }

    Program::~Program()
    {
        glDeleteProgram(id_);
    }

    void Program::attach(const Shader& shader) const
    {
        glAttachShader(id_, shader);
        TEST_OPENGL_ERROR();
    }

    void Program::link() const
    {
        glLinkProgram(id_);
        TEST_OPENGL_ERROR();

        auto link_status = GL_TRUE;
        glGetProgramiv(id_, GL_LINK_STATUS, &link_status);

        if (!link_status)
        {
            GLint log_size;
            glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &log_size);

            std::string log;
            log.reserve(log_size + 1);

            GLchar* log_str = const_cast<char*>(log.c_str());

            glGetProgramInfoLog(id_, log_size, &log_size, log_str);

            std::cerr << "Error linking program: " << log_str << std::endl;
        }
        else
        {
            std::cerr << "Program linked!" << std::endl;
        }
    }

    void Program::use() const
    {
        glUseProgram(id_);
        TEST_OPENGL_ERROR();
    }

} // namespace gl
