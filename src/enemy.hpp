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
        float movement_speed = 0.3f;
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
        glm::ivec2 pos;
        Clock clock;

        Enemy(Map &map) : map(map)
        {
            stbi_set_flip_vertically_on_load(false);
            model = Model("../assets/models/enemy/monster.obj");
            initial_pos = map.enemy_start_position;
            model.transform.position = map.enemy_start_position;
            model.transform.position.y += 0.3;
            model.transform.scale *= 0.1f;
            pos = tile_pos(model.transform.position);
        }

        void render(Shader shader, Camera3D &camera)
        {
            model.draw(shader, camera);
        }

        void init()
        {
            model.transform.position = map.enemy_start_position;
            call_one = false;
            clock.reset();
            it = 0;
            pos = tile_pos(model.transform.position);
            path.clear();
            path = breadth(map, pos, map.random_walkable_pos());
        }

        void update()
        {
            clock.update();
            if (it < path.size()) {
                compute_direction();
            } else {
                path = breadth(map, pos, map.random_walkable_pos());
                call_one = false;
                it = 0;
            }
            
            map.enemy_position = model.transform.position;
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

        void compute_direction()
        {
            glm::vec3 forward = glm::normalize(front);
            glm::vec2 currentDirection = {forward.z, forward.x };
            pos = tile_pos(model.transform.position);
            glm::vec2 direction = (path[it] - pos);
            angle = glm::degrees(atan2(direction.y, direction.x) - atan2(currentDirection.y, currentDirection.x));

            if (on_tile(path[it])) {
                it++;
                call_one = false;
                // pos = path[it];
                std::cout << "TRUE" << std::endl;
                std::cout << "IT = " << it << std::endl;
                std::cout << " PATH IT = " << path[it].x << " , "<< path[it].y << std::endl;
                std::cout << "pos1 x: " << pos.x << " pos1 y: " << pos.y << std::endl;
                std::cout << "path size = " << path.size() << std::endl;
                move_forward();
            }
            else {
                move_forward();
            }

            if (call_one == false) {
                call_one = true;
                direction = (path[it] - pos);
                currentDirection = {forward.z, forward.x };
                float dotProduct = glm::dot(direction, currentDirection);
                std::cout << "pos1 x: " << pos.x << " pos1 y: " << pos.y << std::endl;
                std::cout << "dot product : " << dotProduct << std::endl;
                std::cout << "model pos x: " << model.transform.position.z << " model pos y: " << model.transform.position.x << std::endl;
                if (dotProduct < -0.95f) {
                    std::cout << "backward" << std::endl;
                    change_direction(BACKWARD);
                } else if (dotProduct > 0.95f) {
                    std::cout << "forward" << std::endl;
                    // move_forward();
                } else {
                    float crossProduct = direction.x * currentDirection.y - direction.y * currentDirection.x;
                    if (crossProduct > 0.0f) {
                        std::cout << "right" << std::endl;
                        change_direction(RIGHT);
                    } else {
                        std::cout << "left" << std::endl;
                        change_direction(LEFT);
                    }
                }
            }
        }

        bool on_tile(glm::ivec2 path_tile)
        {
            float offset = 0.01f;
            float min_x = path_tile.y - offset;
            float max_x = path_tile.y + offset;
            float min_z = path_tile.x - offset;
            float max_z = path_tile.x + offset;
    
            if (model.transform.position.x >= min_x && model.transform.position.x <= max_x && model.transform.position.z >= min_z && model.transform.position.z <= max_z)
            {
                call_one = false;
                pos = path_tile;
                return true;
            }
            return false;
        }

        glm::ivec2 tile_pos(glm::vec3 pos)
        {
            return {int(std::round(pos.z / 1)) * 1, int(std::round(pos.x / 1)) * 1 };
            //3D WORLD X = Y 2D WORLD
            //3D WORLD Z = X 2D WORLD
        }

    private:
        Map &map;
        
        glm::vec3 front = glm::vec3(0.0f, 0.0f, 1.0f);
        glm::vec3 right = glm::vec3(-1.0f, 0.0f, 0.0f);
};