#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "mygl/context.hpp"
#include "game_scene.hpp"
#include "menu_scene.hpp"

int main()
{
    Context ctx;
    glfwSwapInterval(1); //vsync

    MenuScene menu(ctx);
    GameScene scene(ctx);

    ctx.load_scene(ctx.scenes[0]);
    ctx.run();

    glfwTerminate();
    return 0;
}