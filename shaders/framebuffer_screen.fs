#version 460 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

uniform float time;
uniform float noise_intensity;

float noise(vec2 pos, float evolve) {
    
    // Loop the evolution (over a very long period of time).
    float e = fract((evolve*0.01));
    
    float cx  = pos.x*e;
    float cy  = pos.y*e;
    
    return fract(23.0*fract(2.0/fract(fract(cx*2.4/cy*23.0+pow(abs(cy/22.4),3.3))*fract(cx*evolve/pow(abs(cy),0.050)))));
}

void main()
{
    vec3 noise_color = vec3(noise(gl_FragCoord.xy, time));
    vec3 col = texture(screenTexture, TexCoords).rgb;
    vec3 final = mix(col, noise_color, noise_intensity);
    FragColor = vec4(final, 1.0);
} 