#include "mygl/camera_3D.hpp"

class Player
{
    public:
        Camera3D player_camera;
        Clock clock;
        float velocity;

        Player(glm::vec3 position = {0.0f, 0.0f, 0.0f}, float win_width = 800, float win_height = 600)
        {
            player_camera = Camera3D(position, win_width, win_height, 1.0f, true);
        }

        void update()
        {
            clock.update();
            if (velocity > 0.0f)
            {
                player_camera.position.y = headbob(clock.delta_time, clock.current_time) + player_camera.initial_pos.y;
            }
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
            velocity = player_camera.movement_speed * delta_time;
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
            std::cout << velocity << std::endl;
        }

    private:
        float headbob(float delta_time, float current_time)
        {
            float headbob_frequency = 0.5f;
            float headbob_amount_y = 0.03f;


            // float bobbing = glm::abs(glm::sin(current_time * headbob_frequency)) * headbob_amount_y;
            float bobbing = glm::abs(glm::sin(glm::pi<float>() * (current_time / headbob_frequency))) * headbob_amount_y;
            return bobbing;
        }
};