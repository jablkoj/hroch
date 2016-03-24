#ifndef HISTORY_H
#define HISTORY_H

#include"constants.h"

class History {
public:
    vector<Sequence*> sequences;
    vector<Event*> events;

    
    void generate_random(double time, int sequence_length);

    History() {

    }
};

#endif
