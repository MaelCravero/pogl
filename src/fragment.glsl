#version 450

in vec4 color;
out vec4 fragColor;

void main()
{
    fragColor = color;
    // fragColor = vec4(0.0 + color.x, 1.0, 0.0, 1.0);
}
