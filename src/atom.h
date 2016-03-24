#ifndef ATOM_H
#define ATOM_H

#include"constants.h"

class AtomType {
    static int type_cnt;
public:
    static void reset_ids() {type_cnt = 0;}
    int id;
    Atom* root;
    
    void split(int position);
    AtomType(Atom* root);
};

class Atom {
    AtomType* type;
    vector<Atom*> children;
    string dna;
    bool inverted;
public:
    Atom *parent, *next;

    int length() { return dna.size(); }
    AtomType* get_type() { return type; }
    int get_id() { return inverted?-type->id:type->id; }
    const string& get_dna() { return dna; }
    bool is_inverted() { return inverted; }

    bool change_parent(Atom* new_parent); // return if changed
    void unlink_parent() { change_parent(nullptr); }
    void invert();
    void mutate(double time);
    void split(int position, Atom* first_parent, Atom* second_parent); 
    Atom* duplicate();                                      
    Atom(int length = 0);
    Atom(Atom* parent, const string& dna);

    
    void write_dna(ostream& os = cout, string sep = " ");
    void write_type(ostream& os = cout, string sep = " ");
};
ostream& operator<<(ostream& os, Atom& atom);

#endif
