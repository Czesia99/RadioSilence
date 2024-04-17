#pragma once

#include "mygl/shader.h"
#include "mygl/shape.hpp"
#include "mygl/iscene.hpp"
#include "mygl/context.hpp"
#include "mygl/button.hpp"
#include "mygl/skybox.hpp"
#include "mygl/model.hpp"
#include "mygl/clock.hpp"

#include "map.hpp"
#include "player.hpp"
#include "enemy.hpp"

class GameScene : public IScene {
    public:
        GameScene(Context &ctx);
        void store_scene_in_ctx() override;
        void open_scene() override;
        void close_scene() override;
        void update() override;
        void scene_clear() override;
        void process_input() override;
        void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) override;
        void left_click_callback(GLFWwindow* window, int button, int action, int mods) override;
        void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) override;
        void framebuffer_size_callback(GLFWwindow* window, int width, int height) override;

    public:
        Context &ctx;

    private:
        float deltaTime = 0.0f;
        float lastFrame = 0.0f;
        double currentTime = 0.0f;

        float lastX = ctx.win_width / 2.0f;
        float lastY = ctx.win_height / 2.0f;
        bool first_mouse = true;

        bool torchlight = true;
    
        Player *player = nullptr;
        Enemy *enemy = nullptr;

        Map map;
        Shader light_shader;
        Shader map_shader;
        
        Clock clock;
        glm::vec3 light_pos;

        Shader modelShader;
};