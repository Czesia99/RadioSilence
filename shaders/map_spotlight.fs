#version 460 core

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    sampler2D flashlight;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

uniform vec2 viewPort;

uniform float time;

// float noise(vec2 pos, float evolve) {
    
//     // Loop the evolution (over a very long period of time).
//     float e = fract((evolve*0.01));
    
//     float cx  = pos.x*e;
//     float cy  = pos.y*e;
    
//     return fract(23.0*fract(2.0/fract(fract(cx*2.4/cy*23.0+pow(abs(cy/22.4),3.3))*fract(cx*evolve/pow(abs(cy),0.050)))));
// }

void main()
{
    // ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    // spotlight (soft edges)
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    //flashlight cookie mask
    float ratio = viewPort.x / viewPort.y;
    vec2 uv = (2.0 * gl_FragCoord.xy) / viewPort.xy - 1.0;
    uv.x *= ratio;
    uv *= 0.5;
    uv += 0.5;

    intensity *= length(vec3(texture(light.flashlight, uv)));

    diffuse  *= intensity;
    specular *= intensity;
    
    // attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;


    vec3 result = ambient + diffuse + specular;

    float color_num = 20;
    vec3 final = floor(result * (color_num - 1.0) + 0.5) / (color_num - 1.0);

    // vec3 noise_color = vec3(noise(gl_FragCoord.xy, time));
    // mix(final, noise_color, 0.5)
    FragColor = vec4(final, 1.0);
}