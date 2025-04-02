#include "constraint.h"
double Constraint::AspectRatio(double w, double h) // aspect ratio
{
    double r1 = w / h;
    double r2 = h / w;
    double ans;
    if (r1 > r2)
        ans = r1;
    else
        ans = r2;
    return ans;
}
double Constraint::Cost(double w, double h, double HPWL, const double &ratio) // cost function
{
    double cost;
    cost = (0.25 * w * h + 0.75 * HPWL) * (1 + (AspectRatio(w, h) - ratio / 3) * (AspectRatio(w, h) - ratio / 3));
    return cost;
}
bool Constraint::TimeLimited(const int &times, time_t start, time_t end) // time limited and need to end the program
{
    time(&(end));
    if (times > 100 || (double(end) - double(start)) > 500)
    {
        cout << "time: " << (double(end) - double(start)) << endl;
        return true;
    }
    else
        return false;
}
bool Constraint::pick(const double &cost, const double &temperature)
{
    if (cost <= 0)
        return true;
    else if (rand() % 100 < exp(-1 * cost / temperature) * 100)
        return true;
    else
        return false;
}