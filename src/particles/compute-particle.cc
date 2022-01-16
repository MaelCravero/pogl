#include "particles/compute-particle.hh"

#include <cassert>
#include <cmath>

namespace particles
{
    ComputeParticle::ComputeParticle(vertices_t::data_t vertices,
                                     std::size_t nb_particles,
                                     life_t::value_type life_span,
                                     gl::Program& main_prog)

        : vertices_(vertices)
        , nb_particles_(nb_particles)
        , positions_(positions_t::data_t(nb_particles))
        , colors_(colors_t::data_t(nb_particles))
        , life_(nb_particles)
        , life_span_(life_span)
        , main_prog_(main_prog)
        , update_prog_()
    {
        spawn();

        gl::ComputeShader update("data/update.glsl");

        update_prog_.attach(update);
        update_prog_.link();

        pos_data_.reset(new positions_ssbo_t(positions_, 0));
    }

    void ComputeParticle::bind() const
    {
        vertices_.bind(0, 3, GL_FLOAT, GL_FALSE);
        positions_.bind(1, 4, GL_FLOAT, GL_FALSE);
        colors_.bind(2, 4, GL_FLOAT, GL_FALSE);
    }

    void ComputeParticle::spawn()
    {
        for (auto i = 0u; i < nb_particles_; i++)
        {
            gl::Color initial_color = {1.0f, 1.0f, 0.0f, 1.0f};
            auto rand = []() { return ((double)std::rand()) / RAND_MAX; };

            auto t = 2 * 3.14 * rand();
            auto u = rand() + rand();
            auto r = u > 1 ? 2 - u : u;

            float x = r * std::cos(t);
            float y = r * std::sin(t);

            // circle of radius centered in (0, -0.8)
            // auto x_radius = 2.1f;
            auto x_radius = 0.1f;
            auto y_radius = 0.05f;

            initial_color.g -= std::sqrt(x * x + y * y) / 3;

            auto center_x = 0.0f;
            // auto center_y = -1.0f;
            auto center_y = -0.8f;

            positions_[i] = {x_radius * x + center_x, y_radius * y + center_y,
                             0.0f, 1.0f};
            colors_[i] = initial_color;
        }

        positions_.update();
        colors_.update();
    }

    void ComputeParticle::update(float time)
    {
        (void)time;

        update_prog_.use();
        pos_data_->bind();
        update_prog_.dispatch_compute(nb_particles_ / 1024 + 1, 1, 1);

        // pos_data_->map_buffer();
        // pos_data_->unmap();
        // pos_data_->update();

        main_prog_.use();

        // positions_.update();
        // colors_.update();

        // std::cout << pos_data_->data_get()[0].y << " "
        //           << positions_.data_get()[0].y << std::endl;
        // std::cout << pos_data_->data_get()[20057].x << " "
        //           << positions_.data_get()[20057].x << std::endl;
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
