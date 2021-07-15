#pragma once

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/freeglut.h>

namespace camera
{
    // Mouse position in window
    static double x_mouse = 0;
    static double y_mouse = 0;

    // Mouse settings
    static float speed = 3.0f;
    static float mouse_speed = 0.005f;

    static float time_delta = 1.0f;
    // Initial camera position
    static glm::vec3 position = glm::vec3(0, 0, 5);

    static float horizontal_angle = 3.14f;
    static float vertical_angle = 0.0f;

    // Window size
    static int window_width = 0;
    static int window_height = 0;

    class Camera
    {
    public:

        Camera();

        glm::mat4 compute_mvp();

    private:

        glm::mat4 compute_projection_matrix() const;
        glm::mat4 compute_view_matrix();

        void update();

        void update_time_delta();
        void update_mouse_pos();
        void update_position_keyboard();

        void mouse_set(int x, int y);

        float fov = 45.0f;

        double prev_time = glutGet(GLUT_ELAPSED_TIME);
    };
}
