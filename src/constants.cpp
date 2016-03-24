#include"constants.h"

char bases[BASES] = {'A', 'C', 'G', 'T'};
int base_id[256];
char base_inv[256];
int debuging = 0;

void setup_constants() {
    random_init();
    For(i, BASES) {
        base_id[(int)bases[i]] = i;
        base_inv[(int)bases[i]] = bases[BASES-1-i];
    }
}
