#version 450

layout(location = 0) in vec3 aPos;

uniform float pos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y + pos, aPos.z, 1.0);
}
