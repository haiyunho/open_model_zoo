// Copyright (C) 2018-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include <memory>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>

#include <opencv2/opencv.hpp>

#ifdef USE_NATIVE_CAMERA_API
#include "multicam/controller.hpp"
#endif

#include "decoder.hpp"

class Detections {
public:
    template <typename T> T& get() const {
        return *std::static_pointer_cast<T>(detections);
    }
    template <typename T> void set(T* detections) {
        this->detections.reset(detections);
    }
private:
    std::shared_ptr<void> detections;
};

struct YoloParams {
    int num;
    int classes;
    int coords;

    std::vector<float> anchors;
    // std::map<std::string, std::vector<float>> anchors;

    YoloParams(int num, int classes, int coords, std::vector<float>anchors) {
    // YoloParams(int num, int classes, int coords, std::map<std::string, std::vector<float>> anchors) {
        this->num = num;
        this->classes = classes;
        this->coords = coords;
        this->anchors = anchors;
    }
};

class VideoFrame final {
public:
    cv::Mat frame;
    std::size_t sourceIdx = 0;
    Detections detections;
    VideoFrame() = default;

    void operator =(VideoFrame const& vf) = delete;
};

class VideoSource;
class VideoSourceNative;
class VideoSourceOCV;
class VideoSourceStreamFile;

class VideoSources {
private:
    Decoder decoder;
#ifdef USE_NATIVE_CAMERA_API
    mcam::controller controller;
#endif

    std::mutex decode_mutex;  // hardware decoding enqueue lock

    std::vector<std::unique_ptr<VideoSource>> inputs;
    const bool isAsync = false;
    const bool collectStats = false;

    bool realFps;

    const size_t queueSize = 1;
    const size_t pollingTimeMSec = 1000;

    void stop();

    friend VideoSourceNative;
    friend VideoSourceOCV;
    friend VideoSourceStreamFile;

public:
    struct InitParams {
        std::size_t queueSize = 5;
        std::size_t pollingTimeMSec = 1000;
        bool isAsync = true;
        bool collectStats = false;
        bool realFps = false;
        unsigned expectedWidth = 0;
        unsigned expectedHeight = 0;
    };

    explicit VideoSources(const InitParams& p);
    ~VideoSources();

    void openVideo(const std::string& source, bool native);

    void start();

    virtual bool isRunning() const;

    bool getFrame(size_t index, VideoFrame& frame);

    struct Stats {
        std::vector<float> readTimes;
        float decodingLatency = 0.0f;
    };

    Stats getStats() const;
};
