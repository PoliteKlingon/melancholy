// ################################################################################
// Common Framework for Computer Graphics Courses at FI MUNI.
// 
// Copyright (c) 2021-2022 Visitlab (https://visitlab.fi.muni.cz)
// All rights reserved.
// ################################################################################

#include "camera.h"
#include "glm/ext/matrix_transform.inl"
#include <GLFW/glfw3.h>
#include <math.h>

// ----------------------------------------------------------------------------
// Variables
// ----------------------------------------------------------------------------
const float Camera::min_elevation = -1.5f;
const float Camera::max_elevation = 1.5f;
const float Camera::min_distance = 0.1f;
const float Camera::angle_sensitivity = 0.008f;
const float Camera::zoom_sensitivity = 0.003f;

// ----------------------------------------------------------------------------
// Constructors
// ----------------------------------------------------------------------------
Camera::Camera(bool switch_axes)
    : angle_direction(4.56f), angle_elevation(-0.06f), distance(0.1f), last_x(0), last_y(0), is_rotating(false), is_zooming(false),
      switch_axes(switch_axes) {
    update_eye_pos();
}

// ----------------------------------------------------------------------------
// Methods
// ----------------------------------------------------------------------------
void Camera::update_eye_pos() {
    float update_x = 0.0f;
    float update_z = 0.0f;
    if (go_forward) {
        update_z = speed;
    } else if (go_backward) {
        update_z = -speed;
    }
    if (go_left) {
        update_x = speed;
    } else if (go_right) {
        update_x = -speed;
    }

    update_camera_center(update_x, 0.0f, update_z);
}

void Camera::update_eye_position_data() {
    if (switch_axes) {
        eye_position.x = camera_center.x + distance * cosf(angle_elevation) * sinf(angle_direction);
        eye_position.z = glm::max(0.2f, camera_center.y + distance * sinf(angle_elevation));
        eye_position.y = camera_center.z + distance * cosf(angle_elevation) * cosf(angle_direction);
    } else {
        eye_position.x = camera_center.x + distance * cosf(angle_elevation) * -sinf(angle_direction);
        eye_position.y = glm::max(0.2f, camera_center.y + distance * sinf(angle_elevation));
        eye_position.z = camera_center.z + distance * cosf(angle_elevation) * cosf(angle_direction);
    }
}



// ----------------------------------------------------------------------------
// Input Events
// ----------------------------------------------------------------------------
void Camera::on_mouse_move(double x, double y) {
    float dx = static_cast<float>(x - last_x);
    float dy = static_cast<float>(y - last_y);
    last_x = static_cast<int>(x);
    last_y = static_cast<int>(y);

    if (!is_zooming) {
        angle_direction += dx * angle_sensitivity;
        angle_elevation += dy * angle_sensitivity;

        // Clamps the results.
        angle_elevation = glm::clamp(angle_elevation, min_elevation, max_elevation);
    }
    if (is_zooming) {
        distance *= (1.0f + dy * zoom_sensitivity);

        // Clamps the results.
        if (distance < min_distance)
            distance = min_distance;
    }
    update_eye_position_data();
}

void Camera::on_mouse_button(int button, int action, int mods) {
    // Left mouse button affects the angles.
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            is_rotating = true;
        } else {
            is_rotating = false;
        }
    }
    // Right mouse button affects the zoom.5
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            is_zooming = true;
        } else {
            is_zooming = false;
        }
    }
}

// ----------------------------------------------------------------------------
// Getters & Setters
// ----------------------------------------------------------------------------
void Camera::set_eye_position(float angle_direction, float angle_elevation, float distance) {
    this->angle_direction = angle_direction;
    this->angle_elevation = angle_elevation;
    this->distance = distance;
    update_eye_pos();
}

glm::vec3 Camera::get_eye_position() { 
    if (go_backward || go_forward || go_left || go_right) {
        update_eye_pos();
    }
    return eye_position; 
}

glm::vec3 Camera::get_center() {
    return camera_center;
}
void Camera::set_center(glm::vec3 vec){
    this->camera_center = vec;
    update_eye_position_data();
}

void Camera::update_camera_center(float x, float y, float z) {
    glm::vec4 point = glm::vec4(x, y, z, 0.0f);
    glm::vec4 rotated_point = glm::rotate(glm::mat4(1.0f), float(M_PI) - this->angle_direction, glm::vec3(0.0f, 1.0f, 0.0f)) * point;
    this->camera_center += glm::vec3(rotated_point);
    update_eye_position_data();
}

void Camera::move(action action) {
    switch (action) {
        case FORWARD:
            if (!go_backward) go_forward = true;
            break;
        case BACKWARD:
            if (!go_forward) go_backward = true;
            break;
        case LEFT:
            if (!go_right) go_left = true;
            break;
        case RIGHT:
            if (!go_left) go_right = true;
            break;
        case STOP_FORWARD:
            go_forward = false;
            break;
        case STOP_BACKWARD:
            go_backward = false;
            break;
        case STOP_LEFT:
            go_left = false;
            break;
        case STOP_RIGHT:
            go_right = false;
            break;
        case SPRINT:
            speed *= 2;
            break;
        case STOP_SPRINT:
            speed /= 2;
            break;
        case CRUNCH:
            update_camera_center(0.0f, -1.5f, 0.0f);
            speed /= 2;
            break;
        case STOP_CRUNCH:
            update_camera_center(0.0f, 1.5f, 0.0f);
            speed *= 2;
            break;
        default:
            break;
    }
}

glm::mat4x4 Camera::get_view_matrix() {
    //return lookAt(eye_position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    if (go_backward || go_forward || go_left || go_right) {
        update_eye_pos();
    }
    return lookAt(eye_position, camera_center, glm::vec3(0.0f, 1.0f, 0.0f));
}
