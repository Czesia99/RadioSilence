#include "game_scene.hpp"

GameScene::GameScene(Context &ctx) : ctx(ctx)
{
    map.print_map_txt();
    map.load_map();

    map_shader = Shader("basic_light.vs", "map_spotlight.fs");
    floor_shader = Shader("floor.vs", "floor_spotlight.fs");
    player = new Player(map, ctx.sound_manager, ctx.win_width, ctx.win_height);
    enemy = new Enemy(map, ctx.sound_manager);

    ma_sound_init_from_file(&ctx.sound_manager.engine, "../assets/sfx/screamer.wav", MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC, NULL, &ctx.sound_manager.fence, &scream_sound);
    ma_sound_init_from_file(&ctx.sound_manager.engine, "../assets/sfx/ambiance.wav", MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC, NULL, &ctx.sound_manager.fence, &ambiance_sound);    
    load_texture("../assets/textures/flashlight2.png", cookie_mask_id);
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

    player->init();
    enemy->init();
    clock.reset();
    call_screamer = false;
    ma_engine_start(&ctx.sound_manager.engine);
    ma_sound_start(&ambiance_sound);
    ma_sound_set_looping(&ambiance_sound, true);
}

void GameScene::close_scene() 
{
    ma_engine_stop(&ctx.sound_manager.engine);
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
    map_shader.set_float("light.cutOff",   glm::cos(glm::radians(10.0f)));
    map_shader.set_float("light.outerCutOff", glm::cos(glm::radians(12.0f)));

    map_shader.set_vec3("viewPos", player->player_camera.position);

    // light properties
    if (player->torchlight_on)
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

    //torchlight mask
    map_shader.set_vec2("viewPort", ctx.win_width, ctx.win_height);
    map_shader.set_int("light.flashlight", 2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, cookie_mask_id);

    floor_shader.use();
    floor_shader.set_vec3("light.position", player->player_camera.position);
    floor_shader.set_vec3("light.direction", player->player_camera.front);
    floor_shader.set_float("light.cutOff",   glm::cos(glm::radians(10.0f)));
    floor_shader.set_float("light.outerCutOff", glm::cos(glm::radians(12.0f)));

    floor_shader.set_vec3("viewPos", player->player_camera.position);

    // light properties
    if (player->torchlight_on)
    {
        floor_shader.set_vec3("light.ambient", 0.05f, 0.05f, 0.05f);
        floor_shader.set_vec3("light.diffuse", 0.6f, 0.6f, 0.6f);
        floor_shader.set_vec3("light.specular", 0.1f, 0.1f, 0.1f);

    }
    else 
    {
        floor_shader.set_vec3("light.ambient", 0.005f, 0.005f, 0.005f);
        floor_shader.set_vec3("light.diffuse", 0.0f, 0.0f, 0.0f);
        floor_shader.set_vec3("light.specular", 0.0f, 0.0f, 0.0f);
    }

    floor_shader.set_float("light.constant", 1.0f);
    floor_shader.set_float("light.linear", 0.22f);
    floor_shader.set_float("light.quadratic", 0.20f);

    //material properties
    floor_shader.set_float("material.shininess", 32.0f);

    //torchlight mask
    floor_shader.set_vec2("viewPort", ctx.win_width, ctx.win_height);
    floor_shader.set_int("light.flashlight", 2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, cookie_mask_id);

    map.render(map_shader, floor_shader, player->player_camera);
    enemy->render(map_shader, player->player_camera);

    if (player->victory)
    {
        ctx.load_scene_id(1);
        player->radio->game_over();
    }

    if (enemy->see_player && player->torchlight_on)
    {
        screamer();
    
        if (clock.current_time - scream_start_time >= 3.0f) 
        {
            ma_sound_stop(&scream_sound);
            ctx.load_scene_id(1);
        }
    }

    if (player->dead)
    {
        screamer();
        // std::cout << "t = " << timer << std::endl;
        // std::cout << "current time = " << clock.current_time << std::endl;

        if (clock.current_time - scream_start_time >= 3.0f) 
        {
            ma_sound_stop(&scream_sound);
            ctx.load_scene_id(1);
        }
    }
}

void GameScene::screamer()
{
    if (call_screamer == false)
    {
        call_screamer = true;
        scream_start_time = clock.current_time;
        enemy->screamer(*player);
        ma_sound_seek_to_pcm_frame(&scream_sound, 0);
        ma_sound_start(&scream_sound);
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

    if (call_screamer)
        return;

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

    if (call_screamer)
        return;

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
    if (call_screamer)
        return;

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        ma_engine_play_sound(&ctx.sound_manager.engine, "../assets/sfx/torchlight_click.wav", NULL);
        if (player->torchlight_on)
            player->torchlight_on = false;
        else
            player->torchlight_on = true;
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