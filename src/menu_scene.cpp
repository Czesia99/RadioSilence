#include "menu_scene.hpp"

MenuScene::MenuScene(Context &ctx) : ctx(ctx)
{
    camera = CameraOrtho(glm::vec3(0.0f, 0.0f, 0.0f), ctx.win_width, ctx.win_height);

    // btn = Button(ctx.win_width / 2, ctx.win_height / 2, 100.0f, 50.0f);
    btn = Button(0.0f, 200.0f, 100.0f, 50.0f);
    stbi_set_flip_vertically_on_load(true);
    load_texture("../assets/textures/play_button.png", btn.texture);
    btn_shader = Shader("shader.vs", "shader.fs");
    

    store_scene_in_ctx();
}

void MenuScene::store_scene_in_ctx()
{
    ctx.scenes.push_back(this);
}

void MenuScene::open_scene() 
{
    glfwSetInputMode(ctx.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void MenuScene::close_scene() { return; }

void MenuScene::update()
{
    double currentTime = glfwGetTime();
    deltaTime = currentTime - lastFrame;
    lastFrame = currentTime;

    btn_shader.use();
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, btn.texture);
    btn_shader.set_int("texture1", 0);
    // btn_shader.set_float("time", currentTime);

    btn.render(btn_shader, camera); 
}

void MenuScene::scene_clear()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MenuScene::process_input()
{
    if (glfwGetKey(ctx.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(ctx.window, true);
}

void MenuScene::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) 
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    xpos -= ctx.win_width / 2;
    ypos = ctx.win_height - ypos;
    ypos -= ctx.win_height / 2;

    // if (btn.is_clicked(xpos, ypos))
    // {
        
    // }
}

void MenuScene::left_click_callback(GLFWwindow* window, int button, int action, int mods) 
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    xpos -= ctx.win_width / 2;
    ypos = ctx.win_height - ypos;
    ypos -= ctx.win_height / 2;
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if(btn.is_clicked(xpos, ypos)) {
            std::cout<< "button clicked !" << std::endl;
            ctx.load_scene_id(1);
        }
    }
}

void MenuScene::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {} 
// {
//     camera.process_mouse_scroll(static_cast<float>(yoffset));
// }

void MenuScene::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    camera.width = width;
    camera.height = height;
    ctx.win_width = width;
    ctx.win_height = height;
}