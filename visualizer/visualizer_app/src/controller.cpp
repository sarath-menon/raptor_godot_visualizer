#include "controller.h"
using namespace godot;

///////////////////////////////////////////////////////////////////////////

// Fastdds headers
#include "mocap_quadcopterPubSubTypes.h"
#include "mocap_quadcopterSubscriber.h"
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <unistd.h>
/////////////////////////////////////////////////////////////////////////

namespace subscriber {

void mocap_quadcopterSubscriber::SubListener::on_data_available(
    eprosima::fastdds::dds::DataReader *reader) {
  // Take data
  mocap_quadcopter st;
  eprosima::fastdds::dds::SampleInfo info;

  if (reader->take_next_sample(&st, &info) == ReturnCode_t::RETCODE_OK) {
    if (info.valid_data) {
      // Print your structure data here.
      ++samples;
      std::cout << "\nSample received, count=" << samples << std::endl;
      new_data = true;

      std::cout << "Index=" << st.index() << std::endl;

      std::cout << "Object Name:" << st.object_name() << std::endl;

      object_name = st.object_name();
      frame_number = st.index();

      position[0] = st.position().at(0);
      position[1] = st.position().at(1);
      position[2] = st.position().at(2);

      orientation[0] = st.orientation_quaternion().at(0);
      orientation[1] = st.orientation_quaternion().at(1);
      orientation[2] = st.orientation_quaternion().at(2);
      orientation[3] = st.orientation_quaternion().at(3);

      latency = st.delay();

      // Sleep for 500 microseconds
      usleep(500);
    }
  }
}
} // namespace subscriber

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