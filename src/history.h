#ifndef HISTORY_H
#define HISTORY_H

#include"constants.h"

class History {
    vector<Sequence*> sequences;
    set<GAtomType*> types;

public:
    vector<HEvent*> events;
    
    void clean();
    void generate_random(double time, int sequence_length);
    void save_to_files(string basename, string id = "");
    void write_stats(ostream& os);
    void write_final_sequence(ostream& os, const string& sep = "\n");
    void write_atoms(ostream& os);
    void write_atoms_align(string basepath);
    void write_events(ostream& os);

    //void load_from_files(string basename, string id = "");

    History() {

    }
};

#endif
