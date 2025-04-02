PCKT second_pass(ifstream& inFile, PCKT c){

    string s;
    int address, fanout, fanin;
    char name[256], type[256];
    int col = 0;

    while(getline(inFile, s)){
        istringstream iss(s);
        if(s[0] == '*') continue;
        iss >> address >> name >> type >> fanout >> fanin;
        cout << address << " " << name << " " << type << " " << fanout << " " << fanin << endl;

        stringstream add;
            add << address;

            string fullname = "gat" + add.str();

            c->nodes[col] = (NODE *)malloc(sizeof(NODE));
            c->nodes[col]->address = address;
            c->nodes[col]->name = strdup(name);
            c->nodes[col]->type = INPT;
            c->nodes[col]->numFanin = fanin;
            c->nodes[col]->fanins = (int *)malloc(fanin * sizeof(int));
            c->nodes[col]->func = strdup(type);
            if(strcmp(type, "buff") == 0)
                c->nodes[col]->func = strdup("buf");
            c->nodes[col]-> outname = strdup(fullname.c_str());

            cout << c->nodes[col]->address << " " << c->nodes[col]->name << " " << c->nodes[col]->type << " " << c->nodes[col]->numFanin <<" " <<  c -> nodes[col] -> func <<endl;

            if (fanin > 0)
            {
                getline(inFile, s);
                istringstream iss(s);
                for (int i = 0; i < fanin; i++)
                {
                    iss >> c -> nodes[col] -> fanins[i]; // nodes drive the gate
                }
            }

        if(fanin != 0){
            c -> nodes[col] -> type = DRIVER;
            fullname = "gat_out" + add.str();
            c->nodes[col]-> outname = strdup(fullname.c_str());

            if(fanout == 0)
                c -> nodes[col] -> type = OUTPT;   
        }

        col++;
        
        char fan[256];
        if(fanout > 1)
        {
            for(int i = 0; i < fanout; i++){
                 getline(inFile, s);
                istringstream iss(s);
                iss >> address >> fan;
                c -> nodes[col] = (NODE *) malloc (sizeof(NODE));
                c -> nodes[col] -> address = address;
                c -> nodes[col] -> name = strdup(name);
                c -> nodes[col] -> type = FROM;
                c -> nodes[col] ->numFanin = 1;
                c -> nodes[col] -> fanins = (int *) malloc (sizeof(int));
                c -> nodes[col] -> fanins[0] = address;
                c -> nodes[col] -> func = strdup(type);   /// reading type from file 
                c->nodes[col]-> outname = strdup(fullname.c_str());
                cout << "Name fanout: " << c->nodes[col]-> outname << endl;

                col++;
            }
        }

        c-> numNode = col;

        cout << "Number of col: " << c-> numNode << endl << endl; 
    }
    return c;
}