#pragma once

#include <functional>
#include <memory>

#include "data-pack.hh"
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
        using life_t = gl::VBO<GLfloat>;

        using positions_ssbo = gl::SSBO<gl::Point4D>;
        using colors_ssbo = gl::SSBO<gl::Color>;
        using life_ssbo = gl::SSBO<GLfloat>;

        ComputeParticle(const DataPack& data, std::size_t nb_particles,
                        gl::Program& main_prog);

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
        gl::Program spawn_prog_;

        struct ssbo_data
        {
            positions_ssbo positions;
            colors_ssbo colors;
            life_ssbo life;
        };

        std::unique_ptr<ssbo_data> data_ = nullptr;
    };
} // namespace particles
