#include "player.hpp"

#define MINIAUDIO_IMPLEMENTATION
#include "../include/miniaudio.h"

Player::Player(Map &map, float win_width = 800, float win_height = 600) : my_map(map)
{
    ma_result result;
    ma_engine engine;

    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize audio engine.");
        return;
    }
    ma_engine_play_sound(&engine, "../assets/sfx/horror.wav", NULL);

    // SoundEngine = createIrrKlangDevice();
    // SoundEngine->play2D("../assets/sfx/horror.wav", true);
    // if (snd)
    //     snd->setVolume(0.5f);
    player_camera = Camera3D(map.player_position, win_width, win_height, 1.0f, true);

    // torchlight_shader = Shader("basic_light.vs", "map_spotlight.fs");
    // torchlight = Model("../assets/models/torchlight/torchlight.obj");
    // torchlight.transform.scale *= 3;
    // torchlight.transform.position = player_camera.position;
    // torchlight.transform.position.z -= 0.3f;
    // torchlight.transform.rotation -= 90.0f;
}
