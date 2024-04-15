#include "mygl/camera_3D.hpp"
#include "mygl/model.hpp"
#include "map.hpp"
#include "radio.hpp"

class Player
{
    public:
        Camera3D player_camera;
        Clock clock;
        float velocity;

        Shader torchlight_shader;
        Model torchlight;
        Map &my_map;

        bool step = false;
        bool victory = false;
        bool dead = false;

        Radio *radio;

        Sound &sound_manager;
        ma_sound step_sound;

        Player(Map &map, Sound &sound_manager, float win_width = 800, float win_height = 600) : my_map(map), sound_manager(sound_manager)
        {
            player_camera = Camera3D(map.player_position, win_width, win_height, 1.0f, true);
            radio = new Radio(sound_manager);

            sound_manager.result = ma_sound_init_from_file(&sound_manager.engine, "../assets/sfx/footstep.wav", 0, NULL, NULL, &step_sound);
            ma_sound_set_volume(&step_sound, 0.2f);
            // ma_sound_set_volume(&step_sound, 1.0f);
        }

        void update()
        {
            clock.update();
            radio->update();
            if (velocity > 0.0f)
            {
                player_camera.position.y = headbob(clock.delta_time, clock.current_time) + player_camera.initial_pos.y;
            }

            is_victory();
            is_dead();
        }

        void update_velocity(bool k_pressed)
        {
            if (!k_pressed) {
                velocity = 0.0f;
            } else {
                velocity = player_camera.movement_speed * clock.delta_time;
            }
        }

        void process_keyboard(Camera3D_Movement direction, float delta_time, bool k_pressed)
        {
            if (collide(direction))
            {
                velocity = 0 * delta_time;
            } else {
               velocity = player_camera.movement_speed * delta_time; 
            }

            if (direction == FORWARD) {
                player_camera.position += player_camera.front * velocity;
                my_map.player_position = player_camera.position;
            }
            if (direction == BACKWARD) {
                player_camera.position -= player_camera.front * velocity;
                my_map.player_position = player_camera.position;
            }
            if (direction == LEFT) {
                player_camera.position -= player_camera.right * velocity;
                my_map.player_position = player_camera.position;
            }
            if (direction == RIGHT) {
                player_camera.position += player_camera.right * velocity;
                my_map.player_position = player_camera.position;
            }
            if (player_camera.fps)
                player_camera.position.y = player_camera.initial_pos.y;
        }

    private:

        float headbob(float delta_time, float current_time)
        {
            float headbob_frequency = 0.7f;
            float headbob_amount_y = 0.03f;
            
            // float bobbing = glm::abs(glm::sin(current_time * headbob_frequency)) * headbob_amount_y;
            float bobbing = glm::abs(glm::sin(glm::pi<float>() * (current_time / headbob_frequency))) * headbob_amount_y;

            if (bobbing >= 0.005 && step)
            {
                step = false;
            }
            if (bobbing <= 0.005 && !step) {
                step = true;
                ma_sound_start(&step_sound);
            }

            return bobbing;
        }

        void is_victory()
        {
            float offset = 0.11f;
            float min_x = my_map.win_position.x - 0.4f - offset;
            float max_x = my_map.win_position.x + 0.4f + offset;
            float min_z = my_map.win_position.z - 0.4f - offset;
            float max_z = my_map.win_position.z + 0.4f + offset;
    
            if (player_camera.position.x >= min_x && player_camera.position.x <= max_x && player_camera.position.z >= min_z && player_camera.position.z <= max_z)
            {
                victory = true;
            }
        }

        void is_dead()
        {
            dead = &radio->player_dead;
        }

        bool collide(Camera3D_Movement direction)
        {
            glm::vec3 futurePos = player_camera.position;
            if (direction == FORWARD)
                futurePos += player_camera.front * velocity;
            if (direction == BACKWARD)
                futurePos -= player_camera.front * velocity;
            if (direction == LEFT)
                futurePos -= player_camera.right * velocity;
            if (direction == RIGHT)
                futurePos += player_camera.right * velocity;

            for (auto& wall_pos : my_map.walls_position)
            {
                float offset = 0.11f;
                float min_x = wall_pos.x - 0.5f - offset;
                float max_x = wall_pos.x + 0.5f + offset;
                float min_z = wall_pos.z - 0.5f - offset;
                float max_z = wall_pos.z + 0.5f + offset;

                if (futurePos.x >= min_x && futurePos.x <= max_x && futurePos.z >= min_z && futurePos.z <= max_z)
                {
                    return true;
                }
            }
            return false;
        }
};