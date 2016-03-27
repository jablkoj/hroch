#ifndef HATOM_H
#define HATOM_H

#include"constants.h"

class HAtom {
    static map<string, int> si_map;
    static map<int, string> is_map;
    
    vector<int> ids;
public:
    static string id_to_str(const int& id);
    static int str_to_id(const string& str);
    static void clear_strid_mapping();

    void add_id(int id);
    void add_ids(const vector<int>& what);
    vector<int>& get_ids();
    int type;
    int dna_length;

    HAtom(int type, int dna_length);
};

#endif
