//
// Created by Andrei Nechaev on 2019-09-01.
//

#ifndef SPEAKER_REPEATERAUDIOCALLBACK_H
#define SPEAKER_REPEATERAUDIOCALLBACK_H

#include <oboe/Oboe.h>

class RepeaterAudioCallback: public oboe::AudioStreamCallback {

public:
    RepeaterAudioCallback() {
    }

    virtual ~RepeaterAudioCallback();

    oboe::DataCallbackResult
    onAudioReady(oboe::AudioStream *audioStream, void *audioData, int32_t numFrames);

    void onErrorBeforeClose(oboe::AudioStream *oboeStream, oboe::Result error);

    void onErrorAfterClose(oboe::AudioStream *oboeStream, oboe::Result error);
};


#endif //SPEAKER_REPEATERAUDIOCALLBACK_H
