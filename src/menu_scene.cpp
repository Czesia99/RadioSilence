#include "menu_scene.hpp"

MenuScene::MenuScene(Context &ctx) : ctx(ctx)
{
    camera = CameraOrtho(glm::vec3(0.0f, 0.0f, 0.0f), ctx.win_width, ctx.win_height);

    play_btn = Button(-400.0f, 100.0f, 200.0f, 80.0f);
    quit_btn = Button(-400.0f,  0.0f, 200.0f, 80.0f);
    credit_btn = Button(-400.0f,  -100.0f, 200.0f, 80.0f);
    stbi_set_flip_vertically_on_load(true);
    load_texture("../assets/textures/menu_bg.png", bg.texture);
    load_texture("../assets/textures/play_button.png", play_btn.texture);
    load_texture("../assets/textures/quit_button.png", quit_btn.texture);
    load_texture("../assets/textures/credits_button.png", credit_btn.texture);

    float bg_ar = 1665.0f / 1369.0f;
    float scale_factor;

    if (ctx.aspect_ratio > bg_ar) {
        scale_factor = ctx.win_height / 1369.0f;
    }
    else {
        scale_factor = ctx.win_width / 1665.0f;
    }

    bg.transform.scale.x = 1665.0f * scale_factor;
    bg.transform.scale.y = 1369.0f * scale_factor;

    btn_shader = Shader("button.vs", "button.fs");
    bg_shader = Shader("bg_texture.vs", "bg_texture.fs");
    
    store_scene_in_ctx();
}

void MenuScene::store_scene_in_ctx()
{
    ctx.scenes.push_back(this);
}

void MenuScene::open_scene() 
{
    glDisable(GL_DEPTH_TEST);
    glfwSetInputMode(ctx.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void MenuScene::close_scene() { return; }

void MenuScene::update()
{
    clock.update();
    glActiveTexture(GL_TEXTURE0);
    
    glBindTexture(GL_TEXTURE_2D, bg.texture);
    bg.render(bg_shader, camera);

    btn_shader.use();
    btn_shader.set_int("texture0", 0);
    
    glBindTexture(GL_TEXTURE_2D, play_btn.texture);
    btn_shader.set_bool("hovered", play_btn.hover);
    play_btn.render(btn_shader, camera);

    glBindTexture(GL_TEXTURE_2D, quit_btn.texture);
    btn_shader.set_bool("hovered", quit_btn.hover);
    quit_btn.render(btn_shader, camera);

    glBindTexture(GL_TEXTURE_2D, credit_btn.texture);
    btn_shader.set_bool("hovered", credit_btn.hover);
    credit_btn.render(btn_shader, camera);
}

void MenuScene::scene_clear()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MenuScene::process_input() {}

void MenuScene::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) 
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    xpos -= ctx.win_width / 2;
    ypos = ctx.win_height - ypos;
    ypos -= ctx.win_height / 2;

    play_btn.is_hovered(xpos, ypos);
    quit_btn.is_hovered(xpos, ypos);
    credit_btn.is_hovered(xpos, ypos);
}

void MenuScene::left_click_callback(GLFWwindow* window, int button, int action, int mods) 
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    xpos -= ctx.win_width / 2;
    ypos = ctx.win_height - ypos;
    ypos -= ctx.win_height / 2;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if(play_btn.hover) {
            ctx.load_scene_id(2);
        }
        if(quit_btn.hover) {
            glfwSetWindowShouldClose(ctx.window, true);
        }
        if(credit_btn.hover) {
            ctx.load_scene_id(3);
        }
    }
}

void MenuScene::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {}

void MenuScene::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    camera.width = width;
    camera.height = height;
    ctx.win_width = width;
    ctx.win_height = height;
}