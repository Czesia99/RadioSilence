#include <iostream>
#include <fstream>
#include <vector>

#include "mygl/shape.hpp"

class Map {
    public:
        std::vector<std::vector<char>> txt_map;
        std::vector<Cube> walls;
        Cube floor;

        Map()
        {
            read_map_file("../map.txt");
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
            floor.transform.scale.x *= txt_map[0].size();
            floor.transform.scale.z *= txt_map.size();
            floor.transform.scale.y *= 0.1;
            floor.transform.position.x += txt_map[0].size() / 2;
            floor.transform.position.z += txt_map.size() / 2;
            floor.transform.position.y -= 0.5;

            for (const auto &row : txt_map)
            {
               for (const auto &element : row)
               {
                    if (element == '#')
                    {
                        Cube cube;
                        cube.add_texture("../textures/brick_wall.jpg", cube.diffuse_texture);
                        cube.transform.position = position;
                        walls.push_back(cube);
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
            for (auto &wall : walls)
            {
                wall.render(shader, camera);
            }
            floor.render(shader, camera);
        }

    private:
};