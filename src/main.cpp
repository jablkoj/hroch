#include"constants.h"

void generate() {
    vector<double> stimes;
    For(i, 20) stimes.push_back(0.02);
    For(i, 20) stimes.push_back(0.03);
    For(i, 20) stimes.push_back(0.04);
    For(i, 20) stimes.push_back(0.05);
    For(i, SIZE(stimes)) {
        GHistory* history = new GHistory();
        history->generate_random(stimes[i], 100000);
        history->save_to_files(DATAPATH "generated", to_string(i));
    }
}

void write_all_stats(map<string, vector<double>>& stats, ostream& os) {
    os << "all stats" << endl;
    for(auto sp : stats) {
        vector<double> stat = sp.second;
        sort(stat.begin(), stat.end());
        double avg = 0;
        for(double d : stat) avg += d;
        avg /= SIZE(stat);
        double p10 = stat[int(SIZE(stat)*0.1)];
        double p50 = stat[int(SIZE(stat)*0.5)];
        double p90 = stat[int(SIZE(stat)*0.9)];

        os << sp.first << "  AGV: " << avg << "  10% " << p10
           << "  50% " << p50 << "  90% " << p90 << endl;
    }
}

void test_last() {
    vector<string> stypes = {
        "0","1","2","3","4","5","6","7","8","9",
        //"047","02468","147","048","8","8","8","8","8",
        
        "0o","1o","2o","3o","4o","5o","6o","7o","8o","9o",
        "8a","8b","8c","8d","8e",
        //"047o","02468o","147o","048o","8o","8o","8o","8o","8o",
    };
    vector<History*> histories;
    map<string, vector<double>> stat_val;

    cout << "here" << endl;
    ofstream file("stats");
    for(int hid = 0; hid<20; ++hid) {
        History* h0 = new History(DATAPATH "generated", to_string(hid));
        History* h1 = new History(h0);
        histories.push_back(h1);
        h1->stats["name"] = hid;

        for(auto stype : stypes) h1->proc_test(stype);

        h1->write_stats(file);
        for(auto sp : h1->stats) stat_val[sp.first].push_back(sp.second);
    }
    file.close();

    for(auto h1 : histories) {
        h1->write_stats(cout);
        delete h1->original;
        delete h1;
    }
    histories.clear();
    write_all_stats(stat_val,cout);
}

void reconstruct(int hid) {
    History* h0 = new History(DATAPATH "generated", to_string(hid));
    for(auto ev : h0->events) ev.second->test_stats(h0, cout);
    for(auto ev : h0->events) ev.second->write_detailed(cout);
    For(i, 5) {
        if (i%1==0) cout << "attempt " << i << endl;
        History* h1 = new History(h0);
        h1->set_cherry_mode(KNOW_HOW);
        h1->proc_reconstruct();
        for(auto ev : h1->events) ev.second->test_stats(h1, cout);
        for(auto ev : h1->events) ev.second->write_detailed(cout);

        //h1->write_events(cout);
        if (h1->is_correct()) {
            cout << "Yay!! " << i << endl;
            h1->write_events(cout);
            break;
        }

        delete h1;
    }
    
}

int main(int argc, char **argv) {
    setup_constants();
    auto args = parse_arguments(argc, argv);
    cout << "Program started" << endl;
    if (args.count("generate")) generate();
    if (args.count("test")) test_last();
    if (args.count("rec")) {
        reconstruct(1);
        //reconstruct(2);
        //reconstruct(6);
    }
}
