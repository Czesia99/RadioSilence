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

        Enemy(Map &map) : map(map)
        {
            stbi_set_flip_vertically_on_load(false);
            model = Model("../assets/models/enemy/monster.obj");
            initial_pos = map.enemy_position;
            model.transform.position = map.enemy_position;
            model.transform.position.y += 0.3;
            model.transform.scale *= 0.1f;

            // change_direction(LEFT);
            // model.transform.rotation.y += glm::radians(90.0f);
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

        void process_movement(Enemy_Movement direction)
        {
            float velocity = movement_speed * clock.delta_time;
            if (direction == FORWARD) 
            {
                model.transform.position += front * velocity;
            }

        }
        void move_forward()
        {
            float velocity = movement_speed * clock.delta_time;
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

        // }

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

        void update_enemy_vectors()
        {
            // glm::vec3 nfront;

        }
};