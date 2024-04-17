#pragma once

#include "mygl/model.hpp"
#include "mygl/camera_3D.hpp"
#include "map.hpp"

class Enemy 
{
    public:
        Enemy(Map &map)
        {
            stbi_set_flip_vertically_on_load(false);
            model = Model("../assets/models/enemy/monster.obj");
            model.transform.position = map.enemy_position;
            model.transform.position.y += 0.3;
            model.transform.scale *= 0.1f;
        }

        void render(Shader shader, Camera3D &camera)
        {
            model.draw(shader, camera);
        }

    private:
        Model model;
};