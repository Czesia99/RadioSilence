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

        Node(Node *parent, glm::ivec2 pos) : parent(parent), n_pos(pos) {}

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
    // std::cout << "rows" << rows << std::endl;
    // std::cout << "cols" << cols << std::endl;
    return (x >= 0 && x < rows && y >= 0 && y < cols && txt_map[y][x] == ' ');
}

inline bool is_destination(const Node &current, const Node &end)
{
    return current.n_pos == end.n_pos;
}

inline std::vector<glm::ivec2> get_path(Node *current_node)
{
    // std::cout << "get path" << std::endl;
    std::vector<glm::ivec2> path;
    while (current_node != nullptr)
    {
        path.push_back(current_node->n_pos);
        current_node = current_node->parent;
    }
    path.pop_back();
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

    std::cout << "start node x: " << start_node.n_pos.x << " y:" << start_node.n_pos.y << std::endl;

    open.push_back(&start_node);

    while (!open.empty())
    {
        Node *current_node = open[0];
        int current_index = 0;
        std::cout << "current_node pos =" << current_node->n_pos.x << " , " << current_node->n_pos.y << std::endl;
        for (int i = 0; i < open.size(); i++)
        {
            if (open[i]->f_cost < current_node->f_cost)
            {
                current_node = open[i];
                current_index = i;
            } //|| n->f_cost == current_node->f_cost && n->h_cost < current_node->h_cost)
        }

        // open.erase(std::remove(open.begin(), open.end(), current_node), open.end());
        open.erase(open.begin() + current_index);
        closed.push_back(current_node);

        if (is_destination(*current_node, end_node))
        {
            std::cout << "path found" << std::endl;
            print_path(get_path(current_node));
            return (get_path(current_node));
        }

        const std::vector<glm::ivec2> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        std::vector<Node*> children;
        for (const auto &dir : directions)
        {
            glm::ivec2 neighbor_pos(current_node->n_pos + dir);

            if (!is_valid(map.txt_map, neighbor_pos.x, neighbor_pos.y)) {
                std::cout << "not valid:" <<  map.txt_map[neighbor_pos.y][neighbor_pos.x]<< std::endl;
                continue;
            }
            
            Node *neighbor = new Node(current_node, neighbor_pos);

            bool should_continue3  = false;
            for (auto &n : closed)
            {
                if (neighbor->n_pos == n->n_pos) {
                    should_continue3 = true;
                    break;
                }
            }

            if (should_continue3)
                continue;

            children.push_back(neighbor);
        }
        
        for (auto &child : children)
        {
            bool should_continue  = false;
            for (auto &n : closed)
            {
                if (child->n_pos == n->n_pos) {
                    should_continue = true;
                    break;
                }
            }

            if (should_continue)
                continue;
            // if (std::find(closed.begin(), closed.end(), child) != closed.end())
            // {
            //     continue;
            // }

            child->g_cost = current_node->g_cost + 1;
            child->h_cost = child->hcost(child->n_pos, end_pos);
            child->f_cost = child->fcost();

            // auto it = std::find(open.begin(), open.end(), child);
            bool should_continue2 = false;
            for (auto &n : open)
            {
                if (child->n_pos == n->n_pos && child->g_cost > n->g_cost)
                {
                    should_continue2 = true;
                    break;
                }

                if (should_continue2)
                    continue;
            }

            // if (it != open.end())
            // {
            //     if (child->g_cost > (*it)->g_cost)
            //     {
            //         continue;
            //     }
            // }
            open.push_back(child);
        }
    }

    std::cout << "path not found" << std::endl;
    return {};
}