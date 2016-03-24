#include"constants.h"
#include"history.h"

int main(int argc, char **argv) {
    setup_constants();
    
    History* history = new History();
    history->generate_random(0.025, 100);
}
