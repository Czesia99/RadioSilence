#pragma once

#include <miniaudio.h>
#include <vector>

class Sound {
    public:
        ma_engine engine;
        ma_result result;
        ma_fence fence;

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
        }

        void load_sound()
        {
            return;
        }
};