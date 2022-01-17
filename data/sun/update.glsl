#include "src/noise/perlin.glsl"

layout(local_size_x = 1024) in;

layout(binding = 0) buffer posData
{
    vec4 pos[];
};

layout(binding = 1) buffer colorData
{
    vec4 color[];
};

layout(binding = 2) buffer lifeData
{
    float life[];
};

uniform uint nb_particles;
uniform float life_span;
uniform float time;

float k = 0.01;
float sparsity = 10.0;

float _rand(vec2 co)
{
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

float rand(float v)
{
    return _rand(vec2(v, gl_GlobalInvocationID.x));
}

void main()
{
    uint X = gl_GlobalInvocationID.x;

    if (X > nb_particles / 2)
        sparsity *= 3;

    float wind_intensity =
        noise_point(sparsity * pos[X].x, sparsity * pos[X].y, sparsity * time);

    float direction_x = rand(42) - 0.5;
    float direction_y = rand(51) - 0.5;
    float direction_z = rand(3) - 0.5;

    pos[X].x += k * wind_intensity * direction_x;
    pos[X].y += k * wind_intensity * direction_y;
    pos[X].z += k * wind_intensity * direction_z;

    float color_decrement = pow(1.0f - life[X] / life_span, 2.0f);

    color[X].a -= color[X].a * k * (abs(wind_intensity) + (pos[X].y + 1.0));

    color[X].g -= color[X].a * k * (abs(wind_intensity) + (pos[X].y + 1.0));

    life[X] -= 2 * k * abs(wind_intensity);

    if (color[X].a < 0.01)
        life[X] = 0;
}
