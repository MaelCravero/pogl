#pragma once

#include <functional>
#include <memory>

#include "gl/program.hh"
#include "gl/ssbo.hh"
#include "gl/vbo.hh"

namespace particles
{
    class ComputeParticle
    {
    public:
        using vertices_t = gl::VBO<GLfloat>;
        using positions_t = gl::VBO<gl::Point4D>;
        using colors_t = gl::VBO<gl::Color>;
        using life_t = std::vector<float>;

        using positions_ssbo_t = gl::SSBO<gl::Point4D>;
        using colors_ssbo_t = gl::SSBO<gl::Color>;

        ComputeParticle(vertices_t::data_t vertices, std::size_t nb_particles,
                        life_t::value_type life_span, gl::Program& main_prog);

        void bind() const;

        void update(float time);
        void draw() const;

    protected:
        void spawn();

    private:
        const vertices_t vertices_;

    protected:
        std::size_t nb_particles_;

    private:
        positions_t positions_;
        colors_t colors_;
        life_t life_;

    protected:
        const life_t::value_type life_span_;

        gl::Program& main_prog_;
        gl::Program update_prog_;

        std::unique_ptr<positions_ssbo_t> pos_data_ = nullptr;
    };
} // namespace particles
