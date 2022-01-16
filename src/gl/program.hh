#pragma once

#include <array>

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
        void dispatch_compute(int x, int y, int z) const;

        void set_uniform(std::string name, GLfloat v1);
        void set_uniform(std::string name, GLuint v1);
        void set_uniform(std::string name, GLfloat v1, GLfloat v2);
        void set_uniform(std::string name, GLfloat v1, GLfloat v2, GLfloat v3);
        void set_uniform(std::string name, GLfloat v1, GLfloat v2, GLfloat v3,
                         GLfloat v4);

        template <std::size_t N>
        inline void set_uniform_array(std::string name, std::array<GLint, N> v)
        {
            auto loc = uniform_location(name);
            glUniform1iv(loc, N, v.data());
            TEST_OPENGL_ERROR();
        }

    private:
        GLuint uniform_location(std::string name);
    };

} // namespace gl
