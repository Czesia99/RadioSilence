#pragma once

#include "shader.h"
#include "shape.hpp"
#include "icamera.hpp"

class Button {
    public:
        MyRectangle shape;
        unsigned int texture;
        // Transform transform;

        bool hover = false;

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

        bool is_hovered(double xpos, double ypos) 
        {
            if (xpos >= shape.transform.position.x - shape.transform.scale.x / 2 && 
                    xpos <= shape.transform.position.x + shape.transform.scale.x / 2 && 
                    ypos >= shape.transform.position.y - shape.transform.scale.y / 2 && 
                    ypos <= shape.transform.position.y + shape.transform.scale.y / 2) {hover = true;}
            else hover = false;

            return hover;
        }

    private:
        float button_posx;
        float button_posy;
        float button_width;
        float button_height;
};