#include "particles/fire3d.hh"

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

    Fire3D::Fire3D(std::size_t nb_particles, float life_span)
        : Particle(vertices, nb_particles, life_span)
    {
        for (auto i = 0u; i < nb_particles; i++)
            spawn(i);
    }

    std::pair<gl::Point4D, gl::Color> Fire3D::spawn_sub(std::size_t i)
    {
        gl::Color initial_color = {1.0f, 1.0f, 0.0f, 1.0f};

        auto rand = []() { return ((double)std::rand()) / RAND_MAX; };

        auto t = 2 * 3.14 * rand();
        auto p = std::acos(2 * rand() * 3.14 - 1);
        auto u = rand() + rand();
        auto r = u > 1 ? 2 - u : u;

        float x = r * std::cos(t) * std::sin(p);
        float y = r * std::sin(t) * std::sin(p);
        float z = r * std::cos(t);

        auto x_radius = 0.1f;
        auto y_radius = 0.1f;
        auto z_radius = 0.1f;

        initial_color.g -= std::sqrt(x * x + y * y + z * z) / 3;

        auto center_x = 0.0f;
        auto center_y = -1.0f;
        auto center_z = 0.0f;

        return {{x_radius * x + center_x, y_radius * y + center_y,
                 z * z_radius + center_z, 1.0f},
                initial_color};
    }

    void Fire3D::update_sub(std::size_t i, float time, gl::Point4D& pos,
                            gl::Color& color, float& life)
    {
        auto k = 0.01f;
        auto sparsity = 15.0f;

        if (i > nb_particles_ / 2)
            sparsity *= 3;

        auto wind_intensity = noise_.generate_noise_point(
            sparsity * pos.x, sparsity * pos.y, sparsity * time);

        auto wind_intensity_z = noise_.generate_noise_point(
            sparsity * pos.z, sparsity * pos.y, sparsity * time);

        pos.y += 0.01;
        pos.x += k * wind_intensity;
        pos.z += k * wind_intensity_z;

        auto color_decrement = std::pow(1 - life / life_span_, 2);
        if (color.g > 0)
            color.g -= 4 * color_decrement * (color.g < 0.5 ? 0.1 : 1);
        else
            color.a -= 3 * k * color_decrement;
        color.a -= k * (std::abs(wind_intensity) + (pos.y + 1.0));

        life -= std::abs(wind_intensity);
        life -= k * (pos.y + 1.0);

        if (color.a < 0)
            life = 0;
    }

} // namespace particles
