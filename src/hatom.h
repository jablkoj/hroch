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
    int atype() { return abs(type); }

    HAtom(int type);
    HAtom(int type, int id);

    friend bool operator==(const HAtom& h1, const HAtom& h2) {
        return h1.type == h2.type && h1.ids == h2.ids;
    }
};


#endif
