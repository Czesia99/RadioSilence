#include "mygl/camera_3D.hpp"
#include "mygl/model.hpp"
#include "map.hpp"

class Player
{
    public:
        Camera3D player_camera;
        Clock clock;
        float velocity;

        Shader torchlight_shader;
        Model torchlight;
        Map &my_map;

        Player(Map &map, float win_width = 800, float win_height = 600) : my_map(map)
        {
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

        void update_velocity(bool k_pressed, float delta_time)
        {
            if (!k_pressed) {
                velocity = 0.0f;
            } else {
                velocity = player_camera.movement_speed * delta_time;
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

            // float bobbing = glm::abs(glm::sin(current_time * headbob_frequency)) * headbob_amount_y;
            float bobbing = glm::abs(glm::sin(glm::pi<float>() * (current_time / headbob_frequency))) * headbob_amount_y;
            return bobbing;
        }

        bool collide(Camera3D_Movement direction)
        {
            // Calculate the future position of the player's camera after movement
            glm::vec3 futurePos = player_camera.position;
            if (direction == FORWARD)
                futurePos += player_camera.front * velocity;
            if (direction == BACKWARD)
                futurePos -= player_camera.front * velocity;
            if (direction == LEFT)
                futurePos -= player_camera.right * velocity;
            if (direction == RIGHT)
                futurePos += player_camera.right * velocity;

            // std::cout << futurePos.x << std::endl;
            // Check for collision with each wall
            for (auto& wall_pos : my_map.walls_position)
            {
                float offset = 0.12f;
                float wallMinX = wall_pos.x - 0.5f - offset;
                float wallMaxX = wall_pos.x + 0.5f + offset;
                float wallMinZ = wall_pos.z - 0.5f - offset;
                float wallMaxZ = wall_pos.z + 0.5f + offset;

                // Check if the future position is inside the bounding box of the wall
                if (futurePos.x >= wallMinX && futurePos.x <= wallMaxX && futurePos.z >= wallMinZ && futurePos.z <= wallMaxZ)
                {
                    // Collision detected
                    return true;
                }
            }
            // No collision detected
            return false;
        }

};