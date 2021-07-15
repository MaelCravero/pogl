#include "particles/sun3d.hh"

#include <cmath>

namespace particles
{
    namespace
    {
        static Particle::vertices_t::data_t vertices{
            -0.01f, -0.01f, 0.0f, // left
            0.01f,  -0.01f, 0.0f, // right
            0.0f,   0.01f,  0.0f, // top
            0.0f,   -0.02f, 0.0f // top
        };

    }

    Sun3D::Sun3D(std::size_t nb_particles, float life_span)
        : Particle(vertices, nb_particles, life_span)
        , noise_()
        , direction_(nb_particles)
    {
        for (auto i = 0u; i < nb_particles; i++)
            spawn(i);
    }

    std::pair<gl::Point4D, gl::Color> Sun3D::spawn_sub(std::size_t i)
    {
        auto rand = []() -> float { return ((double)std::rand()) / RAND_MAX; };
        gl::Color initial_color = {1.0f, 1.0f, 0.0f, rand()};

        direction_[i] = {rand() - 0.5f, rand() - 0.5f, rand() - 0.5f, 0};

        auto t = 2 * 3.14 * rand();
        auto p = std::acos(2 * rand() * 3.14 - 1);
        auto u = rand() + rand();
        auto r = u > 1 ? 2 - u : u;

        float x = r * std::cos(t) * std::sin(p);
        float y = r * std::sin(t) * std::sin(p);
        float z = r * std::cos(t);

        auto x_radius = 0.4f;
        auto y_radius = 0.4f;
        auto z_radius = 0.4f;

        return {{x_radius * x, y_radius * y, z * z_radius, 1.0f},
                initial_color};
    }

    void Sun3D::update_sub(std::size_t i, float time, gl::Point4D& pos,
                           gl::Color& color, float& life)
    {
        auto k = 0.01f;
        auto sparsity = 10.0f;

        if (i > nb_particles_ / 2)
            sparsity *= 3;

        auto wind_intensity = noise_.generate_noise_point(
            sparsity * pos.x, sparsity * pos.y, sparsity * time);

        pos.x += k * wind_intensity * direction_[i].x;
        pos.y += k * wind_intensity * direction_[i].y;
        pos.z += k * wind_intensity * direction_[i].z;
        // pos.x += k * wind_intensity;

        color.a -= color.a * k * (std::abs(wind_intensity) + (pos.y + 1.0));

        color.g -= color.a * k * (std::abs(wind_intensity) + (pos.y + 1.0));

        life -= 2 * k * std::abs(wind_intensity);

        if (color.a < 0.01)
            life = 0;
    }

} // namespace particles
