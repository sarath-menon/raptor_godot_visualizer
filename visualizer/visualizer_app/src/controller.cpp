#include "controller.h"

using namespace godot;

Controller::Controller(){};
Controller::~Controller(){};

void Controller::_init() {}

void godot::Controller::_register_methods() {
  register_method((char *)"_process", &Controller::_process);
  register_method((char *)"UpdateMotionFromInput",
                  &Controller::UpdateMotionFromInput);
}

void godot::Controller::_process() {
  UpdateMotionFromInput();
  move_and_slide(position);
  set_rotation_degrees(orientation);
}

void godot::Controller::UpdateMotionFromInput() {
  position = Vector3(0, 0, 0);
  Input *input = Input::get_singleton();

  if (input->is_action_pressed("ui_up") &&
      input->is_action_pressed("ui_down")) {
  }

  else if (input->is_action_pressed("ui_up")) {
    position.z = speed;
    std::cout << "ok";
  }

  else if (input->is_action_pressed("ui_down")) {
    position.z = -speed;
    std::cout << "ok";
  }

  else {
    position.z *= friction_coeff;
  }

  if (input->is_action_pressed("ui_left") &&
      input->is_action_pressed("ui_right")) {
  }

  else if (input->is_action_pressed("ui_left")) {
    position.x = -speed;
  }

  else if (input->is_action_pressed("ui_right")) {
    position.x = speed;
  }

  else {
    position.x *= friction_coeff;
  }

  if (input->is_action_pressed("up") && input->is_action_pressed("down")) {
  }

  else if (input->is_action_pressed("up")) {
    position.y = speed;
  }

  else if (input->is_action_pressed("down")) {
    position.y = -speed;
  }

  else {
    position.y *= friction_coeff;
  }

  if (input->is_action_pressed("roll_left")) {
    orientation.z += 1;
  }

  else if (input->is_action_pressed("roll_right")) {
    orientation.z -= 1;
  }

  if (input->is_action_pressed("pitch_front")) {
    orientation.x += 1;
  }

  else if (input->is_action_pressed("pitch_back")) {
    orientation.x -= 1;
  }
  if (input->is_action_pressed("yaw")) {
    orientation.y += 1;
  }
}