//
// Created by Andrei Nechaev on 2019-09-01.
//

#ifndef SPEAKER_REPEATER_H
#define SPEAKER_REPEATER_H

#include <oboe/Oboe.h>
#include <atomic>

class Repeater {

public:

    Repeater();

    virtual ~Repeater();

    bool start(int32_t deviceIn, int32_t deviceOut);

    bool stop();
};


#endif //SPEAKER_REPEATER_H
