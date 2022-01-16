#pragma once

#include <functional>

#include "gl/program.hh"
#include "gl/vao.hh"

class Engine
{
public:
    using callback_t = void (*)(void);
    using update_t = std::function<void()>;

    static void init(int argc, char** argv);

    void attach(const gl::Shader& shader) const;

    void init_shaders(const gl::VertexShader& vertex_shader,
                      const gl::FragmentShader& fragment_shader) const;

    void main_loop(callback_t display, callback_t idle = nullptr) const;

    inline void bind_vao() const
    {
        vao_.bind();
    }

private:
    gl::VAO vao_;

public:
    gl::Program program;
    update_t draw = []() {};
    update_t update = []() {};
};
