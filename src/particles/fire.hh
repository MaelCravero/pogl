#pragma once

#include "noise/perlin3d.hh"
#include "particles/particle.hh"

namespace particles
{
    class Fire : public Particle
    {
    public:
        Fire(std::size_t nb_particles, float life_span);

        std::pair<gl::Point4D, gl::Color> spawn_sub(std::size_t i) override;

        void update_sub(std::size_t i, float time, gl::Point4D& pos,
                        gl::Color& color, float& life) override;

    private:
        noise::Perlin3D noise_;
    };

} // namespace particles
