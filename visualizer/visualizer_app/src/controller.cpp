#include "controller.h"
using namespace godot;
#include <chrono>
#include <cmath>

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
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  }
}
} // namespace subscriber

Controller::Controller() { mysub.init(); };
Controller::~Controller(){};

void Controller::_init() {}

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

  position = Vector3(10, 10, 20);

  if (subscriber::new_data == true) {

    ///////////////////////////////////////////////////////////////////////////
    // Do godot processing here

    position.x = subscriber::position[0] / 100;
    position.z = subscriber::position[1] / 100;
    position.y = subscriber::position[2] / 100;

    q.x = subscriber::orientation[0];
    q.y = subscriber::orientation[1];
    q.z = subscriber::orientation[2];
    q.w = subscriber::orientation[3];

    // angle = 2 * acos(q.w);
    float norm = sqrt(q.x * q.x + q.y * q.y + q.z * q.z);
    angle = 2 * atan2(norm, q.w);

    axis.x = q.x / sqrt(1 - q.w * q.w);
    axis.z = q.y / sqrt(1 - q.w * q.w);
    axis.y = q.z / sqrt(1 - q.w * q.w);

    ////////////////////////////////////////////////////////////

    // Set flag to false after data has been processed
    subscriber::new_data = false;
  }

  else {
    // std::cout << "No new data" << std::endl;
  }

  pose.set_origin(position);

  // Sleep for 500 microseconds
  std::this_thread::sleep_for(std::chrono::milliseconds(40));
}