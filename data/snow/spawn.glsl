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
// http://byteblacksmith.com/improvements-to-the-canonical-one-liner-glsl-rand-for-opengl-es-2-0/
highp float _rand(vec2 co)
{
    highp float a = 12.9898;
    highp float b = 78.233;
    highp float c = 43758.5453;
    highp float dt = dot(co.xy, vec2(a, b));
    highp float sn = mod(dt, 3.14);
    return fract(sin(sn) * c);
}

highp float rand(float v)
{
    return _rand(vec2(v * 10000, gl_GlobalInvocationID.x));
}

float x_radius = 8.0f;
float y_radius = 8.0f;

float center_x = 1.5f;
float center_y = 1.8f;

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

    pos[X] =
        vec4(x_radius * x + center_x, y_radius * y + center_y, random.x, 1.0f);

    color[X] = initial_color;
    color[X].a = rand(random.z);

    life[X] = life_span;
}
