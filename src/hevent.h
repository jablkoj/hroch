#ifndef HEVENT_H
#define HEVENT_H

#include"constants.h"


class HEvent {
public:
    string species, name, type;
    double edge_time, event_time;
    HEvent* parent;
   

};

#endif
