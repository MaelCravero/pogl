#pragma once

#include <functional>

#include "gl/vbo.hh"

class Particle
{
public:
    using vertices_t = gl::VBO<GLfloat>;
    using positions_t = gl::VBO<GLfloat>;
    using colors_t = gl::VBO<GLfloat>;
    using update_func_t = std::function<void(float x, float y, float z,
                                             GLfloat& pos, GLfloat& color)>;

    Particle(vertices_t::data_t vertices, std::size_t nb_particles,
             update_func_t update);

    void update(float time);
    void draw() const;

    inline std::size_t nb_particles() const
    {
        return vertices_.data_get().size();
    }

private:
    vertices_t vertices_;
    positions_t positions_;
    colors_t colors_;
    update_func_t update_;
};
