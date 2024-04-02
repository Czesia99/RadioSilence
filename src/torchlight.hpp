#include "mygl/shader.h"
#include "mygl/icamera.hpp"

class Torchlight
{
    public:
        Torchlight(Shader &shader, const ICamera &camera)
        {
            shader = shader;
        };

        void run()
        {
            shader.use();
            
            //spotlight properties
            shader.set_vec3("light.position", camera.position);
            shader.set_vec3("light.direction", camera.front);
            shader.set_float("light.cutOff",   glm::cos(glm::radians(12.5f)));
            shader.set_float("light.outerCutOff", glm::cos(glm::radians(17.5f)));

            shader.set_vec3("viewPos", camera.position);

            // light properties
            if (on) 
            {
                shader.set_vec3("light.ambient", 0.05f, 0.05f, 0.05f);
                shader.set_vec3("light.diffuse", 0.8f, 0.8f, 0.8f);
                shader.set_vec3("light.specular", 1.0f, 1.0f, 1.0f);
            }

            if (!on)
            {
                shader.set_vec3("light.ambient", 0.0f, 0.0f, 0.0f);
                shader.set_vec3("light.diffuse", 0.0f, 0.0f, 0.0f);
                shader.set_vec3("light.specular", 1.0f, 1.0f, 1.0f);
            }

            shader.set_float("light.constant", 1.0f);
            shader.set_float("light.linear", 0.22f);
            shader.set_float("light.quadratic", 0.20f);

            //material properties
            shader.set_float("material.shininess", 64.0f);
        }
        bool on;
        Shader shader;
    private:
};