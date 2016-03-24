#include"history.h"
#include <iomanip>

void History::generate_random(double time, int sequence_length) {
    for(auto s : sequences) delete s;
    sequences.clear();
    AtomType::reset_ids(); 
    
    sequences.push_back(new Sequence(0.0, sequence_length));

    cout << std::fixed << std::setprecision(6);
    cout << *sequences.back();

    double current_time = 0.0;
    while(current_time < time) {
        Sequence* seq = sequences.back();
        Event* event = Model::instance()->get_random_event(seq->length());
        if (event->get_length() == 0) {
            double time = event->get_time(0);
            delete event;
            event = new NoEvent(time);
        }
        current_time = event->get_time(current_time);
        cout << *event << endl;
        
        Sequence* nextseq = event->perform(seq);
        delete event;
        sequences.push_back(nextseq);

        for (auto s : sequences)
            cout << *s;
        cout << endl;

    }
}
