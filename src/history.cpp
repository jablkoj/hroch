#include"history.h"
#include<sys/stat.h>

void History::generate_random(double time, int sequence_length) {
    if (SIZE(sequences)) {
        this->types = sequences[0]->retype_atoms(0);
        for(auto t : types) delete t;
        types.clear();
    }
    for(auto s : sequences) delete s;
    sequences.clear();
    for(auto e : events) delete e;
    events.clear();
    GAtomType::reset_ids(); 
    
    sequences.push_back(new Sequence(0.0, sequence_length));

    double current_time = 0.0;
    while(current_time < time) {
        Sequence* seq = sequences.back();
        GEvent* event = Model::instance()->get_random_event(seq->length());
        while (event->get_length() == 0) {
            delete event;
            event = Model::instance()->get_random_event(seq->length());
        }
        current_time = event->get_time(current_time);
        
        Sequence* nextseq = event->perform(seq);
        delete event;
        sequences.push_back(nextseq);
    }
    this->types = sequences.back()->retype_atoms(Model::instance()->length_threshold);
    
    /*for(auto s : sequences) cout << *s;
    cout << endl;
    for(auto s : sequences) s->write_atoms_short();
    cout << endl;*/
    sequences.back()->write_atoms_short();
    cout << SIZE(sequences) << " " << sequences.back()->length() << endl;
}

void History::save_to_files(string basename, string id) {
    if (SIZE(id)) basename += "-" + id;
    ofstream f;
    cout << "Saving " << basename << endl; 

    f.open(basename+".dna", fstream::out);
    write_final_sequence(f);
    f.close();
    f.open(basename+".atoms", fstream::out);
    write_atoms(f);
    f.close();
    write_atoms_align(basename+"/"); 
    f.open(basename+".nhistory", fstream::out);
    write_events(f);
    f.close();

    cout << "       " << basename << " saved" << endl; 
}

void History::write_stats(ostream& os) {
    os << "Number of events: " << SIZE(events) << endl;
}

void History::write_final_sequence(ostream& os, const string& sep) {
    mustbe(SIZE(sequences), "Missing sequences.");
    sequences.back()->write_dna(os, sep);
}

void History::write_atoms(ostream& os) {
    mustbe(SIZE(sequences), "Missing sequences.");
    sequences.back()->write_atoms(os); 
}

void History::write_atoms_align(string basepath) {
    mustbe(SIZE(sequences), "Missing sequences.");
    map<int, fstream> files;
    mkdir(basepath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    for(const auto& type : types) 
        files[type->id].open(basepath+to_string(type->id)+".aln", fstream::out);
    GAtom* atom = sequences.back()->first;
    while(atom != nullptr) {
        int id = atom->get_type()->id;
        if (id) {
            mustbe(files.count(id), "Not found type for atom");
            files[id] << ">" << atom->get_name() << endl
                      << atom->get_dna() << endl;
        }
        atom = atom->next;
    }
    for(auto& f : files) f.second.close();
}

void History::write_events(ostream& os) {

}
