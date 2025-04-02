#ifndef CONSTRAINT_H
#define CONSTRAINT_H
#include <iostream>
#include <math.h>
using namespace std;
class Constraint
{
public:
    double AspectRatio(double w, double h);                                // aspect ratio
    double Cost(double area, double HPWL, double AR, const double &ratio); // cost function
    bool TimeLimited(const int &times, time_t start, time_t end);       // time limited and need to end the programs
    bool pick(const double &cost, const double &temperature);              // if cost <= 0 choose pick?
    double ratio;
};
#endif