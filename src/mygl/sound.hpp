#pragma once

#include <miniaudio.h>
#include <vector>

class Sound {
    public:
        ma_engine engine;
        ma_result result;

        std::vector<ma_sound> sounds;

        Sound() 
        {
            result = ma_engine_init(NULL, &engine);
            if (result != MA_SUCCESS) {
                printf("Failed to initialize audio engine.");
                return;
            }
        }

        void load_sound()
        {
            return;
        }
};