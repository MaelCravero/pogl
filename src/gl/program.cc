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
        TEST_OPENGL_ERROR();

        if (!link_status)
        {
            GLint log_size;
            glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &log_size);
            TEST_OPENGL_ERROR();

            std::string log;
            log.reserve(log_size + 1);

            GLchar* log_str = const_cast<char*>(log.c_str());

            glGetProgramInfoLog(id_, log_size, &log_size, log_str);
            TEST_OPENGL_ERROR();

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

    void Program::dispatch_compute(int x, int y, int z) const
    {
        glDispatchCompute(x, y, z);
        TEST_OPENGL_ERROR();
        glMemoryBarrier(GL_ALL_BARRIER_BITS);
        TEST_OPENGL_ERROR();
    }

} // namespace gl
