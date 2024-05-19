#version 460 core

out vec4 FragColor;
in vec2 TexCoords;
in vec3 ourPosition;

//uniform float time;
uniform sampler2D texture0;
uniform bool hovered;

void main()
{
   vec4 color = texture(texture0, TexCoords);

   if (hovered) {
      color.rgb *= 1.5;
   } else {color.rgb *= 1.0f;}

   FragColor = color;
}