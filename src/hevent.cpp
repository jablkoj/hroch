#include"hevent.h"

ostream& operator<<(ostream& os, const HEvent& event) {
    os << event.species << " " << event.name << " " << (event.parent?event.parent->name:"root") 
       << " " << event.event_time << " " << event.type << " ";

    for(auto a : event.atom_types) os <<  a << " ";
    os << "#";
    for(auto a : event.atom_parents) os << " " << a;
    return os << endl; 
}

HEvent::HEvent() {
    edge_time = event_time = 0.;
    parent = nullptr;
}

HEvent::HEvent(const string& name, GEvent* event, Sequence* after) :
    HEvent::HEvent() {
    this->species = after->name;
    this->name = name;
    this->edge_time = event->get_time();
    this->event_time = after->age;
    this->type = event->name();
}

void HEvent::compute_atoms(HEvent* parent, Sequence* before, Sequence* after) {
    this->parent = parent;
    map<GAtom*, int> position;
    if (before == nullptr) {
        position[nullptr] = -1;
    } else {
        int pos = 0;
        ForGAtom(atom, before) if (atom->get_id()) {
            position[atom] = pos;
            pos++;
        }
    }
    ForGAtom(atom, after) if (atom->get_id()) {
        atoms.push_back(HAtom(atom->get_id(), atom->length()));
        atom_types.push_back(atom->get_id());
        atom_parents.push_back(position[atom->parent]);
    }
}

void HEvent::compute_atom_ids(Sequence* after) {
    int pos = 0;
    ForGAtom(atom, after) if (atom->get_id()) {
        if (SIZE(atom->get_name()))
            atoms[pos].add_id(HAtom::str_to_id(atom->get_name()));
        if (parent != nullptr)
            parent->atoms[atom_parents[pos]].add_ids(atoms[pos].get_ids());
        pos++;
    }
    assert(pos == SIZE(atoms));
}
