#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D textura_minimap;

void main()
{
FragColor = vec4(texture(textura_minimap, TexCoords).rgb,1.0);
}
