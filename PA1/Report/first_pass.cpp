PCKT first_pass(ifstream& inFile, PCKT c){
    string s;
    int address, fanout, fanin;
    string name, type;
    int numOutput = 0, numInput = 0;
    int maxAddress = -1;
    int lineCount = 0;                  // column line cause by number of fanout

    while(getline(inFile, s)){
        istringstream iss(s);
        if(s[0] == '*') continue;
        iss >> address >> name >> type >> fanout >> fanin;
        lineCount += fanout;
        maxAddress = (address > maxAddress) ? address : maxAddress;
        if(fanin > 0) getline(inFile, s);
        if(fanout > 1){
            while(fanout-- > 0){
                getline(inFile, s);
            }
        }
        if(fanout == 0) numOutput++;
        if(fanin == 0)  numInput++;
    }

    c -> numOutput = numOutput;
    c -> numInput = numInput;
    c -> nodes = (PNODE *) malloc ((maxAddress + lineCount + 8) * sizeof(PNODE));
    cout << "LineCount: " << lineCount << endl;
    cout << "maxAddress: " << maxAddress << endl << endl;
 
    return c;
}