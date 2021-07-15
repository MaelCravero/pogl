#pragma once

#include "noise/perlin3d.hh"
#include "particles/particle.hh"

namespace particles
{
    class Sun3D : public Particle
    {
    public:
        Sun3D(std::size_t nb_particles, float life_span);

        std::pair<gl::Point4D, gl::Color> spawn_sub(std::size_t i) override;

        void update_sub(std::size_t i, float time, gl::Point4D& pos,
                        gl::Color& color, float& life) override;

    private:
        noise::Perlin3D noise_;
        std::vector<gl::Point4D> direction_;
    };

} // namespace particles
