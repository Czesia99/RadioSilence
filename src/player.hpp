#include "mygl/camera_3D.hpp"
#include "mygl/model.hpp"
#include "map.hpp"

#include <irrKlang.h>
using namespace irrklang;

class Player
{
    public:
        Camera3D player_camera;
        Clock clock;
        float velocity;

        Shader torchlight_shader;
        Model torchlight;
        Map &my_map;
        // ISoundEngine *SoundEngine;
        bool step = false;
        bool victory = false;

        Player(Map &map, float win_width = 800, float win_height = 600) : my_map(map)
        {
            // SoundEngine = createIrrKlangDevice();
            // SoundEngine->play2D("../assets/sfx/horror.wav", true);
            player_camera = Camera3D(map.player_position, win_width, win_height, 1.0f, true);
            // torchlight_shader = Shader("basic_light.vs", "map_spotlight.fs");
            // torchlight = Model("../assets/models/torchlight/torchlight.obj");
            // torchlight.transform.scale *= 3;
            // torchlight.transform.position = player_camera.position;
            // torchlight.transform.position.z -= 0.3f;
            // torchlight.transform.rotation -= 90.0f;
        }

        void update()
        {
            clock.update();
            if (velocity > 0.0f)
            {
                player_camera.position.y = headbob(clock.delta_time, clock.current_time) + player_camera.initial_pos.y;
            }
            is_victory();
            // torchlight_shader.use();
            // torchlight_shader.set_vec3("light.position", player_camera.position);
            // torchlight_shader.set_vec3("light.direction", player_camera.front);
            // torchlight_shader.set_float("light.cutOff",   glm::cos(glm::radians(12.5f)));
            // torchlight_shader.set_float("light.outerCutOff", glm::cos(glm::radians(17.5f)));

            // torchlight_shader.set_vec3("viewPos", player_camera.position);

            // // light properties
            // torchlight_shader.set_vec3("light.ambient", 0.05f, 0.05f, 0.05f);
            // torchlight_shader.set_vec3("light.diffuse", 1.0f, 1.0f, 1.0f);
            // torchlight_shader.set_vec3("light.specular", 0.1f, 0.1f, 0.1f);

            // torchlight_shader.set_float("light.constant", 1.0f);
            // torchlight_shader.set_float("light.linear", 0.22f);
            // torchlight_shader.set_float("light.quadratic", 0.20f);

            // //material properties
            // torchlight_shader.set_float("material.shininess", 32.0f);
            // torchlight.transform.position = player_camera.position;
            // torchlight.transform.position.z -= 0.3f;
            // torchlight.draw(torchlight_shader, player_camera);
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
            
            // velocity = player_camera.movement_speed * delta_time;
            if (collide(direction))
            {
                velocity = 0 * delta_time;
            } else {
               velocity = player_camera.movement_speed * delta_time; 
            }
            // player_camera.velocity = player_camera.movement_speed * delta_time;
            if (direction == FORWARD)
                player_camera.position += player_camera.front * velocity;
            if (direction == BACKWARD)
                player_camera.position -= player_camera.front * velocity;
            if (direction == LEFT)
                player_camera.position -= player_camera.right * velocity;
            if (direction == RIGHT)
                player_camera.position += player_camera.right * velocity;
            if (player_camera.fps)
                player_camera.position.y = player_camera.initial_pos.y;
        }

    private:

        float headbob(float delta_time, float current_time)
        {
            float headbob_frequency = 0.7f;
            float headbob_amount_y = 0.03f;
            
            vec3d soundPos = {player_camera.position.x, player_camera.position.y, player_camera.position.z};
            // float bobbing = glm::abs(glm::sin(current_time * headbob_frequency)) * headbob_amount_y;
            float bobbing = glm::abs(glm::sin(glm::pi<float>() * (current_time / headbob_frequency))) * headbob_amount_y;

            std::cout << bobbing << std::endl;

            if (bobbing >= 0.005 && step)
            {
                step = false;
            }
            if (bobbing <= 0.005 && !step) {
                step = true;
                // SoundEngine->play3D("../assets/sfx/footstep.wav", soundPos, false);
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