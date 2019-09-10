//
// Created by Andrei Nechaev on 2019-09-02.
//

#include <android/log.h>
#include <thread>
#include "AudioEngine.h"

constexpr int32_t kRate = 22000;

AudioEngine::AudioEngine(): in_(nullptr), out_(nullptr) {
}

AudioEngine::~AudioEngine() {
    release();
};

bool AudioEngine::init(int32_t deviceInId, int32_t deviceOutId) {
    AudioStreamBuilder builder;
    auto resultOut = builder.setPerformanceMode(PerformanceMode::LowLatency)
            ->setBufferCapacityInFrames(256)
            ->setFormat(AudioFormat::I16)
            ->setChannelCount(ChannelCount::Mono)
            ->setDeviceId(deviceOutId)
            ->setSharingMode(SharingMode::Shared)
            ->setContentType(ContentType::Speech)
            ->setSampleRate(kRate)
            ->setDirection(Direction::Output)
            ->openStream(&out_);

    if (resultOut != Result::OK) {
        __android_log_print(ANDROID_LOG_ERROR, "AudioEngine", "Could not open out stream %s",
                            convertToText(resultOut));
        return false;
    }


    auto resultIn = builder.setDirection(Direction::Input)
            ->setDeviceId(deviceInId)
            ->setCallback(this)
            ->openStream(&in_);

    if (resultIn != Result::OK) {
        __android_log_print(ANDROID_LOG_ERROR, "AudioEngine", "Could not open in stream %s",
                            convertToText(resultIn));
        return false;
    }

    return true;
}

bool AudioEngine::start() {
    if (out_ == nullptr || in_ == nullptr) {
        __android_log_print(ANDROID_LOG_ERROR, "AudioEngine", "Engine must be initialized first");
        return false;
    }

    auto resultOut = out_->requestStart();
    if (resultOut != Result::OK) {
        __android_log_print(ANDROID_LOG_ERROR, "AudioEngine", "Could not start out stream %s",
                            convertToText(resultOut));
        return false;
    }

    auto resultIn = in_->requestStart();
    if (resultIn != Result::OK) {
        __android_log_print(ANDROID_LOG_ERROR, "AudioEngine", "Could not start in stream %s",
                            convertToText(resultIn));
        return false;
    }

    return true;
}

bool AudioEngine::stop() {
    if (out_ == nullptr || in_ == nullptr) {
        __android_log_print(ANDROID_LOG_ERROR, "AudioEngine", "Engine was not initialized");
        return false;
    }

    if (in_->requestStop() != Result::OK) {
        __android_log_print(ANDROID_LOG_ERROR, "AudioEngine", "Engine could not stop In stream");
    }



    auto inputState = StreamState::Stopping;
    auto nextState = StreamState::Stopped;
    int64_t timeoutNanos = 100 * kNanosPerMillisecond;
    out_->requestStop();
    auto result = out_->waitForStateChange(inputState, &nextState, timeoutNanos);
    if (result !=  Result::OK) {
        __android_log_print(ANDROID_LOG_ERROR, "AudioEngine", "Engine could not stop Out stream");
        return false;
    }

    if (out_->requestFlush() != Result::OK) {
        __android_log_print(ANDROID_LOG_ERROR, "AudioEngine", "Engine could not flush Out stream");
        return false;
    }

    return true;
}

void AudioEngine::release() {
    if (in_ != nullptr && in_->getState() != StreamState::Closed) {
        in_->close();
    }

    if (out_ != nullptr && out_->getState() != StreamState::Closed) {
        out_->close();
    }

    delete in_;
    delete out_;
}

DataCallbackResult
AudioEngine::onAudioReady(AudioStream *, void *audioData,
                                    int32_t numFrames) {
    auto result = out_->write(audioData, numFrames, 0);
    auto data = static_cast<int32_t* >(audioData);
    std::thread t([&] {

        for (int i = 0; i < numFrames; ++i) {
            auto v = data[i];
            queue_.push(v);
        }
        __android_log_print(ANDROID_LOG_ERROR, "AudioEngine",
                            "Write: size: %d with frames: %d", queue_.size(), numFrames);
    });
    t.detach();
    if (result != Result::OK) {
        __android_log_print(ANDROID_LOG_ERROR, "AudioEngine",
                            "Error writing to output stream");
        return DataCallbackResult::Stop;
    }

    return DataCallbackResult::Continue;
}

void AudioEngine::onErrorBeforeClose(AudioStream *, Result error) {
    __android_log_print(ANDROID_LOG_ERROR, "AudioEngine", "Error on before close %s",
                        convertToText(error));
}

void AudioEngine::onErrorAfterClose(AudioStream *, Result error) {
    __android_log_print(ANDROID_LOG_ERROR, "AudioEngine", "Error on after close %s",
                        convertToText(error));
}


AudioStream *AudioEngine::getOutputStream() const {
    return out_;
}

AudioStream *AudioEngine::getInputStream() const {
    return in_;
}


