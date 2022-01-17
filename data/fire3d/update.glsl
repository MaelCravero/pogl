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
float sparsity = 15.0;

void main()
{
    uint X = gl_GlobalInvocationID.x;

    if (X > nb_particles / 2)
        sparsity *= 3;

    float wind_intensity =
        noise_point(sparsity * pos[X].x, sparsity * pos[X].y, sparsity * time);

    float wind_intensity_z =
        noise_point(sparsity * pos[X].z, sparsity * pos[X].y, sparsity * time);

    pos[X].y += 0.01;
    pos[X].x += k * wind_intensity;
    pos[X].z += k * wind_intensity_z;

    float color_decrement = pow(1.0f - life[X] / life_span, 2.0f);

    if (color[X].g > 0)
        color[X].g -= 4 * color_decrement * (color[X].g < 0.5 ? 0.1 : 1);
    else
        color[X].a -= 3 * k * color_decrement;
    color[X].a -= 5 * k * (abs(wind_intensity) + (pos[X].y + 1.0));

    life[X] -= abs(wind_intensity);
    life[X] -= k * (pos[X].y + 1.0);
}
