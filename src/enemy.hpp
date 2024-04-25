#pragma once

#include "mygl/model.hpp"
#include "mygl/clock.hpp"
#include "map.hpp"
#include "breadth.hpp"
#include <cstdlib>
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
        glm::vec3 position;
        Model model;
        glm::vec3 initial_pos;
        std::vector<glm::ivec2> path;

        Enemy(Map &map) : map(map)
        {
            stbi_set_flip_vertically_on_load(false);
            model = Model("../assets/models/enemy/monster.obj");
            initial_pos = map.enemy_start_position;
            model.transform.position = map.enemy_start_position;
            model.transform.position.y += 0.3;
            model.transform.scale *= 0.1f;
            change_direction(LEFT);
            std::cout << "map char = " << map.txt_map[3][6] << std::endl;
            path = breadth(map,tile_pos(model.transform.position), {18, 12});
        }

        void render(Shader shader, Camera3D &camera)
        {
            model.draw(shader, camera);
        }

        void update()
        {
            // compute_direction();
            map.enemy_position = model.transform.position;
            glm::ivec2 tile = tile_pos(model.transform.position);
            // std::cout << "tile x: " << tile.x << " tile z: " << tile.y << std::endl;
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

        // void compute_direction2()
        // {
        //     for(auto &p : path)
        //     {
        //         glm::ivec2 pos = tile_pos(model.transform.position);
        //         if (p.x < pos.x)
        //         {

        //         }
        //     }
        // }

        void compute_direction()
        {
            glm::vec3 future_pos = model.transform.position;
            glm::vec3 forward_pos = model.transform.position + glm::normalize(front) * 0.5f;
            future_pos += front * velocity;
            
            bool forward_blocked = is_wall(forward_pos, FORWARD);
            bool left_blocked = is_wall(future_pos, LEFT);
            bool right_blocked = is_wall(future_pos, RIGHT);

            if (!forward_blocked) {
                move_forward();
                std::cout << "forward not blocked" << std::endl;
                return;
            }

            // bool left_blocked = is_wall(future_pos, LEFT);
            // bool right_blocked = is_wall(future_pos, RIGHT);

            std::vector<Enemy_Movement> available_directions;

            if (!left_blocked)
                available_directions.push_back(LEFT);
            if (!right_blocked)
                available_directions.push_back(RIGHT);

            if (available_directions.empty()) {
                std::cout << "backward" << std::endl; 
                change_direction(BACKWARD);
                return;
            }
            if (available_directions.size() == 1)
                change_direction(available_directions[0]);
            else {
                srand(static_cast<unsigned int>(time(nullptr)));
                int random_index = rand() % available_directions.size();
                change_direction(available_directions[random_index]);
            }
        }

        bool is_wall(const glm::vec3& future_pos, Enemy_Movement direction)
        {
            glm::vec3 future_pos_side;
            if (direction == LEFT)
                future_pos_side = future_pos - glm::cross(front, glm::vec3(0, 1, 0)) * 0.5f;
            else if (direction == RIGHT)
                future_pos_side = future_pos + glm::cross(front, glm::vec3(0, 1, 0)) * 0.5f;
            else if (direction == FORWARD)
                future_pos_side = future_pos;

            for (auto& wall_pos : map.walls_position)
            {
                float offset = 0.11f;
                float wall_min_x = wall_pos.x - 0.5f - offset;
                float wall_max_x = wall_pos.x + 0.5f + offset;
                float wall_min_z = wall_pos.z - 0.5f - offset;
                float wall_max_z = wall_pos.z + 0.5f + offset;

                if (future_pos_side.x >= wall_min_x && future_pos_side.x <= wall_max_x &&
                    future_pos_side.z >= wall_min_z && future_pos_side.z <= wall_max_z)
                {
                    return true;
                }
            }
            return false;
        }

        glm::ivec2 tile_pos(glm::vec3 pos)
        {
            std::cout << "tile pos x = " << int(pos.x / 1) * 1 << "tile pos y" << int(pos.z / 1) * 1 << std::endl;
            return {int(pos.z / 1) * 1, int(pos.x / 1) * 1}; 
            //3D WORLD X = Y 2D WORLD
            //3D WORLD Z = X 2D WORLD
        }

    private:
        Map &map;
        Clock clock;
        glm::vec3 front = glm::vec3(0.0f, 0.0f, 1.0f);
        glm::vec3 right = glm::vec3(-1.0f, 0.0f, 0.0f);
};