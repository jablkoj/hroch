#include"constants.h"
#include"history.h"

int main(int argc, char **argv) {
    setup_constants();
    cout << "Program started" << endl;
    
    History* history = new History();
    history->generate_random(0.03, 100000);
    history->save_to_files(DATAPATH "generated", "01");
}
