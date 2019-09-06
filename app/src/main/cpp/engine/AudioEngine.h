//
// Created by Andrei Nechaev on 2019-09-02.
//

#ifndef SPEAKER_AUDIOENGINE_H
#define SPEAKER_AUDIOENGINE_H

#include <oboe/Oboe.h>
#include "../RepeaterAudioCallback.h"

using namespace oboe;

class AudioEngine {
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


private:
    AudioStream* out_;
    AudioStream* in_;
    std::unique_ptr<RepeaterAudioCallback> callback_;

};


#endif //SPEAKER_AUDIOENGINE_H
