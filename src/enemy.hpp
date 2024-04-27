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
        float movement_speed = 0.1f;
        float velocity;
        float direction_timer;
        float duration_interval = 30.0f;
        bool direction_called = false;
        glm::vec3 position;
        Model model;
        glm::vec3 initial_pos;
        std::vector<glm::ivec2> path;
        bool call_one = false;
        int it = 0;
        float angle;

        Enemy(Map &map) : map(map)
        {
            stbi_set_flip_vertically_on_load(false);
            model = Model("../assets/models/enemy/monster.obj");
            initial_pos = map.enemy_start_position;
            model.transform.position = map.enemy_start_position;
            model.transform.position.y += 0.3;
            model.transform.scale *= 0.1f;
            change_direction(LEFT);
            change_direction(LEFT);
            // change_direction(LEFT);
            // std::cout << "map char = " << map.txt_map[3][6] << std::endl;
            path = breadth(map,tile_pos(model.transform.position), {18, 12});
        }

        void render(Shader shader, Camera3D &camera)
        {
            model.draw(shader, camera);
        }

        void update()
        {
            clock.update();
            // compute_direction();
            compute_direction2();
            map.enemy_position = model.transform.position;
            // glm::ivec2 tile = tile_pos(model.transform.position);
            // std::cout << "tile x: " << tile.x << " tile z: " << tile.y << std::endl;
            // std::cout << "enemy pos x: " << model.transform.position.x << " enemy pos z: " << model.transform.position.y << std::endl;
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

        void compute_direction2()
        {
            glm::vec3 forward = glm::normalize(front);
            glm::ivec2 currentDirection = {forward.z, forward.x };
            glm::ivec2 pos = tile_pos(model.transform.position);
            // glm::ivec2 target = glm::normalize(path[it]);
            glm::vec2 direction = (path[it] - pos);
            angle = (atan2(direction.y, direction.x));
            std::cout << "angle = " << glm::degrees(angle) << std::endl;
            // glm::ivec2 targetDirection = path[it] - pos;
            // glm::ivec2 rotatedDirection = { currentDirection.y, -currentDirection.x};
            // std::cout << "rotated direction x: " << rotatedDirection.x << " y: " << rotatedDirection.y << std::endl;
            // int dotProduct = targetDirection.x * rotatedDirection.x + targetDirection.y * rotatedDirection.y;
            //glm::dot(targetDirection, rotatedDirection);
            // move_forward();
            // std::cout << "target direction x: " << targetDirection.x << " , y: " << targetDirection.y << std::endl;
            if (on_tile(path[it])) {
                it++;
                call_one = false;
                std::cout << "TRUE" << std::endl;
                std::cout << "IT = " << it << std::endl;
                std::cout << " PATH IT = " << path[it].x << " , "<< path[it].y << std::endl;
                // angle = atan2(direction.x, direction.y);
                move_forward();
            }
            else {
                move_forward();
                // std::cout << "FALSE" << std::endl;
            }

            if (call_one == false) {
                call_one = true;
                std::cout << "angle = " << glm::degrees(angle) << std::endl;
                if (glm::degrees(angle) == 45.0f) { //&& glm::degrees(angle) < 135.0f) {
                    std::cout << "right" << std::endl;
                    change_direction(RIGHT);
                    return;
                } else if (glm::degrees(angle) <= -45.0f && glm::degrees(angle) > -135.0f) {
                    std::cout << "left" << std::endl;
                    change_direction(LEFT);
                    return;
                } else if (glm::degrees(angle) >= 135.0f || glm::degrees(angle) < -135.0f) {
                    std::cout << "backward" << std::endl;
                    return;
                } else {
                    std::cout << "forward" << std::endl;
                    return;
                }
            }
            // if (glm::distance(glm::vec2(path[it]), {model.transform.position.z, model.transform.position.x}) >= 0.1f)
            // move_forward();
            // if (call_one == false) {
            //     if (glm::degrees(angle) == 90) {
            //         std::cout << "right" << std::endl;
            //         change_direction(RIGHT);
            //         call_one = true;
            //         return;
            //     } else if (glm::degrees(angle) ==  ) {
            //         std::cout << "left" << std::endl;
            //         std::cout << "dotproduct" << dotProduct << std::endl;
            //         change_direction(LEFT);
            //         call_one = true;
            //         return;
            //     } else {
            //         std::cout << "forward" << std::endl;
            //         std::cout << "dotproduct" << dotProduct << std::endl;
            //         change_direction(FORWARD);
            //         call_one = true;
            //         return;
            //     }
            // }
            // if (call_one == false)
            // {
            //     pos = tile_pos(model.transform.position);
            //     if (path[it].x < pos.x) {
            //         std::cout << "left" << std::endl;
            //         change_direction(LEFT);
            //     } else if (path[it].x > pos.x) {
            //         std::cout << "right" << std::endl;
            //         change_direction(RIGHT);
            //     } else if (path[it].y < pos.y) {
            //         std::cout << "backward" << std::endl;
            //         change_direction(BACKWARD);
            //     } else {
            //         std::cout << "forward" << std::endl;
            //         change_direction(FORWARD);
            //     }
            // }
        }

        bool on_tile(glm::ivec2 path_tile)
        {
            std::cout << "path tile: " << path_tile.x << " , " << path_tile.y << std::endl;
            float offset = 0.001f;
            float min_x = path_tile.y - offset;
            float max_x = path_tile.y + offset;
            float min_z = path_tile.x - offset;
            float max_z = path_tile.x + offset;
    
            if (model.transform.position.x >= min_x && model.transform.position.x <= max_x && model.transform.position.z >= min_z && model.transform.position.z <= max_z)
            {
                call_one = false;
                return true;
            }
            return false;
        }

        glm::ivec2 tile_pos(glm::vec3 pos)
        {
            // std::cout << "tile pos x = " << pos.z<< "tile pos y" << pos.x << std::endl;
            // std::cout << "tile pos x = " << int(pos.z / 1) * 1 << "tile pos y" << int(pos.x / 1) * 1 << std::endl;
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