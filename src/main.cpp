#include"constants.h"
#include"ghistory.h"

int main(int argc, char **argv) {
    setup_constants();
    cout << "Program started" << endl;
    
    GHistory* history = new GHistory();
    history->generate_random(0.03, 100000);
    history->save_to_files(DATAPATH "generated", "01");

    History* h0 = new History(DATAPATH "generated", "01");
    h0->write_events(cout);
}
