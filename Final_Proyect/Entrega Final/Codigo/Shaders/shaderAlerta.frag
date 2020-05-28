#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform float transparencia;

void main()
{
FragColor = vec4(1.0,0.0,0.0,transparencia);
}
