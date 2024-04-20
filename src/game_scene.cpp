#include "game_scene.hpp"

GameScene::GameScene(Context &ctx) : ctx(ctx)
{
    map.print_map_txt();
    map.load_map();

    player = new Player(map, ctx.sound_manager, ctx.win_width, ctx.win_height);

    map_shader = Shader("basic_light.vs", "map_spotlight.fs");
    enemy = new Enemy(map);

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

    player->victory = false;
    player->radio->player_dead = false;
    player->player_camera.position = map.player_start_position;
    enemy->model.transform.position = map.enemy_start_position;
    ma_engine_play_sound(&ctx.sound_manager.engine, "../assets/sfx/ambiance.wav", NULL);
}

void GameScene::close_scene() 
{
    // sound->drop();
    // player->~Player();
    return;
}

void GameScene::update()
{
    clock.update();
    player->update();
    enemy->update();
    map_shader.use();
    
    //spotlight properties
    map_shader.set_vec3("light.position", player->player_camera.position);
    map_shader.set_vec3("light.direction", player->player_camera.front);
    map_shader.set_float("light.cutOff",   glm::cos(glm::radians(12.5f)));
    map_shader.set_float("light.outerCutOff", glm::cos(glm::radians(16.5f)));

    map_shader.set_vec3("viewPos", player->player_camera.position);

    // light properties
    if (torchlight)
    {
        map_shader.set_vec3("light.ambient", 0.05f, 0.05f, 0.05f);
        map_shader.set_vec3("light.diffuse", 0.6f, 0.6f, 0.6f);
        map_shader.set_vec3("light.specular", 0.1f, 0.1f, 0.1f);

    }
    else 
    {
        map_shader.set_vec3("light.ambient", 0.005f, 0.005f, 0.005f);
        map_shader.set_vec3("light.diffuse", 0.0f, 0.0f, 0.0f);
        map_shader.set_vec3("light.specular", 0.0f, 0.0f, 0.0f);
    }

    map_shader.set_float("light.constant", 1.0f);
    map_shader.set_float("light.linear", 0.22f);
    map_shader.set_float("light.quadratic", 0.20f);

    //material properties
    map_shader.set_float("material.shininess", 32.0f);

    map.render(map_shader, player->player_camera);
    enemy->render(map_shader, player->player_camera);

    if (player->victory)
    {
        ctx.load_scene_id(0);
        player->radio->game_over();
    }

    if (player->dead)
    {
        std::cout << "player dead " << player->dead <<  std::endl;
        ctx.load_scene_id(0);
    }
}

void GameScene::scene_clear()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GameScene::process_input()
{
    bool k_pressed = false;

    if (glfwGetKey(ctx.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(ctx.window, true);

    if (glfwGetKey(ctx.window, GLFW_KEY_W) == GLFW_PRESS) {
        k_pressed = true;
        player->process_keyboard(FORWARD, clock.delta_time, k_pressed);
    }
    if (glfwGetKey(ctx.window, GLFW_KEY_S) == GLFW_PRESS) {
        k_pressed = true;
        player->process_keyboard(BACKWARD, clock.delta_time, k_pressed);
    }
    if (glfwGetKey(ctx.window, GLFW_KEY_A) == GLFW_PRESS) {
        k_pressed = true;
        player->process_keyboard(LEFT, clock.delta_time, k_pressed);
    }
    if (glfwGetKey(ctx.window, GLFW_KEY_D) == GLFW_PRESS) {
        k_pressed = true;
        player->process_keyboard(RIGHT, clock.delta_time, k_pressed);
    }

    player->update_velocity(k_pressed);
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

    player->player_camera.process_mouse_movement(xoffset, yoffset);
}

void GameScene::left_click_callback(GLFWwindow* window, int button, int action, int mods)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        ma_engine_play_sound(&ctx.sound_manager.engine, "../assets/sfx/torchlight_click.wav", NULL);
        if (torchlight)
            torchlight = false;
        else
            torchlight = true;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        if (player->radio->radio_on)
            player->radio->turn_off();
        else
            player->radio->turn_on();
    }
}

void GameScene::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) 
{
    player->player_camera.process_mouse_scroll(static_cast<float>(yoffset));
}

void GameScene::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    player->player_camera.width = width;
    player->player_camera.height = height;
    ctx.win_width = width;
    ctx.win_height = height;
}