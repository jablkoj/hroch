#ifndef CHERRY_H
#define CHERRY_H

#include"constants.h"

class CherryTree {
    map<HAtom, int> id;
    vector<vi> edges;
    int atype;
public:
    double probability;
    bool has_cherry(const HAtom& a, const HAtom& b);
    void merge(const HAtom& a, const HAtom& b);
    int parse(map<string,int>& ids, const string& word, int from, int to);

    CherryTree(const CherryTree& ct);
    CherryTree(double probability, int atype, map<string, int>& ids, const string& word);
};

class CherryForest {
    map<int, vector<CherryTree>> trees;
    map<int, int> sizes;
public:
    double cherryness(const HAtom& a, const HAtom& b);
    void merge(const HAtom& a, const HAtom& b);
    
    void read_atom(History* history, int type, string filename);
    CherryForest();
    CherryForest(CherryForest* original);
};




#endif
