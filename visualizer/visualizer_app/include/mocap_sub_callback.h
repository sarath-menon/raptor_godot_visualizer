#pragma once
#include "MocapPubSubTypes.h"
#include "visualizer_subscriber.h"
#include "sensor_msgs/msgs/Mocap.h"

namespace sub {
inline msgs::Mocap st;
inline bool new_data_flag{false};
} // namespace sub

inline void DDSSubscriber::SubListener::on_data_available(DataReader *reader) {
  SampleInfo info;

  if (reader->take_next_sample(&sub::st, &info) == ReturnCode_t::RETCODE_OK) {
    if (info.valid_data) {
      { // Protection against race condition using mutex
        std::unique_lock<std::mutex> lock(m);

        // Print your structure data here.
        ++samples;
        // std::cout << "Sample received, count=" << samples << std::endl;

        sub::new_data_flag = true;
        // std::cout << "subscriber signals data ready for processing\n";
        lock.unlock();
      }
      cv.notify_one();

    }
  }
}