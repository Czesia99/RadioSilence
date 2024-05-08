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
        bool scream = false;
        bool see_player = false;

        Enemy(Map &map) : map(map)
        {
            stbi_set_flip_vertically_on_load(false);
            model = Model("../assets/models/enemy/monster.obj");

            model.transform.position = map.enemy_start_position;
            model.transform.position.y += 0.3;
            model.transform.scale *= 0.1f;

            path_pos = tile_pos(model.transform.position);
        }

        void render(Shader shader, Camera3D &camera)
        {
            model.draw(shader, camera);
        }

        void init()
        {
            clock.reset();
            front = glm::vec3(0.0f, 0.0f, 1.0f);
            model.transform.position = map.enemy_start_position;
            model.transform.rotation.y = 0.0f;
            choose_direction = false;
            see_player = false;
            scream = false;
            movement_speed = 0.3f;
            it = 0;
            path_pos = tile_pos(model.transform.position);
            path.clear();
            path = breadth(map, path_pos, map.random_walkable_pos());
        }

        void update()
        {
            clock.update();

            if (scream == true)
            {
                float dist = glm::distance(model.transform.position, map.player_position);
                std::cout << "dist = " << dist << std::endl;
                if (dist >= 0.3f)
                {
                    std::cout << "scream move forward" << std::endl;
                    move_forward();
                }
                return;
            }

            if (it < path.size()) {
                compute_direction();
            } else {
                path = breadth(map, path_pos, map.random_walkable_pos());
                choose_direction = false;
                it = 0;
            }
            
            map.enemy_position = model.transform.position;
        }

        void screamer(Player &player)
        {
            player.torchlight_on = true;
            model.transform.position = player.player_camera.position + player.player_camera.front * 3.0f;
            model.transform.position.y -= 0.2f;
            
            float dist = glm::distance(model.transform.position, player.player_camera.position);
            glm::vec3 direction_to_player = glm::normalize(player.player_camera.position - model.transform.position);
            glm::mat4 rotation_matrix = glm::lookAt(glm::vec3(0.0f), direction_to_player, glm::vec3(0.0f, 1.0f, 0.0f));
            float angle = atan2(rotation_matrix[0][2], rotation_matrix[2][2]) - M_PI;
            model.transform.rotation.y = angle;
            front = -player.player_camera.front;

            movement_speed = 4.0f;
            scream = true;
        }

        glm::ivec2 tile_pos(glm::vec3 pos)
        {
            return {int(std::round(pos.z / 1)) * 1, int(std::round(pos.x / 1)) * 1};
            //3D WORLD X = Y 2D WORLD
            //3D WORLD Z = X 2D WORLD
        }

        void detect_player()
        {
            //add torchlight detection and radio
            std::cout << "==== DETECT PLAYER ====" << std::endl;
            std::cout << "distance p / e = " << glm::distance(map.player_position, map.enemy_position) << std::endl;
            if (glm::distance(map.player_position, map.enemy_position) <= 5.0f)
            {
                std::vector<glm::ivec2> p = breadth(map, path_pos, tile_pos(map.player_position));
                if (p.size() <= 5)
                {
                    see_player = true;
                    // scream = true;
                }
            }
        }

    private:
        Clock clock;
        Map &map;
        Model model;

        glm::vec3 front = glm::vec3(0.0f, 0.0f, 1.0f);
        glm::vec3 right = glm::vec3(-1.0f, 0.0f, 0.0f);

        std::vector<glm::ivec2> path;
        glm::ivec2 path_pos;
        
        float movement_speed = 0.3f;
        float velocity;

        int it = 0;
        bool choose_direction = false;

        void move_forward()
        {
            velocity = movement_speed * clock.delta_time;
            model.transform.position += front * velocity;
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
                choose_direction = false;
                path_pos = path_tile;
                return true;
            }
            return false;
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
            glm::vec2 currentDirection = {forward.z, forward.x};
            glm::vec2 direction = (path[it] - path_pos);

            path_pos = tile_pos(model.transform.position);

            if (on_tile(path[it])) {
                it++;
                choose_direction = false;
                // pos = path[it];
                std::cout << "TRUE" << std::endl;
                std::cout << "IT = " << it << std::endl;
                std::cout << " PATH IT = " << path[it].x << " , "<< path[it].y << std::endl;
                std::cout << "pos1 x: " << path_pos.x << " pos1 y: " << path_pos.y << std::endl;
                std::cout << "path size = " << path.size() << std::endl;
                detect_player();
                move_forward();
            }
            else {
                move_forward();
            }

            if (choose_direction == false) {
                choose_direction = true;
                direction = (path[it] - path_pos);
                currentDirection = {forward.z, forward.x};

                float dotProduct = glm::dot(direction, currentDirection);

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
};