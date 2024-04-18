#pragma once

#include "mygl/model.hpp"
#include "mygl/clock.hpp"
#include "map.hpp"

class Enemy 
{
    enum Enemy_Movement  {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    public:
        float movement_speed = 0.01f;
        float velocity;

        Enemy(Map &map) : map(map)
        {
            stbi_set_flip_vertically_on_load(false);
            model = Model("../assets/models/enemy/monster.obj");
            initial_pos = map.enemy_position;
            model.transform.position = map.enemy_position;
            model.transform.position.y += 0.3;
            model.transform.scale *= 0.1f;
        }

        void render(Shader shader, Camera3D &camera)
        {
            model.draw(shader, camera);
        }

        void update()
        {
            // std::cout << "In Enemy update" << std::endl;
            move_forward();
        }

        void move_forward()
        {
            velocity = movement_speed * clock.delta_time;
            model.transform.position += front * velocity;
        }

        void change_direction(Enemy_Movement direction)
        {
            if (direction == BACKWARD) 
            {
                model.transform.rotation.y += glm::radians(180.0f);
                front = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(front, 1.0f));
            }
            if (direction == LEFT) 
            {
                model.transform.rotation.y += glm::radians(90.0f);
                front = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(front, 1.0f));
            }
            if (direction == RIGHT) 
            {
                model.transform.rotation.y += glm::radians(-90.0f);
                front = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(front, 1.0f));
            }
        }

        // void compute_direction()
        // {
        //     glm::vec3 future_pos = model.transform.position;
        //     future_pos += front * velocity;

        //     for (auto &wall_pos : map.walls_position)
        //     {
        //         float offset = 1.0f;
        //         float min_x = wall_pos.x - 0.5f - offset;
        //         float max_x = wall_pos.x + 0.5f + offset;
        //         float min_z = wall_pos.z - 0.5f - offset;
        //         float max_z = wall_pos.z + 0.5f + offset;

        //         if (future_pos.x >= min_x && future_pos.x <= max_x && future_pos.z >= min_z && future_pos.z <= max_z)
        //         {
        //             // return true;
        //         }
        //     }
        // }

        bool detect_path()
        {

        }
        // bool intersection()
        // {
        //     return true;
        // }

    private:
        Model model;
        Map &map;
        Clock clock;
        glm::vec3 front = glm::vec3(0.0f, 0.0f, 1.0f);
        glm::vec3 right = glm::vec3(-1.0f, 0.0f, 0.0f);
        glm::vec3 position;
        glm::vec3 initial_pos;
};