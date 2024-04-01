#pragma once

#include "shader.h"
#include "shape.hpp"
#include "icamera.hpp"

class Button {
    public:
        Rectangle shape;
        // Transform transform;

    public:
        Button(float posx = 0, float posy = 0, float width = 150, float height = 100) : button_posx(posx), button_posy(posy), button_width(width), button_height(height)
        {
            shape.transform.position.x = button_posx;
            shape.transform.position.y = button_posy;
            shape.transform.scale.x = button_width;
            shape.transform.scale.y = button_height;
        };

        void render(Shader shader, const ICamera &camera) 
        {
            shape.render(shader, camera);
        };

        bool is_clicked(double xpos, double ypos) 
        {
            return (xpos >= shape.transform.position.x - shape.transform.scale.x / 2 && 
                    xpos <= shape.transform.position.x + shape.transform.scale.x / 2 && 
                    ypos >= shape.transform.position.y - shape.transform.scale.x / 2 && 
                    ypos <= shape.transform.position.y + shape.transform.scale.x / 2);
        }

    private:
        float button_posx;
        float button_posy;
        float button_width;
        float button_height;
};