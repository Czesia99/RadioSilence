#include "instructions_scene.hpp"

InstructionsScene::InstructionsScene(Context &ctx) : ctx(ctx)
{
    camera = CameraOrtho(glm::vec3(0.0f, 0.0f, 0.0f), ctx.win_width, ctx.win_height);

    stbi_set_flip_vertically_on_load(true);
    load_texture("../assets/textures/instructions.png", instructions_texture);
    shape.transform.scale.x = ctx.win_width;
    shape.transform.scale.y = ctx.win_height;
    credits_shader = Shader("button.vs", "button.fs");


    store_scene_in_ctx();
}

void InstructionsScene::store_scene_in_ctx()
{
    ctx.scenes.push_back(this);
}

void InstructionsScene::open_scene() 
{
    glfwSetInputMode(ctx.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    float bg_ar = 800.0f / 600.0f;
    float scale_factor;

    if (ctx.aspect_ratio > bg_ar) {
        scale_factor = ctx.win_height / 800.0f;
    }
    else {
        scale_factor = ctx.win_width / 600.0f;
    }

    shape.transform.scale.x = 800.0f * scale_factor;
    shape.transform.scale.y = 600.0f * scale_factor;

    clock.update();
    start_time = clock.current_time;
    std::cout << "start time instructions = " << start_time << std::endl;
}

void InstructionsScene::close_scene() { return; }

void InstructionsScene::update()
{
    clock.update();

    std::cout << "clock current = " << clock.current_time << std::endl;
    if (clock.current_time - start_time >= 6.0f)
    {
        ctx.load_scene_id(2);
    }

    credits_shader.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, instructions_texture);
    credits_shader.set_int("texture0", 0);
    shape.render(credits_shader, camera);
}

void InstructionsScene::scene_clear()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void InstructionsScene::process_input()
{
    // if (glfwGetKey(ctx.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    //     glfwSetWindowShouldClose(ctx.window, true);
}

void InstructionsScene::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) 
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    xpos -= ctx.win_width / 2;
    ypos = ctx.win_height - ypos;
    ypos -= ctx.win_height / 2;
}

void InstructionsScene::left_click_callback(GLFWwindow* window, int button, int action, int mods) 
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    xpos -= ctx.win_width / 2;
    ypos = ctx.win_height - ypos;
    ypos -= ctx.win_height / 2;
    // if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    //     ctx.load_scene_id(2);
    // }
}

void InstructionsScene::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {}

void InstructionsScene::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    camera.width = width;
    camera.height = height;
    ctx.win_width = width;
    ctx.win_height = height;
}