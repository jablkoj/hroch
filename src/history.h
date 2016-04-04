#ifndef HISTORY_H
#define HISTORY_H

#include"constants.h"

class History {
    string leaf_dna_sequence;
    map<int, string> leaf_atom_dna;
    History* original;
public:
    vector<HAtom> leaf_atoms;
    map<string, HEvent*> events;

    void clean();
    void reconstruct();
    
    void read_final_sequence(istream& is);
    void read_atoms(istream& is);
    void read_atoms_align(string basepath);
    void read_events(istream& is);
    
    void write_events(ostream& os);

    History(string basename, string id = "");
    History(History* original);
};

#endif
