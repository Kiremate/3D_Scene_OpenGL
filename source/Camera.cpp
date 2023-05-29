#include "Camera.h"


Matrix44 Camera::get_view_matrix()
{
    Vector3f center = transform.position + view_direction;
    return look_at(transform.position, center);
}

Matrix44 Camera::get_inverse_transform_matrix()
{
    return inverse(transform.get_transform_matrix());
}

glm::mat4 Camera::get_projection_matrix(float aspect_ratio) const
{
    return glm::perspective(fov, aspect_ratio, near_plane, far_plane);
}

void Camera::move_camera(float x, float y, float z) {
    transform.position += Vector3f(x, y, z);
}

void Camera::update_directions()
{
    glm::mat4 rotation_matrix = glm::mat4_cast(transform.rotation);

    glm::vec4 forward = rotation_matrix * glm::vec4(0, 0, 1, 0);
    view_direction = glm::normalize(glm::vec3(forward));

    glm::vec4 up = rotation_matrix * glm::vec4(0, 1, 0, 0);
    up_direction = glm::normalize(glm::vec3(up));

    glm::vec4 right = rotation_matrix * glm::vec4(1, 0, 0, 0);
    right_direction = glm::normalize(glm::vec3(right));
}

void Camera::handle_input(sf::Window& window, float delta_time)
{
    // Update the camera's position based on input
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        transform.translate_local(-view_direction * delta_time * move_speed_);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        transform.translate_local(view_direction * delta_time * move_speed_);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        transform.translate_local(cross(-up_direction, view_direction) * delta_time * move_speed_);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        transform.translate_local(cross(view_direction, -up_direction) * delta_time * move_speed_);
    }
    // Capture mouse when the left button is pressed
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !mouse_captured)
    {
        mouse_captured = true;
        window.setMouseCursorVisible(false);
        sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2), window);
    }

    // Release mouse when the right button is pressed
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && mouse_captured)
    {
        mouse_captured = false;
        window.setMouseCursorVisible(true);
    }

    if (mouse_captured)
    {
        sf::Vector2i current_mouse_pos = sf::Mouse::getPosition(window);
        sf::Vector2i center_pos = sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2);
        sf::Vector2i mouse_delta = current_mouse_pos - center_pos;

        float mouse_sensitivity = 0.0001f;

        float yaw = -mouse_delta.x * mouse_sensitivity;
        float pitch = -mouse_delta.y * mouse_sensitivity;

        // Limit pitch to avoid gimbal lock
        const float pitch_limit = glm::radians(89.0f);
        pitch = glm::clamp(pitch, -pitch_limit, pitch_limit);

        // Update the camera's orientation
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            Quaternion q_yaw = glm::angleAxis(yaw, glm::vec3(0, 1, 0));
            Quaternion q_pitch = glm::angleAxis(pitch, glm::vec3(1, 0, 0));
            transform.rotation = glm::normalize(transform.rotation * q_yaw * q_pitch);
        }

        update_directions();
    }
}
