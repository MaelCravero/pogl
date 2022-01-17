#include "particles/pulsar.hh"

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

    Pulsar::Pulsar(std::size_t nb_particles, float life_span)
        : Particle(vertices, nb_particles, life_span)
        , noise_()
    {
        for (auto i = 0u; i < nb_particles; i++)
            spawn(i);
    }

    namespace
    {
        float rand()
        {
            return ((double)std::rand()) / RAND_MAX;
        }

        std::pair<GLfloat, GLfloat> rotate(GLfloat x, GLfloat y, float angle)
        {
            auto s = std::sin(angle);
            auto c = std::cos(angle);

            return {x * c - y * s, x * s + y * c};
        }

    } // namespace

    std::pair<gl::Point4D, gl::Color> Pulsar::spawn_sub(std::size_t i)
    {
        gl::Color initial_color = {1.0f, 1.0f, 0.0f, rand()};

        auto t = 2 * 3.14 * rand();
        auto u = rand() + rand();
        auto r = u > 1 ? 2 - u : u;

        if (i >= nb_particles_ / 10)
            r = (u > 0.5f ? 0.75f : 0.0f) + u / 4;

        float x = r * std::cos(t);
        float y = r * std::sin(t);

        auto x_radius = 0.4f;
        auto y_radius = 0.4f;

        if (i < nb_particles_ / 10)
        {
            initial_color.b = 1.0f;

            x_radius = 0.1f;
            y_radius = 0.1f;
        }

        return {{x_radius * x, y_radius * y, 0.0f, 1.0f}, initial_color};
    }

    void Pulsar::update_sub(std::size_t i, float time, gl::Point4D& pos,
                            gl::Color& color, float& life)
    {
        auto k = 0.01f;
        auto sparsity = 10.0f;

        if (i > nb_particles_ / 2)
            sparsity *= 3;

        auto wind_intensity = noise_.generate_noise_point(
            sparsity * pos.x, sparsity * pos.y, sparsity * time);

        auto [x, y] = rotate(pos.x, pos.y, 0.3);
        pos.x = x;
        pos.y = y;
        // pos.x += k * wind_intensity;

        color.a -= color.a * k * (std::abs(wind_intensity) + (pos.y + 1.0));

        if (i >= nb_particles_ / 10)
        {
            color.g -= color.a * k * (std::abs(wind_intensity) + (pos.y + 1.0));
        }
        else
        {
            pos.x += k * wind_intensity;
            pos.y += k * wind_intensity;
        }

        life -= rand() * k * std::abs(wind_intensity);

        if (color.a < 0.01)
            life = 0;
    }

} // namespace particles
