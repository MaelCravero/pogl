#version 450

uniform mat4 mvp;

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec4 pos;
layout(location = 2) in vec4 in_color;

out vec4 color;

void main()
{
    // gl_Position =
    // vec4(vertex.x + pos.x, vertex.y + pos.y, vertex.z + pos.z, 1.0);
    // gl_Position = vec4(0.0, 0.0, 0.0, 1.0);

    // gl_Position = vec4(vertex.x, vertex.y, vertex.z, 1.0);
    gl_Position = mvp *
        vec4(vertex.x + pos.x, vertex.y + pos.y, vertex.z + pos.z, 1.0);

    color = in_color;
}
