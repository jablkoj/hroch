#ifndef HISTORY_H
#define HISTORY_H

#include"constants.h"

#define DIFFERENT 0
#define SAME_ANY 1
#define SAME_LAST 2

#define NO_CHERRY 0
#define CHERRY_TREE 1
#define KNOW_HOW 2

class History {
    int cherry_mode;
    vector<string> leaf_species;
    map<string, string> leaf_dna_sequence;
    map<int, string> leaf_atom_dna;
    CherryForest* cherry_forest;
public:
    History* original;
    map<string, double> stats;
    map<string, vector<HAtom>> leaf_atoms;
    map<string, HEvent*> events;

    void proc_reconstruct(int number = -1); // -1 means infinity
    void proc_test(string stypes);
    void rec_parent(HEvent* event);
    int rec_test(HEvent* event, vector<Dynamics>& ds);
    int rec_test2(HEvent* event, vector<Dynamics>& ds);
    void rec_compute_parent(const Candidate& C, HEvent* event);
    HEvent* rec_see_event(const Candidate& C, HEvent* event);
    void rec_merge_candidate(const Candidate& C, HEvent* event);
    vector<Candidate> rec_candidates(HEvent* event);
    double rec_score(const Candidate& c, HEvent* event);
    void set_cherry_mode(int mode);

    HEvent* nth_from_end(int n);
    int is_original(HEvent* event);
    int is_correct();
    double cherryness(const HAtom& a, const HAtom& b);
    void merge(const HAtom& a, const HAtom& b);

    void read_final_sequence(string species, istream& is);
    void read_atoms(istream& is);
    void read_atoms_align(const string& basepath);
    void read_events(istream& is);
    void read_cherryness(const string& basepath);
    
    string gen_event_name();
    void write_events(ostream& os);
    void write_stats(ostream& os);
    double get_time();

    History(string basename, string id = "");
    History(History* original);
    ~History();
};

#endif
