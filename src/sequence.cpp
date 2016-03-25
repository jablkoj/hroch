#include"sequence.h"

Sequence::Sequence(double age, int length) {
    this->name = "unicorn";
    this->age = age;
    this->first = nullptr;
    if (length > 0) first = new GAtom(length);
}

Sequence::Sequence(Sequence* parent) : Sequence(parent->age, 0) {
    first = (!parent->first)?nullptr:new GAtom(parent->first, parent->first->get_dna());
    GAtom* atom = first;
    while(atom) {
        atom->next = (!atom->next)?nullptr:new GAtom(atom->next, atom->next->get_dna());
        atom = atom->next;
    }
}

Sequence::~Sequence() {
    GAtom* atom = first;
    while(atom != nullptr) {
        GAtom* old = atom;
        atom = atom->next;
        delete old;
    }
}

int Sequence::length() {
    int len = 0;
    GAtom* atom = first;
    while(atom != nullptr) {
        len += atom->length();
        atom = atom->next;
    }
    return len;
}

void Sequence::mutate(double time) {
    age += time;
    GAtom* atom = first;
    while(atom != nullptr) {
        atom->mutate(time);
        atom = atom->next;
    }
}

void Sequence::split_breakpoints(vector<int> positions) {
    sort(positions.begin(), positions.end());
    GAtom* atom = this->first;
    int pos = 0;
    for(int p : positions) {
        while(atom != nullptr && pos + atom->length() <= p) {
            pos += atom->length();
            atom = atom->next;
        }
        if (pos < p) atom->get_type()->split(atom->is_inverted()?atom->length()-p+pos:p-pos);
    }
}

set<GAtomType*> Sequence::retype_atoms(int length_threshold) {
    set<GAtomType*> res; 
    GAtom* atom = first;
    int name_id = 0;
    while(atom != nullptr) {
        if (atom->length() < length_threshold) {
            atom->get_type()->id = 0;
        } else {
            atom->set_name(name.substr(0,1) + to_string(++name_id));
            if (res.count(atom->get_type()) == 0) {
                res.insert(atom->get_type());
                atom->get_type()->id = SIZE(res);
                if (atom->is_inverted()) atom->get_type()->invert();
            }
        }
        atom = atom->next;
    }
    return res;
}

void Sequence::write_dna(ostream& os, const string& sep) {
    GAtom* atom = first;
    while(atom != nullptr) {
        atom->write_dna(os, "");
        atom = atom->next;
    }
    os << sep;
}

void Sequence::write_atoms_short(ostream& os, const string& sep) {
    GAtom* atom = first;
    while(atom != nullptr) {
        if (atom->get_id()) atom->write_type(os, " ");
        atom = atom->next;
    }
    os << sep;
}

void Sequence::write_atoms(ostream& os) {
    GAtom* atom = first;
    int pos = 0;
    while(atom != nullptr) {
        if (atom->get_id()) {
            os << name << " " << atom->get_name() << " " << abs(atom->get_id()) << " "
               << sign(atom->get_id()) << " " << pos << " " << pos + atom->length() << endl;
        }
        pos += atom->length();
        atom = atom->next;    
    }
}

ostream& operator<<(ostream& os, const Sequence& sequence) {
    os << sequence.age << ": ";
    GAtom* atom = sequence.first;
    while(atom != nullptr) {
        os << *atom << " ";
        atom = atom->next;
    }
    os << endl;
    return os;
}

