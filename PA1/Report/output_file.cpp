void output_file(char *vFile, PCKT c){

    ofstream outFile(vFile);

    if(!outFile){
        cout << "v can't be generate.";
    }
    
    outFile << "`timescale 1ns/1ps\n";

    bool notFirst = 0;

    // module 
    outFile << "module " << string(c->name).substr(0, string(c->name).length() - 4) << " (";
    for(int i = 0; i < c-> numNode; i++){
        if(c -> nodes[i] -> type == INPT || c -> nodes[i] -> type == OUTPT){
            if(notFirst)
                outFile << ", ";
            outFile << c -> nodes[i] -> outname;
            notFirst = 1;
        }
    }
    outFile << ");\n";
    
    // input
    notFirst = 0;
    outFile << "input ";
    for(int i = 0; i < c->numNode; i++){
        if(c -> nodes[i] -> type == INPT){
            if(notFirst)
                outFile << ", ";
            outFile << c -> nodes[i] -> outname;
            notFirst = 1;
        }
    }
    outFile << ";\n";

    // outputs
    notFirst = 0;
    outFile << "output ";
    for(int i = 0; i < c->numNode; i++){
        if(c -> nodes[i] -> type == OUTPT){
            if(notFirst)
                outFile << ", ";
            outFile << c -> nodes[i] -> outname;
            notFirst = 1;
        }
    }
    outFile << ";\n";

    // wire
    if(c -> numDriver > 0){
        notFirst = 0;
        outFile << "wire ";
        notFirst = 0;
        for(int i = 0; i < c -> numNode; i++){

            if(c -> nodes[i] -> type == DRIVER){
                if(notFirst)
                    outFile << ", ";
                outFile << c -> nodes[i] -> outname;
                notFirst = 1;
            }
        }
            outFile << ";\n\n";
    }

    outFile << "\n";

    for(int i = 0; i < c -> numNode; i++){
        if(c -> nodes[i] -> type != INPT  
        && c -> nodes[i] -> type != FROM){
            outFile << c -> nodes[i] -> func << " ";
            outFile << c -> nodes[i] -> func << c -> nodes[i] -> address << " ( "; 
            outFile << c -> nodes[i] -> outname ;
            for(int j = 0; j < c -> nodes[i] -> numFanin; j++)
            {
                int k = 0;
                while(k < c -> numNode){
                    if (c->nodes[k]->address == c->nodes[i]->fanins[j]) {
                            outFile << ", " << c->nodes[k]-> outname;
                    }
                    k++;
                }
            }
            outFile << " );\n";
        }
    }
    outFile << "\nendmodule\n";
}