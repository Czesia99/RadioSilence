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
        float max_listening_time = 7.0f;

        bool radio_on;
        bool player_dead;

        glm::vec3 &player_pos;
        glm::vec3 &win_pos;

        Radio(Sound &sound_manager, glm::vec3 &player_pos, glm::vec3 &win_pos) : sound_manager(sound_manager), player_pos(player_pos), win_pos(win_pos)
        {
            srand (time(NULL));

            load_sounds();
            player_dead = false;
            radio_on = false;
            activation_number = 0;
            max_activation = random_int(7, 10);
            std::cout << "max activation = " <<  max_activation << std::endl;
        };

        void update()
        {
            if (activation_number > max_activation)
                game_over();

            clock.update();
            if (radio_on)
            {
                listening_time += clock.delta_time;
                if (listening_time > max_listening_time)
                {
                    std::cout << "listening time"<< listening_time << std::endl;
                    game_over();
                }
            }
        }

        void turn_on()
        {
            

            if (glm::distance(player_pos, win_pos) < 10.0f) 
            {
                ma_sound_seek_to_pcm_frame(&radio_sounds[0], 0);
                ma_sound_start(&radio_sounds[0]);
            }
            else if (glm::distance(player_pos, win_pos) <= 30.0f) {
                ma_sound_seek_to_pcm_frame(&radio_sounds[1], 0);
                ma_sound_start(&radio_sounds[1]);
            }
            else {
                ma_sound_seek_to_pcm_frame(&radio_sounds[2], 0);
                ma_sound_start(&radio_sounds[2]);
            }
            activation_number += 1;
            std::cout << activation_number << std::endl;
            radio_on = true;
        }

        void turn_off()
        {
            listening_time = 0.0f;
            radio_on = false;
            ma_sound_stop(&radio_sounds[0]);
            ma_sound_stop(&radio_sounds[1]);
            ma_sound_stop(&radio_sounds[2]);
        }

        void game_over()
        {
            player_dead = true;
            // activation_number = 0;
            turn_off();
        }
        
    private:
        Clock clock;
        Sound &sound_manager;
        
        // ma_sound radio_sound_far;
        // ma_sound radio_sound_between;
        // ma_sound radio_sound_near;

        ma_sound radio_sounds[3];
        const char *sound_files[3] = {
            "../assets/sfx/radio_ultra_near.wav",
            "../assets/sfx/radio_between.wav",
            "../assets/sfx/radio_far.wav"
        };

        void load_sounds()
        {
            for (int i = 0; i < 3; i += 1)
            {
                ma_sound_init_from_file(&sound_manager.engine, sound_files[i], MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC, NULL, &sound_manager.fence, &radio_sounds[i]);
            }
            ma_fence_wait(&sound_manager.fence);
        }

        int random_int(int min, int max)
        {
            assert(max > min); 
            return (rand() % (max - min + 1) + min);
        }
};