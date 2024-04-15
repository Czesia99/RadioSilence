#pragma once

#include <glm/gtc/random.hpp>

#include "mygl/clock.hpp"
#include "mygl/sound.hpp"
#include "math.h"

class Radio 
{
    public:
        int activation_number;
        int max_activation;
    
        float listening_time;
        float max_listening_time;

        bool radio_on = false;

        bool player_dead = false;
        Sound &sound_manager;
        // ma_sound radio_sound;

        Radio(Sound &sound_manager) : sound_manager(sound_manager)
        {
            srand (time(NULL));
            activation_number = 0;
            max_activation = random_int(7, 10);
            std::cout << "max activation = " <<  max_activation << std::endl;
            max_listening_time = 10.0f;
        };

        void update()
        {
            if (activation_number > max_activation)
                player_dead = true;

            if (radio_on)
            {
                
            }
        }

        void turn_on() 
        {
            ma_engine_play_sound(&sound_manager.engine, "../assets/sfx/radio1.wav", NULL);
            activation_number += 1;
            radio_on = true;
        }

        void turn_off()
        {
            listening_time = 0.0f;
            radio_on = false;
        }

        float random_float(float min, float max)
        {
            assert(max > min); 
            float random = ((float) rand()) / (float) RAND_MAX;
            float range = max - min;

            return (random*range) + min;
        }

        int random_int(int min, int max)
        {
            assert(max > min); 
            int random = (rand()) / RAND_MAX;
            int range = max - min;

            return (random*range) + min;
        }
    private:
        Clock clock;
};