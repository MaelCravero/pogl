#version 450

uniform sampler2D texture_sampler;
uniform sampler2D lighting_sampler;
uniform sampler2D normalmap_sampler;

in vec3 vertex_out_color;
in vec2 vertex_out_uv;

layout(location=0) out vec4 output_color;

void main() {
    output_color = texture(texture_sampler, vertex_out_uv);
}
