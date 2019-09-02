//
// Created by Andrei Nechaev on 2019-09-01.
//

#include <android/log.h>
#include <functional>
#include <thread>
#include "Repeater.h"

// Double-buffering offers a good tradeoff between latency and protection against glitches.
constexpr int32_t kBufferSizeInBursts = 2;
constexpr int32_t kRate = 22000;

static AAudioStream* outGlobal_;

aaudio_data_callback_result_t dataCallback(
        AAudioStream *stream,
        void *input,
        void *audioData,
        int32_t numFrames) {

    int32_t fpb = AAudioStream_getFramesPerBurst(outGlobal_);
    auto result = AAudioStream_write(outGlobal_, audioData, numFrames, 0);
    __android_log_print(ANDROID_LOG_DEBUG, "Repeater", "Written something");
    if (result != AAUDIO_OK) {
        __android_log_print(ANDROID_LOG_ERROR, "Repeater", "Error writing to output stream %s",
                            AAudio_convertResultToText(result));
    }
    return AAUDIO_CALLBACK_RESULT_CONTINUE;
}

void errorCallback(AAudioStream *stream,
                   void *userData,
                   aaudio_result_t error){
    if (error == AAUDIO_ERROR_DISCONNECTED){
        __android_log_print(ANDROID_LOG_ERROR, "Repeater", "Error on callback. Writing to output stream %s",
                            AAudio_convertResultToText(error));
//        std::function<void(void)> restartFunction = std::bind(&AudioEngine::restart,
//                                                              static_cast<AudioEngine *>(userData));
//        new std::thread(restartFunction);
    }
}

Repeater::Repeater() {

}

Repeater::~Repeater() {
    stop();
}

bool Repeater::start(int32_t deviceIn, int32_t deviceOut) {
    AAudioStreamBuilder *builder;
    AAudio_createStreamBuilder(&builder);
    AAudioStreamBuilder_setFormat(builder, AAUDIO_FORMAT_PCM_I16);
    AAudioStreamBuilder_setChannelCount(builder, 1);
    AAudioStreamBuilder_setDeviceId(builder, deviceOut);
//    AAudioStreamBuilder_setContentType(builder, AAUDIO_CONTENT_TYPE_SPEECH);
    AAudioStreamBuilder_setSampleRate(builder, kRate);
    AAudioStreamBuilder_setPerformanceMode(builder, AAUDIO_PERFORMANCE_MODE_LOW_LATENCY);
    AAudioStreamBuilder_setDirection(builder, AAUDIO_DIRECTION_OUTPUT);
    auto resultOut = AAudioStreamBuilder_openStream(builder, &out_);
    if (resultOut != AAUDIO_OK) {
        __android_log_print(ANDROID_LOG_ERROR, "Repeater", "Could not open out stream %s",
                            AAudio_convertResultToText(resultOut));
        AAudioStreamBuilder_delete(builder);
        return false;
    }


    AAudioStreamBuilder_setDirection(builder, AAUDIO_DIRECTION_INPUT);
    AAudioStreamBuilder_setDeviceId(builder, deviceIn);
    AAudioStreamBuilder_setDataCallback(builder, ::dataCallback, &in_);
    AAudioStreamBuilder_setErrorCallback(builder, ::errorCallback, this);

    auto resultIn = AAudioStreamBuilder_openStream(builder, &in_);
    if (resultIn != AAUDIO_OK) {
        __android_log_print(ANDROID_LOG_ERROR, "Repeater", "Could not open in stream %s",
                            AAudio_convertResultToText(resultIn));
        AAudioStreamBuilder_delete(builder);
        return false;
    }

    resultOut = AAudioStream_requestStart(out_);
    if (resultOut != AAUDIO_OK) {
        __android_log_print(ANDROID_LOG_ERROR, "AudioEngine", "Error starting stream %s",
                            AAudio_convertResultToText(resultOut));
        return false;
    }
    outGlobal_ = out_;

    resultIn = AAudioStream_requestStart(in_);
    if (resultIn != AAUDIO_OK) {
        __android_log_print(ANDROID_LOG_ERROR, "AudioEngine", "Error starting stream %s",
                            AAudio_convertResultToText(resultIn));
        return false;
    }

    AAudioStreamBuilder_delete(builder);
    return true;
}

bool Repeater::stop() {
    if (in_ != nullptr) {
        AAudioStream_requestStop(in_);
        AAudioStream_close(in_);
    }
    if (out_ != nullptr) {
        AAudioStream_requestStop(out_);
        AAudioStream_close(out_);
    }
    return false;
}


