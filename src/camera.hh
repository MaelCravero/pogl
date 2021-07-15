#pragma once

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "gl/gl.hh"

namespace camera
{
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
} // namespace camera
