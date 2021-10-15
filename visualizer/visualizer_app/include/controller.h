#ifndef GDNATIVEEXPLORATION_CONTROLLER_H
#define GDNATIVEEXPLORATION_CONTROLLER_H

#include <Godot.hpp>
#include <Image.hpp>
#include <Input.hpp>
#include <KinematicBody.hpp>
#include <Label.hpp>
#include <PoolArrays.hpp>
#include <SceneTree.hpp>
#include <String.hpp>
#include <Viewport.hpp>
#include <ViewportTexture.hpp>
///////////////////////////////////////////////////////////////////////////

// Non-godot headers

#include "geometry_msgs/msgs/Position.h"
#include "mocap_sub_callback.h"
#include "sensor_msgs/msgs/ImageHd.h"
#include "visualizer_publisher.h"
#include <array>
#include <chrono>
#include <future>
#include <sys/types.h>
/////////////////////////////////////////////////////////////////////////

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

  // Fastdds subscriber
  DDSSubscriber mocap_sub;
  // Create publisher with msg type

  DDSPublisher image_pub;
  ImageHD st;

  // Image pointer
  constexpr static int img_size = 1080 * 720 * 3;

  // // Async future return (used only for asynchronous publishing)
  // std::future<void> status;
};

#endif // GDNATIVEEXPLORATION_CONTROLLER_H