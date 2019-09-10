//
// Created by Andrei Nechaev on 2019-09-02.
//

#ifndef SPEAKER_AUDIOENGINE_H
#define SPEAKER_AUDIOENGINE_H

#include <oboe/Oboe.h>
#include "../collections/Queue.h"

using namespace oboe;

class AudioEngine: public AudioStreamCallback {
public:
    static AudioEngine &get() {
        static AudioEngine instance;
        return instance;
    }

private:
    AudioEngine();

public:
    virtual ~AudioEngine();

    AudioEngine(AudioEngine const &) = delete;

    void operator=(AudioEngine const &) = delete;

    AudioStream *getOutputStream() const;

    AudioStream *getInputStream() const;

    bool init(int32_t deviceInId, int32_t deviceOutId);

    bool start();

    bool stop();

    void release();

    oboe::DataCallbackResult
    onAudioReady(oboe::AudioStream *, void *audioData, int32_t numFrames);

    void onErrorBeforeClose(oboe::AudioStream *, oboe::Result error);

    void onErrorAfterClose(oboe::AudioStream *, oboe::Result error);


private:
    AudioStream* out_;
    AudioStream* in_;
    Queue<int32_t> queue_;
};


#endif //SPEAKER_AUDIOENGINE_H
