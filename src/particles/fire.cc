#include "particles/fire.hh"

#include <cmath>

namespace particles
{
    namespace
    {
        static Particle::vertices_t::data_t vertices{
            -0.01f, -0.01f, 0.0f, // left
            0.01f,  -0.01f, 0.0f, // right
            0.0f,   0.01f,  0.0f, // top
            // 0.0f,   -0.02f, 0.0f // top
        };

    }

    Fire::Fire(std::size_t nb_particles, float life_span)
        : Particle(vertices, nb_particles, life_span)
    {
        for (auto i = 0u; i < nb_particles; i++)
            spawn(i);
    }

    std::pair<gl::Point4D, gl::Color> Fire::spawn_sub(std::size_t i)
    {
        gl::Color initial_color = {1.0f, 1.0f, 0.0f, 1.0f};

        // t = 2*pi*random()
        // u = random()+random()
        // r = if u>1 then 2-u else u
        //[r*cos(t), r*sin(t)]
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

        return {{x_radius * x + center_x, y_radius * y + center_y, 0.0f, 1.0f},
                initial_color};
    }

    void Fire::update_sub(std::size_t i, float time, gl::Point4D& pos,
                          gl::Color& color, float& life)
    {
        auto k = 0.03f;
        auto sparsity = 10.0f;

        if (i > nb_particles_ / 2)
            sparsity *= 3;

        auto wind_intensity = noise_.generate_noise_point(
            sparsity * pos.x, sparsity * pos.y, sparsity * time);

        pos.y += 0.01;
        pos.x += k * wind_intensity;

        auto color_decrement = std::pow(1 - life / life_span_, 2);
        if (color.g > 0)
            color.g -= 4 * color_decrement * (color.g < 0.5 ? 0.1 : 1);
        else
            color.a -= 3 * k * color_decrement;
        color.a -= 5 * k * (std::abs(wind_intensity) + (pos.y + 1.0));

        life -= std::abs(wind_intensity);
        life -= k * (pos.y + 1.0);
    }

} // namespace particles
