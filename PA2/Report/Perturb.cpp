void Partitioning::Perturb(AB now_ABnet, AB next_ABnet)
{
    *next_ABnet = *now_ABnet;

    int chooseonecut = rand() % (circuit->net_count) + 1;

    if (ABnet->cut_size != 0)
    {
        int i = 0;
        while (i < chooseonecut)
        {
            int r = rand() % (circuit->net_count) + 1;
            if (i <= ABnet->cut_size * 0.1)
            {
                if (!next_ABnet->cut.count(r))
                {
                    next_ABnet->cut.insert(r);
                }
                else
                {
                    next_ABnet->cut.erase(r);
                }
                StartPartition(next_ABnet);
                if (!IsConstraint1(next_ABnet))
                {
                    if (next_ABnet->cut.count(r))
                        next_ABnet->cut.erase(r);
                    else
                        next_ABnet->cut.insert(r);
                }
                StartPartition(next_ABnet);
            }
            i++;
        }
    }
    else
    {
        int i = 1;
        while (1)
        {
            int r = rand() % (circuit->net_count) + 1;
            next_ABnet->cut.insert(r); // Mark it as cut
            StartPartition(next_ABnet);
            if (next_ABnet->cells.size() >= 2 && IsConstraint1(next_ABnet))
                break;
            i++;
        }
    }
    StartPartition(next_ABnet);
    ShrinkCut(next_ABnet);
    next_ABnet->cut_size = CalCutSize(next_ABnet);
}