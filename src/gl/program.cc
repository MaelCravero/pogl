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

    void Program::set_uniform(std::string name, GLfloat v1)
    {
        auto loc = uniform_location(name);
        glUniform1f(loc, v1);
        TEST_OPENGL_ERROR();
    }

    void Program::set_uniform(std::string name, GLuint v1)
    {
        auto loc = uniform_location(name);
        glUniform1ui(loc, v1);
        TEST_OPENGL_ERROR();
    }

    void Program::set_uniform(std::string name, GLfloat v1, GLfloat v2)
    {
        auto loc = uniform_location(name);
        glUniform2f(loc, v1, v2);
        TEST_OPENGL_ERROR();
    }

    void Program::set_uniform(std::string name, GLfloat v1, GLfloat v2,
                              GLfloat v3)
    {
        auto loc = uniform_location(name);
        glUniform3f(loc, v1, v2, v3);
        TEST_OPENGL_ERROR();
    }

    void Program::set_uniform(std::string name, GLfloat v1, GLfloat v2,
                              GLfloat v3, GLfloat v4)
    {
        auto loc = uniform_location(name);
        glUniform4f(loc, v1, v2, v3, v4);
        TEST_OPENGL_ERROR();
    }

    GLuint Program::uniform_location(std::string name)
    {
        auto loc = glGetUniformLocation(id_, name.c_str());
        TEST_OPENGL_ERROR();
        return loc;
    }
} // namespace gl
