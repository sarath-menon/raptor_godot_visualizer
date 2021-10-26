#ifndef GDNATIVEEXPLORATION_CONTROLLER_H
#define GDNATIVEEXPLORATION_CONTROLLER_H

#include "include_helper.h"

class Controller : public godot::Spatial {
private:
  // We need to register some information to Godot
  GODOT_CLASS(Controller, Spatial)
public:
  Controller();
  ~Controller();

  // Finds out which methods can be called on our NativeScript and which
  // properties it exposes
  static void _register_methods();
  // Initialize
  void _init();
  // Describe what happens in the simulation
  void _process(float delta);

  godot::Vector3 position = godot::Vector3(0.0, 0.0, 0.0);
  godot::Vector3 orientation = godot::Vector3(0.0, 0.0, 0.0);

  godot::Quat q;
  godot::Vector3 axis = godot::Vector3(0.0, 0.0, 0.0);
  float angle;

  godot::Transform pose;

  // Member functions
  void UpdateMotionFromInput(float delta);

  // Scaling factor to adjust real world vs sim distance
  constexpr static float scaling_factor = 4;

  // Image pointer
  constexpr static int img_size = 1280 * 720 * 3;

  // Create doamin participant
  std::shared_ptr<DefaultParticipant> dp;
  // DefaultParticipant *dp;

  // Image publisher
  // std::unique_ptr<DDSPublisher> image_pub_;
  VideoPublisher *image_pub;

  idl_msg::Image720p st;

  // Motion capture data subscriber
  // std::unique_ptr<DDSSubscriber> mocap_sub;
  DDSSubscriber<idl_msg::MocapPubSubType, cpp_msg::Mocap> *mocap_sub;

  // // Async future return (used only for asynchronous publishing)
  // std::future<void> status;
};

#endif // GDNATIVEEXPLORATION_CONTROLLER_H