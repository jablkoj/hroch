#ifndef RANDOM_H
#define RANDOM_H

#include"constants.h"

void random_init();
double random_double(); // nahodne [0.0, 1.0)
double random_double(double from, double to); // nahodne [from, to)
int random_int(int from, int to); // nahodne [from, to)

class Model {
private:
    geometric_distribution<int> length_distribution;
    geometric_distribution<int> distance_distribution;

    const double prob_del = 0.05;
    const double prob_inv = 0.39;
    const int mean_len = 30;//14307;
    const int mean_dist = 200;//306718;
    const double event_rate = 200;
    const double mut_alpha = 1./3.;
    
    static Model* _instance;
    Model();
public:
    static Model* instance();
    
    bool is_random_del();
    bool is_random_inv();
    int get_random_len();
    int get_random_dist();
    double get_random_time();
    char get_mutated_base(char base, double time);

    Event* get_random_event(int sequence_length);
};

#endif