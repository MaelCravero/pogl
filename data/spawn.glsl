#version 450

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

uniform vec3 random;
uniform float life_span;
uniform vec4 initial_color;

// Noise function taken from
// https://stackoverflow.com/questions/4200224/random-noise-functions-for-glsl
float _rand(vec2 co)
{
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

float rand(float v)
{
    return _rand(vec2(v, gl_GlobalInvocationID.x));
}

float x_radius = 0.1f;
float y_radius = 0.05f;

float center_x = 0.0f;
float center_y = -0.8f;

void main()
{
    uint X = gl_GlobalInvocationID.x;

    if (life[X] > random.x)
        return;

    float t = 3.0f * 3.14f * rand(random.x);
    float u = rand(random.y) + rand(random.z);
    float r = u > 1.0f ? 2.0f - u : u;

    float x = r * cos(t);
    float y = r * sin(t);

    pos[X] = vec4(x_radius * x + center_x, y_radius * y + center_y, 0.0f, 1.0f);

    color[X] = initial_color;
    color[X].g -= sqrt(x * x + y * y) / 3.0f;

    life[X] = life_span;
}
