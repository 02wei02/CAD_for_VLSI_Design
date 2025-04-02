bool Partitioning::IsConstraint1(AB now_ABnet)
{
    if (now_ABnet->cells.size() < 2)
    {
        return false;
    }

    if (now_ABnet->cells[0].empty() || now_ABnet->cells[1].empty())
    {
        return false;
    }
    else
    {

        int countA = now_ABnet->cells[0].size();
        int countB = now_ABnet->cells[1].size();
        if (double(abs(countA - countB)) <= double(circuit->cell_count) / 5)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}