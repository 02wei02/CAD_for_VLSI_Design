void Partitioning::ShrinkCut(AB now_ABnet)
{
    auto iter = now_ABnet->cut.begin();
    while (iter != now_ABnet->cut.end())
    {
        if (IsRedundantCut(circuit->nets[*iter], now_ABnet->cells))
        {
            now_ABnet->cut_size--;
            iter = now_ABnet->cut.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

bool Partitioning::IsRedundantCut(set<int> net, vector<set<int>> cells)
{
    int countA = 0;
    int countB = 0;
    for (auto &i : net)
    {
        if (!cells[0].count(i)) // can't find in A
            countA++;
        else
            countB++;
    }
    if (countA == 0 || countB == 0)
        return true;
    else
        return false;
}