#include"event.h"

double Event::get_time(double time_start) {
    return time_start + time_interval;
}

Event::Event(double time_interval) {
    this->time_interval = time_interval;
}

EventDup::EventDup(int from, int to, int cpos, double time_interval) : 
    Event(time_interval) {
    this->from = from;
    this->to = to;
    this->cpos = cpos;
}

EventDupi::EventDupi(int from, int to, int cpos, double time_interval) : 
    EventDup(from, to, cpos, time_interval) {
}

EventDel::EventDel(int from, int to, double time_interval) : 
    Event(time_interval) {
    this->from = from;
    this->to = to;
}
NoEvent::NoEvent(double time_interval) :
    Event(time_interval) {
}

Sequence* NoEvent::perform(Sequence* sequence) {
    Sequence* s = new Sequence(sequence);
    s->mutate(time_interval);
    return s;    
}

Sequence* EventDup::iperform(Sequence* sequence, bool invert) {
    sequence->split_breakpoints({from, to, cpos});
    cout << "after break   " << *sequence;
    Sequence* s = new Sequence(sequence);
    int pos = 0;
    Atom *cpos_atom = nullptr, *cfront_atom = nullptr, *cback_atom = nullptr;
    Atom *n_atom = s->first, *p_atom = nullptr;
    while(n_atom != nullptr) {
        if (pos == cpos) cpos_atom = p_atom;
        if (pos == from) {
            cfront_atom = new Atom(n_atom->parent, n_atom->parent->get_dna());
            cback_atom = cfront_atom;
        }
        if (pos > from && pos < to) {
            cback_atom->next = new Atom(n_atom->parent, n_atom->parent->get_dna());
            cback_atom = cback_atom->next;
        }
        pos += n_atom->length();
        p_atom = n_atom;
        n_atom = n_atom->next;
    }
    if (invert) {
        Atom *a = nullptr, *b = cfront_atom;
        while(b != cback_atom) {
            b->invert();
            Atom *c = b;
            b = b->next;
            c->next = a;
            a = c; 
        }
        b->invert();
        b->next = a;
        swap(cfront_atom, cback_atom);
    }

    if (cpos_atom == nullptr) {
       cback_atom->next = s->first; 
       s->first = cfront_atom;
    } else {
        cback_atom->next = cpos_atom->next;
        cpos_atom->next = cfront_atom;
    }
    s->mutate(time_interval);
    return s;
}

Sequence* EventDup::perform(Sequence* sequence) {
    return iperform(sequence, false);
}
Sequence* EventDupi::perform(Sequence* sequence) {
    return iperform(sequence, true);
}

Sequence* EventDel::perform(Sequence* sequence) {
    sequence->split_breakpoints({from, to});
    Sequence* s = new Sequence(sequence);
    int pos = 0;
    Atom* n_atom = s->first;
    Atom* p_atom = nullptr;
    while(n_atom != nullptr) {
        if (pos >= from && pos < to) {
            if (p_atom) p_atom->next = n_atom->next;
            else s->first = n_atom->next;
            pos += n_atom->length();
            n_atom->unlink_parent();
            delete n_atom;
            n_atom = (p_atom)?p_atom->next:s->first;
        } else {
            pos += n_atom->length();
            p_atom = n_atom;
            n_atom = n_atom->next;
        }
    }
    s->mutate(time_interval);
    return s;
}

ostream& operator<<(ostream& os, Event& event) {
    return os << event.get_string();
}
