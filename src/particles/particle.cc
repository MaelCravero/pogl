#include "particle.hh"

#include <cassert>

namespace particles
{
    Particle::Particle(vertices_t::data_t vertices, std::size_t nb_particles,
                       life_t::value_type life_span)

        : vertices_(vertices)
        , nb_particles_(nb_particles)
        , positions_(positions_t::data_t(nb_particles))
        , colors_(colors_t::data_t(nb_particles))
        , life_(nb_particles)
        , life_span_(life_span)
    {}

    void Particle::bind() const
    {
        vertices_.bind(0, 3, GL_FLOAT, GL_FALSE);
        positions_.bind(1, 4, GL_FLOAT, GL_FALSE);
        colors_.bind(2, 4, GL_FLOAT, GL_FALSE);
    }

    void Particle::spawn(std::size_t i)
    {
        auto [position, color] = spawn_sub(i);
        positions_.data_get()[i] = position;
        colors_.data_get()[i] = color;
        life_[i] = life_span_;
    }

    void Particle::update(float time)
    {
        for (auto i = 0u; i < nb_particles_; i++)
        {
            update_sub(i, time, positions_.data_get()[i], colors_.data_get()[i],
                       life_[i]);

            auto rand = []() -> float {
                return ((double)std::rand()) / RAND_MAX;
            };

            if (rand() > life_[i])
            {
                spawn(i);
                life_[i] = life_span_;
            }
        }

        positions_.update();
        colors_.update();
    }

    void Particle::draw() const
    {
        glVertexAttribDivisor(0, 0);
        TEST_OPENGL_ERROR();
        glVertexAttribDivisor(1, 1);
        TEST_OPENGL_ERROR();
        glVertexAttribDivisor(2, 1);
        TEST_OPENGL_ERROR();

        glDrawArraysInstanced(GL_TRIANGLES, 0, 4, nb_particles_);
        TEST_OPENGL_ERROR();
    }

} // namespace particles
