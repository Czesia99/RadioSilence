#pragma once

#include "mygl/model.hpp"
#include "mygl/clock.hpp"
#include "map.hpp"

#include <cstdlib> // Include for rand() function
#include <ctime> 

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
        float direction_timer;
        float duration_interval = 30.0f;
        bool direction_called = false;

        Enemy(Map &map) : map(map)
        {
            stbi_set_flip_vertically_on_load(false);
            model = Model("../assets/models/enemy/monster.obj");
            initial_pos = map.enemy_position;
            model.transform.position = map.enemy_position;
            model.transform.position.y += 0.3;
            model.transform.scale *= 0.1f;
            change_direction(LEFT);
        }

        void render(Shader shader, Camera3D &camera)
        {
            model.draw(shader, camera);
        }

        void update()
        {
            // std::cout << "In Enemy update" << std::endl;
            move_forward();
            direction_called = false;
            direction_timer += clock.delta_time;

            if (direction_timer >= duration_interval && !direction_called)
            {
                compute_direction();
                direction_timer = 0.0f;
                direction_called = true;
            }
        }

        void move_forward()
        {
            velocity = movement_speed * clock.delta_time;
            model.transform.position += front * velocity;
            std::cout << "( " << model.transform.position.x << "," << model.transform.position.z << " )" << std::endl;
        }

        void change_direction(Enemy_Movement direction)
        {
            if (direction == FORWARD)
            {
                move_forward();
            }
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

        void compute_direction()
        {
            glm::vec3 future_pos = model.transform.position;
            glm::vec3 forward_pos = model.transform.position;
            future_pos += front * velocity;
            forward_pos += front * velocity + 0.5f;
            

            bool left_blocked = is_wall_on_side(future_pos, LEFT);
            bool right_blocked = is_wall_on_side(future_pos, RIGHT);
            bool forward_blocked = is_wall_on_side(forward_pos, FORWARD);
            std::vector<Enemy_Movement> available_directions;
            if (!forward_blocked)
                available_directions.push_back(FORWARD);
            if (!left_blocked)
                available_directions.push_back(LEFT);
            if (!right_blocked)
                available_directions.push_back(RIGHT);


            if (available_directions.size() == 1)
                change_direction(available_directions[0]);
            else {
                srand(static_cast<unsigned int>(time(nullptr)));
                int random_index = rand() % available_directions.size();
                change_direction(available_directions[random_index]);
            }

            for (auto &directions : available_directions)
            {
                std::cout << "number of directions =  " << available_directions.size() << std::endl;
                std::cout << "direction: " << directions << std::endl;
            }
            std::cout << "left wall = " << left_blocked << std::endl;
            std::cout << "right wall = " << right_blocked << std::endl;
            //std::cout << "forward wall = " << forward_blocked << std::endl;



        }

        bool is_wall_on_side(const glm::vec3& future_pos, Enemy_Movement direction)
        {
            // Calculate future position on the side specified by the direction
            glm::vec3 future_pos_side;
            if (direction == LEFT)
                future_pos_side = future_pos - glm::cross(front, glm::vec3(0, 1, 0)) * 0.5f;
            else if (direction == RIGHT)
                future_pos_side = future_pos + glm::cross(front, glm::vec3(0, 1, 0)) * 0.5f;
            std::cout << direction << std::endl;
            std::cout << future_pos_side.x << ", " << future_pos_side.z << std::endl;
            std::cout << "enemy pos = " << model.transform.position.x << ", " << model.transform.position.z << std::endl;
            // Check if there's a wall on the specified side
            for (auto& wall_pos : map.walls_position)
            {
                float offset = 0.5f;
                float wall_min_x = wall_pos.x - 0.5f; // Adjust with wall size if needed
                float wall_max_x = wall_pos.x + 0.5f;
                float wall_min_z = wall_pos.z - 0.5f;
                float wall_max_z = wall_pos.z + 0.5f;

                // Check if future position on the specified side intersects with the wall
                if (future_pos_side.x >= wall_min_x && future_pos_side.x <= wall_max_x &&
                    future_pos_side.z >= wall_min_z && future_pos_side.z <= wall_max_z)
                {
                    // Wall on the specified side
                    return true;
                }
            }
            return false;
        }

    private:
        Model model;
        Map &map;
        Clock clock;
        glm::vec3 front = glm::vec3(0.0f, 0.0f, 1.0f);
        glm::vec3 right = glm::vec3(-1.0f, 0.0f, 0.0f);
        glm::vec3 position;
        glm::vec3 initial_pos;
};