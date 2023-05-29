#version 330

uniform sampler2D sampler2d;

in  vec2 texture_uv;
out vec4 fragment_color;

void main()
{
    fragment_color = vec4(texture(sampler2d, texture_uv.st).rgb, 1.0);
}
