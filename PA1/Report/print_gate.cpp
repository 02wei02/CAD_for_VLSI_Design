string print_gate(PCKT c, int i, ofstream &outFile, int j, int k) {
    string result;  // String to hold the concatenated names
    
    if (c->nodes[k]->address == c->nodes[i]->fanins[j]) {
        if(c -> nodes[k] -> type == INPT)
            result += ", " + string(c->nodes[k]-> outname);
        else if (c->nodes[k]->type == FROM)
            result += ", " + string(c->nodes[k]-> outname);
        else if(c->nodes[k]->type == DRIVER)
            result += ", " + string(c->nodes[k]-> outname);

        return result;
    }
    // If the condition is not met, return an empty string
    return "";
}