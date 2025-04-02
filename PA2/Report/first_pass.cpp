void Partitioning::FirstPass(ifstream &inFile)
{
    string s;
    string idle;

    string net;
    while (getline(inFile, s))
    {
        istringstream iss(s);
        iss >> idle >> net;

        int netc = stoi(net.substr(1, net.size() - 1));
        NetCount(netc);

        set<int> temp;
        while (iss >> idle)
        {
            if (idle == "{")
                continue;
            if (idle == "}")
                break; // if '}' break this line
            int idlec = stoi(idle.substr(1, idle.size() - 1));
            CellCount(idlec);

            temp.insert(idlec);
        }
        circuit->nets[netc] = temp;
    }
}