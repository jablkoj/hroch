#include"sequence.h"

Sequence::Sequence(double age, int length) {
    this->age = age;
    this->first = nullptr;
    if (length > 0) first = new Atom(length);
}

Sequence::Sequence(Sequence* parent) : Sequence(parent->age, 0) {
    first = (!parent->first)?nullptr:new Atom(parent->first, parent->first->get_dna());
    Atom* atom = first;
    while(atom) {
        atom->next = (!atom->next)?nullptr:new Atom(atom->next, atom->next->get_dna());
        atom = atom->next;
    }
}

Sequence::~Sequence() {
    Atom* atom = first;
    while(atom != nullptr) {
        Atom* old = atom;
        atom = atom->next;
        delete old;
    }
}

int Sequence::length() {
    int len = 0;
    Atom* atom = first;
    while(atom != nullptr) {
        len += atom->length();
        atom = atom->next;
    }
    return len;
}

void Sequence::mutate(double time) {
    age += time;
    Atom* atom = first;
    while(atom != nullptr) {
        atom->mutate(time);
        atom = atom->next;
    }
}

void Sequence::split_breakpoints(vector<int> positions) {
    sort(positions.begin(), positions.end());
    cout << "Splitting: ";
    for(auto p : positions) cout << p << " ";
    cout << endl;
    Atom* atom = this->first;
    int pos = 0;
    for(int p : positions) {
        while(atom != nullptr && pos + atom->length() <= p) {
            pos += atom->length();
            atom = atom->next;
        }
        if (pos < p) atom->get_type()->split(atom->is_inverted()?atom->length()-p+pos:p-pos);
    }
}

ostream& operator<<(ostream& os, const Sequence& sequence) {
    os << sequence.age << ": ";
    Atom* atom = sequence.first;
    while(atom != NULL) {
        os << *atom; 
        atom = atom->next;
    }
    os << endl;
    return os;
}

