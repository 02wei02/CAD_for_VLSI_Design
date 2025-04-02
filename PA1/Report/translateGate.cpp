GATENAME translateGate(string type){
    if(type == "inpt")
        return INPT;
    if(type == "and")
        return AND;
    if(type == "nand")
        return NAND;
    if(type == "or")
        return OR;
    if(type == "nor")
        return NOR;
    if(type == "xor")
        return XOR;
    if(type == "xnor")
        return XNOR;
    if(type == "buff")
        return BUFF;
    if(type == "not")
        return NOT;
    if(type == "from")
        return FROM;
    return DRIVER;
}