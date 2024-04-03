#include "game_scene.hpp"

GameScene::GameScene(Context &ctx) : ctx(ctx)
{

    map.print_map_txt();
    map.load_map();

    camera = Camera3D(map.player_position, ctx.win_width, ctx.win_height, true);

    wall_shader = Shader("basic_light.vs", "map_spotlight.fs");

    store_scene_in_ctx();
}

void GameScene::store_scene_in_ctx()
{
    ctx.scenes.push_back(this);
}

void GameScene::open_scene()
{
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(ctx.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void GameScene::close_scene() {}

void GameScene::update()
{
    double currentTime = glfwGetTime();
    deltaTime = currentTime - lastFrame;
    lastFrame = currentTime;

    wall_shader.use();
    
    //spotlight properties
    wall_shader.set_vec3("light.position", camera.position);
    wall_shader.set_vec3("light.direction", camera.front);
    wall_shader.set_float("light.cutOff",   glm::cos(glm::radians(12.5f)));
    wall_shader.set_float("light.outerCutOff", glm::cos(glm::radians(17.5f)));

    wall_shader.set_vec3("viewPos", camera.position);

    // light properties
    wall_shader.set_vec3("light.ambient", 0.05f, 0.05f, 0.05f);
    wall_shader.set_vec3("light.diffuse", 0.8f, 0.8f, 0.8f);
    wall_shader.set_vec3("light.specular", 1.0f, 1.0f, 1.0f);

    wall_shader.set_float("light.constant", 1.0f);
    wall_shader.set_float("light.linear", 0.22f);
    wall_shader.set_float("light.quadratic", 0.20f);

    //material properties
    wall_shader.set_float("material.shininess", 64.0f);

    map.render(wall_shader, camera);
}

void GameScene::scene_clear()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GameScene::process_input()
{
    if (glfwGetKey(ctx.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(ctx.window, true);

    if (glfwGetKey(ctx.window, GLFW_KEY_W) == GLFW_PRESS)
        camera.process_keyboard(FORWARD, deltaTime);
    if (glfwGetKey(ctx.window, GLFW_KEY_S) == GLFW_PRESS)
        camera.process_keyboard(BACKWARD, deltaTime);
    if (glfwGetKey(ctx.window, GLFW_KEY_A) == GLFW_PRESS)
        camera.process_keyboard(LEFT, deltaTime);
    if (glfwGetKey(ctx.window, GLFW_KEY_D) == GLFW_PRESS)
        camera.process_keyboard(RIGHT, deltaTime);
}

void GameScene::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) 
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (first_mouse)
    {
        lastX = xpos;
        lastY = ypos;
        first_mouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.process_mouse_movement(xoffset, yoffset);
}

void GameScene::left_click_callback(GLFWwindow* window, int button, int action, int mods)
{
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
}

void GameScene::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) 
{
    camera.process_mouse_scroll(static_cast<float>(yoffset));
}

void GameScene::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    camera.width = width;
    camera.height = height;
    ctx.win_width = width;
    ctx.win_height = height;
}