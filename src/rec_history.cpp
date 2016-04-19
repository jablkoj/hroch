#include"history.h"

void History::rec_parent(HEvent* event) {
    cout << "rec parent:" << event->compute_is_left() << endl;
    vector<Candidate> cs = rec_candidates(event);
    assert(SIZE(cs)>0);
    vector<double> sum_scores = {0};
    for(auto c : cs) {
        double score = rec_score(c, event);
        sum_scores.push_back(sum_scores.back() + score);
    }
    double pick = random_double(0, sum_scores.back());
    For(i, SIZE(cs)) if (pick < sum_scores[i+1]) {
        rec_compute_parent(cs[i], event);
        rec_merge_candidate(cs[i], event);
        return;
    }
}

int History::rec_test(HEvent* event, vector<Dynamics>& ds) {
    //cout << "testing " << *event << endl;
    int cnt = 0;
    set<Candidate> cs;
    while(cnt < 2000) {
        Candidate c = ds[cnt%SIZE(ds)].get_candidate();
        if (!c.is_valid()) continue;
        cs.insert(c);
        if (SIZE(cs) > 4*SIZE(event->atoms)) break; 
        cnt++;
        HEvent* e = rec_see_event(c, event);
        bool is_good = is_original(e);
        delete e;
        if (is_good) return 1;
    }
    return 0;
}

int History::rec_test2(HEvent* event, vector<Dynamics>& ds) {
    //cout << "testing " << *event << endl;
    int cnt = 0;
    set<Candidate> cs;
    while(cnt < 1000) {
        Candidate c = ds[cnt%SIZE(ds)].get_candidate();
        if (!c.is_valid()) continue;
        cs.insert(c);
        cnt++;
        HEvent* e = rec_see_event(c, event);
        bool is_good = is_original(e);
        delete e;
        if (is_good) return 100*SIZE(cs)/SIZE(event->atoms);
    }
    return 0;
}

vector<Candidate> History::rec_candidates(HEvent* event) {
    vector<Candidate> res;
    if (cherry_mode == KNOW_HOW) {
        Dynamics d0 = Dynamics(this, event);
        d0.compute_graph(2.,0.0,0.0);
        Candidate c = d0.get_candidate();
        if (c.is_valid()) res.push_back(c);
        return res;
    }

    Dynamics d0 = Dynamics(this, event);
    d0.compute_graph(1.,0.7,0.1);
    For(i, SIZE(event->atoms)) {
        Candidate c = d0.get_candidate();
        if (c.is_valid()) res.push_back(c);
    }
    d0.compute_graph(1.3,0.7,0.1);
    For(i, SIZE(event->atoms)) {
        Candidate c = d0.get_candidate();
        if (c.is_valid()) res.push_back(c);
    }
    d0.compute_graph(1.6,0.7,0.1);
    For(i, SIZE(event->atoms)) {
        Candidate c = d0.get_candidate();
        if (c.is_valid()) res.push_back(c);
    }
    d0.compute_graph(1.9,0.7,0.1);
    For(i, SIZE(event->atoms)) {
        Candidate c = d0.get_candidate();
        if (c.is_valid()) res.push_back(c);
    }
    return res;
}

double History::rec_score(const Candidate& c, HEvent* event) {
    return 1.0;
}


void History::proc_reconstruct(int number) {
    // only linear reconstruction implemented
    assert(SIZE(leaf_species)==1);
    // find last event
    HEvent* current = events.begin()->second;
    for(auto e : events) 
        if (current->event_time > e.second->event_time)
            current = e.second;
    double now_time = current->event_time;
    if (current->type != "") {
        current = new HEvent(current->species, gen_event_name(), "", current);
        events[current->name] = current;
        current->event_time = now_time -= 0.01;
    }
    // reconstruct
    while(number!=0 && !current->is_final()) {
        if (number>0) number--;
        rec_parent(current);
        assert(current->parent != nullptr);
        while(current->parent != nullptr) {
            current = current->parent;
            events[current->name] = current;
            current->event_time = now_time -= 0.01;
        }
    }
}

void History::proc_test(string stypes) {
    // only linear reconstruction implemented
    assert(SIZE(leaf_species)==1);
    assert(SIZE(events)==1);
    HEvent* current = events.begin()->second;
    cherry_mode = CHERRY_TREE;
    if (stypes.back() == 'o') cherry_mode = NO_CHERRY;
    if (stypes.back() == 'p') cherry_mode = KNOW_HOW;
    
    vector<Dynamics> dynam;
    map<char, double> M = {
        {'0', 1.}, 
        {'1', 1.1}, 
        {'2', 1.2}, 
        {'3', 1.3}, 
        {'4', 1.4}, 
        {'5', 1.5}, 
        {'6', 1.6},
        {'7', 1.7},
        {'8', 1.8},
        {'9', 1.9}
    };
    for(char c : stypes) if (c<'@') {
        dynam.push_back(Dynamics(this, current));
        dynam.back().compute_graph(M[c], 0.7, 0.1);
    }
    int tot_cnt = 1000;
    int ok_cnt = 0;
    int oke_cnt = 0;
    int bad_cnt = 0;
    For(i, tot_cnt) {
        int best = 0;
        for(auto ds : dynam) {
            int bad = 0;
            Candidate c = ds.get_candidate();
            while (!c.is_valid() && bad++ < 100)
                c = ds.get_candidate();
            if (!c.is_valid()) {
                bad_cnt++;
                break;
            }
            HEvent *e = rec_see_event(c,current);
            best = max(best, is_original(e));
        }
        ok_cnt += bool(best);
        oke_cnt += best==SAME_LAST;
    }
    stats["etest " + stypes] = double(oke_cnt)/tot_cnt;
    stats["test " + stypes] = double(ok_cnt)/tot_cnt;
    stats["bad_cnt"] += bad_cnt;
}


