#include <jni.h>
#include <string>
#include <aaudio/AAudio.h>
#include "Repeater.h"

static Repeater *repeater = new Repeater();

extern "C" {

JNIEXPORT jstring JNICALL
Java_io_faceless_speaker_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

JNIEXPORT void JNICALL
Java_io_faceless_speaker_MainActivity_start(JNIEnv *env, jobject, jint deviceIn,
                                                       jint deviceOut) {

        repeater->start(deviceIn, deviceOut);
}

JNIEXPORT void JNICALL
Java_io_faceless_speaker_MainActivity_stop(JNIEnv *env, jobject, jint deviceIn,
                                            jint deviceOut) {

    repeater->stop();
}
}

