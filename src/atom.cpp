#include"atom.h"

int AtomType::type_cnt = 0;

void AtomType::split(int position) {
    this->root->split(position, nullptr, nullptr);
}
AtomType::AtomType(Atom* root) {
    this->root = root;
    this->id = ++type_cnt;
}

bool Atom::change_parent(Atom* new_parent) {
    if (new_parent != nullptr) this->type = new_parent->type;
    if (parent == new_parent) return false;
    if (parent != nullptr) { 
        auto it = remove(parent->children.begin(), parent->children.end(), this);
        parent->children.resize(distance(parent->children.begin(), it));
    }
    if (new_parent != nullptr)
        new_parent->children.push_back(this);
    parent = new_parent;
    return true;
}

void Atom::invert() {
    inverted = !inverted; 
}

void Atom::mutate(double time) {
    for(auto& c : dna) 
        c = Model::instance()->get_mutated_base(c, time);
}
void Atom::split(int position, Atom* first_parent, Atom* second_parent) {
    if (inverted) {
        Atom* first = new Atom(first_parent, dna.substr(0, position));
        first->inverted = inverted;
        dna = dna.substr(position);
        this->change_parent(second_parent); 
        first->next = this->next;
        this->next = first;
        vector<Atom*> children_copy = children;
        for(Atom* child : children_copy)
            child->split(position, first, this);
    } else {
        Atom* second = new Atom(second_parent, dna.substr(position));
        second->inverted = inverted;
        dna.resize(position);
        this->change_parent(first_parent);
        second->next = this->next;
        this->next = second;
        vector<Atom*> children_copy = children;
        for(Atom* child : children_copy)
            child->split(position, this, second);
    } 
}
    
Atom::Atom(int length) {
    this->inverted = 0;
    this->next = nullptr;
    this->parent = nullptr;
    this->type = new AtomType(this);
    dna = "";
    For(i, length) dna += bases[rand()%BASES];
}

Atom::Atom(Atom* parent, const string &dna) {
    if ((this->parent = parent) == nullptr) {
        this->inverted = 0;
        this->type = new AtomType(this);
        this->next = nullptr;
    } else {
        this->type = parent->type;
        this->inverted = parent->inverted;
        this->next = parent->next;
        parent->children.push_back(this);
    }
    this->dna = dna;
}

string invert_dna(const string& str, bool really = true) {
    if (!really) return str;
    string res = string(SIZE(str), 0);
    For(i, SIZE(str)) res[i] = base_inv[int(str[SIZE(str)-1-i])];
    return res;
}

void Atom::write_dna(ostream& os, string sep) {
    if (dna.size() <= 50)
        os << invert_dna(dna, inverted) << sep;
    else
        os << ".." << dna.size() << ".." << sep;
}
void Atom::write_type(ostream& os, string sep) {
    os << get_id() << sep;
}
ostream& operator<<(ostream& os, Atom& atom) {
    //atom.write_type(os, "(");
    //atom.write_dna(os, ") ");
    atom.write_dna(os, "");
    return os;
}
