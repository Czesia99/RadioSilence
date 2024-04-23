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
    // std::cout << "rows = " << rows << std::endl;
    int cols = txt_map[0].size();
    // std::cout << "cols = " << cols << std::endl;
    return (x > 0 && x < rows && y > 0 && y < cols && txt_map[x][y] == ' ');
}

inline bool is_destination(const Node &current, const Node &end)
{
    return current.n_pos == end.n_pos;
}

inline std::vector<glm::ivec2> get_path(Node *current_node)
{
    std::vector<glm::ivec2> path;
    std::cout << "CURRENT x: " << current_node->n_pos.x << std::endl;
    std::cout << "PARENT = " << current_node->parent << std::endl;
    std::cout << "PARENT = " << current_node->parent->n_pos.x << std::endl;
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
    std::cout << "path size " << path.size() << std::endl;
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
    std::cout << "start node x: " << start_node.n_pos.x << " y:" << start_node.n_pos.y << std::endl;
    open.push_back(&start_node);

    while (!open.empty())
    {
        Node *current_node = open[0];

        for (auto &n : open)
        {
            if (n->f_cost < current_node->f_cost || n->f_cost == current_node->f_cost && n->h_cost < current_node->h_cost)
                current_node = n;
        }

        closed.push_back(current_node);
        open.erase(std::remove(open.begin(), open.end(), current_node), open.end());



    }

    std::cout << "path not found" << std::endl;
    return {};
}