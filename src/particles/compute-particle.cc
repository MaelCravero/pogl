#include "particles/compute-particle.hh"

#include <cassert>
#include <cmath>

#include "noise/perlin3d.hh"

namespace particles
{
    ComputeParticle::ComputeParticle(vertices_t::data_t vertices,
                                     std::size_t nb_particles,
                                     life_t::value_type life_span,
                                     gl::Program& main_prog)

        : vertices_(vertices)
        , nb_particles_(nb_particles)
        , positions_(nb_particles)
        , colors_(nb_particles)
        , life_(nb_particles)
        , life_span_(life_span)
        , main_prog_(main_prog)
        , update_prog_()
    {
        gl::ComputeShader update_shader("data/update.glsl");
        gl::ComputeShader spawn_shader("data/spawn.glsl");

        noise::Perlin3D noise;

        update_prog_.attach(update_shader);
        update_prog_.link();

        update_prog_.use();
        update_prog_.set_uniform("nb_particles", (GLuint)nb_particles);
        update_prog_.set_uniform_array("p", noise.p);
        update_prog_.set_uniform("life_span", life_span);

        spawn_prog_.attach(spawn_shader);
        spawn_prog_.link();

        spawn_prog_.use();
        spawn_prog_.set_uniform("life_span", life_span);
        spawn_prog_.set_uniform("initial_color", 1.0f, 1.0f, 0.0f, 1.0f);

        data_.reset(new ssbo_data{positions_ssbo(positions_, 0),
                                  colors_ssbo(colors_, 1),
                                  life_ssbo(life_, 2)});

        spawn();
    }

    void ComputeParticle::bind() const
    {
        vertices_.bind(0, 3, GL_FLOAT, GL_FALSE);
        positions_.bind(1, 4, GL_FLOAT, GL_FALSE);
        colors_.bind(2, 4, GL_FLOAT, GL_FALSE);
    }

    void ComputeParticle::spawn()
    {
        spawn_prog_.use();
        spawn_prog_.set_uniform("random", gl::rand(), gl::rand(), gl::rand());

        data_->positions.bind();
        data_->colors.bind();
        data_->life.bind();

        spawn_prog_.dispatch_compute(nb_particles_ / 1024 + 1, 1, 1);

        main_prog_.use();
    }

    void ComputeParticle::update(float time)
    {
        update_prog_.use();
        update_prog_.set_uniform("time", time);

        data_->positions.bind();
        data_->colors.bind();
        data_->life.bind();

        update_prog_.dispatch_compute(nb_particles_ / 1024 + 1, 1, 1);

        main_prog_.use();

        spawn();
    }

    void ComputeParticle::draw() const
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
