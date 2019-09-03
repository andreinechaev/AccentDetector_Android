//
// Created by Andrei Nechaev on 2019-09-01.
//

#include <android/log.h>
#include <functional>
#include <thread>
#include "Repeater.h"
#include "RepeaterAudioCallback.h"
#include "AudioEngine.h"

using namespace oboe;

constexpr int32_t kRate = 22000;

Repeater::Repeater() {

}

Repeater::~Repeater() {
    stop();
}

bool Repeater::start(int32_t deviceIn, int32_t deviceOut) {


    auto& engine = AudioEngine::get();
    if (!engine.init(deviceIn, deviceOut)) {
        __android_log_print(ANDROID_LOG_ERROR, "Repeater", "Could not init AudioEngine");
        return false;
    }

    if (!engine.start()) {
        __android_log_print(ANDROID_LOG_ERROR, "Repeater", "Could not start AudioEngine");
        return false;
    }

    __android_log_print(ANDROID_LOG_INFO, "Repeater", "AudioEngine started");
    return true;
}

bool Repeater::stop() {
    auto& engine = AudioEngine::get();
    return engine.stop();
}


