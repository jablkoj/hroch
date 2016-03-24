#ifndef EVENT_H
#define EVENT_H

#include"constants.h"

class Event {
protected:
    double time_interval;
    int from, to;
public:    
    virtual Sequence* perform(Sequence* sequence) = 0;
    virtual string get_string() = 0;
    int get_length() { return to-from; }
    double get_time(double time_start = 0.0);
    Event(double time_interval);
    virtual ~Event() {};
};

class EventDup: public Event {
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
    EventDup(int from, int to, int cpos, double time_interval);
};

class EventDupi: public EventDup {
protected:
    virtual string name() { return "dupi"; }
public:
    virtual Sequence* perform(Sequence* sequence);
    EventDupi(int from, int to, int cpos, double time_interval);
};

class EventDel: public Event {
public:
    virtual Sequence* perform(Sequence* sequence);
    virtual string get_string() { return "del " + to_string(from) + " " + to_string(to); }
    EventDel(int from, int to, double time_interval);
};

class NoEvent: public Event {
public: 
    virtual Sequence* perform(Sequence* sequence);
    virtual string get_string() { return "noevent"; }
    NoEvent(double time_interval = 0.0);
};

ostream& operator<<(ostream& os, Event& event);

#endif
