#include"constants.h"

void generate_history(double time, string id) {
    GHistory* history = new GHistory();
    history->generate_random(time, 100000);
    history->save_to_files(DATAPATH "generated", id);
    delete history; 
}

void generate_test() {
    for(int i = 100; i < 200; ++i) generate_history(0.02, "F2"+to_string(i));
    for(int i = 100; i < 200; ++i) generate_history(0.03, "F3"+to_string(i));
    for(int i = 100; i < 200; ++i) generate_history(0.04, "F4"+to_string(i));
}

void generate_train() {
    for(int i = 10000; i < 15000; ++i) generate_history(0.04, "L4"+to_string(i));
}

void write_all_stats(map<string, vector<double>>& stats, ostream& os) {
    os << "all stats" << endl;
    for(auto sp : stats) {
        vector<double> stat = sp.second;
        sort(stat.begin(), stat.end());
        double avg = 0;
        for(double d : stat) avg += d;
        avg /= SIZE(stat);
        double p0 = stat[0];
        double p10 = stat[int(SIZE(stat)*0.1)];
        double p50 = stat[int(SIZE(stat)*0.5)];
        double p90 = stat[int(SIZE(stat)*0.9)];

        os << sp.first << "  AGV: " << avg
           << "   0% " <<  p0 << "  10% " << p10
           << "  50% " << p50 << "  90% " << p90 << endl;
    }
}

void test_candi() {
    map<string, vector<double>> stat_val;
    ofstream file("stats/last-stats");
    string prefix = "T4";

    for(int hid = 10; hid<60; ++hid) {
        History* h0 = new History(DATAPATH "generated", prefix+to_string(hid));
        History* h1 = new History(h0);
        h1->stats["_name"] = hid;
        
        h1->proc_test_candi(CHERRY_NO,"1nc");
        h1->proc_test_candi(CHERRY_TREE,"1ct");
        h1->proc_test_candi(CHERRY_LEN,"1cl");
        h1->proc_test_candi(SCORE_BAC_NC,"nc");
        h1->proc_test_candi(SCORE_BAC,"def");
        h1->proc_test_candi(SCORE_CL,"cl");
    
        h1->write_stats(file);
        file << endl;
        for(auto sp : h1->stats) stat_val[sp.first].push_back(sp.second);
        delete h0;
        delete h1;
    }
    write_all_stats(stat_val,cout);
    write_all_stats(stat_val,file);
    file.close();
}

int full = 0, stupid = 0, smart = 0;

void train_history(Machine* machine, string name) {
    error_happened = 0;
    History* h0 = new History(DATAPATH "generated", name);
    if (error_happened) {
        cout << "Could not train with hitory " << name << endl;
        return;
    }
    
    History* hsp = new History(h0);
    hsp->set_strategy(SCORE_LR,machine);
    hsp->proc_learn();
    hsp->save("g-train-"+name);

    bool hasdel = false;
    for (auto ev : h0->events) if (ev.second->type == "del") hasdel = true;
    if (hsp->stats.count("full")) {
        full++;
        if (hasdel) {
            smart++;
            cout << "reconstruced with deletion " << name << endl;
            hsp->write_events(cout);
        }
    } else {
        if (!hasdel) {
            stupid++;
            cout << "not reconstruced without deletion " << name << endl;
        }
    }
    delete hsp;
    delete h0;
}

void train() {
    full = 0;
    Machine* machine = new MachineLinear();
    for(int i = 10000; i < 14000; ++i) train_history(machine,"L4"+to_string(i));
    machine->save();
    delete machine;
    cout << "training finished" << endl;
    cout << "fully reconstructed " << full << endl;
    cout << "smart reconstructed " << smart << endl;
    cout << "stupid reconstructed " << stupid << endl;
}

