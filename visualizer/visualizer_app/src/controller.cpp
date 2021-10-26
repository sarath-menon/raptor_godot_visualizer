#include "controller.h"
#include "default_participant.h"
#include <cstddef>
#include <future>
#include <memory>
using namespace godot;
#include <cmath>

Controller::Controller() {

  // Create domain participant
  dp = std::make_shared<DefaultParticipant>(0, "godot_visualizer_qos");
  // dp = new DefaultParticipant(0, "opencv_demo_qos");

  // Create  publisher
  image_pub = new VideoPublisher(idl_msg::Image720pPubSubType(), "sim_img",
                                 dp->participant());

  // Create  subscriber
  mocap_sub = new DDSSubscriber(idl_msg::MocapPubSubType(), &sub::mocap_msg,
                                "mocap_pose", dp->participant());

  // // Create  publisher
  // image_pub_ = std::make_unique<DDSPublisher>(ImageHDPubSubType(), "sim_img",
  //                                             dp->participant());

  // // Create  subscriber
  // mocap_sub = std::make_unique<DDSSubscriber>(MocapPubSubType(),
  // "mocap_pose",
  //                                             dp->participant());

  // Initialize publisher
  image_pub->init();

  // initialize  subscriberDefaultParticipant
  mocap_sub->init();
}

Controller::~Controller() {
  // delete dp;
  delete image_pub;
  delete mocap_sub;
};

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

  // Lock until read and write are completed

  // wait for the subscriber
  // std::unique_lock<std::mutex> lk(mocap_sub->listener->m);
  // mocap_sub->listener->cv.wait_for(lk, std::chrono::milliseconds(250),
  //                                  [] { return sub::new_data_flag; });

  // sub::new_data_flag = false;

  mocap_sub->listener->wait_for_data();

  position.x = sub::mocap_msg.pose.position.x * scaling_factor;
  position.z = sub::mocap_msg.pose.position.y * scaling_factor;
  position.y = sub::mocap_msg.pose.position.z * scaling_factor;

  q.x = sub::mocap_msg.pose.orientation_quat.x;
  q.y = sub::mocap_msg.pose.orientation_quat.y;
  q.z = sub::mocap_msg.pose.orientation_quat.z;
  q.w = sub::mocap_msg.pose.orientation_quat.w;

  // Do godot processing here
  const float norm = sqrt(q.x * q.x + q.y * q.y + q.z * q.z);
  angle = 2 * atan2(norm, q.w);

  axis.x = q.x / sqrt(1 - q.w * q.w);
  axis.z = q.y / sqrt(1 - q.w * q.w);
  axis.y = q.z / sqrt(1 - q.w * q.w);

  axis.normalized();

  // Get image from the main viewing window
  godot::Ref<godot::Image> img = get_viewport()->get_texture()->get_data();

  if (img.is_valid() && !img->is_empty()) {

    // Covert raw image to RGB 8 channel (code ->4)
    img->convert(4);

    // make sure image doesn't change while being moved
    img->lock();

    // Flip image verically to correct orientation
    img->flip_y();

    // // To save image as PNG file
    // img->save_png("/home/srl/Documents/godot_visualizer/visualizer/visualizer_app/images/waste.png");

    // Move data from godot arrray to std::array
    memcpy(st.frame().data(), img->get_data().write().ptr(), img_size);

    img->unlock();
  }

  // Publish synchronously
  image_pub->publish(st);

  // // Publish asynchronously
  // status =
  //     std::async(std::launch::async, &DDSPublisher::publish, image_pub,
  //     st);

  ////////////////////////////////////////////////////////////

  // Set position in global pose
  pose.set_origin(position);
}