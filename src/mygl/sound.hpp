#pragma once

#include <miniaudio.h>
#include <vector>

class Sound {
    public:
        ma_engine engine;
        ma_result result;
        ma_fence fence;

        ma_spatializer_listener listener;

        std::vector<ma_sound> sounds;

        Sound()
        {
            result = ma_engine_init(NULL, &engine);
            if (result != MA_SUCCESS) {
                printf("Failed to initialize audio engine.");
                return;
            }

            result = ma_fence_init(&fence);
            if (result != MA_SUCCESS) {
                std::cout << "can't intialize fence for radio sounds";
            }
            // ma_spatializer_listener_set_position(&listener, );
        }

        // void set_listener_position(float x, float y, float z)
        // {
        //     ma_spatializer_set_position(&listener, x, y, z);
        // }

        // void load_sounds(const char *files, ma_sound *sounds)
        // {
        //     for (int i = 0; i < strlen(files); i++)
        //     {
        //         ma_sound_init_from_file(&engine, &files[i], MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC, NULL, &fence, &sounds[i]);
        //     }
        //     return;
        // }
};