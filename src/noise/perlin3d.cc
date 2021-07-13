#include "noise/perlin3d.hh"

#include <cmath>
#include <iostream>
#include <random>

namespace noise
{
    Perlin3D::Perlin3D()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, 255);

        for (int i = 0; i < 256; i++)
            p[i] = distrib(gen);

        for (int i = 0; i < 256; i++)
            p[256 + i] = permutation[i];
    }

    void Perlin3D::print(std::size_t size, std::size_t slice, float delta) const
    {
        std::cout << "[\n";
        for (auto i = 0u; i < size; i++)
        {
            for (auto j = 0u; j < size; j++)
            {
                float temp =
                    generate_noise_point(i * delta, j * delta, slice * delta);
                std::cout << std::floor(((temp + 1) / 2) * 255) << ",";
            }
            std::cout << "\n";
        }
        std::cout << "]\n";
    }

    float Perlin3D::generate_noise_point(float x, float y, float z) const
    {
        // Get integer part to find unit cube containing the point
        int X = (int)std::floor(x) & 255;
        int Y = (int)std::floor(y) & 255;
        int Z = (int)std::floor(z) & 255;

        // Get decimal part to find point position relative to cube
        x -= std::floor(x);
        y -= std::floor(y);
        z -= std::floor(z);

        // Compute fade curves
        float u = fade(x);
        float v = fade(y);
        float w = fade(z);

        // Get the 6 cube faces
        int A = p[X] + Y;
        int AA = p[A] + Z;
        int AB = p[A + 1] + Z;
        int B = p[X + 1] + Y;
        int BA = p[B] + Z;
        int BB = p[B + 1] + Z;

        // clang-format off
        return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z),
                                       grad(p[BA], x-1, y, z)),
                               lerp(u, grad(p[AB], x, y-1, z),
                                       grad(p[BB], x-1, y-1, z))),
                       lerp(v, lerp(u, grad(p[AA+1], x , y, z-1),
                                       grad(p[BA+1], x-1, y, z-1)),
                               lerp(u, grad(p[AB+1], x, y-1, z-1),
                                       grad(p[BB+1], x-1, y-1, z-1))));
        // clang-format on
    }

    float Perlin3D::fade(float t) const
    {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    float Perlin3D::lerp(float t, float a, float b) const
    {
        return a + t * (b - a);
    }

    float Perlin3D::grad(int hash_val, float x, float y, float z) const
    {
        int h = hash_val & 15;

        float u = h < 8 ? x : y;

        float _v = h == 12 || h == 14 ? x : z;
        float v = h < 4 ? y : _v;

        float res = (h & 1) == 0 ? u : -u;
        res += (h & 2) == 0 ? v : -v;

        return res;
    }
} // namespace noise
