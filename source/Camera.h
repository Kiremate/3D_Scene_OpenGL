#pragma once
#include "math.hpp"
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
using namespace example;

struct Transform {
  example::Vector3f position;
  Quaternion rotation;
  example::Vector3f scale;

  Matrix44 get_transform_matrix() {
      Matrix44 scale_matrix = glm::scale(example::Matrix44(1.0f), scale);
      Matrix44 rotation_matrix = glm::mat4_cast(rotation);
      Matrix44 translation_matrix = glm::translate(example::Matrix44(1.0f), position);
        return translation_matrix * rotation_matrix * scale_matrix;
    }

    void move(const example::Vector3f& offset) {
        position += offset;
    }

    void rotate(const example::Quaternion& q) {
        rotation = glm::normalize(rotation * q);
    }

    void set_rotation(const example::Quaternion& q) {
        rotation = q;
    }

    void scale_up(const example::Vector3f& factor) {
        scale *= factor;
    }

    void scale_down(const example::Vector3f& factor) {
        scale /= factor;
    }

    void translate_local(const example::Vector3f& offset) {
        // Get the rotation matrix from the quaternion
      Matrix44 rotation_matrix = glm::mat4_cast(rotation);

        // Transform the offset vector by the rotation matrix
      Vector4f transformed_offset = glm::vec4(offset, 0.0f) * rotation_matrix;

        // Add the transformed offset to the position
        position += example::Vector3f(transformed_offset);
    }

    void rotate(float pitch, float yaw) {
    // Update the rotation quaternion with the new pitch and yaw angles
      Quaternion q_yaw = glm::angleAxis(glm::radians(yaw), example::Vector3f(0, 1, 0));
      Quaternion q_pitch = glm::angleAxis(glm::radians(pitch), example::Vector3f(1, 0, 0));
      rotation = glm::normalize(rotation * q_yaw * q_pitch);
    }

};

class Camera {
    
public:
  Matrix44 get_view_matrix();
  Matrix44 get_inverse_transform_matrix();
  glm::mat4 get_projection_matrix(float aspect_ratio) const;
    void handle_input(sf::Window& window, float delta_time);
    Camera() = default;
    Camera(float fov, float near_plane, float far_plane, example::Vector3f position, example::Vector3f target, example::Vector3f up)
        : fov(fov),
        near_plane(near_plane),
        far_plane(far_plane),
        move_speed_(5.0f),
        rotation_speed_(0.1f),
        transform{},
        view_direction(normalize(target - position)),
        up_direction(normalize(up)),
        mouse_captured(false)
    {
        transform.position = position;

        example::Vector3f forward = normalize(target - position);
        example::Vector3f right = normalize(cross(up, forward));
        example::Vector3f new_up = cross(forward, right);
        Matrix33 rotation_matrix(right, new_up, forward);
        transform.rotation = Quaternion(rotation_matrix);
    }
    void move_camera(float x, float y, float z);
    void update_directions();

private:
    float fov;
    float near_plane;
    float far_plane;
    float move_speed_ = 5.0f;
    float rotation_speed_ = 0.1f;
    float current_pitch = 0.0f;
    float current_yaw = 0.0f;
    float look_speed = 0.1f; 
    float min_mouse_delta = 0.5f;
    Transform transform;
    example::Vector3f view_direction;
    example::Vector3f up_direction;
	example::Vector3f right_direction;
    bool mouse_captured = false;
    example::Vector2f last_mouse_position;
};