#include "particles/compute-particle.hh"

#include <cassert>
#include <cmath>

#include "noise/perlin3d.hh"

namespace particles
{
    static unsigned NB_BATCHES = 0;
    static unsigned BATCH_SIZE = 1024;

    ComputeParticle::ComputeParticle(const DataPack& data,
                                     std::size_t nb_particles,
                                     gl::Program& main_prog)

        : vertices_(data.vertices)
        , nb_particles_(nb_particles)
        , positions_(nb_particles)
        , colors_(nb_particles)
        , life_(nb_particles)
        , life_span_(data.life_span)
        , main_prog_(main_prog)
    {
        gl::ComputeShader update_shader(data.update_path);
        gl::ComputeShader spawn_shader(data.spawn_path);

        noise::Perlin3D noise;

        update_prog_.attach(update_shader);
        update_prog_.link();

        update_prog_.use();
        update_prog_.set_uniform("nb_particles", (GLuint)nb_particles);
        update_prog_.set_uniform_array("p", noise.p);
        update_prog_.set_uniform("life_span", life_span_);

        spawn_prog_.attach(spawn_shader);
        spawn_prog_.link();

        spawn_prog_.use();
        spawn_prog_.set_uniform("life_span", life_span_);
        spawn_prog_.set_uniform("initial_color", data.initial_color.r,
                                data.initial_color.g, data.initial_color.b,
                                data.initial_color.a);

        data_.reset(new ssbo_data{positions_ssbo(positions_, 0),
                                  colors_ssbo(colors_, 1),
                                  life_ssbo(life_, 2)});

        NB_BATCHES =
            nb_particles_ / BATCH_SIZE + (nb_particles_ % BATCH_SIZE != 0);

        spawn();

        data_->life.map_buffer();
        data_->life.unmap();

        auto i = 0u;
        while (life_[i] != 0)
            i++;

        std::cerr << NB_BATCHES << " batches\n";
        std::cerr << i << " initialized particles\n";
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

        spawn_prog_.dispatch_compute(NB_BATCHES, 1, 1);

        main_prog_.use();
    }

    void ComputeParticle::update(float time)
    {
        update_prog_.use();
        update_prog_.set_uniform("time", time);

        data_->positions.bind();
        data_->colors.bind();
        data_->life.bind();

        update_prog_.dispatch_compute(NB_BATCHES, 1, 1);

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

        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, nb_particles_);
        TEST_OPENGL_ERROR();
    }

} // namespace particles
