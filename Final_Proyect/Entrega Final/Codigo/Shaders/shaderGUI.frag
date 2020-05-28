#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D textura;
uniform bool derrota;
void main()
{
vec4 texturaColor=texture(textura, TexCoords);
if(texturaColor.w<0.1 && derrota==false)
{
	discard;
}
if(texturaColor.w<0.1 && derrota==true)
{
	texturaColor=vec4(1.0,0.0,0.0,0.6);
}
FragColor = texturaColor;
}
