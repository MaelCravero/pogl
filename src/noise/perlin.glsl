#version 450

uniform int p[512];

float fade(float t)
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float lerp(float t, float a, float b)
{
    return a + t * (b - a);
}

float grad(int hash_val, float x, float y, float z)
{
    int h = hash_val & 15;

    float u = h < 8 ? x : y;

    float _v = h == 12 || h == 14 ? x : z;
    float v = h < 4 ? y : _v;

    float res = (h & 1) == 0 ? u : -u;
    res += (h & 2) == 0 ? v : -v;

    return res;
}

float noise_point(float x, float y, float z)
{
    // Get integer part to find unit cube containing the point
    int X = int(floor(x)) & 255;
    int Y = int(floor(y)) & 255;
    int Z = int(floor(z)) & 255;

    // Get decimal part to find point position relative to cube
    x -= floor(x);
    y -= floor(y);
    z -= floor(z);

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
