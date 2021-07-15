#include "camera.hh"

#include <iostream>

namespace camera
{
    namespace
    {
        // Mouse position in window
        static double x_mouse = 0;
        static double y_mouse = 0;

        // Mouse settings
        static float speed = 0.1f;
        static float mouse_speed = 0.005f;

        static float time_delta = 1.0f;
        // Initial camera position
        static glm::vec3 position = glm::vec3(0, 0, 5);

        static float horizontal_angle = 3.14f;
        static float vertical_angle = 0.0f;

        // Window size

        static int window_width = 0;
        static int window_height = 0;

        void mouse_callback(int x, int y)
        {
            x_mouse = (window_width / 2.0) - x;
            y_mouse = (window_height / 2.0) - y;

            x_mouse /= window_width / 2.0;
            y_mouse /= window_height / 2.0;
        }

        void keyboard_callback(unsigned char key, int, int)
        {
            if (key == ' ')
            {
                x_mouse = 0;
                y_mouse = 0;
            }

            // Up
            if (key == 'a')
                position -= speed * glm::vec3(0, 1, 0);
            // Down
            if (key == 'z')
                position += speed * glm::vec3(0, 1, 0);
        }

        void special_callback(int key, int, int)
        {
            // Forward
            if (key == GLUT_KEY_UP)
                position -= speed * glm::vec3(0, 0, 1);
            // Backward
            if (key == GLUT_KEY_DOWN)
                position += speed * glm::vec3(0, 0, 1);
            // Right
            if (key == GLUT_KEY_RIGHT)
                position += speed * glm::vec3(1, 0, 0);
            // Left
            if (key == GLUT_KEY_LEFT)
                position -= speed * glm::vec3(1, 0, 0);
        }

    } // namespace

    Camera::Camera()
    {
        window_width = glutGet(GLUT_WINDOW_WIDTH);
        window_height = glutGet(GLUT_WINDOW_HEIGHT);
        glutMotionFunc(mouse_callback);
        glutSpecialFunc(special_callback);
        glutKeyboardFunc(keyboard_callback);
    }

    glm::mat4 Camera::compute_mvp()
    {
        this->update();

        glm::mat4 projection = compute_projection_matrix();
        glm::mat4 view = compute_view_matrix();

        return projection * view;
    }

    glm::mat4 Camera::compute_projection_matrix() const
    {
        return glm::perspective(glm::radians(fov), 16.0f / 9.0f, 0.1f, 100.0f);
    }

    glm::mat4 Camera::compute_view_matrix()
    {
        horizontal_angle += mouse_speed * x_mouse;
        vertical_angle += mouse_speed * y_mouse;

        // Where to
        glm::vec3 direction(std::cos(vertical_angle)
                                * std::sin(horizontal_angle),
                            std::sin(vertical_angle), std::cos(vertical_angle));
        // How much to the right
        glm::vec3 right(std::sin(horizontal_angle - 3.14f / 2.0f), 0,
                        std::cos(horizontal_angle - 3.14f / 2.0f));
        // How much upwards
        glm::vec3 up = glm::vec3(0, 1, 0);

        return glm::lookAt(position, position - direction, up);
    }

    void Camera::update()
    {
        // update_time_delta();
        update_mouse_pos();
        update_position_keyboard();
    }

    void Camera::update_time_delta()
    {
        double curr_time = glutGet(GLUT_ELAPSED_TIME);
        time_delta = float(curr_time - prev_time);
        prev_time = curr_time;
    }

    void Camera::update_mouse_pos()
    {
        // glutPassiveMotionFunc(mouse_callback);
        glutMotionFunc(mouse_callback);
    }

    void Camera::update_position_keyboard()
    {
        glutSpecialFunc(special_callback);
        glutKeyboardFunc(keyboard_callback);
    }
} // namespace camera
