#version 460 core

out vec4 FragColor;
in vec2 TexCoords;
in vec3 ourPosition;

//uniform float time;
uniform sampler2D texture1;

void main()
{
   FragColor = texture(texture1, TexCoords);
}