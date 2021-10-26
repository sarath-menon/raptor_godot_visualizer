#pragma once
#include "MocapPubSubTypes.h"
#include "default_subscriber.h"
#include "sensor_msgs/msgs/Mocap.h"

namespace sub {
inline cpp_msg::Mocap mocap_msg;
inline bool new_data_flag{false};
} // namespace sub
