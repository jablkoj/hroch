#include"history.h"

History::History(string basename, string id) {
    if (SIZE(id)) basename += "-" + id;
    ifstream f;
    cout << "Loading " << basename << endl; 

    f.open(basename+".dna", fstream::in);
    read_final_sequence(f);
    f.close();
    f.open(basename+".atoms", fstream::in);
    read_atoms(f);
    f.close();
    read_atoms_align(basename+"/"); 
    f.open(basename+".nhistory", fstream::in);
    read_events(f);
    f.close();

    cout << "       " << basename << " loaded" << endl; 
}

History::History(History* original) {
    
}

void History::clean() {
    for(auto e : events) delete e.second;
    events.clear();
    leaf_atoms.clear();
    leaf_atom_dna.clear();
}

void History::reconstruct() {

}


void History::read_final_sequence(istream& is) {
    is >> leaf_dna_sequence;
}

void History::read_atoms(istream& is) {
    string species, name;
    int type, inverz, from, to;
    while(is >> species >> name >> type >> inverz >> from >> to) {
        type *= inverz;
        leaf_atoms.push_back(HAtom(type, HAtom::str_to_id(name)));
    }
}

void History::read_atoms_align(string basepath) {
    map<int, fstream> files;
    for(HAtom atom : leaf_atoms) if (files.count(atom.atype()) == 0)
        files[atom.atype()].open(basepath+to_string(atom.atype())+".aln", fstream::in);
    for(auto& f : files) {
        string word, buffer, name;
        while(f.second >> word) {
            if (word[0] == '>') {
                if (SIZE(name)) leaf_atom_dna[HAtom::str_to_id(name)] = buffer;
                buffer.clear();
                name = word.substr(1);
            } else buffer += word;
        }
        if (SIZE(name)) leaf_atom_dna[HAtom::str_to_id(name)] = buffer;
        f.second.close();
    }
}

void History::read_events(istream& is) {
    string line;
    HEvent* e;
    while(getline(is, line)){
        istringstream iss(line);
        e = new HEvent(this, iss);
        events[e->name] = e;
    }
    while(e->parent != nullptr) {
        e->parent->compute_atom_ids(e);
        e = e->parent;
    }
}

void History::write_events(ostream& os) {
    for(auto e : events) os << *(e.second);
}
