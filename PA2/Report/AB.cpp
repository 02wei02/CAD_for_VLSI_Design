void Partitioning::ABMerge(AB next_ABnet)
{
    if (!next_ABnet->cells.empty())
    {
        if (next_ABnet->cells.size() > 2)
        {
            for (int i = 2; i < next_ABnet->cells.size(); i++)
            {
                if (next_ABnet->cells[0].size() < next_ABnet->cells[1].size())
                {
                    for (int cell : next_ABnet->cells[i])
                        next_ABnet->cells[0].insert(cell);
                }
                else
                {
                    for (int cell : next_ABnet->cells[i])
                        next_ABnet->cells[1].insert(cell);
                }
                next_ABnet->cells[i].clear();
            }
        }
    }
    for (int i = 1; i <= circuit->cell_count; i++)
    {
        set<int> cell;
        cell.clear();
        cell.insert(i);
        if (!IsFindABnet(next_ABnet, i))
        {
            if (next_ABnet->cells.size() < 2)
                next_ABnet->cells.push_back(cell);
            else if (next_ABnet->cells[0].size() < next_ABnet->cells[1].size())
                next_ABnet->cells[0].insert(i);
            else
                next_ABnet->cells[1].insert(i);
        }
    }
}