#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

#include "mygl/context.hpp"
#include "game_scene.hpp"
#include "menu_scene.hpp"
#include "credits_scene.hpp"
#include "ycube_scene.hpp"
#include "instructions_scene.hpp"

int main()
{
    Context ctx;
    glfwSwapInterval(1); //vsync

    YCubeScene splashscreen(ctx); // idx 0
    InstructionsScene instructions(ctx); //idx 1
    MenuScene menu(ctx); //idx 2
    GameScene scene(ctx); // idx 3
    CreditsScene credits(ctx); //idx 4
    

    ctx.load_scene(ctx.scenes[0]);
    ctx.run();

    glfwTerminate();
    return 0;
}