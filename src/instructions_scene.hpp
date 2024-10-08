#include "mygl/shader.h"
#include "mygl/shape.hpp"
#include "mygl/camera_ortho.hpp"
#include "mygl/iscene.hpp"
#include "mygl/context.hpp"
#include "mygl/button.hpp"
#include "mygl/clock.hpp"
#include "texture.hpp"
#include "stb_image.h"

class InstructionsScene : public IScene {
    public:
        InstructionsScene(Context &ctx);
        void store_scene_in_ctx() override;
        void open_scene() override;
        void close_scene() override;
        void update() override;
        void scene_clear() override;
        void process_input() override;
        void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) override;
        void left_click_callback(GLFWwindow* window, int button, int action, int mods) override;
        void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) override;
        void framebuffer_size_callback(GLFWwindow* window, int width, int height) override;

    public:
        Context &ctx;
    
    private:
        Clock clock;

        CameraOrtho camera;
        Shader credits_shader;
        MyRectangle shape;
        unsigned int instructions_texture;
        float start_time;
};