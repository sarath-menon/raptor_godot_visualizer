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

void Controller::_init() {
  subscriber::mocap_quadcopterSubscriber mysub;
  mysub.init();
}

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
  std::cout << "Ok ryt";
  position = Vector3(0, 0, 0);
  Input *input = Input::get_singleton();

  if (subscriber::new_data == true) {
    // std::cout << "Position      :" << subscriber::position[0] << '\t'
    //           << subscriber::position[1] << '\t' << subscriber::position[2]
    //           << std::endl;

    // std::cout << "Orientation:" << subscriber::orientation[0] << '\t'
    //           << subscriber::orientation[1] << '\t'
    //           << subscriber::orientation[2] << '\t'
    //           << subscriber::orientation[3] << std::endl
    //           << std::endl;

    ///////////////////////////////////////////////////////////////////////////
    // Do godot processing here
    position.z += 5;

    ////////////////////////////////////////////////////////////

    // Set flag to false after data has been processed
    subscriber::new_data = false;
  }

  else {
    std::cout << "No new data" << std::endl;
  }
  // Sleep for 500 microseconds
  usleep(500);
}