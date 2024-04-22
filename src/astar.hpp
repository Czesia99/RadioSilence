#pragma once

#include "map.hpp"

class Node
{
    public:
        glm::ivec2 n_pos;
        int g_cost = 0;
        int h_cost = 0;
        int f_cost = 0;
        Node *parent = nullptr;

        Node(Node *parent, glm::ivec2 pos)
        {
            parent = parent;    
            n_pos = pos;
        }

        int fcost()
        {
            return g_cost + h_cost;
        }

        int hcost(const glm::ivec2 &a, const glm::ivec2 &b)
        {
            return std::abs(a.x - b.x) + std::abs(a.y - b.y);
        }
};

inline bool is_valid(const std::vector<std::vector<char>> &txt_map, int x, int y)
{
    int rows = txt_map.size();
    int cols = txt_map[0].size();
    return (x > 0 && x < rows && y > 0 && y < cols && txt_map[x][y] == ' ');
}

inline bool is_destination(const Node &current, const Node &end)
{
    return current.n_pos == end.n_pos;
}

inline std::vector<glm::ivec2> get_path(Node *current_node)
{
    std::vector<glm::ivec2> path;
    while (current_node != nullptr)
    {
        path.push_back(current_node->n_pos);
        current_node = current_node->parent;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

inline void print_path(std::vector<glm::ivec2> path)
{
    for (auto &way : path)
    {
        std::cout << "way x = " << way.x << "way y = " << way.y << std::endl;
    }
}

inline std::vector<glm::ivec2> astar(Map &map, glm::ivec2 start_pos, glm::ivec2 end_pos)
{
    Node start_node(NULL, start_pos);
    Node end_node(NULL, end_pos);
    std::vector<Node*> open;
    std::vector<Node*> closed;

    // Node *current_node;

    open.push_back(&start_node);
    while (!open.empty())
    {
        Node *current_node = open[0];
        int index = 0;

        for (int i = 0; i <= open.size(); i++)
        {
            if (open[i]->f_cost < current_node->f_cost )
            {
                current_node = open[i];
                index = i;
            }
        }
        //erase truc
        open.erase(open.begin() + index);
        closed.push_back(current_node);

        if (is_destination(*current_node, end_node))
        {
            std::cout << "Path Found!" << std::endl;
            print_path(get_path(current_node));
            return get_path(current_node);
        }

        const std::vector<glm::ivec2> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

        for (const auto &dir : directions)
        {
            glm::ivec2 neighbor_pos(current_node->n_pos.x + dir.x, current_node->n_pos.y + dir.y);

            if (!is_valid(map.txt_map, neighbor_pos.x, neighbor_pos.y))
                continue;

            Node *neighbor = new Node(current_node, neighbor_pos);

            if (std::find(closed.begin(), closed.end(), neighbor) != closed.end())
                continue;

            neighbor->g_cost = current_node->g_cost + 1;
            neighbor->h_cost = neighbor->hcost(neighbor_pos, end_pos);
            neighbor->f_cost = neighbor->fcost();

            if (std::find(open.begin(), open.end(), neighbor) == open.end() || neighbor->f_cost < current_node->f_cost)
            {
                open.push_back(neighbor);
            }
        }
    }
    std::cout << "path not found" << std::endl;
    return {};
}