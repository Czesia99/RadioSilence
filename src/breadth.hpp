#pragma once

#include "map.hpp"
#include <queue>
#include <unordered_map>
#include "glm/gtx/hash.hpp"

inline bool is_valid(const std::vector<std::vector<char>> &txt_map, int x, int y)
{
    int rows = txt_map.size();
    int cols = txt_map[0].size();
    return (x >= 0 && x < rows && y >= 0 && y < cols && txt_map[x][y] == ' ');
}

inline void print_path(std::vector<glm::ivec2> path)
{
    std::cout << "path size " << path.size() << std::endl;
    for (auto &way : path)
    {
        std::cout << "way x = " << way.x << "way y = " << way.y << std::endl;
    }
}

inline std::vector<glm::ivec2> breadth(Map &map, glm::ivec2 start_pos, glm::ivec2 end_pos)
{
    std::queue<glm::ivec2> frontier;
    std::unordered_map<glm::ivec2, glm::ivec2> came_from;

    frontier.push(start_pos);
    glm::ivec2 furthest = start_pos;

    do {
        glm::ivec2 current = frontier.front();
        frontier.pop();

        if (current == end_pos)
        {
            break;
        }

        const std::vector<glm::ivec2> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        bool found_next = false;

        for (int i = 0; i < directions.size(); i++)
        {
            glm::ivec2 next(current + directions[i]);

            if (!is_valid(map.txt_map, next.x, next.y))
                continue;

            if (came_from.find(next) == came_from.end()) // if doesn't exist
            {
                frontier.push(next);
                came_from[next] = current;
                found_next = true;
            }
        }
    } while (!frontier.empty());

    glm::ivec2 current = end_pos;
    std::vector<glm::ivec2> path;

    if (came_from.find(end_pos) == came_from.end())
        return path;

    while(current != start_pos)
    {
        path.push_back(current);
        current = came_from[current];
    }

    std::reverse(path.begin(), path.end());
    // print_path(path);
    return path;
}