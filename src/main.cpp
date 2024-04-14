#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

#include "mygl/context.hpp"
#include "game_scene.hpp"
#include "menu_scene.hpp"

int main()
{
    Context ctx;
    glfwSwapInterval(1); //vsync

    MenuScene menu(ctx); //idx 0
    GameScene scene(ctx); // idx 1

    ctx.load_scene(ctx.scenes[0]);
    ctx.run();

    glfwTerminate();
    return 0;
}