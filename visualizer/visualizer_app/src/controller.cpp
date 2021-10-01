#include "controller.h"
using namespace godot;
#include <cmath>

///////////////////////////////////////////////////////////////////////////

// Fastdds headers
#include "mocap_quadcopterPubSubTypes.h"
#include "mocap_quadcopterSubscriber.h"
#include <chrono>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
/////////////////////////////////////////////////////////////////////////

Controller::Controller() { mysub.init(); };
Controller::~Controller(){};

void Controller::_init() { position = Vector3(10, 10, 20); }

void Controller::_register_methods() {
  register_method((char *)"_process", &Controller::_process);
  register_method((char *)"UpdateMotionFromInput",
                  &Controller::UpdateMotionFromInput);
}

void Controller::_process(float delta) {

  UpdateMotionFromInput(delta);
  // move_and_slide(position);
  // set_rotation_degrees(orientation);
  set_global_transform(pose);
  global_rotate(axis, angle);
}

void Controller::UpdateMotionFromInput(float delta) {

  if (mocap_sub::new_data == true) {

    ///////////////////////////////////////////////////////////////////////////
    // Do godot processing here

    position.x = mocap_sub::position[0] / 100;
    position.z = mocap_sub::position[1] / 100;
    position.y = mocap_sub::position[2] / 100;

    q.x = mocap_sub::orientation[0];
    q.y = mocap_sub::orientation[1];
    q.z = mocap_sub::orientation[2];
    q.w = mocap_sub::orientation[3];

    const float norm = sqrt(q.x * q.x + q.y * q.y + q.z * q.z);
    angle = 2 * atan2(norm, q.w);

    axis.x = q.x / sqrt(1 - q.w * q.w);
    axis.z = q.y / sqrt(1 - q.w * q.w);
    axis.y = q.z / sqrt(1 - q.w * q.w);

    ////////////////////////////////////////////////////////////

    // Set flag to false after data has been processed
    mocap_sub::new_data = false;
  }

  else {
    // std::cout << "No new data" << std::endl;
  }

  pose.set_origin(position);

  // Sleep for 1 millisecond
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
}