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

float k = 0.05;
float sparsity = 55.0;

void main()
{
    /* return; */
    uint X = gl_GlobalInvocationID.x;

    /* if (X > nb_particles / 2) */
    /*     sparsity *= 3; */

    float wind_intensity =
        noise_point(sparsity * pos[X].x, sparsity * pos[X].y, sparsity * time);

    float direction =
        noise_point(sparsity * pos[X].z, sparsity * pos[X].z, time);

    pos[X].y += direction * k * max(abs(wind_intensity), 0.01);
    pos[X].x += direction * k * max(abs(wind_intensity), 0.01);

    float color_decrement = pow(1.0f - life[X] / life_span, 2.0f);

    color[X].a -= k * k * abs(wind_intensity);

    life[X] -= k * abs(wind_intensity);
    if (pos[X].x < -5.0f && pos[X].y < -5.0f)
        life[X] *= color[X].a;

    if (color[X].a < 0.1)
        life[X] *= color[X].a;
}
