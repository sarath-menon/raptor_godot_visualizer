#pragma once

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/qos/DataWriterQos.hpp>
#include <fastdds/dds/publisher/qos/PublisherQos.hpp>

#include <chrono>
#include <thread>

#include "ImageHDPubSubTypes.h"


class DDSPublisher {
public:
  DDSPublisher()
      : participant_(nullptr), publisher_(nullptr), topic_(nullptr),
        writer_(nullptr), type_(new ImageHDPubSubType()) {
  }

  std::string topic_name_{};

  virtual ~DDSPublisher();

  bool init();

 void publish(ImageHD *msg) {
    while (listener.matched() == 0) {
      std::this_thread::sleep_for(
          std::chrono::milliseconds(250)); // Sleep 250 ms
    }

    // Publish Message
    writer_->write(msg);
  }

private:
  eprosima::fastdds::dds::DomainParticipant *participant_;
  eprosima::fastdds::dds::Publisher *publisher_;
  eprosima::fastdds::dds::Topic *topic_;
  eprosima::fastdds::dds::DataWriter *writer_;
  eprosima::fastdds::dds::TypeSupport type_;

public:
  class PubListener : public eprosima::fastdds::dds::DataWriterListener {

  public:
    PubListener() = default;
    ~PubListener() override = default;

  private:
    void on_publication_matched(
        eprosima::fastdds::dds::DataWriter *writer,
        const eprosima::fastdds::dds::PublicationMatchedStatus &info) override;

    int matched_ = 0;

  public:
    /// Getter function
    const float matched() const { return matched_; }

  } listener;
};
