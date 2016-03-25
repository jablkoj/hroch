#ifndef GEVENT_H
#define GEVENT_H

#include"constants.h"

class GEvent {
protected:
    double time_interval;
    int from, to;
public:    
    virtual Sequence* perform(Sequence* sequence) = 0;
    virtual string get_string() = 0;
    int get_length() { return to-from; }
    double get_time(double time_start = 0.0);
    GEvent(double time_interval);
    virtual ~GEvent() {};
};

class GEventDup: public GEvent {
protected:
    int cpos;
    virtual string name() { return "dup"; }
    Sequence* iperform(Sequence* sequence, bool invert);
public:
    virtual Sequence* perform(Sequence* sequence);
    virtual string get_string() {
        return this->name() + " " + to_string(from) + " " + to_string(to) +
            " " + to_string(cpos);
    }
    GEventDup(int from, int to, int cpos, double time_interval);
};

class GEventDupi: public GEventDup {
protected:
    virtual string name() { return "dupi"; }
public:
    virtual Sequence* perform(Sequence* sequence);
    GEventDupi(int from, int to, int cpos, double time_interval);
};

class GEventDel: public GEvent {
public:
    virtual Sequence* perform(Sequence* sequence);
    virtual string get_string() { return "del " + to_string(from) + " " + to_string(to); }
    GEventDel(int from, int to, double time_interval);
};

class NoGEvent: public GEvent {
public: 
    virtual Sequence* perform(Sequence* sequence);
    virtual string get_string() { return "noevent"; }
    NoGEvent(double time_interval = 0.0);
};

ostream& operator<<(ostream& os, GEvent& event);

#endif
