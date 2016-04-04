#ifndef HEVENT_H
#define HEVENT_H

#include"constants.h"

class HEvent {
public:
    string species, name, type;
    double edge_time, event_time;
    HEvent* parent;
  
    vector<HAtom> atoms;
    vector<int> atom_parents;

    HEvent();
    HEvent(History* history, istringstream& iss);
    HEvent(const string& name, GEvent* event, Sequence* after);

    // call when you know the atoms
    void compute_atoms(HEvent* parent, Sequence* before, Sequence* after);
    // call from leaves to root
    void compute_atom_ids(Sequence* after);
    void compute_atom_ids(HEvent* after);
};

ostream& operator<<(ostream& os, const HEvent& event);

#endif
