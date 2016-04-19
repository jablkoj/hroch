#include"history.h"

History::History(string basename, string id) {
    cherry_mode = CHERRY_TREE;
    if (SIZE(id)) basename += "-" + id;
    ifstream f;
    cout << "Loading " << basename << endl; 

    for(string species : {"unicorn"}) {
        f.open(basename+"-"+species+".dna", fstream::in);
        read_final_sequence(species, f);
        f.close();
    }
    f.open(basename+".atoms", fstream::in);
    read_atoms(f);
    f.close();
    read_atoms_align(basename+"/"); 
    f.open(basename+".nhistory", fstream::in);
    read_events(f);
    f.close();
    cherry_forest = nullptr;
    if (do_cheeryness) {
        for(string species : {"unicorn"}) {
            read_cherryness(DATAPATH "dupstemp_data-generated-" + id + "-" + species + "-dna/");
        }
    } 
    cout << "       " << basename << " loaded" << endl; 
}

string History::gen_event_name() {
    return "e-"+to_string(SIZE(events));
}

History::History(History* original) {
    cherry_mode = CHERRY_TREE;
    this->original = original;
    this->leaf_species = original->leaf_species;
    this->leaf_atoms = original->leaf_atoms;
    for(auto la : leaf_atoms) {
        HEvent* e = new HEvent(la.first, gen_event_name(), "leaf");
        e->atoms = la.second;
        events[e->name] = e;
    }
    if (do_cheeryness)
        this->cherry_forest = new CherryForest(original->cherry_forest);
}

History::~History() {
    for(auto e : events) delete e.second;
    events.clear();
    leaf_species.clear();
    leaf_atoms.clear();
    leaf_atom_dna.clear();
    if (cherry_forest != nullptr) delete cherry_forest;
}

void History::set_cherry_mode(int mode) {
    cherry_mode = mode;
}

void History::read_final_sequence(string species, istream& is) {
    string word;
    is >> word >> leaf_dna_sequence[species];
    assert(">"+species == word);
}

void History::read_atoms(istream& is) {
    string species, name;
    int type, inverz, from, to;
    while(is >> species >> name >> type >> inverz >> from >> to) {
        type *= inverz;
        leaf_atoms[species].push_back(HAtom(type, HAtom::str_to_id(name)));
    }
    for(auto la : leaf_atoms) leaf_species.push_back(la.first);
}

void History::read_atoms_align(const string& basepath) {
    map<int, fstream> files;
    for(auto atoms : leaf_atoms) for(HAtom atom : atoms.second) 
        if (files.count(atom.atype()) == 0)
            files[atom.atype()].open(basepath+to_string(atom.atype())+".aln", 
                                     fstream::in);
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
    HEvent* e = nullptr;
    while(getline(is, line)){
        istringstream iss(line);
        e = new HEvent(this, iss);
        events[e->name] = e;
    }
    e->atoms = leaf_atoms[e->species];
    while(e->parent != nullptr) {
        e->parent->compute_atom_ids(e);
        e->compute_diff();
        e = e->parent;
    }
}

void History::read_cherryness(const string& basepath) {
    cherry_forest = new CherryForest();
    set<int> atom_types;
    for(auto la : leaf_atoms) for (auto atom : la.second) 
        atom_types.insert(atom.atype());
    for(const auto& at : atom_types) 
        cherry_forest->read_atom(this, at, basepath + to_string(at) + ".nex.trprobs");
}

bool compare(HEvent* e1, HEvent* e2) {
    return e1->event_time < e2->event_time;
}

void History::write_events(ostream& os) {
    vector<HEvent*> just_events;
    for(auto e : events) just_events.push_back(e.second);
    sort(just_events.begin(), just_events.end(), compare);
    for(auto e : just_events) os << *e;
}

void History::write_stats(ostream& os) {
    assert(SIZE(leaf_species)==1);
    string species = leaf_species[0];
    os << species << " " << SIZE(leaf_atoms[species]) << " atoms ";
    os << SIZE(original->events) << " events; time: " << original->get_time();
    os << " (dc" << do_cheeryness << ")" << endl;
    nth_from_end(2)->test_stats(this, os);
    

    for(auto sp : stats) {
        os << sp.first << " " << sp.second << endl;
    }
}

double History::get_time() {
    double min_time = 0.0, max_time = 0.0;
    for(auto e : events) {
        min_time = min(min_time, e.second->event_time);
        max_time = max(max_time, e.second->event_time);
    }
    return max_time - min_time;
}

int History::is_original(HEvent* event) {
    event->compute_diff();
    for(auto ev : original->events) 
        if (*(ev.second) == *(event)) {
            return (nth_from_end(2)==ev.second)?SAME_LAST:SAME_ANY;
        }
    return DIFFERENT;
}

int History::is_correct() {
    if (SIZE(events) != SIZE(original->events))
        return false;
    for(auto ev : events)
        if(!is_original(ev.second)) {
            return false;
        }
    return true;
}

HEvent* History::nth_from_end(int n) {
    auto it = events.end();
    For(i, n) it--;
    return it->second;
}

double History::cherryness(const HAtom& a, const HAtom& b) {
    if (cherry_mode == KNOW_HOW) {
        HEvent* event = original->nth_from_end(SIZE(events));
        int x = -1, y = -2;
        For(i, SIZE(event->atoms)) {
            if (event->atoms[i]==a) x = i;
            if (event->atoms[i]==b) y = i;
        }
        if ((event->atom_parents[x] == event->atom_parents[y])
          &&((x > y) ^ event->compute_is_left())) {
            cout << x << " " << y << " y\n";
            return 80.1;
        } else {
            return 0;
        }
    }
    if (cherry_mode == NO_CHERRY || cherry_forest == nullptr) return 1.0;
    return cherry_forest->cherryness(a,b);
}

void History::merge(const HAtom& a, const HAtom& b) {
    cherry_forest->merge(a,b);
}
