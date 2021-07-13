#pragma once

#include <functional>

#include "gl/vbo.hh"

class Particle
{
public:
    using update_func_t = std::function<void(float x, float y, float z,
                                             GLfloat& pos, GLfloat& color)>;

    Particle(gl::VBO::data_t vertices, gl::VBO::data_t positions,
             gl::VBO::data_t colors, update_func_t update);

    void update(float time);
    void draw() const;

    inline std::size_t nb_particles() const
    {
        return vertices_.data_get().size();
    }

private:
    gl::VBO vertices_;
    gl::VBO positions_;
    gl::VBO colors_;
    update_func_t update_;
};
