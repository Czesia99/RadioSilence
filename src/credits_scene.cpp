#include "credits_scene.hpp"

CreditsScene::CreditsScene(Context &ctx) : ctx(ctx)
{
    camera = CameraOrtho(glm::vec3(0.0f, 0.0f, 0.0f), ctx.win_width, ctx.win_height);

    stbi_set_flip_vertically_on_load(true);
    load_texture("../assets/textures/credits4.png", credits_texture);
    shape.transform.scale.x = ctx.win_width;
    shape.transform.scale.y = ctx.win_height;
    credits_shader = Shader("button.vs", "button.fs");
    store_scene_in_ctx();
}

void CreditsScene::store_scene_in_ctx()
{
    ctx.scenes.push_back(this);
}

void CreditsScene::open_scene() 
{
    glfwSetInputMode(ctx.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void CreditsScene::close_scene() { return; }

void CreditsScene::update()
{
    double currentTime = glfwGetTime();
    deltaTime = currentTime - lastFrame;
    lastFrame = currentTime;

    credits_shader.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, credits_texture);
    credits_shader.set_int("texture0", 0);
    shape.render(credits_shader, camera);
}

void CreditsScene::scene_clear()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CreditsScene::process_input()
{
    if (glfwGetKey(ctx.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(ctx.window, true);
}

void CreditsScene::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) 
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    xpos -= ctx.win_width / 2;
    ypos = ctx.win_height - ypos;
    ypos -= ctx.win_height / 2;
}

void CreditsScene::left_click_callback(GLFWwindow* window, int button, int action, int mods) 
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    xpos -= ctx.win_width / 2;
    ypos = ctx.win_height - ypos;
    ypos -= ctx.win_height / 2;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        ctx.load_scene_id(0);
    }
}

void CreditsScene::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {} 
// {
//     camera.process_mouse_scroll(static_cast<float>(yoffset));
// }

void CreditsScene::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    camera.width = width;
    camera.height = height;
    ctx.win_width = width;
    ctx.win_height = height;
}