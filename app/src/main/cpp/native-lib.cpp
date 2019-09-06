#include <jni.h>
#include <string>
#include <aaudio/AAudio.h>
#include <android/log.h>
#include "engine/AudioEngine.h"

extern "C" {

JNIEXPORT jboolean JNICALL
Java_io_faceless_speaker_audio_Repeater_initEngine(JNIEnv *env, jobject, jint deviceIn,
                                              jint deviceOut) {

    auto& engine = AudioEngine::get();
    engine.release();

    if (!engine.init(deviceIn, deviceOut)) {
        __android_log_print(ANDROID_LOG_ERROR, "Native Lib", "Could not init AudioEngine");
        return static_cast<jboolean>(false);
    }

    return static_cast<jboolean>(true);
}

JNIEXPORT jboolean JNICALL
Java_io_faceless_speaker_audio_Repeater_start(JNIEnv *env, jobject) {
    auto& engine = AudioEngine::get();
    if (!engine.start()) {
        __android_log_print(ANDROID_LOG_ERROR, "Native Lib", "Could not start AudioEngine");
        return static_cast<jboolean>(false);
    }

    __android_log_print(ANDROID_LOG_INFO, "Native Lib", "AudioEngine started");
    return static_cast<jboolean>(true);
}

JNIEXPORT jboolean JNICALL
Java_io_faceless_speaker_audio_Repeater_stop(JNIEnv *env, jobject, jint deviceIn,
                                             jint deviceOut) {
    auto& engine = AudioEngine::get();
    return static_cast<jboolean>(engine.stop());
}
}

