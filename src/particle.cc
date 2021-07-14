#include "particle.hh"

#include <cassert>

Particle::Particle(vertices_t::data_t vertices, std::size_t nb_particles,
                   update_func_t update)

    : vertices_(vertices)
    , positions_(positions_t::data_t(nb_particles))
    , colors_(colors_t::data_t(nb_particles))
    , update_(update)
{}

void Particle::update(float time)
{
    for (auto i = 0u; i < nb_particles(); i++)
        // FIXME pos
        update_(0, 0, 0, positions_.data_get()[i], colors_.data_get()[i]);

    positions_.update();
    colors_.update();
}

void Particle::draw() const
{
    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, nb_particles());
}
