double Constraint::Cost(double w, double h, double HPWL, const double &ratio) // cost function
{
    double cost;
    cost = (0.25 * w * h + 0.75 * HPWL) * (1 + (AspectRatio(w, h) - ratio / 3) * (AspectRatio(w, h) - ratio / 3));
    return cost;
}