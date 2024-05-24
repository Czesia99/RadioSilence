#pragma once

#include "map.hpp"
#include <queue>
#include <unordered_map>
#include <map>

struct KeyFuncs
{
    size_t operator()(const glm::ivec2& k)const
    {
        return std::hash<int>()(k.x) ^ std::hash<int>()(k.y);
    }

    bool operator()(const glm::ivec2& a, const glm::ivec2& b)const
    {
        return a.x == b.x && a.y == b.y;
    }
};

inline bool is_valid(const std::vector<std::vector<char>> &txt_map, int x, int y)
{
    int rows = txt_map.size();
    int cols = txt_map[0].size();
    // std::cout << "rows" << rows << std::endl;
    // std::cout << "cols" << cols << std::endl;
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
    std::cout << "in breadth" << std::endl;
    std::queue<glm::ivec2> frontier;
    std::unordered_map<glm::ivec2, glm::ivec2, KeyFuncs, KeyFuncs> came_from;

    frontier.push(start_pos);
    glm::ivec2 furthest = start_pos;

    do {
        glm::ivec2 current = frontier.front();
        frontier.pop();

        if (current == end_pos)
            break;

        const std::vector<glm::ivec2> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        bool found_next = false;
        // for (const auto &dir : directions)
        for (int i = 0; i < directions.size(); i++)
        {
            glm::ivec2 next(current + directions[i]);

            if (!is_valid(map.txt_map, next.x, next.y))
                continue;

            if (came_from.find(next) == came_from.end()) // if doesn't exist
            {
                std::cout << "doesn't exits" << std::endl;
                frontier.push(next);
                came_from[next] = current;
                found_next = true;
            }
        }

        if (!found_next)
        {
            end_pos = current;
            std::cout << "Dead end at " << current.x << ", " << current.y << std::endl;
        };

    } while (!frontier.empty());

    glm::ivec2 current = end_pos;
    std::vector<glm::ivec2> path;

    while(current != start_pos)
    {
        path.push_back(current);
        current = came_from[current];
    }
    std::reverse(path.begin(), path.end());
    print_path(path);
    return path;
}