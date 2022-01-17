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

float x_radius = 0.1f;
float y_radius = 0.1f;
float z_radius = 0.1f;

float center_x = 0.0f;
float center_y = -1.0f;
float center_z = 0.0f;

void main()
{
    uint X = gl_GlobalInvocationID.x;

    if (life[X] > random.x)
        return;

    float t = 2.0f * 3.14f * rand(random.x);
    float p = acos(2 * rand(random.y) - 1);
    float r = pow(rand(random.z), 0.33f);

    float x = r * sin(p) * cos(t);
    float y = r * sin(p) * sin(t);
    float z = r * cos(p);

    pos[X] = vec4(x_radius * x + center_x, y_radius * y + center_y,
                  z_radius * z + center_z, 1.0f);

    color[X] = initial_color;
    color[X].g -= sqrt(x * x + y * y + z * z) / 3.0f;

    life[X] = life_span;
}
