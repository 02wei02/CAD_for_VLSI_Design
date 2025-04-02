bool Partitioning::Metropolis(double t, int cost)
{
    double r = static_cast<double>(rand()) / (RAND_MAX + 1.0);
    return (exp(-double(cost) / t) > r);
}