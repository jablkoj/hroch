#ifndef CONSTANTS_H
#define CONSTANTS_H

#include<cstdio>
#include<iostream>
#include<sstream>
#include<string>
#include<algorithm>
#include<vector>
#include<map>
#include<set>
#include<cmath>
#include<cassert>
#include<queue>
using namespace std;

#define For(i,n) for(int i=0; i<int(n); ++i)
#define SIZE(i) int(i.size())
#define BASES 4

extern char bases[];
struct trint;
class Atom;
class Event;
class Sequence;
//class AtomTree;
//class History;

#include"random.h"
#include"atom.h"
#include"event.h"
#include"sequence.h"

typedef pair<int, int> pii;
typedef pair<pii, int> piii;
typedef pair<pii, double> piid;

typedef vector<pii> vpi;
typedef vector<vpi> vvpi;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<vvi> vvvi;
typedef vector<double> vdo;
typedef vector<vdo> vvdo;
//typedef pair<string, int> atom_element;
typedef vector<trint> vtri;

struct trint {
    int a,b,c;
    trint() {a = b = c = 0;}
    trint(const int& a, const int& b, const int& c) {
        this->a = a;
        this->b = b;
        this->c = c;
    }
};
inline bool operator==(const trint& t1, const trint& t2){
    return t1.a==t2.a && t1.b==t2.b && t1.c==t2.c;
}
inline bool operator!=(const trint& t1, const trint& t2){
    return t1.a!=t2.a || t1.b!=t2.b || t1.c!=t2.c;
}
inline bool operator<(const trint& t1, const trint& t2){
    if (t1.a!=t2.a) return t1.a<t2.a;
    if (t1.b!=t2.b) return t1.b<t2.b;
    return t1.c<t2.c;
}

template<typename T>
ostream& operator<<(ostream& os, const vector<T> v) {
    os << "[";
    For(i, SIZE(v)) (i?(os << ", "):os) << v[i]; 
    return os << ']';
}

extern int debuging;
extern char bases[BASES];
extern int base_id[256];
extern char base_inv[256];

inline void mustbe(const bool& True, const string& message=""){
    if (True!=true){
        cerr << "Error: " << message << endl; 
        exit(1);
    }
}

void setup_constants();

#endif
