#include "camera.hh"

namespace camera
{
    namespace
    {
        void mouse_callback(int x, int y)
        {
            x_mouse = x;
            y_mouse = y;
        }

        void keyboard_callback(unsigned char key, int, int)
        {

            horizontal_angle += mouse_speed * time_delta * float(window_width / 2 - x_mouse);
            vertical_angle += mouse_speed * time_delta * float(window_height / 2 - y_mouse);

            // Where to
            glm::vec3 direction(std::cos(vertical_angle) * std::sin(horizontal_angle),
                                std::sin(vertical_angle), std::cos(vertical_angle));
            // How much to the right
            glm::vec3 right(std::sin(horizontal_angle - 3.14f / 2.0f), 0,
                            std::cos(horizontal_angle - 3.14f / 2.0f));
            // Forward
            if (key == GLUT_KEY_LEFT)
                position += direction * time_delta * speed;
            // Backward
            if (key == GLUT_KEY_DOWN)
                position -= direction * time_delta * speed;
            // Right
            if (key == GLUT_KEY_RIGHT)
                position += right * time_delta * speed;
            // Left
            if (key == GLUT_KEY_LEFT)
                position -= right * time_delta * speed;
        }
    }

    Camera::Camera()
    {
        window_width = glutGet(GLUT_WINDOW_WIDTH);
        window_height = glutGet(GLUT_WINDOW_HEIGHT);
        glutMotionFunc(mouse_callback);
        x_mouse = window_width / 2;
        y_mouse = window_height / 2;
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
        return glm::perspective(glm::radians(fov), 4.0f / 3.0f, 0.1f, 100.0f);
    }

    glm::mat4 Camera::compute_view_matrix()
    {
        horizontal_angle += mouse_speed * time_delta * float(window_width / 2 - x_mouse);
        vertical_angle += mouse_speed * time_delta * float(window_height / 2 - y_mouse);

        // Where to
        glm::vec3 direction(std::cos(vertical_angle) * std::sin(horizontal_angle),
                            std::sin(vertical_angle), std::cos(vertical_angle));
        // How much to the right
        glm::vec3 right(std::sin(horizontal_angle - 3.14f / 2.0f), 0,
                        std::cos(horizontal_angle - 3.14f / 2.0f));
        // How much upwards
        glm::vec3 up = glm::cross(right, direction);

        return glm::lookAt(position, position + direction, up);
    }

    void Camera::update()
    {
        update_time_delta();
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
        glutPassiveMotionFunc(mouse_callback);
        glutMotionFunc(mouse_callback);
    }

    void Camera::update_position_keyboard()
    {
        glutKeyboardFunc(keyboard_callback);
    }
}
