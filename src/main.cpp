#include"constants.h"
#include"history.h"

int main(int argc, char **argv) {
    setup_constants();
    
    History* history = new History();
    For(i, 100)
        history->generate_random(0.08, 100000);
    history->generate_random(0.020, 1000);
    history->save_to_files(DATAPATH "generated", "01");
}
