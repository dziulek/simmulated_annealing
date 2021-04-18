#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <limits>
#include <string>
#include <iostream>
#include <unistd.h>
#include <pthread.h>

const float eps = 10e-9;
const float infinity = std::numeric_limits<float>::infinity();
const int LATENCY_MICROSECONDS = 10000;

const int N_RANDOM_ITERATIONS = 20;

extern bool GRAPHICAL_INTERFACE;
extern pthread_mutex_t annealing_operation_mutex;

#endif