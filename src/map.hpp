#include <iostream>
#include <fstream>
#include <vector>

#include "mygl/shape.hpp"

class Map {
    public:
        std::vector<std::vector<char>> txt_map;
        std::vector<glm::vec3> walls_position;
        Cube floor;
        Cube roof;
        glm::vec3 player_position = {0.0f, 0.0f, 0.0f};

        Map()
        {
            read_map_file("../map.txt");
            wall.add_texture("../textures/brick_wall.jpg", wall.diffuse_texture);
        }

        void read_map_file(const char *path)
        {
            std::string line;
            std::ifstream infile(path);

            if (!infile.is_open())
            {
                std::cout << "can't open file" << std::endl;
                return;
            }

            while (std::getline(infile, line)) {
                std::vector<char> row;

                for (char &c : line) {
                    row.push_back(c);
                }

                txt_map.push_back(row);
            }
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

        void load_map() 
        {
            glm::vec3 position = {0.0f, 0.0f, 0.0f};
            floor.add_texture("../textures/concrete2.jpg", floor.diffuse_texture);
            floor.transform.scale.x *= txt_map[0].size();
            floor.transform.scale.z *= txt_map.size();
            floor.transform.scale.y *= 0.1f;
            floor.transform.position.x += txt_map[0].size() / 2;
            floor.transform.position.z += txt_map.size() / 2;
            floor.transform.position.y -= 0.5f;

            roof.transform.scale.x *= txt_map[0].size();
            roof.transform.scale.z *= txt_map.size();
            roof.transform.scale.y *= 0.1f;
            roof.transform.position.x += txt_map[0].size() / 2;
            roof.transform.position.z += txt_map.size() / 2;
            roof.transform.position.y += 1.5f;

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
                        player_position = {position.x, 0, position.z};
                    }
                    position.x += 1.0f;
               }
               position.x = 0.0f;
               position.z += 1.0f;
            }

            std::cout << "size x = " << txt_map[0].size() << std::endl;
            std::cout << "size y = " << txt_map.size() << std::endl;
        }
        
        void render(Shader shader, const ICamera &camera)
        {
            for (auto &pos : walls_position)
            {
                wall.transform.position = pos;
                wall.render(shader, camera);
                wall.transform.position.y += 1.0f;
                wall.render(shader, camera);
            }
            floor.render(shader, camera);
            roof.render(shader, camera);
        }

    private:
        Cube wall;
};