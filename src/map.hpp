#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "mygl/shape.hpp"

class Map {
    public:
        std::vector<std::vector<char>> txt_map;
        std::vector<glm::vec3> walls_position;
        glm::vec3 player_position = {0.0f, 0.5f, 0.0f};
        glm::vec3 player_start_position = {0.0f, 0.5f, 0.0f};
        glm::vec3 win_position;
        glm::vec3 statue_position;
        glm::vec3 enemy_position;

        Map()
        {
            read_map_file("../assets/map2.txt");
            statue = Model("../assets/models/statue2/untitled2.obj");
            enemy = Model("../assets/models/enemy/monster.obj");
            stbi_set_flip_vertically_on_load(true);
            wall = Model("../assets/models/wall/wall.obj");
            cage = Model("../assets/models/cage/Cage.obj");
        }
        
        void render(Shader shader, const ICamera &camera)
        {
            for (auto &pos : walls_position)
            {
                wall.transform.position = pos;
                wall.draw(shader, camera);
            }

            cage.draw(shader, camera);
            statue.draw(shader, camera);
            floor.render(shader, camera);
            roof.render(shader, camera);
            enemy.draw(shader, camera);
            glm::vec3 direction_to_player = glm::normalize(player_position - statue_position);
            float angle = atan2(direction_to_player.x, direction_to_player.z) - glm::radians(+90.0f);
            statue.transform.rotation.y = angle;
        }

        void load_map() 
        {
            glm::vec3 position = {0.0f, 0.0f, 0.0f};
            floor.add_texture("../assets/textures/concrete2.jpg", floor.diffuse_texture);
            floor.transform.scale.x *= txt_map[0].size();
            floor.transform.scale.z *= txt_map.size();
            floor.transform.scale.y *= 0.1f;
            floor.transform.position.x += txt_map[0].size() / 2;
            floor.transform.position.z += txt_map.size() / 2;

            roof.transform.scale.x *= txt_map[0].size();
            roof.transform.scale.z *= txt_map.size();
            roof.transform.scale.y *= 0.1f;
            roof.transform.position.x += txt_map[0].size() / 2;
            roof.transform.position.z += txt_map.size() / 2;
            roof.transform.position.y += 2.0f;

            for (const auto &row : txt_map)
            {
               for (const auto &element : row)
               {
                    if (element == '#')
                    {
                        walls_position.push_back(position);
                    }
                    if (element == '@')
                    {
                        player_start_position = {position.x, player_position.y, position.z};
                    }
                    if (element == 'w')
                    {
                        win_position = position;
                    }
                    if (element == 'S')
                    {
                        statue_position = {position.x, 0.03, position.z};
                    }
                    if (element == '&')
                    {
                        enemy_position = position;
                    }
                    position.x += 1.0f;
               }
               position.x = 0.0f;
               position.z += 1.0f;
            }
            cage.transform.position = win_position;
            cage.transform.scale *= 0.5f;
            statue.transform.position = statue_position;
            statue.transform.scale *= 0.4f;

            enemy.transform.position = enemy_position;
            enemy.transform.position.y += 0.3;
            enemy.transform.scale *= 0.1f;
            std::cout << "size x = " << txt_map[0].size() << std::endl;
            std::cout << "size y = " << txt_map.size() << std::endl;
        }

        void print_map_txt()
        {
            std::cout << "in print map txt" << std::endl; 
            for (int i = 0; i < txt_map.size(); i++)
            {
                for (int j = 0; j < txt_map[i].size(); j++)
                {
                    std::cout << txt_map[i][j];
                }
                std::cout << std::endl;
            }
        }
    private:
        Model cage;
        Model wall;
        Model statue;
        Model enemy;
        Cube floor;
        Cube roof;

        void read_map_file(const char *path)
        {
            std::string line;
            std::ifstream infile(path);

            if (!infile.is_open())
            {
                std::cout << "can't open file" << std::endl;
                exit(-1);
            }

            while (std::getline(infile, line)) {
                std::vector<char> row;

                for (char &c : line) {
                    row.push_back(c);
                }

                txt_map.push_back(row);
            }
        }
};