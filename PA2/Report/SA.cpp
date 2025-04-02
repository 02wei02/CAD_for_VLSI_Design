void Partitioning::SimulatedAnnealing()
{
    double ti = 3675, tend = 1;
    double t = ti;
    auto start_time = std::chrono::steady_clock::now();

    // ABnet is not empty
    if (ABnet == nullptr)
    {
        std::cout << "ABnet pointer is null." << endl;
        return;
    }
    AB now_ABnet = new ab();
    AB next_ABnet = new ab();
    ABnet->cut_size = 0;
    Initial(now_ABnet, now_ABnet); // Initialize the partition assignment

    do
    {
        do
        {
            auto end_time = std::chrono::steady_clock::now();
            auto elapsed_time = std::chrono::duration_cast<std::chrono::minutes>(end_time - start_time).count();
            if (elapsed_time >= 9)
            {
                std::cout << "Time limit exceeded. Terminating program." << std::endl;
                break;
            }
            Perturb(now_ABnet, next_ABnet);

            if (Cost(next_ABnet) <= Cost(now_ABnet) && IsConstraint1(now_ABnet))
            {
                *now_ABnet = *next_ABnet;
                if (Cost(now_ABnet) <= Cost(ABnet) && IsConstraint1(now_ABnet))
                {
                    *ABnet = *now_ABnet;
                }
            }
            else if (Metropolis(t, Cost(next_ABnet) - Cost(now_ABnet)))
            {
                *now_ABnet = *next_ABnet;
            }
        } while (!IsConstraint1(ABnet));
        t = 0.9 * t;
    } while ((t > tend));

    delete now_ABnet;
    delete next_ABnet;
}