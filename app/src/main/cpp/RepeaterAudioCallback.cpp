//
// Created by Andrei Nechaev on 2019-09-01.
//

#include <android/log.h>
#include <thread>
#include "RepeaterAudioCallback.h"
#include "AudioEngine.h"

oboe::DataCallbackResult
RepeaterAudioCallback::onAudioReady(oboe::AudioStream *audioStream, void *audioData,
                                    int32_t numFrames) {
    auto &engine = AudioEngine::get();
    auto out = engine.getOutputStream();
    auto result = out->write(audioData, numFrames, 0);
    if (result != oboe::Result::OK) {
        __android_log_print(ANDROID_LOG_ERROR, "RepeaterAudioCallback",
                            "Error writing to output stream");
    }

    return oboe::DataCallbackResult::Continue;
}

void RepeaterAudioCallback::onErrorBeforeClose(oboe::AudioStream *oboeStream, oboe::Result error) {
    __android_log_print(ANDROID_LOG_ERROR, "RepeaterAudioCallback", "Error on before close %s",
                        oboe::convertToText(error));
}

void RepeaterAudioCallback::onErrorAfterClose(oboe::AudioStream *oboeStream, oboe::Result error) {
    __android_log_print(ANDROID_LOG_ERROR, "RepeaterAudioCallback", "Error on after close %s",
                        oboe::convertToText(error));
}

RepeaterAudioCallback::~RepeaterAudioCallback() = default;
