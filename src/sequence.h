#ifndef SEQUENCE_H
#define SEQUENCE_H

#include"constants.h"

class Sequence {
public:
    Atom* first;
    double age;
   
    Sequence(double age = 0.0, int length = 0);
    Sequence(Sequence* parent);
    ~Sequence();
    int length();
    void mutate(double time);
    void split_breakpoints(vector<int> positions);
};

ostream& operator<<(ostream& os, const Sequence& sequence);

#endif
