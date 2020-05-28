#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float puntoTam;

void main()
{
    vec4 position=projection * view * model * vec4(aPos, 1.0);
    gl_Position = vec4(position.x, position.y, 0.0, position.w);
    gl_PointSize=puntoTam;
}