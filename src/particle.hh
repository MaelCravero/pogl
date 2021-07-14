#pragma once

#include <functional>

#include "gl/vbo.hh"

class Particle
{
public:
    using vertices_t = gl::VBO<GLfloat>;
    using positions_t = gl::VBO<gl::Point4D>;
    using colors_t = gl::VBO<gl::Color>;
    using life_t = std::vector<float>;
    using update_func_t =
        std::function<void(std::size_t i, float time, gl::Point4D& pos,
                           gl::Color& color, float& life)>;
    using spawn_func_t =
        std::function<std::pair<gl::Point4D, gl::Color>(std::size_t i)>;

    Particle(vertices_t::data_t vertices, std::size_t nb_particles,
             life_t::value_type life_span, update_func_t update,
             spawn_func_t spawn);

    void bind() const;

    void spawn(std::size_t pos);

    void update(float time);
    void draw() const;

private:
    const vertices_t vertices_;
    std::size_t nb_particles_;
    positions_t positions_;
    colors_t colors_;
    life_t life_;
    const life_t::value_type life_span_;
    update_func_t update_;
    spawn_func_t spawn_;
};