void reconstruct_one(History* h0, string hid, int strategy) {
    cout << "Reconstructing " << hid << " with strategy " << strategy << endl;

    Machine* machine = nullptr;
    if (strategy == SCORE_CL) machine = new MachineOne();
    if (strategy == SCORE_BAC_NC) machine = new MachineBachelor();
    if (strategy == SCORE_BAC) machine = new MachineBachelor();
    if (strategy == SCORE_LR) machine = new MachineLinear();
    if (machine != nullptr) machine->load();

    int attempts = 1000;
    int max_events = 0;
    double avg_num_events = 0.0;
    vector<int> dist_num_events(1000,0);
    vector<int> cnt_events(SIZE(h0->events)-1,0);
    
    For(i, attempts) {
        if (i%2000==0) cout << "history " << hid << "  attempt " << i << endl;
        History* h1 = new History(h0);
        h1->set_strategy(strategy, machine);
        h1->proc_reconstruct(stats?EVAL_INF:EVAL_LAZY);
        max_events = max(max_events, int(h1->stats["max_events"]));
        cnt_events[int(h1->stats["max_events"])]++;
        if (stats) {
            avg_num_events += SIZE(h1->events);
            dist_num_events[SIZE(h1->events)]++;
        }
        delete h1;
    }
    avg_num_events /= attempts;
    while(SIZE(dist_num_events) && dist_num_events.back() == 0) 
        dist_num_events.pop_back();
    dist_num_events[SIZE(h0->events)-1]--;

    cout << "  " << SIZE(h0->events) << " events,  " 
         << SIZE(h0->leaf_atoms.begin()->second) << " atoms" << endl;
    cout << "  max_events " << max_events << "    " << cnt_events << endl;
    if (stats) {
        cout << "  length of events: avg=" <<avg_num_events << "  ::  ";
        cout << dist_num_events << endl;
    }
    cout << endl;
    if (machine != nullptr) delete machine;
}

void reconstruct_many(string hid) {
    History* h0 = new History(DATAPATH "generated", hid);
    if (debuging) {
        for(auto ev : h0->events) ev.second->test_stats(h0, cout);
        for(auto ev : h0->events) ev.second->write_detailed(cout);
    }
    History* hsp = new History(h0);
    hsp->set_strategy(KNOW_HOW);
    hsp->proc_reconstruct();
    if (!hsp->is_correct()) {
        cout << hid << " cannot be reconstructed" << endl;
        delete hsp;
        return;
    }
    delete hsp;
    //reconstruct_one(h0, hid, CHERRY_NO);
    //reconstruct_one(h0, hid, CHERRY_TREE);
    //reconstruct_one(h0, hid, CHERRY_LEN);
    //reconstruct_one(h0, hid, SCORE_BAC_NC);
    reconstruct_one(h0, hid, SCORE_BAC);
    reconstruct_one(h0, hid, SCORE_LR);
    delete h0;
}

void help(string name) {
    cout << "Usage: "<< name << " TESTNAME COUNT" << endl;
    cout << "       reconstruct COUNT histories of TESTMAME DNA" << endl;

    cout << endl << "other (undocumented) options:" << endl;
    cout << "  --gen-test:      generate test data" << endl;
    cout << "  --gen-train:     generate train data" << endl;
    cout << "  --train:         produce lr-train file" << endl;
    cout << "  --test:          make statistics of candidate proposal algorithms" << endl;
    cout << "  --rec:           make statistics of history reconstructin algorithms" << endl;
}

int main(int argc, char **argv) {
    setup_constants();
    auto args = parse_arguments(argc, argv);
    cout << "Program started" << endl;
    if (args.count("--help")) {
        help(argv[0]);
        return 0;
    }
    if (args.count("--gen-test")) generate_test();
    if (args.count("--gen-train")) generate_train();
    if (args.count("--train")) train();
    if (args.count("--test")) test_candi();
    if (args.count("--rec")) {
        for(int i = 10; i<30; ++i)
            reconstruct_many("T2"+to_string(i));
    }
}
