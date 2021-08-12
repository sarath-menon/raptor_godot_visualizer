#ifndef GDNATIVEEXPLORATION_CONTROLLER_H
#define GDNATIVEEXPLORATION_CONTROLLER_H

#include <Godot.hpp>
#include <Input.hpp>
#include <KinematicBody.hpp>

///////////////////////////////////////////////////////////////////////////

// Fastdds headers
#include "mocap_quadcopterPubSubTypes.h"
#include "mocap_quadcopterSubscriber.h"
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
/////////////////////////////////////////////////////////////////////////

// namespace godot {
class Controller : public godot::KinematicBody {
private:
  // We need to register some information to Godot
  GODOT_CLASS(Controller, KinematicBody)
public:
  // Controller() {}
  // ~Controller() {}

  Controller();
  ~Controller();

  // Finds out which methods can be called on our NativeScript and which
  // properties it exposes
  static void _register_methods();
  // Initialize
  void _init();
  // Describe what happens in the simulation
  void _process();

  // Member fields
  float speed = 8;
  const float friction_coeff = 0.9;
  godot::Vector3 position = godot::Vector3(0.0, 0.0, 0.0);
  godot::Vector3 orientation = godot::Vector3(0.0, 0.0, 0.0);

  // Member functions
  void UpdateMotionFromInput();

  // Fastdds subscriber
  subscriber::mocap_quadcopterSubscriber mysub;
};
// } // namespace godot

#endif // GDNATIVEEXPLORATION_CONTROLLER_H